#include "Gloomhaven.h"
#include "ReadCharacterData.h"
#include "ReadMonsterData.h"

int main(int argc, char* argv[])
{
	string characterFile = "character1.txt";
	string monsterFile = "monster1.txt";
	string debugMode = "0";
	if (argc >= 4)
	{
		characterFile = argv[1];
		monsterFile = argv[2];
		debugMode = argv[3];
	}
	int debug;
	if (debugMode == "1")
		debug = 1;
	else if (debugMode == "0")
		debug = 0;
	else
	{
		cout << "Error. " << endl;
		return 0;
	}

	string command;
	while (1)
	{
		cin >> command;
		while (command != "play" && command != "exit" && command != "EXIT")
		{
			cout << "Unrecognized command. " << endl;
			cout << "Please re-enter the command: " << endl;
			cin >> command;
		}
		if (command == "play")
		{
			cout << endl;
			cout << "A new game start!!!" << endl;
			Gloomhaven game(characterFile, monsterFile, debug);
			game.showIntroduction("introduction.txt");
			game.startGame();
		}
		else
			return 0;
	}
}