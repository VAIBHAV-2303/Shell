#include<stdio.h>
#include<stdlib.h>

#include"shell.h"

void echocommand(char** args, int notreqind, int lowerind, int upperind, char* previouspipedoutput){
	for(int i=lowerind+1;args[i]!=NULL && i<upperind;i++){
		if(i!=notreqind){
			if(args[i][0]=='$'){
				printf("%s", getenv(&args[i][1]));
			}
			else{
				printf("%s ", args[i]);
			}
		}
	}

	if(previouspipedoutput!=NULL){
		printf("%s ", previouspipedoutput);
	}

	printf("\n");
}