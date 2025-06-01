#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>

int main(int argc, char const *argv[])
{
	// 1. 打开触摸屏文件
	int fd = open("/dev/input/event0", O_RDWR);
	
	// 2. 读取数据
	struct input_event buf;
	int x, y;
	while(1)
	{
		// sizeof(int) --> 4
		read(fd, &buf, sizeof(buf));
		// read函数是有阻塞属性，直到下次读取的对象有新的数据产生
		// 检查压力事件
		// if(buf.type==EV_KEY && buf.code==BTN_TOUCH && buf.value==0)
		// {
		// 	printf("离开屏幕\n");
		// }

		// if(buf.type==EV_KEY && buf.code==BTN_TOUCH && buf.value==1)
		// {
		// 	printf("没有离开屏幕\n");
		// }

		// 坐标事件
		if(buf.type==EV_ABS  && buf.code==ABS_X)
		{
			x = buf.value;
		}

		if(buf.type==EV_ABS  && buf.code==ABS_Y)
		{
			y = buf.value;
		}

		if(buf.type==EV_KEY && buf.code==BTN_TOUCH && buf.value==0)
		{
			x = x*800/1024;
			y = y*480/600;
			printf("离开屏幕: %d, %d\n", x, y);
		}
	}
	
	// 3. 关闭文件
	close(fd);
	return 0;
}