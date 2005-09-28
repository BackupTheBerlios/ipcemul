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
#include "time.h"
#include <time.h>

extern struct task *root_task;
extern struct task *current_proc;
//extern int *timeptr;

int scheduler(void)
{
    struct timespec *timevalue;
    struct task *tsk_add_time = root_task;
    struct task *tsk;
    long begin;

    timevalue = (struct timespec *)malloc(sizeof(struct timespec));

    tsk = Find_max_prio();

    current_proc = tsk;

    usleep(300);
    
    if (clock_gettime(CLOCK_REALTIME, timevalue)!=0)
        printf("Error getting time\n");
    
    begin=((int)(timevalue->tv_sec))*1000000000.0+((int)(timevalue->tv_nsec));
    printf("\nbegin work task with pid %d at time %e\n",tsk->pid, (double)begin);

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

    if (clock_gettime(CLOCK_REALTIME, timevalue)!=0)
                printf("Error getting time\n");
    
    printf("end work task with pid %d at time %e\n",tsk->pid, ((int)(timevalue->tv_sec))*1000000000.0+((int)(timevalue->tv_nsec)));
    printf("time in work is %e\n\n",((int)(timevalue->tv_sec))*1000000000.0+((int)(timevalue->tv_nsec))-begin);

    return 0;
}
