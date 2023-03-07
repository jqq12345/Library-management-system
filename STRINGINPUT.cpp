# include "windows.h"
# include "math.h" //数学运算函数库
# include "stdio.h" //标准输入输出函数库
# include "stdlib.h" //标准函数库
# include "string.h" //字符串函数库
# include "conio.h" //屏幕操作函数库


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
void stringinput(char* t, unsigned int lens, char const* notice)/*格式化输入数据*/
{
	char n[255];
	int x, y;
	do {
		printf(notice);      /*显示提示信息*/
		scanf("%s", n);      /*输入字符串*/
		if (strlen(n) > lens)
		{
			POINT cp;
			GetCursorPos(&cp);
			x = cp.x; y = cp.y;
			igotoxy(x + 2, y + 1); printf("exceed the required length ! \n");
		}/*进行长度校验，超过lens值重新输入*/
	} while (strlen(n) > lens);
	strcpy(t, n);       /*将输入的字符串复制到字符串t中  */
}
int main() {
	char a[20];
	stringinput(a, 15, "I'am a gir");
	return 1;
}
