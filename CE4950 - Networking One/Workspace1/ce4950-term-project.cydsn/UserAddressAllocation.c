/**
 * UserAddressAllocation
 * @author Curt Henrichs
 * @date 2-3-18
 *
 * Defines and initializes a user name lookup table that corresponds to CE4950
 * Networking 1's class roster for Winter Term 2017-2018 academic year.
 *
 * Use the mapping functions to convert from string to user to addresses.
 */

//=============================================================================
//                                 Libraries
//=============================================================================

#include "UserAddressAllocation.h"

#include <string.h>

//=============================================================================
//                            Data Structure Declaration
//=============================================================================
    
/**
 * Username table element captures relevant characteristics for all users of 
 * network bus.
 */
typedef struct User_Info {
    UserName_e name;    //! Enumerated name used for networking user association
    const char* str;    //! Stringified identity for application layer
    uint8_t lvl_1_addr; //! First address for user
    uint8_t lvl_2_addr; //! Second address for user
    uint8_t lvl_3_addr; //! Third address for user
} User_Info_t;

//=============================================================================
//                                Private Attributes
//=============================================================================
    
/**
 * Stores all username information.
 * Initalized during module initalization.
 */
static User_Info_t _userTable[_USER_TABLE_SIZE];

//=============================================================================
//                          Public Function Iplementation
//=============================================================================

/**
 * Initialization for user table. Must be called before attempting to convert
 * between username forms.
 */
