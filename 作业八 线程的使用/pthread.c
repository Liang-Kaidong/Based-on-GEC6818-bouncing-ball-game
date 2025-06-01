#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

pthread_t id;	// 线程号，线程的标识
pthread_t id_1;	// 线程号，线程的标识

void *apple_tree(void *arg)
{
	for(int i=0;i<5;i++)
	{
		printf("apple tree\n");
		sleep(1);

	}
	pthread_cancel(id_1);
}

void *music(void *arg)
{
	while(1)
	{
		printf("听歌\n");
		sleep(1);
	}
}

int main(int argc, char const *argv[])
{
	pthread_create(&id, NULL, apple_tree, NULL);
	pthread_create(&id_1, NULL, music, NULL);
	
	// 线程是依附于进程存在的
	// 一般就是有几个线程我们就写几个等待
	pthread_join(id, NULL);
	return 0;
}