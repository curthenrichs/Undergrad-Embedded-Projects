/**
 * LCD Module
 * @author Curt Henrichs
 * @date 1-4-16, revised 3-10-17
 *
 * Implements the API to control the LCD attached to the DE0. Note that errors
 * will occur in timing and missing characters if interrupts are active during
 * operation. Note that timer 1 is used for delay during output to the LCD.
 */

//=============================================================================
//						           Libraries
//=============================================================================

#include "LCD.h"

//=============================================================================
//                         Private Function Declaration
//=============================================================================

/**
 * Polls the busy flag of the LCD to synchronize the program to the LCD
 * instruction execution.
 */
static void lcd_util_pollBusyFlag();

//=============================================================================
//							  Function Implementation
//=============================================================================

/**
 * Writes a command to the LCD.
 * @param cmd command to be sent to LCD.
 */
void lcd_cmd(uint8_t cmd){
	//temporary value for data pin manipulation
	uint8_t ctrlVal;

	//Wait till LCD is ready to accept new instruction
	lcd_util_pollBusyFlag();

	// write E=RS=RW=0
	(LCD_CTRL->L_CTRL) = LCD_DATA_CMD_SELECT_CMD_MASK;

	// write command to data port
	(LCD_DATA->DATA) = cmd;

	// set enable ON
	ctrlVal = (LCD_CTRL->L_CTRL) | LCD_ENABLE_ON_MASK;
	(LCD_CTRL->L_CTRL) = ctrlVal;

	// delay for 500ns
	timer_delay(500, TIMER_1);

	// set enable OFF
	ctrlVal = (LCD_CTRL->L_CTRL) & LCD_ENABLE_OFF_MASK;
	(LCD_CTRL->L_CTRL) = ctrlVal;

}

/**
 * Writes a character to the LCD.
 * @param c character that is to be sent to LCD.
 */
void lcd_prt(char c){

	//temporary value for data pin manipulation
	uint8_t ctrlVal;

	//Wait till LCD is ready to accept new instruction
	lcd_util_pollBusyFlag();

	// write RW=E=0 RS=1  "0b010"
	(LCD_CTRL->L_CTRL) = LCD_DATA_CMD_SELECT_DATA_MASK;

	// write character to data port
	(LCD_DATA->DATA) = c;

	// set enable ON
	ctrlVal = (LCD_CTRL->L_CTRL) | LCD_ENABLE_ON_MASK;
	(LCD_CTRL->L_CTRL) = ctrlVal;

	// delay for 500ns
	timer_delay(500, TIMER_1);

	// set enable OFF
	ctrlVal = (LCD_CTRL->L_CTRL) & LCD_ENABLE_OFF_MASK;
	(LCD_CTRL->L_CTRL) = ctrlVal;
}

/**
 * Writes the clear command to the LCD.
 */
void lcd_clr(void){
	lcd_cmd(LCD_CMD_CLEAR);
}

/**
 * Moves the cursor back to home on the LCD.
 */
void lcd_home(void){
	lcd_cmd(LCD_CMD_INCREMENT_CURSOR_1);
	lcd_cmd(LCD_CMD_CURSOR_HOME);
}

/**
 * Initializes the LCD. Sends out the command stream to setup the display. Must
 * call LCDportInit first for direction setting.
 */
void lcd_init(void){
	lcd_cmd(LCD_CMD_DATALENGTH_8BIT);
	lcd_cmd(LCD_CMD_DISPLAY_CURSOR_BLINK_ON);
	lcd_cmd(LCD_CMD_CLEAR);
	lcd_cmd(LCD_CMD_INCREMENT_CURSOR_1);
	lcd_cmd(LCD_CMD_CURSOR_HOME);
}

/**
 * Sets the direction of the LCD data port.
 * @param dir Port direction mask for the LCD data port
 */
void lcd_portInit(uint8_t dir){
	(LCD_DATA->DIRECTION) = dir;
}

/**
 * Prints a string of characters to the LCD.
 * @param ptr pointer to the start of a C-string ended with a nul character.
 */
void lcd_printString(char* ptr){
	int counter;
	for(counter = 0; (*ptr != '\0')&&(counter != 32); counter++){

		// If enter character
		if(*ptr == '\n'){
			//If on first line move to second
			if(counter < 16){
				lcd_cmd(LCD_SECONDLINE_CMD);
				counter = 16;
			}
			// If on second line already end print
			else {
				return;
			}

			//index pointer
			ptr++;
		}
		//Roll over to second line when first line fills
		else{
			if(counter == 16)
				lcd_cmd(LCD_SECONDLINE_CMD);

			//print the character and index pointer
			lcd_prt(*ptr);
			ptr++;
		}
	}
}

//=============================================================================
//                        Private Function Implementation
//=============================================================================

/**
 * Polls the busy flag of the LCD to synchronize the program to the LCD
 * instruction execution.
 */
static void lcd_util_pollBusyFlag(){

	//Set data register to input
	uint32_t tempDataDir = (LCD_DATA->DIRECTION);
	(LCD_DATA->DIRECTION) = 0;

	// Set RW=E=1, RS=0 to get visible busy flag
	(LCD_CTRL->L_CTRL) = 0x5;

	// Needs small delay since processor is faster than LCD. Thus this noop
	// is ran.
	int i = 0; i++;

	// Poll busy flag while it is active, since old operation is still running
	while((LCD_DATA->DATA) & 0x80);

	// Restore the initial direction of the data ports
	(LCD_DATA->DIRECTION) = tempDataDir;
	(LCD_CTRL->L_CTRL) = 0;
}
