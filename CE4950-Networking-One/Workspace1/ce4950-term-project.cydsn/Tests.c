/**
 * Tests
 * @author Curt Henrichs
 * @date 1-11-18
 *
 * Tests module encapsulates the test cases applied to the term project. 
 * Each test case is build with reference to a written test plan corresponding
 * to the enumerated week.
 *
 * Before building the application one should verify the TEST_CASE 
 * configuration parameter is valid. Additionally, ensure all necessary 
 * hardware and system drivers are initialized outside of this module.
 */

//=============================================================================
//                                 Libraries
//=============================================================================

#include "Tests.h"

#include "project.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "UserAddressAllocation.h"
#include "DataLinkLayer.h"

#include "CSMA_CD_Bus.h"
#include "ChannelMonitor.h"
#include "Transmitter.h"
#include "Receiver.h"
#include "Uart.h"

//=============================================================================
//                       Constant and Macro Definitions : Week 5
//=============================================================================

/** 
 * States for test menu application state-machine.
 */
typedef enum Menu_States {
    MENU_START,     //! Initalization state for state machine
    MENU_DISPLAY,   //! User driven menu
    MENU_UART,      //! Transmit characters from UART
    MENU_NUL,       //! Transmit 0x00
    MENU_REPEAT_1,  //! Transmit 0xAA
    MENU_REPEAT_2,  //! Transmit 0x55
    MENU_ONES       //! Transmit 0xFF
} Menu_States_e;

#define NUM_SELECTABLE_OPTIONS (5) //! Menu selectable states

#define CAPSENSE_SETTLE_TIME  100 //! Time to delay during initialization (ms)

//=============================================================================
//                        Private Function Prototypes : Week 5
//=============================================================================

/** 
 * Display test case menu
 * @param on_enter first entry of state-machine for one-time logic
 * @return next state as selected by user else this state if no selection
 */
static Menu_States_e display(bool on_enter);
/** 
 * UART test case - transmit buffered message from USBUART.
 * @param on_enter first entry of state-machine for one-time logic
 * @return this state until user presses return button
 */
static Menu_States_e uart_mode(bool on_enter);
/** 
 * 0x00 test case - transmit string of 0x00 characters
 * @param on_enter first entry of state-machine for one-time logic
 * @return this state until user presses return button
 */
static Menu_States_e nul_string_mode(bool on_enter);
/** 
 * 0xAA test case - transmit string of 0xAA characters
 * @param on_enter first entry of state-machine for one-time logic
 * @return this state until user presses return button
 */
static Menu_States_e repeat_string_1_mode(bool on_enter);
/** 
 * 0x55 test case - transmit string of 0x55 characters
 * @param on_enter first entry of state-machine for one-time logic
 * @return this state until user presses return button
 */
static Menu_States_e repeat_string_2_mode(bool on_enter);
/** 
 * 0xFF test case - transmit string of 0xFF characters
 * @param on_enter first entry of state-machine for one-time logic
 * @return this state until user presses return button
 */
static Menu_States_e ones_mode(bool on_enter);

//=============================================================================
//                          Private Attribute : Week 5
//=============================================================================

/** 
 * Menu states selected
 */
static Menu_States_e MENU_OPTIONS_LIST[NUM_SELECTABLE_OPTIONS];
/** 
 * Menu string naming the options
 */
static const char* MENU_OPTIONS_TAGS[NUM_SELECTABLE_OPTIONS];

//=============================================================================
//                   Private Function Implementation : Week 10
//=============================================================================

/**
 * CRC calulation algorithm used in both the transmitter and receiver
 * @param buffer is pointer to message to calculate over
 * @param len is number of bytes in message to calculate over
 * @return 4-bit CRC value
 */
static uint8_t crc_algorithm(uint8_t* buffer, uint8_t len);

//=============================================================================
//                         Public Function Implementation
//=============================================================================

/**
 * Testing module main function will run selected test week
 * @return --ignore
 */
