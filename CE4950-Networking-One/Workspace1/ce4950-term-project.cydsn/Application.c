/**
 * Application
 * @author Curt Henrichs
 * @date 1-11-18
 *
 * Application layer demo program for term project.
 *
 * Demo project consists of a serial terminal, command-line interface to 
 *  - log into device as a user (thereby allocating a set of addresses)
 *  - send short messages (44 characters or less) accross the network bus
 *  - receive short messages on logged in addresses and broadcast address
 *  - generate help menu for explaining commands
 *  - logout of device
 *  - crc option to generate a transmit with or without CRC.
 */

//=============================================================================
//                                 Libraries
//=============================================================================

#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "Application.h"
#include "UserAddressAllocation.h"
#include "DataLinkLayer.h"
#include "Uart.h"

//=============================================================================
//                          Constant and Macro Definition
//=============================================================================

#define UART_RX_BUFFER_SIZE (128)   //! Length of input buffer from UART host
#define UART_TX_BUFFER_SIZE (128)   //! Length of output buffer to UART host

/**
 * UART print function abstracts wait behavior
 * @param string is C-String to print
 */
#define _print(str)     do{                                 \
                            while(!uart_tx_ready());        \
	                        uart_print_str(str);            \
                        }while(0)                           \
                    

//=============================================================================
//                               Private Attributes
//=============================================================================

/**
 * Currently logged in user, _INVALID_NAME if not logged in
 */
static UserName_e _currentUser;
/**
 * Current CRC Transmit settings, Default is DLL_NO_CRC
 */
static DLL_CRC_Use_e _crc_option;
/**
 * Flag noting that the UART has been connected with host device
 */
static bool _previously_connected;
/**
 * Allocated temporary buffer for transmitting messages to UART host device
 */
static char _uart_txBuffer[UART_TX_BUFFER_SIZE];
/**
 * Allocated temporary buffer for receiving messages from UART host device
 */
static char _uart_rxBuffer[UART_RX_BUFFER_SIZE];
/**
 * Current length of receive buffer, used for tracking between iterations
 */
static uint16_t _uart_rxBuffer_len;

//=============================================================================
//                           Private Function Prototypes
//=============================================================================

/**
 * Display welcome message on UART Terminal
 */
static void _print_welcome_msg(void);
/**
 * Handle command message from UART host after a newline character received
 */
static void _handle_command(char* user_msg);
/**
 * Display help message on UART terminal
 */
static void _handle_command_help(void);
/**
 * Parse command string for login parameters. Display message to user.
 * Updates the logged in user state if successful
 * @param param is string to parse for command parameters
 */
static void _handle_command_login(char* param);
/**
 * Log out current user if a user is logged in. Display a message to user.
 * Updates state of logged in user and settings for transmit CRC.
 */
static void _handle_command_logout(void);
/**
 * Parse command string for message contents. Display message to user.
 * Starts transmission in network stack.
 * @param param is string to parse for command parameters
 */
static void _handle_command_send(char* param);
/**
 * Parse command string for CRC transmission options. Display message to user.
 * Modifies settings for transmit CRC.
 * @param param is string to parse for command parameters
 */
static void _handle_command_crc(char* param);
/**
 * Print error message to user notifying them to log in before using command.
 */
static void _print_not_logged_in_error_msg(void);
/**
 * Print error message to user stating that provided command is invalid.
 */
static void _print_command_not_recognized_error_msg(char* cmd);
/**
 * Function that captures raw data from UART and searches for newline.
 * Will modify _uart_rxBuffer with new contents.
 * @return newline is found.
 */
static bool _uart_receive_data(void);

//=============================================================================
//                         Public Function Implementation
//=============================================================================

/**
 * Application main function will run demo program for term project
 * @return --ignore
 */
int app_main(void){
    
    //initialize application state
	_currentUser = _INVALID_NAME;
    _previously_connected = false;
    _uart_rxBuffer_len = 0;
    
    //initialize hardware and network stack
    CyGlobalIntEnable; 
    dll_init();
	user_init();
    uart_init();
    LCD_Start();
    LCD_ClearDisplay();
    LCD_PrintString("Application");
    
    //wait for uart to connect
    while(!uart_update()){/* Wait Loop */}

    //application main loop
	while(1){

		uart_update();
        
        //print welcome message on usbuart configuration change
        if(uart_has_connected() && !_previously_connected){
            _previously_connected = true;  
            _print_welcome_msg();
        }else if(!uart_has_connected()){
            _previously_connected = false;   
        }
        
        //build up user message buffer until enter
        if(uart_has_data()){
            //capture data and handle command on newline
            if(_uart_receive_data()){
                _handle_command(_uart_rxBuffer);  
                memset(_uart_rxBuffer,0,sizeof(_uart_rxBuffer));
                _uart_rxBuffer_len = 0;
                _print("\n\n>>");
            }
        }

        // Network stack has received message, display and refresh current message
		if(dll_update()){
            
            //Print network received data
			DLL_Message_t rx_msg;
			dll_read(&rx_msg);
			sprintf(_uart_txBuffer,"\n\n\nRx: %d %d ",rx_msg.src,rx_msg.dst);
            _print(_uart_txBuffer);
            memcpy(_uart_txBuffer,rx_msg.msg,rx_msg.msg_size);
            _uart_txBuffer[rx_msg.msg_size] = '\n';
            _uart_txBuffer[rx_msg.msg_size+1] = '\0';
            _print(_uart_txBuffer);
            sprintf(_uart_txBuffer,"Rx-CRC: %d, Header: %s, Data: %s\n",rx_msg.use_crc,
                    (rx_msg.failed_header_crc)? "Error" : "Valid", 
                    (rx_msg.failed_data_crc) ? "Error" : "Valid");
            _print(_uart_txBuffer);

            //reprint user entered data
            _print("\n\n>>");
            _print(_uart_rxBuffer);
        }
	}
    
	return 0; /* ignore */
}

