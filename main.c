#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<dirent.h>
#include<sys/stat.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>
#include<fcntl.h> 

#include"shell.h"


int main(){

	GLOBALSTDOUT=dup(1);
	GLOBALSTDIN=dup(0);

	bgprocarr = (pair*)malloc(sizeof(pair)*1000);

	for(int i=0;i<1000;i++){
		bgprocarr[i].name=NULL;
		bgprocarr[i].pid=-2;
	}

	homepath = getenv("PWD");
	char* commandline;
	char** individualcommands;
	char** args;
	int status, bgprocpid;
	// Main loop
	while(1){
		prompt();
		// Read the entire line
		commandline = readcommandline();

		// Seperate commands on the basis of ';'
		individualcommands = splitcommands(commandline);
		for(int i=0;individualcommands[i]!=NULL;i++){
			// Breaking a single command
			args = splitintoargs(individualcommands[i]);
			// printf("%s\n", args[0]);
			execute(args);

			free(args);
		}
		// Clearing memory resources
		free(commandline);
		free(individualcommands);

		while((bgprocpid = waitpid(-1, &status, WNOHANG))>0){
			if(WIFEXITED(status)){
				for(int i=0;i<1000;i++){
					if(bgprocarr[i].pid==bgprocpid){
						printf("Process %s with pid %d exited normally\n", bgprocarr[i].name, bgprocpid);
						free(bgprocarr[i].name);
						bgprocarr[i].name=NULL;
						bgprocarr[i].pid=-2;
						break;
					}
				}
			}
			if(WIFSIGNALED(status)){
				for(int i=0;i<1000;i++){
					if(bgprocarr[i].pid==bgprocpid){
						fprintf(stderr, "Process %s with pid %d was terminated by a signal\n", bgprocarr[i].name, bgprocpid);
						free(bgprocarr[i].name);
						bgprocarr[i].name=NULL;
						bgprocarr[i].pid=-2;
						break;
					}
				}
			}
		}

	}	
	return 0;
}