CC=gcc

Shell: allocationerror.o prompt.o readcommandline.o splitcommands.o splitintoargs.o catflags.o catargs.o cdcommand.o echocommand.o pwdcommand.o lscommand.o remindmefn.o clockfn.o pinfocommand.o jobscommand.o kjobcommand.o bg.o fg.o execute.o main.o
	$(CC) -g -o Shell $^

clean:
	@rm -f *.o shell

allocationerror.o: shell.h allocationerror.c
	$(CC) -g -c allocationerror.c

prompt.o: shell.h prompt.c
	$(CC) -g -c prompt.c

readcommandline.o: shell.h readcommandline.c
	$(CC) -g -c readcommandline.c

splitcommands.o: shell.h splitcommands.c
	$(CC) -g -c splitcommands.c

splitintoargs.o: shell.h splitintoargs.c
	$(CC) -g -c splitintoargs.c

catflags.o: shell.h catflags.c
	$(CC) -g -c catflags.c

catargs.o: shell.h catargs.c
	$(CC) -g -c catargs.c

cdcommand.o: shell.h cdcommand.c
	$(CC) -g -c cdcommand.c

remindmefn.o: shell.h remindmefn.c
	$(CC) -g -c remindmefn.c

clockfn.o: shell.h clockfn.c
	$(CC) -g -c clockfn.c

echocommand.o: shell.h echocommand.c
	$(CC) -g -c echocommand.c

pwdcommand.o: shell.h pwdcommand.c
	$(CC) -g -c pwdcommand.c

pinfocommand.o: pinfocommand.c
	$(CC) -g -c pinfocommand.c

lscommand.o: shell.h lscommand.c
	$(CC) -g -c lscommand.c

jobscommand.o: shell.h jobscommand.c
	$(CC) -g -c jobscommand.c

kjobcommand.o: shell.h kjobcommand.c
	$(CC) -g -c kjobcommand.c

bg.o: shell.h bg.c
	$(CC) -g -c bg.c

fg.o: shell.h fg.c
	$(CC) -g -c fg.c

execute.o: shell.h execute.c
	$(CC) -g -c execute.c

main.o: shell.h main.c
	$(CC) -g -c main.c
