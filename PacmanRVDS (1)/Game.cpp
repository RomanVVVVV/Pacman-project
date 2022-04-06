#define _CRT_SECURE_NO_WARNINGS
#include "game.h"




void Game::init(int argc, char* argv[])
{
	if (argc == 1)
		setMode(DEFAULT);
	else {
		if (strcmp(argv[1], "-save") == 0)
			setMode(SAVE);
		else if (strcmp(argv[1], "-load") == 0)
		{
			if (argc == 3 && strcmp(argv[2], "-silent") == 0)
				setMode(LOADSILENT);
			else setMode(LOAD);
		}
	}

	

	ghosts.reserve(4);

	mapNames = getMaps();
	score = 0;
	lives = 3;
	numOfMaps = mapNames.size();

	newMap(mapNames[0]);
	setColorFlags();
	setSleepTimer();
	if(runMode == mode::SAVE)  
		setSaveFlagsTrue();

}                  


void Game::newMap(string mapName)
{ 
	bd = board(mapName);

	ghosts.clear();
	ghosts.reserve(4);

	int gi=0;
	for (; gi < bd.numOfGhosts(); ++gi)
	{
		if(gi != 0)
			ghosts.push_back(Ghost(bd.getGhostLoc(gi), Color(gi + 1)));
		else
			ghosts.push_back(Ghost(bd.getGhostLoc(gi), Color::LIGHTBLUE));//blue color sucks :)
		ghosts[gi].setBrains(difficulty);
		
	}
	pacman = Pacman(bd.getPacLoc());
	pacman.getMoves().resize(100, '\0');
	fill(pacman.getMoves().begin(), pacman.getMoves().end(), '\0');
	//score = 0;
	//moves.resize(100,'\0');
	//int fruitCount = 0;
	fruit.setLocation(pacman.getLocation());
	fruit.setInitLocation(pacman.getLocation());
	fruit.getMoves().resize(100, '\0');
	fill(fruit.getMoves().begin(), fruit.getMoves().end(), '\0');
	fruit.getVisibl().resize(100, '\0');
	fill(fruit.getVisibl().begin(), fruit.getVisibl().end(), '\0');

	setColorFlags();




	if (runMode == mode::SAVE)
		setSaveFlagsTrue();
	//initPrint();
}


void Game::setColorFlags()
{
	pacman.setColorFlag(colorFlag);
	fruit.setColorFlag(colorFlag);
	for (Ghost& ghost : ghosts)
	{
		ghost.setColorFlag(colorFlag);
		ghost.setBrains(difficulty);
	}
}

void Game::setSaveFlagsTrue()
{	
	pacman.setSaveFlag(true);
	fruit.setSaveFlag(true);
	for (Ghost& ghost : ghosts)	
		ghost.setSaveFlag(true);
}

