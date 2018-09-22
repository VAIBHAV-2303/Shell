#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
#include<limits.h>

#include"shell.h"

void pwdcommand(){
	char* cwd = (char*)malloc(sizeof(char)*CWD_SIZE);

	if(!cwd){
		allocationerror();
	}

	getcwd(cwd, CWD_SIZE);
	printf("%s\n", cwd);
}