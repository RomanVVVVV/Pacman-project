#include "Game.h"

int main(int argc, char* argv[])
{
	int ret = 0;
	Game pacmanGame;
	while (ret != 9)
	{
		pacmanGame.init(argc, argv);
		hideCursor();
		ret = pacmanGame.run();
		if (pacmanGame.getMode() == mode::LOADSILENT && ret == 99)
		{ 
			cout << "LOAD SILENT CHECK FAILED";
			Sleep(7000);
			ret = 9;
		}			
		if (pacmanGame.getMode() == mode::LOADSILENT && ret == 100)
		{
			cout << "LOAD SILENT CHECK SUCCEEDED";
			Sleep(7000);
			ret = 9;
		}			
	}
	return ret;
}
