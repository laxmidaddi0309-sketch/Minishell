#include"minishell.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include<unistd.h>
#include<sys/wait.h>


//char *external_commands[154];
extern char prompt_str[50];
static char input_buf[260];
int pid=0;
extern int status;
void signal_handler(int signum)
{
	if(signum==SIGINT)
	{
		if(pid==0)
		{
			printf("%s\n",prompt_str);
			fflush(stdout);
		}
	}
	else if(signum==SIGTSTP)
	{
		if(pid==0)
		{
			printf("%s\n",prompt_str);
			fflush(stdout);
		}
		else
		{
			//insert first(logic);for storing stopped process
			insert_first(pid);
			//printf("\n[Stopped]\n");
			printf("\n[%d]+ Stopped\t%s\n",head->job_id,head->command);
		}
	}
	else if(signum==SIGCHLD)
	{
		//how to cleae the resource
		//waitpid();
		while (waitpid(-1,&status,WNOHANG)>0);
	}
}
void scan_input(char *prompt_str,char *input_str)
{
	//without command 
	signal(SIGINT,signal_handler);
	signal(SIGTSTP,signal_handler);

	
	while(1)
	{
		printf(ANSI_COLOR_GREEN "%s" ANSI_COLOR_RESET, prompt_str);
		scanf("%[^\n]",input_str);
		getchar();

		//step1: check the input string contain ps1 or not
		if(strncmp(input_str,"PS1=",4)==0)
		{

		//step2: if it is PS1=
		////check the input_str[4]is space or not
			if(input_str[4]!=' ')
			{

			//if no space 
			//from 4th index we need copy
			//if space is present 
				strcpy(prompt_str,input_str+4);
			}
			else
			{
				//print the error msg
				printf("ERROR: Space not allowed after PS1=\n");
			}
		}

		else
		{

			//if it is not PS1=
			//char *ptr=get_command(intput_str);
			char *ptr=get_command(input_str);
			 //printf("command=%s\n",ptr);
			//int ret=check_command_type(ptr);
			int ret=check_command_type(ptr);
			//printf("ret=%d\n",ret);
			if(ret==BUILTIN)
			{
				//logic for internal command
				execute_internal_commands(input_str);
			}
			
			else if(ret==EXTERNAL)
			{
				//LOGIC FOR EXTERNAL COMMAND
				//create child process pid=fork();
				pid=fork();
				if(pid>0)
				{
					waitpid(pid,&status,WUNTRACED);
				}
				else if(pid==0)
				{
					signal(SIGINT,SIG_DFL);
					signal(SIGTSTP,SIG_DFL);
					execute_external_commands(input_str);
				}
			}
			
			else
			//print error msg
			printf("ERROR: command not found\n");
		}

	}
}



