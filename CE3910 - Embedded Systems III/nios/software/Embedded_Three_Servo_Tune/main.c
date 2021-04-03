/*
 * main.c
 *
 *  Created on: Mar 16, 2017
 *      Author: henrichsc
 */

#include "PWM.h"

#include <stdio.h>
#include <system.h>

int main(){

	char inputStr[80];
	unsigned int value;

	PWM_writeOCRA(1400);
	PWM_writeOCRA(1400);

	while(1){

		printf("Enter OCR Value:\n");

		if(fgets(inputStr,sizeof inputStr, stdin) != NULL){
			char* newline = strchr(inputStr,'\n');
			if(newline != NULL){
				*newline = '\0';
			}
		}

		sscanf(inputStr,"%d",&value);
		PWM_writeOCRA(value);
	}

	return 0;
}
