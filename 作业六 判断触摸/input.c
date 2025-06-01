/**************************************************************************************************************
File name:input.c
Author:KD
Version:V_1.0
Build date: 2024-04-26
Description:This code implements the function of reading touch screen input events in the main function. It 
            first opens the/dev/input/event0 device file in read-write mode, then enters an infinite loop and 
            continuously reads input events using the read function, which blocks until a new event is 
            generated. During the reading process, if the event is an absolute coordinate event, record the x 
            and y coordinates of the touch point; If it is a key event and indicates the end of touch, the 
            coordinates will be scaled and converted before printing. In theory, the device file will be closed
            before the program ends, but due to an infinite loop, the closing operation will not be executed.
Others:Usage requires preservation of original author attribution.
Log:1.First create file.
**************************************************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>

int main(int argc, char const *argv[])
{
    /* 1.打开触摸屏文件 */ 
    int fd = open("/dev/input/event0", O_RDWR);
    
    /* 2.读取数据 */ 
    struct input_event buf;
    int x, y;
    while(1) {
        /* sizeof(int) --> 4 */ 
        read(fd, &buf, sizeof(buf));
        /* 
        * read函数是有阻塞属性，直到下次读取的对象有新的数据产生
        * 检查压力事件
        * if(buf.type==EV_KEY && buf.code==BTN_TOUCH && buf.value==0)
        * {
        *   printf("离开屏幕\n");
        * }
        * if(buf.type==EV_KEY && buf.code==BTN_TOUCH && buf.value==1)
        * {
        * 	printf("没有离开屏幕\n");
        * }       
        */

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
            x = x*800/1024;
            y = y*480/600;
            printf("离开屏幕: %d, %d\n", x, y);
        }
    }
    
    /* 3. 关闭文件 */ 
    close(fd);
    return 0;
}