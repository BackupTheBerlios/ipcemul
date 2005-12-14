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
#include <string.h>
#include "sched.h"
#include "fork.h"
#include "msg.h"
//#include "sort.h"
#include "generator.h"

extern int debug;

int main(int argc, char **argv)
{
	//int mode;

	// hmmmmm........why strcmp don't check any param to NULL????????? 
	if (argv[1] != NULL)
	{
		if (strcmp(argv[1],"--debug") == 0)
		{
			printf("ho ho\n");
			debug = 1;
		}
	}
	// process 1        
    if(fork_p(1,1,1,2) < 0)
    {
      printf("cannot create proc\n");
        return -1;
    }
    if(Lab_msgget(1, IPC_CREAT) < 0)
      {
        printf("mistake in msgget\n");
        return -1;
    }
   if(Lab_msgsnd(1,0,"hello1") < 0)
    {
        printf("mistake in msgsnd\n");
        return -1;
    }
   if(Lab_msgsnd(2,0,"hello2") < 0)
    {
       printf("mistake in msgsnd\n");
        return -1;
   }
    if(Lab_msgsnd(3,0,"hello3") < 0)
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
    if(Lab_msgget(1, IPC_CREAT/*|IPC_EXCL*/) < 0)
    {
        printf("mistake in msgget\n");
        return -1;
    }
     if(Lab_msgrcv(3,0) < 0)
     {
         printf("mistake in msgrcv\n");
         return -1;
     }
     
    if(Lab_msgrcv(2,0) < 0)
    {
    	printf("mistake in msgrcv\n");
    	return -1;
    }
    if(Lab_msgrcv(1,0) < 0)
    {
    	printf("mistake in msgrcv\n");
    	return -1;
    }

	

	if (run() == -1)
	{
		return -1;
	}
	
	return 0;
}
