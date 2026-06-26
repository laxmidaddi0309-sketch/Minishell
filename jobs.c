#include "minishell.h"
#include<stdio.h>
#include<string.h>
Node *head=NULL;
int job_id=1;
extern char input_str[50];
void insert_first(int pid)
{
    Node *new=malloc(sizeof(Node));

    if(new==NULL)
    {
        perror("malloc");
        return;
    }

    new->job_id = job_id++;
    new->pid = pid;
    strcpy(new->command,input_str);

    new->next=head;
    head = new;
}

void delete_first(void)
{
	if(head == NULL)
    {
        printf("No jobs available\n");
        return;
    }

    Node *temp = head;
    head = head->next;
    free(temp);
}

void print_list(void)
{
	if(head == NULL)
    {
        printf("No jobs found\n");
        return;
    }

    Node *temp = head;

    printf("Job_ID\tPID\tCOMMAND\n");

    while(temp != NULL)
    {
        printf("[%d]\t%d\t%s\n",temp->job_id,temp->pid,temp->command);
	temp = temp->next;
    }
}