int Game::run()
{
	bool loadModes = false;
	bool resultValidity = true;
	if (runMode == mode::LOAD || runMode == mode::LOADSILENT)	
		loadModes = true;
		

	if (!loadModes)
	{
		int ret = menuPrint();
		if (ret == 1)
			return 9;
		if (ret == 3)
			loadSingleMapByName();
	}
	
	
	setColorFlags();
	
	int deaths[3];//saveMode
	int deathCount = 0;//saveMode
	
	
	char key = 'S', tmpkey = key;//TMPKEY is used to save key for possible future direction change.	 
	int checkerDir = pacman.getCurrDirection(), count=0, fruitCount = 0,
		threshold = bd.getPoints(), mapNum = 0, lineNum = 0;
	Point checker(pacman.getLocation());


	string changes;
	ifstream stepsFile;
	string newFile;

	if (loadModes)
	{
		newFile = mapNames[mapNum];
		newFile.erase(newFile.find(".") + 1);
		newFile = newFile + "steps";
		stepsFile.open(newFile);
		getline(stepsFile, changes);
		lineNum = stoi(changes.substr(0, changes.find(" ")));
	}

	

	initPrint();//prints 
	while (lives > 0 && mapNum < getNumOfMaps())
	{
		count = 0;
		while (lives > 0 && score < threshold)
		{
			if (loadModes && !stepsFile.eof() && (count > lineNum))
			{				//||(count==0&&lineNum==0)
				getline(stepsFile, changes);
				if(!stepsFile.eof())
					lineNum = stoi(changes.substr(0,changes.find(" ")));			
			}	
			if (!isEaten())
			{
				//++count;
				++fruitCount;

				score += pelletCheck(pacman.getLocation());

				printScore();
				printLives();

				if((!loadModes&&(_kbhit()))||(pacmanChanged(changes)&&lineNum==count))//changed
				{
					if (!loadModes)
						key = _getch();
					else//load dir from file
						key = charToKey(getPacmanChange(changes));
					if (!pause(key))
					{
						checkerDir = pacman.getDirection(key);//converts input char to direction index
						if (checkerDir != -1)//if the key is valid
						{
							checker.pmove(checkerDir, bd.getWidth(), bd.getLength());
							if (!bd.isWall(checker))
							{
								pacman.setDirection(checkerDir,count);
								tmpkey = 'S';
							}
							else
							{
								tmpkey = key;
								checker = pacman.getLocation();
								checkerDir = pacman.getCurrDirection();
							}
						}
					}
				}
				/*else
				{*/
					checkerDir = pacman.getDirection(tmpkey);
					checker.pmove(checkerDir, bd.getWidth(), bd.getLength());
					if (bd.isWall(checker) || checkerDir == STAY)//if it is impossible to turn to where was inputed by user.
					{
						checker = pacman.getLocation();
						checker.pmove(pacman.getCurrDirection(), bd.getWidth(), bd.getLength());
						if (bd.isWall(checker))// if also impossible to keep on going in the same direction.
							pacman.setDirection(STAY, count);
					}
					else//if is possible to turn.
						pacman.setDirection(checkerDir,count);

					
				/*}*/

				pacman.move(bd.getWidth(), bd.getLength(), count,runMode == mode::LOADSILENT);
				pacman.print(runMode == mode::LOADSILENT);
				checker = pacman.getLocation();
				Sleep(sleepTime);


				if (isEaten())
				{
					deaths[deathCount] = count;//saveMode
					++deathCount;//saveMode
					//count = count+2;
					tmpkey = 'S';
					++count;
					restartGame(count);					
					continue;
				}

				if(count % 2 == 0 && !loadModes)//so ghosts move at half speed of pacman.
					ghostPlay(count);
				if(count % 2 == 0 && loadModes)
					ghostPlayLoad(count, changes, lineNum);

				fruitPlay(threshold,fruitCount,count,loadModes,changes,lineNum);	
				++count;
			}
			else
			{
				deaths[deathCount] = count;//saveMode
				++deathCount;//saveMode
				tmpkey = 'S';///maybe bad ??? changed
				++count;
				//--count;
				restartGame(count);
			}
			//++count;
		}


		if(runMode==mode::SAVE)
		{
			gameWrite(mapNum,count);//saveMode
	    	gameResult(mapNum,count,deathCount,deaths);//saveMode
			deathCount = 0;
			deaths[0] = 0;
			deaths[1] = 0;
			deaths[2] = 0;
		}
		else if (runMode == mode::LOADSILENT)
		{
			if (silentModeComparison(count, mapNum, deathCount, deaths))
				resultValidity = true;
			else return 99;

			
			deathCount = 0;
			deaths[0] = 0;
			deaths[1] = 0;
			deaths[2] = 0;
		}
			
		
		if (runMode == mode::LOADSILENT && resultValidity)
			return 100;

		if (score < bd.getPoints())
		{
			clear_screen();
			if(runMode != mode::LOADSILENT)
				cout << "GAME OVER!" << endl;
			while (!_kbhit());
			clear_screen();
			if (loadModes) return 9;
			return 0;
		}
		else//NEW MAP RESTART
		{
			clear_screen();			
			mapNum++;
			//CHECK
			if (mapNum >= mapNames.size())
				continue;
	    	newMap(getMap(mapNum));
			fruitCount = 0;
			threshold += bd.getPoints();
			tmpkey = 'S';
			//
			if (loadModes)
			{
				stepsFile.close();
				newFile = mapNames[mapNum];
				newFile.erase(newFile.find(".") + 1);
				newFile = newFile + "steps";
				stepsFile.open(newFile);
				getline(stepsFile, changes);
				lineNum = stoi(changes.substr(0, changes.find(" ")));
			}
			//

			initPrint();
		}
	}


	if (runMode == mode::LOADSILENT && resultValidity)
		return 100;
		
	
	if(lives==0)
	{
		clear_screen();
		if (runMode != mode::LOADSILENT)
			cout << "GAME OVER!" << endl;
		while (!_kbhit());
		clear_screen();
		if (loadModes) return 9;
		return 0;
	}
	else
	{
    	if(runMode != mode::LOADSILENT)
			cout << "CONGRATULATIONS,YOU'VE WON!" << endl;
		while (!_kbhit());
		clear_screen();
		return 0;
	}

}
int Game::pelletCheck(const Point& p)
{
	if(bd.isFood(p))
	{
		bd.updateBoard(p);
		return 1;
	}
	return 0;
}

