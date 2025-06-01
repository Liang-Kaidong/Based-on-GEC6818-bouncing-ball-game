#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <linux/input.h>
#include <pthread.h>
#include <stdlib.h>//cmp

struct game_info
{
	int lcd_fd;			// lcd屏幕文件的代号
	int *mmap_start;	// 映射空间的起始地址
	int ball_color;		// 球的颜色
	int back_color;		// 背景颜色
	int input_fd;		// 触摸屏文件的代号
	int board_color; 	// 板子的颜色
	int board_back_color; 	// 板子的背景颜色
	int length;			// 板子的长度
	int high;			// 板子的宽度
};

struct score_history {			//得分统计
	char name_icon_path[20];
	int score;
};

struct score_history stu[3] = {{"user_00.bmp",2},{"user_01.bmp",3},{ "user_02.bmp",9}}; //显示用户数量

int score = 0;
int flag_pause = 0; //游戏暂停

// 定义全局变量GI（存放游戏内所有信息的）
struct game_info GI;
int input_x, input_y;
pthread_t ball_id;		// 球的线程id号
pthread_t board_id;		// 板子的线程id号

int game_init()
{
	// 1. 打开屏幕文件
	GI.lcd_fd = open("/dev/fb0", O_RDWR);
	if (GI.lcd_fd == -1)
	{
		printf("打开lcd文件失败\n");
		return -1;
	}

	// 2. 申请映射空间
	GI.mmap_start = mmap(NULL, 800*480*4, PROT_READ|PROT_WRITE, MAP_SHARED, GI.lcd_fd, 0);
	if (GI.mmap_start == MAP_FAILED)
	{
		printf("mmap error\n");
		return -2;
	}

	// 3. 定义球的颜色
	GI.ball_color = 0x1C2BDB; //蓝色

	// 4. 定义背景颜色
	GI.back_color = 0xffffff; //白色

	// 5. 打开触摸屏文件 
	GI.input_fd = open("/dev/input/event0", O_RDWR);
	if (GI.input_fd == -1)
	{
		printf("open file error\n");
		return -3;
	}

	// 6. 定义板子的颜色
	GI.board_color = 0x8375A8; //紫色

	// 7. 定义板子的背景颜色
	GI.board_back_color = 0x9499A8; //灰色

	// 8. 定义板子的长度和宽度
	GI.length=150, GI.high=50;
	score = 0; //初始分数
}

int show_dights(int x,int y,int index){		//图片索引
	void anywhere_InsertPhoto();
	switch (index)
	{
	case 0:
		anywhere_InsertPhoto(x, y, 40, 40, "0.bmp");
	case 1:
		anywhere_InsertPhoto(x, y, 40, 40, "1.bmp");
		break;
	case 2:
		anywhere_InsertPhoto(x, y, 40, 40, "2.bmp");
		break;
	case 3:
		anywhere_InsertPhoto(x, y, 40, 40, "3.bmp");
		break;
	case 4:
		anywhere_InsertPhoto(x, y, 40, 40, "4.bmp");
		break;
	case 5:
		anywhere_InsertPhoto(x, y, 40, 40, "5.bmp");
		break;
	case 6:
		anywhere_InsertPhoto(x, y, 40, 40, "6.bmp");
		break;
	case 7:
		anywhere_InsertPhoto(x, y, 40, 40, "7.bmp");
		break;
	case 8:
		anywhere_InsertPhoto(x, y, 40, 40, "8.bmp");
		break;
	case 9:
		anywhere_InsertPhoto(x, y, 40, 40, "9.bmp");
		break;
	default:
		break;
	}
}

int cmp(const void* a, const void* b) {
	/* *(stu*)a是因为：a是个void *类型，要先
	用(stu*）将它转成stu*类型，然后再用*取值，
	变成stu类型，才能比较大小。*/
	struct score_history c = *(struct score_history*)a;
	struct score_history d = *(struct score_history*)b;
	//按成绩升序排序 
	return c.score - d.score;
}

