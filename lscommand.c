#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<dirent.h>
#include<string.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>

#include"shell.h"

void lscommand(char* flags, char* path){
	DIR *mydir;
	struct dirent *myfile;
	struct stat st;
	char* cwd = (char*)malloc(sizeof(char)*CWD_SIZE);
	int file = 0;

	if(!cwd){
		allocationerror();
	}

	char* buffer = (char*)malloc(sizeof(char)*500);

	if(!buffer){
		allocationerror();
	}

	if(path==NULL || path[0]=='\0'){
		mydir = opendir(".");
		cwd[0] = '.';
		cwd[1] = '\0';
	}
	else{
		if(path[0]=='~'){
			char* aux = (char*)malloc(sizeof(char)*500);
			if(!aux){
				allocationerror();
			}
			aux[0]='\0';
			strcat(aux, homepath);
			strcat(aux, &path[1]);
			stat(aux, &st);
			if(S_ISREG(st.st_mode)){
				file=1;
			}
			mydir = opendir(aux);
			cwd = aux;
		}
		else{
			stat(path, &st);
			if(S_ISREG(st.st_mode)){
				file=1;
			}
			mydir = opendir(path);
			if(path[0]=='/'){
				cwd[0] = '\0';
			}
			else{
				cwd[0] = '.';
				cwd[1] = '/';
				for(int i=0;i<strlen(path);i++){
					cwd[2+i] = path[i];
				}
				cwd[strlen(path)+2] = '\0';
			}
		}
	}

	if(file){
		if(flags[0]=='-' && (flags[1]=='l' || flags[2]=='l')){
			if(S_ISREG(st.st_mode)){
	       		printf("-");
	       	}
	       	else if(S_ISDIR(st.st_mode)){
	       		printf("d");
	   	    }
	       	else if(S_ISLNK(st.st_mode)){
	       		printf("l");
	       	}
	       	else{
	       		printf("s");
	       	}


	        printf(st.st_mode & S_IRUSR?"r":"-");
	        printf(st.st_mode & S_IWUSR?"w":"-");
	        printf(st.st_mode & S_IXUSR?"x":"-");
	        printf(st.st_mode & S_IRGRP?"r":"-");
	        printf(st.st_mode & S_IWGRP?"w":"-");
	        printf(st.st_mode & S_IXGRP?"x":"-");
	        printf(st.st_mode & S_IROTH?"r":"-");
	        printf(st.st_mode & S_IWOTH?"w":"-");
	        printf(st.st_mode & S_IXOTH?"x":"-");

	        printf(" %lu", st.st_nlink);

	        printf(" %s", getpwuid(st.st_uid)->pw_name);
	        printf("\t%s", getgrgid(st.st_gid)->gr_name);

	        printf("\t%ld", st.st_size);

		    char* date = (char*)malloc(sizeof(char)*20);
		    strftime(date, 20, "%x - %I:%M%p", localtime(&(st.st_ctime)));
		    printf("\t%s", date);
		    free(date);
		    printf("\t%s\n", path);
		}
		else if(flags == NULL || flags[0] == '\0' || (flags[0]=='-' && (flags[1]=='a' || flags[2]=='a'))){
			printf("%s\n", path);
		}
		return;
	}

	if(flags == NULL || flags[0] == '\0'){
		while((myfile = readdir(mydir))!=NULL){
	        if((myfile->d_name)[0]!='.'){
        		printf(" %s ", myfile->d_name);
	        }
		}
		printf("\n");
	}
	else if(flags[0]=='-' && ((flags[1]=='l' && flags[2]=='a') || (flags[1]=='a' && flags[2]=='l'))){
		// Complete long listing
		while((myfile = readdir(mydir))!=NULL){
			sprintf(buffer, "%s/%s", cwd, myfile->d_name);
			stat(buffer, &st);

	        if(S_ISREG(st.st_mode)){
	        	printf("-");
	        }
	        else if(S_ISDIR(st.st_mode)){
	        	printf("d");
	        }
	        else if(S_ISLNK(st.st_mode)){
	        	printf("l");
	        }
	        else{
	        	printf("s");
	        }

	        printf(st.st_mode & S_IRUSR?"r":"-");
	        printf(st.st_mode & S_IWUSR?"w":"-");
	        printf(st.st_mode & S_IXUSR?"x":"-");
	        printf(st.st_mode & S_IRGRP?"r":"-");
	        printf(st.st_mode & S_IWGRP?"w":"-");
	        printf(st.st_mode & S_IXGRP?"x":"-");
	        printf(st.st_mode & S_IROTH?"r":"-");
	        printf(st.st_mode & S_IWOTH?"w":"-");
	        printf(st.st_mode & S_IXOTH?"x":"-");

	        printf(" %lu", st.st_nlink);

	        printf(" %s", getpwuid(st.st_uid)->pw_name);
	        printf("\t%s", getgrgid(st.st_gid)->gr_name);

	        printf("\t%ld", st.st_size);

	        char* date = (char*)malloc(sizeof(char)*20);
	        strftime(date, 20, "%x - %I:%M%p", localtime(&(st.st_ctime)));
	        printf("\t%s", date);
	        free(date);

        	printf("\t%s\n", myfile->d_name);
		}
	}
	else if(flags[0]=='-' && flags[1]=='a'){
		while((myfile = readdir(mydir))!=NULL){    
        	printf(" %s ", myfile->d_name);
		}
		printf("\n");
	}
	else if(flags[0]=='-' && flags[1]=='l'){
		while((myfile = readdir(mydir))!=NULL){
			if((myfile->d_name)[0]!='.'){
				sprintf(buffer, "%s/%s", cwd, myfile->d_name);
		        stat(buffer, &st);

		        if(S_ISREG(st.st_mode)){
	        		printf("-");
	        	}
	        	else if(S_ISDIR(st.st_mode)){
	        		printf("d");
	    	    }
	        	else if(S_ISLNK(st.st_mode)){
	        		printf("l");
	        	}
	        	else{
	        		printf("s");
	        	}


		        printf(st.st_mode & S_IRUSR?"r":"-");
		        printf(st.st_mode & S_IWUSR?"w":"-");
		        printf(st.st_mode & S_IXUSR?"x":"-");
		        printf(st.st_mode & S_IRGRP?"r":"-");
		        printf(st.st_mode & S_IWGRP?"w":"-");
		        printf(st.st_mode & S_IXGRP?"x":"-");
		        printf(st.st_mode & S_IROTH?"r":"-");
		        printf(st.st_mode & S_IWOTH?"w":"-");
		        printf(st.st_mode & S_IXOTH?"x":"-");

		        printf(" %lu", st.st_nlink);

		        printf(" %s", getpwuid(st.st_uid)->pw_name);
		        printf("\t%s", getgrgid(st.st_gid)->gr_name);

		        printf("\t%ld", st.st_size);

		        char* date = (char*)malloc(sizeof(char)*20);
		        strftime(date, 20, "%x - %I:%M%p", localtime(&(st.st_ctime)));
		        printf("\t%s", date);
		        free(date);

	        	printf("\t%s\n", myfile->d_name);
			}
		}
	}
	else{
		perror("Invalid format!");
	}
	closedir(mydir);
	free(buffer);
}