void user_init(void){
    _userTable[0].name = NETWORK_BROADCAST;
    _userTable[0].str = "broadcast";
    _userTable[0].lvl_1_addr = 0x00;
    _userTable[0].lvl_2_addr = 0x00;
    _userTable[0].lvl_3_addr = 0x00;
    
    _userTable[1].name = PAMELA_ANTAR;
    _userTable[1].str = "pamela antar";
    _userTable[1].lvl_1_addr = 0x01;
    _userTable[1].lvl_2_addr = 0x02;
    _userTable[1].lvl_3_addr = 0x03;
    
    _userTable[2].name = SAMUEL_CONROY;
    _userTable[2].str = "samuel conroy";
    _userTable[2].lvl_1_addr = 0x04;
    _userTable[2].lvl_2_addr = 0x05;
    _userTable[2].lvl_3_addr = 0x06;
    
    _userTable[3].name = SAMUEL_HALLERMAN;
    _userTable[3].str = "samuel hallerman";
    _userTable[3].lvl_1_addr = 0x07;
    _userTable[3].lvl_2_addr = 0x08;
    _userTable[3].lvl_3_addr = 0x09;
    
    _userTable[4].name = CURT_HENRICHS;
    _userTable[4].str = "curt henrichs";
    _userTable[4].lvl_1_addr = 0x0A;
    _userTable[4].lvl_2_addr = 0x0B;
    _userTable[4].lvl_3_addr = 0x0C;
    
    _userTable[5].name = JOSEPHINE_LOCURTO;
    _userTable[5].str = "josephine locurto";
    _userTable[5].lvl_1_addr = 0x0D;
    _userTable[5].lvl_2_addr = 0x0E;
    _userTable[5].lvl_3_addr = 0x0F;
    
    _userTable[6].name = ANTHONY_MARLOWE;
    _userTable[6].str = "anthony marlowe";
    _userTable[6].lvl_1_addr = 0x10;
    _userTable[6].lvl_2_addr = 0x11;
    _userTable[6].lvl_3_addr = 0x12;
    
    _userTable[7].name = MUNAZZA_NAEEM;
    _userTable[7].str = "munazza naeem";
    _userTable[7].lvl_1_addr = 0x13;
    _userTable[7].lvl_2_addr = 0x14;
    _userTable[7].lvl_3_addr = 0x15;
    
    _userTable[8].name = MATHEW_PIKE;
    _userTable[8].str = "mathew pike";
    _userTable[8].lvl_1_addr = 0x16;
    _userTable[8].lvl_2_addr = 0x17;
    _userTable[8].lvl_3_addr = 0x18;
    
    _userTable[9].name = SHAHRUKE_QURESHI;
    _userTable[9].str = "shahruke qureshi";
    _userTable[9].lvl_1_addr = 0x19;
    _userTable[9].lvl_2_addr = 0x1A;
    _userTable[9].lvl_3_addr = 0x1B;
    
    _userTable[10].name = DIVIN_RAMACHANDRAN;
    _userTable[10].str = "divin ramachandran";
    _userTable[10].lvl_1_addr = 0x1C;
    _userTable[10].lvl_2_addr = 0x1D;
    _userTable[10].lvl_3_addr = 0x1E;
    
    _userTable[11].name = ALEXANDER_ROBERTS;
    _userTable[11].str = "alexander roberts";
    _userTable[11].lvl_1_addr = 0x1F;
    _userTable[11].lvl_2_addr = 0x20;
    _userTable[11].lvl_3_addr = 0x21;
    
    _userTable[12].name = REID_RUMACK;
    _userTable[12].str = "reid rumack";
    _userTable[12].lvl_1_addr = 0x22;
    _userTable[12].lvl_2_addr = 0x23;
    _userTable[12].lvl_3_addr = 0x24;
    
    _userTable[13].name = JOHN_SELFORS;
    _userTable[13].str = "john selfors";
    _userTable[13].lvl_1_addr = 0x25;
    _userTable[13].lvl_2_addr = 0x26;
    _userTable[13].lvl_3_addr = 0x27;
    
    _userTable[14].name = ZACHARY_STANKE;
    _userTable[14].str = "zachary stanke";
    _userTable[14].lvl_1_addr = 0x28;
    _userTable[14].lvl_2_addr = 0x29;
    _userTable[14].lvl_3_addr = 0x2A;
    
    _userTable[15].name = ANDREW_STOEHR;
    _userTable[15].str = "andrew stoehr";
    _userTable[15].lvl_1_addr = 0x2B;
    _userTable[15].lvl_2_addr = 0x2C;
    _userTable[15].lvl_3_addr = 0x2D;
    
    _userTable[16].name = TYLER_WELLS;
    _userTable[16].str = "tyler wells";
    _userTable[16].lvl_1_addr = 0x2E;
    _userTable[16].lvl_2_addr = 0x2F;
    _userTable[16].lvl_3_addr = 0x30;
    
    _userTable[17].name = JOHNATHON_WENZEL;
    _userTable[17].str = "johnathon wenzel";
    _userTable[17].lvl_1_addr = 0x31;
    _userTable[17].lvl_2_addr = 0x32;
    _userTable[17].lvl_3_addr = 0x33;
    
    _userTable[18].name = JOSHUA_WIDDER;
    _userTable[18].str = "joshua widder";
    _userTable[18].lvl_1_addr = 0x34;
    _userTable[18].lvl_2_addr = 0x35;
    _userTable[18].lvl_3_addr = 0x36;
    
    _userTable[19].name = ANDREW_WINSTON;
    _userTable[19].str = "andrew winston";
    _userTable[19].lvl_1_addr = 0x37;
    _userTable[19].lvl_2_addr = 0x38;
    _userTable[19].lvl_3_addr = 0x39;
    
    _userTable[20].name = XU_ZHANG;
    _userTable[20].str = "xu zhang";
    _userTable[20].lvl_1_addr = 0x3A;
    _userTable[20].lvl_2_addr = 0x3B;
    _userTable[20].lvl_3_addr = 0x3C;
    
    _userTable[21].name = ALWIN_ABRAHAM;
    _userTable[21].str = "alwin abraham";
    _userTable[21].lvl_1_addr = 0x3D;
    _userTable[21].lvl_2_addr = 0x3E;
    _userTable[21].lvl_3_addr = 0x3F;
    
    _userTable[22].name = NATHEER_ALKHUNAIZY;
    _userTable[22].str = "natheer alkhunaizy";
    _userTable[22].lvl_1_addr = 0x40;
    _userTable[22].lvl_2_addr = 0x41;
    _userTable[22].lvl_3_addr = 0x42;
    
    _userTable[23].name = CALEB_BAHR;
    _userTable[23].str = "caleb bahr";
    _userTable[23].lvl_1_addr = 0x43;
    _userTable[23].lvl_2_addr = 0x44;
    _userTable[23].lvl_3_addr = 0x45;
    
    _userTable[24].name = PALLAV_BANIK;
    _userTable[24].str = "pallav banik";
    _userTable[24].lvl_1_addr = 0x46;
    _userTable[24].lvl_2_addr = 0x47;
    _userTable[24].lvl_3_addr = 0x48;
    
    _userTable[25].name = ZACHARY_BARTMER;
    _userTable[25].str = "zachary bartmer";
    _userTable[25].lvl_1_addr = 0x49;
    _userTable[25].lvl_2_addr = 0x4A;
    _userTable[25].lvl_3_addr = 0x4B;
    
    _userTable[26].name = NICHOLAS_BOUSKA;
    _userTable[26].str = "nicholas bouska";
    _userTable[26].lvl_1_addr = 0x4C;
    _userTable[26].lvl_2_addr = 0x4D;
    _userTable[26].lvl_3_addr = 0x4E;
    
    _userTable[27].name = RILEY_BRUCE;
    _userTable[27].str = "riley bruce";
    _userTable[27].lvl_1_addr = 0x4F;
    _userTable[27].lvl_2_addr = 0x50;
    _userTable[27].lvl_3_addr = 0x51;
    
    _userTable[28].name = GERALD_GEHRKE;
    _userTable[28].str = "gerlad gehrke";
    _userTable[28].lvl_1_addr = 0x52;
    _userTable[28].lvl_2_addr = 0x53;
    _userTable[28].lvl_3_addr = 0x54;
    
    _userTable[29].name = BENTON_GEIMER;
    _userTable[29].str = "benton geitmer";
    _userTable[29].lvl_1_addr = 0x55;
    _userTable[29].lvl_2_addr = 0x56;
    _userTable[29].lvl_3_addr = 0x57;
    
    _userTable[30].name = ADAM_HAY;
    _userTable[30].str = "adam hay";
    _userTable[30].lvl_1_addr = 0x58;
    _userTable[30].lvl_2_addr = 0x59;
    _userTable[30].lvl_3_addr = 0x5A;
    
    _userTable[31].name = QUIHAO_JIN;
    _userTable[31].str = "quihao jin";
    _userTable[31].lvl_1_addr = 0x5B;
    _userTable[31].lvl_2_addr = 0x5C;
    _userTable[31].lvl_3_addr = 0x5D;
    
    _userTable[32].name = SETH_LIABRAATEN;
    _userTable[32].str = "seth liabraaten";
    _userTable[32].lvl_1_addr = 0x5E;
    _userTable[32].lvl_2_addr = 0x5F;
    _userTable[32].lvl_3_addr = 0x60;
    
    _userTable[33].name = MAURIZIO_PADILLA;
    _userTable[33].str = "maurizio padilla";
    _userTable[33].lvl_1_addr = 0x61;
    _userTable[33].lvl_2_addr = 0x62;
    _userTable[33].lvl_3_addr = 0x63;
    
    _userTable[34].name = HUNTER_PARKS;
    _userTable[34].str = "hunter parks";
    _userTable[34].lvl_1_addr = 0x64;
    _userTable[34].lvl_2_addr = 0x65;
    _userTable[34].lvl_3_addr = 0x66;
    
    _userTable[35].name = NIRVAN_POONACHA_THEETHIRA;
    _userTable[35].str = "nirvam poonacha theethira";
    _userTable[35].lvl_1_addr = 0x67;
    _userTable[35].lvl_2_addr = 0x68;
    _userTable[35].lvl_3_addr = 0x69;
    
    _userTable[36].name = JOHN_SCHLEDERER;
    _userTable[36].str = "john schlederer";
    _userTable[36].lvl_1_addr = 0x6A;
    _userTable[36].lvl_2_addr = 0x6B;
    _userTable[36].lvl_3_addr = 0x6C;
    
    _userTable[37].name = JAMES_SHIPP;
    _userTable[37].str = "james shipp";
    _userTable[37].lvl_1_addr = 0x6D;
    _userTable[37].lvl_2_addr = 0x6E;
    _userTable[37].lvl_3_addr = 0x6F;
    
    _userTable[38].name = LAWRENCE_SKUSE;
    _userTable[38].str = "lawrence skuse";
    _userTable[38].lvl_1_addr = 0x70;
    _userTable[38].lvl_2_addr = 0x71;
    _userTable[38].lvl_3_addr = 0x72;
    
    _userTable[39].name = WOODROW_WALKER;
    _userTable[39].str = "woodrow walker";
    _userTable[39].lvl_1_addr = 0x73;
    _userTable[39].lvl_2_addr = 0x74;
    _userTable[39].lvl_3_addr = 0x75;
    
    _userTable[40].name = DMITRI_WATERS;
    _userTable[40].str = "woodrow walker";
    _userTable[40].lvl_1_addr = 0x76;
    _userTable[40].lvl_2_addr = 0x77;
    _userTable[40].lvl_3_addr = 0x78;
    
    _userTable[41].name = JAMES_WINDORJJ;
    _userTable[41].str = "james windorjj";
    _userTable[41].lvl_1_addr = 0x79;
    _userTable[41].lvl_2_addr = 0x7A;
    _userTable[41].lvl_3_addr = 0x7B;
    
    _userTable[42].name = KAMITH_MIRISSAGE;
    _userTable[42].str = "kamith mirissage";
    _userTable[42].lvl_1_addr = 0x7C;
    _userTable[42].lvl_2_addr = 0x7D;
    _userTable[42].lvl_3_addr = 0x7E;
}

