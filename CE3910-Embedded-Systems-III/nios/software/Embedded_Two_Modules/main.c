/**
 * Embedded 2 Module Driver Program
 * @author Curt Henrichs
 * @date 3-9-17
 * @version 0.1
 * @file main.c
 *
 * Driver program for the software interface modules developed in embedded 2.
 * Note that this code version will change the base address of the modules and
 * thus they are not compatible with the old NIOS image used. Also the NIOS
 * image used for this course (embedded 3) removes some of the hardware.
 * Modules that are not used on the new image will not be updated nor will
 * they be included in the final project. A list will be provided.
 *
 * Modules Supported:
 *   * GEN_IO_LIB
 *      -> GEN_IO_LIB.h
 *   * LCD_LIB
 *      -> LCD_LIB.h
 *      -> LCD_LIB.c
 *   * LED_LIB
 *      -> LED_LIB.h
 *      -> LED_LIB.c
 *   * PUSHBUTTON_LIB
 *      -> PUSHBUTTON_LIB.h
 *      -> PUSHBUTTON_LIB.c
 *   * PWM_LIB
 *      -> PWM_LIB.h
 *      -> PWM_LIB.c
 *   * TIMER_LIB
 *      -> TIMER_LIB.h
 *      -> TIMER_LIB.c
 *   * UART
 *      -> UART.h
 *      -> UART.c
 *   * INTERUPT_TABLE
 *      -> INTERUPT_TABLE.S
 *
 * Modules Unsupported:
 *   * ADC_LIB
 *       -> ADC_LIB.h
 *       -> ADC_LIB.c
 *   * DC_MOTOR_CONTROL_LIB
 *       -> DC_MOTOR_CONTROL_LIB.h
 *       -> DC_MOTOR_CONTROL_LIB.c
 *   * KEYPAD_LIB
 *       -> KEYPAD_LIB.h
 *       -> KEYPAD_LIB.c
 *   * SLIDER_SWITCHES_LIB
 *      -> SLIDER_SWITCHES_LIB.h
 *      -> SLIDER_SWITCHES_LIB.c
 *
 * In addition to changing the base address of the modules, some internal
 * changes to IRQs, number of internal peripherals, and code changes are made
 * to make the code work for the new image. Finally all comments are changed
 * to Doxygen format for more clarity and additional comments / modifications
 * will be made.
 */

//=============================================================================
//                               Libraries
//=============================================================================

#include <stdint.h>
#include <stdio.h>

#include "LED.h"
#include "TIMER.h"
#include "PUSHBUTTON.h"
#include "UART.h"
#include "PWM.h"
#include "LCD.h"

//=============================================================================
//                               ISR Functions
//=============================================================================

/**
 * Literally a noop function in case it ever gets called by an unhandled
 * interrupt. Used in this project to temporarily fill the vector table
 * between ISR switching
 */
extern void default_isr();

/**
 * Timer interrupt handler.
 */
void testTimerInterrupt(){
	LED_write(0xAAAA);
	timer_ISR_TIMER_1();
}

//=============================================================================
//                                   Main
//=============================================================================

/**
 * Main uses the alt_main form since interrupts are being used. Driver program
 * for all of the modules used in embedded 2 that are compatible with the
 * current NIOS image for embedded 3.
 * @return does not, Caught in the infinite loop.
 */
int alt_main(){

	// LED test
	LED_write(0xFFFF);

	// TIMER test
	timer_delay(SECOND_CONSTANT * 2,TIMER_0);
	LED_write(0x0000);
	timer_delay(SECOND_CONSTANT,TIMER_1);
	LED_write(0x5555);

	//start interrupts
	timer_interrupt_init(1,SECOND_CONSTANT,TIMER_1);
	timer_interrupt_start_stop(1,TIMER_1);
	asm volatile(
					"movia r7, default_isr\n\t"
					"movia r6, IRQ_0\n\t"
					"stw r7, 0(r6)\n\t"
					"movia r7, testTimerInterrupt\n\t"
					"movia r6, IRQ_2\n\t"
					"stw r7, 0(r6)\n\t"
					".equ PIE, 0\n\t"
					"movi	r7, (1 << PIE)\n\t"
					"wrctl	status, r7\n\t"
					::
				);

	timer_delay(SECOND_CONSTANT*2,TIMER_0);

	//disable interrupts
	asm volatile(
					"wrctl status, r0\n\t"
					"movia r7, default_isr\n\t"
					"movia r6, IRQ_0\n\t"
					"stw r7, 0(r6)\n\t"
					"movia r7, default_isr\n\t"
					"movia r6, IRQ_2\n\t"
					"stw r7, 0(r6)\n\t"
					::
				);
	timer_interrupt_start_stop(0, TIMER_1);

	LED_write(0x0000);

	// PUSHBUTTON test
	while(pushbutton_read(PUSHBUTTON_1_MASK | PUSHBUTTON_2_MASK)){
		//do nothing until condition met
	}
	LED_write(0x01);

	// UART test
	uart_init(9600,UART_0);

	char* str = "Hello World!\n";
	while(*str != '\0'){
		uart_putch(*str,UART_0);
		str++;
	}
	char c = uart_getch(UART_0);
	uart_putch(c,UART_0);

	// PWM test
	PWMwriteOCRA(50);
	PWMwriteOCRB(25);

	// LCD test
	lcd_portInit(0xff);
	lcd_init();
	lcd_printString("Hello World!\nAwesome.");

	while(1){
		/* Trap */
	}
	return 0;
}
