/**
 *I2C Hardware Module
 * @author Curt Henrichs
 * @date 3-23-17
 * I2C module is used as a network connection to the Camera's settings.
 */

//=============================================================================
//                                 Libraries
//=============================================================================

#include "I2C.h"

//=============================================================================
//                        Private Function Declaration
//=============================================================================

/**
 * Needed by every I2C transfer operation. It sets the command needed by the
 * particular I2C transfer, polls TIP for end of transfer and returns the
 * RxACK status (0 for success).
 * @param command bit flags specifying the commands to apply to I2C control
 * @return success/fail of command (ACK = 1, NACK = 0)
 */
uint8_t I2C_action(uint8_t command);

//=============================================================================
//                         Public Function Implementation
//=============================================================================

/**
 * This function will initialize the pre-scaler register and enable the I2C
 * peripheral
 */
void I2C_init(){
	uint16_t scaler = I2C_PRESCALER_FORMULA(50000000,100000);
	I2C->PRESCALER_LO = (scaler)&0xFF;
	I2C->PRESCALER_HI = (scaler>>8)&0xFF;
	I2C->CNTRL = I2C_CNTRL_ENABLE_MASK;
}

/**
 * This function is used to read the contents of any of the camera's registers.
 * It returns the 8-bit contents of the specified register.
 * @param addr slave address of device 7-bit
 * @param regNumber register address to read on slave device.
 * @return byte read from the device.
 */
uint8_t I2C_read(uint8_t addr, uint8_t regNumber){
	uint8_t nacked;

	//write slave address
	I2C->RX_TX = addr << 1;
	nacked = I2C_action(I2C_CMD_STA_MASK | I2C_CMD_WR_MASK);
	if(nacked){
		return -1; //error occurred
	}

	//write register address and end transmit
	I2C->RX_TX = regNumber;
	nacked = I2C_action(I2C_CMD_WR_MASK| I2C_CMD_STO_MASK);
	if(nacked){
		return -1; //error occurred
	}

	//write slave read address, data received is from device
	I2C->RX_TX = (addr << 1) | 1;
	nacked = I2C_action(I2C_CMD_STA_MASK | I2C_CMD_WR_MASK);
	if(nacked){
		return -1; //error occurred
	}
	nacked = I2C_action(I2C_CMD_RD_MASK | I2C_CMD_STO_MASK | I2C_CMD_ACK_MASK);
	return I2C->RX_TX;

}

/**
 * This function takes the value passed in and writes it to the specified
 * register. It returns nothing.
 * @param addr slave address of device 7-bit
 * @param regNumber register address to write on slave device.
 * @param value data byte to send to slave register.
 * @return 1 if nacked, 0 if acked
 */
uint8_t I2C_write(uint8_t addr, uint8_t regNumber, uint8_t value){
	uint8_t nacked;

	//write slave address
	I2C->RX_TX = addr << 1;
	nacked = I2C_action(I2C_CMD_STA_MASK | I2C_CMD_WR_MASK);
	if(nacked){
		return nacked; //error occurred
	}

	//write register address
	I2C->RX_TX = regNumber;
	nacked = I2C_action(I2C_CMD_WR_MASK);
	if(nacked){
		return nacked; //error occurred
	}

	//write data
	I2C->RX_TX = value;
	nacked = I2C_action(I2C_CMD_WR_MASK | I2C_CMD_STO_MASK);
	return nacked;
}

//=============================================================================
//                          Private Function Implementation
//=============================================================================

/**
 * Needed by every I2C transfer operation. It sets the command needed by the
 * particular I2C transfer, polls TIP for end of transfer and returns the
 * RxACK status (0 for success).
 * @param command bit flags specifying the commands to apply to I2C control
 * @return success/fail of command (NACK != 0, ACK = 0)
 */
uint8_t I2C_action(uint8_t command){
	I2C->STATUS_CMD = command;
	while(I2C->STATUS_CMD&I2C_STATUS_TIP_FLAG); //wait for TIP
	return I2C->STATUS_CMD&I2C_STATUS_RXACK_FLAG;
}
