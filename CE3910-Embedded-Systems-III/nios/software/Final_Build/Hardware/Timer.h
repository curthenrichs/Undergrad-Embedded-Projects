/**
 * TIMER Module
 * @author Curt Henrichs
 * @date 1-24-16, revised 3-10-17
 *
 * Defines the API for the hardware timers for both interrupt and non-interrupt
 * based timing. Note: Timer works in nanoseconds.
 */

#ifndef TIMER_LIB_H
#define TIMER_LIB_H

//=============================================================================
//						           Libraries
//=============================================================================

#include "GenericIO.h"

#include <stdint.h>
#include <system.h>

//=============================================================================
//							  Structure Declaration
//=============================================================================

/**
 * Defines the timer register hardware interface.
 */
struct TIMER_HARDWARE{
	uint32_t STATUS;
	uint32_t CTRL;
	uint32_t PERIODL;
	uint32_t PERIODH;
};

//=============================================================================
//							  Constant Declaration
//=============================================================================

// Timer setup constants
#define START_TIMER_MASK 0x04
#define CONT_TIMER_MASK 0x02
#define TIMER_STATUS_T0_MASK 0x01
#define INTERRUPT_TIMER_MASK 0x01
#define TIME_PER_CYCLE 20

// Time constant
#define SECOND_CONSTANT 1000000000
#define SCREEN_DELAY_TIME 2000000000
#define MICROSECOND_DELAY_50 50000

// Register Pointers
#define TIMER_0 ((struct TIMER_HARDWARE volatile *)(TIMER_0_BASE | BYPASS_CACHE_FLAG))
#define TIMER_1 ((struct TIMER_HARDWARE volatile *)(TIMER_1_BASE | BYPASS_CACHE_FLAG))

//=============================================================================
//							  Function Declaration
//=============================================================================

/**
 * Delay for the time passed into the timer as a polling loop.
 * @param timeToDelay nanoseconds to delay
 * @param timer Pointer to TIMER hardware.
 */
void timer_delay(uint32_t timeToDelay, struct TIMER_HARDWARE volatile * timer);

/**
 * Sets the timer to be interrupt enabled
 * @param isInterruptable 0 is disabling interrupts, 1 is enabling interrupts
 * @param timeToDelay Time to set in timer before interrupt once started.
 * @param timer Pointer to Timer hardware
 */
void timer_interrupt_init(uint8_t isInterruptable, uint32_t timeToDelay,
		struct TIMER_HARDWARE volatile * timer);

/**
 * Sets the timer to be started or stopped for interrupt use.
 * @param isStart boolean value whether to start of stop timer
 * @param timer Pointer to Timer hardware
 */
void timer_interrupt_start_stop(uint8_t isStart,
		struct TIMER_HARDWARE volatile * timer);

/**
 * Handles the interrupt for the given timer in name. Note that this clears the
 * interrupt but does not perform a callback. Instead template this function or
 * call from actual ISR.
 */
void timer_ISR_TIMER_0();

/**
 * Handles the interrupt for the given timer in name. Note that this clears the
 * interrupt but does not perform a callback. Instead template this function or
 * call from actual ISR.
 */
void timer_ISR_TIMER_1();

#endif