//=============================================================================
//                         Private Function Implementation
//=============================================================================

/**
 * Display welcome message on UART Terminal
 */
static void _print_welcome_msg(void){
    _print("Welcome to the Manchester United Terminal!\n\n");
    _handle_command_help();
    _print("\n\n>>");
}

/**
 * Handle command message from UART host after a newline character received
 */
static void _handle_command(char* instr){
    
    char cmd[21];
    int count = sscanf(instr,"%20s",cmd);
    if(count != EOF && count > 0){
        
        //convert command to lowercase
        for(unsigned int i = 0; i < strlen(cmd); i++){
		    cmd[i] = tolower(cmd[i]);
	    } 
        
        //parse command tree
    	if(0 == strcmp(cmd,"login")){
            if(strlen(cmd) + 1 < strlen(instr)){
                _handle_command_login(instr + strlen(cmd) + 1);
            }else{
                _print("Missing parameters.\n"); 
            }
    	}else if(0 == strcmp(cmd,"help")){
    		_handle_command_help();
    	}else if(0 == strcmp(cmd,"logout")){
            if(_currentUser != _INVALID_NAME){
                _handle_command_logout();
            }else{
                _print_not_logged_in_error_msg();
            }
    	}else if(0 == strcmp(cmd,"send")){
            if(_currentUser != _INVALID_NAME){
                if(strlen(cmd) + 1 < strlen(instr)){
                    _handle_command_send(instr + strlen(cmd) + 1);
                }else{
                    _print("Missing parameters.\n"); 
                }
            }else{
                _print_not_logged_in_error_msg();
            }	
    	}else if(0 == strcmp(cmd,"crc")){
            if(_currentUser != _INVALID_NAME){
                if(strlen(cmd) + 1 < strlen(instr)){
                    _handle_command_crc(instr + strlen(cmd) + 1);
                }else{
                    _print("Missing parameters.\n"); 
                }
            }else{
                _print_not_logged_in_error_msg();   
            }
        }else{
    	    _print_command_not_recognized_error_msg(cmd);
    	}
    }else{
        _print("Command must be supplied.\n"); 
    }
}

/**
 * Display help message on UART terminal
 */
static void _handle_command_help(void){
    _print("To log in: 'login <first name> <last name>' where name is ");
    _print("as displayed in 'Interface Standard' manual \n");
    _print("To send a message: 'send <src> <dest> <message>' where ");
    _print("<dest> is the destination name or address and <message> ");
    _print("is the message to send\n");
    _print("To change CRC transmit settings: 'crc <option>' where ");
    _print("<option> is 0=NO_CRC, 1=HEADER_CRC, 2=DATA_CRC, ");
    _print("3=BOTH_CRC\n");
    _print("To log out: 'logout'\n");
    _print("To show this list of commands again: 'help'\n");
    _print("All commands are case-insensitive!\n");
}

/**
 * Parse command string for login parameters. Display message to user.
 * Updates the logged in user state if successful
 * @param param is string to parse for command parameters
 */
static void _handle_command_login(char* param){
    char* name = strtok(param, "\n");
    for(int i = 0; name[i]; i++){
		name[i] = tolower(name[i]);
	}
	_currentUser = user_string_to_id(name);
	dll_set_user(_currentUser);
    if(_currentUser != _INVALID_NAME){
	    sprintf(_uart_txBuffer,"Welcome, %s :)\n",user_id_to_string(_currentUser));
        _print(_uart_txBuffer);
    }else{
        sprintf(_uart_txBuffer,"Failed to login user, %s :(\n",name);
        _print(_uart_txBuffer);
    }
}

/**
 * Log out current user if a user is logged in. Display a message to user.
 * Updates state of logged in user and settings for transmit CRC.
 */
