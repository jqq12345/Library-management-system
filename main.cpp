# include "windows.h"
# include "math.h" //数学运算函数库
# include "stdio.h" //标准输入输出函数库
# include "stdlib.h" //标准函数库
# include "string.h" //字符串函数库
# include "conio.h" //屏幕操作函数库
//与按键有关的宏定义
#define LEFT 65 //光标左移
#define RIGHT 68 //光标右移
#define DOWN 83 //光标下移
#define UP 87 //光标上移
#define ESC 27 //Esc键：取消菜单打开操作
#define H 72 // F1键：打开文件菜单
#define J 74//F2键：打开编辑菜单
#define K 75//F3键：打开帮助菜单
#define ENTER 13 //回车键：换行
//与Book菜单选项有关的宏定义
#define ADD_BOOK 100
#define QUERY_BOOK 101
#define MODIFY_BOOK 102
#define DEL_BOOK 103
#define SORT_BOOK 104
#define COUNT_BOOK 105
/*与Reader菜单选项相关的宏定义*/
#define ADD_READER 200
#define QUERY_READER 201
#define MODIFY_READER 202
#define DEL_READER 203
#define SORT_READER 204
#define COUNT_READER 205
//与B&R菜单选项有关的宏定义，B表示借书，R表示还书
#define BORROW_BOOK 300
#define RETURN_BOOK 301
#define EXIT  302
//与book记录格式化输出有关的宏定义
#define HEADER1 "|Number    |Name      |Author    |Publish   |Price| F|Reader    |Total|\n" 
#define FORMAT1 "|%-10s|%-10s|%-10s|%-10s|%.2f|%2d|%-10s|%5d| \n"
#define DATA1 lll->data.num,lll->data.name,lll->data.author,lll->data.publish,lll->data.price,lll->data.borrow_flag,lll->data.reader,lll->data.total_num
//与reader记录格式输出有关的宏定义
#define HEADER2 "|Number      |Name      |Sex |Age |Tele           |Total|\n"
#define FORMAT2 "|%-12s|%-10s|%-4s|%4d|%15s|%5d| \n"
#define DATA2 lll->data.num,lll->data.name,lll->data.sex,lll->data.age,lll->data.tele,lll->data.total_num
int saveflag = 0; //是否需要存盘的全局标志变量
#define clrscr() system("cls")
//定义与图书有关的数据结构
typedef struct book //标记为book
{
	char num[15]; //图书编号
	char name[15]; //图书名
	char author[15]; //图书作者
	char publish[15]; //出版社
	float price; //图书定价
	int borrow_flag; //图书是否借出，1表示借出，0表示未借出
	char reader[12]; //借阅人编号
	int total_num; //图书被借次数 
};
//定义与读者有关的数据结构
typedef struct reader //标记为reader
{
	char num[12];//读者编号
	char name[15];//读者姓名
	char sex[4];//读者性别，M为男性，F为女性
	int age;//读者年龄
	char tele[15];//读者联系电话
	int total_num;//读者目前已借阅图书册数 
};
//定义每条图书记录的数据结构，标记为book_node
typedef struct book_node
{
	struct book data;//数据域
	struct book_node* next;//指针域 
}Book_Node, * Book_Link; //*Book_Node为Book_node类型的结构变量， *Book_Link为Book_node类型的指针变量
typedef struct reader_node
{
	struct reader data;    //数据域
	struct reader_node* next;   //指针域 
}Reader_Node, * Reader_Link;  //*Reader_Node为readder_node类型的结构变量， *Reader_Link为reader_node类型的指针变量
void drawmain();
void drawmenu(int m, int n);
int menuctrl();
Book_Node* Locate(Book_Link l, char findmess[], char nameornum[]);
Reader_Node* LocateReader(Reader_Link l, char findmess[], char nameornum[]);
void stringinput(char* t, int lens, char* notice);
void AddBook(Book_Link l);
void AddReader(Reader_Link l);
void QueryBook(Book_Link l);
void QueryReader(Reader_Link l);
void DelBook(Book_Link l);
void DelReader(Reader_Link L);
void ModifyBook(Book_Link l);
void ModifyReader(Reader_Link l);
void CountBook(Book_Link l);
void CountReader(Reader_Link l);
void SortBook(Book_Link l);
void SortReader(Reader_Link l);
void SaveBook(Book_Link l);
void SaveReader(Reader_Link l);
void BorrowBook(Book_Link l, Reader_Link ll);
void ReturnBook(Book_Link l, Reader_Link ll);
void gotoxy(int x, int y);
void insline();
int wherex();
int wherey();
void modeset(int w, int h)
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD size = { w,h };
	SetConsoleScreenBufferSize(hOut, size);
	SMALL_RECT rc = { 1,1,w,h };
	SetConsoleWindowInfo(hOut, true, &rc);
	system("cls");
	return;
}
void mainmenu();
void main()
{
	modeset(90, 35);
	Book_Link l;  //定义图书有关的指针
	Reader_Link ll;   //定义读者有关的指针
	FILE* fp1, * fp2;   //*fp1为指向图书的文件指针，fp2为指向读者的文件指针
	char ch;  //保存（y，Y，n，N）  
	int count1 = 0, count2 = 0;  //分别保存 图书文件或读者文件中的记录条数
	Book_Node* p, * r;  //定义图书记录指针变量
	Reader_Node* p2, * r2;  //定义读者记录指针变量
	int B;  //保存用户选择的子菜单项 
	drawmain();  //显示主窗口 
	/*打开图书文件book，将其调入链表中存储*/
	l = (Book_Node*)malloc(sizeof(Book_Node));
	if (!l)
	{
		clrscr(); gotoxy(2, 3);
		printf("\n allocate memory failure");//如果没有申请到打印提示信息 
		return;//返回主界面 
	}
	l->next = NULL;
	r = l;
	fp1 = fopen("E:\\book", "ab+");//以追加方式打开一个二进制文件，可读可写，若此文件不存在，会创建此文件
	if (fp1 == NULL)
	{
		clrscr(); gotoxy(2, 3);
		printf("\n=====>can not open file!\n");
		exit(0);
	}
	while (!feof(fp1))  /*feof()函数检测流上的文件结束符，若文件结束，则返回非零的值，否则返回零*/
	{
		p = (Book_Node*)malloc(sizeof(Book_Node));
		if (!p)
		{
			clrscr(); gotoxy(2, 3);
			printf("memory malloc failure!\n");//没有申请成功 
			exit(0); //退出 
		}
		if (fread(p, sizeof(Book_Node), 1, fp1) == 1)//一次从文件中读取一条图书记录 
		{
			p->next = NULL;
			r->next = p;
			r = p;//r指针向后移一个位置 
			count1++;
		}
	}
	fclose(fp1);//关闭文件
	printf("\n=====>open file sucess,the total records number is:%d.\n", count1);
	p = r;
	//打开图书文件reader，将其调入链表中存储
	ll = (Reader_Node*)malloc(sizeof(Reader_Node));
	if (!ll)
	{
		printf("\n allocate memory failure");//如没有申请到，打印提示信息
		return;
	}
	ll->next = NULL;
	r2 = ll;
	fp2 = fopen("E:\\reader", "ab+");//以追加方式打开一个二进制文件，可读可写，若此文件不存在，会创建此文件
	if (fp2 == NULL)
	{
		printf("\n====>can not open file!\n");
		exit(0);
	}
	while (!feof(fp2))
	{
		p2 = (Reader_Node*)malloc(sizeof(Reader_Node));
		if (!p2)
		{
			printf("memory malloc failure!\n");//没有申请成功
			exit(0); //退出 
		}
		if (fread(p2, sizeof(Reader_Node), 1, fp2) == 1)//一次从文件中读取一条图书记录
		{
			p2->next = NULL;
			r2->next = p2;
			r2 = p2;
			count2++;
		}
	}
	fclose(fp2);//关闭文件
	printf("\n=====>open file sucess,the total records number is:%d.\n", count2);
	p2 = r2;
	while (1)
	{
		B = menuctrl();
		switch (B)
		{
		case ADD_BOOK:AddBook(l); break;//增加图书记录
		case QUERY_BOOK:QueryBook(l); break;//查询图书记录
		case MODIFY_BOOK:ModifyBook(l); break;//修改图数记录
		case DEL_BOOK:DelBook(l); break;//删除图书记录
		case SORT_BOOK:SortBook(l); break;//排序图书记录
		case COUNT_BOOK:CountBook(l); break;//统计图书记录
		case ADD_READER:AddReader(ll); break;//增加读者记录
		case QUERY_READER:QueryReader(ll); break;//查询读者记录
		case MODIFY_READER:ModifyReader(ll); break;//修改读者记录
		case DEL_READER:DelReader(ll); break;//删除读者记录
		case SORT_READER:SortReader(ll); break;//排序读者记录
		case COUNT_READER:CountReader(ll); break;//统计读者记录
		case BORROW_BOOK:BorrowBook(l, ll); break;//借书
		case RETURN_BOOK:ReturnBook(l, ll); break;//还书
		case EXIT: //退出系统	
		{
			clrscr();  mainmenu(); gotoxy(3, 3);
			cprintf("====>Are you really exit the Book Management System?(y/n):");
			scanf("%c", &ch);
			if (ch == 'y' || ch == 'Y')
			{
				SaveBook(l); SaveReader(ll); exit(0);
			}
		}
		}
		clrscr();
		drawmain();/*绘制系统主界面*/
	}
}
//绘制系统主界面 
void drawmain()               /*绘制系统主界面*/
{
	int i;
	/*打印伪窗口竖线*/
	for (i = 0; i <= 26; i++)
	{
		gotoxy(1, i);
		cprintf("%c", '|');
		gotoxy(79, i);
		cprintf("%c", '|');
	}
	/*打印伪窗口横线*/
	gotoxy(2, 0);	insline();
	gotoxy(2, 2);	insline();
	gotoxy(2, 24);	insline();
	gotoxy(2, 26);	insline();
	/*打印信息*/
	gotoxy(7, 2);	printf("%c  %c Book %c  %c", '|', '<', '>', '|');
	gotoxy(27, 2);	printf("%c  %c Reader %c  %c", '|', '<', '>', '|');
	gotoxy(47, 2);	printf("%c  %c B&R %c  %c", '|', '<', '>', '|');
	gotoxy(30, 8);	printf("press H -> Book");
	gotoxy(30, 10);	printf("press J -> Reader");
	gotoxy(30, 12);	printf("press K -> B&R");
	gotoxy(5, 25);	printf("Book Managment System");
	gotoxy(65, 25); printf("Version 2.0");
	gotoxy(65, 28);
}
void mainmenu()               /*绘制系统主界面*/
{
	int i;
	/*打印菜单内容*/
	for (i = 0; i <= 26; i++)
	{
		gotoxy(1, i);
		cprintf("%c", '#');
		gotoxy(79, i);
		cprintf("%c", '#');
	}
	gotoxy(2, 0);	insline();
	gotoxy(2, 2);	insline();
	gotoxy(2, 24);	insline();
	gotoxy(2, 26);	insline();
	gotoxy(7, 1);	printf("%c  %c Book %c  %c", '|', '<', '>', '|');
	gotoxy(27, 1);	printf("%c  %c Reader %c  %c", '|', '<', '>', '|');
	gotoxy(47, 1);	printf("%c  %c B&R %c  %c", '|', '<', '>', '|');
	gotoxy(5, 25);	printf("Book Managment System");
	gotoxy(65, 25); printf("Version 2.0");
	gotoxy(65, 28);
}
//绘制菜单
void drawmenu(int m, int n) {
	int i;
	if (m % 3 == 0)
	{
		system("cls");
		/*打印边框*/
		for (i = 0; i < 10; i++)
		{
			gotoxy(7, 2 + i);	 cprintf("%c", '*');
			gotoxy(20, 2 + i); cprintf("%c", '*');
		}
		gotoxy(7, 11);  cprintf("**************");
		/*打印菜单内容*/
		for (i = 0; i <= 26; i++)
		{
			gotoxy(1, i);
			cprintf("%c", '#');
			gotoxy(79, i);
			cprintf("%c", '#');
		}
		gotoxy(2, 0);	insline();
		gotoxy(2, 2);	insline();
		gotoxy(2, 24);	insline();
		gotoxy(2, 26);	insline();
		gotoxy(7, 1);	printf("%c  %c Book %c  %c", '|', '<', '>', '|');
		gotoxy(27, 1);	printf("%c  %c Reader %c  %c", '|', '<', '>', '|');
		gotoxy(47, 1);	printf("%c  %c B&R %c  %c", '|', '<', '>', '|');
		gotoxy(9, 3);	cprintf("Add      ");
		gotoxy(9, 4);	cprintf("Query    ");
		gotoxy(9, 5);	cprintf("Modify   ");
		gotoxy(9, 6);	cprintf("Delete   ");
		gotoxy(9, 7);	cprintf("Sort     ");
		gotoxy(9, 8);	cprintf("Count    ");
		/*打印选项*/
		switch (n % 6) {
		case 0:gotoxy(9, 3); cprintf("Add     &"); break;
		case 1:gotoxy(9, 4); cprintf("Query   &"); break;
		case 2:gotoxy(9, 5); cprintf("Modify  &"); break;
		case 3:gotoxy(9, 6); cprintf("Delete  &"); break;
		case 4:gotoxy(9, 7); cprintf("Sort    &"); break;
		case 5:gotoxy(9, 8); cprintf("Count   &"); break;
		}
		gotoxy(5, 25);	printf("Book Managment System");
		gotoxy(65, 25); printf("Version 2.0");
		gotoxy(65, 28);
	}
	if (m % 3 == 1)
	{
		system("cls");
		/*打印边框*/
		for (i = 1; i < 10; i++)
		{
			gotoxy(27, 2 + i);	cprintf("%c", '*');
			gotoxy(42, 2 + i);	cprintf("%c", '*');
		}
		gotoxy(27, 11);  cprintf("**************");//
		                          
		/*打印菜单内容*/
		for (i = 0; i <= 26; i++)
		{
			gotoxy(1, i);
			cprintf("%c", '#');
			gotoxy(79, i);
			cprintf("%c", '#');
		}
		gotoxy(2, 0);	insline();
		gotoxy(2, 2);	insline();
		gotoxy(2, 24);	insline();
		gotoxy(2, 26);	insline();
		gotoxy(7, 1);	printf("%c  %c Book %c  %c", '|', '<', '>', '|');
		gotoxy(27, 1);	printf("%c  %c Reader %c  %c", '|', '<', '>', '|');
		gotoxy(47, 1);	printf("%c  %c B&R %c  %c", '|', '<', '>', '|');
		gotoxy(29, 3);	cprintf("Add      ");
		gotoxy(29, 4);	cprintf("Query    ");
		gotoxy(29, 5);	cprintf("Modify   ");
		gotoxy(29, 6);	cprintf("Delete   ");
		gotoxy(29, 7);	cprintf("Sort     ");
		gotoxy(29, 8);	cprintf("Count    ");
		/*打印选项*/
		switch (n % 6) {
		case 0:gotoxy(29, 3); cprintf("Add     &"); break;
		case 1:gotoxy(29, 4); cprintf("Query   &"); break;
		case 2:gotoxy(29, 5); cprintf("Modify  &"); break;
		case 3:gotoxy(29, 6); cprintf("Delete  &"); break;
		case 4:gotoxy(29, 7); cprintf("Sort    &"); break;
		case 5:gotoxy(29, 8); cprintf("Count   &"); break;
		}
		gotoxy(5, 25);	printf("Book Managment System");
		gotoxy(65, 25); printf("Version 2.0");
		gotoxy(65, 28);
	}
	if (m % 3 == 2)
	{
		system("cls");
		/*打印边框*/
		for (i = 1; i < 10; i++)//列
		{
			gotoxy(47, 2 + i);	cprintf("%c", '*');
			gotoxy(60, 2 + i);	cprintf("%c", '*');
		}
		gotoxy(47, 11);	cprintf("**************");
		/*打印菜单内容*/
		for (i = 0; i <= 26; i++)
		{
			gotoxy(1, i);
			cprintf("%c", '#');
			gotoxy(79, i);
			cprintf("%c", '#');
		}
		gotoxy(2, 0);	insline();
		gotoxy(2, 2);	insline();
		gotoxy(2, 24);	insline();
		gotoxy(2, 26);	insline();
		gotoxy(7, 1);	printf("%c  %c Book %c  %c", '|', '<', '>', '|');
		gotoxy(27, 1);	printf("%c  %c Reader %c  %c", '|', '<', '>', '|');
		gotoxy(47, 1);	printf("%c  %c B&R %c  %c", '|', '<', '>', '|');
		gotoxy(49, 3);	cprintf("Borrow   ");
		gotoxy(49, 5);	cprintf("Return   ");
		gotoxy(49, 7);	cprintf("Exit     ");
		/*打印选项*/
		switch (n % 3) {
		case 0:gotoxy(49, 3); cprintf("Borrow  &"); break;
		case 1:gotoxy(49, 5); cprintf("Return  &"); break;
		case 2:gotoxy(49, 7); cprintf("Exit    &"); break;
		}
		gotoxy(5, 25);	printf("Book Managment System");
		gotoxy(65, 25); printf("Version 2.0");
		gotoxy(65, 28);
	}
}
int menuctrl()           /*菜单控制*/
{
	int A, B, value, flag = 36, a, b, i;
	while (1)
	{
		/*获取键值*/
		while (1)
		{
			if (_kbhit())
			{
				A = getch();
				A = getch();
				if (A == 'H' || A == 'J' || A == 'K') break;
			}
		}
		if (A == 'H')
		{
			drawmenu(0, flag);
			value = 300;
		}
		if (A == 'J') {
			drawmenu(1, flag);
			value = 301;
		}
		if (A == 'K') {
			drawmenu(2, flag);
			value = 302;
		}
		while (1)
		{
			/*获取键值*/
			while (1)
			{
				if (_kbhit()) {
					B = getch();
					if (B == ESC || B == ENTER) {
						break;
					}
					B = getch();
					if (B == LEFT || B == RIGHT || B == UP || B == DOWN) break;
				}
			}
			/*返回上一级菜单*/
			if (B == ESC)
			{
				system("cls");
				drawmain();
				break;
			}
			if (flag == 0) flag = 36;
			if (value == 0) value = 300;
			/*返回功能键值*/
			if (B == ENTER)
			{
				if (value % 3 == 0) b = 6;
				if (value % 3 == 1) b = 6;
				if (value % 3 == 2) b = 3;
				a = (value % 3) * 10 + flag % b;
				if (a == 0) return ADD_BOOK;
				if (a == 1) return QUERY_BOOK;
				if (a == 2) return MODIFY_BOOK;
				if (a == 3) return DEL_BOOK;
				if (a == 4) return SORT_BOOK;
				if (a == 5) return COUNT_BOOK;
				if (a == 10) return ADD_READER;
				if (a == 11) return QUERY_READER;
				if (a == 12) return MODIFY_READER;
				if (a == 13) return DEL_READER;
				if (a == 14) return SORT_READER;
				if (a == 15) return COUNT_READER;
				if (a == 20) return BORROW_BOOK;
				if (a == 21) return RETURN_BOOK;
				if (a == 22) return EXIT;
			}
			else if (B == UP) { /*切换选项*/
				drawmenu(value, --flag);
			}
			else if (B == DOWN) {
				drawmenu(value, ++flag);
			}
			else if (B == LEFT) {/*切换菜单*/
				drawmenu(--value, flag);
			}
			else {
				drawmenu(++value, flag);
			}
		}
	}

}
//记录查找定位 
Book_Node* Locate(Book_Link l, char findmess[], char nameornum[])//图书定位 
/**********************************************************************************
作用：用于定位链表中符合要求的节点，并返回指向该节点的指针
参数：findmess[]保存要查找的具体内容；nameornum[]保存按什么查找；
************************************************************************************/
{
	Book_Node* r;
	if (strcmp(nameornum, "num") == 0)//按图书编号查询
	{
		r = l->next;
		while (r)
		{
			if (strcmp(r->data.num, findmess) == 0)//若找到findmess值的图书编号
				return r;
			r = r->next;
		}
	}
	else if (strcmp(nameornum, "name") == 0)//按图书名查询
	{
		r = l->next;
		while (r)
		{
			if (strcmp(r->data.name, findmess) == 0)//若找到findmess值的图书名
				return r;
			r = r->next;
		}
	}
	return 0;//若未找到，返回一个空指针 
}
Reader_Node* LocateReader(Reader_Link l, char findmess[], char nameornum[])//读者定位 
{
	Reader_Node* r;
	if (strcmp(nameornum, "num") == 0)//按读者编号查询
	{
		r = l->next;
		while (r)
		{
			if (strcmp(r->data.num, findmess) == 0)//若找到findmess值的读者编号
				return r;
			r = r->next;
		}
	}
	else if (strcmp(nameornum, "name") == 0)//按读者姓名查询
	{
		r = l->next;
		while (r)
		{
			if (strcmp(r->data.name, findmess) == 0)//若找到findmess值的读者 
				return r;
			r = r->next;
		}
	}
	return 0;//若未找到，返回一个空指针 
}

