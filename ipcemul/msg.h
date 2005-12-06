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
struct list_head
{
        struct list_head *next, *prev;
};

//struct msg_msgseg
//{
//      struct msg_msgseg* next;
        /* the next part of the message follows immediately */
//};

/* used by in-kernel data structures */
struct kern_ipc_perm
{
        //spinlock_t      lock;
        //int             deleted;
        int key;//key_t           key;
        int uid;//uid_t           uid;
        int gid;//gid_t           gid;
        //uid_t           cuid;
        //gid_t           cgid;
        int mode;//mode_t          mode; 
        //unsigned long   seq;
        //void            *security;
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
//<<<<<<< msg.h
        //struct msg_msg *next;
        struct list_head m_list;  //list of messages
        long  m_type;            //type of message

        struct msg_msg *next;
//=======
//      struct msg_msg *next; 
        char * text;
//      long  m_type;
//>>>>>>> 1.10
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
int Lab_msgsnd(long msg_type, int msg_flag,char *text);
int Lab_msgrcv(long msg_type, int msg_flag);
int Lab_sys_msgrcv(long type, int flag);
int Lab_sys_msgsnd(int msg_type, int flag,char text[20]);
int Lab_sys_msgget(int key, int flag);
int convert_mode(long *msgtyp, int msgflg);
int testmsg(struct msg_msg* msg,long type,int mode);
