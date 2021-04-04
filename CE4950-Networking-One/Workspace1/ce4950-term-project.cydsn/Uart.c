/**
 * Uart
 * @author Curt Henrichs
 * @date 9-23-17
 *
 * USB CDC Virtual UART applicaiton interface module generalizes the hardware
 * API for the specific use cases of the application. The ability to read,
 * write, check if data is available, and handling of host configuration change
 * is abstracted by this module.
 */

//=============================================================================
//                                  Libraries
//=============================================================================

#include "Uart.h"
#include <project.h>

//=============================================================================
//                        Constant and Macro Defintions
//=============================================================================

#define USBFS_DEVICE        (0u)    //! Device number as defined in hardware
#define WAIT_TIMEOUT_COUNT  1000    //! Iterations of loop before timeout

//=============================================================================
//                         Public Function Implementation
//=============================================================================

/**
 * USB UART initailization function will start the virtual serial port
 */
void uart_init(void){
    USBUART_Start(USBFS_DEVICE,USBUART_5V_OPERATION);
}

/**
 * USB UART update function will handle configuration change from the USB host
 * @return on connection true else false
 */
bool uart_update(void){
    bool retval = false;
    //reconfiguration
    if(USBUART_IsConfigurationChanged()){
        if(USBUART_GetConfiguration()){
            USBUART_CDC_Init();
            retval = true;
        }
    }
    return retval;
}

/**
 * USB UART has been connected by host
 * @return true if USBUART is configured else false
 */
bool uart_has_connected(void){
    return USBUART_GetConfiguration() != 0u;
}

/**
 * USB UART received new data, needs to be taken from internal buffer
 * @return true if new data else false
 */
bool uart_has_data(void){
    return USBUART_GetConfiguration() ? USBUART_DataIsReady() : false;
}

/**
 * Get data from USB UART internal receive buffer.
 * @param str is pointer to buffer to write into
 * @param str_len is buffer size
 * @return number of bytes read from UART internal buffer into supplied
 */
uint16_t uart_read(char* str, uint16_t str_len){
    
    //parameter check
    if(str == NULL || str_len != USBUART_BUFFER_SIZE){
        return 0;   
    }
    
    //service CDC interface
    uint16_t count = 0;
    if(USBUART_GetConfiguration()){
        //input data from host
        if(USBUART_DataIsReady()){
            count = USBUART_GetAll((uint8_t*)(str));
        }
    }
    return count;
}

/**
 * Write data to USB UART internal transmit buffer.
 * @param str is pointer to buffer to read from
 * @param str_len is size of buffer to write out.
 * @return true of successfully wrote data, else false
 */
bool uart_write(const char* str, uint16_t str_len){
    bool retVal = false;
    
    //parameter check
    if(str == NULL || str_len > USBUART_BUFFER_SIZE){
        return false;
    }else if(str_len == 0){
        return true;
    }

    //send data
    if(USBUART_GetConfiguration()){       
        //wait for uart to be ready
        int16_t timeout_count = WAIT_TIMEOUT_COUNT;
        while(!USBUART_CDCIsReady() && timeout_count > 0){
            timeout_count--;
        }
        //if not timeout
        if(timeout_count > 0){
            retVal = true;
            USBUART_PutData((const uint8_t*)(str),str_len);
        }       
    } 
 
    return retVal;
}