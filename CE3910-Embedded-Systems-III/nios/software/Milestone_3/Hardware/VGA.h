/**
 * VGA Module
 * @author Curt Henrichs
 * @date 3-31-17
 * VGA display hardware interface, implements clear functionality and the
 * address pointers. All application code must manipulate directly for fast
 * response by the system.
 */

#ifndef VGA_H_
#define VGA_H_

//=============================================================================
//                                  Libraries
//=============================================================================

#include <stdint.h>
#include <system.h>
#include "GenericIO.h"

//=============================================================================
//						 Constant and Macro Declaration
//=============================================================================

#define VGA_ROW_MAX 60
#define VGA_COL_MAX 80

#define VGA_MEM_START (uint8_t volatile*)(ONCHIP_MEMORY2_0_BASE | BYPASS_CACHE_FLAG)
#define VGA_MEM_END  (VGA_MEM_START + ((VGA_ROW_MAX-1) << 7) + (VGA_COL_MAX-1))

//=============================================================================
//                         Public Function Declaration
//=============================================================================

/**
 * Clears the VGA device by making the screen black on all pixels.
 */
void vga_clearDisplay();

#endif /* VGA_H_ */
