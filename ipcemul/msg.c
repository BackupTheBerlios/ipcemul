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
#include <unistd.h>
#include "msg.h"
#include "fork.h"
 
struct Lab_msg_queue *root_msg_queue = NULL;

struct msg_msg *root_msg_msg = NULL;

struct msg_receiver *root_msg_reciever = NULL;

extern struct task *current_proc;

struct Lab_msg_queue *Find_ipc_key(int key)
{
	struct Lab_msg_queue *ipc_k = root_msg_queue;
	
	while(ipc_k != NULL)
	{
		if (ipc_k->key == key)
		{
			return ipc_k;
		}
		ipc_k = ipc_k->next;
	}
	
	return NULL;
}

int Lab_sys_msgget(int key)
{
	struct Lab_msg_queue *ipc_ = NULL;
	
	ipc_ = Find_ipc_key(key);
	if(ipc_ == NULL)
	{
		ipc_ = (struct Lab_msg_queue *)malloc(sizeof(struct Lab_msg_queue));
		if(ipc_ == NULL)
		{
			printf("cannot alloc mem\n");
			return -1;
		}
		
		ipc_->key = key;
//		ipc_->q_receivers = NULL;
//		ipc_->q_senders = NULL;
		ipc_->msgid = 1+(int) (10.0*rand()/(RAND_MAX+1.0));
		ipc_->next = root_msg_queue;
		root_msg_queue = ipc_;
	}
	else
	{
		Add2proc_dscrptr(ipc_->msgid);
		printf("queue already exist\n");
		return ipc_->msgid;
	}
	
	Add2proc_dscrptr(ipc_->msgid);

	//printf("time create queue with key %d is %d\n", key, *timeptr);
	
	return ipc_->msgid;
}

int Lab_sys_msgrcv(long type, int flag)
{
	int mode;
	struct Lab_msg_queue *queue = NULL;
	struct msg_msg *msg = root_msg_msg;
//	struct list_head *list = NULL;
	struct msg_receiver *msg_r;
	int result;
	struct task *tsk = current_proc;
	
	mode = convert_mode(&type, flag);

	queue = FindQueue(tsk->dscrptr->descrptr);
	if(queue == NULL)
	{
		printf("cannot find queue with id %d\n", tsk->dscrptr->descrptr);
		return -1;
	}
	
	while(msg != NULL)
	{
		current_proc->search_msg++;
		result = testmsg(msg,current_proc->code->param[1],mode);
		if(result == 1)
			break;
		else
			msg = msg->next;
	}
	
	if(msg == NULL)
	{
		if (flag & IPC_NOWAIT)
			return 0;
		printf("msg == NULL\n");
		msg_r = (struct msg_receiver *)malloc(sizeof(struct msg_receiver));
		if(msg_r == NULL)
		{
			printf("cannot alloc mem\n");
			return -1;
		}
		msg_r->r_tsk = current_proc;
		msg_r->r_mode = mode;
		msg_r->r_msgtype = current_proc->code->param[1];
		msg_r->next = root_msg_reciever;
		root_msg_reciever = msg_r;
		return 1;
	}
	else
	{
		printf("\nfound msg on time %d\n", tsk->run_time);  ////!!!!!!!!!!!!!!!!!!!!!!!!!
		printf("rcv msg\n");
		FreeMsg(msg);
	}
	
	return 0;
}

int testmsg(struct msg_msg* msg,long type,int mode)
{
	switch(mode)
	{
		case SEARCH_ANY:
			return 1;
		case SEARCH_LESSEQUAL:
			if(msg->m_type <=type)
				return 1;
			break;
		case SEARCH_EQUAL:
			if(msg->m_type == type)
				return 1;
			break;
		case SEARCH_NOTEQUAL:
			if(msg->m_type != type)
				return 1;
			break;
	}
	
	return 0;
}

int Lab_sys_msgsnd(int msg_type, int flag)
{
	struct msg_msg *msg = NULL;
	
	if (msg_type < 0)
	{
		printf("you must use msg type > 0\n");
		exit(-1);
	}
	
	msg = (struct msg_msg *)malloc(sizeof(struct msg_msg));
	if(msg == NULL)
	{
		printf("cannot alloc mem\n");
		return -1;
	}
	msg->m_type = msg_type;
	msg->next = root_msg_msg;
	root_msg_msg = msg;
	

	//printf("time send msg is %d\n", *timeptr);
	
	return 0;
}

void FreeMsg(struct msg_msg *msg)
{
	struct msg_msg *msg_h = root_msg_msg;
	
	
//      finding previous msg
	if (msg_h!=msg)
		while(msg_h->next!=msg)
			msg_h=msg_h->next;
	msg_h=msg->next;
	free(msg);
}

int convert_mode(long* msgtyp, int msgflg)
{
	/* 
	*  find message of correct type.
	*  msgtyp = 0 => get first.
	*  msgtyp > 0 => get first message of matching type.
	*  msgtyp < 0 => get message with least type must be < abs(msgtype).  
	*/
	if((*msgtyp) == 0)
		return SEARCH_ANY;
	if((*msgtyp) < 0)
	{
		(*msgtyp)*=-1;
		return SEARCH_LESSEQUAL;
	}
	if(msgflg & MSG_EXCEPT)
		return SEARCH_NOTEQUAL;
	
	return SEARCH_EQUAL;
}

struct Lab_msg_queue *FindQueue(int descriptor)
{
	struct Lab_msg_queue *que = root_msg_queue;
	
	while(que != NULL)
	{
		if(que->msgid == descriptor)
		{
			return que;
		}
		que = que->next;
	}
	
	return NULL;
}

int Lab_msgget(int key)
{
	return AddCode(2,2,key);
}

int Lab_msgsnd(long msg_type, int msg_flag)
{
	return AddCode(3,0, msg_type, msg_flag);
}

int Lab_msgrcv(int msg_type, long msg_flag)
{
	return AddCode(3,1, msg_type, msg_flag);
}
