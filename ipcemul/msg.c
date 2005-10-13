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
//#include "util.h"
#include "list.h"

struct msg_sender
{
	struct list_head list;
	struct task_struct* tsk;
};

struct Lab_msg_queue *root_msg_queue = NULL;
struct msg_msg *root_msg_msg = NULL;
struct msg_receiver *root_msg_reciever = NULL;

//static struct Lab_ipc_ids msg_ids;

extern struct process *current_proc;

//#define msg_buildid(id, seq) \
//	ipc_buildid(&msg_ids, id, seq)

//static int Lab_newque(int key, int msgflg)
//{
//	int id;
//	int retval;
//	struct Lab_msg_queue *msq = NULL;

//	msq = (struct Lab_msq_queue *)malloc(sizeof(struct Lab_msg_queue));
//	if (msq == NULL)
//	{
//		printf("\tcannot alloc memory for new queue\n");
//		return -1;
//	}
//
//	INIT_LIST_HEAD(&msq->q_messages);
//	INIT_LIST_HEAD(&msq->q_receivers);
//	INIT_LIST_HEAD(&msq->q_senders);
//
//	return msg_buildid(1+(int) (10.0*rand()/(RAND_MAX+1.0)),1+(int) (10.0*rand()/(RAND_MAX+1.0)));
//}

struct Lab_msg_queue *Find_ipc_key(int key)
{
    struct Lab_msg_queue *ipc_k = root_msg_queue;

    printf("\ttrying to find queue with key = %d\n",key);
    while(ipc_k != NULL)
    {
        if (ipc_k->key == key)
        {
            	printf("\twooow found queue\n");
		return ipc_k;
        }
        ipc_k = ipc_k->next;
    }

    return NULL;
}

int Lab_sys_msgget(int key, int flag)
{
	struct Lab_msg_queue *ipc_ = NULL;

	if (key == IPC_PRIVATE)
	{
	    
	    printf("\ti see key == IPC_PRIVATE\n");
	    ipc_ = (struct Lab_msg_queue *)malloc(sizeof(struct Lab_msg_queue));
	    if(ipc_ == NULL)
	    {
		printf("\tcannot alloc mem\n");
		return -1;
	    }

	    ipc_->key = key;
	    INIT_LIST_HEAD(&ipc_->q_messages);
	    INIT_LIST_HEAD(&ipc_->q_receivers);
	    INIT_LIST_HEAD(&ipc_->q_senders);
	    ipc_->msgid = 1+(int) (10.0*rand()/(RAND_MAX+1.0));
	    
	    while (FindQueue(ipc_->msgid) != NULL)
		    ipc_->msgid = 1+(int) (10.0*rand()/(RAND_MAX+1.0));
	    
	    ipc_->next = root_msg_queue;
	    root_msg_queue = ipc_;
    	}
	else if ((ipc_ = Find_ipc_key(key)) == NULL)
	{
		if (flag & IPC_CREAT)
		{
			ipc_ = (struct Lab_msg_queue *)malloc(sizeof(struct Lab_msg_queue));
			if(ipc_ == NULL)
			{
				printf("\tcannot alloc mem\n");
				return -1;
			}

			ipc_->key = key;
			INIT_LIST_HEAD(&ipc_->q_messages);
			INIT_LIST_HEAD(&ipc_->q_receivers);
			INIT_LIST_HEAD(&ipc_->q_senders);
			ipc_->msgid = 1+(int) (10.0*rand()/(RAND_MAX+1.0));
			ipc_->next = root_msg_queue;
			root_msg_queue = ipc_;
		}
		else
		{
			printf("\t no such file or dir\n");
			return -1;
		}
	}
	else if (flag & IPC_CREAT && flag & IPC_EXCL)
		printf("\talready exist\n");

	Add2proc_dscrptr(ipc_->msgid);

	printf("\t(added to proc) made queue with msgid = %d\n", ipc_->msgid);
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
	struct process *prc = current_proc;
	int time = 0;
	
	printf("\ttrying rcv msg with type = %d\n", type);
	
	mode = convert_mode(&type, flag);

	queue = FindQueue(prc->dscrptr->descrptr);
	if(queue == NULL)
	{
		printf("cannot find queue with id %d\n", prc->dscrptr->descrptr);
		return -1;
	}
	printf("\t\tbegin find msg, time = 0\n");
	while(msg != NULL)
	{
		time++;
		current_proc->search_msg++;
		result = testmsg(msg,current_proc->code->param[1],mode);
		if(result == 1)
			break;
		else
			msg = msg->next;
	}
	printf("\t\ttotal time = %d\n", time);
	if(msg == NULL)
	{
		if (flag & IPC_NOWAIT)
			return 0;
		printf("\tmsg == NULL\n");
		msg_r = (struct msg_receiver *)malloc(sizeof(struct msg_receiver));
		if(msg_r == NULL)
		{
			printf("cannot alloc mem\n");
			return -1;
		}
		printf("\tadd proc to msg_rcv\n");
		msg_r->r_prc = current_proc;
		msg_r->r_mode = mode;
		msg_r->r_msgtype = current_proc->code->param[1];
		msg_r->next = root_msg_reciever;
		root_msg_reciever = msg_r;
		return 1;
	}
	else
	{
		printf("\tpolychil soobschenie!!!))) yeahh baby))\n");
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
	struct Lab_msg_queue *msq = NULL;
	struct msg_msg *msg = NULL;
	
	if (msg_type < 1)
	{
		printf("you must use msg type > 1\n");
		exit(-1);
	}
	
//	if ((msq = FindQueue(current_proc->dscrptr->descrptr)) == NULL)
//	{
//		printf("\tno such queue\n");
//		return -1;
//	}

//	if ()
	msg = (struct msg_msg *)malloc(sizeof(struct msg_msg));
	if(msg == NULL)
	{
		printf("cannot alloc mem\n");
		return -1;
	}
	msg->m_type = msg_type;
	msg->next = root_msg_msg;
	root_msg_msg = msg;
	

	printf("\tenter msg with type = %d\n", msg_type);
	
	return 0;
}

void FreeMsg(struct msg_msg *msg)
{
	struct msg_msg *msg_h = root_msg_msg;
	
	printf("\tfree memory\n");

	if (msg_h->next == NULL)
	{
		free(msg);
		root_msg_msg = NULL;
	}
	else
	{
		while(msg_h->next->m_type != msg->m_type)
			msg_h = msg_h->next;
	
		msg_h->next = msg->next;
	
		free(msg);
	}
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
	
	printf("\ttrying find queue with descriptor = %d\n", descriptor);
	
	while(que != NULL)
	{
		if(que->msgid == descriptor)
		{
			printf("\tfound queue!!)) \n");
			return que;
		}
		que = que->next;
	}
	
	printf("\tno queue with such descriptor((\n");
	return NULL;
}

int Lab_msgget(int key, int flag)
{
	return AddCode(3, MSGGET, key, flag);
}

int Lab_msgsnd(long msg_type, int msg_flag)
{
	return AddCode(3, MSGSND, msg_type, msg_flag);
}

int Lab_msgrcv(int msg_type, long msg_flag)
{
	return AddCode(3, MSGRCV, msg_type, msg_flag);
}
