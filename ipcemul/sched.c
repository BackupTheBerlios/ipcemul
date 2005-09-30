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
#include "fork.h"
#include <unistd.h>
#include <time.h>

extern struct task *root_task;
extern struct task *current_proc;
//extern int *timeptr;

double time_substr(struct timespec x1,struct timespec x2)
{
	return (double)(((x2.tv_sec-x1.tv_sec))*1000000000.0+((x2.tv_nsec-x1.tv_nsec)));
}

int scheduler(void)
{
    struct timespec timevalue ,begin;
    struct task *tsk_add_time = root_task;
    struct task *tsk;

    tsk = Find_max_prio();

    current_proc = tsk;

    
    if (clock_gettime(CLOCK_REALTIME, &begin)!=0)
        printf("Error getting time\n");
    
    printf("\nbegin work task with pid %d\n",tsk->pid);

    if(tsk->code == NULL)
    {
        printf("NULL proc\n");
    }
    else
    {
        ExecCode(tsk);
    }

    while(tsk_add_time != NULL)
    {
        tsk_add_time->run_time+=1;
        tsk_add_time = tsk_add_time->next;
    }

    if (clock_gettime(CLOCK_REALTIME, &timevalue)!=0)
                printf("Error getting time\n");

    
    printf("end work task with pid %d\n",tsk->pid);

    printf("time in work is %18.0f\n\n",time_substr(begin,timevalue));

    return 0;
}
