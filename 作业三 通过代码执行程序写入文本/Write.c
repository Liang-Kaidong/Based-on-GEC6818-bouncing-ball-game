#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

// 怎么对1.txt写入数据呢？
int main(int argc, char const *argv[])
{
	// 1. 打开文件   句柄
	int fd = open("./1.txt", O_RDWR);
	if (fd == -1)
	{
		printf("打开文件失败\n");
		return -1;
	}

	// 2. 写入数据（自动保存）
	char buf[20] = "许浪儿子";
	// UTF-8 编码格式下： 一个汉字三个字节
	write(fd, buf, 20);
	write(fd, buf, 20);
	// 3. 关闭文件
	close(fd);
	return 0;
}