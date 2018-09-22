#include<stdio.h>
#include<stdlib.h>

#include"shell.h"

char* readcommandline(){
	char* aux;
	aux = (char*)malloc(sizeof(char)*COMMANDLINE_SIZE);

	if(!aux){
		allocationerror();
	}

	// Taking input
	fgets(aux, COMMANDLINE_SIZE, stdin);
	return aux;
}