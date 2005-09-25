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
struct func
{
	int func; //0 msgsnd, 1 msgrcv, 2 msgget
	int num_param;
	int param_1;
	int param_2;
	int param_3;
	int param_4;
	struct func *next;
};

struct descriptor
{
	int descrptr;
	struct descriptor *next;
};

struct task
{
	int pid;
	int uid;
	int gid;
	int prio;
	int runned;
	int run_time;
	
	struct func *code;
	
	struct descriptor *dscrptr;
	
	int search_msg;
	
	struct task *next;
};

#define SEARCH_ANY              1
#define SEARCH_EQUAL            2
#define SEARCH_NOTEQUAL         3
#define SEARCH_LESSEQUAL        4

#define MSG_EXCEPT      020000  /* recv any msg except of specified type.*/

int AddCode(int num,...);
struct task *Find_max_prio(void);
int fork_p(int pid, int uid, int gid, int prio);
int ExecCode(struct task *tsk);
int Add2proc_dscrptr(int msgid);
