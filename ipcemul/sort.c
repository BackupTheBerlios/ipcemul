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
#include "msg.h"
#include "fork.h"

//extern struct Lab_msg_queue *root_msg_queue;
extern struct msg_msg *root_msg_msg;


int sort_msg(int num)
{
        struct msg_msg *tmp1 = root_msg_msg;
        struct msg_msg *tmp2 = NULL;
        int must_sort = 1;
        int temp;
        int sorted = 1;

        printf("exec Sort\n");

        if (num == 0)
        {
                if (tmp1 != NULL)
                {
                        tmp2 = tmp1->next;

                        while (must_sort)
                        {
//                              printf("\tin main circle - must sort\n");
                                if(sorted)
                                {
                                        sorted = 0;
                                        tmp1 = root_msg_msg;
                                        tmp2 = tmp1->next;
                                        while(tmp2 != NULL)
                                        {
//                                              printf("\t\tin second circle\n");
                                                if (tmp1->m_type > tmp2->m_type)
                                                {
                                                        temp = tmp1->m_type;
                                                        tmp1->m_type = tmp2->m_type;
                                                        tmp2->m_type = temp;
                                                        sorted = 1;
                                                }
                                                tmp1 = tmp1->next;
                                                tmp2 = tmp2->next;
                                        }
                                }
                                else
                                        must_sort = 0;
                        }
                }
                tmp1 = root_msg_msg;
                printf("\tafter sorting\n");
                while (tmp1 != NULL)
                {
                        printf("\tm_type = %ld  ", tmp1->m_type);
                        tmp1 = tmp1->next;
                }
//              printf("\nend sort\n");
        }
        else
                printf("bad number\n");

        return 1;
}