int tests_main(void){
    
    //initialization routine
    CyGlobalIntEnable; 
    uart_init();
    LCD_Start();
    LCD_ClearDisplay();
    CapSense_Start();
    CyDelay(CAPSENSE_SETTLE_TIME);
    CapSense_InitializeAllBaselines();
    
    switch(CURRENT_TEST){
        case TEST_CASE_WEEK_3:
            tests_week3();
            break;
        case TEST_CASE_WEEK_5:
            tests_week5();
            break;
        case TEST_CASE_WEEK_6:
            tests_week6();
            break;
        case TEST_CASE_WEEK_8:
            tests_week8();
            break;
        case TEST_CASE_WEEK_9:
            tests_week9();
            break;
        case TEST_CASE_WEEK_10:
            tests_week10();
            break;
    }
    
    while(1){/* flow capture loop */}
    
    return 0; /* ignore */
}

/** 
 * Test for week 3 is to determine functionality of the channel monitor
 */
void tests_week3(void){
    csma_bus_init();
    cm_init();
    tx_init();
    rx_init();
    
    CM_State_e ch_state = cm_get_bus_state();
    const char* str = NULL;
    uint8_t state_trig = 0;
    
    while(true){
        
        ch_state = cm_get_bus_state();
        switch(ch_state){
            case CM_STATE_INIT:
                str = "INIT";
                break;
            case CM_STATE_IDLE:
                str = "IDLE";
                break;
            case CM_STATE_BUSY:
                str = "BUSY";
                break;
            case CM_STATE_COLLISION:
                str = "COLLISION";
                break;
            default:
                str = "";
                break;
        }
        
        LCD_ClearDisplay();
        LCD_Position(0,0);
        LCD_PrintString(str);
        LCD_Position(1,0);
        LCD_PrintHexUint8(state_trig);
        state_trig |= CM_Test_State_Reg_Read();
        CyDelay(10);
    }
}

/**
 * Test for week 5 is a state-machine with user driven menu to select between
 * several test cases for transmitter functionality tests.
 */
void tests_week5(void){
    csma_bus_init();
    cm_init();
    tx_init();
    rx_init();
    
    MENU_OPTIONS_LIST[0] = MENU_UART;
    MENU_OPTIONS_LIST[1] = MENU_NUL;
    MENU_OPTIONS_LIST[2] = MENU_REPEAT_1;
    MENU_OPTIONS_LIST[3] = MENU_REPEAT_2;
    MENU_OPTIONS_LIST[4] = MENU_ONES;
    
    MENU_OPTIONS_TAGS[0] = "UART";
    MENU_OPTIONS_TAGS[1] = "REPEAT 0x00";
    MENU_OPTIONS_TAGS[2] = "REPEAT 0xAA";
    MENU_OPTIONS_TAGS[3] = "REPEAT 0x55";
    MENU_OPTIONS_TAGS[4] = "REPEAT 0xFF";
    
    // Test State-Machine for menu driven control
    Menu_States_e curr_state = MENU_DISPLAY;
    Menu_States_e next_state = MENU_START;
    bool on_enter = true;
    while(true){
        
        //update capacitive touch user input
        if(!CapSense_IsBusy()){
            CapSense_UpdateEnabledBaselines();
            CapSense_ScanEnabledWidgets();
        }
        
        switch(curr_state){
            case MENU_DISPLAY:
                next_state = display(on_enter);
                break;
            case MENU_UART:
                next_state = uart_mode(on_enter);
                break;
            case MENU_NUL:
                next_state = nul_string_mode(on_enter);
                break;
            case MENU_REPEAT_1:
                next_state = repeat_string_1_mode(on_enter);
                break;
            case MENU_REPEAT_2:
                next_state = repeat_string_2_mode(on_enter);
                break;
            case MENU_ONES:
                next_state = ones_mode(on_enter);
                break;
            default:
                next_state = MENU_DISPLAY;
                break;
        }
        
        on_enter = (next_state != curr_state);
        curr_state = next_state;
    }
}

/** 
 * Test week 6 will verify correct behavior for interaction between channel
 * monitor and transmission module.
 */
