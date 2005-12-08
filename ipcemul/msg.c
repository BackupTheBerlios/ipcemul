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
#include <string.h>
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
extern int number_of_tasks;

int FindInWaitingProc(int msg_type)
{
	struct msg_receiver *prc_rcv = root_msg_reciever;
	struct msg_receiver *prev_prc_rcv = NULL;
	struct process *sleep_proc;

	if (root_msg_reciever != NULL)
	{
		switch (prc_rcv->r_mode)
		{
			case SEARCH_EQUAL:
				while (prc_rcv != NULL)
				{
					if (prc_rcv->r_msgtype == msg_type)
					{
						printf("\t found in rcv_queue_wait msg))!\n");
						//now we must delete that function from sleeping proc
						sleep_proc = Find_process(prc_rcv->r_prc->pid);
						if (sleep_proc == NULL)
						{
							printf("\t Cannot find sleeping process!!\n");
						}
						sleep_proc->run = 1;
						RemoveCode(sleep_proc);
						if (prev_prc_rcv == NULL)
						{
							root_msg_reciever = prc_rcv->next;
							free(prc_rcv);
							return 1;
						}
					}
					prev_prc_rcv = prc_rcv;
					prc_rcv = prc_rcv->next;
				}
				break;
			case SEARCH_ANY: 
				while (prc_rcv != NULL)
				{
					if (1)
					{
						printf("\t found in rcv_queue_wait msg))!\n");
						//now we must delete that function from sleeping proc
						sleep_proc = Find_process(prc_rcv->r_prc->pid);
						if (sleep_proc == NULL)
						{
							printf("\t Cannot find sleeping process!!\n");
						}
						sleep_proc->run = 1;
						RemoveCode(sleep_proc);
						if (prev_prc_rcv == NULL)
						{
							root_msg_reciever = prc_rcv->next;
							free(prc_rcv);
							return 1;
						}
					}
					prev_prc_rcv = prc_rcv;
					prc_rcv = prc_rcv->next;
				}
				break;
			case SEARCH_LESSEQUAL: 
				while (prc_rcv != NULL)
				{
					if (prc_rcv->r_msgtype < msg_type)
					{
						printf("\t found in rcv_queue_wait msg))!\n");
						//now we must delete that function from sleeping proc
						sleep_proc = Find_process(prc_rcv->r_prc->pid);
						if (sleep_proc == NULL)
						{
							printf("\t Cannot find sleeping process!!\n");
						}
						sleep_proc->run = 1;
						RemoveCode(sleep_proc);
						if (prev_prc_rcv == NULL)
						{
							root_msg_reciever = prc_rcv->next;
							free(prc_rcv);
							return 1;
						}
					}
					prev_prc_rcv = prc_rcv;
					prc_rcv = prc_rcv->next;
				}
				break;
			case SEARCH_NOTEQUAL: 
				while (prc_rcv != NULL)
				{
					if (prc_rcv->r_msgtype != msg_type)
					{
						printf("\t found in rcv_queue_wait msg))!\n");
						//now we must delete that function from sleeping proc
						sleep_proc = Find_process(prc_rcv->r_prc->pid);
						if (sleep_proc == NULL)
						{
							printf("\t Cannot find sleeping process!!\n");
						}
						sleep_proc->run = 1;
						RemoveCode(sleep_proc);
						if (prev_prc_rcv == NULL)
						{
							root_msg_reciever = prc_rcv->next;
							free(prc_rcv);
							return 1;
						}
					}
					prev_prc_rcv = prc_rcv;
					prc_rcv = prc_rcv->next;
				}
				break;
		}
	}

	return 0;
}

struct Lab_msg_queue *Find_ipc_key(int key)
{
    struct Lab_msg_queue *ipc_k = root_msg_queue;

    printf("\tfinding queue with key = %d ------> ",key);
    while(ipc_k != NULL)
    {
        if (ipc_k->key == key)
        {
                printf("found\n");
                return ipc_k;
        }
        ipc_k = ipc_k->next;
    }

    printf("not found\n");

    return NULL;
}