void Game::initPrint()const
{	
    if(runMode != mode::LOADSILENT)
		bd.printBoard(runMode == mode::LOADSILENT);
	Point leg = (this->bd).getLegend();
	gotoxy(leg.getx(), leg.gety());
	if(runMode != mode::LOADSILENT)
      cout << "SCORE: ";
	gotoxy(leg.getx(), leg.gety()+1);
	if(runMode != mode::LOADSILENT)
	  cout << "LIVES: ";
	gotoxy(leg.getx(), leg.gety()+2);
	if(runMode != mode::LOADSILENT)
	  cout << "GAME STATUS: RUNNING";
	for (const Ghost& ghost : ghosts)
		ghost.print(runMode == mode::LOADSILENT);
	pacman.print(runMode == mode::LOADSILENT);
	printLives();
	Sleep(sleepTime*10);
	
}
void Game::printScore()const
{
	gotoxy(((bd.getLegend()).getx()) + 7, bd.getLegend().gety() );
	if(runMode != mode::LOADSILENT)
		cout<<score;
}

void Game::printLives()const
{
	gotoxy((bd.getLegend().getx()) + 7, bd.getLegend().gety()+1);
	if(runMode != mode::LOADSILENT)
		cout<<lives;
}
bool Game::pause(const char& k)
{
	if (k == ESC)
	{
		gotoxy(bd.getLegend().getx() + 13, bd.getLegend().gety()+2);
		if(runMode != mode::LOADSILENT)
	    	cout << "PAUSED ";
		while (!_kbhit())
		{
			if(_getch()==ESC)
				break;
		}
		gotoxy(bd.getLegend().getx() + 13, bd.getLegend().gety() + 2);
		if(runMode != mode::LOADSILENT)
		   cout << "RUNNING";
		return true;
	}
	return false;
}


int Game::menuPrint()
{
	int exit=0;
	colorFlag = true;
	char c='\n';
	if(runMode != mode::LOADSILENT)
	{
		cout << "Press 1 to start a new game" << endl;
		cout << "Press 2 to start a new game WITHOUT COLORS" << endl;
		cout << "Press 3 to load a specific map" << endl;
		cout << "Press 8 for instructions and keys " << endl;
		cout << "Press 9 to exit " << endl;
	}
	

	while(c == '\n')
		c = getchar();

	while (c != '1' && c != '2' && c != '8' && c != '9' && c != '3')
	{
		if(runMode != mode::LOADSILENT)
			cout << "Enter valid number." << endl;
		c = getchar();
		while (c == '\n')
			c = getchar();
	}
	if (c == '2')
		colorFlag = false;
	else if (c == '9')
		exit=1;
	else if (c == '3')
		exit = 3;
	else if (c == '8')
	{
		clear_screen();
		if(runMode != mode::LOADSILENT)
		{
			cout << "Use keys: W = UP, A = LEFT , D = RIGHT, X =DOWN , S = STAY" << endl;
			cout << "To move pacman around the maze while eating breadcrumbs and avoiding ghosts." << endl;
			cout << "You have 3 lives, each time a ghost catches you, you lose a life." << endl;
			cout << "Objective: eat all the breadcrumbs." << endl;
			cout << "Use ESC for pausing the game.\n" << endl;
			cout << "Press 1 to start a new game" << endl;
			cout << "Press 2 to start a new game WITHOUT COLORS" << endl;
			cout << "Press 3 to load a specific map" << endl;
			cout << "Press 9 to exit " << endl;
		}	
		cin.ignore();

		c = getchar();

		while (c == '\n')
			c = getchar();
		if (c == '2')
			colorFlag = false;
		if(c=='9')
			exit=1;
		if (c == '3')
			exit = 3;
	}
	clear_screen();
	if (exit != 1)
	{
		if(runMode != mode::LOADSILENT)
		{
			cout << "Choose difficulty:\n" << endl;
			cout << "Press 1 for EASY" << endl;
			cout << "Press 2 for INTERMIDIATE " << endl;
			cout << "Press 3 for HARD" << endl;
		}
		

		c = getchar();

		while (c == '\n')
			c = getchar();

		while (c != '1' && c != '2' && c != '3')
		{
		    if(runMode != mode::LOADSILENT)
			   cout << "Enter valid number." << endl;
			c = getchar();
			while (c == '\n')
				c = getchar();
		}

		difficulty = c - '0';
	}
	clear_screen();
	return exit;
}

