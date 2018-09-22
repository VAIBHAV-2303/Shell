#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>

#include"shell.h"

int stdoutclone;

int fg(char* args1){

	if(pipe(pipedesfg)==-1){
		perror("Pipe not formed\n");
		return -1;
	}

	GLOBALSTDOUT = dup(1);
	int reqpid;

	dup2(pipedesfg[1], 1);
	close(pipedesfg[1]);
	
	jobscommand();

	dup2(GLOBALSTDOUT, 1);


	char buffer[10000];

	read(pipedesfg[0], buffer, 10000);

	// printf("%s\n", buffer);

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
	
	kill(reqpid, SIGSTOP);	
	kill(reqpid, SIGCONT);
	return reqpid;
}