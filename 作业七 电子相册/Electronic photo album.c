/*************************************************************************************************************
File name:Electronic photo album.c
Author:KD
Version:V_1.0
Build date: 2024-04-26
Description:This C code implements the function of displaying BMP images on LCD screens. By default, 'ad. bmp'
			is displayed as the main interface, and users can select manual or automatic playback mode by 
			clicking on the screen. In manual mode, it is possible to flip pages back and forth or return to
			the main interface. In automatic mode, four images will be displayed in sequence, each for one 
			second. After completion, it will return to the main interface.
Others:Usage requires preservation of original author attribution.
Log:1.First create file.
*************************************************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <sys/mman.h>
/*
* 1.根据函数的功能确定函数的数据类型、名字（贴合功能）、参数
* 2.写注释--》重要
*/

/*
* 功能：显示800*480分辨率的bmp图片画面到lcd屏幕上
* @param picname ：需要显示的bmp图片名
* @return -1：打开lcd失败 -2：打开图片失败 -3：申请映射空间失败 0：成功
* 调用示范：show_bmp_lcd("1.bmp");
*/

int show_bmp_lcd(char *picname)
{
	/* 1. 打开文件 */ 
	int lcd_fd = open("/dev/fb0", O_RDWR);
	int bmp_fd = open(picname, O_RDWR);
	if (lcd_fd == -1)
	{
		printf("open lcd error\n");
		return -1;
	}
	if (bmp_fd == -1)
	{
		printf("open bmp error\n");
		return -2;
	}

	/* 1.5.获取bmp图片的颜色数据 */ 
	char buf[800*480*3];
	lseek(bmp_fd, 54, SEEK_SET);	/* 偏移文件的光标 */ 
	read(bmp_fd, buf, 800*480*3);

	/* 2.写入数据 */ 
	/* 申请映射空间 */ 
	int *mmap_start = mmap(NULL, 800*480*4, PROT_READ|PROT_WRITE, MAP_SHARED, lcd_fd, 0);
	if (mmap_start == (void *)-1)
	{
		printf("申请映射空间失败\n");
		return -3;
	}

	/* 使用映射空间 */ 
	int n=0;
	for(int y=0; y<480; y++)
	{
		for(int x=0; x<800; x++, n+=3)
		{
			*(mmap_start+800*(479-y)+x) =   buf[n+0]<<0|
									  buf[n+1]<<8|
									  buf[n+2]<<16|
									  0<<24;
		}
	}

	/* 撤销映射空间 --》 内存泄露 */ 
	munmap(mmap_start, 800*480*4);

	/* 3.关闭文件 */ 
	close(lcd_fd);
	close(bmp_fd);

	return 0;
}

/* input_x : x轴坐标 input_y : y轴坐标 */ 
int input_x, input_y;
/*
* 功能：获取离开屏幕时的坐标
* 返回值： -1：打开触摸屏文件失败，0：成功
*/
int input_fun()
{
	/* 1.打开触摸屏文件 */ 
	int fd = open("/dev/input/event0", O_RDWR);
	
	/* 2.读取数据 */ 
	struct input_event buf;
	int x, y;
	while(1)
	{
		read(fd, &buf, sizeof(buf));
		/* 坐标事件 */ 
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
			x = x*800/1024;	/* 黑色板才会用到 */ 
			y = y*480/600;	/* 黑色板才会用到 */ 
			input_x = x;
			input_y = y;
			printf("离开屏幕: %d, %d\n", input_x, input_y);
			break;
		}
	}
	
	/* 3.关闭文件 */ 
	close(fd);
	return 0;
}

/*
* a.默认会显示一张图片
* b.点击左边显示上一张图片
* c.点击右边显示下一张图片
* d.点击中间退出程序
*/

int main(int argc, char const *argv[])
{
	/* 使用goto进行跳转；设定start为跳转的主界面 */
	start:
	show_bmp_lcd("ad.bmp"); 	/* 设置主界面的图片为ad.bmp */

	/* 若选择手动播放 */
	input_fun();
	if (input_x >= 106 && input_x < 280)
	{
		int i = 1;
		while(1)
		{
			input_fun();
			if (input_x<300)	/* 点击左边 */ 
			{
				printf("左边\n");
				i--;
				if (i==0)
				{
					i = 4;
				}
				switch(i)
				{
					case 1:
						show_bmp_lcd("1.bmp");
						break;
					case 2:
						show_bmp_lcd("2.bmp");
						break;
					case 3:
						show_bmp_lcd("3.bmp");
						break;
					case 4:
						show_bmp_lcd("4.bmp");
						break;
				}
			}

			/* 点击中间 */ 
			if (input_x>=300 && input_x<=500)
			{
				/* printf("中间\n"); */
				printf("感谢使用，程序结束, 返回主页面\n");
				goto start;
			}

			/* 点击右边 */ 
			if (input_x>500)
			{
				printf("右边\n");
				i++;
				if (i==5)
				{
					i = 1;
				}
				switch(i)
				{
					case 1:
						show_bmp_lcd("1.bmp");
						break;
					case 2:
						show_bmp_lcd("2.bmp");
						break;
					case 3:
						show_bmp_lcd("3.bmp");
						break;
					case 4:
						show_bmp_lcd("4.bmp");
						break;
				}
			}
		}
	}

	/* 若选择自动播放 */
	input_fun();
	if (input_x >=500 && input_x < 673)
	{
		int i = 1;
		show_bmp_lcd("1.bmp");
		sleep(1); /* 延迟一秒钟 */
		i++;
		
		while (i >= 2 && i < 5)
		{
			switch (i)
			{
			/*
			* 不会写当自动播放点击屏幕跳转主界面。
			* 是否考虑获取屏幕压力信息，若点击屏幕并松手后跳回主界面，否则程序继续？
			* 是否考虑增加停止区域，当触摸该区域停止播放并返回主界面？
			* 是否考虑使用多线程，单独进行自动播放的控制？
			* 还有其他好办法？
			*/
			case 2:
				show_bmp_lcd("2.bmp");
				sleep(1);
				i++;
				break;
			case 3:
				show_bmp_lcd("3.bmp");
				sleep(1);
				i++;
				break;
			case 4:
				show_bmp_lcd("4.bmp");
				sleep(1);
				i++;
				break;
			}
		}
		
		if(i==5)
		{
			goto start;
		}
	}

	return 0;	
}

//部分代码编写BY:KD 我也不是很会，菜鸡一个@_@