bool Game::isEaten()
{
	for(Ghost& ghost:ghosts)
		if (pacman.getLocation().pointCmp(ghost.getLocation()))
		{
			--lives;
			return true;			
		}
	return false;
}
bool Game::fruitBonus(int timePoint)
{

		if (pacman.getLocation().pointCmp(fruit.getLocation()))
		{
			score += fruit.getDigit();
			bd.setPoints(bd.getPoints() + fruit.getDigit());
			
			fruit.setVis(false,timePoint);
			return true;
		}
		else
		{
			for(Ghost& ghost:ghosts)
				if (ghost.getLocation().pointCmp(fruit.getLocation()))
				{
					fruit.setVis(false,timePoint);
					return false;//was true
				}
					
		}
		return false;
}

void Game::restartGame(int timePoint)
{
	pacman.restart(bd.getVal(pacman.getLocation()), timePoint,runMode == mode::LOADSILENT);
	for (Ghost& ghost : ghosts)
	{
		ghost.restart(bd.getVal(ghost.getLocation()), timePoint,runMode == mode::LOADSILENT);
		ghost.print(runMode == mode::LOADSILENT);
	}
	pacman.print(runMode == mode::LOADSILENT);
	Sleep(sleepTime*7);
}


vector<string> Game::getMaps()
{
	vector<string> vec;
	string path_string;
	string temp;
	path_string.reserve(500);
	temp.reserve(100);
	

	for (const auto& entry : fs::directory_iterator(current_path()))
	{
		path_string = ((entry.path()).filename()).string();
		temp = path_string.substr(path_string.find_last_of(".") + 1);


		if (temp.compare("screen") == 0)
		{
			vec.push_back(path_string);
		}
		
	}
	sort(vec.begin(), vec.end());
	return vec;
}
void Game::fruitPlay(int& threshold,int fruitCount,const int timePoint,const bool loadModes, const string changes,
	const int lineNum)
{
	if (fruit.getVis() && fruitBonus(timePoint))//check BEFORE movement
		{
			threshold += fruit.getDigit();
			fruitCount = 1;
		}
	else if (fruitCount % 4 == 0)//to slow down fruit
	{
		if(loadModes)
			{
				if(timePoint==lineNum && fruitChanged(changes))
					fruit.setDirection(charToDir(getFruitChange(changes)), timePoint);
				gotoxy(fruit.getLocation().getx(), fruit.getLocation().gety());
				if(runMode != mode::LOADSILENT)
				cout << bd.getVal(fruit.getLocation());
				fruit.getRefLoc().pmove(fruit.getCurrDirection(),bd.getWidth(),bd.getLength());
			}
		else
			fruit.ghostMover(bd,pacman.getLocation(),timePoint,runMode == mode::LOADSILENT);
		if (fruit.getVis())
		{
			fruit.print(runMode == mode::LOADSILENT);
			if (fruitBonus(timePoint))//check AFTER movement
			{
				threshold += fruit.getDigit();
				fruitCount = 1;
			}
		}
	}
	if(!loadModes)
		{ 
		if (fruitCount % 60 == 0)
			{
				fruit.setVis(!fruit.getVis(),timePoint);
				fruitCount = 1;
			}
		}
	else if(timePoint==lineNum && visiblChanged(changes) )
	{
		fruit.setVis((getFruitVisbilChange(changes)=='+'), timePoint);
	}
				
}
void Game::ghostPlay(const int timePoint)
{
	Point cmp;
	for (int gi = 0; gi < ghosts.size(); gi++)
	{
		cmp = ghosts[gi].getLocation();
		cmp.pmove(ghosts[gi].getCurrDirection(), bd.getWidth(), bd.getLength());
		for (int i = 0; i < ghosts.size(); i++)//separates ghosts if hook up	
		{			
			if (cmp.pointCmp(ghosts[i].getLocation()))
					ghosts[gi].setDirection(ghosts[gi].oppDir(ghosts[gi].getCurrDirection()), timePoint);
		}
		ghosts[gi].ghostMover(bd, pacman.getLocation(),timePoint,runMode == mode::LOADSILENT);
		ghosts[gi].print(runMode == mode::LOADSILENT);	
	}
}

