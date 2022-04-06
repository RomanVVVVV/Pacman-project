#define _CRT_SECURE_NO_WARNINGS
#include "Board.h"

board::board(string filename)
{
	bool end = false;
	int numOfGhosts = 0, numOfPacMans = 0,numOfLegends=0;
	int boardlength = 0, i;
	string line;
	line.reserve(80);
	ifstream myfile(filename);
	getline(myfile, line);
	width = line.length();
	for (i = 0; (!(myfile.eof())) && (i < 25); ++i)
	{
		strcpy(map[i], line.c_str());
		getline(myfile, line);
	}
	strcpy(map[i], line.c_str());
	i++;
	length = i;


	boardlength = length;
	for (i = 0; i < width && !end; ++i)//checks if & is in the first line
	{
		if (map[0][i] == '&' )
		{
			if(width < i + 20)
				width = i + 20;
			end = true;
		}
	}
				
			
	for (int j = 0; j < length; ++j)
	{
	if ((static_cast<string>(map[j])).find('#')>=0)//checks the  first line without a wall			    
				boardlength = j+1;
		for (i = 0; i < width; ++i)
		{
			if (map[j][i] == '&')
			{
				if(numOfLegends<1)		
					legend.set(i, j);
				numOfLegends++;
				map[j][i] = ' ';
			}			
			if (map[j][i] == '@')
			{
			if(numOfPacMans<1)
				pacmanLoc.set(i, j);
				numOfPacMans++;
				map[j][i] = ' ';
			}
			if (map[j][i] == '$')
			{
			if(numOfGhosts <4)
				ghostsLoc.push_back(Point(i, j));
				numOfGhosts++;
				map[j][i] = ' ';		
			}
			
		}
	}

	int firstwall,lastwall;
	for(int j=0;j< boardlength;++j) 
	{
		firstwall = (static_cast<string>(map[j])).find('#');
		lastwall = (static_cast<string>(map[j])).rfind('#');
		for (int i = 0; i < width; ++i)
		{
			if (map[j][i] == ' ' && notinLegend(j,i)&&(i>firstwall)&&(i<lastwall))
			{
				map[j][i] = '.';
				points++;
			}					
			else
			if(map[j][i] == '%')			
					map[j][i] = ' ';
		}
	}
	for (int j = 0; j < length; ++j)
		for (i = 0; i < width; ++i)
			if(map[j][i] == '%')			
					map[j][i] = ' ';


	myfile.close();		
}
 
 void board::printBoard(const bool silent) const
{
	if(!silent)
	{
		for (int i = 0; i < length; i++)
			cout << map[i] << endl;
	}
	
}
void board::updateBoard(const Point& p,const char& ch)
{
	int x = p.getx();
	int y = p.gety();
	map[y][x] = ch;
}
int board::isWall(const Point& p) const
{
	if (map[p.gety()][p.getx()] == '#')
		return 1;
	return 0;
}
int board::isFood(const Point& p) const
{
	if (map[p.gety()][p.getx()] == '.')
		return 1;
	return 0;
}

char board::getVal(const Point& p) const
{
	return (map[p.gety()][p.getx()]);
}
bool board::notinLegend(const int row, const int column)const
{
	if ((legend.getx() < row && row < legend.getx() + 3) && (legend.gety() < column && column < (legend.gety() + 20)))
			return false;
		return true;
}
