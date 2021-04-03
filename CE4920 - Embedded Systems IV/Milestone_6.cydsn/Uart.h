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

#ifndef UART_H
#define UART_H
   
//=============================================================================
//                                  Libraries
//=============================================================================    
    
#include <stdint.h>
#include <stdbool.h>
 
//=============================================================================
//                        Constant and Macro Defintions
//=============================================================================

#define USBUART_BUFFER_SIZE (64u)   //! Maximum size of USB buffer for Rx, Tx

//=============================================================================
//                         Public Function Prototypes
//=============================================================================
 
/**
 * USB UART initailization function will start the virtual serial port
 */
void uart_init(void);
/**
 * USB UART update function will handle configuration change from the USB host
 */
void uart_update(void);
/**
 * USB UART received new data, needs to be taken from internal buffer
 * @return true if new data else false
 */
bool uart_has_data(void);
/**
 * Get data from USB UART internal receive buffer.
 * @param str is pointer to buffer to write into
 * @param str_len is buffer size
 * @return number of bytes read from UART internal buffer into supplied
 */
uint8_t uart_read(char* str, uint16_t str_len);
/**
 * Write data to USB UART internal transmit buffer.
 * @param str is pointer to buffer to read from
 * @param str_len is size of buffer to write out.
 * @return true of successfully wrote data, else false
 */
bool uart_write(const char* str, uint16_t str_len);
    
#endif
