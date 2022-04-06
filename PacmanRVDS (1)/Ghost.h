#pragma once
#include "Point.h"
#include "color.h"
#include "Board.h"
#include "Creature.h"
#include <algorithm>


class Ghost: public Creature{
	int brains = 1;
public:
	Ghost() { initLoc = Point(5,1); location = Point(5,1); color = Color::WHITE; direction = 2;  figure = 'G';}
	Ghost(const Point& p,const Color& c);
	void move(int width, int length,int timePoint,const bool silent) override;
	bool isOppDir(const int& dir)const;
	void setBrains(int i) { brains = i; }
	int oppDir(int dir); 
	void ghostMover(const board& bd, const Point& pacLoc,int timePoint,const bool silent);
	void noviceMover(const board& bd, int timePoint,const bool silent);
	void goodMover(const board& bd,const Point& pacLoc,int timePoint,const bool silent);
	void bestMover(const board& bd,const Point& pacLoc, int timePoint,const bool silent);
	Point& getRefLoc() { return location; };
	bool deadEnd(const board& bd)const;
	bool sidesAreWalls(const board& b)const;
	
};


