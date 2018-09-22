#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>

#include"shell.h"

void kjobcommand(char* args1, char* args2){
	int jobsnum, killsignal, reqpid;
	sscanf(args2, "%d", &killsignal);

	if(pipe(pipedeskjob)==-1){
		perror("Pipe not formed\n");
		return;
	}

	GLOBALSTDOUT = dup(1);
	dup2(pipedeskjob[1], 1);
	close(pipedeskjob[1]);

	jobscommand();

	dup2(GLOBALSTDOUT, 1);

	char buffer[10000];

	read(pipedeskjob[0], buffer, 10000);


	for(int i=0;i<10000;i++){
		if(buffer[i]=='['){
			int j=i+1, flag=0;
			while(args1[j-i-1]!='\0'){
				if(args1[j-i-1]!=buffer[j]){
					flag=1;
				}
				j++;
			}
			if(!flag){
				int count=0;
				while(count!=3){
					if(buffer[i]==' '){
						count++;
					}
					i++;
				}
				sscanf(&buffer[i], "%d", &reqpid);
			}
		}
	}
	printf("%d\n", reqpid);
	kill(reqpid, killsignal);
}