//格式化输入数据
void stringinput(char* t, int lens, char* notice)//输入字符串，并进行长度验证（长度<lens）
{
	POINT cp;
	char n[255];
	int x, y;
	do
	{
		printf(notice);//显示提示信息
		GetCursorPos(&cp);
		x = cp.x; y = cp.y;
		scanf("%s", n);//输入字符串
		if (strlen(n) > lens)//进行长度校验，超过lens值重新输入 
		{
			gotoxy(0,0); printf("exceed the required length ! \n");
		}
	} while (strlen(n) > lens);
	strcpy(t, n);//将输入的字符串复制到字符串t中 
}

//增加记录
void AddBook(Book_Link l)
{
	Book_Node* p, * r, * s;//实现添加操作的临时的结构体指针变量
	char ch, flag = 0, num[10];
	float temp;
	r = l;
	s = l->next;
	clrscr();
	while (r->next != NULL)
		r = r->next;//将指针移至于链表最末尾，准备添加记录
	while (1)//一次可输入多条记录，直至输入图书编号为0的记录结点添加操作
	{
		while (1)//输入图书编号，保证该图书编号没有被使用，若输入图书编号为0，则退出添加记录操作
		{
			clrscr();
			mainmenu();
			gotoxy(3, 4);
			stringinput(num, 15, "input book number(press '0' return menu):");//格式化输入图书编号并检验
			flag = 0;
			if (strcmp(num, "0") == 0)//输入为0，则退出添加操作，返回主界面
			{
				return;
			}
			while (s)//查询该图书编号是否已经存在，若存在则要求重新输入一个未被占用的图书编号
			{
				if (strcmp(s->data.num, num) == 0)
				{
					flag = 1;
					break;
				}
				s = s->next;
			}
			if (flag == 1)//提示用户是否重新输入
			{
				gotoxy(3, 5);
				getchar();
				printf("====>The number %s is existing,please try again(y/n)?", num);
				scanf("%c", &ch);
				if (ch == 'y' || ch == 'Y')
					continue;
				else
					return;
			}
			else
			{
				break;
			}
		}
		p = (Book_Node*)malloc(sizeof(Book_Node));//申请内存空间
		if (!p)
		{
			printf("\n allocate memory failure ");//如没有申请到，打印提示信息
			return;
		}
		//给图书记录赋值
		strcpy(p->data.num, num);//将字符串num复制到p->data.num中
		gotoxy(3, 5); stringinput(p->data.name, 20, "Book Name:");
		gotoxy(3, 6); stringinput(p->data.author, 20, "Book Author:");
		gotoxy(3, 7); stringinput(p->data.publish, 20, "Book Publishing Company:");
		gotoxy(3, 8); printf("Book Price:"); scanf("%f", &temp);
		p->data.price = temp;
		p->data.borrow_flag = 0;//图书初始为未借出，1表示借出
		strcpy(p->data.reader, " ");
		p->data.total_num = 0;//图书被借次数初始为0
		gotoxy(3, 9); printf(">>>>press any key to start next record!");
		getchar(); getchar();
		p->next = NULL;//表明这是链表的尾部结点
		r->next = p;//将新建的结点加入链表尾部中
		r = p;
		saveflag = 1;
	}
	return;
}
void AddReader(Reader_Link l)
{
	Reader_Node* p, * r, * s;//实现添加操作的临时的结构体指针变量
	char ch, flag = 0, num[10];
	int temp;
	r = l;
	s = l->next;
	clrscr();
	while (r->next != NULL)
		r = r->next;//将指针移至于链表最末尾，准备添加记录
	while (1)//一次可输入多条记录，直至输入读者编号为0的记录结点添加操作
	{
		while (1)//输入读者编号，保证该读者编号没有被使用，若输入读者编号为0，则退出添加记录操作
		{
			clrscr();
			mainmenu();
			gotoxy(3, 4);
			stringinput(num, 15, "input reader number(press '0' return menu):");//格式化输入读者编号并检验
			flag = 0;
			if (strcmp(num, "0") == 0)//输入为0，则退出添加操作，返回主界面
			{
				return;
			}
			s = l->next;
			while (s)//查询该读者编号是否已经存在，若存在则要求重新输入一个未被占用的读者编号
			{
				if (strcmp(s->data.num, num) == 0)
				{
					flag = 1;
					break;
				}
				s = s->next;
			}
			if (flag == 1)//提示用户是否重新输入
			{
				gotoxy(3, 5);
				getchar();
				printf("====>The number %s is existing,please try again(y/n)?", num);
				scanf("%c", &ch);
				if (ch == 'y' || ch == 'Y')
					continue;
				else
					return;
			}
			else
			{
				break;
			}
		}
		p = (Reader_Node*)malloc(sizeof(Book_Node));//申请内存空间
		if (!p)
		{
			printf("\n allocate memory failure ");//如没有申请到，打印提示信息
			return;
		}
		//给图书记录赋值
		strcpy(p->data.num, num);//将字符串num复制到p->data.num中
		gotoxy(3, 5); stringinput(p->data.name, 15, "Reader Name:");
		gotoxy(3, 6); stringinput(p->data.sex, 15, "Reader Sex:");
		gotoxy(3, 7); stringinput(p->data.tele, 15, "Reader Tele:");
		gotoxy(3, 8); printf("Reader Age:"); scanf("%d", &temp);
		p->data.age = temp;
		p->data.total_num = 0;//读者目前已借图书册数初始为0
		gotoxy(3, 9); printf(">>>>press any key to start next record!");
		getchar(); getchar();
		p->next = NULL;//表明这是链表的尾部结点
		r->next = p;//将新建的结点加入链表尾部中
		r = p;
		saveflag = 1;
	}
	return;
}
//查询记录
void QueryBook(Book_Link l)//按图书编号或图书书名查询
{
	int select;//1:按图书编号查 2：按图书名查，其他：返回主界面 
	char searchinput[20];//保存用户输入的查询内容
	Book_Node* p;
	if (!l->next)//若链表为空
	{
		clrscr();
		mainmenu();
		gotoxy(3, 5);
		printf("\n====>No Book Record!\n");
		getchar(); getchar();
		return;
	}
	clrscr();
	mainmenu();
	gotoxy(3, 5);
	printf("=====>l Query by book number		=====>2 Query by book name");
	gotoxy(3, 6);
	printf("please choice[1,2]:");
	scanf("%d", &select);
	if (select == 1)//按图书编号查询
	{
		gotoxy(3, 7);
		stringinput(searchinput, 15, "input the existing book number:");
		p = Locate(l, searchinput, "num");
		//在l中查找图书编号为searchinput值的节点，并返回节点的指针
		if (p)//若p!=NULL
		{
			gotoxy(3, 8); printf("------------------------------------------------------");
			gotoxy(3, 9); printf("Book Number:%s", p->data.num);
			gotoxy(3, 10); printf("Book Name:%s", p->data.name);
			gotoxy(3, 11); printf("Book Author:%s", p->data.author);
			gotoxy(3, 12); printf("Book Publishing Company:%s", p->data.publish);
			gotoxy(3, 13); printf("Book Price:%.2f", p->data.price);
			gotoxy(3, 14); printf("Book Borrow_Flag(1:borrowed,0:un-borrowed):%d", p->data.borrow_flag);
			gotoxy(3, 15); printf("Book Current Reader:%s", p->data.reader);
			gotoxy(3, 16); printf("Total Number of Book Borrowed:%d", p->data.total_num);
			gotoxy(3, 17); printf("------------------------------------------------------");
			gotoxy(3, 18); printf("press any key to return");
			getchar(); getchar();
		}
		else
		{
			gotoxy(3, 8); printf("====>Not find this book!\n");
			getchar(); getchar();
		}
	}
	else if (select == 2)//按图书名查询
	{
		gotoxy(3, 7);
		stringinput(searchinput, 15, "input the existing book name:");
		p = Locate(l, searchinput, "name");    /*用户选择2，通过名字查找，即在name字段中查找*/
		if (p)
		{
			gotoxy(3, 8); printf("------------------------------------------------------");
			gotoxy(3, 9); printf("Book Number:%s", p->data.num);
			gotoxy(3, 10); printf("Book Name:%s", p->data.name);
			gotoxy(3, 11); printf("Book Author:%s", p->data.author);
			gotoxy(3, 12); printf("Book Publishing Company:%s", p->data.publish);
			gotoxy(3, 13); printf("Book Price:%.2f", p->data.price);
			gotoxy(3, 14); printf("Book Borrow_Flag(1:borrowed,0:un-borrowed):%d", p->data.borrow_flag);
			gotoxy(3, 15); printf("Book Current Reader:%s", p->data.reader);
			gotoxy(3, 16); printf("Total Number of Book Borrowed:%d", p->data.total_num);
			gotoxy(3, 17); printf("------------------------------------------------------");
			gotoxy(3, 18); printf("press any key to return");
			getchar(); getchar();
		}
		else
		{
			gotoxy(3, 8); printf("====>Not find this book!\n");
			getchar(); getchar();
		}
	}
	else
	{
		gotoxy(3, 8); printf("*****Error:input has wrong! press any key to continue******");
		getchar(); getchar();
	}
}
void QueryReader(Reader_Link l)
{
	int select;//1:按图书编号查 2：按图书名查，其他：返回主界面 
	char searchinput[20];//保存用户输入的查询内容
	Reader_Node* p;
	if (!l->next)//若链表为空
	{
		clrscr();
		mainmenu();
		gotoxy(3, 5);
		printf("\n====>No Reader Record!\n");
		getchar(); getchar();
		return;
	}
	clrscr();
	mainmenu();
	gotoxy(3, 5);
	printf("=====>l Query by reader number		=====>2 Query by reader name");
	gotoxy(3, 6);
	printf("please choice[1,2]:");
	scanf("%d", &select);
	if (select == 1)//按读者编号查询
	{
		gotoxy(3, 7);
		stringinput(searchinput, 15, "input the existing reader number:");
		p = LocateReader(l, searchinput, "num");
		//在l中查找图书编号为searchinput值的节点，并返回节点的指针
		if (p)//若p!=NULL
		{
			gotoxy(3, 8); printf("------------------------------------------------------");
			gotoxy(3, 9); printf("Reader Number:%s", p->data.num);
			gotoxy(3, 10); printf("Reader Name:%s", p->data.name);
			gotoxy(3, 11); printf("Reader Sex:%s", p->data.sex);
			gotoxy(3, 12); printf("Reader Age:%d", p->data.age);
			gotoxy(3, 13); printf("Reader Tele:%s", p->data.tele);
			gotoxy(3, 14); printf("Total Number of books that the Reader has Borrowed:%d", p->data.total_num);
			gotoxy(3, 15); printf("------------------------------------------------------");
			gotoxy(3, 18); printf("press any key to return");
			getchar(); getchar();
		}
		else
		{
			gotoxy(3, 8); printf("====>Not find this reader!\n");
			getchar(); getchar();
		}
	}
	else if (select == 2)//按读者名查询
	{
		gotoxy(3, 7);
		stringinput(searchinput, 15, "input the existing reader name:");
		p = LocateReader(l, searchinput, "name");
		if (p)
		{
			gotoxy(3, 8); printf("------------------------------------");
			gotoxy(3, 9); printf("Reader Number:%s", p->data.num);
			gotoxy(3, 10); printf("Reader Name:%s", p->data.name);
			gotoxy(3, 11); printf("Reader Sex:%s", p->data.sex);
			gotoxy(3, 12); printf("Reader Age:%d", p->data.age);
			gotoxy(3, 13); printf("Reader Tele:%s", p->data.tele);
			gotoxy(3, 14); printf("Total Number of books that the Reader has Borrowed:%d", p->data.total_num);
			gotoxy(3, 15); printf("------------------------------------");
			gotoxy(3, 18); printf("press any key to return");
			getchar(); getchar();
		}
		else
		{
			gotoxy(3, 8); printf("====>Not find this Reader!\n");
			getchar(); getchar();
		}
	}
	else
	{
		gotoxy(3, 8); printf("*****Error:input has wrong! press any key to continue******");
		getchar(); getchar();
	}
}
//删除记录
void DelBook(Book_Link l)
{
	int sel;
	Book_Node* p, * r;
	char findmess[20];
	if (!l->next)
	{
		clrscr();
		mainmenu();
		gotoxy(3, 5);
		printf("\n====>No book record!\n");
		getchar();
		return;
	}
	clrscr();
	mainmenu();
	gotoxy(3, 5);
	printf("=====>l Delete by book number		=====>2 Delete by book name");
	gotoxy(3, 6);
	printf("please choice[1,2]:");
	scanf("%d", &sel);
	if (sel == 1)
	{
		gotoxy(3, 7);
		stringinput(findmess, 10, "input the existing book number:");
		p = Locate(l, findmess, "num");
		if (p)//p!=NULL
		{
			r = l;
			while (r->next != p)
				r = r->next;
			r->next = p->next;//将p所指节点从链表中去除 
			free(p);//释放内存空间
			gotoxy(3, 9);
			printf("=====>delete success!");
			getchar(); getchar();
			saveflag = 1;
		}
		else
		{
			gotoxy(3, 8); printf("=====>Not find this book!\n");
			getchar(); getchar();
		}
	}
	else if (sel == 2)//先按图书名查询到该记录所在的节点
	{
		stringinput(findmess, 15, "input the existing book name:");
		p = Locate(l, findmess, "name");
		if (p)
		{
			r = l;
			while (r->next != p)
				r = r->next;
			r->next = p->next;
			free(p);
			gotoxy(3, 9);
			printf("=====>delete success!\n");
			getchar(); getchar();
			saveflag = 1;
		}
		else
		{
			gotoxy(3, 9); printf("=====>Not find this book!\n");
			getchar(); getchar();
		}
	}
	else
	{
		gotoxy(3, 9); printf("*****Error:input has wrong! press any key to continue******");
		getchar(); getchar();
	}
}
void DelReader(Reader_Link l)
{
	int sel;
	Reader_Node* p, * r;
	char findmess[20];
	if (!l->next)
	{
		clrscr();
		mainmenu();
		gotoxy(3, 5);
		printf("\n====>No Reader record!\n");
		getchar();
		return;
	}
	clrscr();
	mainmenu();
	gotoxy(3, 5);
	printf("=====>l Delete by Reader number		=====>2 Delete by Reader name");
	gotoxy(3, 6);
	printf("please choice[1,2]:");
	scanf("%d", &sel);
	if (sel == 1)
	{
		gotoxy(3, 7);
		stringinput(findmess, 10, "input the existing Reader number:");
		p = LocateReader(l, findmess, "num");
		if (p)//p!=NULL
		{
			r = l;
			while (r->next != p)
				r = r->next;
			r->next = p->next;//将p所指节点从链表中去除 
			free(p);//释放内存空间
			gotoxy(3, 9);
			printf("=====>delete success!");
			getchar(); getchar();
			saveflag = 1;
		}
		else
		{
			gotoxy(3, 9); printf("=====>Not find this Raeder!\n");
			getchar(); getchar();
		}
	}
	else if (sel == 2)//先按图书名查询到该记录所在的节点
	{
		stringinput(findmess, 15, "input the existing Reader name:");
		p = LocateReader(l, findmess, "name");
		if (p)
		{
			r = l;
			while (r->next != p)
				r = r->next;
			r->next = p->next;
			free(p);
			gotoxy(3, 9);
			printf("=====>delete success!\n");
			getchar(); getchar();
			saveflag = 1;
		}
		else
		{
			gotoxy(3, 9); printf("=====>Not find this Reader!\n");
			getchar(); getchar();
		}
	}
	else
	{
		gotoxy(3, 9); printf("*****Error:input has wrong! press any key to continue******");
		getchar(); getchar();
	}
}
//修改记录
void ModifyBook(Book_Link l)
{
	Book_Node* p;
	char findmess[20];
	float temp;
	if (!l->next)
	{
		clrscr();
		mainmenu();
		gotoxy(3, 5);
		printf("\n=====>No book record!\n");
		getchar();
		return;
	}
	clrscr();
	mainmenu();
	gotoxy(3, 5);
	stringinput(findmess, 10, "input the existing book number；");//输入并检验该图书编号
	p = Locate(l, findmess, "num");//查询该节点
	if (p)//若p!=NULL,表明已经找到该节点
	{
		gotoxy(3, 6); printf("------------------------------------------------------");
		gotoxy(3, 7); printf("Book Number:%s", p->data.num);
		gotoxy(3, 8); printf("Book Name:%s", p->data.name);
		gotoxy(3, 9); printf("Book Author:%s", p->data.author);
		gotoxy(3, 10); printf("Book Publishing Company:%s", p->data.publish);
		gotoxy(3, 11); printf("Book Price:%.2f", p->data.price);
		gotoxy(3, 12); printf("Book Borrow_Flag(1:borrowed,0:un-borrowed):%d", p->data.borrow_flag);
		gotoxy(3, 13); printf("Book Current Reader:%s", p->data.reader);
		gotoxy(3, 14); printf("Total Number of Book Borrowed:%d", p->data.total_num);
		gotoxy(3, 15); printf("------------------------------------------------------");
		gotoxy(3, 16); printf("please modify book recorder:");
		gotoxy(3, 17); stringinput(p->data.name, 15, "Book Name:");
		gotoxy(3, 18); stringinput(p->data.author, 15, "Book Author:");
		gotoxy(3, 19); stringinput(p->data.publish, 15, "Book Publishing Company:");
		gotoxy(3, 20); printf("Book Price:"); scanf("%f", &temp); p->data.price = temp;
		gotoxy(3, 21); printf("------------------------------------------------------");
		gotoxy(3, 22); printf("=====>modify success!"); getchar(); getchar();
		saveflag = 1;
	}
	else
	{
		gotoxy(3, 6); printf("=====>Not find this book!\n"); getchar(); getchar();
	}

}
void ModifyReader(Reader_Link l)
{
	Reader_Node* p;
	char findmess[20];
	int temp;
	if (!l->next)
	{
		clrscr();
		mainmenu();
		gotoxy(3, 5);
		printf("\n=====>No Reader record!\n");
		getchar();
		return;
	}
	clrscr();
	mainmenu();
	gotoxy(3, 5);
	stringinput(findmess, 10, "input the existing Reader number；");//输入并检验该读者编号
	p = LocateReader(l, findmess, "num");//查询该节点
	if (p)//若p!=NULL,表明已经找到该节点
	{
		gotoxy(3, 6); printf("------------------------------------------------------");
		gotoxy(3, 7); printf("Reader Number:%s", p->data.num);
		gotoxy(3, 8); printf("Reader Name:%s", p->data.name);
		gotoxy(3, 9); printf("Reader Sex:%s", p->data.sex);
		gotoxy(3, 10); printf("Reader Age:%d", p->data.age);
		gotoxy(3, 11); printf("Reader Tele:%s", p->data.tele);
		gotoxy(3, 12); printf("Total Number of Reader has Borrowed:%d", p->data.total_num);
		gotoxy(3, 13); printf("------------------------------------------------------");
		gotoxy(3, 14); printf("please modify Reader recorder:");
		gotoxy(3, 15); stringinput(p->data.name, 15, "Reader Name:");
		gotoxy(3, 16); stringinput(p->data.sex, 15, "Reader Sex:");
		gotoxy(3, 17); printf("Reader Age:"); scanf("%d", &temp); p->data.age = temp;
		gotoxy(3, 18); stringinput(p->data.tele, 15, "Reader Tele:");
		gotoxy(3, 19); printf("------------------------------------------------------");
		gotoxy(3, 20); printf("=====>modify success!"); getchar(); getchar();
		saveflag = 1;
	}
	else
	{
		gotoxy(3, 6); printf("=====>Not find this Raeder!\n"); getchar(); getchar();
	}
}
//统计记录
void CountBook(Book_Link l)
{
	Book_Node* r = l->next;
	int countc = 0, countm = 0, counte = 0;
	char bookname[15];
	if (!r)
	{
		clrscr();
		mainmenu();
		gotoxy(3, 5);
		printf("=====>Not book record!");
		getchar();
		return;
	}
	counte = r->data.total_num;
	strcpy(bookname, r->data.name);
	while (r)
	{
		countc++;//统计图书数量
		if (r->data.borrow_flag == 1) countm++;//统计已借出图书数
		//保存借出次数最多的图书名
		if (r->data.total_num > counte)
		{
			counte = r->data.total_num; strcpy(bookname, r->data.name);
		}
		r = r->next;
	}
	clrscr();
	mainmenu();
	gotoxy(3, 5);
	printf("------------------------the TongJi result--------------------------");
	gotoxy(3, 6); printf("Total number of books:%d", countc);
	gotoxy(3, 7); printf("Total number of borrowed books:%d", countm);
	gotoxy(3, 8); printf("Book name of maximum borrowed number:%s", bookname);
	gotoxy(3, 9); printf("-------------------------------------------------------------------");
	getchar(); getchar();
}
void CountReader(Reader_Link l)//读者总数量、男性读者数量、女性读者数量、目前借书最多的读者名
{
	Reader_Node* r = l->next;
	int countc = 0, countm = 0, countf = 0, counte = 0;
	char Readername[15];
	if (!r)
	{
		clrscr();
		mainmenu();
		gotoxy(3, 5);
		printf("=====>Not Reader record!");
		getchar();
		return;
	}
	counte = r->data.total_num;//读者借书数量
	strcpy(Readername, r->data.name);
	while (r)
	{
		countc++;//统计读者数量
		if (strcmp(r->data.sex, "M") == 0) countm++;//统计男性数量
		else countf++;//统计女性数量
		//保存借书次数最多的读者名
		if (r->data.total_num > counte)
		{
			counte = r->data.total_num; strcpy(Readername, r->data.name);
		}
		r = r->next;
	}
	clrscr();
	mainmenu();
	gotoxy(3, 5);
	printf("-----------------------the TongJi result--------------------------");
	gotoxy(3, 6); printf("Total number of readers:%d", countc);
	gotoxy(3, 7); printf("Total number of male readers:%d", countm);
	gotoxy(3, 8); printf("Total number of female readers:%d", countf);
	gotoxy(3, 9); printf("Reader name of maximum borrowed number:%s", Readername);
	gotoxy(3, 10); printf("-------------------------------------------------------------------");
	getchar(); getchar();
}
//排序图书记录
void SortBook(Book_Link l) //利用直接选择排序法实现按图书价格字段的升序排序，从低到高
{
	Book_Link lll;//临时指针
	Book_Node* p, * q, * r, * s, * h1;//临时指针
	int x, y;//保存当前光标所在位置的坐标值
	int i = 0;
	if (l->next == NULL)
	{
		clrscr();
		mainmenu();
		gotoxy(3, 5);
		printf("=====>Not book record!");
		getchar();
		return;
	}
	h1 = p = (Book_Node*)malloc(sizeof(Book_Node));//用于创建新的头节点
	if (!p)
	{
		gotoxy(3, 5);
		printf("allocate memory failure");//如果没有申请到打印信息
		return;//返回主界面 
	}
	//显示排序前的所有记录
	clrscr();
	mainmenu();
	gotoxy(3, 4);
	printf(HEADER1);
	lll = l->next;
	x = wherex(); y = wherey(); i = 0;
	while (lll != NULL)//当p不为空时，进行下列操作
	{
		gotoxy(3, i + y);//换行
		i++;
		printf(FORMAT1, DATA1);//见头部宏定义 
		lll = lll->next;//指针后移 
	}
	getchar(); getchar();
	gotoxy(3, y+i);
	printf("=====>sort by price..............");
	//排序
	p->next = l->next;//l所指节点为不存有任何记录的节点，下一个节点才有图书记录
	while (p->next != NULL)//外层循环决定待排序位置
	{
		q = p->next;
		r = p;
		while (q->next != NULL)//内部循环找到当前关键字最小节点
		{
			if (q->next->data.price < r->next->data.price)
				r = q; q = q->next;//移至下一个节点 
		}
		if (r != p)//表示原来的第一个节点不是关键字最小的节点，改变指针关系，将关键字最小的节点与本轮循环的首节点进行位置互换
		{
			s = r->next;//s指向最小节点
			r->next = s->next;//r的指针域指向最小节点的下一个节点
			s->next = p->next;//s的指针域指向当前p指针所指的下一个节点
			p->next = s;//p的指针域指向本次循环结束后关键字最小的节点 
		}
		p = p->next;//移至下一个节点 
	}
	l->next = h1->next;//将排序好的链表首节点地址赋给原来链表的指针域
	lll = l->next;
	x = wherex(); y = wherey(); i = 0;
	while (lll != NULL)//当p不为空时，进行下列操作
	{
		gotoxy(3, i + y);
		i++;
		printf(FORMAT1, DATA1);
		lll = lll->next;//指针后移 
	}
	free(h1);
	saveflag = 1;
	x = wherex(); y = wherey();
	gotoxy(3, y );
	printf("=====>sort complete!");
	getchar(); getchar();
	return;
}
void SortReader(Reader_Link l)
{
	Reader_Link lll;//临时指针
	Reader_Node* p, * q, * r, * s, * h1;//临时指针
	int x, y;//保存当前光标所在位置的坐标值
	int i = 0;
	if (l->next == NULL)
	{
		clrscr();
		mainmenu();
		gotoxy(3, 5);
		printf("\n=====>Not Reader record!");
		getchar();
		return;
	}
	h1 = p = (Reader_Node*)malloc(sizeof(Reader_Node));//用于创建新的头节点
	if (!p)
	{
		gotoxy(3, 5);
		printf("allocate memory failure");//如果没有申请到打印信息
		return;//返回主界面 
	}
	//显示排序前的所有记录
	clrscr();
	mainmenu();
	gotoxy(3, 4);
	printf(HEADER2);
	lll = l->next;
	x = wherex(); y = wherey(); i = 0;
	while (lll != NULL)//当p不为空时，进行下列操作
	{
		gotoxy(3, i + y);//换行
		i++;
		printf(FORMAT2, DATA2);//见头部宏定义 
		lll = lll->next;//指针后移 
	}
	getchar(); getchar();
	gotoxy(3, y+i);
	printf("=====>sort by number..............\n");
	//排序
	p->next = l->next;//l所指节点为不存有任何记录的节点，下一个节点才有图书记录
	while (p->next != NULL)//外层循环决定待排序位置
	{
		q = p->next;
		r = p;
		while (q->next != NULL)//内部循环找到当前关键字最小节点
		{
			if (strlen(q->next->data.num) < strlen(r->next->data.num))
			{
				r = q; q = q->next;
			}//移至下一个节点 
			else if (strlen(q->next->data.num) == strlen(r->next->data.num) && strcmp(q->next->data.num, r->next->data.num) < 0)
			{
				r = q; q = q->next;
			}//移至下一个节点 
			else q = q->next;


		}
		if (r != p)//表示原来的第一个节点不是关键字最小的节点，改变指针关系，将关键字最小的节点与本轮循环的首节点进行位置互换
		{
			s = r->next;//s指向最小节点
			r->next = s->next;//r的指针域指向最小节点的下一个节点
			s->next = p->next;//s的指针域指向当前p指针所指的下一个节点
			p->next = s;//p的指针域指向本次循环结束后关键字最小的节点 
		}
		p = p->next;//移至下一个节点 
	}
	l->next = h1->next;//将排序好的链表首节点地址赋给原来链表的指针域
	lll = l->next;
	gotoxy(3, y + i +1);
	x = wherex(); y = wherey(); i = 0;
	while (lll != NULL)//当p不为空时，进行下列操作
	{
		gotoxy(3, i + y); i++;
		printf(FORMAT2, DATA2);
		lll = lll->next;//指针后移 
	}
	free(h1);
	saveflag = 1;
	x = wherex(); y = wherey();
	gotoxy(3, y );
	printf("=====>sort complete!");
	getchar(); getchar();
	return;
}

