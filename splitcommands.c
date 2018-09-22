#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"shell.h"

char** splitcommands(char* commandline){
	char** individualcommands;

	individualcommands = (char**)malloc(sizeof(char*)*COMMANDLINE_SIZE);

	if(!individualcommands){
		allocationerror();
	}

	int pos = 0;
	// Assuming ; to be command delimitter
	char* token = strtok(commandline, ";");

	while(token != NULL){	
		individualcommands[pos] = (char*)malloc(sizeof(char)*COMMAND_SIZE);
		if(!individualcommands[pos]){
			allocationerror();
		}

		individualcommands[pos] = token;
		token = strtok(NULL, ";");

		pos++;
	}

	if(pos>COMMAND_SIZE-10){
		perror("Too long command!");
		exit(1);
	}

	individualcommands[pos] = NULL;

	return individualcommands;
}