void tests_week6(void){
    csma_bus_init();
    cm_init();
    tx_init();
    rx_init();
    
    static char buffer[USBUART_BUFFER_SIZE];
    static char disp[16];
    static char txStr[33];
    static uint8_t txStr_index;
    
    memset(txStr,0,sizeof(txStr));
    txStr_index = 0;
    
    LCD_ClearDisplay();
    LCD_Position(0,0);
    LCD_PrintString("UART:");

    while(true){
        uart_update();
        
        if(uart_has_data()){
            memset(buffer,'\0',sizeof(buffer));
            uint16_t numChar = uart_read(buffer,sizeof(buffer));
            uart_write(buffer,strlen(buffer));
            
            LCD_ClearDisplay();
            LCD_Position(0,0);
            LCD_PrintString("UART:");
            LCD_Position(1,0);
            memcpy(disp,buffer,15);
            disp[15] = 0;
            LCD_PrintString(disp);
            
            bool nl_found = false;
            uint8_t index = 0;
            char c;
            while(!nl_found && txStr_index < (sizeof(txStr)-1) && index < numChar){
                c = buffer[index];
                index++;
                if(c == '\n' || c == '\r'){
                    txStr[txStr_index] = '\0';
                    nl_found = true;
                }else{
                    txStr[txStr_index] = c;
                }
                txStr_index++;
            }
            
            //buffer full, last character is nul
            if(txStr_index >= (sizeof(txStr)-1)){
                txStr[32] = '\0';
                nl_found = true;
            }
            
            //transmit if newline or full
            if(nl_found){
                txStr_index = 0;
                LCD_Position(1,0);
                LCD_PrintString("Transmitting");
                tx_write(txStr,strlen(txStr));
                while(tx_get_status() != TX_DONE){/*Intentionally Empty*/}
                CyDelay(100);
                LCD_Position(1,0);
                LCD_PrintString("            ");
            }
        }
    }
}

/**
 * Test week 8 will test the developed receiver module for valid interpretation
 * of characters. Note that header information is not handled.
 */
void tests_week8(void){
    csma_bus_init();
    cm_init();
    tx_init();
    rx_init();
    
    static char txStr[33];
    static uint8_t txStr_index;
    memset(txStr,0,sizeof(txStr));
    txStr_index = 0;
    
    char buffer[USBUART_BUFFER_SIZE];
    char str[17];
    
    LCD_ClearDisplay();
   
    while(1){
        uart_update();
        
        if(uart_has_data()){
            memset(buffer,'\0',sizeof(buffer));
            uint16_t numChar = uart_read(buffer,sizeof(buffer));
            uart_write(buffer,strlen(buffer));
            
            bool nl_found = false;
            uint8_t index = 0;
            char c;
            while(!nl_found && txStr_index < (sizeof(txStr)-1) && index < numChar){
                c = buffer[index];
                index++;
                if(c == '\n' || c == '\r'){
                    txStr[txStr_index] = '\0';
                    nl_found = true;
                }else{
                    txStr[txStr_index] = c;
                }
                txStr_index++;
            }
            
            //buffer full, last character is nul
            if(txStr_index >= (sizeof(txStr)-1)){
                txStr[32] = '\0';
                nl_found = true;
            }
            
            //transmit if newline or full
            if(nl_found){
                txStr_index = 0;
                tx_write(txStr,strlen(txStr));
                while(tx_get_status() != TX_DONE){/*Intentionally Empty*/}
            }
        }
        
        if(rx_has_data()){
            uint8_t count = rx_buffer(buffer,sizeof(buffer));
            LCD_ClearDisplay();
            LCD_Position(0,0);
            sprintf(str,"Size: %d",count);
            LCD_PrintString(str);
            LCD_Position(1,0);
            uint8_t cpy = (count < 16) ? count : 16;
            memcpy(str,buffer,cpy);
            str[cpy] = 0;
            //sprintf(str,"ASCII: %d",(uint8_t)(buffer[0]));
            LCD_PrintString(str);
            uart_write(buffer,count);
        }
        
        if(!CapSense_IsBusy()){
            CapSense_UpdateEnabledBaselines();
            CapSense_ScanEnabledWidgets();
        }
        
        if(CapSense_CheckIsWidgetActive(CapSense_LEFT__BTN)){
            LCD_ClearDisplay();   
        }
    }
}

/**
 * Test week 9 will prove basic interoperability and compliance to CSMA/CD 
 * protocol using the expected data headers.
 */