//存储记录
void SaveBook(Book_Link l)
{
	FILE* fp;
	Book_Node* p;
	int count = 0;
	mainmenu();
	fp = fopen("D:\\book", "wb");//以只写方式打开二进制文件
	if (fp == NULL)//打开文件失败
	{
		clrscr();
		mainmenu();
		gotoxy(3, 5);
		printf("=====>open file error!\n");
		getchar();
		return;
	}
	p = l->next;
	while (p)
	{
		if (fwrite(p, sizeof(Book_Node), 1, fp) == 1)//每次写一条记录或一个节点信息至文件
		/*p -- 这是指向要被写入的元素数组的指针。
          size -- 这是要被写入的每个元素的大小，以字节为单位。
          nmemb -- 这是元素的个数，每个元素的大小为 size 字节。
          stream -- 这是指向 FILE 对象的指针，该 FILE 对象指定了一个输出流。*/
		{
			p = p->next;
			count++;
		}
		else
		{
			break;
		}
	}
	if (count > 0)
	{
		gotoxy(3, 8);
		printf("=====>save book,total saved record number is:%d\n", count);
		getchar();
		saveflag = 0;
	}
	else
	{
		system("cls");
		mainmenu();
		gotoxy(3, 8);
		printf("the current link is empty,no record is saved!\n");
		getchar();
	}
	fclose(fp);//关闭book文件 

}
void SaveReader(Reader_Link l)
{
	FILE* fp;
	Reader_Node* p;
	int count = 0;
	mainmenu();
	fp = fopen("D:\\reader", "wb");//以只写方式打开二进制文件
	if (fp == NULL)//打开文件失败
	{
		clrscr();
		mainmenu();
		gotoxy(3, 5);
		printf("=====>open file error!\n");
		getchar();
		return;
	}
	p = l->next;
	while (p)
	{
		if (fwrite(p, sizeof(Reader_Node), 1, fp) == 1)//每次写一条记录或一个节点信息至文件
		{
			p = p->next;
			count++;
		}
		else
		{
			break;
		}
	}
	if (count > 0)
	{
		gotoxy(3, 9);
		printf("=====>save reader,total saved record number is:%d\n", count);
		getchar();
		saveflag = 0;
	}
	else
	{
		system("cls");
		mainmenu();
		gotoxy(3, 8);
		printf("the current link is empty,no record is saved!\n");
		getchar();
	}
	fclose(fp);//关闭reader文件 
}
//借阅图书
void BorrowBook(Book_Link l, Reader_Link ll)
{
	Book_Node* p1;//定义图书记录指针变量
	Reader_Node* p2;//定义读者记录变量
	char readernum[15], bookname[15];
	int flag = 0;
	p1 = l->next;
	p2 = ll->next;
	clrscr();
	mainmenu();
	gotoxy(3, 5); stringinput(readernum, 15, "Reader Number:");
	while (p2)//查询该读者编号是否已经存在，若不存在则不允许执行借书操作
	{
		if (strcmp(p2->data.num, readernum) == 0)
		{
			flag = 1;
			break;
		}
		p2 = p2->next;
	}
	if (flag == 0)
	{
		gotoxy(3, 6);
		printf("The Reader Number %s is not existing!", readernum);
		getchar(); getchar();
		return;
	}
	if (p2->data.total_num >= 19)//每人共限借20本
	{
		gotoxy(3, 6);
		printf("The number of reader allowed borrowed book can't be more than 20!", readernum);
		getchar(); getchar();
		return;
	}
	gotoxy(3, 6); stringinput(bookname, 15, "Book Name:");
	while (p1)//查询该图书编号是否存在且该图书是否为可借状态，若任意条件不成立则不允许执行借书操作
	{
		if (strcmp(p1->data.name, bookname) == 0)
		{
			if (p1->data.borrow_flag == 0)//0表示未借出，1表示已借出
			{
				p1->data.borrow_flag = 1;//标记此图书已借出
				strcpy(p1->data.reader, readernum);//借阅人编号
				p1->data.total_num++;//图书被借阅次数
				p2->data.total_num++;//读者目前已借图书册数增1
				gotoxy(3, 8);
				printf("The book %s is borrowed by %s (Num:%s)Successfully!", bookname, p2->data.name, p2->data.num);
				getchar(); getchar();
				return;
			}
			else
			{
				gotoxy(3, 8);
				printf("The book %s can't be borrowed currently!", bookname);
				getchar(); getchar();
				return;
			}
		}
		else
		{
			p1 = p1->next;
		}
	}
	gotoxy(3, 8);
	printf("The book %s is not existing !", bookname);
	getchar(); getchar();
	return;
}
//归还图书
void ReturnBook(Book_Link l, Reader_Link ll)
{
	Book_Node* p1;//定义图书记录指针变量
	Reader_Node* p2;//定义读者记录指针变量
	char readernum[15], bookname[15];
	int flag = 0;
	p1 = l->next;
	p2 = ll->next;
	clrscr();
	mainmenu();
	gotoxy(3, 5); stringinput(readernum, 15, "Reader Number:");
	while (p2)//查询该读者编号是否已经存在，若不存在则不允许执行还书操作
	{
		if (strcmp(p2->data.num, readernum) == 0)
		{
			flag = 1;
			break;
		}
		p2 = p2->next;
	}
	if (flag == 0)
	{
		gotoxy(3, 6);
		printf("The Reader Number %s is not existing!", readernum);
		getchar(); getchar();
		return;
	}
	gotoxy(3, 6); stringinput(bookname, 15, "Book Name:");
	while (p1)//查询该图书编号是否存在且该图书是否为已借出状态，同时读者编号一致，若任意条件不满足则不允许执行还书操作
	{
		if (strcmp(p1->data.name, bookname) == 0)
		{
			if (p1->data.borrow_flag == 1 && strcmp(p1->data.reader, readernum) == 0)//0表示未借出，1表示已借出
			{
				p1->data.borrow_flag = 0;//标记此图书已还
				strcpy(p1->data.reader, " ");//在book文件中借阅人编号置空
				p2->data.total_num--;//读者目前已借图书册数减一
				gotoxy(3, 8);
				printf("The book %s is rerurned by %s (Num:%s)Successfully!", bookname, p2->data.name, p2->data.num);
				getchar(); getchar();
				return;
			}
			else
			{
				gotoxy(3, 8);
				printf("The book %s is not borrowed,or the number of reader is different!", bookname);
				getchar(); getchar();
				return;
			}
		}
		else
		{
			p1 = p1->next;
		}
	}
	gotoxy(3, 8);
	printf("The book %s is not existing !", bookname);
	getchar(); getchar();
	return;
}

void gotoxy(int x, int y)
{
	int xx = 0x0b;
	HANDLE hOutput;
	COORD loc;
	loc.X = x;
	loc.Y = y;
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOutput, loc);
	return;
}

void insline() {
	int i;
	for (i = 0; i < 77; i++) {
		printf("%c", '*');
	}
}
int wherex()

{

	CONSOLE_SCREEN_BUFFER_INFO pBuffer;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &pBuffer);

	return (pBuffer.dwCursorPosition.X + 1);

}
int wherey()

{

	CONSOLE_SCREEN_BUFFER_INFO pBuffer;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &pBuffer);

	return (pBuffer.dwCursorPosition.Y + 1);

}
