#include<stdio.h>
#include<stdlib.h>

#include"shell.h"

void allocationerror(){
	perror("ALLOCATION ERROR!");
	exit(1);
}