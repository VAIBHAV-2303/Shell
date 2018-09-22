#include<stdlib.h>

#include"shell.h"

char* catargs(char** args, int notreqind, int upperind, int lowerind, char* previouspipedoutput){
	char* combined = (char*)malloc(sizeof(char)*COMMAND_SIZE);

	if(!combined){
		allocationerror();
	}

	int pos = 0;

	for(int i=lowerind+1;args[i]!=NULL && i<upperind;i++){
		if(i != notreqind){
			if(args[i][0] == '-'){
				continue;
			}
			else{
				for(int j=0;args[i][j]!='\0';j++){
					combined[pos] = args[i][j];
					pos++;
				}
				combined[pos] = ' ';
				pos++;
			}
		}
	}
	if(previouspipedoutput!=NULL){
		for(int i=0;previouspipedoutput[i]!='\0';i++){
			combined[pos]=previouspipedoutput[i];
			pos++;
		}
	}
	if(pos!=0){
		combined[pos-1] = '\0';
	}
	else{
		combined[0] = '\0';
	}
	return combined;
}