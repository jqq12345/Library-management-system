# include "windows.h"
# include "math.h" //��ѧ���㺯����
# include "stdio.h" //��׼�������������
# include "stdlib.h" //��׼������
# include "string.h" //�ַ���������
# include "conio.h" //��Ļ����������


void igotoxy(int x, int y)
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
void stringinput(char* t, unsigned int lens, char const* notice)/*��ʽ����������*/
{
	char n[255];
	int x, y;
	do {
		printf(notice);      /*��ʾ��ʾ��Ϣ*/
		scanf("%s", n);      /*�����ַ���*/
		if (strlen(n) > lens)
		{
			POINT cp;
			GetCursorPos(&cp);
			x = cp.x; y = cp.y;
			igotoxy(x + 2, y + 1); printf("exceed the required length ! \n");
		}/*���г���У�飬����lensֵ��������*/
	} while (strlen(n) > lens);
	strcpy(t, n);       /*��������ַ������Ƶ��ַ���t��  */
}
int main() {
	char a[20];
	stringinput(a, 15, "I'am a gir");
	return 1;
}
