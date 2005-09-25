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
#include "fork.h"
#include "protocol.h"

extern struct task *R_task;
extern struct task *current_proc;

int scheduler(void)
{
	struct task *tsk_add_time = R_task;
	struct task *tsk;
	
	tsk = Find_max_prio();
	
	current_proc = tsk;
	
	printf("\nbegin work task with pid %d\n",tsk->pid);
	
	if(tsk->code == NULL)
	{
		printf("NULL proc\n");
	}
	else
	{
		OneStringToProtocol("\tbefore ExecCode");
		ExecCode(tsk);
	}
	
	while(tsk_add_time != NULL)
	{
		tsk_add_time->run_time+=1;
		tsk_add_time = tsk_add_time->next;
	}
	
	printf("end work task with pid %d\n",tsk->pid);
	
	return 0;
}
