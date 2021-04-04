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

#ifndef USERADDRESSALLOCATION_H
#define USERADDRESSALLOCATION_H
    
//=============================================================================
//                                 Libraries
//=============================================================================

#include <stdint.h>
 
//=============================================================================
//                        Constant and Macro Definitions
//=============================================================================
    
/**
 * Multiple addresses assigned to each user, addresses are seperated by their
 * use "level".
 */
typedef enum UserAddressLevel {
    USER_ADDRESS_LVL_1,     //! Address 1
    USER_ADDRESS_LVL_2,     //! Address 2
    USER_ADDRESS_LVL_3      //! Address 3
} UserAddressLevel_e;

/**
 * CE4950 Networking 1 class roster. Used to allocate user names and addresses
 */
typedef enum UserName {
    NETWORK_BROADCAST,  //! <Special> General bus call
    PAMELA_ANTAR,
    SAMUEL_CONROY,
    SAMUEL_HALLERMAN,
    CURT_HENRICHS,
    JOSEPHINE_LOCURTO,
    ANTHONY_MARLOWE,
    MUNAZZA_NAEEM,
    MATHEW_PIKE,
    SHAHRUKE_QURESHI,
    DIVIN_RAMACHANDRAN,
    ALEXANDER_ROBERTS,
    REID_RUMACK,
    JOHN_SELFORS,
    ZACHARY_STANKE,
    ANDREW_STOEHR,
    TYLER_WELLS,
    JOHNATHON_WENZEL,
    JOSHUA_WIDDER,
    ANDREW_WINSTON,
    XU_ZHANG,
    ALWIN_ABRAHAM,
    NATHEER_ALKHUNAIZY,
    CALEB_BAHR,
    PALLAV_BANIK,
    ZACHARY_BARTMER,
    NICHOLAS_BOUSKA,
    RILEY_BRUCE,
    GERALD_GEHRKE,
    BENTON_GEIMER,
    ADAM_HAY,
    QUIHAO_JIN,
    SETH_LIABRAATEN,
    MAURIZIO_PADILLA,
    HUNTER_PARKS,
    NIRVAN_POONACHA_THEETHIRA,
    JOHN_SCHLEDERER,
    JAMES_SHIPP,
    LAWRENCE_SKUSE,
    WOODROW_WALKER,
    DMITRI_WATERS,
    JAMES_WINDORJJ,
    KAMITH_MIRISSAGE,
    
    //Used for table allocation
    _USER_TABLE_SIZE,   //! Used to generate user table structure
    _INVALID_NAME       //! <Special> Invalid user return code
} UserName_e;
    
//=============================================================================
//                          Public Function Prototypes
//=============================================================================

/**
 * Initialization for user table. Must be called before attempting to convert
 * between username forms.
 */
void user_init(void);
/**
 * Map enumerated ID to an address as specified by the level desired.
 * @name is enumerated user identity
 * @lvl is to select which of the allocated addresses to return
 * @return 7-bit address (8th MSB is 0) or 0xFF if not found
 */
uint8_t user_id_to_address(UserName_e name, UserAddressLevel_e lvl);
/**
 * Map address back to an enumerated identity.
 * @addr is 7-bit address (8th MSB is 0) to match.
 * @return enumerated ID of username or _INVALID_NAME if not found
 */
UserName_e user_address_to_id(uint8_t addr);
/**
 * Map stringified username to an enumerated identity
 * @str is lowervase string matching table's stringified name identity
 * @return enumerated ID of username or _INVALID_NAME if not found
 */
UserName_e user_string_to_id(const char* str);
/**
 * Map enumerated identity to stringified form
 * @name is enumerated user identity
 * @return stringified version of enumerated identity else "invalid name"
 */
const char* user_id_to_string(UserName_e name);
/**
 * Map string username to an address as specified by the level desired.
 * @str is lowercase string matching table's stringified name identity
 * @lvl is to select which of the allocated addresses to return
 * @return 7-bit address (8th MSB is 0) or 0xFF if not found
 */
uint8_t user_string_to_address(const char* str, UserAddressLevel_e lvl);
/**
 * Map address to stringified identity of username
 * @addr is 7-bit address (8th MSB is 0) to match.
 * @return stringified version of enumerated identity else "invalid name"
 */
const char* user_address_to_string(uint8_t addr);
    
#endif
    