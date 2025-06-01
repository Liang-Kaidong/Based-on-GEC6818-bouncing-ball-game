/**************************************************************************************************************
File name:show_bmp_picture.c
Author:KD
Version:V_1.0
Build date: 2024-04-26
Description:This C code implements the function of displaying a BMP image on an LCD screen. It opens the frame 
            buffer device /dev/fb0 and the BMP image file using the open function, skipping the BMP file header 
            to read pixel data. The LCD device is mapped into user space using mmap, and the BMP data is 
            converted line by line and written to the mapped memory through a double loop, completing the image 
            display. Finally, the memory mapping is undone, and the file is closed.
Others:Usage requires preservation of original author attribution.
Log:1.First create file.
**************************************************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>

int main(int argc, char const *argv[])
{
    /* 1.打开两个文件（lcd屏幕、bmp图片） */ 
    int lcd_fd = open("/dev/fb0", O_RDWR);
    int bmp_fd = open("5.bmp", O_RDWR);
    if (lcd_fd==-1 || bmp_fd==-1)
    {
        printf("fail to open file\n");
        return -1;
    }

    /* 1.5.先获取bmp图片的像素数据 */ 
    lseek(bmp_fd, 54, SEEK_SET);  /* 偏移文件的光标 */
    char buf[800*480*3];
    bzero(buf, 800*480*3);    
    /* bzero是用来清空数组的 */ 
    read(bmp_fd, buf, 800*480*3);
    
    /* 2.写入获取到的像素数据到lcd屏幕文件中 */ 
    /*
    * 申请映射空间
    * 使用映射空间
    * 撤销映射空间 --》内存泄漏
    */
    int *mmap_start = (int *)mmap(NULL, 800*480*4, 
                                PROT_READ|PROT_WRITE, 
                                MAP_SHARED, lcd_fd, 0);
    if (mmap_start == (void *)-1)
    {
        printf("mmap error\n");
        return -2;
    }

    int x,y,n;
    for(y=0,n=0; y<480; y++)
    {
        for(x=0; x<800; x++,n+=3)
        {
            *(mmap_start+800*(479-y)+x) = buf[n+0]<<0|
                                          buf[n+1]<<8|
                                          buf[n+2]<<16|
                                          0<<24;
        }
    }

    munmap(mmap_start, 800*480*4);
    
    /* 3.关闭两个文件（lcd屏幕、bmp图片） */ 
    close(lcd_fd);
    close(bmp_fd);
    
    return 0;
}