static void _handle_command_logout(void){
    sprintf(_uart_txBuffer,"Goodbye, %s!\n",user_id_to_string(_currentUser));
    _print(_uart_txBuffer);
	_currentUser = _INVALID_NAME;
    _crc_option = DLL_NO_CRC;
	dll_set_user(_currentUser);
}

/**
 * Parse command string for message contents. Display message to user.
 * Starts transmission in network stack.
 * @param param is string to parse for command parameters
 */
static void _handle_command_send(char* param){

    //parse message frame components
	uint8_t msgSrc = atoi(strtok(param, " "));
    //source is within proper range, continue parsing
	if(msgSrc >= 1 && msgSrc <= 3){
		msgSrc = user_id_to_address(_currentUser,msgSrc-1);
		uint8_t msgDest = atoi(strtok(NULL, " "));
        //destination is within proper range, continue parsing
		if(msgDest >= 0 && msgSrc <= 126){
			char* msgText = strtok(NULL, "\n");
			uint8_t msgSize = strlen(msgText);
            //all criteria filled, populate and send message
			if(msgSize >= 1 && msgSize <= 44){
                 
				//populate message frame
				DLL_Message_t tx_msg;
            	tx_msg.src = msgSrc;
            	tx_msg.dst = msgDest;
            	tx_msg.use_crc = _crc_option;
            	tx_msg.msg_size = msgSize;
                memcpy(tx_msg.msg,msgText,msgSize);
				
				//write message to bus
            	bool result = dll_write(&tx_msg);
                uart_print_str(result ? "..Success\n" : "..Failed\n");
			}
			else{
			    _print("Selected message is too long.  Maximum message "
                               "length is 44 characters.\n");
            }
		}
		else{
		    _print("Improper destination used.  Value should be an "
                           "integer value between 0 and 126.\n");
        }
	}
	else{
	    _print("Improper source used.  Value should be between 1 and 3 "
                       "corresponding to the addresses of the current user.\n");
    }
}

/**
 * Parse command string for CRC transmission options. Display message to user.
 * Modifies settings for transmit CRC.
 * @param param is string to parse for command parameters
 */
static void _handle_command_crc(char* param){
    unsigned int crcOption;
    int count = sscanf(param,"%u",&crcOption);
    if(count != EOF && count > 0){
        if(crcOption < 4){
            _crc_option = (DLL_CRC_Use_e)(crcOption); 
            _print("..OK");
        }else{
            _print("Invalid CRC option requested. Value should be between "
                           "0 to 3. See Help command.\n");
        }
    }else{
        _print("Invalid parameter, refer to documentation for format.\n");
    }
}

/**
 * Print error message to user notifying them to log in before using command.
 */
static void _print_not_logged_in_error_msg(void){
    _print("No user currently logged in :(\n");
    _print("To log in: 'login <first name> <last name>' where name is "
                   "as displayed in 'Interface Standard' manual \n");
}

/**
 * Print error message to user stating that provided command is invalid.
 */
static void _print_command_not_recognized_error_msg(char* cmd){
    sprintf(_uart_txBuffer,"Command '%s' not recognized.  For a list of "
                          "available commands, type 'help'\n", cmd);
    _print(_uart_txBuffer);
}

/**
 * Function that captures raw data from UART and searches for newline.
 * Will modify _uart_rxBuffer with new contents.
 * @return newline is found.
 */
static bool _uart_receive_data(void){
    static char buffer[USBUART_BUFFER_SIZE];
    bool nl_found = false;
    uint8_t index = 0;
    char c;
    
    //read data frame from USBUART
    memset(buffer,'\0',sizeof(buffer));
    uint16_t buffer_len = uart_read(buffer,sizeof(buffer));
    
    //enter characters into receive buffer, until EOL
    while(!nl_found && _uart_rxBuffer_len < (sizeof(_uart_rxBuffer)-1) && index < buffer_len){
        c = buffer[index];
        index++;
        if(c == '\n' || c == '\r'){
            _uart_rxBuffer[_uart_rxBuffer_len] = '\0';
            _uart_rxBuffer_len++;
            nl_found = true;
        }else if(c == 0x1B){ //esc
            _uart_rxBuffer_len = 0;
            memset(_uart_rxBuffer,0,sizeof(_uart_rxBuffer));
        }else{
            _uart_rxBuffer[_uart_rxBuffer_len] = c;
            _uart_rxBuffer_len++;
        }
    }
    
    //buffer full, last character is nul, warn user and discard buffer
    if(_uart_rxBuffer_len >= (sizeof(_uart_rxBuffer)-1)){
        _uart_rxBuffer_len = 0;
        memset(_uart_rxBuffer,0,sizeof(_uart_rxBuffer));
        _print("\nWarning: Discarding buffer due to exceeding size\n");
    }
    
    //echo characters to terminal
    uart_write(buffer,index);
    
    return nl_found;
}

