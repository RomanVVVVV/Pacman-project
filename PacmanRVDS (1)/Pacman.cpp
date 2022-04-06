#include "Pacman.h"
#include "Point.h"

Pacman::Pacman(const Point& p)
{
	initLoc = p;
	location = p;
	direction = 2;
	color = Color::YELLOW;
	figure = '@'; 
	setArrowKeys();
}
void Pacman::setArrowKeys()
{
	arrowKeys[0] = 'a';
	arrowKeys[1] = 'A';
	arrowKeys[2] = 'd';
	arrowKeys[3] = 'D';
	arrowKeys[4] = 'w';
	arrowKeys[5] = 'W';
	arrowKeys[6] = 'x';
	arrowKeys[7] = 'X';
	arrowKeys[8] = 's';
	arrowKeys[9] = 'S';
}

int Pacman::getDirection(const char& key)const
{
	for (int i = 0; i < 10; i++)
	{
		if (key == arrowKeys[i])
			return i/2;
	}
	return -1;
}


void Pacman::move(int width, int length,int timePoint,const bool silent)
{
	gotoxy(location.getx(),location.gety());
	if(!silent)
		cout << ' ';
	location.pmove(direction, width, length);
}