void *move_ball(void *arg)
{
	void anywhere_InsertPhoto();
	anywhere_InsertPhoto(80, 90, 40, 40, "0.bmp");//定义分数0初始位置，分辨率为40*40 

	// 球的信息
	int x0=300, y0=190, r=40;
	// 球心的运动状态
	int x_flag=1, y_flag=1;	// x_flag为0的时候，x0要--， x_flag为1的时候，x0要++

	int x, y;
	while(1)
	{
		// (x-x0)^2+(y-y0)^2<=r^2
		for(y=y0-r; y<=y0+r; y++)
		{
			for(x=x0-r;x<=x0+r;x++)
			{
				if ((x-x0)*(x-x0)+(y-y0)*(y-y0)<r*r)
				{
					*(GI.mmap_start+800*y+x) = GI.ball_color;
				}
				else
					*(GI.mmap_start+800*y+x) = GI.back_color;
			}
		}

		// 控制小球速率的两种方式： 修改休眠时间、++变成别的
		//此处通过计分到某一值时，改变球的速度
		int speed;
		if (score < 5) {
			speed = 3000;
		}
		else if(score <7){
			speed = 1000;
		}
		if (score > 8) {
			speed = 500;
		}
		
		usleep(speed);
		if (flag_pause==0)
		{
			if (x_flag == 1)
			{
				x0++;
			}
			else
			{
				x0--;
			}
			if (y_flag == 1)
			{
				y0++;
			}
			else
			{
				y0--;
			}
		}
		

		if (y0+r==380 && x0>=input_x-GI.length/2 && x0<=input_x+GI.length/2)	//更新计数板
		{
			y_flag = 0;
			score++;
			if(score>=10){
				show_dights(80, 90, score/10); //整除获得十位
				show_dights(80+40+5, 90, score%10); //求余获得个位
			}
			else
			{
				show_dights(80+40+5, 90, score);
			}
			

			for (int i = 0; i < 3; i++) {
				if (strcmp(stu[i].name_icon_path, "user_00.bmp") == 0) {
					stu[i].score = score;//显示某一用户的当前分数
				}
			}
			
			qsort(stu, 3, sizeof(stu[0]), cmp);

			for (int i = 0; i < 3; i++) {
				//printf("%s score:%d\n", stu[i].name,stu[i].score);
				anywhere_InsertPhoto(30, 320+ i*(32+10), 90, 32, stu[i*-1+2].name_icon_path);
				show_dights(130, 320 + i * (32 + 10), stu[i * -1 + 2].score);
			}
		};

		//判断碰撞时，小球位移
		if (x0+r==799)	x_flag = 0;
		if (y0-r==0)	y_flag = 1;
		if (x0-r==210)	x_flag = 1;
		if (y0+r>=400)
		{
			stu[0].score = score;//*end score
			printf("你的得分是 = %d\n", score);
			pthread_cancel(board_id);
			break;
		}
	}
}


void *move_board(void *arg)
{
	int x,y;
	int length=GI.length, high=GI.high;
	for(y=380; y<380+high; y++)
	{
		for(x=210; x<800; x++)
		{
			*(GI.mmap_start+800*y+x) = GI.board_back_color;
		}
	}
	struct input_event buf;

	while(1)
	{
		read(GI.input_fd, &buf, sizeof(buf));
		if(buf.type==EV_ABS && buf.code==ABS_X)
		{
			input_x = buf.value;
			// x = x*800/1024;	// 黑色框的同学才需要这行代码
		}
		if(buf.type==EV_ABS && buf.code==ABS_Y)
		{
			input_y = buf.value;
			// y = y*480/600;	// 黑色框的同学才需要这行代码
		}

		//printf("(%d, %d)\n", input_x, input_y);
		//button
		if (input_x > 56 && input_x < 278 && input_y > 188 && input_y < 334)	//实现游戏暂停功能
		{
			if (buf.type == EV_KEY && buf.code == BTN_TOUCH && buf.value == 0) // 抬手
			{
				if (flag_pause == 0)
				{
					flag_pause = 1;
				}
				else
				{
					flag_pause = 0;
				}
			}
			
		}
		//printf("(%d, %d)\n", input_x, input_y);
		if (input_y>480 && input_y<= 480+high)	//画板子
		{
			for(y=380; y<380+high; y++)
			{
				for(x=208; x<800; x++)
				{
					if(x>=input_x-length/2 && x<=input_x+length/2)
					{
						*(GI.mmap_start+800*y+x) = GI.board_color;
					}
					else
						*(GI.mmap_start+800*y+x) = GI.board_back_color;
				}
			}
		}
		
	}
}

