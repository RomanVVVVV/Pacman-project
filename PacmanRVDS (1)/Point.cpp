#pragma once
#include "Point.h"
#include <cmath>

void Point::pmove(const int& dir,int width, int lenght)
{
	switch (dir) {
	case 2: // UP
		--y;
		if (y < 0) {
			y = lenght-1;
		}
		break;
	case 3: // DOWN
		++y;
		if (y > lenght-1) {
			y = 0;//was 1
		}
		break;
	case 0: // LEFT
		--x;
		if (x < 0) {
			x = width-1;
		}
		break;
	case 1: // RIGHT
		++x;
		if (x > width-1) {
			x = 1;//was 1
		}
		break;	
	case 4:
		break;
	}
	

}
bool Point::operator==(const Point& a)const
{
	if (a.getx() == x && a.gety() == y)
		return true;
	return false;
}
bool Point::pointCmp(const Point& a)const
{
	if (a.getx() == x && a.gety() == y)
		return true;
	else
		return false;
}
double Point::calcDis(const Point& p)
{
	double xd = p.getx() - x;
	double yd = p.gety() - y;
	return pow((pow(xd,2) + pow(yd,2)), 0.5);
}

