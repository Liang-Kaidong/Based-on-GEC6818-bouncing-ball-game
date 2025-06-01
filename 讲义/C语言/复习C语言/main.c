#include <stdio.h>	// 告诉编译器，我们即将要使用哪个函数
/*
	编译： 把代码翻译成二进制给电脑看
	编辑： 写东西
*/ 


// int b = 2;

struct student{
	char id[20];
	int age;
	float high;

};

int main()
{
	// char data = 15;
	// printf("%d\n", data);

	int data = 15;
	printf("%c\n", data);
	// int         a;
	struct student zhangsan;
	// .  结构体变量访问内部成员的方式
	zhangsan.age = 18;
	zhangsan.high = 1.75;
	printf("年龄： %d\n", zhangsan.age);
	printf("身高： %f\n", zhangsan.high);
	printf("\n");
	// int         b;
	struct student lisi;
	lisi = zhangsan;	// b = a; 
	printf("年龄： %d\n", lisi.age);
	printf("身高： %f\n", lisi.high);

	// int arr[5] = {5,4,3,2,1};
	// printf("%d\n", arr[0]);
	// // 数组的下标：从0开始，到n-1结束
	// // 数组名字的含义：
	// // 1. 数组的名字就代表了数组首元素的地址
	// // 2. 数组的名字是可以解引用得到首元素的值 
	// // 3. 数组名字的+1等价于第二个元素的地址
	// printf("%p\n", &arr[0]);
	// printf("%p\n", arr);
	// printf("\n");
	
	// printf("%p\n", &arr[1]);
	// printf("%p\n", arr+1);
	// printf("%d\n", *arr);
	// printf("\n");
	
	// printf("%d\n", arr[1]);
	// printf("%d\n", *(arr+1));
	// 结论： *( +n) = [n];

	// int a = 10;
	// // 定义指针： 指针的类型要跟变量的数据类型一致
	// int *p;	// 指针存放的是地址
	// p = &a;	// & : 取地址符 
	// printf("%d\n", *p);
	// *p = 3;	// a = 3;
	// printf("%d\n", a);
	// // *p等价于a		*p: 解引用
	

	// int *q = NULL;
	// int *new_p;	// 野指针
	// *new_p = 1;	// 不支持的
	// printf("%d\n", *new_p);



	// 循环的三要素： 循环的起点、终点、步长
	// for(int i=1; i<10; i++)
	// {
	// 	printf("%d\n", i);
	// }

	// int i = 1;
	// while(i<10)
	// {
	// 	i++;
	// }

	// {	// 代码块
	// 	int a = 1;
	// }
	// b+=1;	// b = b+1;	b++ ++b
	// int a = 1;
	// printf("%d\n", a++);   // 1
	// printf("%d\n", ++a);   // 3





	// printf("hello world\n");

	// char a = 'A';
	// // 1. 给变量a赋值一个字符
	// // 2. 给变量a指向的空间赋值一个字符
	// printf("%hhd\n", a);
	
	return 0;

}