int Lab_sys_msgget(int key, int msgflag)
{
        struct Lab_msg_queue *ipc_ = NULL;
        int id, ret;// = -EPERM;

        if (key == IPC_PRIVATE)
        {

//          printf("\t\n");
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
                //ret = newque(key, msgflg);
        }
        else if ((ipc_ = Find_ipc_key(key)) == NULL)
        {
                if (msgflag & IPC_CREAT)
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
        else if (msgflag & IPC_CREAT && msgflag & IPC_EXCL)
                printf("\tqueue already exist\n");

        Add2proc_dscrptr(ipc_->msgid); //adding  queue id to the descriptors stek of current process

        printf("\tmake new queue with msgid = %d\n", ipc_->msgid);
        return ipc_->msgid;
}

int Lab_sys_msgrcv(long type, int flag)
{
        int mode;
        struct Lab_msg_queue *queue = NULL;
        struct msg_msg *msg = root_msg_msg;
//      struct list_head *list = NULL;
	struct msg_receiver *msg_r;
        int result;
        struct process *prc = current_proc;
        int time = 0;

        printf("\trcving msg with type = %ld\n", type);

        mode = convert_mode(&type, flag);
//      printf("\tafter conver_mode\n");

      if (prc->dscrptr == NULL)
      {
              printf("\tqueue was deleted during trying rcv msg or it is does not exist\n");
              exit(-1);
      }
        queue = FindQueue(prc->dscrptr->descrptr);
        if(queue == NULL)
        {
                printf("cannot find queue with id %d\n", prc->dscrptr->descrptr);
                return -1;
        }
        printf("\tQueue found with key=%d\n",queue->key);
//      printf("\t\tbegin find msg, time = 0\n");
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
        printf("\t\ttotal time in search = %d\n", time);
        if(msg == NULL)
        {
                if (flag & IPC_NOWAIT)
                        return 0;
		printf("\tmsg == NULL, wait for message\n");
		msg_r = (struct msg_receiver *)malloc(sizeof(struct msg_receiver));
                if(msg_r == NULL)
                {
                      printf("cannot alloc mem\n");
                      return -1;
                }
                printf("\tadd proc to msg_rcv\n");
                current_proc->run = 0; //go to sleep
                msg_r->r_prc = current_proc;
                msg_r->r_mode = mode;
                msg_r->r_msgtype = current_proc->code->param[1];
                msg_r->next = root_msg_reciever;
                root_msg_reciever = msg_r;
		return 1;
        }
        else
        {
                printf("\tResult Ok\n");
                printf("Recived message:%s\n",msg->text);
                FreeMsg(msg);
        }

        return 0;
}

int testmsg(struct msg_msg* msg,long type,int mode)
{
//	OneStringToProtocol("\t\tin testmsg");
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

int Lab_sys_msgsnd(int msg_type, int flag,char*text)
{
//      struct Lab_msg_queue *msq = NULL;
        struct msg_msg *msg = NULL;

        if (msg_type < 1)
        {
                printf("you must use msg type > 1\n");
                exit(-1);
        }
/*      
//      if ((msq = FindQueue(current_proc->dscrptr->descrptr)) == NULL)
//      {
//              printf("\tno such queue\n");
//              return -1;
//      }

//      if ()
*/
        if (!FindInWaitingProc(msg_type))
	{
		msg = (struct msg_msg *)malloc(sizeof(struct msg_msg));
		if(msg == NULL)
		{
			printf("cannot alloc mem\n");
			return -1;
		}
		msg->m_type = msg_type;
		msg->text=text;
		msg->next = root_msg_msg;
		root_msg_msg = msg;


		printf("\tmsg with type = %d send\n", msg_type);
	}

        return 0;
}

void FreeMsg(struct msg_msg *msg)
{
        struct msg_msg *msg_h = root_msg_msg;

        printf("exec free memory\n");

//      printf("\tbefore del\n");
        while (msg_h != NULL)
        {
                printf("\t %ld", msg_h->m_type);
                msg_h = msg_h->next;
        }
        printf("\n");
        msg_h = root_msg_msg;
        if ((msg_h->next == NULL) || (msg_h == msg))
        {
//              printf("\tremove first and last msg\n");
                root_msg_msg = msg_h->next;
//              printf("\tafter free(msg)\n");
        }
        else
        {
                while(msg_h->next != msg)
                        msg_h = msg_h->next;

                msg_h->next = msg->next;
        }

        free(msg);
//      printf("\nafter if else\n");
        msg_h = root_msg_msg;
        printf("\tafter del\n");
        while (msg_h != NULL)
        {
                printf("\t %ld", msg_h->m_type);
                msg_h = msg_h->next;
        }
        printf("\n");
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
        {
                printf("\tSearch any msg\n");
                return SEARCH_ANY;
        }
        if((*msgtyp) < 0)
        {
                (*msgtyp)*=-1;
                printf("\tSearch less m_type\n");
                return SEARCH_LESSEQUAL;
        }
        if(msgflg & MSG_EXCEPT)
        {
                printf("\tSearch not equal\n");

                return SEARCH_NOTEQUAL;
        }
        printf("\tSeach equal\n");
        return SEARCH_EQUAL;
}

struct Lab_msg_queue *FindQueue(int descriptor)
{
        struct Lab_msg_queue *que = root_msg_queue;

        printf("\tFinding queue with descriptor = %d --------> ", descriptor);

        while(que != NULL)
        {
                if(que->msgid == descriptor)
                {
                        printf(" ok \n");
                        return que;
                }
                que = que->next;
        }

        printf(" not found\n");
        return NULL;
}

int Lab_msgget(int key, int flag)
{
    return AddCode(3, MSGGET, key, flag);
}

int Lab_msgsnd(long msg_type, int msg_flag,char text[20])
{
    char *text2;
    text2=(char *)malloc(sizeof(char)*20);
    strcpy(text2,text);

    return AddCode(4, MSGSND, msg_type, msg_flag,text2);
}

int Lab_msgrcv(long msg_type, int msg_flag)
{
    return AddCode(3, MSGRCV, msg_type, msg_flag);
}