/**
 * Map enumerated ID to an address as specified by the level desired.
 * @name is enumerated user identity
 * @lvl is to select which of the allocated addresses to return
 * @return 7-bit address (8th MSB is 0) or 0xFF if not found
 */
uint8_t user_id_to_address(UserName_e name, UserAddressLevel_e lvl){
    uint8_t retval = 0xFF; //default invalid address
    
    uint8_t i;
    for(i=0; i<_USER_TABLE_SIZE; i++){
        if(_userTable[i].name == name){
            switch(lvl){
                case USER_ADDRESS_LVL_1:
                    retval = _userTable[i].lvl_1_addr;
                    break;
                case USER_ADDRESS_LVL_2:
                    retval = _userTable[i].lvl_2_addr;
                    break;
                case USER_ADDRESS_LVL_3:
                    retval = _userTable[i].lvl_3_addr;
                    break;
                default:
                    retval = 0xFF; //invalid level -> invalid address
                    break;
            }
            break;
        }
    }
    
    return retval;
}

/**
 * Map address back to an enumerated identity.
 * @addr is 7-bit address (8th MSB is 0) to match.
 * @return enumerated ID of username or _INVALID_NAME if not found
 */
UserName_e user_address_to_id(uint8_t addr){
    UserName_e retval = _INVALID_NAME; //default invalid name
    
    uint8_t i;
    for(i=0; i<_USER_TABLE_SIZE; i++){
        if(_userTable[i].lvl_1_addr == addr || _userTable[i].lvl_2_addr == addr 
                || _userTable[i].lvl_3_addr == addr){
            retval = _userTable[i].name;
            break;
        }
    }
    
    return retval;
}