void tests_week9(void){
    user_init();
    dll_init();
    dll_set_user(CURT_HENRICHS);
    
    static char txStr[45];
    static uint8_t txStr_index;
    memset(txStr,0,sizeof(txStr));
    txStr_index = 0;
    
    char buffer[USBUART_BUFFER_SIZE];
    char str[50];
    
    LCD_ClearDisplay();
    
    uint8_t state = 0;
    bool button_held_down = false;
    
    uint8_t tx_state = 0;
    bool tx_bool = false;
    uint8_t rx_state = 0;
    uint8_t lk_state = 0;
    
    while(1){
        dll_update();   
        
        if(!CapSense_IsBusy()){
            CapSense_UpdateEnabledBaselines();
            CapSense_ScanEnabledWidgets();
        }
        
        if(CapSense_CheckIsWidgetActive(CapSense_LEFT__BTN)){
            if(!button_held_down){
                button_held_down = true;
                state = (state + 1) % 3;
                tx_state = 0;
                tx_bool = false;
                rx_state = 0;
                lk_state = 0;
            }
        }else{
            button_held_down = false;   
        }
        
        uart_update();
        
        DLL_Message_t msg;
        switch(state){
            case 0: // Transmit mode
                if(!tx_bool){
                    tx_bool = true;
                    LCD_ClearDisplay();
                    LCD_Position(0,0);
                    LCD_PrintString("Transmit");
                }
                switch(tx_state){
                    case 0:
                        //expect transmit
                        if(dll_tx_finished()){
                            CyDelay(50);
                            tx_state = 1;
                            msg.src = 0x0B;
                            msg.dst = 0x00;
                            msg.msg_size = 0;
                            msg.msg[0] = 0;
                            msg.use_crc = DLL_NO_CRC;
                            dll_write(&msg);
                        }
                        break;
                    case 1:
                        //expect transmit
                        if(dll_tx_finished()){
                            CyDelay(50);
                            tx_state = 2;
                            msg.src = 0x0B;
                            msg.dst = 0x15;
                            msg.msg_size = 44;
                            for(int i=0; i<44; i++){
                                msg.msg[i] = 'a';   
                            }
                            msg.use_crc = DLL_NO_CRC;
                            dll_write(&msg);
                        }
                        break;
                    case 2:
                        //expect transmit
                        if(dll_tx_finished()){
                            CyDelay(50);
                            tx_state = 3;
                            msg.src = 0x0B;
                            msg.dst = 0x33;
                            msg.msg_size = 5;
                            strcpy(msg.msg,"Hello");
                            msg.use_crc = DLL_NO_CRC;
                            dll_write(&msg);
                        }
                        break;
                    case 3:
                        //expect no transmit
                        if(dll_tx_finished()){
                            CyDelay(50);
                            tx_state = 0;
                            msg.src = 0x0B;
                            msg.dst = 0x41;
                            msg.msg_size = 45;
                            for(int i=0; i<44; i++){
                                msg.msg[i] = 'q';   
                            }
                            msg.use_crc = DLL_NO_CRC;
                            dll_write(&msg);
                        }
                        break;
                    default:
                        tx_state = 0;
                        break;
                }
                break;
            case 1: // Receive mode
                if(!rx_state){
                    rx_state = 1;   
                    LCD_ClearDisplay();
                    LCD_Position(0,0);
                    LCD_PrintString("Receive");
                }
                
                if(dll_has_data()){
                    uint8_t count = dll_read(&msg);
                    sprintf(str,"SRC: %d, DST: %d, SIZE: %d, MSG: ",msg.src,
                            msg.dst,msg.msg_size);
                    uart_write(str,strlen(str));
                    uart_write(msg.msg,count);
                    uart_write("\n\r",2);
                }
                break;
            case 2: // Linked mode
                if(!lk_state){
                    lk_state = 1;   
                    LCD_ClearDisplay();
                    LCD_Position(0,0);
                    LCD_PrintString("Linked");
                }
                
                if(dll_has_data()){
                    uint8_t count = dll_read(&msg);
                    sprintf(str,"SRC: %d, DST: %d, SIZE: %d, MSG: ",msg.src,
                            msg.dst,msg.msg_size);
                    uart_write(str,strlen(str));
                    uart_write(msg.msg,count);
                    uart_write("\n\r",2);
                }
                
                if(uart_has_data()){
                    memset(buffer,'\0',sizeof(buffer));
                    uint16_t numChar = uart_read(buffer,sizeof(buffer));
                    uart_write(buffer,strlen(buffer));
                    
                    bool nl_found = false;
                    uint8_t index = 0;
                    char c;
                    while(!nl_found && txStr_index < (sizeof(txStr)-1) && 
                            index < numChar){
                        c = buffer[index];
                        index++;
                        if(c == '\n' || c == '\r'){
                            txStr[txStr_index] = '\0';
                            nl_found = true;
                        }else{
                            txStr[txStr_index] = c;
                        }
                        txStr_index++;
                    }
                    
                    //buffer full, last character is nul
                    if(txStr_index >= (sizeof(txStr)-1)){
                        txStr[44] = '\0';
                        nl_found = true;
                    }
                    
                    //transmit if newline or full
                    if(nl_found){
                        txStr_index = 0;
                        msg.src = 0x0B;
                        msg.dst = 0x00;
                        msg.msg_size = strlen(txStr);
                        memcpy(msg.msg,txStr,msg.msg_size);
                        msg.use_crc = DLL_NO_CRC;
                        dll_write(&msg);
                    }
                }
                break;
            default:
                state = 0;
                break;
        }
    }
}

