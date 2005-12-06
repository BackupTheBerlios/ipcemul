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
#include "msg.h"
#include "fork.h"

extern struct msg_msg *root_msg_msg;
extern struct process *current_proc;

int generator_msgs(int pid, int begin, int end, int key)
{
//      struct msg_msg *msg = NULL;
        struct process *proc;

        if ((proc = Find_process(pid)) == NULL)
        {
                printf("no process with pid %d\n", pid);
                return -1;
        }
        else
        {
                current_proc = proc;

                if (Lab_msgget(key, IPC_CREAT) == -1)
                {
                        printf("cannot create queue\n");
                        return -1;
                }

                while (end >= begin)
                {
                        Lab_msgsnd(end, 1,"its genrated msg");

                        end--;
                }
        }

        return 0;
}

int generator_procs(int begin, int end, int mode)
{
        int result;
        int rand_pid;
        int rand_uid, rand_gid, rand_prio;

        if (mode == 1) printf("in manual mode of creating processes\n");

        while(end >= begin)
        {
                if (mode == 0)
                {
                        rand_pid = 1+(int) (10.0*rand()/(RAND_MAX+1.0));
                        rand_uid = 1+(int) (10.0*rand()/(RAND_MAX+1.0));
                        rand_gid = 1+(int) (10.0*rand()/(RAND_MAX+1.0));
                        rand_prio = 1+(int) (10.0*rand()/(RAND_MAX+1.0));
                }
                else if (mode == 1)
                {
//                      printf("in manual mode\n");

                        printf("Enter pid of process: ");
                        scanf("%d", &rand_pid);

                        printf("Enter uid of process: ");
                        scanf("%d", &rand_uid);

                        printf("Enter gid of process: ");
                        scanf("%d", &rand_gid);

                        printf("Enter prio of process: ");
                        scanf("%d", &rand_prio);
                }

                if (0 != (result = fork_p(rand_pid, rand_uid, rand_gid, rand_prio)))
                {
                    printf("error in creating process\n");
                    end++; //this give to user chance input another parameters
                }
                else
                {
                    printf("we have created process with pid=%d uid=%d gid=%d prio=%d\n",rand_pid,rand_uid,rand_gid,rand_prio);
                }

                end--;
        }

        return 0;
}


int make_msgrcv(int pid, int m_type, int msg_flag,int key)  //make task to recive msg(with m_type and msg_flag)  by process with pid 
{
        struct process *proc;

//      printf("in make_msgrcv\n");
        if ((proc = Find_process(pid)) == NULL)
        {
                printf("no process with pid %d\n", pid);
                return -1;
        }
        else
        {
                if (Lab_msgget(key, IPC_CREAT) == -1)
                {
                        printf("cannot create queue\n");
                        return -1;
                }
//              printf("find proc with proc->pid = %d\n", proc->pid);
                current_proc = proc;
                if (Lab_msgrcv(m_type, msg_flag) == -1)
                {
                        printf("\tmistake in make_msgrcv\n");
                        return -1;
                }
        }

        return 0;
}