/*
	函数功能：显示任意大小的bmp图片在lcd屏幕的任意位置上
	参数： picname: bmp图片的文件名	mv_x: 移动的x坐标	mv_y: 移动的y坐标
	返回值：成功返回0，失败返回-1
	调用示例： show_new_bmp("1.bmp", 40, 40);	
	// 从屏幕的（50，50）位置开始显示1.bmp这张图片
*/
void anywhere_InsertPhoto(int x_setoff, int y_setoff, int width_photo, int high_photo, char* pathname)
{
	//printf("ccc\n");
	//打开bmp图片
	int bmpfd = open(pathname, O_RDWR);
	if (bmpfd == -1)
	{
		perror("open bmp2");
		return;
	}

	//打开液晶屏的设备
	int lcdfd = open("/dev/fb0", O_RDWR);
	if (lcdfd == -1)
	{
		perror("open");
		return;
	}

	//准备buf储存图片的像素点信息
	char bmpbuf[width_photo * high_photo * 3];
	int  tmpbuf[width_photo * high_photo];
	int  lastbuf[width_photo * high_photo];

	//跳过图片头信息的54个字节
	lseek(bmpfd, 54, SEEK_SET);

	//保存每一行的无效字节
	int invalid_byte = (4 - (width_photo * 3) % 4) % 4;

	//按行读取bmp图片的信息，跳过无效字节
	for (int i = 0; i < high_photo; ++i)
	{
		read(bmpfd, &bmpbuf[i * width_photo * 3], 3 * width_photo);
		lseek(bmpfd, invalid_byte, SEEK_CUR);
	}

	//将读取到的三字节像素点RGB填充到lcd的四字节像素点ABGR上
	for (int i = 0; i < width_photo * high_photo; ++i)
	{
		tmpbuf[i] = 0x00 << 24 | bmpbuf[3 * i + 2] << 16 |
		bmpbuf[3 * i + 1] << 8 | bmpbuf[3 * i];
	}

	//将读取到的四字节像素点按行中间翻转
	for (int i = 0; i < width_photo; ++i)
		for (int j = 0; j < high_photo; ++j)
		{
			lastbuf[(high_photo - 1 - j) * width_photo + i] =
				tmpbuf[j * width_photo + i];
		}

	//内存映射
	int* mmpfd = mmap(NULL, 800 * 480 * 4, PROT_READ | PROT_WRITE, MAP_SHARED, lcdfd, 0);
	for (int i = 0; i < high_photo; ++i)
		for (int j = 0; j < width_photo; ++j)
		{
			mmpfd[(i + y_setoff) * 800 + x_setoff + j] =
			lastbuf[i * width_photo + j];
		}

	//释放内存
	//不能随意释放，内存发生偏移
	//munmap(mmpfd,800*480*4);

	//关闭文件
	close(bmpfd);
	close(lcdfd);
	return;
}