/**
 * Test week 10 will validate final system drivers for full compliance to
 * protocol.
 */
void tests_week10(void){
    user_init();
    dll_init();
    dll_set_user(CURT_HENRICHS);

    
    bool left_button_held_down = false;
    bool right_button_held_down = false;
    uint8_t state = 0;
    uint8_t crcState = 0;
    bool crcFlag = false;
    uint8_t txState = 0;
    bool txFlag = false;
    bool rxFlag = false;
    
    DLL_Message_t msg;
    char str[50];
    

    while(1){
        
        dll_update();   
        
        if(!CapSense_IsBusy()){
            CapSense_UpdateEnabledBaselines();
            CapSense_ScanEnabledWidgets();
        }
        
        if(CapSense_CheckIsWidgetActive(CapSense_LEFT__BTN)){
            if(!left_button_held_down){
                left_button_held_down = true;
                state = (state + 1) % 3;
                right_button_held_down = false;
                crcState = 0;
                crcFlag = false;
                txState = 0;
                txFlag = false;
                rxFlag = false;
            }
        }else{
            left_button_held_down = false;   
        }
        
        uart_update();
        
        switch(state){
            case 0:
                //run crc case
                switch(crcState){
                    case 0:
                        if(!crcFlag){
                            crcFlag = true;
                            uint8_t buffer[] = {0x85};
                            uint8_t x = crc_algorithm(buffer,1);
                            LCD_ClearDisplay();
                            LCD_Position(0,0);
                            LCD_PrintString("CRC:");
                            LCD_PrintInt8(crcState);
                            LCD_Position(1,0);
                            LCD_PrintHexUint8(x);  
                        }
                        break;
                    case 1:
                        if(!crcFlag){
                            crcFlag = true;
                            uint8_t buffer[] = {0x85, 0xAC};
                            uint8_t x = crc_algorithm(buffer,2);
                            LCD_ClearDisplay();
                            LCD_Position(0,0);
                            LCD_PrintString("CRC:");
                            LCD_PrintInt8(crcState);
                            LCD_Position(1,0);
                            LCD_PrintHexUint8(x);  
                        }
                        break;
                    case 2:
                        if(!crcFlag){
                            crcFlag = true;
                            uint8_t buffer[] = {0xC4, 0xC1, 0xFA};
                            uint8_t x = crc_algorithm(buffer,3);
                            LCD_ClearDisplay();
                            LCD_Position(0,0);
                            LCD_PrintString("CRC:");
                            LCD_PrintInt8(crcState);
                            LCD_Position(1,0);
                            LCD_PrintHexUint8(x);  
                        }
                        break;
                    case 3:
                        if(!crcFlag){
                            crcFlag = true;
                            uint8_t buffer[] = {0x80, 0x81, 0x8A, 0x80, 0x83, 0x83};
                            uint8_t x = crc_algorithm(buffer,6);
                            LCD_ClearDisplay();
                            LCD_Position(0,0);
                            LCD_PrintString("CRC:");
                            LCD_PrintInt8(crcState);
                            LCD_Position(1,0);
                            LCD_PrintHexUint8(x);  
                        }
                        break;
                    default:
                        crcState = 0;
                        break;
                }
                //switch next crc state
                if(CapSense_CheckIsWidgetActive(CapSense_RIGHT__BTN)){
                    if(!right_button_held_down){
                        right_button_held_down = true;
                        crcState = (crcState + 1) % 4;
                        crcFlag = false;
                    }
                }else{
                    right_button_held_down = false;   
                }
                break;
            case 1: 
                if(!txFlag){
                    txFlag = true;
                    LCD_ClearDisplay();
                    LCD_Position(0,0);
                    LCD_PrintString("Transmit");
                    LCD_Position(1,0);
                    LCD_PrintInt8(txState);
                }
                
                switch(txState){
                    case 0:
                        if(dll_tx_finished()){
                            CyDelay(50);
                            msg.src = 0x0A;
                            msg.dst = 0x00;
                            msg.msg_size = 3;
                            strcpy(msg.msg,"DAz");
                            msg.use_crc = DLL_BOTH_CRC;
                            dll_write(&msg);
                        }
                        break;
                    case 1:
                        if(dll_tx_finished()){
                            CyDelay(50);
                            msg.src = 0x0A;
                            msg.dst = 0x00;
                            msg.msg_size = 3;
                            strcpy(msg.msg,"DAz");
                            msg.use_crc = DLL_HEADER_CRC;
                            dll_write(&msg);
                        }
                        break;
                    case 2:
                        if(dll_tx_finished()){
                            CyDelay(50);
                            msg.src = 0x0A;
                            msg.dst = 0x00;
                            msg.msg_size = 3;
                            strcpy(msg.msg,"DAz");
                            msg.use_crc = DLL_MESSAGE_CRC;
                            dll_write(&msg);
                        }
                        break;
                    case 3:
                        if(dll_tx_finished()){
                            CyDelay(50);
                            msg.src = 0x0A;
                            msg.dst = 0x00;
                            msg.msg_size = 3;
                            strcpy(msg.msg,"DAz");
                            msg.use_crc = DLL_NO_CRC;
                            dll_write(&msg);
                        }
                        break;
                    default:
                        txState = 0;
                        break;
                }
                //switch next tx state
                if(CapSense_CheckIsWidgetActive(CapSense_RIGHT__BTN)){
                    if(!right_button_held_down){
                        right_button_held_down = true;
                        txState = (txState + 1) % 4;
                        txFlag = false;
                    }
                }else{
                    right_button_held_down = false;   
                }
                break;
            case 2:
                if(!rxFlag){
                    rxFlag = true;   
                    LCD_ClearDisplay();
                    LCD_Position(0,0);
                    LCD_PrintString("Receive");
                }
                
                if(dll_has_data()){
                    uint8_t count = dll_read(&msg);
                    sprintf(str,"SRC: %d, DST: %d, SIZE: %d, MSG: ",msg.src,
                            msg.dst,msg.msg_size);
                    uart_write(str,strlen(str));
                    uart_write(msg.msg,count);
                    uart_write("\n\r",2);
                    sprintf(str,"CRC_MODE: %d, HEADER CRC: %s, DATA CRC: %s\n\r",
                            msg.use_crc,(msg.failed_header_crc) ? "N" : "Y",
                            (msg.failed_data_crc) ? "N" : "Y");
                    uart_write(str,strlen(str));
                }
                break;
            default:
                state = 0;
                break;
        }
    }
}

