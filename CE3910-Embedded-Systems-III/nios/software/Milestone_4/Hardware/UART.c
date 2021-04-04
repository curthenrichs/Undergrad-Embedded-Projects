/**
 * UART Module
 * @author Prof. Barnekow (Original), Curt Henrichs (Modified)
 * @date 1-17-16, revised 3-10-17
 *
 * Implements the basic UART API to receive and transmit characters through the
 * device using polling method.
 */

//=============================================================================
//						           Libraries
//=============================================================================

#include "UART.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//=============================================================================
//							  Function Implementation
//=============================================================================

/**
 * Sets UART for no interrupts, sets baud rate to specified
 * @param baudRate the baud to transmit at
 * @param UART pointer to the hardware device to interface with
 * @return -1 if error, 0 otherwise
 */
int uart_init(uint16_t baudRate, struct UART_Hardware volatile * UART){

	// select the baud input as hardware signals
	uint8_t baud;
	switch(baudRate){
		case 38400:
						baud = 0x0;
						break;
		case 19200:
						baud = 0x1;
						break;
		case 9600:
						baud = 0x2;
						break;
		case 4800:
						baud = 0x3;
						break;
		case 2400:
						baud = 0x4;
						break;
		case 1200:
						baud = 0x5;
						break;
		case 600:
						baud = 0x6;
						break;
		case 300:
						baud = 0x7;
						break;
		default:
						// there is an error since input is invalid
						return -1;

	}

	// No interrupts, 38500 Baud
	(UART->UCONTROL) = baud;

	return 0;
}

/**
 * Writes a character into the transmit register of UART. Caution: will be
 * stuck in a loop, for polling, until the transmit register is ready for next
 * byte.
 * @param x character to send out.
 * @param UART Hardware register pointer
 */
void uart_putch(char x, struct UART_Hardware volatile * UART){

	// Poll till char is received
	while(((UART -> USTATUS) & TDRE) == 0);

	// Write the character
	(UART -> UDR) = x;
}

/**
 * Gets a character that was received from the UART. Caution: WIll be stuck in
 * a loop, due to polling, until a character is received.
 * @param UART Hardware register pointer
 * @return character received by the UART in ASCII encoding.
 */
char uart_getch(struct UART_Hardware volatile * UART){

	// Temporary memory
	char x;

	//Wait till transmitter ready
	while(((UART -> USTATUS) & RDRF) == 0);

	// Read the character
	x = (UART -> UDR);
	return x;
}

/**
 * Print string to UART device.
 * @param str passes a string through the UART
 * @param UART Hardware register pointer
 */
void uart_printStr(char* str,struct UART_Hardware volatile * UART){
	if(str != NULL){
		return; //error NULL pointer
	}

	while(*str){
		uart_putch(*str,UART);
		str++;
	}
}

/**
 * Read string from UART device. Behaves like fgets().
 * @param inputStr String buffer that will be filled by the
 *        function until either a new line character is
 *        received or length is reached.
 * @param length size of the buffer.
 * @param UART Hardware register pointer
 */
void uart_readStr(char* inputStr, unsigned int length,struct UART_Hardware volatile * UART){
	unsigned int count;
	char c;
	while((c=uart_getch(UART)) != '\n' || count < length-1){
		count++;
		*inputStr = c;
		inputStr++;
	}
	*inputStr = '\0';
}
