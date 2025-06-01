#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    // 1. 打开文件
    int fd = open("/dev/fb0", O_RDWR);
    if (fd == -1)
    {
        printf("open file error\n");
        return -1;
    }
    
    // 2. 写入数据（颜色）
    // 德国国旗的尺寸，假设为 800x480 像素
    int width = 800;
    int height = 480;
    
    // 定义颜色
    int black = 0x000000;
    int red = 0xFF0000;
    int yellow = 0xFFFF00;
    
    // 绘制德国国旗
    // 黑色
    for (int i = 0; i < width * height / 3; ++i)
    {
        write(fd, &black, 4);
    }
    // 红色
    for (int i = 0; i < width * height / 3; ++i)
    {
        write(fd, &red, 4);
    }
    // 黄色
    for (int i = 0; i < width * height / 3; ++i)
    {
        write(fd, &yellow, 4);
    }
    
    // 3. 关闭文件
    int ret = close(fd);
    if (ret == -1)
    {
        printf("close file error\n");
        return -2;
    }
    
    return 0;
}