//=============================================================================
//                   Private Function Implementation : Week 5
//=============================================================================

/** 
 * Display test case menu
 * @param on_enter first entry of state-machine for one-time logic
 * @return next state as selected by user else this state if no selection
 */
static Menu_States_e display(bool on_enter){
    static uint8_t index;
    static bool hold;
    
    if(on_enter){
        index = 0;
        hold = true;
        LCD_ClearDisplay();
        LCD_Position(0,0);
        LCD_PrintString("MENU:");
        LCD_Position(1,0);
        LCD_PrintString(MENU_OPTIONS_TAGS[index]);
    }
    
    bool iterate = CapSense_CheckIsWidgetActive(CapSense_LEFT__BTN);
    if(iterate && !hold){
        index = (index + 1) % NUM_SELECTABLE_OPTIONS;
        LCD_ClearDisplay();
        LCD_Position(0,0);
        LCD_PrintString("MENU:");
        LCD_Position(1,0);
        LCD_PrintString(MENU_OPTIONS_TAGS[index]);
    }
    hold = iterate;
    
    bool nextState = CapSense_CheckIsWidgetActive(CapSense_RIGHT__BTN);
    Menu_States_e retval = (nextState) ? MENU_OPTIONS_LIST[index] : MENU_DISPLAY;
    return retval;
}

