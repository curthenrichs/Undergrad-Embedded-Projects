/**
 * LCD Module
 * @author Curt Henrichs
 * @date 1-4-16, revised 3-10-17
 *
 * Defines the API to control the LCD attached to the DE0. Note that errors
 * will occur in timing and missing characters if interrupts are active during
 * operation. Note that timer 1 is used for delay during output to the LCD.
 */

#ifndef LCD_LIB_H
#define LCD_LIB_H

//=============================================================================
//						           Libraries
//=============================================================================

#include "TIMER.h"
#include "GENERIC_IO.h"

#include <system.h>
#include <stdint.h>

//=============================================================================
//                        Data Structure Declaration
//=============================================================================

/**
 * Defines the control register for the LCD.
 */
struct LCD_CONTROL{
	uint8_t L_CTRL;
};

//=============================================================================
//						 Constant and Macro Declaration
//=============================================================================

// LCD low level interface and addresses
#define LCD_DATA_DIRECTION_MASK 0xff
#define LCD_ENABLE_ON_MASK 0x4
#define LCD_ENABLE_OFF_MASK 0xfb
#define LCD_DATA_CMD_SELECT_DATA_MASK 0x2
#define LCD_DATA_CMD_SELECT_CMD_MASK 0x0

// LCD commands
#define LCD_CMD_DATALENGTH_8BIT 0x38
#define LCD_CMD_DISPLAY_CURSOR_BLINK_ON 0x0f
#define LCD_CMD_CLEAR 0x01
#define LCD_CMD_INCREMENT_CURSOR_1 0x06
#define LCD_CMD_CURSOR_HOME 0x02
#define LCD_CMD_CURSOR_SHIFT_RIGHT1 0x14
#define LCD_SECONDLINE_CMD 0xc0

//Register Locations
#define LCD_DATA ((struct GEN_IO volatile *)(LCDDATA_BASE | BYPASS_CACHE_FLAG))
#define LCD_CTRL ((struct LCD_CONTROL volatile *)(LCDCONTROL_BASE | BYPASS_CACHE_FLAG))

//=============================================================================
//						    Function Declaration
//=============================================================================

/**
 * Writes a command to the LCD.
 * @param cmd command to be sent to LCD.
 */
void lcd_cmd(uint8_t cmd);

/**
 * Writes a character to the LCD.
 * @param c character that is to be sent to LCD.
 */
void lcd_prt(char c);

/**
 * Writes the clear command to the LCD.
 */
void lcd_clr(void);

/**
 * Moves the cursor back to home on the LCD.
 */
void lcd_home(void);

/**
 * Initializes the LCD. Sends out the command stream to setup the display. Must
 * call LCDportInit first for direction setting.
 */
void lcd_init(void);

/**
 * Sets the direction of the LCD data port.
 * @param dir Port direction mask for the LCD data port
 */
void lcd_portInit(uint8_t dir);

/**
 * Prints a string of characters to the LCD.
 * @param ptr pointer to the start of a C-string ended with a nul character.
 */
void lcd_printString(char* ptr);

#endif
