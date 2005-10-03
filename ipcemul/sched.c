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
#include "time.h"
extern struct process *root_process;
extern struct process *current_proc;
extern struct timespec *NULL_time;
int scheduler(void)
{
    struct timespec timevalue ,begin ;
    struct process *prc_add_time = root_process;
    struct process *prc;

    prc = Find_max_prio();

    current_proc = prc;
    
    if (clock_gettime(CLOCK_THREAD_CPUTIME_ID, &begin)!=0)
	printf("Error getting time\n");
    
    printf("\nbegin work process with pid %d  at %9.2f\n",prc->pid,time_substr(*NULL_time,begin));

    if(prc->code == NULL)
    {
        printf("NULL proc\n");
    }
    else
    {
        ExecCode(prc);
    }
//temporary not used, it need to be updated
  /*  while(prc_add_time != NULL)
    {
        prc_add_time->run_time+=1;
        prc_add_time = prc_add_time->next;
    }
*/
    if (clock_gettime(CLOCK_THREAD_CPUTIME_ID, &timevalue)!=0)
                printf("Error getting time\n");

    
    printf("end work process with pid %d at %9.2f \n",prc->pid,time_substr(*NULL_time,timevalue));

    printf("time in work is %9.2f mcsec\n\n",time_substr(begin,timevalue));

    return 0;
}
