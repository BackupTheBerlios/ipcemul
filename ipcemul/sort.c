#include <stdio.h>
#include "msg.h"
#include "fork.h"

//extern struct Lab_msg_queue *root_msg_queue;
extern struct msg_msg *root_msg_msg;

//void make_sort(int num)
//{
//	struct msg_msg *tmp1 = root_msg_msg;
//	struct msg_msg *tmp2 = root_msg_msg->next;
//	int must_sort = 1;
//	int temp;
//	int sorted = 1;

//	if(num == 0)
//	{
//		if (tmp1 != NULL)
//		{
//			printf("\tin make_sort with num 0\n");
//			while (must_sort)
//			{
//				if(sorted)
//				{
//					sorted = 0;
//				
//					while(tmp2 != NULL)
//					{
//						if (tmp1->m_type > tmp2->m_type)
//						{
//							temp = tmp1->m_type;
//							tmp1->m_type = tmp2->m_type;
//							tmp2->m_type = temp;
//							sorted = 1;
//						}
//						tmp1 = tmp1->next;
//						tmp2 = tmp2->next;
//					}
//				}
//				else
//					must_sort = 0;
//			}
//		}
//	}
//}

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
//				printf("\tin main circle - must sort\n");
				if(sorted)
				{
					sorted = 0;
					tmp1 = root_msg_msg;
					tmp2 = tmp1->next;
					while(tmp2 != NULL)
					{
//						printf("\t\tin second circle\n");
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
//		printf("\nend sort\n");
	}
	else
		printf("bad number\n");

	return 1;
}
