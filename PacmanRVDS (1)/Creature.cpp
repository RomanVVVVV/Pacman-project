#pragma once
#include "Creature.h"


void Creature::print(const bool silent)const
{

	if(colorFlag)
 	   setTextColor(color);
	gotoxy(location.getx(), location.gety());
	if(!silent)
		cout << figure;
	setTextColor(Color::WHITE);
}
void Creature::restart(const char& c,int timePoint,const bool silent)
{
	gotoxy(location.getx(), location.gety());
	if(!silent)
		cout << c;
	location = initLoc;
	setDirection(STAY, timePoint);
}
void Creature::setDirection(const int& direct,int timePoint)
{
	// direction = direct;
	 if(direction!=direct)
	 {
		 direction = direct;
		 if(saveMode)
		 {				
			if (timePoint >= moves.size() - 10 )
				moves.resize(2 * timePoint,'\0');
			moves[timePoint] = directionToChar(direct);			
		 }

	 }	
}
char Creature::directionToChar(const int& direct)
{
	char c = '\0';
	switch (direct){
	case 2: // UP
		c='U';
		break;
	case 3: // DOWN
		c='D';
		break;
	case 0: // LEFT
		c='L';
		break;
	case 1: // RIGHT
		c='R';
		break;
	case 4: // STAY
		c = 'S';
		break;
	default:
		c = '\0';
		break;
	}
	return c;
}
