/**
 *I2C Hardware Module
 * @author Curt Henrichs
 * @date 3-23-17
 * I2C module is used as a network connection to the Camera's settings.
 */

#ifndef I2C_H_
#define I2C_H_

//=============================================================================
//                                 Libraries
//=============================================================================

#include "GenericIO.h"
#include <stdint.h>
#include <system.h>

//=============================================================================
//                             I2C Register Structure
//=============================================================================

/**
 * Hardware Register set for I2C device
 */
struct I2C_HARDWARE{
	uint8_t PRESCALER_LO;
	uint8_t PRESCALER_HI;
	uint8_t CNTRL;
	uint8_t RX_TX;
	uint8_t STATUS_CMD;
};

//=============================================================================
//						 Constant and Macro Declaration
//=============================================================================
/**
 * Hardware register pointer for NIOS image
 */
#define I2C ((struct I2C_HARDWARE volatile *)(OC_I2C_MASTER_TOP_0_BASE | BYPASS_CACHE_FLAG))
/**
 * Calculates the desired bit-rate from the NIOS clock frequency
 * @param clock NIOS base clock
 * @param desiredRate rate of the I2C bus
 * @return bit-value for prescaler
 */
#define I2C_PRESCALER_FORMULA(clock,desiredRate) ((clock)/(5*desiredRate)-1)

// I2C Hardware Enable Commands
#define I2C_CNTRL_ENABLE_MASK (1<<7)
#define I2C_CNTRL_IENABLE_MASK (1<<6)

// I2C Hardware Commands
#define I2C_CMD_STA_MASK (1<<7)
#define I2C_CMD_STO_MASK (1<<6)
#define I2C_CMD_RD_MASK (1<<5)
#define I2C_CMD_WR_MASK (1<<4)
#define I2C_CMD_ACK_MASK (1<<3)
#define I2C_CMD_IACK_MASK (1<<0)

// I2C Hardware Flags
#define I2C_STATUS_RXACK_FLAG (1<<7)
#define I2C_STATUS_BUSY_FLAG (1<<6)
#define I2C_STATUS_TIP_FLAG (1<<1)
#define I2C_STATUS_IF_FLAG (1<<0)

//=============================================================================
//                         Public Function Declaration
//=============================================================================

/**
 * This function will initialize the pre-scaler register and enable the I2C
 * peripheral
 */
void I2C_init();
/**
 * This function is used to read the contents of any of the camera's registers.
 * It returns the 8-bit contents of the specified register.
 * @param addr slave address of device 7-bit
 * @param regNumber register address to read on slave device.
 * @return byte read from the device.
 */
uint8_t I2C_read(uint8_t addr, uint8_t regNumber);
/**
 * This function takes the value passed in and writes it to the specified
 * register. It returns nothing.
 * @param addr slave address of device 7-bit
 * @param regNumber register address to write on slave device.
 * @param value data byte to send to slave register.
 * @return 1 if nacked, 0 if acked
 */
uint8_t I2C_write(uint8_t addr, uint8_t regNumber,uint8_t value);


#endif /* I2C_H_ */
