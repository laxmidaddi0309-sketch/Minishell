/*Name: Laxmi B Daddi
Project Name: MiniShell
Date of Submission:26/6/2026
Description of Project:
The MiniShell project is a simple Linux command-line interpreter. It supports basic shell commands and process management.

1.Command Prompt:Displays the shell prompt and accepts commands from the user.
2.Internal Commands:Supports internal commands like cd, pwd, and exit.
3.External Commands:Executes all external Linux commands using fork() and execvp().
4.Echo Commands:Supports echo $$ to display the shell PID, echo $? to display the previous command status, and echo $SHELL to display the shell path.
5.Jobs:Displays the list of stopped or background jobs.
6.Foreground (fg):Brings a selected job to the foreground.
7.Background (bg):Continues a stopped job in the background.
8.Signal Handling:Handles Ctrl+C and Ctrl+Z to control running processes.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minishell.h"
#include <sys/wait.h>
#include <signal.h>
char prompt_str[]="minishell$:";
char input_str[50];
int main()
{
	
	system("clear");
	extract_external_commands();
	//printf("minishell$:");
	scan_input(prompt_str,input_str);
}
