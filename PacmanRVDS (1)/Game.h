#pragma once
#include <vector>
#include "io_utils.h"
#include "Ghost.h"
#include "Pacman.h"
#include "Fruit.h"
#include "Creature.h"
#include <istream>





enum mode{ DEFAULT = 0, SAVE = 1, LOAD = 2, LOADSILENT = 3};

using namespace std;

using std::filesystem::current_path;
using std::filesystem::directory_iterator;
namespace fs = std::filesystem;


class Game {

	enum { ESC = 27};
	int numOfMaps=3;
	vector <string> mapNames;
	int score=0;
	int difficulty = 0;
	int lives=3;
	bool colorFlag = true;
	mode runMode = mode::DEFAULT;
	int sleepTime = 200;
	board bd;
	Pacman pacman;
	vector <Ghost> ghosts;
	Fruit fruit;

public:
	vector <string> getMaps();
	void init(int argc, char* argv[]);
	void newMap(string mapName);
	mode getMode() { return runMode; };
	int run();
	int pelletCheck(const Point& p);
	void printScore()const;
	void printLives()const;
	bool pause(const char& k);
	int menuPrint();
	void initPrint()const;
	bool isEaten();
	bool fruitBonus(int timePoint);
	void restartGame(int timePoint);
	string getMap(int i) { return mapNames[i]; }
	int getNumOfMaps() { return mapNames.size(); }
	void fruitPlay(int& threshold,int fruitCount,const int timePoint,const bool loadModes, const string changes, const int lineNum);
	void ghostPlayLoad(const int timePoint,const string& changes,const int lineNum);
	void ghostPlay(const int);
	void setColorFlags();
	void loadSingleMapByName();
	void gameWrite(const int mapIndex,const int timePoint);
	void gameResult(const int mapIndex,const int timePoint,const int deathCount,const int* deaths);
	void setSaveFlagsTrue();
	void setMode(mode m) { runMode = m; };
	void setSleepTimer();
	ifstream& openMoves(int mapNum);
	bool pacmanChanged(const string& changes);
	char getPacmanChange(const string& changes);
	char getFruitChange(const string& changes);
	char getFruitVisbilChange(const string& changes);
	char getGhostChange(const int i, const string& changes);
	char charToKey(const char& direct);
	int charToDir(const char& c);
	bool ghostChanged(int gi, string changes);
	bool fruitChanged(const string& changes);
	bool visiblChanged(const string& changes);
	bool silentModeComparison(int timePoint, int mapNum, int deathCount, int deaths[]);

};

