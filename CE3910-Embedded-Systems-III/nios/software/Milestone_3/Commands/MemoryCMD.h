/**
 * MemoryCMD Objects
 * @author Curt Henrichs
 * @date 3-31-17
 * Objects used by the command line to access memory for reading and writing.
 */

#ifndef MEMORYCMD_H_
#define MEMORYCMD_H_

//=============================================================================
//                                  Libraries
//=============================================================================

#include "CommandType.h"

//=============================================================================
//                       Command Objects (Use Like Constant)
//=============================================================================

/**
 * Read Command Object
 */
extern command_t RR;

/**
 * Write Command Object
 */
extern command_t WR;

#endif /* MEMORYCMD_H_ */
