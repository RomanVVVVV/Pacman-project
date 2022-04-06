#include "Ghost.h"

Ghost::Ghost(const Point& p,const Color& c)
{ 
	initLoc = p;
	location = p;
	direction = 2;
	color = c;
	figure = 'G';
}



void Ghost::move(int width, int length,int timePoint,const bool silent)
{
	gotoxy(location.getx(), location.gety());
	Point saver = getLocation();

	if (location.getx() == width-1)
	{
		location.pmove(LEFT, width, length);
		setDirection(LEFT, timePoint);
	}
	else if(location.getx() == 1)
	{
		location.pmove(RIGHT, width, length);
		setDirection(RIGHT, timePoint);
	}
	else if (location.gety() == length-1)
	{
		location.pmove(UP, width, length);
		setDirection(UP, timePoint);
	}
	else if (location.gety() == 1)
	{
		location.pmove(DOWN, width, length);
		setDirection(DOWN, timePoint);
	}
	else 
		location.pmove(direction, width, length);
	
}


bool Ghost::isOppDir(const int& dir)const
{
	switch (dir) {
	case 2: // UP
		if (direction == 3)
			return true;
		return false;
	case 3: // DOWN
		if (direction == 2)
			return true;
		return false;
	case 0: // LEFT
		if (direction == 1)
			return true;
		return false;
	case 1: // RIGHT
		if (direction == 0)
			return true;
		return false;
	default:
		return false;
	}
}



void Ghost::ghostMover(const board& bd, const Point& pacLoc,int timePoint,const bool silent) 
{
	int i = brains;
	switch (i) {
	case 1:
		noviceMover(bd, timePoint, silent);
		break;
	case 2:	
		goodMover(bd,pacLoc, timePoint ,silent);
		break;
	case 3:	
		bestMover(bd,pacLoc, timePoint ,silent);
		break;
	default: 
		noviceMover(bd, timePoint ,silent);
		break;
	}
}


bool Ghost::deadEnd(const board& bd) const
{
	
	Point tmp = getLocation();

	tmp.pmove(getCurrDirection(), bd.getWidth(), bd.getLength());
	
	if (bd.getVal(tmp) == '#' && (sidesAreWalls(bd)))
		return true;
	return false;
}
bool Ghost::sidesAreWalls(const board& b)const
{
	Point tmp1= getLocation();
	Point tmp2= getLocation();

	switch (direction) {
	case 2: // UP
	{
		tmp1.setx(tmp1.getx() -1);
		tmp2.setx(tmp2.getx() +1);
		if (b.isWall(tmp1) && b.isWall(tmp2))
			return true;
		else
			return false;
	}
	case 3: // DOWN
		tmp1.setx(tmp1.getx() -1);
		tmp2.setx(tmp2.getx() +1);
		if (b.isWall(tmp1) && b.isWall(tmp2))
			return true;
		else
			return false;
	case 0: // LEFT
		tmp1.sety(tmp1.gety() -1);
		tmp2.sety(tmp2.gety() +1);
		if (b.isWall(tmp1) && b.isWall(tmp2))
			return true;
		else
			return false;
	case 1: // RIGHT
		tmp1.sety(tmp1.gety() -1);
		tmp2.sety(tmp2.gety() +1);
		if (b.isWall(tmp1) && b.isWall(tmp2))
			return true;
		else
			return false;
	default:
		return true;
	}
}

void Ghost::goodMover(const board& bd,const Point& pacLoc,int timePoint,const bool silent)
{
	if ((timePoint % 40) > 0 && (timePoint % 40) <= 10)//because turncount is times 2 its not 20 but 40
		noviceMover(bd,timePoint,silent);
	else
		bestMover(bd, pacLoc,timePoint,silent);
}


void Ghost::noviceMover(const board& bd,int timePoint,const bool silent)
{
	int tmpDir;
	Point tmp;
	int count = 0;
	if (deadEnd(bd))
		setDirection(oppDir(getCurrDirection()), timePoint);
	else
	{
		do {
			tmp = getLocation();
			tmpDir = rand() % 4;
			count++;
			tmp.pmove(tmpDir, bd.getWidth(), bd.getLength());

		} while (bd.isWall(tmp) || isOppDir(tmpDir));
		setDirection(tmpDir,timePoint);
	}
	gotoxy((getLocation().getx()), (getLocation().gety()));
	if(!silent)
		cout << bd.getVal(getLocation());
	move(bd.getWidth(), bd.getLength(), timePoint,silent);
}

void Ghost::bestMover(const board& bd,const Point& pacLoc,int timePoint,const bool silent)
{

	int minInd;
	bool equal = true;
	Point options[4]; //opUP, opDOWN, opLEFT, opRIGHT;
	vector<int> validOptions;
	validOptions.reserve(4);
	for (int i = 0; i < 4; i++)
		options[i] = getLocation();
	options[2].pmove(UP, bd.getWidth(), bd.getLength());
	
	options[3].pmove(DOWN, bd.getWidth(), bd.getLength());
	
	options[0].pmove(LEFT, bd.getWidth(), bd.getLength());
	
	options[1].pmove(RIGHT, bd.getWidth(), bd.getLength());
	double distances[4];

	for (int i = 0; i < 4; i++)
		distances[i] = options[i].calcDis(pacLoc);	
	if (deadEnd(bd))
		setDirection(oppDir(getCurrDirection()), timePoint);
	else
	{
		for (int i = 0; i < 4; i++)		
			if (i != oppDir(direction) && !bd.isWall(options[i]))
				validOptions.push_back(i);
		
		if (validOptions.size() == 1)
			setDirection(validOptions[0],  timePoint);
		else
		{
			for (int i = 0; (i < validOptions.size() - 1) && equal; i++)
				if (distances[validOptions[i]] != distances[validOptions[i + 1]])
					equal = false;
			if (equal == false)
			{
				minInd = validOptions[0];
				for (int i = 1; i < validOptions.size(); i++)
					if (distances[validOptions[i]] < distances[minInd])
						minInd = validOptions[i];
			}
			else
			{
				if (find(validOptions.begin(), validOptions.end(), UP) != validOptions.end())//directions are set by priority. 
					minInd = UP;
				else
					if (find(validOptions.begin(), validOptions.end(), LEFT) != validOptions.end())
						minInd = LEFT;
					else
						if (find(validOptions.begin(), validOptions.end(), DOWN) != validOptions.end())
								minInd = DOWN;
						else
							if (find(validOptions.begin(), validOptions.end(), RIGHT) != validOptions.end())
									minInd = RIGHT;
			}
			setDirection(minInd,timePoint);
		}	
	}	
	gotoxy(getLocation().getx(), getLocation().gety());
	if(!silent)
		cout << bd.getVal(getLocation());
	move(bd.getWidth(), bd.getLength(),timePoint,silent);
}

int Ghost::oppDir(int dir)
{
	switch (dir) {
	case UP:
		return DOWN;
		break;
	case DOWN:
		return UP;
		break;
	case LEFT:
		return RIGHT;
		break;
	case RIGHT:
		return LEFT;
		break;
	}
}

