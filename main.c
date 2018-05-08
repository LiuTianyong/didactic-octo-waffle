#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<time.h>
#include<conio.h>
#pragma comment(lib, "winmm.lib")                 //游戏音乐预处理
/*************************************************
Function:俄罗斯方块/猜数游戏 集合
Description:通过c语言打印特殊字符对俄罗斯方块游戏进行实现
Output:
Return:
Others:
auther:刘XX
*************************************************/

/*宏定义常量*/
#define N 28
#define Windows_N 19
#define Windows_M 11


/*定义全局变量*/

/*俄罗斯方块所用全局变量*/
int Move_x, Move_y, score, Mark_variate, Next;		//x，y坐标的移动  得分 
int Map[Windows_N][Windows_M];						//地图的大小
int Gamespeed = 300;											//沉睡打印时间300毫秒
char key1, key;														//用户输入的键值记录

/*猜数游戏所用全局变量*/
int g, z;																	//次数评价
int playerinput;														//玩家输入值
char playerinput2;													//玩家是否继续玩 输入的值
int random_num;													//猜数游戏随机数赋值变量
int Out_diamongds[28][6] = {							    //7种方块，加上旋转总共28种 即28种图形
											{ 0,-1,0,-2,1,0 },{ 0,1,1,0,2,0 },{ -1,0,0,1,0,2 },{ 0,-1,-1,0,-2,0 },
											{ 0,-1,0,1,-1,0 },{ 0,1,1,0,-1,0 },{ 1,0,0,-1,0,1 },{ 1,0,-1,0,0,-1 },
											{ -1,1,0,1,1,0 },{ 0,-1,1,0,1,1 },{ -1,0,0,-1,1,-1 },{ -1,-1,-1,0,0,1 },
											{ -1,0,0,1,1,1 },{ 0,1,1,-1,1,0 },{ -1,0,0,1,1,1 },{ 0,1,1,-1,1,0 },
											{ -1,0,0,-1,0,-2 },{ -1,0,-2,0,0,1 },{ 0,1,0,2,1,0 },{ 0,-1,1,0,2,0 },
											{ 0,1,1,0,1,1 },{ 0,-1,1,0,1,-1 },{ -1,0,0,-1,-1,-1 },{ -1,0,-1,1,0,1 },
											{ 0,1,0,2,0,3 },{ 1,0,2,0,3,0 },{ 0,-1,0,-2,0,-3 },{ -1,0,-2,0,-3,0 }
										}; 

/*定义函数原型*/
void gotoxy(int x, int y);				//对gotoxy函数实现  即定位光标功能
void Gameover();						//游戏结束函数
void ShowMap(int s);					//打印地图
int move_diamongds();				//方块的移动
void init(int s);							//Move_x，Move_y是全局变量，标记了移动方块的位置
int Border(int x, int y, int s);		//判断是否出界
void fun_score();						//得分情况打印，方块向下平移
int fun_for(int, int);						//使用函数递归调用	完成两个循环中的调用
void login();								//注册 登陆窗口
void MenuSptrip_main();			//主菜单函数
void guess_game();					//猜数游戏主体函数
void function_case1_1();				//猜数游戏初级
void function_case1_2();				//猜数游戏中级
void function_case1_3();				//猜数游戏高级
void function_case2_2();			//猜字母游戏
void windows_1();						//退出窗口

