/***************************************************************************************************************
File name:Pain_flags.c
Author:KD
Version:V_1.0
Build date: 2024-04-26
Description:This C code implements the function of drawing the German flag on the frame buffer device /dev/fb0. 
            It opens the frame buffer device in read-write mode using the open function. If the opening fails, 
            it outputs an error message and returns. The program sequentially draws the three horizontal stripes 
            of the flag—black, red, and yellow—each occupying one-third of the screen height. After each color 
            is drawn, it automatically saves the image. Finally, it closes the device using the close function. 
            If the closing fails, it outputs an error message and returns.
Others:Usage requires preservation of original author attribution.
Log:1.First create file.
***************************************************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    /* 1.打开文件 */ 
    int fd = open("/dev/fb0", O_RDWR);
    if (fd == -1)
    {
        printf("open file error\n");
        return -1;
    }
    
    /* 2.写入数据（颜色） */ 
    /* 德国国旗的尺寸，假设为800x480像素 */ 
    int width = 800;
    int height = 480;
    
    /* 定义颜色 */ 
    int black = 0x000000;
    int red = 0xFF0000;
    int yellow = 0xFFFF00;
    
    /* 绘制德国国旗 */ 
    /* 黑色 */ 
    for (int i = 0; i < width * height / 3; ++i)
    {
        write(fd, &black, 4);
    }
    /* 红色 */ 
    for (int i = 0; i < width * height / 3; ++i)
    {
        write(fd, &red, 4);
    }
    /* 黄色 */ 
    for (int i = 0; i < width * height / 3; ++i)
    {
        write(fd, &yellow, 4);
    }
    
    /* 3.关闭文件 */ 
    int ret = close(fd);
    if (ret == -1)
    {
        printf("close file error.\n");
        return -2;
    }
    
    return 0;
}
