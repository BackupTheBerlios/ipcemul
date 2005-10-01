#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
struct timespec *NULL_time;

int init_time(void)
{
	NULL_time= (struct timespec *)malloc(sizeof(struct timespec));
		if(NULL_time== NULL)
		{
			printf("cannot alloc mem\n");
			return -1;
		}
	if(clock_gettime(CLOCK_THREAD_CPUTIME_ID, NULL_time)!=0)
	{
		printf("Error getting time\n");
		return -1;
	}
	return 0;
}

double time_substr(struct timespec x1,struct timespec x2)
{
	return (double)((((x2.tv_sec-x1.tv_sec))*1000000000.0+((x2.tv_nsec-x1.tv_nsec)))/1000);
}

void time_msg()
{

}

