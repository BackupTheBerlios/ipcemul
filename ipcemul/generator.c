#include<stdio.h>
#include<stdlib.h>
#include"msg.h"
#include"fork.h"

extern struct msg_msg *root_msg_msg;

int generator_msgs(int begin, int end, int key)
{
	struct msg_msg *msg = NULL;

	if (Lab_msgget(key, IPC_CREAT) == -1)
	{
		printf("cannot create queue\n");
		return -1;
	}
		
	while (end >= begin)
	{
		Lab_msgsnd(end, 1);		
		
		end--;					 
	}

	return 0;
}

int generator_procs(int begin, int end, int mode)
{
	int result;
	int rand_pid;
	int rand_uid, rand_gid, rand_prio;
	
	while(end >= begin)
	{
		rand_pid = 1+(int) (10.0*rand()/(RAND_MAX+1.0));
		rand_uid = 1+(int) (10.0*rand()/(RAND_MAX+1.0));
		rand_gid = 1+(int) (10.0*rand()/(RAND_MAX+1.0));
		rand_prio = 1+(int) (10.0*rand()/(RAND_MAX+1.0));
		
		result = fork_p(rand_pid, rand_uid, rand_gid, rand_prio);

		end--;

		printf("");
	}
	
	return 0;
}
