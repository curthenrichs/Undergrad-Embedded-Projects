/**
 * Display
 * @author Curt Henrichs
 * @date 9-10-17
 *
 * Display module abstracts the LCD interface into general high level messages
 * for user. All functions that rely on low level interface should be 
 * implemented in this module.
 */

//=============================================================================
//                                  Libraries
//=============================================================================

#include "Display.h"
#include <project.h>
#include <stdint.h>
#include <stdio.h>

//=============================================================================
//                         Constant and Macro Defintions
//=============================================================================

#define BLOCK_CHAR  LCD_CUSTOM_0 //! Custom Block Character for top row
#define FILL_TO_MSG_DELAY_TIME 250 //! time to transition in welcome message

//=============================================================================
//                         Private Function Prototypes
//=============================================================================

/**
 * Fills the screen with blocks for startup to representing the loading to
 * the user.
 */
static void _fill_blocks(void);
/**
 * Welcome message displayed to the user 
 */
static void _welcome_msg(void);

//=============================================================================
//                        Public Function Implementation
//=============================================================================

/**
 * Intialization function for display module starts hardware
 */
void display_init(void){
    LCD_Start();
}

/**
 * Displays welcome message to the user, intended for startup of the module
 */
void display_welcome(void){
    _fill_blocks();
    display_clear();
    CyDelay(FILL_TO_MSG_DELAY_TIME);
    _welcome_msg();
}

/**
 * Displays the profile menu title screen to explain process to user
 */
void display_profile_title(void){
    display_clear();
    LCD_Position(0,0);
    LCD_PrintString("Profile Menu");
    LCD_Position(1,0);
    LCD_PrintString("Select L, Next R");
}

/**
 * Displays a general profile page used for both selection and operation
 * display purposes
 * @param name is Profile name string
 * @param detail is second line string for specific details for profile
 */
void display_profile(const char* name, const char* detail){
    if(name == NULL || detail == NULL){
        return; //invalid input   
    }else if(strlen(name) > MAX_PROFILE_NAME_LENGTH){
        return; //invalid length
    }else if(strlen(detail) > MAX_PROFILE_DETAIL_LENGTH){
        return; //invalid length
    }
    
    display_clear();
    LCD_Position(0,0);
    char temp[17];
    sprintf(temp,"Name: %s",name);
    LCD_PrintString(temp);
    LCD_Position(1,0);
    LCD_PrintString(detail);
    
}

/**
 * Displays a flashing text message on the LCD screen
 * @param l1 is line one text
 * @param l2 is line two text
 * @param c is count or number of times to flash
 * @param don is delay (in milliseconds) on per count
 * @param doff is delay (in millseconds) off per count
 */
void display_flash_text(const char* l1, const char* l2, uint32_t c, 
        uint32_t don, uint32_t doff){
    //sanity check
    if(l1 == NULL || l2 == NULL){
        return;
    }else if(strlen(l1) > LCD_WIDTH || strlen(l2) > LCD_WIDTH){
        return;
    }else if(c == 0){
        return;   
    }
    
    //flash text
    display_clear();
    for(uint32_t i=0; i<c; ++i){ 
        if(don > 0){
            LCD_Position(0,0);
            LCD_PrintString(l1);
            LCD_Position(1,0);
            LCD_PrintString(l2);
            CyDelay(don);
        }   
        if(doff > 0){
            display_clear();
            CyDelay(doff);
        }
    }
    display_clear();
}

//=============================================================================
//                        Private Function Implementation
//=============================================================================

/**
 * Fills the screen with blocks for startup to representing the loading to
 * the user.
 */
static void _fill_blocks(void){
    display_clear();
    LCD_Position(0,0);
    for(uint8_t c=0; c<16; ++c){
        LCD_PutChar(BLOCK_CHAR);
        CyDelay(100);
    }
}

/**
 * Welcome message displayed to the user 
 */
static void _welcome_msg(void){
    display_clear();
    LCD_Position(0,3);
    LCD_PrintString("Welcome to");
    LCD_Position(1,2);
    LCD_PrintString("My Treadmill");
}