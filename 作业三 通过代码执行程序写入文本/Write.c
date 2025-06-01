/*************************************************************************************************************
File name:Write.c
Author:KD
Version:V_1.0
Build date: 2024-04-26
Description:This C code implements the function of writing data to a file. It opens the 1.txt file in the 
			current directory using the open() function in read-write mode. If the opening fails, it outputs 
			an error message and returns. The write() function is used twice to write 20 bytes of Chinese text
			to the file, and finally, the file is closed using the close() function.
Others:Usage requires preservation of original author attribution.
Log:1.First create file.
*************************************************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/* 怎么对1.txt写入数据呢？ */ 
int main(int argc, char const *argv[])
{
	/* 1.打开文件   句柄 */ 
	int fd = open("./1.txt", O_RDWR);
	if (fd == -1)
	{
		printf("打开文件失败\n");
		return -1;
	}

	/* 2.写入数据（自动保存） */ 
	char buf[20] = "许浪儿子";
	/* UTF-8编码格式下：一个汉字三个字节 */ 
	write(fd, buf, 20);
	write(fd, buf, 20);

	/*3.关闭文件*/ 
	close(fd);
	
	return 0;
}