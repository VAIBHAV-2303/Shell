#ifndef shell_h
#define shell_h

#define HOSTNAME_SIZE 100
#define CWD_SIZE 300
#define PATH_SIZE 300
#define COMMANDLINE_SIZE 1000
#define COMMAND_SIZE 100
#define NUM_OF_ARGS 30
#define ARG_SIZE 30
#define NUM_BUILT_IN 11

// Global root path for my shell

typedef struct Pair{
	int pid;
	char* name;
}pair;

pair* bgprocarr;

int pipedes[2], piping, prevpiping, GLOBALSTDIN, GLOBALSTDOUT, pipedeskjob[2], pipedesfg[2], pipedesbg[2];

char *homepath;

void allocationerror();

void prompt();

char* readcommandline();

char** splitcommands(char* commandline);

char** splitintoargs(char* singlecommnd);

char* catargs(char** args, int notreqind, int upperind, int lowerind, char* previouspipedoutput);

char* catflags(char** args, int notreqind, int upperind, int lowerind);

void cdcommand(char* args);

void echocommand(char** args, int notreqind, int lowerind, int upperind, char* previouspipedoutput);

void pwdcommand();

void lscommand(char* flags, char* path);

void remindmefn(char** args, int lowerind);

void clockfn(char** args, int lowerind);

void pinfocommand(char* args);

void jobscommand();

void kjobcommand(char* args1, char* args2);

void bg(char* args);

int fg(char* args);

void execute(char** args);

#endif