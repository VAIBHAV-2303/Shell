#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"shell.h"

char** splitintoargs(char* singlecommnd){
	char** args = (char**)malloc(sizeof(char*)*NUM_OF_ARGS);

	if(!args){
		allocationerror();
	}

	int pos = 0;
	// Assuming spaces and tabs to be arguments delimitter
	char* token = strtok(singlecommnd, " \n\t\r");

	while(token != NULL){
		args[pos] = (char*)malloc(sizeof(char)*ARG_SIZE);
		if(!args[pos]){
			allocationerror();
		}

		args[pos] = token;
		token = strtok(NULL, " \n\t\r");
		pos++;
	}

	if(pos>NUM_OF_ARGS-10){
		perror("Too many arguments");
		exit(1);
	}

	args[pos] = NULL;

	return args;
}