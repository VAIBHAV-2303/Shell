#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#include"shell.h"

void cdcommand(char* args){

	if(args==NULL || args[0] == '~' || args[0] == '\0'){
		chdir(homepath);
		return;
	}

	if(chdir(args)==-1){
		perror("Invalid path!");
	}
}