#pragma once
#include "Point.h"
#include "color.h"
#include "Board.h"


class Creature {
protected:
	Point initLoc;
	Point location;
	int direction = 0;
	Color color;
	bool colorFlag = true;
	char figure;
	bool saveMode = false;
	vector<char> moves;
public:
	Creature() { moves.resize(100,'\0');};
	void setColor(const Color& c) { color = c; }
	void setColorFlag(const bool& f) { colorFlag = f; }
	void setFigure(const char& c) { figure = c; }
	void setDirection(const int& direct,int timePoint);
	void setDirection(const int& direct) { direction = direct; }
	char directionToChar(const int& direct);
	void setLocation(const int& x,const int& y){ location.set(x, y); }
	void setLocation(Point p) { location = p; }
	void setInitLocation(const int& x, const int& y){ initLoc.set(x, y); }
	void setInitLocation(Point p) { initLoc = p; }
	void setSaveFlag(bool val) { saveMode = val; }
	int getCurrDirection(void)const { return direction; };
	vector<char>& getMoves() { return moves; };
	void print(const bool silent)const;
	virtual void move(int width, int length,int timePoint,const bool silent) = 0;
	Point getLocation() const { return location; }
	void restart(const char& c, int timePoint,const bool silent);
	
};