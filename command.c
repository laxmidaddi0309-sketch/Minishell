#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "minishell.h"
/*builtin commandsi*/
char *builtins[] = {"echo", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs", "let", "eval","jobs","fg","bg",
						"set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source",
						"exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help", NULL};

int status;
char *external_commands[2000];

void extract_external_commands()
{
	//step1 : open the file in read content till \n
	FILE *fp=fopen("shelltext.txt","r");
	if(fp==NULL)
	{
		printf("File is not present\n");
		return;
	}
	char str[50];
	int i=0;
	//step2: take a lenght particular command
	while(fgets(str,50,fp))
	{
		//step3 : allocate the coloumn memory based on length
		//step4 : store the command in external_commands[][] array
		int len=0;
		while(str[len]!='\n' && str[len]!='\0')
		{
			len++;
		}
		external_commands[i]=malloc(len+1);
		int j;
		for(j=0;j<len;j++)
		{
			external_commands[i][j]=str[j];
		}
		external_commands[i][j]='\0';
		i++;
	}
	external_commands[i]=NULL;
	/*printf("Loaded external commands:\n");
    	for (int k=0; external_commands[k] != NULL; k++)
	{
		printf("%s\n", external_commands[k]);
	}*/
	fclose(fp);
}
char *get_command(char *input_str)
{
	static char command[50];
	//fetch the first word-->input_str ==>ls -l==>ls and store in command array and return the address
	int i=0;
	while(input_str[i]!=' ' && input_str[i]!='\0')
	{
		command[i]=input_str[i];
		i++;
	}
	command[i]='\0';

	return command;
}
int check_command_type(char *cmd)
{
	//compare the command with buitin 2d array(index by index)
	int i=0;
	for(i=0;builtins[i]!=NULL;i++)
	{
		//if is matching
		if(strcmp(cmd,builtins[i])==0)
		{
			//return BUILTIN;
			return BUILTIN;
		}
	}
	
	//compare cmd[] with external_command[][] array
	for(i=0;external_commands[i]!=NULL;i++)
	{	
		//if it matching
		/*if (i < 10)
        	printf("[%d] ='%s'\n", i, external_commands[i]);*/
		if(strcmp(cmd,external_commands[i])==0)
		{
			//return EXTERNAL;
			//printf("Matched\n");
			return  EXTERNAL;
		}
	}
       return NO_COMMAND;
		
}
void execute_internal_commands(char *input_str)
{
	/*1.exit
	 2.cd 
	 3.pwd*/

	//check the input string exit or not
	if(strcmp(input_str,"exit")==0)
	{	//if it is exit 
		exit(0);
	}

	//if input contain cd or not
	if (strncmp(input_str,"cd",2)==0)
	{
		if (input_str[2] =='\0')
		{
			chdir(getenv("HOME"));
		}
		else if (input_str[2]==' ')
		{
			if (chdir(input_str + 3)==-1)
				perror("cd");
		
		}
	}
	
	//check the input_str contain pwd or not
	else if(strcmp(input_str,"pwd")==0)
	{
		//if it is
		char buff[50];
		getcwd(buff,50);
		printf("%s\n",buff);
	}

	//check the input string contain echo $$ or not
	else if(strcmp(input_str,"echo $$")==0)
	{
		//if it is true
		//by using getpid();
		printf("%d\n",getpid());
	}
	//check the input string contain echo $? or not
	else if(strcmp(input_str,"echo $?")==0)
	{
		//if it true
		printf("%d\n",WEXITSTATUS(status));
	}
	//check the input string conatin echo echo $shell or not
	else if(strcmp(input_str,"echo $SHELL")==0)
	{
		//if is true
		//print->getenv("shell");
		char *shell=getenv("SHELL");
		if(shell!=NULL)
		{
			printf("%s\n",shell);
		}
	}
	
	//advance commands
	//check the input string contain jobs or not
	else if(strcmp(input_str,"jobs")==0)
	{
		//if it true call the print_list();
		print_list();
	}
	
	//check the input string contain fg or not
	else if(strcmp(input_str,"fg")==0)
	{
		//if it is true
		if(head!=NULL)
		{
			//kill system call -->kill(head->pid,SIGCONT);
			kill(head->pid,SIGCONT);
			//waitpid(pid,&status,WUNTRACED);
			waitpid(head->pid,&status,WUNTRACED);
			//delete_first();
			delete_first();
		}
		else
		{
			printf("NO stopped jobs\n");
		}
	}
	//check the input string contain bg command or not
	else if(strcmp(input_str,"bg")==0)
	{
		//if it is true
		if(head!=NULL)
		{
			kill(head->pid,SIGCONT);
			delete_first();
			signal(SIGCHLD,signal_handler);
		}
		else
		{
			printf("NO stopped jobs\n");
		}
	}

	
}
void execute_external_commands(char *input_str)
{
	//convert 1D array input_str[] to 2d arra[][]
        // extract_external_commands();

        char *argv[20];
        int i = 0;
        argv[i++] = input_str;
        for(int j = 0; input_str[j] != '\0'; j++)
        {
               if(input_str[j]==' ')
	       {
               	        input_str[j] = '\0';
                        if(input_str[j + 1] != '\0')
                        {
                                argv[i++] = &input_str[j + 1];
                        }
                }
        }
        argv[i] = NULL;

        //check the pipe is present or not
        //if there is no pipe
        //execvp(array_name,address of array);
        int pipe_found = 0;
        int cmd_index[20];
        int cmd_count = 1;
        cmd_index[0] = 0;

        for(i = 0; argv[i] != NULL; i++)
	{
                if(strcmp(argv[i], "|") == 0)
                {
                        pipe_found = 1;
                        argv[i] = NULL;
                        cmd_index[cmd_count] = i + 1;
                        cmd_count++;
                }
        }
        if(pipe_found == 0)
        {
                execvp(argv[0], argv);
        }

        //if pipe is present
                //n pipe logic
	else
	{
        	int fd[2];
        	int prev_fd=-1;

        	for(i=0;i<cmd_count;i++)
        	{
                	if(i<cmd_count-1)
               		{
                        	pipe(fd);
                	}

                	int pid=fork();
                	if(pid>0)
                	{
                        	if(prev_fd != -1)
                                close(prev_fd);

                        	if(i < cmd_count - 1)
                        	{
                                	close(fd[1]);
                               		 prev_fd = fd[0];
                        	}
			}
			else if(pid == 0)
                	{
                        	if(prev_fd != -1)
                        	{
                                	dup2(prev_fd, 0);
                                	close(prev_fd);
                        	}
			
                        	if(i<cmd_count - 1)
                        	{
                                	close(fd[0]);
                               		dup2(fd[1], 1);
                                	close(fd[1]);
                        	}

                        	execvp(argv[cmd_index[i]], &argv[cmd_index[i]]);
				perror("execvp");
                        	exit(1);
                	}
        	}

        	while(wait(NULL)>0);
	}
}



















