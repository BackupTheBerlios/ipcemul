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
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<signal.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<fcntl.h>
int *timeptr;
int pid;
//void DtTime(void)
//{
//	shmdt(timeptr1);
//}

void stop(int sig)
{
	printf("exit shild\n");
	exit(1);
}

//void RcvTime(void)
//{
//	int key1;
//	
//	if ((key1 = ftok("a.out", 'A')) < 0)
//	{
//		printf("Cannot get key\n");
//		exit(-1);
//	}
//
//	if ((shmid1 = shmget(key1, 2, 0666|IPC_CREAT)) < 0)
//	{
//		printf("Cannot alloc memory in main\n");
//		exit(-1);
//	}
//
//	if ((timeptr1 = (int *)shmat(shmid1,NULL,0)) == NULL)
//	{
//		printf("mistake in shmad parent\n");
//		exit(-1);
//	}
//	
//}

void TicTac(void)
{
//	int key2, shmid2;
	int fd;
	int zero = 0;
	if ((fd = open("time",O_RDWR | O_CREAT,0666)) == -1)
	{
		printf("cannot make time file\n");
		exit(-1);
	}
	if( write(fd, &zero, sizeof(int)) == -1)
	{
		printf("cannot init time file\n");
		exit(-1);
	}
	timeptr = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	setbuf(stdout,NULL);
	if ((pid = fork()) == 0)
	{
		signal(SIGINT,stop);

//		if ((key2 = ftok("a.out", 'A')) < 0)
//		{
//			printf("Cannot get key\n");
//			exit(-1);
//		}

//		if ((shmid2 = shmget(key2, 2, 0)) < 0)
//		{
//			printf("Cannot alloc memory in child\n");
//			exit(-1);
//		}

//		if ((timeptr2 = (int *)shmat(shmid2,NULL,0)) == NULL)
//		{
//			printf("mistake in shmad child\n");
//			exit(-1);
//		}
		while(1)
		{
			usleep(50);
			(*timeptr)++;

		}
	}
}

void stop_TicTak(void)
{
	kill(pid,9);
	
}
