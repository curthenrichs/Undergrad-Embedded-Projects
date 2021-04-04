/**
 * UART Module
 * @author Prof. Barnekow (Original), Curt Henrichs (Modified)
 * @date 1-17-16, revised 3-10-17
 *
 * Defines the basic UART API to receive and transmit characters through the
 * device using polling method.
 */

#ifndef UART_H
#define UART_H

//=============================================================================
//						           Libraries
//=============================================================================

#include <stdint.h>
#include <system.h>
#include "GENERIC_IO.h"

//=============================================================================
//                         Data Structure Declaration
//=============================================================================

/**
 * Defines the UART register set for hardware interface.
 */
struct UART_Hardware{
	char UDR;
	uint8_t USTATUS;
	uint8_t UCONTROL;
};

//=============================================================================
//		                 Constants and Macro Declaration
//=============================================================================

//Bits in status register
#define TDRE 0x80
#define RDRF 0x40

// UART pointer
#define UART_0 ((struct UART_Hardware volatile *)(MYUART_0_BASE | BYPASS_CACHE_FLAG))

//=============================================================================
//							  Function Declaration
//=============================================================================

/**
 * Sets UART for no interrupts, sets baud rate to specified
 * @param baudRate the baud to transmit at
 * @param UART pointer to the hardware device to interface with
 * @return -1 if error, 0 otherwise
 */
int uart_init(uint16_t baudRate, struct UART_Hardware volatile * UART);

/**
 * Writes a character into the transmit register of UART. Caution: will be
 * stuck in a loop, for polling, until the transmit register is ready for next
 * byte.
 * @param x character to send out.
 * @param UART Hardware register pointer
 */
void uart_putch(char x, struct UART_Hardware volatile * UART);

/**
 * Gets a character that was received from the UART. Caution: WIll be stuck in
 * a loop, due to polling, until a character is received.
 * @param UART Hardware register pointer
 * @return character received by the UART in ASCII encoding.
 */
char uart_getch(struct UART_Hardware volatile * UART);

#endif
