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

/*
 * Simple doubly linked list implementation.
 *
 * Some of the internal functions ("__xxx") are useful when
 * manipulating whole lists rather than single entries, as
 * sometimes we already know the next/prev entries and we can
 * generate better code by using them directly rather than
 * using the generic single-entry routines.
 */
struct list_head
{
	struct list_head *next, *prev;
};

/* one msq_queue structure for each present queue on the system */
struct Lab_msg_queue
{
    int key;
    int msgid;
    struct Lab_msg_queue *next;

    struct list_head q_messages;
    struct list_head q_receivers;
    struct list_head q_senders;
};

/* one msg_msg structure for each message */
struct msg_msg
{
	struct msg_msg *next; 
	long  m_type;
};

/* one msg_receiver structure for each sleeping receiver */
struct msg_receiver
{
	struct msg_receiver *next;
	struct process* r_prc;
 
	int r_mode;
	long r_msgtype;
	long r_maxsize;
};

#define IPC_PRIVATE 0

/* resource get request flags */
#define IPC_CREAT  00001000   /* create if key is nonexistent */
#define IPC_EXCL   00002000   /* fail if key exists */
#define IPC_NOWAIT 00004000   /* return error on wait */

struct Lab_msg_queue *FindQueue(int descriptor);
void FreeMsg(struct msg_msg *msg);
int Lab_msgget(int key, int flag);
int Lab_msgsnd(long msg_type, int msg_flag);
int Lab_msgrcv(int msg_type, int msg_flag);
int Lab_sys_msgrcv(int type, int flag);
int Lab_sys_msgsnd(int msg_type, int flag);
int Lab_sys_msgget(int key, int flag);
int convert_mode(long *msgtyp, int msgflg);
int testmsg(struct msg_msg* msg,long type,int mode);