int input_fun()
{
	// 1. 打开触摸屏文件
	int fd = open("/dev/input/event0", O_RDWR);
	if (fd == -1)
	{
		printf("open file error\n");
		return -1;
	}

	// 2. 读取触摸屏文件存储的信息
	struct input_event buf;
	int x, y;
	while (1)
	{
		// sizeof是运算符，求目标所占内存的大小
		read(fd, &buf, sizeof(buf));
		if (buf.type == EV_ABS && buf.code == ABS_X)
		{
			x = buf.value;
			// x = x*800/1024;	// 黑色框的同学才需要这行代码
		}
		if (buf.type == EV_ABS && buf.code == ABS_Y)
		{
			y = buf.value;
			// y = y*480/600;	// 黑色框的同学才需要这行代码
		}
		if (buf.type == EV_KEY && buf.code == BTN_TOUCH && buf.value == 0)
		{
			// 抬手打印坐标
			input_x = x;
			input_y = y;
			//printf("坐标：(%d, %d)\n", input_x, input_y);
			break;	// 结束死循环
		}
	}


	// 3. 关闭文件
	close(fd);
	return 0;
}

	int game_progress() {
	int game_mode = 0;
	struct input_event buf;
	int isgame = 1; //退出游戏
	while(isgame){
		if (game_mode == 0) {
			anywhere_InsertPhoto(0,0,800, 480,"menu0.bmp" ); //开始游戏选择框
			game_mode = 1;
		}
		if (game_mode == 1) {//游戏button监听开启
			input_fun();
			//printf("(%d, %d)\n", input_x, input_y); x: +77  +123  y：-187 -107
			if (input_x > 233 && input_x < 551 && input_y > 172 && input_y < 288) { //开始游戏按钮位置
				game_mode = 2; //开始游戏

			}
			if (input_x > 946 && input_x < 1050 && input_y > 0 && input_y < 30) { //退出按钮位置
				printf("button new3\n\n");
				isgame = 0;
				anywhere_InsertPhoto(0, 0, 800, 480, "system0.bmp");
				anywhere_InsertPhoto(60 + 50 + 10, 30, 90, 32, "user_00.bmp");
			}
		}
		if (game_mode == 2) { 	//游戏开始
			game_init();	// 游戏初始化
			anywhere_InsertPhoto(0, 0, 800, 480, "menu1.bmp");
	
			pthread_create(&ball_id, NULL, move_ball, NULL); 
			pthread_create(&board_id, NULL, move_board, NULL);

			pthread_join(ball_id, NULL);
			pthread_join(board_id, NULL);
			//分数
			game_mode = 0;
		}
	}
	
	return 1;
}

	int album_progress(){
	anywhere_InsertPhoto(0, 0, 800, 480, "album_menu0.bmp");

	int i = 1;
	int mode = 0; 
	int isrun = 1;
	while (isrun)
	{
		input_fun();//->1 2 3
		printf("%d", mode);
		if (mode == 0) {
			if (input_x > 103 && input_x < 284) { mode = 2; } //自动播放
			if (input_x > 429 && input_x < 578) //返回主界面
			{
				isrun = 0;
				anywhere_InsertPhoto(0, 0, 800, 480, "system0.bmp");
				anywhere_InsertPhoto(60 + 50 + 10, 30, 90, 32, "user_00.bmp");
			}
			if (input_x > 634 && input_x < 859) { mode = 1; } //手动播放
		}
		if (mode == 1) {
			// 点击左边
			if (input_x < 300)
			{
				printf("左边\n");
				i--;
				if (i == 0)
				{
					i = 10;
				}
				switch (i)
				{
				case 1:
					anywhere_InsertPhoto(0, 0, 800, 480, "01.bmp");
					break;
				case 2:
					anywhere_InsertPhoto(0, 0, 800, 480, "02.bmp");
					break;
				case 3:
					anywhere_InsertPhoto(0, 0, 800, 480, "03.bmp");
					break;
				case 4:
					anywhere_InsertPhoto(0, 0, 800, 480, "04.bmp");
					break;
				case 5:
					anywhere_InsertPhoto(0, 0, 800, 480, "05.bmp");
					break;
				case 6:
					anywhere_InsertPhoto(0, 0, 800, 480, "06.bmp");
					break;
				case 7:
					anywhere_InsertPhoto(0, 0, 800, 480, "07.bmp");
					break;
				case 8:
					anywhere_InsertPhoto(0, 0, 800, 480, "08.bmp");
					break;
				case 9:
					anywhere_InsertPhoto(0, 0, 800, 480, "09.bmp");
					break;
				case 10:
					anywhere_InsertPhoto(0, 0, 800, 480, "10.bmp");
					break;
				}
			}

			// 点击中间
			if (input_x > 300 && input_x < 600)
			{
				mode = 0;
				anywhere_InsertPhoto(0, 0, 800, 480, "album_menu0.bmp");
			}

			// 点击右边
			if (input_x > 634)
			{
				printf("右边\n");
				i++;
				if (i == 11)
				{
					i = 1;
				}
				switch (i)
				{
				case 1:
					anywhere_InsertPhoto(0, 0, 800, 480, "01.bmp");
					break;
				case 2:
					anywhere_InsertPhoto(0, 0, 800, 480, "02.bmp");
					break;
				case 3:
					anywhere_InsertPhoto(0, 0, 800, 480, "03.bmp");
					break;
				case 4:
					anywhere_InsertPhoto(0, 0, 800, 480, "04.bmp");
					break;
				case 5:
					anywhere_InsertPhoto(0, 0, 800, 480, "05.bmp");
					break;
				case 6:
					anywhere_InsertPhoto(0, 0, 800, 480, "06.bmp");
					break;
				case 7:
					anywhere_InsertPhoto(0, 0, 800, 480, "07.bmp");
					break;
				case 8:
					anywhere_InsertPhoto(0, 0, 800, 480, "08.bmp");
					break;
				case 9:
					anywhere_InsertPhoto(0, 0, 800, 480, "09.bmp");
					break;
				case 10:
					anywhere_InsertPhoto(0, 0, 800, 480, "10.bmp");
					break;
				}
			}
		}
		if (mode == 2) {
			while (1) {
				//input_fun();
				//printf("(%d, %d)\n", input_x, input_y);
				if (input_x > 429 && input_x < 578)
				{
					mode = 0;
					anywhere_InsertPhoto(0, 0, 800, 480, "album_menu0.bmp");
					break;
				}

				i++;
				printf("%d", i);
				if (i == 11) { i = 1; }
				if (i < 11)
				{
				switch (i)
				{
				case 1:
					anywhere_InsertPhoto(0, 0, 800, 480, "01.bmp");
					break;
				case 2:
					anywhere_InsertPhoto(0, 0, 800, 480, "02.bmp");
					break;
				case 3:
					anywhere_InsertPhoto(0, 0, 800, 480, "03.bmp");
					break;
				case 4:
					anywhere_InsertPhoto(0, 0, 800, 480, "04.bmp");
					break;
				case 5:
					anywhere_InsertPhoto(0, 0, 800, 480, "05.bmp");
					break;
				case 6:
					anywhere_InsertPhoto(0, 0, 800, 480, "06.bmp");
					break;
				case 7:
					anywhere_InsertPhoto(0, 0, 800, 480, "07.bmp");
					break;
				case 8:
					anywhere_InsertPhoto(0, 0, 800, 480, "08.bmp");
					break;
				case 9:
					anywhere_InsertPhoto(0, 0, 800, 480, "09.bmp");
					break;
				case 10:
					anywhere_InsertPhoto(0, 0, 800, 480, "10.bmp");
					break;
					if (input_x > 300 && input_x < 600)			//无法实现自动播放任意位置暂停回主界面？
					{
						mode = 0;
						anywhere_InsertPhoto(0, 0, 800, 480, "album_menu0.bmp");
					}
				}
				}
				usleep(500000);
				
			}


		}

	}

	return 0;
}

int main(int argc, char const *argv[]) //定义初始主界面
{
	anywhere_InsertPhoto(0, 0, 800, 480, "system0.bmp");
	anywhere_InsertPhoto(60+50+10, 30, 90, 32, "user_00.bmp");
	struct input_event buf;
	while (1)
	{
		input_fun();//抬手
		printf("(%d, %d)\n", input_x, input_y);
		if (input_x > 112 && input_x < 363 && input_y > 387 && input_y < 501)	//选择电子相册模式
		{
			printf("button 1\n\n");
			album_progress();
		}
		if (input_x > 634 && input_x < 859 && input_y > 381 && input_y < 480)	//选择游戏模式
		{
			printf("button 2\n\n");
			game_progress();
		}
	}
	return 0;
}