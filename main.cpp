# include "windows.h"
# include "math.h" //��ѧ���㺯����
# include "stdio.h" //��׼�������������
# include "stdlib.h" //��׼������
# include "string.h" //�ַ���������
# include "conio.h" //��Ļ����������
//�밴���йصĺ궨��
#define LEFT 65 //�������
#define RIGHT 68 //�������
#define DOWN 83 //�������
#define UP 87 //�������
#define ESC 27 //Esc����ȡ���˵��򿪲���
#define H 72 // F1�������ļ��˵�
#define J 74//F2�����򿪱༭�˵�
#define K 75//F3�����򿪰����˵�
#define ENTER 13 //�س���������
//��Book�˵�ѡ���йصĺ궨��
#define ADD_BOOK 100
#define QUERY_BOOK 101
#define MODIFY_BOOK 102
#define DEL_BOOK 103
#define SORT_BOOK 104
#define COUNT_BOOK 105
/*��Reader�˵�ѡ����صĺ궨��*/
#define ADD_READER 200
#define QUERY_READER 201
#define MODIFY_READER 202
#define DEL_READER 203
#define SORT_READER 204
#define COUNT_READER 205
//��B&R�˵�ѡ���йصĺ궨�壬B��ʾ���飬R��ʾ����
#define BORROW_BOOK 300
#define RETURN_BOOK 301
#define EXIT  302
//��book��¼��ʽ������йصĺ궨��
#define HEADER1 "|Number    |Name      |Author    |Publish   |Price| F|Reader    |Total|\n" 
#define FORMAT1 "|%-10s|%-10s|%-10s|%-10s|%.2f|%2d|%-10s|%5d| \n"
#define DATA1 lll->data.num,lll->data.name,lll->data.author,lll->data.publish,lll->data.price,lll->data.borrow_flag,lll->data.reader,lll->data.total_num
//��reader��¼��ʽ����йصĺ궨��
#define HEADER2 "|Number      |Name      |Sex |Age |Tele           |Total|\n"
#define FORMAT2 "|%-12s|%-10s|%-4s|%4d|%15s|%5d| \n"
#define DATA2 lll->data.num,lll->data.name,lll->data.sex,lll->data.age,lll->data.tele,lll->data.total_num
int saveflag = 0; //�Ƿ���Ҫ���̵�ȫ�ֱ�־����
#define clrscr() system("cls")
//������ͼ���йص����ݽṹ
typedef struct book //���Ϊbook
{
	char num[15]; //ͼ����
	char name[15]; //ͼ����
	char author[15]; //ͼ������
	char publish[15]; //������
	float price; //ͼ�鶨��
	int borrow_flag; //ͼ���Ƿ�����1��ʾ�����0��ʾδ���
	char reader[12]; //�����˱��
	int total_num; //ͼ�鱻����� 
};
//����������йص����ݽṹ
typedef struct reader //���Ϊreader
{
	char num[12];//���߱��
	char name[15];//��������
	char sex[4];//�����Ա�MΪ���ԣ�FΪŮ��
	int age;//��������
	char tele[15];//������ϵ�绰
	int total_num;//����Ŀǰ�ѽ���ͼ����� 
};
//����ÿ��ͼ���¼�����ݽṹ�����Ϊbook_node
typedef struct book_node
{
	struct book data;//������
	struct book_node* next;//ָ���� 
}Book_Node, * Book_Link; //*Book_NodeΪBook_node���͵Ľṹ������ *Book_LinkΪBook_node���͵�ָ�����
typedef struct reader_node
{
	struct reader data;    //������
	struct reader_node* next;   //ָ���� 
}Reader_Node, * Reader_Link;  //*Reader_NodeΪreadder_node���͵Ľṹ������ *Reader_LinkΪreader_node���͵�ָ�����
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
	Book_Link l;  //����ͼ���йص�ָ��
	Reader_Link ll;   //��������йص�ָ��
	FILE* fp1, * fp2;   //*fp1Ϊָ��ͼ����ļ�ָ�룬fp2Ϊָ����ߵ��ļ�ָ��
	char ch;  //���棨y��Y��n��N��  
	int count1 = 0, count2 = 0;  //�ֱ𱣴� ͼ���ļ�������ļ��еļ�¼����
	Book_Node* p, * r;  //����ͼ���¼ָ�����
	Reader_Node* p2, * r2;  //������߼�¼ָ�����
	int B;  //�����û�ѡ����Ӳ˵��� 
	drawmain();  //��ʾ������ 
	/*��ͼ���ļ�book��������������д洢*/
	l = (Book_Node*)malloc(sizeof(Book_Node));
	if (!l)
	{
		clrscr(); gotoxy(2, 3);
		printf("\n allocate memory failure");//���û�����뵽��ӡ��ʾ��Ϣ 
		return;//���������� 
	}
	l->next = NULL;
	r = l;
	fp1 = fopen("E:\\book", "ab+");//��׷�ӷ�ʽ��һ���������ļ����ɶ���д�������ļ������ڣ��ᴴ�����ļ�
	if (fp1 == NULL)
	{
		clrscr(); gotoxy(2, 3);
		printf("\n=====>can not open file!\n");
		exit(0);
	}
	while (!feof(fp1))  /*feof()����������ϵ��ļ������������ļ��������򷵻ط����ֵ�����򷵻���*/
	{
		p = (Book_Node*)malloc(sizeof(Book_Node));
		if (!p)
		{
			clrscr(); gotoxy(2, 3);
			printf("memory malloc failure!\n");//û������ɹ� 
			exit(0); //�˳� 
		}
		if (fread(p, sizeof(Book_Node), 1, fp1) == 1)//һ�δ��ļ��ж�ȡһ��ͼ���¼ 
		{
			p->next = NULL;
			r->next = p;
			r = p;//rָ�������һ��λ�� 
			count1++;
		}
	}
	fclose(fp1);//�ر��ļ�
	printf("\n=====>open file sucess,the total records number is:%d.\n", count1);
	p = r;
	//��ͼ���ļ�reader��������������д洢
	ll = (Reader_Node*)malloc(sizeof(Reader_Node));
	if (!ll)
	{
		printf("\n allocate memory failure");//��û�����뵽����ӡ��ʾ��Ϣ
		return;
	}
	ll->next = NULL;
	r2 = ll;
	fp2 = fopen("E:\\reader", "ab+");//��׷�ӷ�ʽ��һ���������ļ����ɶ���д�������ļ������ڣ��ᴴ�����ļ�
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
			printf("memory malloc failure!\n");//û������ɹ�
			exit(0); //�˳� 
		}
		if (fread(p2, sizeof(Reader_Node), 1, fp2) == 1)//һ�δ��ļ��ж�ȡһ��ͼ���¼
		{
			p2->next = NULL;
			r2->next = p2;
			r2 = p2;
			count2++;
		}
	}
	fclose(fp2);//�ر��ļ�
	printf("\n=====>open file sucess,the total records number is:%d.\n", count2);
	p2 = r2;
	while (1)
	{
		B = menuctrl();
		switch (B)
		{
		case ADD_BOOK:AddBook(l); break;//����ͼ���¼
		case QUERY_BOOK:QueryBook(l); break;//��ѯͼ���¼
		case MODIFY_BOOK:ModifyBook(l); break;//�޸�ͼ����¼
		case DEL_BOOK:DelBook(l); break;//ɾ��ͼ���¼
		case SORT_BOOK:SortBook(l); break;//����ͼ���¼
		case COUNT_BOOK:CountBook(l); break;//ͳ��ͼ���¼
		case ADD_READER:AddReader(ll); break;//���Ӷ��߼�¼
		case QUERY_READER:QueryReader(ll); break;//��ѯ���߼�¼
		case MODIFY_READER:ModifyReader(ll); break;//�޸Ķ��߼�¼
		case DEL_READER:DelReader(ll); break;//ɾ�����߼�¼
		case SORT_READER:SortReader(ll); break;//������߼�¼
		case COUNT_READER:CountReader(ll); break;//ͳ�ƶ��߼�¼
		case BORROW_BOOK:BorrowBook(l, ll); break;//����
		case RETURN_BOOK:ReturnBook(l, ll); break;//����
		case EXIT: //�˳�ϵͳ	
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
		drawmain();/*����ϵͳ������*/
	}
}
//����ϵͳ������ 
void drawmain()               /*����ϵͳ������*/
{
	int i;
	/*��ӡα��������*/
	for (i = 0; i <= 26; i++)
	{
		gotoxy(1, i);
		cprintf("%c", '|');
		gotoxy(79, i);
		cprintf("%c", '|');
	}
	/*��ӡα���ں���*/
	gotoxy(2, 0);	insline();
	gotoxy(2, 2);	insline();
	gotoxy(2, 24);	insline();
	gotoxy(2, 26);	insline();
	/*��ӡ��Ϣ*/
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
void mainmenu()               /*����ϵͳ������*/
{
	int i;
	/*��ӡ�˵�����*/
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
//���Ʋ˵�
void drawmenu(int m, int n) {
	int i;
	if (m % 3 == 0)
	{
		system("cls");
		/*��ӡ�߿�*/
		for (i = 0; i < 10; i++)
		{
			gotoxy(7, 2 + i);	 cprintf("%c", '*');
			gotoxy(20, 2 + i); cprintf("%c", '*');
		}
		gotoxy(7, 11);  cprintf("**************");
		/*��ӡ�˵�����*/
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
		/*��ӡѡ��*/
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
		/*��ӡ�߿�*/
		for (i = 1; i < 10; i++)
		{
			gotoxy(27, 2 + i);	cprintf("%c", '*');
			gotoxy(42, 2 + i);	cprintf("%c", '*');
		}
		gotoxy(27, 11);  cprintf("**************");//
		                          
		/*��ӡ�˵�����*/
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
		/*��ӡѡ��*/
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
		/*��ӡ�߿�*/
		for (i = 1; i < 10; i++)//��
		{
			gotoxy(47, 2 + i);	cprintf("%c", '*');
			gotoxy(60, 2 + i);	cprintf("%c", '*');
		}
		gotoxy(47, 11);	cprintf("**************");
		/*��ӡ�˵�����*/
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
		/*��ӡѡ��*/
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
int menuctrl()           /*�˵�����*/
{
	int A, B, value, flag = 36, a, b, i;
	while (1)
	{
		/*��ȡ��ֵ*/
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
			/*��ȡ��ֵ*/
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
			/*������һ���˵�*/
			if (B == ESC)
			{
				system("cls");
				drawmain();
				break;
			}
			if (flag == 0) flag = 36;
			if (value == 0) value = 300;
			/*���ع��ܼ�ֵ*/
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
			else if (B == UP) { /*�л�ѡ��*/
				drawmenu(value, --flag);
			}
			else if (B == DOWN) {
				drawmenu(value, ++flag);
			}
			else if (B == LEFT) {/*�л��˵�*/
				drawmenu(--value, flag);
			}
			else {
				drawmenu(++value, flag);
			}
		}
	}

}
//��¼���Ҷ�λ 
Book_Node* Locate(Book_Link l, char findmess[], char nameornum[])//ͼ�鶨λ 
/**********************************************************************************
���ã����ڶ�λ�����з���Ҫ��Ľڵ㣬������ָ��ýڵ��ָ��
������findmess[]����Ҫ���ҵľ������ݣ�nameornum[]���水ʲô���ң�
************************************************************************************/
{
	Book_Node* r;
	if (strcmp(nameornum, "num") == 0)//��ͼ���Ų�ѯ
	{
		r = l->next;
		while (r)
		{
			if (strcmp(r->data.num, findmess) == 0)//���ҵ�findmessֵ��ͼ����
				return r;
			r = r->next;
		}
	}
	else if (strcmp(nameornum, "name") == 0)//��ͼ������ѯ
	{
		r = l->next;
		while (r)
		{
			if (strcmp(r->data.name, findmess) == 0)//���ҵ�findmessֵ��ͼ����
				return r;
			r = r->next;
		}
	}
	return 0;//��δ�ҵ�������һ����ָ�� 
}
Reader_Node* LocateReader(Reader_Link l, char findmess[], char nameornum[])//���߶�λ 
{
	Reader_Node* r;
	if (strcmp(nameornum, "num") == 0)//�����߱�Ų�ѯ
	{
		r = l->next;
		while (r)
		{
			if (strcmp(r->data.num, findmess) == 0)//���ҵ�findmessֵ�Ķ��߱��
				return r;
			r = r->next;
		}
	}
	else if (strcmp(nameornum, "name") == 0)//������������ѯ
	{
		r = l->next;
		while (r)
		{
			if (strcmp(r->data.name, findmess) == 0)//���ҵ�findmessֵ�Ķ��� 
				return r;
			r = r->next;
		}
	}
	return 0;//��δ�ҵ�������һ����ָ�� 
}

//��ʽ����������
void stringinput(char* t, int lens, char* notice)//�����ַ����������г�����֤������<lens��
{
	POINT cp;
	char n[255];
	int x, y;
	do
	{
		printf(notice);//��ʾ��ʾ��Ϣ
		GetCursorPos(&cp);
		x = cp.x; y = cp.y;
		scanf("%s", n);//�����ַ���
		if (strlen(n) > lens)//���г���У�飬����lensֵ�������� 
		{
			gotoxy(0,0); printf("exceed the required length ! \n");
		}
	} while (strlen(n) > lens);
	strcpy(t, n);//��������ַ������Ƶ��ַ���t�� 
}

//���Ӽ�¼
void AddBook(Book_Link l)
{
	Book_Node* p, * r, * s;//ʵ����Ӳ�������ʱ�Ľṹ��ָ�����
	char ch, flag = 0, num[10];
	float temp;
	r = l;
	s = l->next;
	clrscr();
	while (r->next != NULL)
		r = r->next;//��ָ��������������ĩβ��׼����Ӽ�¼
	while (1)//һ�ο����������¼��ֱ������ͼ����Ϊ0�ļ�¼�����Ӳ���
	{
		while (1)//����ͼ���ţ���֤��ͼ����û�б�ʹ�ã�������ͼ����Ϊ0�����˳���Ӽ�¼����
		{
			clrscr();
			mainmenu();
			gotoxy(3, 4);
			stringinput(num, 15, "input book number(press '0' return menu):");//��ʽ������ͼ���Ų�����
			flag = 0;
			if (strcmp(num, "0") == 0)//����Ϊ0�����˳���Ӳ���������������
			{
				return;
			}
			while (s)//��ѯ��ͼ�����Ƿ��Ѿ����ڣ���������Ҫ����������һ��δ��ռ�õ�ͼ����
			{
				if (strcmp(s->data.num, num) == 0)
				{
					flag = 1;
					break;
				}
				s = s->next;
			}
			if (flag == 1)//��ʾ�û��Ƿ���������
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
		p = (Book_Node*)malloc(sizeof(Book_Node));//�����ڴ�ռ�
		if (!p)
		{
			printf("\n allocate memory failure ");//��û�����뵽����ӡ��ʾ��Ϣ
			return;
		}
		//��ͼ���¼��ֵ
		strcpy(p->data.num, num);//���ַ���num���Ƶ�p->data.num��
		gotoxy(3, 5); stringinput(p->data.name, 20, "Book Name:");
		gotoxy(3, 6); stringinput(p->data.author, 20, "Book Author:");
		gotoxy(3, 7); stringinput(p->data.publish, 20, "Book Publishing Company:");
		gotoxy(3, 8); printf("Book Price:"); scanf("%f", &temp);
		p->data.price = temp;
		p->data.borrow_flag = 0;//ͼ���ʼΪδ�����1��ʾ���
		strcpy(p->data.reader, " ");
		p->data.total_num = 0;//ͼ�鱻�������ʼΪ0
		gotoxy(3, 9); printf(">>>>press any key to start next record!");
		getchar(); getchar();
		p->next = NULL;//�������������β�����
		r->next = p;//���½��Ľ���������β����
		r = p;
		saveflag = 1;
	}
	return;
}
void AddReader(Reader_Link l)
{
	Reader_Node* p, * r, * s;//ʵ����Ӳ�������ʱ�Ľṹ��ָ�����
	char ch, flag = 0, num[10];
	int temp;
	r = l;
	s = l->next;
	clrscr();
	while (r->next != NULL)
		r = r->next;//��ָ��������������ĩβ��׼����Ӽ�¼
	while (1)//һ�ο����������¼��ֱ��������߱��Ϊ0�ļ�¼�����Ӳ���
	{
		while (1)//������߱�ţ���֤�ö��߱��û�б�ʹ�ã���������߱��Ϊ0�����˳���Ӽ�¼����
		{
			clrscr();
			mainmenu();
			gotoxy(3, 4);
			stringinput(num, 15, "input reader number(press '0' return menu):");//��ʽ��������߱�Ų�����
			flag = 0;
			if (strcmp(num, "0") == 0)//����Ϊ0�����˳���Ӳ���������������
			{
				return;
			}
			s = l->next;
			while (s)//��ѯ�ö��߱���Ƿ��Ѿ����ڣ���������Ҫ����������һ��δ��ռ�õĶ��߱��
			{
				if (strcmp(s->data.num, num) == 0)
				{
					flag = 1;
					break;
				}
				s = s->next;
			}
			if (flag == 1)//��ʾ�û��Ƿ���������
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
		p = (Reader_Node*)malloc(sizeof(Book_Node));//�����ڴ�ռ�
		if (!p)
		{
			printf("\n allocate memory failure ");//��û�����뵽����ӡ��ʾ��Ϣ
			return;
		}
		//��ͼ���¼��ֵ
		strcpy(p->data.num, num);//���ַ���num���Ƶ�p->data.num��
		gotoxy(3, 5); stringinput(p->data.name, 15, "Reader Name:");
		gotoxy(3, 6); stringinput(p->data.sex, 15, "Reader Sex:");
		gotoxy(3, 7); stringinput(p->data.tele, 15, "Reader Tele:");
		gotoxy(3, 8); printf("Reader Age:"); scanf("%d", &temp);
		p->data.age = temp;
		p->data.total_num = 0;//����Ŀǰ�ѽ�ͼ�������ʼΪ0
		gotoxy(3, 9); printf(">>>>press any key to start next record!");
		getchar(); getchar();
		p->next = NULL;//�������������β�����
		r->next = p;//���½��Ľ���������β����
		r = p;
		saveflag = 1;
	}
	return;
}
//��ѯ��¼
void QueryBook(Book_Link l)//��ͼ���Ż�ͼ��������ѯ
{
	int select;//1:��ͼ���Ų� 2����ͼ�����飬���������������� 
	char searchinput[20];//�����û�����Ĳ�ѯ����
	Book_Node* p;
	if (!l->next)//������Ϊ��
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
	if (select == 1)//��ͼ���Ų�ѯ
	{
		gotoxy(3, 7);
		stringinput(searchinput, 15, "input the existing book number:");
		p = Locate(l, searchinput, "num");
		//��l�в���ͼ����Ϊsearchinputֵ�Ľڵ㣬�����ؽڵ��ָ��
		if (p)//��p!=NULL
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
	else if (select == 2)//��ͼ������ѯ
	{
		gotoxy(3, 7);
		stringinput(searchinput, 15, "input the existing book name:");
		p = Locate(l, searchinput, "name");    /*�û�ѡ��2��ͨ�����ֲ��ң�����name�ֶ��в���*/
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
	int select;//1:��ͼ���Ų� 2����ͼ�����飬���������������� 
	char searchinput[20];//�����û�����Ĳ�ѯ����
	Reader_Node* p;
	if (!l->next)//������Ϊ��
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
	if (select == 1)//�����߱�Ų�ѯ
	{
		gotoxy(3, 7);
		stringinput(searchinput, 15, "input the existing reader number:");
		p = LocateReader(l, searchinput, "num");
		//��l�в���ͼ����Ϊsearchinputֵ�Ľڵ㣬�����ؽڵ��ָ��
		if (p)//��p!=NULL
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
	else if (select == 2)//����������ѯ
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
//ɾ����¼
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
			r->next = p->next;//��p��ָ�ڵ��������ȥ�� 
			free(p);//�ͷ��ڴ�ռ�
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
	else if (sel == 2)//�Ȱ�ͼ������ѯ���ü�¼���ڵĽڵ�
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
			r->next = p->next;//��p��ָ�ڵ��������ȥ�� 
			free(p);//�ͷ��ڴ�ռ�
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
	else if (sel == 2)//�Ȱ�ͼ������ѯ���ü�¼���ڵĽڵ�
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
//�޸ļ�¼
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
	stringinput(findmess, 10, "input the existing book number��");//���벢�����ͼ����
	p = Locate(l, findmess, "num");//��ѯ�ýڵ�
	if (p)//��p!=NULL,�����Ѿ��ҵ��ýڵ�
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
	stringinput(findmess, 10, "input the existing Reader number��");//���벢����ö��߱��
	p = LocateReader(l, findmess, "num");//��ѯ�ýڵ�
	if (p)//��p!=NULL,�����Ѿ��ҵ��ýڵ�
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
//ͳ�Ƽ�¼
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
		countc++;//ͳ��ͼ������
		if (r->data.borrow_flag == 1) countm++;//ͳ���ѽ��ͼ����
		//��������������ͼ����
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
void CountReader(Reader_Link l)//���������������Զ���������Ů�Զ���������Ŀǰ�������Ķ�����
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
	counte = r->data.total_num;//���߽�������
	strcpy(Readername, r->data.name);
	while (r)
	{
		countc++;//ͳ�ƶ�������
		if (strcmp(r->data.sex, "M") == 0) countm++;//ͳ����������
		else countf++;//ͳ��Ů������
		//�������������Ķ�����
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
//����ͼ���¼
void SortBook(Book_Link l) //����ֱ��ѡ������ʵ�ְ�ͼ��۸��ֶε��������򣬴ӵ͵���
{
	Book_Link lll;//��ʱָ��
	Book_Node* p, * q, * r, * s, * h1;//��ʱָ��
	int x, y;//���浱ǰ�������λ�õ�����ֵ
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
	h1 = p = (Book_Node*)malloc(sizeof(Book_Node));//���ڴ����µ�ͷ�ڵ�
	if (!p)
	{
		gotoxy(3, 5);
		printf("allocate memory failure");//���û�����뵽��ӡ��Ϣ
		return;//���������� 
	}
	//��ʾ����ǰ�����м�¼
	clrscr();
	mainmenu();
	gotoxy(3, 4);
	printf(HEADER1);
	lll = l->next;
	x = wherex(); y = wherey(); i = 0;
	while (lll != NULL)//��p��Ϊ��ʱ���������в���
	{
		gotoxy(3, i + y);//����
		i++;
		printf(FORMAT1, DATA1);//��ͷ���궨�� 
		lll = lll->next;//ָ����� 
	}
	getchar(); getchar();
	gotoxy(3, y+i);
	printf("=====>sort by price..............");
	//����
	p->next = l->next;//l��ָ�ڵ�Ϊ�������κμ�¼�Ľڵ㣬��һ���ڵ����ͼ���¼
	while (p->next != NULL)//���ѭ������������λ��
	{
		q = p->next;
		r = p;
		while (q->next != NULL)//�ڲ�ѭ���ҵ���ǰ�ؼ�����С�ڵ�
		{
			if (q->next->data.price < r->next->data.price)
				r = q; q = q->next;//������һ���ڵ� 
		}
		if (r != p)//��ʾԭ���ĵ�һ���ڵ㲻�ǹؼ�����С�Ľڵ㣬�ı�ָ���ϵ�����ؼ�����С�Ľڵ��뱾��ѭ�����׽ڵ����λ�û���
		{
			s = r->next;//sָ����С�ڵ�
			r->next = s->next;//r��ָ����ָ����С�ڵ����һ���ڵ�
			s->next = p->next;//s��ָ����ָ��ǰpָ����ָ����һ���ڵ�
			p->next = s;//p��ָ����ָ�򱾴�ѭ��������ؼ�����С�Ľڵ� 
		}
		p = p->next;//������һ���ڵ� 
	}
	l->next = h1->next;//������õ������׽ڵ��ַ����ԭ�������ָ����
	lll = l->next;
	x = wherex(); y = wherey(); i = 0;
	while (lll != NULL)//��p��Ϊ��ʱ���������в���
	{
		gotoxy(3, i + y);
		i++;
		printf(FORMAT1, DATA1);
		lll = lll->next;//ָ����� 
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
	Reader_Link lll;//��ʱָ��
	Reader_Node* p, * q, * r, * s, * h1;//��ʱָ��
	int x, y;//���浱ǰ�������λ�õ�����ֵ
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
	h1 = p = (Reader_Node*)malloc(sizeof(Reader_Node));//���ڴ����µ�ͷ�ڵ�
	if (!p)
	{
		gotoxy(3, 5);
		printf("allocate memory failure");//���û�����뵽��ӡ��Ϣ
		return;//���������� 
	}
	//��ʾ����ǰ�����м�¼
	clrscr();
	mainmenu();
	gotoxy(3, 4);
	printf(HEADER2);
	lll = l->next;
	x = wherex(); y = wherey(); i = 0;
	while (lll != NULL)//��p��Ϊ��ʱ���������в���
	{
		gotoxy(3, i + y);//����
		i++;
		printf(FORMAT2, DATA2);//��ͷ���궨�� 
		lll = lll->next;//ָ����� 
	}
	getchar(); getchar();
	gotoxy(3, y+i);
	printf("=====>sort by number..............\n");
	//����
	p->next = l->next;//l��ָ�ڵ�Ϊ�������κμ�¼�Ľڵ㣬��һ���ڵ����ͼ���¼
	while (p->next != NULL)//���ѭ������������λ��
	{
		q = p->next;
		r = p;
		while (q->next != NULL)//�ڲ�ѭ���ҵ���ǰ�ؼ�����С�ڵ�
		{
			if (strlen(q->next->data.num) < strlen(r->next->data.num))
			{
				r = q; q = q->next;
			}//������һ���ڵ� 
			else if (strlen(q->next->data.num) == strlen(r->next->data.num) && strcmp(q->next->data.num, r->next->data.num) < 0)
			{
				r = q; q = q->next;
			}//������һ���ڵ� 
			else q = q->next;


		}
		if (r != p)//��ʾԭ���ĵ�һ���ڵ㲻�ǹؼ�����С�Ľڵ㣬�ı�ָ���ϵ�����ؼ�����С�Ľڵ��뱾��ѭ�����׽ڵ����λ�û���
		{
			s = r->next;//sָ����С�ڵ�
			r->next = s->next;//r��ָ����ָ����С�ڵ����һ���ڵ�
			s->next = p->next;//s��ָ����ָ��ǰpָ����ָ����һ���ڵ�
			p->next = s;//p��ָ����ָ�򱾴�ѭ��������ؼ�����С�Ľڵ� 
		}
		p = p->next;//������һ���ڵ� 
	}
	l->next = h1->next;//������õ������׽ڵ��ַ����ԭ�������ָ����
	lll = l->next;
	gotoxy(3, y + i +1);
	x = wherex(); y = wherey(); i = 0;
	while (lll != NULL)//��p��Ϊ��ʱ���������в���
	{
		gotoxy(3, i + y); i++;
		printf(FORMAT2, DATA2);
		lll = lll->next;//ָ����� 
	}
	free(h1);
	saveflag = 1;
	x = wherex(); y = wherey();
	gotoxy(3, y );
	printf("=====>sort complete!");
	getchar(); getchar();
	return;
}

//�洢��¼
void SaveBook(Book_Link l)
{
	FILE* fp;
	Book_Node* p;
	int count = 0;
	mainmenu();
	fp = fopen("D:\\book", "wb");//��ֻд��ʽ�򿪶������ļ�
	if (fp == NULL)//���ļ�ʧ��
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
		if (fwrite(p, sizeof(Book_Node), 1, fp) == 1)//ÿ��дһ����¼��һ���ڵ���Ϣ���ļ�
		/*p -- ����ָ��Ҫ��д���Ԫ�������ָ�롣
          size -- ����Ҫ��д���ÿ��Ԫ�صĴ�С�����ֽ�Ϊ��λ��
          nmemb -- ����Ԫ�صĸ�����ÿ��Ԫ�صĴ�СΪ size �ֽڡ�
          stream -- ����ָ�� FILE �����ָ�룬�� FILE ����ָ����һ���������*/
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
	fclose(fp);//�ر�book�ļ� 

}
void SaveReader(Reader_Link l)
{
	FILE* fp;
	Reader_Node* p;
	int count = 0;
	mainmenu();
	fp = fopen("D:\\reader", "wb");//��ֻд��ʽ�򿪶������ļ�
	if (fp == NULL)//���ļ�ʧ��
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
		if (fwrite(p, sizeof(Reader_Node), 1, fp) == 1)//ÿ��дһ����¼��һ���ڵ���Ϣ���ļ�
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
	fclose(fp);//�ر�reader�ļ� 
}
//����ͼ��
void BorrowBook(Book_Link l, Reader_Link ll)
{
	Book_Node* p1;//����ͼ���¼ָ�����
	Reader_Node* p2;//������߼�¼����
	char readernum[15], bookname[15];
	int flag = 0;
	p1 = l->next;
	p2 = ll->next;
	clrscr();
	mainmenu();
	gotoxy(3, 5); stringinput(readernum, 15, "Reader Number:");
	while (p2)//��ѯ�ö��߱���Ƿ��Ѿ����ڣ���������������ִ�н������
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
	if (p2->data.total_num >= 19)//ÿ�˹��޽�20��
	{
		gotoxy(3, 6);
		printf("The number of reader allowed borrowed book can't be more than 20!", readernum);
		getchar(); getchar();
		return;
	}
	gotoxy(3, 6); stringinput(bookname, 15, "Book Name:");
	while (p1)//��ѯ��ͼ�����Ƿ�����Ҹ�ͼ���Ƿ�Ϊ�ɽ�״̬������������������������ִ�н������
	{
		if (strcmp(p1->data.name, bookname) == 0)
		{
			if (p1->data.borrow_flag == 0)//0��ʾδ�����1��ʾ�ѽ��
			{
				p1->data.borrow_flag = 1;//��Ǵ�ͼ���ѽ��
				strcpy(p1->data.reader, readernum);//�����˱��
				p1->data.total_num++;//ͼ�鱻���Ĵ���
				p2->data.total_num++;//����Ŀǰ�ѽ�ͼ�������1
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
//�黹ͼ��
void ReturnBook(Book_Link l, Reader_Link ll)
{
	Book_Node* p1;//����ͼ���¼ָ�����
	Reader_Node* p2;//������߼�¼ָ�����
	char readernum[15], bookname[15];
	int flag = 0;
	p1 = l->next;
	p2 = ll->next;
	clrscr();
	mainmenu();
	gotoxy(3, 5); stringinput(readernum, 15, "Reader Number:");
	while (p2)//��ѯ�ö��߱���Ƿ��Ѿ����ڣ���������������ִ�л������
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
	while (p1)//��ѯ��ͼ�����Ƿ�����Ҹ�ͼ���Ƿ�Ϊ�ѽ��״̬��ͬʱ���߱��һ�£�����������������������ִ�л������
	{
		if (strcmp(p1->data.name, bookname) == 0)
		{
			if (p1->data.borrow_flag == 1 && strcmp(p1->data.reader, readernum) == 0)//0��ʾδ�����1��ʾ�ѽ��
			{
				p1->data.borrow_flag = 0;//��Ǵ�ͼ���ѻ�
				strcpy(p1->data.reader, " ");//��book�ļ��н����˱���ÿ�
				p2->data.total_num--;//����Ŀǰ�ѽ�ͼ�������һ
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
