#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<dirent.h>
#include<string.h>
#include<fcntl.h>

#include"shell.h"

void pinfocommand(char* args){
	DIR *mydir;
	struct dirent* myfile; 
	mydir = opendir("/proc");
	int fd;

	if(args==NULL || args[0]=='\0'){
		while((myfile = readdir(mydir))!=NULL){
			if(myfile->d_name[0]>='1' && myfile->d_name[0]<='9'){
				printf("pid -- %s\n", myfile->d_name);
				char* finalpath = (char*)malloc(sizeof(char)*100);
				if(!finalpath){
					allocationerror();
				}
				finalpath[0]='\0';
				strcat(finalpath, "/proc/");
				strcat(finalpath, myfile->d_name);
				strcat(finalpath, "/status");
			
				fd=open(finalpath, O_RDONLY);

				char* buffer = (char*)malloc(sizeof(char)*1000);
				buffer[0]='\0';
				read(fd, buffer, 1000);
				for(int i=0;i<1000;i++){
					if(buffer[i]=='S' && buffer[i+1]=='t' && buffer[i+2]=='a' && buffer[i+3]=='t' && buffer[i+4]=='e'){
						printf("Process status -- %c\n", buffer[i+7]);
					}
					if(buffer[i]=='V' && buffer[i+1]=='m' && buffer[i+2]=='S' && buffer[i+3]=='i' && buffer[i+4]=='z' && buffer[i+5]=='e'){
						int j=i+8;
						printf("Virtual memory -- ");
						while(buffer[j]!='k'){
							printf("%c", buffer[j]);
							j++;
						}
						printf("\n");
						break;
					}
				}



				finalpath = (char*)malloc(sizeof(char)*100);
				if(!finalpath){
					allocationerror();
				}
				strcat(finalpath, "/proc/");
				strcat(finalpath, myfile->d_name);
				strcat(finalpath, "/exe");
				buffer = (char*)malloc(sizeof(char)*100);
				if(!buffer){
					allocationerror();
				}

				readlink(finalpath, buffer, 100);

				if(buffer[0]=='\0'){
					printf("Executable path - NA\n");
				}
				else{
					printf("Executable path - %s\n", buffer);
				}

				printf("\n");
				free(buffer);
				free(finalpath);
			} 
		}
	}
	else{
		char* dirpath = (char*)malloc(sizeof(char)*100);
		dirpath[0]='\0';
		strcat(dirpath, "/proc/");
		strcat(dirpath, args);
		strcat(dirpath, "/status");
			
		fd=open(dirpath, O_RDONLY);
		// printf("%s\n", dirpath);
		if(fd==-1){
			perror("No such pid exists");
			return;
		}

		char* buffer = (char*)malloc(sizeof(char)*1000);
		read(fd, buffer, 1000);
		for(int i=0;i<1000;i++){
			if(buffer[i]=='S' && buffer[i+1]=='t' && buffer[i+2]=='a' && buffer[i+3]=='t' && buffer[i+4]=='e'){
				printf("Process status -- %c\n", buffer[i+7]);
			}
			if(buffer[i]=='V' && buffer[i+1]=='m' && buffer[i+2]=='S' && buffer[i+3]=='i' && buffer[i+4]=='z' && buffer[i+5]=='e'){
				int j=i+8;
				printf("Virtual memory -- ");
				while(buffer[j]!='k'){
					printf("%c", buffer[j]);
					j++;
				}
				printf("\n");
				break;
			}
		}



		dirpath = (char*)malloc(sizeof(char)*100);
		if(!dirpath){
			allocationerror();
		}
		dirpath[0]='\0';
		strcat(dirpath, "/proc/");
		strcat(dirpath, args);
		strcat(dirpath, "/exe");
		buffer = (char*)malloc(sizeof(char)*100);
		
		if(!buffer){
			allocationerror();
		}
		buffer[0]='\0';
		readlink(dirpath, buffer, 100);

		if(buffer[0]=='\0'){
			printf("Executable path - NA\n");
		}
		else{
			printf("Executable path - %s\n", buffer);
		}

		printf("\n");
		free(buffer);
		free(dirpath);
	}
}