int main()
{
	PlaySound(TEXT("注册.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	int SW_i;								//菜单变量
	login();
	scanf_s("%d", &SW_i);
	if (SW_i == 1)
	{
		PlaySound(TEXT("bgm.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		system("mode con cols=70 lines=50");
		move_diamongds();			//俄罗斯方块
	}
	else if (SW_i = 2)
	{
		guess_game();					//猜数游戏
	 }
	return 0;
}
/*************************************************
Function: goto函数实现			
Description:  定位光标函数实现
Calls:
Called By:各个需要定位光标打印的函数体
*************************************************/
void gotoxy(int x, int y)			  //对gotoxy函数的实现   即定位屏幕光标
{
	COORD c;
	c.X = x - 1; 
	c.Y = y - 1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
/*************************************************
Function: 游戏结束函数
Description:  判断游戏结束
Calls:
Called By: fun_for(int set, int s)
*************************************************/
void Gameover()					 //俄罗斯方块游戏结束的判断
{
	int i, j, flag = 0;
	int Exit_Markets;
	for (j = 1; j<Windows_M - 1; j++)
	{
		if (Map[1][j] != 0)
		{
			flag = 1;
			break;
		}
	}
	if (flag == 1)
	{
		for (i = 1; i<Windows_N - 1; i++)
		{
			gotoxy(2, i + 1);
			for (j = 1; j<Windows_M - 1; j++)
			{
				printf("■");
			}
			puts("");
		}
		gotoxy(7, 9);
		PlaySound(TEXT("游戏结束.wav"), NULL, SND_FILENAME | SND_ASYNC );
		printf("游戏结束");
		gotoxy(1, Windows_N + 1);
		gotoxy(Windows_M, Windows_N);
		printf("\n\n");
		printf("是否再来一局\n");
		printf("1.是\t2.否\n");
		printf(">>");
		scanf_s("%d", &Exit_Markets);
		if (Exit_Markets == 1)
		{
			PlaySound(TEXT("bgm.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
			move_diamongds();			//俄罗斯方块
		}
		else
		{
			exit(0);
		}
		system("pause");
		exit(0);									//退出程序
	}
}
/*************************************************
Function: 地图打印
Description:  地图的实现
Calls:
Called By:fun_score()		init(int s)
*************************************************/
void ShowMap(int s)						//程序核心1  定位光标输出
{
	int i, j;
	gotoxy(1, 1);
	if (s != -1)
	{
		for (i = 0; i<Windows_N; i++)
		{
			for (j = 0; j<Windows_M; j++)
			{
				if (i == 0 && j == 0 || i == 0 && j == Windows_M - 1 || j == 0 && i == Windows_N - 1 || j == Windows_M - 1 && i == Windows_N - 1)
				{
					printf(" ");
				}
				else if (i == 0 || i == Windows_N - 1)
				{
					printf("▄");
				}
				else if (j == 0 || j == Windows_M - 1)
				{
					printf("▌");
				}
				else if (Map[i][j] == 2)
				{
					printf("■");
				}/*方块是否满一行进行判断*/
				else if (i == Move_x + Out_diamongds[s][0] && j == Move_y + Out_diamongds[s][1] ||i == Move_x + Out_diamongds[s][2] && j == Move_y + Out_diamongds[s][3] ||i == Move_x + Out_diamongds[s][4] && j == Move_y + Out_diamongds[s][5] ||i == Move_x && j == Move_y)
				{
					printf("■");
				}
				else if (Map[i][j] == 0)			//方块满行消失
				{
					printf("  ");
				}
			}
			if (i == 1)
			{
				printf("   方块:");
			}
			if (i == 11)
			{
				printf("   得分 :   %d", score);
			}
			if (i == 14)
			{
				printf("   级别 :   %d", score / 100 + 1);
			}
			puts("");
		}
	}
	else
	{
		Mark_variate = 1;
		for (i = 0; i<Windows_N; i++)
		{
			for (j = 0; j<Windows_M; j++)
			{
				if (i == 0 && j == 0 || i == 0 && j == Windows_M - 1 || j == 0 && i == Windows_N - 1 || j == Windows_M - 1 && i == Windows_N - 1)
				{
					printf(" ");
				}else if (i == 0 || i == Windows_N - 1)
				{
					printf("▄");
				}
				else if (j == 0 || j == Windows_M - 1)
				{
					printf("▌");
				}
				else if (Map[i][j] == 2)
				{
					printf("■");
				}
				else if (Map[i][j] == 0)
				{
					printf("  ");
				}	
			}

			/*在固定坐标打印   下一个输出方块  得分情况  目前游戏级别*/
			if (i == 1)
			{
				printf("   方块:");
			}
			if (i == 11)
			{
				printf("   得分 :   %d", score);
			}
			if (i == 14)
			{
				printf("   级别 :   %d", score / 100 + 1);
			}
			puts("");
		}
	}

	/*方块的处理，清除打印*/
	for (i = 2; i <= 10; i++)
	{
		for (j = 23; j <= 34; j++)
		{
			gotoxy(j + 1, i + 1);
			printf("  ");
		}
	}
	gotoxy(29, 6);
	printf("■");
	for (i = 0; i<6; i = i + 2)
	{
		gotoxy(29 + 2 * Out_diamongds[Next][i + 1], 6 + Out_diamongds[Next][i]);
		printf("■");
	}
	Sleep(Gamespeed);
}
/*************************************************
Function: 方块的移动
Description:   
Calls:
Called By:Gameover()	main()
*************************************************/
int move_diamongds()
{
	int  s, set = 1;							//随机数赋值变量
	int tmp;
	srand((unsigned)time(NULL));
	s = rand() % N;
	s = (s + N) % N;
	Next = rand() % N;
	Next = (Next + N) % N;
	system("cls");
	init(s);								   //Move_x，Move_y是全局变量，标记了移动方块的位置
	for (;;)
	{
		s = fun_for(set, s);
		key = _getch();				  //游戏核心2   获取键值方块移动
		if (key1 == -32 && key == 72)
		{
			tmp = s;
			s++;
			if (s % 4 == 0 && s != 0)
			{
				s = s - 4;
			}
			if (Border(Move_x, Move_y, s) != 1)
			{
				s = tmp;
			}
		}
		else if (key1 == -32 && key == 80 && Border(Move_x + 1, Move_y, s) == 1)
		{
			Move_x++;
		}
		else if (key1 == -32 && key == 75 && Border(Move_x, Move_y - 1, s) == 1)
		{
			Move_y--;
		}
		else if (key1 == -32 && key == 77 && Border(Move_x, Move_y + 1, s) == 1)
		{
			Move_y++;
		}
		else if (key == 27)
		{
			return 0;
		}
		key1 = key;
	}
	system("pause");
}
/*************************************************
Function: 标记函数 
Description:  
Calls:ShowMap(s);
Called By:move_diamongds()
*************************************************/
void init(int s)								 //初始化函数，Move_x，Move_y是全局变量，标记了移动方块的位置
{
	memset(Map, 0, sizeof(Map));
	int i, j;
	for (i = 0; i<Windows_N; i++)
	{
		for (j = 0; j<Windows_M; j++)
			if (i == Windows_N - 1 || j == 0 || j == Windows_M - 1)
				Map[i][j] = -1;
	}
	Move_x = 0;
	Move_y = 6;
	ShowMap(s);
}

/*************************************************
Function: Border(int x, int y, int s)	
Description:  判断是否出界
Calls:
Called By:move_diamongds()
*************************************************/
int Border(int x, int y, int s)				//判断是否出界，或者说是否合法
{
	if (Map[x][y] != 0)return 0;
	int i;
	for (i = 0; i<6; i = i + 2)
	{
		if (Map[x + Out_diamongds[s][i]][y + Out_diamongds[s][i + 1]] != 0)return 0;
	}
	return 1;
}
/*************************************************
Function: fun_score()
Description:  输出得分情况		游戏级别的升级
Calls:
Called By:fun_for(int set, int s)
*************************************************/
void fun_score()							 //得分情况打印，方块向下平移
{
	int i, j;
	for (i = 1; i<Windows_N - 1; i++)
	{
		int flag = 0;
		for (j = 1; j<Windows_M - 1; j++)
		{
			if (Map[i][j] != 2)
			{
				flag = 1;
				break;
			}
		}
		if (flag == 0)
		{
			int k = 3;
			while (k--)
			{
				gotoxy(2, i + 1);
				int _i;
				for (_i = 1; _i<Windows_M - 1; _i++)
				{
					if (Map[i][_i] == 2)
					{
						if (k % 2 == 1)printf("  ");
						else printf("■");
					}
				}
				Sleep(100);
			}
			int _i, _j;
			for (_i = i; _i>1; _i--)
			{
				for (_j = 1; _j<Windows_M - 1; _j++)Map[_i][_j] = Map[_i - 1][_j];
			}
			ShowMap(-1);
			score += 10;
			if (score % 100 == 0 && score != 0)
			{
				Gamespeed -= 50;			//将打印速度提升   
			}	
		}
	}
}
/*************************************************
Function: fun_for
Description:  
Calls:Gameover();	
Called By: main()
*************************************************/
int fun_for(int set, int s)
{
		int i;
		Mark_variate = 0;
		if (set == 0)
		{
			s = Next; Border;
			Next = rand() % N;
			Next = (Next + N) % N;
			Move_x = 0;
			Move_y = 6;
			set = 1;
		}

		while (!_kbhit())					//包含于include <conio.h>头文件     用于检查是否有键值输入
		{
			Gameover();					//游戏结束
			if (Border(Move_x + 1, Move_y, s) == 1)
			{
				Move_x++;
			}
			else
			{
				Map[Move_x][Move_y] = 2;
				for (i = 0; i < 6; i = i + 2)
				{
					Map[Move_x + Out_diamongds[s][i]][Move_y + Out_diamongds[s][i + 1]] = 2;
				}
				set = 0;
			}
			fun_score();
			if (Mark_variate != 1)
			{
				ShowMap(s);
			}
			s=fun_for(set, s);
		}
		return s;
}
/*************************************************
Function: 登陆注册函数
Description:  游戏主菜单
Calls:MenuSptrip_main()
Called By: login()
*************************************************/
void login()												
{
	system("color A1");
	int random_num;										//随机赋值变量
	int i = 0;
	int choice;													//用户选择登陆还是注册
	int user, password;									//用户注册 账号 密码
	int input_randomnum;								//玩家输入的验证码
	int input_user, input_PW;							//玩家输入的账号 密码
	int user_s, pow_s;										//对文件指针取出值进行赋值变量

	gotoxy(10, 1);
	printf("欢迎来到猜数游戏！\n");
	gotoxy(1, 3);
	printf("   1.登陆  2.注册\n>>");
	scanf_s("%d", &choice);
	if (choice == 1)											//登陆
	{
		system("cls");
		system("color A0");	
		PlaySound(TEXT("注册完成登陆.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		Sleep(1000);
		PlaySound(TEXT("注册.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		printf("请登录！\n");
	
		do {
			srand((unsigned)time(NULL));
			random_num = (rand()*rand()) % 10000 + 1;
			if (i == 3)
			{
				printf("你的次数已经用完，请核对信息后再来！");
				Sleep(1000);
				system("exit");
				exit(0);
			}
			i++;
			if (i>1)
			{
				printf("账户或密码不正确，请重新输入!\n");
				if (input_randomnum != random_num)
				{
					printf("验证码输入错误，请重新注册！\n");
				}
			}
			printf("账号： ");
			FILE *fpRead;
			scanf_s("%d", &input_user);
			fopen_s(&fpRead, "user.txt", "r");
			if (fpRead == NULL)
			{
				return ;
			}
			fscanf_s(fpRead, "%d ", &user);
			user_s = user;
			fclose(fpRead);

			//printf("******%d", user_s);									//测试代码无视 ——测试是否从文件取出账号
			printf("密码： ");
			scanf_s("%d", &input_PW);
			FILE *fpRead_;
			fopen_s(&fpRead_, "password.txt", "r");
			if (fpRead_ == NULL)
			{
				return ;
			}
			fscanf_s(fpRead_, "%d ", &password);
			pow_s = password;
			fclose(fpRead_);
			//printf("******%d", pow_s);									//测试代码无视 ——测试是否从文件取出密码
			printf("你的验证码是%d\t", random_num);
			scanf_s("%d", &input_randomnum);

		} while ((input_user != user_s || input_PW != pow_s || input_randomnum != random_num) && i<4);
		PlaySound(TEXT("注册完成登陆.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		Sleep(1000);
		PlaySound(TEXT("注册.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		printf("登陆成功！");
		Sleep(1000);
		system("cls");
		MenuSptrip_main();
	}
	else																	//注册
	{
		printf("请注册游戏账号\n");

		
		do {
			srand((unsigned)time(NULL));
			random_num = (rand()*rand()) % 10000 + 1;
			printf("请输入你要注册的账户（10位纯数字）：\n");
			FILE *fpWrite;
			fopen_s(&fpWrite, "user.txt", "w");
			if (fpWrite == NULL)
			{
				return ;
			}
			scanf_s("%d", &user);
			fprintf(fpWrite, "%d ", user);
			fclose(fpWrite);

			printf("请输入你的密码（10位纯数字）：\n");
			FILE *fpWrite_;
			fopen_s(&fpWrite_, "password.txt", "w");
			if (fpWrite == NULL)
			{
				return ;
			}
			scanf_s("%d", &password);
			fprintf(fpWrite_, "%d ", password);
			fclose(fpWrite_);
 
			printf("请输入验证码%d\t验证码：", random_num);
			scanf_s("%d", &input_randomnum);
			if (input_randomnum != random_num)
			{
				printf("验证码输入错误，请重新注册！\n");
			}
		} while (input_randomnum != random_num);
		system("cls");
		printf("你的账户是：%d\n你的密码是：%d\n请牢记！！！", user, password);
		Sleep(700);
		system("cls");
		system("color A0");
		printf("请登录！\n");
		PlaySound(TEXT("注册完成登陆.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		Sleep(1000);
		PlaySound(TEXT("注册.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		do {
			srand((unsigned)time(NULL));
			random_num = (rand()*rand()) % 10000 + 1;
			if (i == 3)
			{
				printf("你的次数已经用完，请核对信息后再来！");
				Sleep(1000);
				system("exit");
				exit(0);
			}
			i++;
			if (i>1)
			{
				printf("账户或密码不正确，请重新输入!\n");
				if (input_randomnum != random_num)
				{
					printf("验证码输入错误，请重新注册！\n");
				}
			}

			printf("账号： ");
			scanf_s("%d", &input_user);
			printf("密码： ");
			scanf_s("%d", &input_PW);
			printf("你的验证码是%d\t", random_num);
			scanf_s("%d", &input_randomnum);

		} while ((input_user != user || input_PW != password || input_randomnum != random_num) && i<4);
		PlaySound(TEXT("注册完成登陆.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		Sleep(1000);
		PlaySound(TEXT("注册.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		printf("登陆成功！");
		Sleep(1000);
		system("cls");
		MenuSptrip_main();
	}
}
/*************************************************
Function: MenuSptrip_main()
Description:  游戏主菜单
Calls: 
Called By: login()	
*************************************************/
void MenuSptrip_main()
{
	printf("**************************************************************\n");
	printf("**************** 欢  迎  进  入   游   戏 ********************\n");
	printf("*****                                                    *****\n");
	printf("*****                                                    *****\n");
	printf("*****            1.俄   罗  斯  方   块                  *****\n");
	printf("*****                                                    *****\n");
	printf("*****            2.猜    数   游    戏                   *****\n");
	printf("*****                                                    *****\n");
	printf("*****            3.退    出    游    戏                  *****\n");
	printf("*****                                                    *****\n");
	printf("*****                                                    *****\n");
	printf("**************************************************************\n");
	printf("**************************************************************\n");
}
/*************************************************
Function: guess_game()
Description:  猜数游戏主函数
Calls: function_case1_1()		void function_case1_2()		function_case1_3()		function_case2_2()
Called By: main()
*************************************************/
void guess_game()
{
	Sleep(1000);
	PlaySound(TEXT("猜数背景.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	system("color A1");
	int i = 0;//循环变量
	int x, y;//菜单变量
	system("cls");
	printf("**************************************************************\n");
	printf("************* 欢  迎  进  入  猜  字  游  戏 *****************\n");
	printf("*****                                                    *****\n");
	printf("*****                                                    *****\n");
	printf("*****            1.猜    字    游    戏                  *****\n");
	printf("*****                                                    *****\n");
	printf("*****            2.猜  字  母   游   戏                  *****\n");
	printf("*****                                                    *****\n");
	printf("*****            3.退    出    游    戏                  *****\n");
	printf("*****                                                    *****\n");
	printf("*****                                                    *****\n");
	printf("**************************************************************\n");
	printf("**************************************************************\n");
	scanf_s("%d", &x);
	system("cls");//系统清屏
	switch (x)
	{
	case 1:
		system("color B0");
		printf("**************************************************************\n");
		printf("************* 请   选   择   游   戏   难   度 ***************\n");
		printf("*****                                                    *****\n");
		printf("*****                                                    *****\n");
		printf("*****            1.初   级   难   度                     *****\n");
		printf("*****                                                    *****\n");
		printf("*****            2.中   级   难   度                     *****\n");
		printf("*****                                                    *****\n");
		printf("*****            3.高   级   难   度                     *****\n");
		printf("*****                                                    *****\n");
		printf("*****                                                    *****\n");
		printf("**************************************************************\n");
		printf("**************************************************************\n");
		scanf_s("%d", &y);
		system("cls");
		switch (y)
		{
		case 1://初级难度
			srand((unsigned)time(NULL));
			random_num = rand() % 10 + 1;
			function_case1_1();
			break;
		case 2://中级难度
			srand((unsigned)time(NULL));
			random_num = rand() % 100 + 1;
			function_case1_2();
			break;
		case 3://高级难度
			srand((unsigned)time(NULL));
			random_num = rand() % 1000 + 1;
			function_case1_3();
			break;
		default:
			break;
		}
		break;
	case 2://猜字母游戏
		system("color 20");
		srand((unsigned)time(NULL));
		random_num = rand() % 25 + 65;//随机数65~90
		function_case2_2();
		break;
	default:
		break;
	}
}
/*************************************************
Function: function_case1_1()
Description:  初级猜数游戏
Calls: windows_1()
Called By: guess_game()
*************************************************/
void function_case1_1()
{
	for (g = 1;; g++)
	{
		printf("第%d次输入！", g);
		printf("请输入1~10的数字来猜取：\n");
		scanf_s("%d", &playerinput);
		if (playerinput > random_num)
		{
			printf("你输入的数字大了哟！\t");
		}
		else if (playerinput < random_num)
		{
			printf("你输入的数字小了哟！\t");
		}
		else
		{
			system("cls");
			system("color 40");
			printf("恭喜你猜对了！！！\n");
			if (g == 1)
			{
				printf("大佬你已看破红尘，此难度以及不适合你了！你仅用了%d次哟！\n", g);
			}
			else if (1<g<4)
			{
				printf("大神还错哟！你仅用了%d次哟！\n", g);
			}
			else if (3<g<6)
			{
				printf("还可以哟！！你仅用了%d次哟！\n", g);
			}
			else
			{
				printf("请喝完六个核桃再来吧！！\n");
			}
			g = 0;
			windows_1();
			scanf_s("%d", &z);
			if (z == 1)
			{
				system("color B0");
				srand((unsigned)time(NULL));
				random_num = rand() % 10;

			}
			else
			{
				printf("回车后程序退出");
				break;
			}
			continue;
		}
	}
}
/*************************************************
Function: function_case1_2()
Description:  中级猜数游戏
Calls: windows_1()
Called By: guess_game()
*************************************************/
void function_case1_2()
{
	for (g = 1;; g++)
	{
		printf("第%d次输入！", g);
		printf("请输入1~100的数字来猜取：\n");
		scanf_s("%d", &playerinput);
		if (playerinput > random_num)
		{
			printf("你输入的数字大了哟！\t");
		}
		else if (playerinput < random_num)
		{
			printf("你输入的数字小了哟！\t");
		}
		else
		{
			system("cls");
			system("color 40");
			printf("恭喜你猜对了！！！\n");
			if (g == 1)
			{
				printf("你已超越六界之外！你仅用了%d次哟！\n", g);
			}
			else if (1<g<11)
			{
				printf("大神还错哟！你仅用了%d次哟！\n", g);
			}
			else if (10<g<30)
			{
				printf("还可以哟！！你仅用了%d次哟！\n", g);
			}
			else
			{
				printf("请喝完六个核桃再来吧！！\n", g);
			}
			g = 0;
			windows_1();
			scanf_s("%d", &z);
			if (z == 1)
			{
				system("color B0");
				srand((unsigned)time(NULL));
				random_num = rand() % 100 + 1;

			}
			else
			{
				printf("回车后程序退出");
				break;
			}
			continue;
		}
	}
}
/*************************************************
Function: function_case1_3()
Description:  高级猜数游戏
Calls: windows_1()
Called By: guess_game()
*************************************************/
void function_case1_3()
{
	for (g = 1;; g++)
	{
		printf("第%d次输入！", g);
		printf("请输入1~1000的数字来猜取：\n");
		scanf_s("%d", &playerinput);
		if (playerinput > random_num)
		{
			printf("你输入的数字大了哟！\t");
		}
		else if (playerinput < random_num)
		{
			printf("你输入的数字小了哟！\t");
		}
		else
		{
			system("cls");
			system("color 40");
			printf("恭喜你猜对了！！！\n");
			if (g == 1)
			{
				printf("老板请手头抱头交出外挂！你仅用了%d次哟！\n", g);
			}
			else if (1<g<31)
			{
				printf("大神还错哟！你仅用了%d次哟！\n", g);
			}
			else if (30<g<71)
			{
				printf("还可以哟！！你仅用了%d次哟！\n", g);
			}
			else
			{
				printf("请喝完六个核桃再来吧！！\n", g);
			}
			g = 0;
			windows_1();
			scanf_s("%d", &z);
			if (z == 1)
			{
				system("color B0");
				srand((unsigned)time(NULL));
				random_num = rand() % 1000 + 1;

			}
			else
			{
				printf("回车后程序退出");
				break;
			}
			continue;
		}
	}
}
/*************************************************
Function: function_case2_2()
Description: 猜字母游戏
Calls: windows_1()
Called By: guess_game()
*************************************************/
void function_case2_2()
{

	printf("请输入一个大写字母进行猜取：\n");
	for (g = 1;; g++)
	{
		printf("第%d次输入！", g);
		getchar();
		scanf_s("%c", &playerinput2);

		if (random_num>playerinput2)
		{
			printf("不对哟！它在你输入的字母的后面哟！请输入：\n");
		}
		else if (random_num<playerinput2)
		{
			printf("不对哟！它在你输入的字母的前面哟！请输入：\n");
		}
		else
		{
			system("cls");
			system("color 40");
			printf("恭喜你猜对了！！！\n");
			if (g == 1)
			{
				printf("你已超越六界之外！你仅用了%d次哟！\n", g);
			}
			else if (1<g<11)
			{
				printf("大神还错哟！你仅用了%d次哟！\n", g);
			}
			else if (10<g<30)
			{
				printf("还可以哟！！你仅用了%d次哟！\n", g);
			}
			else
			{
				printf("请喝完六个核桃再来吧！！\n", g);
			}
			g = 0;
			windows_1();
			scanf_s("%d", &z);
			if (z == 2)
			{
				printf("回车后程序退出");
				break;
			}
			else
			{
				system("color B0");
				srand((unsigned)time(NULL));
				random_num = rand() % 25 + 65;
			}
		}
	}
}
/*************************************************
Function: windows_1()
Description: 调用窗口  进行窗口间的切换
Calls: windows_1()
Called By: function_case1_1()		function_case1_2()		function_case1_3()		function_case2_1()
*************************************************/
void windows_1()
{
	printf("**************************************************************\n");
	printf("*************    是      否      继      续    ***************\n");
	printf("*****                                                    *****\n");
	printf("*****                                                    *****\n");
	printf("*****            1.是                                    *****\n");
	printf("*****                                                    *****\n");
	printf("*****            2.否                                    *****\n");
	printf("*****                                                    *****\n");
	printf("**************************************************************\n");
	printf("**************************************************************\n");
}
