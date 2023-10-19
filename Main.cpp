#include<graphics.h>
#include<conio.h>

int main()
{
	initgraph(400, 400);
	circle(200, 200, 100);
	outtextxy(170, 200, _T("chen"));
	_getch();
	closegraph();
	return 0;
}



