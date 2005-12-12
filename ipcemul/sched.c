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

extern struct process *root_process;
extern struct process *current_proc;
extern int number_of_tasks;

int check()
{
	int check=0;
	struct process *prc;
	prc=root_process;
	while(prc)
	{
		if( (prc->code) && (prc->run) )
		{
			check=1;
			break;
		};
		prc=prc->next;
	};
	return check;
};

int find_print_who_sleep()
{
	struct process *prc;
	prc=root_process;
	while(prc)
	{
		if( !(prc->run) )
			printf("Process with pid #%d\n",prc->pid);
		prc=prc->next;
	};
	printf("===============================================\n");
	return 0;
};
	

int scheduler(void)
{
	struct process *prc;

	prc = Find_max_prio();

	current_proc = prc;

	if (prc->run)
	{
		printf("\nbegin working on process with pid %d\n",prc->pid);

		if(prc->code == NULL)
		{
			printf("NULL proc(there are no tasks for it)\n");
		}
		else
			ExecCode(prc);

		printf("finished working on process with pid %d\n",prc->pid);
	}

	return 0;
}

int run (void)
{
        printf("\n---------------------------------------------------\n");
        printf("------------------Starting scheduler---------------\n");
        printf("---------------------------------------------------\n");
        while(number_of_tasks != 0)
        {	
		if(!(check())) //if there are only sleeping procs or procs with NULL code
				// so we are to do nothing
		{
			printf("Finished, but there are some sleeping processes:\n");
			find_print_who_sleep();
			break;
			//while(1);
		};
                printf("\n----------- number of tasks left = %d --------", number_of_tasks);
                if ((scheduler()) == 1)
                        break;
        }
	printf("\n---------------------------------------------------\n");
        printf("----------------------Finished-----------------------\n");
        printf("---------------------------------------------------\n");
        return 0;
}
