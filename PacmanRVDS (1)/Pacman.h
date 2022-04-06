#pragma once
#include "Point.h"
#include "color.h"
#include "Board.h"
#include "Creature.h"

class Pacman: public Creature {
	char arrowKeys[10];
	
public:
	Pacman() { initLoc = Point(10, 15);  location = Point(10, 15); direction = 2; color = Color::YELLOW; figure = '@'; setArrowKeys(); }
	Pacman(const Point& p);
	int getDirection(const char& key)const;
	void setArrowKeys();
	void move(int width, int length,int timePoint,const bool silent) override;
};