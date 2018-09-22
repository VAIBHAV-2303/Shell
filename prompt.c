#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<limits.h>

#include"shell.h"

void prompt(){
	char* username = getenv("USER");
	char *hostname, *cwd, *path;
	hostname = (char*)malloc(sizeof(char)*HOSTNAME_SIZE);

	if(!hostname){
		allocationerror();
	}

	cwd = (char*)malloc(sizeof(char)*CWD_SIZE);

	if(!cwd){
		allocationerror();
	}

	path = (char*)malloc(sizeof(char)*PATH_SIZE);

	if(!path){
		allocationerror();
	}

	getcwd(cwd, CWD_SIZE);

	if(strlen(cwd) == strlen(homepath) && strcmp(cwd, homepath)==0){
		path[0]='~';
		path[1]='\0';
	}
	else if(strlen(cwd)<=strlen(homepath)){
		for(int i=0;i<strlen(cwd);i++){
			path[i]=cwd[i];
		}
		path[strlen(cwd)]='\0';
	}
	else{
		int flag=0;
		for(int i=0;i<strlen(homepath);i++){
			if(cwd[i]!=homepath[i]){
				flag=1;
			}
		}
		
		if(flag){
			for(int i=0;i<strlen(cwd);i++){
				path[i]=cwd[i];
			}
			path[strlen(cwd)]=='\0';
		}
		else{
			path[0]='~';
			for(int i=strlen(homepath);i<strlen(cwd);i++){
				path[i-strlen(homepath)+1]=cwd[i];
			}
			path[strlen(cwd)-strlen(homepath)+1]='\0';
		}
	}

	gethostname(hostname, HOSTNAME_SIZE);
	printf("<%s@%s:%s>", username, hostname, path);
	return;
}