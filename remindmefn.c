#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#include"shell.h"

void remindmefn(char** args, int lowerind){
	int timer;
	sscanf(args[lowerind+1], "%d", &timer);
	if(timer<=0){
		perror("Invalid command");
		return;
	}
	else{
		int pid = fork();
		if(pid==0){
			sleep(timer);
			printf("Reminder: ");
			for(int i=lowerind+2;args[i]!=NULL;i++){
				printf("%s ", args[i]);
			}
			printf("\n");
			exit(0);
		}
	}
}