/**
 * Map stringified username to an enumerated identity
 * @str is lowervase string matching table's stringified name identity
 * @return enumerated ID of username or _INVALID_NAME if not found
 */
UserName_e user_string_to_id(const char* str){
    UserName_e retval = _INVALID_NAME; //default invalid name
    
    uint8_t i;
    for(i=0; i<_USER_TABLE_SIZE; i++){
        if(strcmp(str,_userTable[i].str) == 0){
            retval = _userTable[i].name;
            break;
        }
    }
    
    return retval;
}

/**
 * Map enumerated identity to stringified form
 * @name is enumerated user identity
 * @return stringified version of enumerated identity else "invalid name"
 */
const char* user_id_to_string(UserName_e name){
    const char* retval = "invalid name";
    
    uint8_t i;
    for(i=0; i<_USER_TABLE_SIZE; i++){
        if(_userTable[i].name == name){
            retval = _userTable[i].str;
            break;
        }
    }
    
    return retval;
}

/**
 * Map string username to an address as specified by the level desired.
 * @str is lowercase string matching table's stringified name identity
 * @lvl is to select which of the allocated addresses to return
 * @return 7-bit address (8th MSB is 0) or 0xFF if not found
 */
uint8_t user_string_to_address(const char* str, UserAddressLevel_e lvl){
    uint8_t retval = 0xFF; //default invalid address
    
    uint8_t i;
    for(i=0; i<_USER_TABLE_SIZE; i++){
        if(strcmp(str,_userTable[i].str) == 0){
            switch(lvl){
                case USER_ADDRESS_LVL_1:
                    retval = _userTable[i].lvl_1_addr;
                    break;
                case USER_ADDRESS_LVL_2:
                    retval = _userTable[i].lvl_2_addr;
                    break;
                case USER_ADDRESS_LVL_3:
                    retval = _userTable[i].lvl_3_addr;
                    break;
                default:
                    retval = 0xFF; //invalid level -> invalid address
                    break;
            }
            break;
        }
    }
    
    return retval;
}

/**
 * Map address to stringified identity of username
 * @addr is 7-bit address (8th MSB is 0) to match.
 * @return stringified version of enumerated identity else "invalid name"
 */
const char* user_address_to_string(uint8_t addr){
    const char* retval = "invalid name";
    
    uint8_t i;
    for(i=0; i<_USER_TABLE_SIZE; i++){
        if(_userTable[i].lvl_1_addr == addr || _userTable[i].lvl_2_addr == addr 
                || _userTable[i].lvl_3_addr == addr){
            retval = _userTable[i].str;
            break;
        }
    }
    
    return retval;
}