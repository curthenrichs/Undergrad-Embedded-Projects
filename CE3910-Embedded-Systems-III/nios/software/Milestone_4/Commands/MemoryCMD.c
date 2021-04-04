/**
 * MemoryCMD Objects
 * @author Curt Henrichs
 * @date 3-31-17
 * Objects used by the command line to access memory for reading and writing.
 */

//=============================================================================
//                                  Libraries
//=============================================================================

#include "MemoryCMD.h"

#include <stdio.h>
#include <string.h>
#include "../Hardware/GenericIO.h"
#include "../Hardware/LCD.h"

//=============================================================================
//                        Private Function Declaration
//=============================================================================

/**
 * Read Command Function
 * @param arg string to parse
 */
static void _read(char* arg);
/**
 * Write Command Function
 * @param arg string to parse
 */
static void _write(char* arg);
/**
 * Prints out memory starting at the address for the count amount of bytes
 * @param address the address of memory to start printing
 * @param count the number of bytes to read and print
 */
static void _printMem(unsigned int address, unsigned int count);

//=============================================================================
//                       Command Object Instantiation
//=============================================================================

/**
 * Read Command Object
 */
command_t RR = {
	.name = "RR",
	.helpInfo = "Reads from selected memory.\n\tForm RR <address> or RR "
			    "<address> <count>.\n",
	.cmdFnt = _read
};
/**
 * Write Command Object
 */
command_t WR = {
	.name = "WR",
	.helpInfo = "Writes to selected memory.\n\tForm WR <address> <value>.\n",
	.cmdFnt = _write
};

//=============================================================================
//                          Private Function Implementation
//=============================================================================

/**
 * Read Command Function
 * @param arg string to parse
 */
static void _read(char* arg){
	lcd_printString("Reading Memory");
	char cmd[3];
	unsigned int address, count;
	int matched = sscanf(arg,"%s %x %x",cmd,&address,&count);
	if(matched == 3){
		//Read multiple
		_printMem(address,count);
	}else if(matched == 2){
		//Read single
		_printMem(address,1);
	}else{
		printf("Invalid RR Command Structure.\n");
	}
}

/**
 * Write Command Function
 * @param arg string to parse
 */
static void _write(char* arg){
	lcd_printString("Writing Memory");
	char cmd[3];
	unsigned int address, value;
	int matched = sscanf(arg,"%s %x %x",cmd,&address,&value);
	if(matched == 3){
		//always apply the store IO version of the instruction for immediate
		// change and store in original for memory required versions
		*((volatile unsigned char*)(address | BYPASS_CACHE_FLAG)) = value;
	}else{
		printf("Invalid WR Command Structure.\n");
	}
}


/**
 * Prints out memory starting at the address for the count amount of bytes
 * @param address the address of memory to start printing
 * @param count the number of bytes to read and print
 */
static void _printMem(unsigned int address, unsigned int count){

	//sanity check count for valid specification
	if(count < 1){
		printf("Error - Invalid Number of Addresses Specified.\n");
		return; //error case so leave early
	}else if(address + count < address){
		printf("Warning - Address Overflow.\n");
	}

	//buffer allocation
	char buffer[80];
	char temp[12];

	//print header
	memset(buffer,'\0',sizeof buffer);
	strcat(buffer,"  Base   :");
	int i; for(i = 0; i < count && i < 16; ++i){
		*(temp+3) = '\0';
		sprintf(temp," +%x",i);
		strcat(buffer,temp);
	}
	strcat(buffer,"\n");
	printf(buffer);

	//print bytes as chunks of buffer
	int j; for(j = 0; j < (count - 1) / 16 + 1; ++j){
		memset(buffer,'\0',sizeof buffer);

		//print address
		*(temp+10) = '\0';
		sprintf(temp,"%08x :",address + j*16);
		strcat(buffer,temp);

		//print row or less if count
		int a; for(a = 0; a < 16 && a < (count - j * 16); ++a){
			unsigned int addr = address + j*16+a;
			unsigned char val = *((unsigned char*)(addr | BYPASS_CACHE_FLAG));
			*(temp+3) = '\0';
			sprintf(temp," %02x",val);
			strcat(buffer,temp);
		}

		//send to serial device
		strcat(buffer,"\n");
		printf(buffer);
	}

	//send final character to clean up display
	printf("\n");
}
