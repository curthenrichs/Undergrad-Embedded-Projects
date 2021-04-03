/**
 * TIMER Module
 * @author Curt Henrichs
 * @date 1-24-16, revised 3-10-17
 *
 * Implements the API for the hardware timers for both interrupt and
 * non-interrupt based timing. Note: Timer works in nanoseconds.
 */

//=============================================================================
//						           Libraries
//=============================================================================

#include "Timer.h"

#include <stdlib.h>

//=============================================================================
//							  Function Implementation
//=============================================================================

/**
 * Delay for the time passed into the timer as a polling loop.
 * @param timeToDelay nanoseconds to delay
 * @param timer Pointer to TIMER hardware.
 */
void timer_delay(uint32_t timeToDelay, struct TIMER_HARDWARE volatile * timer){

	//Convert from seconds to clock cycles
	timeToDelay = timeToDelay / TIME_PER_CYCLE;

	//Set initial state of the timer needs both since they are 16 bit registers
	(timer -> PERIODL) = timeToDelay;
	(timer -> PERIODH) = timeToDelay >> 16;

	// Clear t0 flag to reset timer if it was set before for whatever reason
	(timer -> STATUS) = TIMER_STATUS_T0_MASK;

	//Start timer count down by setting start bit
	uint8_t timerStatus = START_TIMER_MASK;
	(timer -> CTRL) = timerStatus;

	//Loop while t0 flag is a zero
	while(((timer -> STATUS) & TIMER_STATUS_T0_MASK) == 0)
		;

	// Clear t0 flag to reset timer
	(timer -> STATUS) = TIMER_STATUS_T0_MASK;
}

/**
 * Sets the timer to be interrupt enabled
 * @param isInterruptable 0 is disabling interrupts, 1 is enabling interrupts
 * @param timeToDelay Time to set in timer before interrupt once started.
 * @param timer Pointer to Timer hardware
 */
void timer_interrupt_init(uint8_t isInterruptable, uint32_t timeToDelay,
		struct TIMER_HARDWARE volatile * timer){

	//check to see if enabling or disabling interrupts
	if(isInterruptable){

		// set IRQ0
		if(timer == TIMER_0){
			// set interrupt ienable
			asm volatile(
							".equ irq0, 0\n\t"			// interrupt mask
							"movi r7, (1<<irq0)\n\t"
							"rdctl  r6, ienable\n\t"	// combine previous
							"or		r7, r7, r6\n\t"     // ienable and irq0
							"wrctl	ienable, r7\n\t"
							::
						);

		}
		// set IRQ2
		else if (timer == TIMER_1){
			// set interrupt ienable
			asm volatile(
							".equ irq2, 2\n\t"			// interrupt mask
							"movi r7, (1<<irq2)\n\t"
							"rdctl  r6, ienable\n\t"	// combine previous
							"or		r7, r7, r6\n\t"     // ienable and irq2
							"wrctl	ienable, r7\n\t"
							::
						);
		}

		//Clear t0 flag to reset timer if it was
		// set before for whatever reason
		(timer -> STATUS) = TIMER_STATUS_T0_MASK;

		//Convert from seconds to clock cycles
		timeToDelay = timeToDelay / TIME_PER_CYCLE;

		//Set initial state of the timer needs both since
		// they are 16 bit registers
		(timer -> PERIODL) = timeToDelay;
		(timer -> PERIODH) = timeToDelay >> 16;

		// set interrupt on control register
		(timer -> CTRL) = (timer -> CTRL) | (INTERRUPT_TIMER_MASK | CONT_TIMER_MASK);

	} else {
		// set IRQ to off
		if(timer == TIMER_0){
			asm volatile(
							".equ irq0, 0\n\t"			// interrupt mask
							"movi	r7, (~(1<<irq0))\n\t"
							"rdctl	r6, ienable\n\t"	// combine previous
							"and	r7, r7, r6\n\t"     // ienable and irq0
							"wrctl	ienable, r7"
							::
						);
		}else if(timer == TIMER_1){
			asm volatile(
							".equ irq0, 0\n\t"			// interrupt mask
							"movi	r7, (~(1<<irq2))\n\t"
							"rdctl	r6, ienable\n\t"	// combine previous
							"and	r7, r7, r6\n\t"     // ienable and irq2
							"wrctl	ienable, r7"
							::
						);
		}

		// disable interrupt
		(timer -> CTRL) = (timer -> CTRL) & !(INTERRUPT_TIMER_MASK | CONT_TIMER_MASK);
	}
}

/**
 * Sets the timer to be started or stopped for interrupt use.
 * @param isStart boolean value whether to start of stop timer
 * @param timer Pointer to Timer hardware
 */
void timer_interrupt_start_stop(uint8_t isStart,
		struct TIMER_HARDWARE volatile * timer){
	//Start the timer
	if(isStart){
		(timer -> CTRL) = (timer -> CTRL) | START_TIMER_MASK;
	}
	//Stop the timer
	else {
		(timer -> CTRL) = (timer -> CTRL) & !START_TIMER_MASK;
	}
}

//=============================================================================
//								  Generic ISR
//=============================================================================

/**
 * Handles the interrupt for the given timer in name. Note that this clears the
 * interrupt but does not perform a callback. Instead template this function or
 * call from actual ISR.
 */
void timer_ISR_TIMER_0(){
	//write T0 flag to clear interrupt
	(TIMER_0 -> STATUS) = TIMER_STATUS_T0_MASK;
}

/**
 * Handles the interrupt for the given timer in name. Note that this clears the
 * interrupt but does not perform a callback. Instead template this function or
 * call from actual ISR.
 */
void timer_ISR_TIMER_1(){
	//write T0 flag to clear interrupt
	(TIMER_1 -> STATUS) = TIMER_STATUS_T0_MASK;
}
