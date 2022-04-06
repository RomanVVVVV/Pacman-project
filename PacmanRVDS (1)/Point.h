#pragma once

#include <iostream>
#include "io_utils.h"

using namespace std;

enum dir{ LEFT = 0, RIGHT = 1, UP = 2, DOWN = 3,STAY = 4 };

class Point {
	int x = 1, y = 1;
public:
	Point() :x(1), y(1) {}
	Point(const Point& tmp)
	{
		x = tmp.x;
		y = tmp.y;
	}
	Point(const int& _x, const int& _y):x(_x),y(_y){}
public:

	void pmove(const int& dir, int width, int lenght);
	int getx(void)const { return x; };
	int gety(void)const { return y; };
	void setx(const int& _x) { x = _x; };
	void sety(const int& _y) { y = _y; };
	void set(const int& _x, const int& _y) { x = _x; y = _y; };
	bool pointCmp(const Point& a)const;
	bool operator==(const Point& a)const;
	double calcDis(const Point& p);//calculates distance between two points.
};