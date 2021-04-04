/**
 * Display
 * @author Curt Henrichs
 * @date 9-10-17
 *
 * Display module abstracts the LCD interface into general high level messages
 * for user. All functions that rely on low level interface should be 
 * implemented in this module.
 */

#ifndef DISPLAY_H
#define DISPLAY_H

//=============================================================================
//                                  Libraries
//=============================================================================
    
#include <project.h>
    
//=============================================================================
//                        Constant and Macro Defintions
//=============================================================================
    
#define LCD_HEIGHT      2   //! Rows of display
#define LCD_WIDTH       16  //! Columns of display
#define MAX_PROFILE_NAME_LENGTH   10 //! Max size for valid display of string
#define MAX_PROFILE_DETAIL_LENGTH 16 //! Max size for valid display of string 
    
#define display_clear() LCD_ClearDisplay() //! Aliase for clearing screen
    
//=============================================================================
//                         Public Function Prototypes
//=============================================================================

/**
 * Intialization function for display module starts hardware
 */
void display_init(void);
/**
 * Displays welcome message to the user, intended for startup of the module
 */
void display_welcome(void);
/**
 * Displays the profile menu title screen to explain process to user
 */
void display_profile_title(void);
/**
 * Displays a general profile page used for both selection and operation
 * display purposes
 * @param name is Profile name string
 * @param detail is second line string for specific details for profile
 */
void display_profile(const char* name, const char* detail);
/**
 * Displays a flashing text message on the LCD screen
 * @param l1 is line one text
 * @param l2 is line two text
 * @param c is count or number of times to flash
 * @param don is delay (in milliseconds) on per count
 * @param doff is delay (in millseconds) off per count
 */
void display_flash_text(const char* l1, const char* l2, uint32_t c, 
        uint32_t don, uint32_t doff);
/**
 * Displays ramping motor text to user
 */
void display_ramp_text(void);
/**
 * Displays stopping motor text to user
 */
void display_stop_text(void);
/**
 * Displays a string at the provided position without clearing
 * screen.
 * @param r is row position of string origin
 * @param c is column position of string origin
 * @param str is the string to print to the display
 */
void display_print_string(uint8_t r, uint8_t c, const char* str);
    
#endif
