/***************************************************************************
 *   Copyright (C) 2005 by root   *
 *   root@Nirvana   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "fork.h"
#include "msg.h"
#include "sort.h"
#include <stdarg.h>

int nr_running = 0;
int number_of_tasks = 0; //gives to scheduler knowledge how much work
int debug = 0; //4 debug

extern struct Lab_msg_queue *root_msg_queue;
extern struct msg_receiver *root_msg_reciever;

struct process *current_proc = NULL;
struct process *root_process = NULL;

struct process *Find_process(int pid)  //finding link to process with pid 
{
    struct process *prc = root_process;

    if (debug == 1)
            printf("\tin Find_process\n\t");
    while(prc != NULL)
    {
        if (debug == 1)
                printf("  pid = %d", prc->pid);
        if (prc->pid == pid)
            return prc;
        prc = prc->next;
    }

    if (debug == 1)
            printf("\nnothing found\n");
    return NULL;
}

void print_task_type(int task_type,int pid)
{
        printf("\nProcess with pid=%d got new task. ",pid);
        printf("Task number %d have type ",number_of_tasks);
        switch (task_type)
        {
                case 0:printf("msgsnd\n");break;
                case 1:printf("msgrcv\n");break;
                case 2:printf("msgget\n");break;
        }
        return;
}

int fork_p(int pid, int uid, int gid, int prio)
{
    struct process *prc = NULL;

    prc = Find_process(pid);
    if(NULL==prc)
    {
        prc = (struct process *)malloc(sizeof(struct process));
        if(NULL==prc)
        {
            printf("cannot alloc mem\n");
            return -1;
        }
        prc->dscrptr = NULL;
        prc->code = NULL;
        prc->pid = pid;
        prc->uid = uid;
        prc->gid = gid;
        prc->prio = prio;
        prc->runned = 0;
        prc->run_time = 0;
        prc->search_msg = 0;
        prc->run = 1;
        /* New process is situated before first process (stack) */
        prc->next = root_process;

        /* New process becomes the first process */
        root_process = prc;

        nr_running++;

        /* New process becomes the current process */
        current_proc = prc;
    }
    else
    {
        printf("tsk fork: ");
        printf("we have process with pid = %d\n",pid);

        return (-1);
    }

    //printf("time create process with pid %d is %d\n",pid, *timeptr);
    return 0;
}

int AddCode(int num,...)
{
    int i;

    /* Using va_ functions from stdvar.h gives us OS and arch independence */
    /* In future we can remove unnecessary num variable                    */
    va_list ap;
    int *pp;
    struct process *prc = NULL;
    struct tsk *task = NULL;
    struct tsk *tsk_temp = NULL;

    va_start(ap,num);
    pp = &num;

    task = (struct tsk *)malloc(sizeof(struct tsk));
    if(task == NULL)
    {
        printf("cannot alloc mem\n");
        return -1;
    }
        number_of_tasks++;
    //prc = Find_process(*(++pp));
    prc = current_proc;
    task->tsk = va_arg(ap,int);
    print_task_type(task->tsk,current_proc->pid);  //printing number and  type of new task and process pid 
   // printf("\ttask number %d\n", task->tsk);
    task->num_param = num - 1;
    for(i=1;i<num-1;i++)
    {
        task->param[i]=va_arg(ap,int);
        printf("\t\tadded param %d = %d\n",i, task->param[i]);
    }
//this is not absolutly good thing but it works
//if this sending task then copy link to text else copy last int param
//also it can be we can write it like this
//if (num==4). maybe it will be better
    if (task->tsk==MSGSND)
        {
        task->text=va_arg(ap,char*);
        printf("\t\tadded msg text  = %s\n", task->text);
        }
    else
        {
            task->text=NULL;
            task->param[i]=va_arg(ap,int);
        }
    task->next = NULL;
        //adding new task at the end of stack
	tsk_temp = prc->code;
    va_end(ap);
    while (tsk_temp != NULL)
    {
        if(tsk_temp->next == NULL)
        {
            tsk_temp->next = task;
            //prc->code->next = task;
            return 0;
        }
        tsk_temp = tsk_temp->next;
        //prc->code = prc->code->next;
    }
        //if there are no tasks this will be the first
    prc->code = task;
    return 0;
}

void RemoveCode(struct process *prc)
{
    struct tsk *tsk_h = prc->code;

    printf("\ttask executed, remove it\n");
    prc->code = prc->code->next;
    free(tsk_h);

    number_of_tasks--;
}

int Add2proc_dscrptr(int msgid)
{
    struct descriptor *desc = NULL;
    struct descriptor *dsc = current_proc->dscrptr;


    desc = (struct descriptor *)malloc(sizeof(struct descriptor));
    if(desc == NULL)
    {
        printf("cannot alloc mem\n");
        return -1;
    }
    desc->descrptr = msgid;
    desc->next = NULL;

    if (current_proc->dscrptr==NULL)
    {
        current_proc->dscrptr=desc;
        return 0;
    }
    while(dsc->next!=NULL)
        dsc=dsc->next;
    dsc->next= desc;

    return 0;
}

int ExecCode(struct process *prc)
{
    int result;
    if(prc->code->tsk == MSGSND)
    {
        printf("exec msgsnd\n");
        result = Lab_sys_msgsnd(prc->code->param[1], prc->code->param[2],prc->code->text);
        if(result < 0)
        {
            printf("mistake in msgrcv\n");
            return -1;
        }
        RemoveCode(prc);
    }
    else if(prc->code->tsk == MSGRCV)
    {
        printf("before Sort\n");
        sort_msg(0);
        printf("exec msgrcv\n");
        result = Lab_sys_msgrcv(prc->code->param[1], prc->code->param[2]);
        if(result < 0)
        {
            printf("mistake in msgrcv\n");
            return -1;
        }
        else if (result == 1)
            ;
        else
            RemoveCode(prc);
    }
    else if(prc->code->tsk == MSGGET)
    {
        printf("exec msgget\n");
        if (Lab_sys_msgget(prc->code->param[1], prc->code->param[2]) < 0)
        {
            printf("mistake in msgget\n");
            return -1;
        }
        //prc->dscrptr = current_proc->dscrptr; //????? what this means?????
        RemoveCode(prc);
    }
    else
    {

        return -1;
    }


    return 0;
}

struct process *Find_max_prio(void)
{
    struct process *max_prc = NULL;
    int max = -1;
    struct process *list = root_process;
        /*if there are no runned processes 
           it will bring them up
           and  count them
        */
    if(nr_running == 0)
    {
        while(list != NULL)
        {
            //I don't now what this thing have to do but without this thing prog works
            /*if(root_msg_reciever != NULL)
              {
              if(list->pid == root_msg_reciever->r_prc->pid)
              ;
              }
              else*/
            {
                list->runned = 0;
                nr_running++;
                list = list->next;
            }
        }
    }

    list = root_process;

        //finding process with max prio

    while(list != NULL)
    {
        if(list->runned != 1)
        {
            if(list->prio > max)
            {
                max = list->prio;
                max_prc = list;
            }
        }
        list = list->next;
    }

    max_prc->runned = 1;
    nr_running--;
    return max_prc;
}

