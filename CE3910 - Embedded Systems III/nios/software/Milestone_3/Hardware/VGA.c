/**
 * VGA Module
 * @author Curt Henrichs
 * @date 3-31-17
 * VGA display hardware interface, implements clear functionality and the
 * address pointers. All application code must manipulate directly for fast
 * response by the system.
 */

//=============================================================================
//                                  Libraries
//=============================================================================

#include "VGA.h"

//=============================================================================
//                         Public Function Implementation
//=============================================================================

/**
 * Clears the VGA device by making the screen black on all pixels.
 */
void vga_clearDisplay(){
	uint8_t row,col;
	for(row=0;row<VGA_ROW_MAX;row++){
		for(col=0;col<VGA_COL_MAX;col++){
			*(VGA_MEM_START + (row << 7) + col) = 0x00;
		}
	}
}