/** 
 * UART test case - transmit buffered message from USBUART.
 * @param on_enter first entry of state-machine for one-time logic
 * @return this state until user presses return button
 */
static Menu_States_e uart_mode(bool on_enter){
    static char buffer[USBUART_BUFFER_SIZE];
    static char disp[16];
    static char txStr[33];
    static uint8_t txStr_index;
    
    if(on_enter){
        memset(txStr,0,sizeof(txStr));
        txStr_index = 0;
        
        LCD_ClearDisplay();
        LCD_Position(0,0);
        LCD_PrintString("UART:");
    }
    
    uart_update();
        
    if(uart_has_data()){
        memset(buffer,'\0',sizeof(buffer));
        uint16_t numChar = uart_read(buffer,sizeof(buffer));
        uart_write(buffer,strlen(buffer));
        
        LCD_ClearDisplay();
        LCD_Position(0,0);
        LCD_PrintString("UART:");
        LCD_Position(1,0);
        memcpy(disp,buffer,15);
        disp[15] = 0;
        LCD_PrintString(disp);
        
        bool nl_found = false;
        uint8_t index = 0;
        char c;
        while(!nl_found && txStr_index < (sizeof(txStr)-1) && index < numChar){
            c = buffer[index];
            index++;
            if(c == '\n' || c == '\r'){
                txStr[txStr_index] = '\0';
                nl_found = true;
            }else{
                txStr[txStr_index] = c;
            }
            txStr_index++;
        }
        
        //buffer full, last character is nul
        if(txStr_index >= (sizeof(txStr)-1)){
            txStr[32] = '\0';
            nl_found = true;
        }
        
        //transmit if newline or full
        if(nl_found){
            txStr_index = 0;
            LCD_Position(1,0);
            LCD_PrintString("Transmitting");
            tx_write(txStr,strlen(txStr));
            while(tx_get_status() != TX_DONE){/* Intentionally Empty */}
            CyDelay(100);
            LCD_Position(1,0);
            LCD_PrintString("            ");
        }
    }
    
    bool returnMenu = CapSense_CheckIsWidgetActive(CapSense_LEFT__BTN);
    Menu_States_e retval = (returnMenu) ? MENU_DISPLAY : MENU_UART;
    return retval;
}

/** 
 * 0x00 test case - transmit string of 0x00 characters
 * @param on_enter first entry of state-machine for one-time logic
 * @return this state until user presses return button
 */
static Menu_States_e nul_string_mode(bool on_enter){
    static bool hold;
    bool active = CapSense_CheckIsWidgetActive(CapSense_RIGHT__BTN);
    
    if(on_enter){
        hold = true;
        LCD_ClearDisplay();
        LCD_Position(0,0);
        LCD_PrintString("REPEAT 0x00:");
    }
    
    if(active && !hold){
        LCD_Position(1,0);
        LCD_PrintString("Transmitting");
        char buffer[] = { 0x00, 0x00, 0x00, 0x00 };
        tx_write(buffer,sizeof(buffer));
        while(tx_get_status() != TX_DONE){/*Intentionally Empty*/}
        CyDelay(100);
        LCD_Position(1,0);
        LCD_PrintString("            ");
    }
    hold = active;
    
    bool returnMenu = CapSense_CheckIsWidgetActive(CapSense_LEFT__BTN);
    Menu_States_e retval = (returnMenu) ? MENU_DISPLAY : MENU_NUL;
    return retval;
}

/** 
 * 0xAA test case - transmit string of 0xAA characters
 * @param on_enter first entry of state-machine for one-time logic
 * @return this state until user presses return button
 */