void Game::ghostPlayLoad(const int timePoint, const string& changes,const int lineNum)
{
	for (int gi = 0; gi < ghosts.size(); gi++)
	{
		if ((ghostChanged(gi, changes) )&& (timePoint == lineNum))
		{
			ghosts[gi].setDirection(charToDir(getGhostChange(gi, changes)), timePoint);
		}

		gotoxy(ghosts[gi].getLocation().getx(), ghosts[gi].getLocation().gety());
		if(runMode != mode::LOADSILENT)
		cout << bd.getVal(ghosts[gi].getLocation());
		ghosts[gi].getRefLoc().pmove(ghosts[gi].getCurrDirection(), bd.getWidth(), bd.getLength());
		ghosts[gi].print(runMode == mode::LOADSILENT);	
	}
}
	


void Game::loadSingleMapByName()
{
	string filename;
	filename.reserve(200);
	clear_screen();
	if(runMode != mode::LOADSILENT)
	cout << "Enter valid file name:" << endl;
	getchar();
	getline(cin, filename);

	while (find(mapNames.begin(), mapNames.end(), filename) == mapNames.end())
	{
    	if(runMode != mode::LOADSILENT)
		cout << "File not found, try again" << endl;
		getline(cin, filename);
	}

	for (int i = 0; i < mapNames.size(); i++)
	{
		if (mapNames[i] != filename)
		{
			mapNames.erase(mapNames.begin() + i);
			i--;
		}
	}

	numOfMaps = mapNames.size();
	newMap(mapNames[0]);
	clear_screen();
}
void Game::gameWrite(const int mapIndex,const int timePoint)
{
	bool lineFlag = false;
	string newFile = mapNames[mapIndex];
	newFile.erase(newFile.find(".")+1);
	newFile = newFile + "steps";
	ofstream myFile(newFile);
	for(int i=0;i<timePoint;i++)
	{
		if (i < pacman.getMoves().size() &&  pacman.getMoves()[i] != '\0')
			lineFlag = true;
		if (i < fruit.getMoves().size() && fruit.getMoves()[i] != '\0')
			lineFlag = true;
		for(auto& ghost : ghosts)
			if (i < ghost.getMoves().size() && ghost.getMoves()[i] != '\0')
				lineFlag = true;
		if (i < fruit.getVisibl().size() && fruit.getVisibl()[i] != '\0')
			lineFlag = true;
		if(lineFlag)
		{			
			myFile << i << " ";
			if (i < pacman.getMoves().size() && pacman.getMoves()[i] != '\0')
				myFile <<"P"<<pacman.getMoves()[i]<<" ";	
			for (int gi = 0; gi < ghosts.size() ; gi++)
				if (i < ghosts[gi].getMoves().size() && ghosts[gi].getMoves()[i] != '\0')
					myFile <<static_cast<char>(gi + '0')<<ghosts[gi].getMoves()[i]<<" ";
			if (i < fruit.getMoves().size() && fruit.getMoves()[i] != '\0')
				myFile <<"F"<<fruit.getMoves()[i]<<" ";
			if (i < fruit.getVisibl().size() && fruit.getVisibl()[i] != '\0')
				myFile <<"V"<<fruit.getVisibl()[i]<<" ";	
			myFile << endl;
		}
		
		lineFlag = false;
	}
	myFile.close();
}
void Game::gameResult(const int mapIndex,const int timePoint,const int deathCount,const int* deaths)
{
	string newFile = mapNames[mapIndex];
	newFile.erase(newFile.find(".")+1);
	newFile = newFile + "result";
	ofstream myFile(newFile);
	for(int i=0;i<deathCount;++i)
	{
		myFile << "D" << deaths[i] << " Is the time Pacman died" << endl;
	}		
	myFile << "P" << score << " Points collected by pacman" << endl;
	myFile.close();
}

