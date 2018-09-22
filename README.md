# MyShell

## Description
This is a shell program written solely on C in which semicolon sperated commands are accepted. The program is driven by an infinite loop in which first an entire line is taken as input which is in turn broken into individual commands and then broken into arguments. Then execute function is called in which built in commands are seperated and the remaining commands are executed using execvp system call. ls is implemented mainly using stat and dirent. A global array is maintained to handle the process names of currently running background processes. Piping and input/output redirection is also handled in the execute function itself. Appropriate signal handlers have been used.

## To run
```console
bar@foo:~$ make
bar@foo:~$ ./Shell
```

## To exit
```console
bar@foo:~$ quit
```

## Bonus
* Reminder function was implemented using sleep call and forking. 
* The clock function was implemented using the system RTC files. The command can be terminated using CTRL+C or CTRL+Z.
* Completely modular code.

## Author
Vaibhav Garg(Roll No. 20171005)