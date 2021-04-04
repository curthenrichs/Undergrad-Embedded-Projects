/**
 * GENERIC_IO Module
 * @author Curt Henrichs
 * @date 1-24-16, revised 3-10-17
 *
 * Defines a structure that is the general purpose parallel IO device. Should
 * be used for all specific devices that fit the register pattern for a
 * parallel IO device.
 */

#ifndef GEN_IO_LIB_H_
#define GEN_IO_LIB_H_

//=============================================================================
//						    Libraries Needed
//=============================================================================

#include <stdint.h>

//=============================================================================
//                          Constants and Macros
//=============================================================================

/**
 * Defines the Flag placed on an address to notify the program to bypass
 * the cache and directly update memory.
 */
#define BYPASS_CACHE_FLAG 0x80000000

//=============================================================================
//							  Structure Declaration
//=============================================================================

/**
 * Generic Structure of an IO device for NIOS.
 */
struct GEN_IO{
	uint32_t DATA;
	uint32_t DIRECTION;
	uint32_t INTERRUPT;
	uint32_t EDGE_CAPTURE;
};

#endif
