# Mini Shell

# Overview

Mini Shell is a simple Linux command-line interpreter developed in C. It provides basic shell functionality by executing internal and external commands, handling processes, and managing foreground and background jobs.

# Features
1.Execute external Linux commands.
2.Support for internal commands:
  1.cd
  2.pwd
  3.exit
  4.echo $$
  5.echo $?
  6.echo $SHELL
  7.jobs
  8.fg
  9.bg
3.Foreground and background process management.
4.Signal handling using Ctrl + C and Ctrl + Z.
5.Process creation using fork() and command execution using execvp().
6.Displays the exit status of the previously executed command.
7.Displays the current shell process ID.
8.Displays the current shell path using the `$SHELL` environment variable.

# Technologies Used
1.C Programming
2.Linux System Calls
3.Process Management
4.fork()
5.execvp()
6.wait()
7.Signal Handling
8.GCC Compiler

# Project Structure
text
Mini-Shell/
│── main.c
│── command.c
│── execute.c
│── signal.c
│── jobs.c
│── minishell.h
└── README.md

# Compilation
gcc *.c -o minishell
## Run
./minishell

## Sample Execution
minishell$ pwd
/home/user
minishell$ ls
minishell$ cd ..
minishell$ echo $$
minishell$ echo $?
minishell$ echo $SHELL
minishell$ jobs
minishell$ fg
minishell$ bg
minishell$ exit

# Learning Outcomes
1.Learned Linux process creation using fork().
2.Understood command execution using execvp().
3.Implemented internal and external commands.
4.Gained experience in signal handling (SIGINT and SIGTSTP).
5.Learned foreground and background process management.
6.Improved understanding of Linux system calls and shell programming.

# Conclusion
The Mini Shell project demonstrates the basic working of a Linux shell by implementing command execution, process management, signal handling, and job control. It provided practical experience with Linux system programming concepts and strengthened my understanding of operating system fundamentals.



