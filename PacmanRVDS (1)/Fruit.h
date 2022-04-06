#pragma once
#include "Ghost.h"


class Fruit : public Ghost {
	int digit;
	bool visibility = false;
	vector<char> visibl;
public:
	Fruit() { digit = rand() % 5 + 5; setFigure(digit + 48); setColor(Color::MAGENTA);visibl.resize(100,'\0'); };
	Fruit(Point p) { digit = rand() % 5 + 5; setLocation(p); setInitLocation(p); setFigure(digit + 60); }
	bool getVis() { return visibility; }
	int getDigit(){return digit;}
	vector<char>& getVisibl() { return visibl; }
	void setVis(bool b,int timePoint)
	{
	 visibility = b;
	 if(saveMode)
	 {
		 if (timePoint >= visibl.size() )
			 visibl.resize(2 * timePoint,'\0');
		 if(b)
			visibl[timePoint] = '+';
		 else
			visibl[timePoint] = '-';
	 }
	}
	
};
