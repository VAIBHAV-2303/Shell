#include<stdlib.h>

#include"shell.h"

char* catflags(char** args, int notreqind, int upperind, int lowerind){
	char* combined = (char*)malloc(sizeof(char)*COMMAND_SIZE);

	if(!combined){
		allocationerror();
	}

	combined[0]='-';
	int pos = 1;

	for(int i=lowerind+1;args[i]!=NULL && i<upperind;i++){
		if(i != notreqind){
			if(args[i][0] != '-'){
				continue;
			}
			else{
				for(int j=1;args[i][j]!='\0';j++){
					combined[pos] = args[i][j];
					pos++;
				}
			}
		}
	}
	combined[pos]='\0';

	if(combined[1]=='\0'){
		combined[0]='\0';
	}

	return combined;
}