ifstream& Game::openMoves(int mapNum)
{
	string newFile = mapNames[mapNum];
	newFile.erase(newFile.find(".") + 1);
	newFile = newFile + "steps";
	ifstream stepsFile(newFile);
	return stepsFile;	
}

bool Game::pacmanChanged(const string& changes)
{
	if (changes.find("P") == string::npos)
		return false;
	return true;
}

bool Game::fruitChanged(const string& changes)
{
	if (changes.find("F") == string::npos)
		return false;
	return true;
}
bool Game::visiblChanged(const string& changes)
{
	if (changes.find("V") == string::npos)
		return false;
	return true;
}

bool Game::ghostChanged(int gi,string changes)
{
	string sub = changes.substr(changes.find(" ") + 1,changes.size());
	if (sub.find(static_cast<char>(gi+'0')) == string::npos)
		return false;
	return true;
}



char Game::charToKey(const char& direct)
{
	char c = '\0';
	switch (direct){
	case 'U': // UP
		c='W';
		break;
	case 'D': // DOWN
		c='X';
		break;
	case 'L': // LEFT
		c='A';
		break;
	case 'R': // RIGHT
		c='D';
		break;
	case 'S': // STAY
		c='S';
		break;
	default:
		c = '\0';
		break;
	}
	return c;
}

int Game::charToDir(const char& c)
{
	switch (c) {
	case 'U': // UP
		return dir::UP;
		break;
	case 'D': // DOWN
		return dir::DOWN;
		break;
	case 'L': // LEFT
		return dir::LEFT;
		break;
	case 'R': // RIGHT
		return dir::RIGHT;
		break;
	case 'S'://STAY
		return dir::STAY;
		break;
	}
}

char Game::getPacmanChange(const string& changes)
{
	int found = changes.find("P");
	return changes[found + 1];
}

char Game::getFruitChange(const string& changes)
{
	int found = changes.find("F");
	return changes[found + 1];
}
char Game::getFruitVisbilChange(const string& changes)
{
	int found = changes.find("V");
	return changes[found + 1];
}
char Game::getGhostChange(const int i, const string& changes)
{
	int found = changes.rfind(static_cast<char>(i + '0'));
	return changes[found + 1];
}

void Game::setSleepTimer()
{
	switch (runMode)
	{
	case mode::DEFAULT:
		sleepTime = 200;
		break;
	case mode::SAVE:
		sleepTime = 200;
		break;
	case mode::LOAD:
		sleepTime = 50;
		break;
	case mode::LOADSILENT:
		sleepTime = 0;
		break;
	}
}

bool Game::silentModeComparison(int timePoint, int mapNum,int deathCount , int deaths[] )
{
	ifstream resFile;
	string fname,line;
	int num;

	fname = mapNames[mapNum];
	fname.erase(fname.find(".") + 1);
	fname = fname + "result";
	resFile.open(fname);
	getline(resFile, line);

	for(int i =0;i<deathCount;i++)
	{
		if (resFile.eof() || line[0] != 'D')
			return false;
		num = stoi(line.substr(1, line.find(" ")));
		if (num != deaths[i])
			return false;
		getline(resFile, line);
	}

	if (resFile.eof() || line[0] != 'P')
		return false;
	num = stoi(line.substr(1, line.find(" ")));
	return (num == score);
}