#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<signal.h>

#include"shell.h"

static volatile int keeprun = 1;

void inthandler(int dummy){
	keeprun = 0;
}

void clockfn(char** args, int lowerind){
	int timer;
	if(args[lowerind+1]==NULL || args[lowerind+1][0]=='\0'){
		perror("Invalid command");
		return;
	}
	sscanf(args[lowerind+1], "%d", &timer);
	if(timer<=0){
		perror("Invalid command");
		return;
	}
	else{
		char* buffer = (char*)malloc(sizeof(char)*10);
		buffer[0]='\0';
		signal(SIGINT, inthandler);
		signal(SIGTSTP, inthandler);
		while(keeprun){
			int datefd = open("/sys/class/rtc/rtc0/date", O_RDONLY);
			int timefd = open("/sys/class/rtc/rtc0/time", O_RDONLY);
			read(datefd, buffer, 10);
			buffer[10]='\0';
			printf("%s, ", buffer);
			read(timefd, buffer, 8);
			buffer[8]='\0';
			printf("%s\n", buffer);
			sleep(timer);
			close(datefd);
			close(timefd);
		}
	}
}