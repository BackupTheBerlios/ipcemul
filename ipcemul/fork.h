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
#define MSGSND 0
#define MSGRCV 1
#define MSGGET 2

struct tsk
{
	int tsk; //0 msgsnd, 1 msgrcv, 2 msgget
	int num_param;
	int param[5];
	struct tsk *next;
};

struct descriptor
{
	int descrptr;
	struct descriptor *next;
};

/* Structure for the process */
struct process
{
	int pid;
	int uid;
	int gid;
	int prio;
	int runned;
	int run_time;
	int run;       //sleep or not for msg
	
	struct tsk *code;
	
	struct descriptor *dscrptr;
	
	int search_msg;
	
	struct process *next;
};

#define SEARCH_ANY              1
#define SEARCH_EQUAL            2
#define SEARCH_NOTEQUAL         3
#define SEARCH_LESSEQUAL        4

#define MSG_EXCEPT      020000  /* recv any msg except of specified type.*/
#define IPC_NOWAIT 00004000   /* return error on wait */
int AddCode(int num,...);
struct process *Find_max_prio(void);
int fork_p(int pid, int uid, int gid, int prio);
int ExecCode(struct process *prc);
int Add2proc_dscrptr(int msgid);
