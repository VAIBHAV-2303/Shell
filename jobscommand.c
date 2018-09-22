#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<dirent.h>
#include<string.h>
#include<fcntl.h>

#include"shell.h"

typedef struct Filetime{
	int pid;
	long long time;
	char name[100];
	char state;
}filetime;

int compare(const void* a, const void* b){
	filetime *filetimeA = (filetime*)a;
	filetime *filetimeB = (filetime*)b;
	return (filetimeA->time - filetimeB->time);
}

void jobscommand(){

	filetime ftimearr[1000];

	DIR *mydir;
	struct dirent* myfile; 
	mydir = opendir("/proc");
	int fd, t=0;
	
	while((myfile = readdir(mydir))!=NULL){
		if(myfile->d_name[0]>='1' && myfile->d_name[0]<='9'){
			sscanf(myfile->d_name, "%d", &ftimearr[t].pid);
			
			char finalpath[100];
			finalpath[0]='\0';

			strcat(finalpath, "/proc/");
			strcat(finalpath, myfile->d_name);
			strcat(finalpath, "/stat");

			fd = open(finalpath, O_RDONLY);

			char buffer[1000];
			buffer[0]='\0';
			read(fd, buffer, 1000);


			int count=0, i=0;

			// 3rd column contains the state
			while(count!=2){
				if(buffer[i]==' '){
					count++;
				}
				i++;
			}

			ftimearr[t].state = buffer[i];

			// 22nd column contains the time
			while(count!=21){
				if(buffer[i]==' '){
					count++;
				}
				i++;
			}
			char timestring[20];
			timestring[0]='\0';
			int j=0;
			while(buffer[i]!=' '){
				timestring[j]=buffer[i];
				i++;
				j++;
			}
			timestring[j]='\0';

			sscanf(timestring, "%lld", &ftimearr[t].time);

			finalpath[0]='\0';
			strcat(finalpath, "/proc/");
			strcat(finalpath, myfile->d_name);
			strcat(finalpath, "/cmdline");

			fd = open(finalpath, O_RDONLY);
			
			ftimearr[t].name[0]='\0';
			read(fd, ftimearr[t].name, 100);
			t++;
		} 
	}

	qsort(ftimearr, t, sizeof(filetime), compare);

	for(int i=0;i<t;i++){
		printf("[%d] ", i+1);
		printf("%c ", ftimearr[i].state);
		printf("%s ", ftimearr[i].name);
		printf("%d\n", ftimearr[i].pid);
	}
}