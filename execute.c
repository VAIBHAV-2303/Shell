#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<signal.h>
#include<fcntl.h>

#include"shell.h"

// Built_in_commands
const char * Built_in_commands[] = {
	"cd",
	"echo",
	"pwd",
	"pinfo",
	"setenv",
	"unsetenv",
	"jobs",
	"kjob",	
	"fg",
	"bg",
	"overkill",
};

static volatile int keeprunning;
void signalhandler(int dummy){
	keeprunning = 0;
}

void execute(char** args){

	int commandindexupper=0, breakallowed=0, commandindexlower=0;
	pipedes[0]=-1;
	pipedes[1]=-1;
	prevpiping=0;
	char* previouspipeoutput=(char*)malloc(sizeof(char)*1000);
	// Loop to handle piping
	while(!breakallowed){
		
		piping=0;
		previouspipeoutput[0]='\0';

		commandindexlower=commandindexupper;
		
		int outputfd, outputredirection=0;
		for(int i=commandindexlower;args[i]!=NULL;i++){
			commandindexupper=i;
			if(args[i][0]=='|'){
				piping=1;
				commandindexupper=i+1;
				args[i]=NULL;
				break;
			}
			if(args[i][0]=='>'){
				commandindexupper = i+2;
				if(args[i][1]=='>' && args[i][2]=='\0'){
					outputfd = open(args[i+1], O_WRONLY | O_CREAT | O_APPEND, 0644);
				}
				else{
					outputfd = open(args[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
				}
				if(outputfd==-1){
					perror("Output file redirection error\n");
					return;
				}
				outputredirection=1;
				args[i]=NULL;
				args[i+1]=NULL;
				break;
			}
		}

		if(outputredirection){
			breakallowed=1;
		}
		else if(!piping){
			// printf("This time no pipe\n");
			commandindexupper++;
			breakallowed=1;
		}

		prevpiping++;

		// Checking for input redirection
		int inputredirection=0, inputfd=-1;
		if(prevpiping<=1){
			for(int i=commandindexlower;i<commandindexupper;i++){
				if(args[i]!=NULL && args[i][0]=='<' && args[i][1]=='\0'){
					inputredirection=1;
					inputfd=open(args[i+1], O_RDONLY);
					if(inputfd==-1){
						perror("Input file redirectionerror\n");
					}
					args[i]=NULL;
					args[i+1]=NULL;
					break;
				}
			}
		}
		if(prevpiping>1){
			close(pipedes[1]);
			GLOBALSTDIN=dup(0);
			dup2(pipedes[0], 0);
			close(pipedes[0]);
		}
		if(piping){
			if(pipe(pipedes)==-1){
				perror("pipe failed\n");
			}
			GLOBALSTDOUT=dup(1);
			dup2(pipedes[1], 1);
		}
		if(outputredirection){
			GLOBALSTDOUT=dup(1);
			dup2(outputfd, 1);
		}
		if(inputredirection){
			GLOBALSTDIN=dup(0);
			dup2(inputfd, 0);
			close(inputfd);
		}


		keeprunning = 1;

		// Invaliod command 
		if(args[commandindexlower] == NULL){
			perror("Invalid command!");
			return;
		}

		// Empty command
		if(args[commandindexlower][0] == '\0'){
			return;
		}

		// Quit coommand

		if(strcmp(args[commandindexlower], "quit")==0 || strcmp(args[commandindexlower], "exit")==0){
			printf("Bye\n");
			exit(EXIT_SUCCESS);
			return;
		}

		// Bonus commands

		if(strcmp(args[commandindexlower], "remindme") == 0){
			remindmefn(args, commandindexlower);
			return;
		}

		if(strcmp(args[commandindexlower], "clock") == 0){
			clockfn(args, commandindexlower);
			return;
		}

		// Checking if a background process

		int bgproc = 0, notreqind=NUM_OF_ARGS + 100;

		for(int i=commandindexlower;args[i]!=NULL && i<commandindexupper;i++){
			if(args[i][0] == '&'){
				bgproc = 1;
				notreqind = i;
				args[i]=NULL;
				break;
			}
		}


		char** parsedargs = (char**)malloc(sizeof(char*)*2);

		parsedargs[0] = catflags(args, notreqind, commandindexupper, commandindexlower);
		parsedargs[1] = catargs(args, notreqind, commandindexupper, commandindexlower, previouspipeoutput);

		// cd command should be called in parent itself
		if(strcmp(Built_in_commands[0], args[commandindexlower])==0){
			if(bgproc==0){
				cdcommand(parsedargs[1]);
			}
			else{
				int pid=fork();
				if(pid==0){
					cdcommand(parsedargs[1]);
					exit(0);
				}
				else{
					printf("%d\n", pid);
				}
			}
			return;
		}

		// setenv and unsetenv
		if(strcmp(Built_in_commands[4], args[commandindexlower])==0){
			char envvar[50], valvar[50];
			int ind;
			for(int i=0;i<parsedargs[1][i]!='\0';i++){
				if(parsedargs[1][i]=='['){
					envvar[i]='\0';
					ind=i;
					break;
				}
				envvar[i]=parsedargs[1][i];
			}
			for(int i=ind+1;parsedargs[1][i]!='\0';i++){
				if(parsedargs[1][i]==']'){
					valvar[i-ind-1]='\0';
					break;
				}
				valvar[i-ind-1]=parsedargs[1][i];
			}

			if(setenv(envvar, valvar, 1)==-1){
				perror("No such environment variable\n");
			}
		}
		else if(strcmp(Built_in_commands[5], args[commandindexlower])==0){
			unsetenv(parsedargs[1]);
		}

		// fg
		if(strcmp("fg", args[commandindexlower])==0){
			int pid=fg(args[commandindexlower+1]), status;
			signal(SIGINT, signalhandler);
			signal(SIGTSTP, signalhandler);
				
			do{
				waitpid(pid, &status, WUNTRACED);
			}while(!WIFEXITED(status) && !WIFSIGNALED(status) && keeprunning);
			return;
		}

		// Creating child process
		// printf("pi\n");
		int pid = fork(), status;
	
		if(pid == 0){
			// Checking if a built in command


			for(int i=1;i<NUM_BUILT_IN;i++){
				if(strcmp(Built_in_commands[i], args[commandindexlower])==0){
					if(i==1){
						echocommand(args, notreqind, commandindexlower, commandindexupper, previouspipeoutput);
						exit(0);
					}
					else if(i==2){
						pwdcommand();
						exit(0);
					}
					else if(i==3){
						pinfocommand(parsedargs[1]);
						exit(0);
					}
					else if(i==6){
						jobscommand();
						exit(0);
					}
					else if(i==7){
						kjobcommand(args[commandindexlower+1], args[commandindexlower+2]);
						exit(0);
					}
					else if(i==9){
						bg(args[commandindexlower+1]);
						exit(0);
					}
					else if(i==10){
						for(int i=0;i<1000;i++){
							if(bgprocarr[i].name!=NULL){
								kill(bgprocarr[i].pid, 9);
							}
						}
						exit(0);
					}
					return;
				}
			}

			// Checking for ls
			if(args[commandindexlower][0] == 'l' && args[commandindexlower][1] == 's' && args[commandindexlower][2]=='\0'){
				lscommand(parsedargs[0], parsedargs[1]);
				exit(0);
			}

			if(execvp(args[commandindexlower], &args[commandindexlower])==-1){	
				fprintf(stderr, "Invalid command\n");
				exit(0);
			}

			// Final check
			exit(0);

		}
		else{
			// Waiting for completion of child because its a foreground process
			if(bgproc==0){
				signal(SIGINT, signalhandler);
				signal(SIGTSTP, signalhandler);
				
				do{
					waitpid(pid, &status, WUNTRACED);
				}while(!WIFEXITED(status) && !WIFSIGNALED(status) && keeprunning);
			}
			else{
				for(int i=0;i<1000;i++){
					if(bgprocarr[i].name==NULL){
						bgprocarr[i].pid = pid;
						bgprocarr[i].name = (char*)malloc(sizeof(char)*100);
						int pos = 0;
						while(args[0][pos]!='\0'){
							bgprocarr[i].name[pos]=args[0][pos];
							pos++;
						}
						bgprocarr[i].name[pos]='\0';
						break;
					}
				}
				printf("%d\n", pid);
			}
		}

		// Restoring stdin and stdouts
		dup2(GLOBALSTDIN, 0);
		dup2(GLOBALSTDOUT, 1);
	}
	free(previouspipeoutput);
}