static Menu_States_e repeat_string_1_mode(bool on_enter){
    static bool hold;
    bool active = CapSense_CheckIsWidgetActive(CapSense_RIGHT__BTN);
    
    if(on_enter){
        hold = true;
        LCD_ClearDisplay();
        LCD_Position(0,0);
        LCD_PrintString("REPEAT 0xAA:");
    }
    
    if(active && !hold){
        LCD_Position(1,0);
        LCD_PrintString("Transmitting");
        char buffer[] = { 0xAA, 0xAA, 0xAA, 0xAA };
        tx_write(buffer,sizeof(buffer));
        while(tx_get_status() != TX_DONE){/*Intentionally Empty*/}
        CyDelay(100);
        LCD_Position(1,0);
        LCD_PrintString("            ");
    }
    hold = active;
    
    bool returnMenu = CapSense_CheckIsWidgetActive(CapSense_LEFT__BTN);
    Menu_States_e retval = (returnMenu) ? MENU_DISPLAY : MENU_REPEAT_1;
    return retval;
}

/** 
 * 0x55 test case - transmit string of 0x55 characters
 * @param on_enter first entry of state-machine for one-time logic
 * @return this state until user presses return button
 */
static Menu_States_e repeat_string_2_mode(bool on_enter){
    static bool hold;
    bool active = CapSense_CheckIsWidgetActive(CapSense_RIGHT__BTN);
    
    if(on_enter){
        hold = true;
        LCD_ClearDisplay();
        LCD_Position(0,0);
        LCD_PrintString("REPEAT 0x55:");
    }
    
    if(active && !hold){
        LCD_Position(1,0);
        LCD_PrintString("Transmitting");
        char buffer[] = { 0x55, 0x55, 0x55, 0x55 };
        tx_write(buffer,sizeof(buffer));
        while(tx_get_status() != TX_DONE){/*Intentionally Empty*/}
        CyDelay(100);
        LCD_Position(1,0);
        LCD_PrintString("            ");
    }
    hold = active;
    
    bool returnMenu = CapSense_CheckIsWidgetActive(CapSense_LEFT__BTN);
    Menu_States_e retval = (returnMenu) ? MENU_DISPLAY : MENU_REPEAT_2;
    return retval;
}

/** 
 * 0xFF test case - transmit string of 0xFF characters
 * @param on_enter first entry of state-machine for one-time logic
 * @return this state until user presses return button
 */
static Menu_States_e ones_mode(bool on_enter){
    static bool hold;
    bool active = CapSense_CheckIsWidgetActive(CapSense_RIGHT__BTN);
    
    if(on_enter){
        hold = true;
        LCD_ClearDisplay();
        LCD_Position(0,0);
        LCD_PrintString("REPEAT 0xFF:");
    }
    
    if(active && !hold){
        LCD_Position(1,0);
        LCD_PrintString("Transmitting");
        char buffer[] = { 0xFF, 0xFF, 0xFF, 0xFF };
        tx_write(buffer,sizeof(buffer));
        while(tx_get_status() != TX_DONE){/*Intentionally Empty*/}
        CyDelay(100);
        LCD_Position(1,0);
        LCD_PrintString("            ");
    }
    hold = active;
    
    bool returnMenu = CapSense_CheckIsWidgetActive(CapSense_LEFT__BTN);
    Menu_States_e retval = (returnMenu) ? MENU_DISPLAY : MENU_ONES;
    return retval;
}

//=============================================================================
//                   Private Function Implementation : Week 10
//=============================================================================

/**
 * CRC calulation algorithm used in both the transmitter and receiver
 * @param buffer is pointer to message to calculate over
 * @param len is number of bytes in message to calculate over
 * @return 4-bit CRC value
 */
static uint8_t crc_algorithm(uint8_t* buffer, uint8_t len){
    const uint8_t gen = 0x13;
    uint8_t i,k,b,msb;
    uint8_t x = 0;
    
    //shift buffer in
    for(i=0; i<len; i++){
        b = buffer[i];
        for(k=0; k<8; k++){
            if(k == 0){
                msb = 1; //MSB is always one during transmission
            }else{
                msb = (b & (1 << (7 - k))) >> (7 - k);
            }
            x = (x << 1) | msb;
            x = x ^ (gen & ((x & 0x10) ? (0x0F) : (0x00)));
        }
    }
    
    //shift zero bits to find FCS
    for(k=0; k<4; k++){
        x = (x << 1) | 0;
        x = x ^ (gen & ((x & 0x10) ? (0x0F) : (0x00)));
    }
    
    return x & 0x0F;
}