#pragma once
#include <string>
#include <filesystem>
#include <fstream>
#include "Point.h"


class board {
	int width=80;
	int length=25;
	int points=0;
	Point legend{80,25};
	Point pacmanLoc;
	vector<Point> ghostsLoc;
	
	char map[25][80];
public:
	board() {};
	Point getLegend() const { return legend; }
	int getWidth() const { return width; }
	int getLength() const { return length; }
	int numOfGhosts() const { return ghostsLoc.size(); }
	Point getGhostLoc(int i) { return ghostsLoc[i]; }
	Point getPacLoc() { return pacmanLoc; }
	board(string filename);
	void printBoard(const bool silent) const;
	void updateBoard(const Point& p, const char& ch= ' ');
	void setPoints(int p) { points = p; }
	int isWall(const Point& p) const;
	int isFood(const Point& p) const;
	char getVal(const Point& p) const;
	void init();  
	bool notinLegend(const int row, const int column)const;
	int getPoints() { return points; }
};