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
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include "sched.h"
#include "fork.h"
#include "msg.h"

extern int pid;

int main(int argc, char **argv)
{
        int i;

// process 1        
	if(fork_p(1,1,1,2) < 0)
        {
                printf("cannot create proc\n");
                return -1;
        }
        if(Lab_msgget(1) < 0)
        {
                printf("mistake in msgget\n");
                return -1;
        }
        if(Lab_msgrcv(1,0) < 0)
        {
                printf("mistake in msgsnd\n");
                return -1;
        }

//process 2        
	if(fork_p(2,1,1,1) < 0)
        {
                printf("cannot create proc\n");
                return -1;
        }
        if(Lab_msgget(1) < 0)
        {
                printf("mistake in msgget\n");
                return -1;
        }
        if(Lab_msgsnd(1,0) < 0)
        {
                printf("mistake in msgrcv\n");
                return -1;
        }
	if(Lab_msgsnd(2,0) < 0)
        {
                printf("mistake in msgrcv\n");
                return -1;
        }
	
//process 3
	if(fork_p(3,1,1,2) < 0)
        {
                printf("cannot create proc\n");
                return -1;
        }
        if(Lab_msgget(1) < 0)
        {
                printf("mistake in msgget\n");
                return -1;
        }
        if(Lab_msgrcv(1,0) < 0)
        {
                printf("mistake in msgsnd\n");
                return -1;
        }

//process 4
	if(fork_p(4,1,1,5) < 0)
        {
                printf("cannot create proc\n");
                return -1;
        }


	for (i = 0; i < 10; i++)
	{	
		scheduler();
	}
	
//	waitpid(pid, NULL, 0);

        return 0;
}
