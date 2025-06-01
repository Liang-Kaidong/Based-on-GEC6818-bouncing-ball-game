#include <stdio.h>

/*
* gcc main.c -o main
* gcc: 编译工具包
* main.c：编译对象
* -o: output 输出/生成一个二进制文件
* main：给即将生成的二进制文件取名叫main
* ./main：运行当前路径下名为main的程序
*/

int main(int argc, char const *argv[])
{
	printf("hello world\n");
	return 0;
}