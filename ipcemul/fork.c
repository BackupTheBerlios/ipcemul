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
#include "time.h"

int nr_running = 0;

extern struct Lab_msg_queue *R_ipc;

extern struct msg_receiver *R_msg_r; 

//extern int *timeptr;

struct task *current_proc = NULL;

struct task *root_task = NULL;

struct task *Find_task(int pid)
{
    struct task *tsk = root_task;

    usleep(50);
    while(tsk != NULL)
    {
        if (tsk->pid == pid)
            return tsk;
        tsk = tsk->next;
    }

    return NULL;
}

int fork_p(int pid, int uid, int gid, int prio)
{
    struct task *tsk = NULL;

    tsk = Find_task(pid);
    if(tsk == NULL)
    {
        tsk = (struct task *)malloc(sizeof(struct task));
        if(tsk == NULL)
        {
            printf("cannot alloc mem\n");
            return -1;
        }
        tsk->dscrptr = NULL;
        tsk->code = NULL;
        tsk->pid = pid;
        tsk->uid = uid;
        tsk->gid = gid;
        tsk->prio = prio;
        tsk->runned = 0;
        tsk->run_time = 0;
        tsk->search_msg = 0;

        /* New task is situated before first task (stack) */
        tsk->next = root_task;

        /* New task becomes the first task */
        root_task = tsk;

        nr_running++;

        /* New task becomes the current task */
        current_proc = tsk;
    }
    else
    {
        printf("func fork: ");
        printf("we have process with pid = %d\n",pid);
    }

    usleep(50);
    //printf("time create process with pid %d is %d\n",pid, *timeptr);
    return 0;
}

int AddCode(int num,...)
{
    int i;
    int *pp = &num;
    struct task *tsk = NULL;
    struct func *function = NULL;
    struct func *add_code = NULL; 

    function = (struct func *)malloc(sizeof(struct func));
    if(function == NULL)
    {
        printf("cannot alloc mem\n");
        return -1;
    }

    //tsk = Find_task(*(++pp));
    tsk = current_proc;

    function->func = *(++pp);
    function->num_param = num - 1;
    for (i=1;i<num;i++)
        function->param[i]=*(++pp);
    function->next = NULL;

    add_code = tsk->code;
    while (add_code != NULL)
    {
        if(add_code->next == NULL)
        {
            add_code->next = function;
            return 0;
        }
        add_code = add_code->next;
    }
    tsk->code = function;

    return 0;
}

void RemoveCode(struct task *tsk)
{
    struct func *func_h = tsk->code;
    tsk->code = tsk->code->next;
    free(func_h);
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
    dsc= desc;

    return 0;
}

int ExecCode(struct task *tsk)
{
    int result;

    //	OneStringToProtocol("\tin ExecCode");
    if(tsk->code->func == 0)
    {
        result = Lab_sys_msgsnd(tsk->code->param[1], tsk->code->param[2]);
        if(result < 0)
        {
            printf("mistake in msgrcv\n");
            return -1;
        }
        RemoveCode(tsk);
    }
    else if(tsk->code->func == 1)
    {
        result = Lab_sys_msgrcv(tsk->code->param[1], tsk->code->param[2]);
        if(result < 0)
        {
            printf("mistake in msgrcv\n");
            return -1;
        }
        else if (result == 1)
            ;
        else
            RemoveCode(tsk);
    }
    else if(tsk->code->func == 2)
    {
        if (Lab_sys_msgget(tsk->code->param[1]) < 0)
        {
            printf("mistake in msgget\n");
            return -1;
        }
        tsk->dscrptr = current_proc->dscrptr;
        RemoveCode(tsk);
    }
    else
        return -1;

    usleep(100);

    return 0;
}

struct task *Find_max_prio(void)
{
    struct task *max_tsk = NULL;
    int max = -1;
    struct task *list = root_task;

    if(nr_running == 0)
    {
        while(list != NULL)
        {
            //I don't now what this thing have to do but without this thing prog works
            /*if(R_msg_r != NULL)
              {
              if(list->pid == R_msg_r->r_tsk->pid)
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

    list = root_task;

    while(list != NULL)
    {
        if(list->runned != 1)
        {
            if(list->prio > max)
            {
                max = list->prio;
                max_tsk = list;
            }
        }
        list = list->next;
    }

    max_tsk->runned = 1;
    nr_running--;
    return max_tsk;
}
