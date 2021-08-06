#include "Gloomhaven.h"
#include "ReadCharacterData.h"
#include "ReadMonsterData.h"
#include <ctime>
#include <iostream>
#include <list>
#include <conio.h>
#include <Windows.h>
#include <cmath>

Gloomhaven::Gloomhaven(string characterFileName, string monsterFileName, int _debugMode)
{
	allMonsters = readMonsterData(monsterFileName);
	allCharacters = readCharacterData(characterFileName);
	players.resize(0);
	monsters.resize(0);
	playersNum = 0;
	monstersNum = 0;
	debugMode = _debugMode;
	// Load all the data
}

// Print out all the text in the file
// introductionFileName (string): the file you want to print out to the console
void Gloomhaven::showIntroduction(string introductionFileName)
{
	ifstream FILE;
	FILE.open(introductionFileName, ios::in);
	if (!FILE.is_open())
	{
		cout << "Error: " << endl;
		cout << "Can't open " << introductionFileName << endl;
		exit(1);
	}
	string tempStr;
	while (getline(FILE, tempStr))
	{
		cout << tempStr << endl;
	}
}

// Start the game
void Gloomhaven::startGame()
{
	inputPlayerData();
	cout << "請選擇一張地圖：" << endl;
	outputAllMapsName();
	cout << "請輸入地圖名稱：" << endl;
	setMapName();
	loadMap();
	//cout << "obstacles number: " << obstacles.size() << endl;
	system("CLS");
	chooseStartPosition();
	showMap(availableRegions);
	cout << endl;
	int counter = 1;
	while (1)
	{
		cout << endl << "Round " << counter << ": " << endl;
		counter++;
		eventDetermine(players);
		update(players, monsters);
		if (players.size() == 0)
		{
			cout << "Monster win. " << endl;
			break;
		}
		if (monsters.size() == 0)
		{
			cout << "Player win. " << endl;
			break;
		}
		for (Player player : players)
		{
			if (onDoor(player) != Position(-1, -1))
			{
				if (getMonstersNum(availableRegions) == 0)
				{
					openTheDoor(player.getPosition());
					showMap(availableRegions);
					cout << endl;
				}
			}
		}
	}
	/*while (1)
	{
		cout << "請輸入移動指令(測試用，對於A): " << endl;
		string command;
		cin >> command;
		move(players[0], command);
		showMap(availableRegions);
		cout << endl;
	}*/
	// continue...
}

// 輸入players的資訊(提示輸出也包辦了，不過可以再優化)
void Gloomhaven::inputPlayerData()
{
	cout << "請輸入角色數量：" << endl;
	bool pass = true;
	string tempStr;
	while (1)
	{
		pass = true;
		cin >> tempStr;
		for (char ch : tempStr)
		{
			if (ch > '9' || ch < '0')
			{
				pass = false;
				break;
			}
		}
		if (pass == false)
		{
			cout << "This is not a integer. " << endl;
			cout << "Please re-enter the number. " << endl;
			continue;
		}
		break;
	}
	playersNum = stoi(tempStr);
	while (playersNum < 2 || playersNum > 4)
	{
		cout << "Number of players has to be between 2 and 4 (including)" << endl;
		cout << "請輸入角色數量：" << endl;
		while (1)
		{
			pass = true;
			cin >> tempStr;
			for (char ch : tempStr)
			{
				if (ch > '9' || ch < '0')
				{
					pass = false;
					break;
				}
			}
			if (pass == false)
			{
				cout << "This is not a integer. " << endl;
				cout << "Please re-enter the number. " << endl;
				continue;
			}
			break;
		}
		playersNum = stoi(tempStr);
	}
	for (int t = 0; t < playersNum; t++)
	{
		Player player;
		cout << "Please choose player number " << (t + 1) << "'s cards: " << endl;
		player.inputData(allCharacters, 'A' + t);
		players.push_back(player);
	}
}

// 輸出maps檔案底下所有的map的名稱
void Gloomhaven::outputAllMapsName()
{
	vector<string> filesName;
	getTxtFiles("maps", filesName);
	for (string str : filesName)
	{
		cout << str << endl;
	}
}

// 設定選定的map名稱
void Gloomhaven::setMapName()
{
	vector<string> filesName;
	getTxtFiles("maps", filesName);
	while (1)
	{
		cin >> mapName;
		if (find(filesName.begin(), filesName.end(), mapName) == filesName.end())
		{
			cout << "Please input the exact same name of a map. " << endl;
			cout << "Please re-enter again: " << endl;
		}
		else
		{
			break;
		}
	}
}

// 載入地圖檔所有的資訊
void Gloomhaven::loadMap()
{
	ifstream FILE;
	FILE.open("maps/" + mapName, ios::in);
	if (!FILE.is_open())
	{
		cout << "Error: " << endl;
		cout << "Can't open " << "maps/" << mapName << endl;
		exit(1);
	}

	int height, width;
	FILE >> height >> width;
	map.resize(height);
	for (int i = 0; i < height; i++)
	{
		FILE >> map[i];
	}
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (map[i][j] == '0')
			{
				map[i][j] = '#';
				walls.push_back(Position(i, j));
			}
			else if (map[i][j] == '1')
				map[i][j] = ' ';
			else if (map[i][j] == '2')
				map[i][j] = '@';
			else if (map[i][j] == '3')
				map[i][j] = '3';
		}
	}

	separateRegion();

	//cout << "loading initial position..." << endl;
	string input;
	getline(FILE, input);
	//cout << input << endl;
	stringstream ss(input);

	while (ss.eof() || input == "")
	{
		getline(FILE, input);
		ss.str(input);
	}

	int x, y;

	while (ss >> y)
	{
		ss >> x;
		//cout << x << " " << y << endl;
		initialPosition.push_back(Position(x, y));
		if (find(availableRegions.begin(), availableRegions.end(), map[x][y]) == availableRegions.end())
		{
			availableRegions.push_back(map[x][y]);
		}
	}

	string tempStr;
	int mode[5];
	FILE >> monstersNum;
	//cout << monstersNum << endl;
	for (int i = 0; i < monstersNum; i++)
	{
		FILE >> tempStr >> y >> x >> mode[2] >> mode[3] >> mode[4];
		if (mode[playersNum] != 0)
		{
			Monster tempMonster = Monster(tempStr, Position(x, y), mode[playersNum]);
			tempMonster.setCode('a' + i);
			monsters.push_back(tempMonster);
			monsters[monsters.size() - 1].loadData(allMonsters);
		}
	}

}

// 輸出由getMap獲取的地圖
void Gloomhaven::showMap(vector<int> regions)
{
	vector<string> tempMap = getMap(regions);
	int currentColor = 255;
	setColor(255);
	for (string str : tempMap)
	{
		for (char ch : str)
		{
			if (ch == '#' && currentColor != 128)
			{
				setColor(128);
				currentColor = 128;
			}
			else if (ch == ' ' && currentColor != 15)
			{
				setColor(15);
				currentColor = 15;
			}
			else if (ch == '@' && currentColor != 14)
			{
				setColor(14);
				currentColor = 14;
			}
			else if (ch >= 'A' && ch <= 'Z')
			{
				setColor(11);
				currentColor = 11;
			}
			else if (ch >= 'a' && ch <= 'z')
			{
				setColor(12);
				currentColor = 12;
			}
			else if (ch == '3' && currentColor != 143)
			{
				setColor(143);
				currentColor = 143;
			}
			cout << ch;
		}
		setColor(15);
		currentColor = 15;
		cout << endl;
	}
	setColor(15);
}

// 根據提供的regions顯示完整的地圖
vector<string> Gloomhaven::getMap(vector<int> regions)
{
	vector<string> tempMap;
	tempMap.resize(map.size());
	for (int i = 0; i < map.size(); i++)
	{
		tempMap[i] = "";
	}
	for (int i = 0; i < map.size(); i++)
	{
		for (int j = 0; j < map[i].size(); j++)
		{
			if (map[i][j] >= 1 && map[i][j] < totalRegionNum)
			{
				if (find(regions.begin(), regions.end(), map[i][j]) != regions.end())
				{
					//printf("%d", map[i][j]);
					tempMap[i].append(" ");
				}
				else
				{
					tempMap[i].append("#");
				}
			}
			else
			{
				tempMap[i].append(string(1, map[i][j]));
			}
		}
	}
	//cout << "adding obstacles..." << endl;
	for (Obstacle obstacle : obstacles)
	{
		if (isInRegions(obstacle.getPosition(), availableRegions))
		{
			tempMap[obstacle.getPosition().x][obstacle.getPosition().y] = '@';
		}
	}
	//cout << "adding monsters..." << endl;
	for (Monster monster : monsters)
	{
		if (isInRegions(monster.getPosition(), availableRegions))
		{
			tempMap[monster.getPosition().x][monster.getPosition().y] = monster.getCode();
		}
	}
	//cout << "adding players..." << endl;
	for (Player player : players)
	{
		if (isInRegions(player.getPosition(), availableRegions))
		{
			tempMap[player.getPosition().x][player.getPosition().y] = player.getCode();
		}
	}
	return tempMap;
}

// 根據提供的regions顯示只有牆與地板的地圖，而withInitialPosition可決定是否要顯示初始位置
vector<string> Gloomhaven::getBlankMap(vector<int> regions, bool withInitialPosition)
{
	vector<string> tempMap;
	tempMap.resize(map.size());
	for (int i = 0; i < map.size(); i++)
	{
		tempMap[i] = "";
	}
	for (int i = 0; i < map.size(); i++)
	{
		for (int j = 0; j < map[i].size(); j++)
		{
			if (map[i][j] >= 1 && map[i][j] < totalRegionNum)
			{
				if (find(regions.begin(), regions.end(), map[i][j]) != regions.end())
				{
					//printf("%d", map[i][j]);
					tempMap[i].append(" ");
				}
				else
				{
					tempMap[i].append("#");
				}
			}
			else
			{
				tempMap[i].append(string(1, map[i][j]));
			}
		}
	}
	if (withInitialPosition)
	{
		for (int i = 0; i < initialPosition.size(); i++)
		{
			tempMap[initialPosition[i].x][initialPosition[i].y] = '_';
		}
		tempMap[firstPosition.x][firstPosition.y] = '*';
	}
	return tempMap;
}

// 輸出由getBlankMap獲取的地圖
void Gloomhaven::showBlankMap(vector<int> regions, bool withInitialPosition)
{
	vector<string> tempMap = getBlankMap(regions, withInitialPosition);
	for (string str : tempMap)
	{
		cout << str << endl;
	}
}

// 選擇起始位置(如有角色已經選好並且擋路，會套用與move同樣的規則)
void Gloomhaven::chooseStartPosition()
{
	//cout << "choosing..." << endl;
	vector<Position> firstPositions = initialPosition;
	std::sort(firstPositions.begin(), firstPositions.end(), compareForGetFirstPosition);
	firstPosition = firstPositions[0];
	//cout << firstPosition.x << " " << firstPosition.y << endl;
	vector<int> regions;
	for (Position position : initialPosition)
	{
		if (find(regions.begin(), regions.end(), map[position.x][position.y]) == regions.end())
		{
			regions.push_back(map[position.x][position.y]);
		}
	}
	vector<string> tempBlankMap = getBlankMap(regions, true);
	for (Monster monster : monsters)
	{
		if (isInRegions(monster.getPosition(), regions))
		{
			tempBlankMap[monster.getPosition().x][monster.getPosition().y] = monster.getCode();
		}
	}
	for (Obstacle obstacle : obstacles)
	{
		if (isInRegions(obstacle.getPosition(), regions))
		{
			tempBlankMap[obstacle.getPosition().x][obstacle.getPosition().y] = '@';
		}
	}

	cout << endl;
	string command = "";
	for (int i = 0; i < players.size(); i++)
	{
		players[i].setPosition(firstPosition);
		//cout << players[i].getPosition().x << " " << players[i].getPosition().y << endl;
		Position nextPosition = players[i].getPosition();
		bool finishInput = false;
		command = "";
		system("cls");
		cout << "請選擇第" << (i + 1) << "位角色的初始位置：" << endl << endl;
		for (string str : tempBlankMap)
		{
			cout << str << endl;
		}
		cout << endl << endl;
		cout << "Please input the moving command: " << endl;
		while (1)
		{
			if (_kbhit())
			{
				switch (_getch())
				{
				case 'W':
				case 'w':
					nextPosition = Position(players[i].getPosition().x - 1, players[i].getPosition().y);
					command.append(string(1, 'w'));
					break;
				case 'A':
				case 'a':
					nextPosition = Position(players[i].getPosition().x, players[i].getPosition().y - 1);
					command.append(string(1, 'a'));
					break;
				case 'S':
				case 's':
					nextPosition = Position(players[i].getPosition().x + 1, players[i].getPosition().y);
					command.append(string(1, 's'));
					break;
				case 'D':
				case 'd':
					nextPosition = Position(players[i].getPosition().x, players[i].getPosition().y + 1);
					command.append(string(1, 'd'));
					break;
				case 'E':
				case 'e':
					nextPosition = Position(players[i].getPosition().x, players[i].getPosition().y);
					command.append(string(1, 'e'));
					break;
				case 13:
					finishInput = true;
					break;
				}
				if (finishInput == true)
				{
					break;
				}
				if (nextPosition.x < 0 || nextPosition.x > map.size() || nextPosition.y < 0 || nextPosition.y > map[0].size())
				{
					cout << "ERROR" << endl;
					cout << "In chooseStartPosition, nextPosition is illegal. " << endl;
					cout << nextPosition.x << " " << nextPosition.y << endl;
					exit(1);
				}
				if (tempBlankMap[nextPosition.x][nextPosition.y] == '_')
				{
					tempBlankMap[players[i].getPosition().x][players[i].getPosition().y] = '_';
					tempBlankMap[nextPosition.x][nextPosition.y] = '*';
					players[i].setPosition(nextPosition);
				}
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 0 });
				cout << "請選擇第" << (i + 1) << "位角色的初始位置：" << endl << endl;
				for (string str : tempBlankMap)
				{
					cout << str << endl;
				}
				cout << endl << endl;
				cout << "Please input the moving command: " << endl;
				cout << command << endl;
			}
		}
		// 移轉初始位置
		if (firstPosition == players[i].getPosition() && firstPositions.size() > 1)
		{
			firstPositions.erase(firstPositions.begin());
			firstPosition = firstPositions[0];
		}
		else
		{
			for (int j = 0; j < firstPositions.size(); j++)
			{
				if (firstPositions[j] == players[i].getPosition())
				{
					firstPositions.erase(firstPositions.begin() + j);
				}
			}
		}
		tempBlankMap[firstPosition.x][firstPosition.y] = '*';
		tempBlankMap[players[i].getPosition().x][players[i].getPosition().y] = players[i].getCode();
		if (find(availableRegions.begin(), availableRegions.end(), map[players[i].getPosition().x][players[i].getPosition().y]) == availableRegions.end())
		{
			availableRegions.push_back(map[players[i].getPosition().x][players[i].getPosition().y]);
		}
	}
	system("cls");
}

// 使用於chooseStartPosition()內的sort
bool Gloomhaven::compareForGetFirstPosition(Position const& pos_1, Position const& pos_2)
{
	if (pos_1.x < pos_2.x)
		return true;
	else if (pos_1.x == pos_2.x)
	{
		if (pos_1.y < pos_2.y)
			return true;
		else if (pos_1.y == pos_2.y)
		{
			cout << "Warning: " << endl;
			cout << "Same initial position. " << endl;
			return true;
		}
		else
			return false;
	}
	return false;
}

// 依照command移動所給予的生物，如果 player 移動失敗則回傳 -1。其他情況皆回傳 1
// creature (Creature&): 可放入player 或 monster 來進行移動
// command (string): 移動的指令(可包括大小寫的WASD)
int Gloomhaven::move(Creature& creature, string command)
{
	for (char c : command)
	{
		if (c != 'W' && c != 'w' && c != 'A' && c != 'a' && c != 'S' && c != 's' && c != 'D' && c != 'd' && c != 'E' && c != 'e')
		{
			cout << "Error: " << endl;
			cout << "command is illegal (contain " << c << ")" << endl;
			exit(1);
		}
	}

	vector<string> tempMap = getMap(availableRegions);
	tempMap[creature.getPosition().x][creature.getPosition().y] = ' ';

	Position nextPosition = creature.getPosition();
	Position lastPosition = creature.getPosition();
	Position initialPosition = creature.getPosition();
	for (int i = 0; i < command.size(); i++)
	{
		switch (command[i])
		{
		case 'W':
		case 'w':
			nextPosition = Position(creature.getPosition().x - 1, creature.getPosition().y);
			break;
		case 'A':
		case 'a':
			nextPosition = Position(creature.getPosition().x, creature.getPosition().y - 1);
			break;
		case 'S':
		case 's':
			nextPosition = Position(creature.getPosition().x + 1, creature.getPosition().y);
			break;
		case 'D':
		case 'd':
			nextPosition = Position(creature.getPosition().x, creature.getPosition().y + 1);
			break;
		}
		if (tempMap[nextPosition.x][nextPosition.y] == ' ' || tempMap[nextPosition.x][nextPosition.y] == '3')
		{
			//cout << "set" << endl;
			//cout << "Previous: " << creature.getPosition().x << " " << creature.getPosition().y << endl;
			creature.setPosition(nextPosition);
			lastPosition = nextPosition;
			//cout << "Now: " << creature.getPosition().x << " " << creature.getPosition().y << endl;
		}
		if ((tempMap[nextPosition.x][nextPosition.y] <= 'Z' && tempMap[nextPosition.x][nextPosition.y] >= 'A' && creature.getType() == Type::player) ||
			(tempMap[nextPosition.x][nextPosition.y] <= 'z' && tempMap[nextPosition.x][nextPosition.y] >= 'a' && creature.getType() == Type::monster))
		{
			creature.setPosition(nextPosition);
		}
		if ((tempMap[nextPosition.x][nextPosition.y] <= 'z' && tempMap[nextPosition.x][nextPosition.y] >= 'a') ||
			tempMap[nextPosition.x][nextPosition.y] == '#' || tempMap[nextPosition.x][nextPosition.y] == '@')
		{
			if (creature.getType() == Type::player)
			{
				creature.setPosition(initialPosition);
				return -1;
			}
		}
	}
	if (lastPosition != creature.getPosition())
	{
		if (creature.getType() == Type::monster)
			creature.setPosition(lastPosition);
		else if (creature.getType() == Type::player)
		{
			creature.setPosition(initialPosition);
			return -1;
		}
	}
	return 1;
}

// 判斷該角色是否在門上，並回傳該門的 Position，如果不為門則回傳 (-1, -1)
// player (Player&): 要判斷的角色
Position Gloomhaven::onDoor(Player& player)
{
	if (map[player.getPosition().x][player.getPosition().y] == '3')
	{
		return player.getPosition();
	}
	return Position(-1, -1);
}

// 打開所屬位置的門，並增加新區域
// position (Position): 該門的位置
void Gloomhaven::openTheDoor(Position position)
{
	if (map[position.x][position.y] != '3')
	{
		cout << "Error: " << endl;
		cout << "The position(" << position.x << ", " << position.y << ") given is not a door. (openTheDoor)" << endl;
		exit(1);
	}
	else
	{
		int regionsBeside[4];
		regionsBeside[0] = map[position.x + 1][position.y];
		regionsBeside[1] = map[position.x - 1][position.y];
		regionsBeside[2] = map[position.x][position.y + 1];
		regionsBeside[3] = map[position.x][position.y - 1];
		for (int regionBeside : regionsBeside)
		{
			if (regionBeside >= 1 && regionBeside <= totalRegionNum)
			{
				if (find(availableRegions.begin(), availableRegions.end(), regionBeside) == availableRegions.end())
					availableRegions.push_back(regionBeside);
				else
					map[position.x][position.y] = regionBeside;
			}
		}
	}
}

// 或許在特定區域內的怪物數量
// regions (vector<int>&): 區域
int Gloomhaven::getMonstersNum(const vector<int>& regions)
{
	int result = 0;
	for (Monster monster : monsters)
	{
		for (int region : regions)
		{
			if (map[monster.getPosition().x][monster.getPosition().y] == region)
			{
				result++;
				break;
			}
		}
	}
	return result;
}

// 該position是否在給予的regions(vector<int>)內
// 但如果該position位於門上('3')，則四周只要任一個區域在給予的regions內，則回傳true
bool Gloomhaven::isInRegions(Position position, vector<int> regions)
{
	int region = map[position.x][position.y];
	if (region == '3')
	{
		if (isInRegions(Position(position.x + 1, position.y), regions) == true ||
			isInRegions(Position(position.x - 1, position.y), regions) == true ||
			isInRegions(Position(position.x, position.y + 1), regions) == true ||
			isInRegions(Position(position.x, position.y - 1), regions) == true)
		{
			return true;
		}
		else
			return false;
	}
	bool answer = false;
	for (int i : regions)
	{
		if (region == i)
		{
			answer = true;
			break;
		}
	}
	return answer;
}

// Get all the files within the directory
// path (string): the path of the directory you want to search
// files (vector<string>&): it will push all the files' name in this vector
void Gloomhaven::getFiles(string path, vector<string>& files)
{
	//檔案控制代碼
	intptr_t hFile = 0;
	//檔案資訊  
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目錄,迭代之  
			//如果不是,加入列表  
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else
			{
				files.push_back(p.assign(fileinfo.name));
			}

		} while (_findnext(hFile, &fileinfo) == 0);

		_findclose(hFile);
	}
}

// Get all the .txt files within the directory
// path (string): the path of the directory you want to search
// files (vector<string>&): it will push all the .txt files' name in this vector
void Gloomhaven::getTxtFiles(string path, vector<string>& files)
{
	vector<string> tempVector;
	getFiles(path, tempVector);
	for (int i = 0; i < tempVector.size(); i++)
	{
		if (tempVector[i].find(".txt", 0) == string::npos)
		{
			tempVector.erase(tempVector.begin() + i);
		}
	}
	for (string str : tempVector)
	{
		files.push_back(str);
	}
}

// 使用於loadMap()裡，對各個區域做編號
void Gloomhaven::separateRegion()
{
	for (int i = 0; i < map.size(); i++)
	{
		for (int j = 0; j < map[i].size(); j++)
		{
			if (map[i][j] == ' ' || map[i][j] == '@')
			{
				placeNumber(i, j);
				totalRegionNum++;
			}
		}
	}
}

// 使用於separateRegion()裡，用作遞迴編號
void Gloomhaven::placeNumber(int x, int y)
{
	if (map[x][y] == '@')
	{
		obstacles.push_back(Obstacle(Position(x, y), totalRegionNum));
	}
	map[x][y] = totalRegionNum;
	if (x + 1 != map.size() && (map[x + 1][y] == ' ' || map[x + 1][y] == '@'))
		placeNumber(x + 1, y);
	if (x - 1 != -1 && (map[x - 1][y] == ' ' || map[x - 1][y] == '@'))
		placeNumber(x - 1, y);
	if (y + 1 != map[0].size() && (map[x][y + 1] == ' ' || map[x][y + 1] == '@'))
		placeNumber(x, y + 1);
	if (y - 1 != -1 && (map[x][y - 1] == ' ' || map[x][y - 1] == '@'))
		placeNumber(x, y - 1);
}

// 判斷兩者生物是否在給予的 range 中
// creature1 (Creature&): 其中一隻生物
// creature2 (Creature&): 另一隻生物
// range (float): 所給予的 range
bool Gloomhaven::inRange(Creature& creature1, Creature& creature2, float range)
{
	if (range < abs(creature1.getPosition().x - creature2.getPosition().x) + abs(creature1.getPosition().y - creature2.getPosition().y))
	{
		return false;
	}
	Position alterPositionForCreature1(creature1.getPosition().x * 2 + 1, creature1.getPosition().y * 2 + 1);
	Position alterPositionForCreature2(creature2.getPosition().x * 2 + 1, creature2.getPosition().y * 2 + 1);
	// y = a * x + b
	if (alterPositionForCreature1.x != alterPositionForCreature2.x)
	{
		float a = ((float)alterPositionForCreature1.y - (float)alterPositionForCreature2.y) / (float)(alterPositionForCreature1.x - (float)alterPositionForCreature2.x);
		float b = (float)alterPositionForCreature1.y - a * (float)alterPositionForCreature1.x;
		//cout << "y = " << a << " * x + " << b << endl;
		Position alterPosition;
		//cout << "obstacle number: " << obstacles.size() << endl;

		for (Position wallPosition : walls)
		{
			alterPosition.x = wallPosition.x * 2;
			alterPosition.y = wallPosition.y * 2;
			//cout << "(" << alterPosition.x << ", " << alterPosition.y << ")" << endl;
			if (a * (float)(alterPosition.x) - (float)(alterPosition.y) >= -b &&
				a * (float)(alterPosition.x + 2) - (float)(alterPosition.y) >= -b &&
				a * (float)(alterPosition.x) - (float)(alterPosition.y + 2) >= -b &&
				a * (float)(alterPosition.x + 2) - (float)(alterPosition.y + 2) >= -b)
			{
				continue;
			}
			else if (a * (float)(alterPosition.x) - (float)(alterPosition.y) <= -b &&
					 a * (float)(alterPosition.x + 2) - (float)(alterPosition.y) <= -b &&
					 a * (float)(alterPosition.x) - (float)(alterPosition.y + 2) <= -b &&
					 a * (float)(alterPosition.x + 2) - (float)(alterPosition.y + 2) <= -b)
			{
				continue;
			}
			else if (wallPosition.x < min(creature1.getPosition().x, creature2.getPosition().x) ||
					 wallPosition.x > max(creature1.getPosition().x, creature2.getPosition().x) ||
					 wallPosition.y < min(creature1.getPosition().y, creature2.getPosition().y) ||
					 wallPosition.y > max(creature1.getPosition().y, creature2.getPosition().y))
			{
				continue;
			}
			else
			{
				return false;
			}
		}
	}
	else
	{
		int biggerY, smallerY;
		if (creature1.getPosition().y > creature2.getPosition().y)
		{
			biggerY = creature1.getPosition().y;
			smallerY = creature2.getPosition().y;
		}
		else
		{
			biggerY = creature2.getPosition().y;
			smallerY = creature1.getPosition().y;
		}

		for (Position wallPosition : walls)
		{
			if (wallPosition.x == creature1.getPosition().x &&
				wallPosition.y < biggerY && wallPosition.y > smallerY)
			{
				return false;
			}
		}
	}
	return true;
}

// 回傳給予的 monster 的優先攻擊目標，如沒有則回傳 -1
// monster (Monster&): 給予的 monster
// range (float): 搜尋的 range
int Gloomhaven::getTargetSequence(Monster& monster, float range)
{
	vector<TS> ts;
	TS temp;
	for (Player player : players)
	{
		if (inRange(monster, player, range))
		{
			temp.code = player.getCode();
			temp.sequence = player.getSequence();
			temp.agility = player.getAgility();
			temp.range = abs(monster.getPosition().x - player.getPosition().x) + abs(monster.getPosition().y - player.getPosition().y);
			ts.push_back(temp);
		}
	}
	if (ts.size() == 0)
	{
		return -1;
	}
	sort(ts.begin(), ts.end(), compareForGetTargetSequence);
	return ts[0].sequence;
}

bool Gloomhaven::compareForGetTargetSequence(TS ts1, TS ts2)
{
	if (ts1.range < ts2.range)
	{
		return true;
	}
	else if (ts1.range == ts2.range)
	{
		if (ts1.agility < ts2.agility)
		{
			return true;
		}
		else if (ts1.agility == ts2.agility)
		{
			if (ts1.code < ts2.code)
			{
				return true;
			}
			else if (ts1.code == ts2.code)
			{
				return true;
			}
		}
	}
	return false;
}

////////////////////////////////////////////////////////////


void Gloomhaven::eventDetermine(vector<Player>& players)
{
	for (int t = 0; t < players.size(); t++)
	{
		cout << endl;
		cout << players[t].getCode() << "'s \tCards Amount: " << players.at(t).getAllCards().size() << endl;
		cout << "\tDiscarded Cards Amount: " << players.at(t).getDiscardCards().size() << endl;
		if (players.at(t).getAllCards().size() < 2 && players.at(t).getDiscardCards().size() < 2)
		{
			cout << players.at(t).getCode() << " was killed." << endl;
			players.erase(players.begin() + t);
			showMap(availableRegions);
			cout << endl;
			t--;
		}
	}
	//°w¹ï¨C­Óplayer°µ¨Æ
	for (int t = 0; t < players.size(); t++)
	{
		//«ü©wplayer
		Player* player = &players[0];
		cout << endl << "Please input the character and the chosen cards of him/her or the command: " << endl;
		string selectPlayer;
		bool find = false;
		string temp;
		int index = 0;
		stringstream ss;
		
		//input checking
		while (1)
		{
			cin >> selectPlayer;
			if (selectPlayer == "exit" || selectPlayer == "EXIT")
			{
				exit(0);
			}
			bool pass = true;
			find = false;
			for (int i = 0; i < players.size(); i++)
			{
				if (string(1, players.at(i).getCode()) == selectPlayer)
				{
					index = i;
					find = true;
					if (players.at(i).choose == true)
					{
						temp = "";
						do {
							getline(std::cin, temp);
							ss.clear();
							ss.str(temp);
							ss >> temp;
						} while (temp == "");
						if (temp == "check")
						{
							player = &players[index];
							cout << endl << "Hand:" << endl;
							//¦Lcards¸ê®Æ(hand&discard)
							if (player->getAllCards().size() != 0)
							{
								for (int i = 0; i < player->getAllCards().size(); i++)
								{

									cout << "\tNum: " << player->getAllCards().at(i).number << " Agility: " << player->getAllCards().at(i).agility << endl;
									cout << "\t\tTop ->";
									if (player->getAllCards().at(i).topAttack.size() != 0)
									{
										for (int j = 0; j < player->getAllCards().at(i).topAttack.size(); j++)
										{
											cout << " Attack: " << player->getAllCards().at(i).topAttack.at(j);
										}
									}
									if (player->getAllCards().at(i).topHeal.size() != 0)
									{
										for (int j = 0; j < player->getAllCards().at(i).topHeal.size(); j++)
										{
											cout << " Heal: " << player->getAllCards().at(i).topHeal.at(j);
										}
									}
									if (player->getAllCards().at(i).topShield.size() != 0)
									{
										for (int j = 0; j < player->getAllCards().at(i).topShield.size(); j++)
										{
											cout << " Shield: " << player->getAllCards().at(i).topShield.at(j);
										}
									}
									if (player->getAllCards().at(i).topMove.size() != 0)
									{
										for (int j = 0; j < player->getAllCards().at(i).topMove.size(); j++)
										{
											cout << " Move: " << player->getAllCards().at(i).topMove.at(j);
										}
									}
									if (player->getAllCards().at(i).topRange != 0)
									{
										cout << " Range: " << player->getAllCards().at(i).topRange;
									}
									cout << endl;
									cout << "\t\tBot ->";
									if (player->getAllCards().at(i).botAttack.size() != 0)
									{
										for (int j = 0; j < player->getAllCards().at(i).botAttack.size(); j++)
										{
											cout << " Attack: " << player->getAllCards().at(i).botAttack.at(j);
										}
									}
									if (player->getAllCards().at(i).botHeal.size() != 0)
									{
										for (int j = 0; j < player->getAllCards().at(i).botHeal.size(); j++)
										{
											cout << " Heal: " << player->getAllCards().at(i).botHeal.at(j);
										}
									}
									if (player->getAllCards().at(i).botShield.size() != 0)
									{
										for (int j = 0; j < player->getAllCards().at(i).botShield.size(); j++)
										{
											cout << " Shield: " << player->getAllCards().at(i).botShield.at(j);
										}
									}
									if (player->getAllCards().at(i).botMove.size() != 0)
									{
										for (int j = 0; j < player->getAllCards().at(i).botMove.size(); j++)
										{
											cout << " Move: " << player->getAllCards().at(i).botMove.at(j);
										}
									}
									if (player->getAllCards().at(i).botRange != 0)
									{
										cout << " Range: " << player->getAllCards().at(i).botRange;
									}
									cout << endl;
								}
							}
							else
							{
								cout << "\t<Empty>" << endl;
							}
							cout << "Discard:" << endl;
							if (player->getDiscardCards().size() != 0)
							{
								for (int i = 0; i < player->getDiscardCards().size(); i++)
								{
									cout << "\tNum: " << player->getDiscardCards().at(i).number << " Agility: " << player->getDiscardCards().at(i).agility << endl;
									cout << "\t\tTop ->";
									if (player->getDiscardCards().at(i).topAttack.size() != 0)
									{
										for (int j = 0; j < player->getDiscardCards().at(i).topAttack.size(); j++)
										{
											cout << " Attack: " << player->getDiscardCards().at(i).topAttack.at(j);
										}
									}
									if (player->getDiscardCards().at(i).topHeal.size() != 0)
									{
										for (int j = 0; j < player->getDiscardCards().at(i).topHeal.size(); j++)
										{
											cout << " Heal: " << player->getDiscardCards().at(i).topHeal.at(j);
										}
									}
									if (player->getDiscardCards().at(i).topShield.size() != 0)
									{
										for (int j = 0; j < player->getDiscardCards().at(i).topShield.size(); j++)
										{
											cout << " Shield: " << player->getDiscardCards().at(i).topShield.at(j);
										}
									}
									if (player->getDiscardCards().at(i).topMove.size() != 0)
									{
										for (int j = 0; j < player->getDiscardCards().at(i).topMove.size(); j++)
										{
											cout << " Move: " << player->getDiscardCards().at(i).topMove.at(j);
										}
									}
									if (player->getDiscardCards().at(i).topRange != 0)
									{
										cout << " Range: " << player->getDiscardCards().at(i).topRange;
									}
									cout << endl;
									cout << "\t\tBot ->";
									if (player->getDiscardCards().at(i).botAttack.size() != 0)
									{
										for (int j = 0; j < player->getDiscardCards().at(i).botAttack.size(); j++)
										{
											cout << " Attack: " << player->getDiscardCards().at(i).botAttack.at(j);
										}
									}
									if (player->getDiscardCards().at(i).botHeal.size() != 0)
									{
										for (int j = 0; j < player->getDiscardCards().at(i).botHeal.size(); j++)
										{
											cout << " Heal: " << player->getDiscardCards().at(i).botHeal.at(j);
										}
									}
									if (player->getDiscardCards().at(i).botShield.size() != 0)
									{
										for (int j = 0; j < player->getDiscardCards().at(i).botShield.size(); j++)
										{
											cout << " Shield: " << player->getDiscardCards().at(i).botShield.at(j);
										}
									}
									if (player->getDiscardCards().at(i).botMove.size() != 0)
									{
										for (int j = 0; j < player->getDiscardCards().at(i).botMove.size(); j++)
										{
											cout << " Move: " << player->getDiscardCards().at(i).botMove.at(j);
										}
									}
									if (player->getDiscardCards().at(i).botRange != 0)
									{
										cout << " Range: " << player->getDiscardCards().at(i).botRange;
									}
									cout << endl;
								}
							}
							else
							{
								cout << "\t<Empty>" << endl;
							}

							cout << endl;
						}
						cout << "You have chosen this player before. " << endl;
						cout << "Please re-enter another player: " << endl;
						pass = false;
					}
					break;
				}
			}
			if (pass == false)
			{
				continue;
			}
			if (find == false)
			{
				cout << "Can't find the player named \"" << selectPlayer << "\". " << endl;
				cout << "Please re-enter: " << endl;
				continue;
			}

			//select player
			player = &players[index];
			players[index].choose = true;
			//player action
			string CMD = "";
			do {
				getline(std::cin, temp);
				ss.clear();
				ss.str(temp);
				ss >> CMD;
			} while (CMD == "");

			if (CMD == "exit" || CMD == "EXIT")
			{
				exit(0);
			}
			if (CMD == "-1")
			{
				if (player->getDiscardCards().size() == 0)
				{
					cout << "Unable to sleep" << endl;
					cout << "Please re-enter the command or choose two cards: " << endl;
					players[index].setSleep(false);
					players[index].choose = false;
					continue;
				}
				else
				{
					player->setSleep(true);
					player->setAgility(100);
					break;
				}
			}
			//checking
			else if (CMD == "check")
			{
				cout << endl << "Hand:" << endl;
				//¦Lcards¸ê®Æ(hand&discard)
				if (player->getAllCards().size() != 0)
				{
					for (int i = 0; i < player->getAllCards().size(); i++)
					{

						cout << "\tNum: " << player->getAllCards().at(i).number << " Agility: " << player->getAllCards().at(i).agility << endl;
						cout << "\t\tTop ->";
						if (player->getAllCards().at(i).topAttack.size() != 0)
						{
							for (int j = 0; j < player->getAllCards().at(i).topAttack.size(); j++)
							{
								cout << " Attack: " << player->getAllCards().at(i).topAttack.at(j);
							}
						}
						if (player->getAllCards().at(i).topHeal.size() != 0)
						{
							for (int j = 0; j < player->getAllCards().at(i).topHeal.size(); j++)
							{
								cout << " Heal: " << player->getAllCards().at(i).topHeal.at(j);
							}
						}
						if (player->getAllCards().at(i).topShield.size() != 0)
						{
							for (int j = 0; j < player->getAllCards().at(i).topShield.size(); j++)
							{
								cout << " Shield: " << player->getAllCards().at(i).topShield.at(j);
							}
						}
						if (player->getAllCards().at(i).topMove.size() != 0)
						{
							for (int j = 0; j < player->getAllCards().at(i).topMove.size(); j++)
							{
								cout << " Move: " << player->getAllCards().at(i).topMove.at(j);
							}
						}
						if (player->getAllCards().at(i).topRange != 0)
						{
							cout << " Range: " << player->getAllCards().at(i).topRange;
						}
						cout << endl;
						cout << "\t\tBot ->";
						if (player->getAllCards().at(i).botAttack.size() != 0)
						{
							for (int j = 0; j < player->getAllCards().at(i).botAttack.size(); j++)
							{
								cout << " Attack: " << player->getAllCards().at(i).botAttack.at(j);
							}
						}
						if (player->getAllCards().at(i).botHeal.size() != 0)
						{
							for (int j = 0; j < player->getAllCards().at(i).botHeal.size(); j++)
							{
								cout << " Heal: " << player->getAllCards().at(i).botHeal.at(j);
							}
						}
						if (player->getAllCards().at(i).botShield.size() != 0)
						{
							for (int j = 0; j < player->getAllCards().at(i).botShield.size(); j++)
							{
								cout << " Shield: " << player->getAllCards().at(i).botShield.at(j);
							}
						}
						if (player->getAllCards().at(i).botMove.size() != 0)
						{
							for (int j = 0; j < player->getAllCards().at(i).botMove.size(); j++)
							{
								cout << " Move: " << player->getAllCards().at(i).botMove.at(j);
							}
						}
						if (player->getAllCards().at(i).botRange != 0)
						{
							cout << " Range: " << player->getAllCards().at(i).botRange;
						}
						cout << endl;
					}
				}
				else
				{
					cout << "\t<Empty>" << endl;
				}
				cout << "Discard:" << endl;
				if (player->getDiscardCards().size() != 0)
				{
					for (int i = 0; i < player->getDiscardCards().size(); i++)
					{
						cout << "\tNum: " << player->getDiscardCards().at(i).number << " Agility: " << player->getDiscardCards().at(i).agility << endl;
						cout << "\t\tTop ->";
						if (player->getDiscardCards().at(i).topAttack.size() != 0)
						{
							for (int j = 0; j < player->getDiscardCards().at(i).topAttack.size(); j++)
							{
								cout << " Attack: " << player->getDiscardCards().at(i).topAttack.at(j);
							}
						}
						if (player->getDiscardCards().at(i).topHeal.size() != 0)
						{
							for (int j = 0; j < player->getDiscardCards().at(i).topHeal.size(); j++)
							{
								cout << " Heal: " << player->getDiscardCards().at(i).topHeal.at(j);
							}
						}
						if (player->getDiscardCards().at(i).topShield.size() != 0)
						{
							for (int j = 0; j < player->getDiscardCards().at(i).topShield.size(); j++)
							{
								cout << " Shield: " << player->getDiscardCards().at(i).topShield.at(j);
							}
						}
						if (player->getDiscardCards().at(i).topMove.size() != 0)
						{
							for (int j = 0; j < player->getDiscardCards().at(i).topMove.size(); j++)
							{
								cout << " Move: " << player->getDiscardCards().at(i).topMove.at(j);
							}
						}
						if (player->getDiscardCards().at(i).topRange != 0)
						{
							cout << " Range: " << player->getDiscardCards().at(i).topRange;
						}
						cout << endl;
						cout << "\t\tBot ->";
						if (player->getDiscardCards().at(i).botAttack.size() != 0)
						{
							for (int j = 0; j < player->getDiscardCards().at(i).botAttack.size(); j++)
							{
								cout << " Attack: " << player->getDiscardCards().at(i).botAttack.at(j);
							}
						}
						if (player->getDiscardCards().at(i).botHeal.size() != 0)
						{
							for (int j = 0; j < player->getDiscardCards().at(i).botHeal.size(); j++)
							{
								cout << " Heal: " << player->getDiscardCards().at(i).botHeal.at(j);
							}
						}
						if (player->getDiscardCards().at(i).botShield.size() != 0)
						{
							for (int j = 0; j < player->getDiscardCards().at(i).botShield.size(); j++)
							{
								cout << " Shield: " << player->getDiscardCards().at(i).botShield.at(j);
							}
						}
						if (player->getDiscardCards().at(i).botMove.size() != 0)
						{
							for (int j = 0; j < player->getDiscardCards().at(i).botMove.size(); j++)
							{
								cout << " Move: " << player->getDiscardCards().at(i).botMove.at(j);
							}
						}
						if (player->getDiscardCards().at(i).botRange != 0)
						{
							cout << " Range: " << player->getDiscardCards().at(i).botRange;
						}
						cout << endl;
					}
				}
				else
				{
					cout << "\t<Empty>" << endl;
				}
				player->setSleep(false);
				cout << endl;
				players[index].choose = false;
				cout << endl << "Please input the character and the chosen cards of him/her or the command: " << endl;
			}
			//if choose card
			else if (isNumber(CMD))
			{
				int card1 = stoi(CMD);
				CMD == "";
				ss >> CMD;
				if (isNumber(CMD) == false)
				{
					cout << "Unrecognized input. " << endl;
					cout << "Please re-enter again: " << endl;
					players[index].setSleep(false);
					players[index].choose = false;
					continue;
				}
				int card2 = stoi(CMD);

				if (player->findIndexCards(card1) != true || player->findIndexCards(card2) != true || card1 == card2)
				{
					cout << "One of the chosen cards is invalid or not exist, or both of the cards are the same, or there's only one card. " << endl;
					cout << "Please re-enter the command or choose two cards: " << endl;
					players[index].setSleep(false);
					players[index].choose = false;
					continue;
				}
				//delete from hand and make into discarded
				player->deleteIndex(card1);
				player->deleteIndex(card2);
				//set agility of player
				player->setAgility(player->getDiscardCards().at(player->getDiscardCards().size()-2).agility);
				player->setSecAgility(player->getDiscardCards().at(player->getDiscardCards().size() - 1).agility);
				player->setSleep(false);
				break;
			}
			else
			{
				cout << "Can't recognize the command. " << endl;
				cout << "Please re-enter the command or choose two cards: " << endl;
				players[index].setSleep(false);
				players[index].choose = false;
				continue;
			}
		}
	}
	//after all operation, all unchosen
	for (int i = 0; i < players.size(); i++)
	{
		players.at(i).choose = false;
	}
}

//update all elements
void Gloomhaven::update(vector<Player>& players, vector<Monster>& monsters)
{
	/////////////////////modifications for debug mode
	static bool firstTime = true;
	if (firstTime == true)
	{
		srand(time(NULL));
		firstTime = false;
	}
	int rndm = rand();
	cout << endl;
	//creature vector
	vector<Creature*>creatures;
	//set agility and if is in visable region of monster
	for (int i = 0; i < monsters.size(); i++)
	{
		Monster& temp = monsters.at(i);
		Position monPos = temp.getPosition();
		temp.open = false;
		if (isInRegions(monPos, availableRegions))
		{
			temp.open = true;
			if (debugMode == 1)
			{
				temp.select = 0;
			}
			else
			{
				temp.select = rndm % temp.getCards().size();
			}
			temp.setAgility(temp.getCards().at(temp.select).agility);
		}
	}
	///////////////////////////////////////////////////////

	//pushback into creature vector for sort
	for (int i = 0; i < monsters.size(); i++)
	{
		//if in region
		if (monsters.at(i).open == true)
		{
			creatures.push_back(&monsters.at(i));
		}
	}
	//pushback players
	for (int i = 0; i < players.size(); i++)
	{
		creatures.push_back(&players.at(i));
	}

	//sort
	sort(creatures.begin(), creatures.end(), cmp);

	/*for (int i = 0; i < creatures.size(); i++)
	{
		cout << creatures.at(i)->getAgility() << endl;
	}*/

	//print sequence & remember sequence
	for (int i = 0; i < creatures.size(); i++)
	{
		creatures.at(i)->setSequence(i);
		creatures.at(i)->initialize();
	}

	//if is player, print player stats
	for (int i = 0; i < creatures.size(); i++)
	{
		if (creatures.at(i)->getType() == Type::player && creatures.at(i)->dead == false)
		{
			Player* p = &players.at(0);
			bool f = false;
			for (int j = 0; j < players.size(); j++)
			{
				if (players.at(j).getSequence() == i)
				{
					p = &players.at(j);
					f = true;
					break;
				}
			}
			if (f == false)
			{
				cout << "NF" << endl;
			}
			cout << p->getCode() << ' ';
			if (p->getAgility() != 100)
			{
				cout << p->getAgility() << ' ' << p->getDiscardCards().at(p->getDiscardCards().size() - 2).number << ' ' << p->getDiscardCards().at(p->getDiscardCards().size() - 1).number << endl;
			}
			else
			{
				cout << 99 << " Sleeping" << endl;
				p->setAgility(99);
			}

		}
		//if is monster, print monster stats
		else if (creatures.at(i)->getType() == Type::monster && creatures.at(i)->dead == false)
		{
			Monster* m = &monsters.at(0);
			for (int j = 0; j < monsters.size(); j++)
			{
				if (monsters.at(j).getSequence() == i && monsters.at(j).open == true)
				{
					m = &monsters.at(j);
					break;
				}
			}
			MonsterActCard temp = m->getCards().at(m->select);
			cout << m->getName() << ' ' << m->getAgility();
			if (temp.isAttack == true)
			{
				cout << ' ' << "attack " << temp.attack;
			}
			if (temp.isHeal == true)
			{
				cout << ' ' << "heal " << temp.heal;
			}
			if (temp.isShield == true)
			{
				cout << ' ' << "shield " << temp.shield;
			}
			if (temp.isRange == true)
			{
				cout << ' ' << "range " << temp.range;
			}
			cout << endl;
		}
	}
	cout << endl;
	

	//do moves
	for (int i = 0; i < creatures.size(); i++)
	{
		string moveseq;
		//if is player, find which & find card
		if (creatures.at(i)->getType() == Type::player && creatures.at(i)->dead == false)
		{
			Player* p = &players.at(0);
			for (int j = 0; j < players.size(); j++)
			{
				if (players.at(j).getSequence() == i)
				{
					p = &players.at(j);
					break;
				}
			}
			//if sleep
			string CMD;
			if (p->getSleep() == true)
			{
				bool done = false;
				string tempStr;
				int num;
				//enter card to discard, if invalid continue input
				cout << p->getCode() << "'s turn, sleep" << endl;
				if (p->getDiscardCards().size() != 0)
				{
					cout << "Select a card to discard: " << endl;
					bool finish = true;
					while (1)
					{
						finish = true;
						cin >> tempStr;
						for (char ch : tempStr)
						{
							if (ch > '9' || ch < '0')
							{
								cout << "Invalid input. " << endl;
								cout << "Please re-enter again: " << endl;
								finish = false;
								break;
							}
						}
						if (finish == true)
						{
							break;
						}
					}
					num = stoi(tempStr);
					while (1)
					{
						//find index of discarding card
						for (int j = 0; j < p->getDiscardCards().size(); j++)
						{
							//if found
							if (p->getDiscardCards().at(j).number == num)
							{
								//delete card
								p->getDiscardCards().erase(p->getDiscardCards().begin() + j);
								//reuse remaining cards in discard
								p->reuse();
								done = true;
								p->setSleep(false);
								break;
							}
						}
						if (done == true)
						{
							p->setHP(2);
							cout << p->getCode() << " heal 2, and now hp is " << p->getHP() << ". " << endl;
							cout << "Removed card: " << num << endl;
							break;
						}
						cout << "Invalid input. " << endl;
						cout << "Please re-enter the card you want to discard: " << endl;
						while (1)
						{
							finish = true;
							cin >> tempStr;
							for (char ch : tempStr)
							{
								if (ch > '9' || ch < '0')
								{
									cout << "Invalid input. " << endl;
									cout << "Please re-enter again: " << endl;
									finish = false;
									break;
								}
							}
							if (finish == true)
							{
								break;
							}
						}
						num = stoi(tempStr);
					}
				}
				else
				{
					cout << "No cards to discard" << endl;
				}
			}
			//if not sleep
			else
			{
				//get two selected cards
				CharacterActCard temp1 = p->getDiscardCards().at(p->getDiscardCards().size() - 2);
				CharacterActCard temp2 = p->getDiscardCards().at(p->getDiscardCards().size() - 1);
				cout << p->getCode() << "'s turn, card: " << temp1.number << ' ' << temp2.number << endl;

				cout << "Please enter the part of the cards you want to use or enter \"check\" to check the status of your character. " << endl;
				//enter command
				while (cin >> CMD)
				{
					//if the user input "check"
					if (CMD == "check")
					{
						cout << endl;
						for (int i = 0; i < creatures.size(); i++)
						{
							Creature checkingElement = *creatures.at(i);
							if (checkingElement.dead == false)
							{
								cout << checkingElement.getCode() << "-hp: " << checkingElement.getHP() << ", shield: " << checkingElement.getShield() << endl;
							}
							
						}
						cout << endl;
						cout << "Please enter the part of the cards you want to use or enter \"check\" to check the status of your character. " << endl;
					}
					//let the user decide which part of the cards they want to use
					else if (CMD.length() >= 2)
					{
						string numPart = CMD.substr(0, CMD.length() - 1);
						char UpDown = CMD.at(CMD.length() - 1);
						bool numValid = true;
						bool UDValid = false;
						for (int times = 0; times < numPart.length() - 1; times++)
						{
							if (numPart.at(times) < '0' || numPart.at(times) > '9')
							{
								numValid = false;
								break;
							}
						}
						if (UpDown == 'U' || UpDown == 'u' || UpDown == 'D' || UpDown == 'd')
						{
							UDValid = true;
						}
						int N;
						if (numValid == true)
						{
							stringstream ss;
							ss.clear();
							ss << numPart;
							ss >> N;
						}

						if (numValid == false || UDValid == false)
						{
							;
						}
						else if (N == temp1.number && (UpDown == 'U' || UpDown == 'u'))
						{
							playerManipUp(*p, temp1);
							playerManipBot(*p, temp2);
							break;
						}
						else if (N == temp1.number && (UpDown == 'D' || UpDown == 'd'))
						{
							playerManipBot(*p, temp1);
							playerManipUp(*p, temp2);
							break;
						}
						else if (N == temp2.number && (UpDown == 'D' || UpDown == 'd'))
						{
							playerManipBot(*p, temp2);
							playerManipUp(*p, temp1);
							break;
						}
						else if (N == temp2.number && (UpDown == 'U' || UpDown == 'u'))
						{
							playerManipUp(*p, temp2);
							playerManipBot(*p, temp1);
							break;
						}
						cout << "Invalid input. " << endl;
						cout << "Please re-enter the part you want to use or \"check\" to view the status of your character: " << endl;
					}
					else
					{
						cout << "Invalid input. " << endl;
						cout << "Please re-enter the part you want to use or \"check\" to view the status of your character: " << endl;
					}
				}
			}
		}

		//if is monster
		if (creatures.at(i)->getType() == Type::monster && creatures.at(i)->dead == false)
		{
			Monster* m = &monsters.at(0);
			//find which monster
			for (int j = 0; j < monsters.size(); j++)
			{
				if (monsters.at(j).getSequence() == i && monsters.at(j).open == true)
				{
					m = &monsters.at(j);
					break;
				}
			}
			//get move
			MonsterActCard temp = m->getCards().at(m->select);
			m->getDiscardCards().push_back(m->getCards().at(m->select));
			m->getCards().erase(m->getCards().begin() + m->select);
			//if reuseable
			if (temp.reuse == true)
			{
				m->reuse();
			}
			if (temp.move != "")
			{
				move(*m, temp.move);
				showMap(availableRegions);
				cout << endl;
				if (debugMode == 0)
				{
					char cha;
					cout << endl;
					cout << "Press any key to continue" << endl;
					cin.clear();
					cout << endl;
					cha = _getch();
				}
			}
			if (temp.isAttack == true)
			{
				//set attack
				m->setAttack(temp.attack);
				m->setRange(temp.range);
				//find target
				int target = getTargetSequence(*m, m->getRange());
				if (target == -1)
				{
					cout << "No one lock" << endl;
					if (debugMode == 0)
					{
						char cha;
						cout << endl;
						cout << "Press any key to continue" << endl;
						cout << endl;
						cin.clear();
						cha = _getch();
					}
				}
				//attack
				else
				{
					
					//find target
					for (int j = 0; j < players.size(); j++)
					{
						if (players.at(j).getSequence() == target)
						{
							Position pl = players.at(j).getPosition();
							Position mo = m->getPosition();
							int dist = abs(pl.x - mo.x) + abs(pl.y - mo.y);
							cout << m->getCode() << " lock " << players.at(j).getCode() << " at distance " << dist << endl;
							//attack
							players.at(j).attacked(m->getAttack());
							cout << endl;
							cout << m->getCode() << " attack " << players.at(j).getCode() << " " << m->getAttack() << " damage, ";
							cout << players.at(j).getCode() << " shield " << players.at(j).getShield() << ", " << players.at(j).getCode() << " remain " << players.at(j).getHP() << " hp" << endl;
							//if dead, erase
							if (players.at(j).getHP() <= 0)
							{
								players.at(j).dead = true;
								cout << endl << players.at(j).getCode() << " was killed" << endl << endl;
								if (debugMode == 0)
								{
									char cha;
									cout << endl;
									cout << "Press any key to continue" << endl;
									cout << endl;
									cin.clear();
									cha = _getch();
								}
								players.erase(players.begin() + j);
								showMap(availableRegions);
								cout << endl;
							}
							else if (debugMode == 0)
							{
								char cha;
								cout << endl;
								cout << "Press any key to continue" << endl;
								cout << endl;
								cin.clear();
								cha = _getch();
							}
							break;
						}
					}
				}
			}
			if (temp.isShield == true)
			{
				m->setShield(temp.shield);
				cout << m->getCode() << " shield " << temp.shield << " this turn." << endl;
				if (debugMode == 0)
				{
					char cha;
					cout << endl;
					cout << "Press any key to continue" << endl;
					cout << endl;
					cin.clear();
					cha = _getch();
				}
			}
			if (temp.isHeal == true)
			{
				m->setHeal(temp.heal);
				cout << m->getCode() << " heal " << temp.heal << " this turn, now hp is " << m->getHP() <<endl;
				if (debugMode == 0)
				{
					char cha;
					cout << endl;
					cout << "Press any key to continue" << endl;
					cout << endl;
					cin.clear();
					cha = _getch();
				}
			}
		}
	}
}

bool Gloomhaven::cmp(Creature* a, Creature* b)
{
	if (a->getAgility() == b->getAgility())
	{
		if (a->getType() == Type::monster && b->getType() == Type::player)
		{
			return a->conflict < b->conflict;
		}
		else if (a->getType() == Type::player && b->getType() == Type::monster)
		{
			return a->conflict < b->conflict;
		}
		else if (a->getType() == Type::monster && b->getType() == Type::monster)
		{
			return a->getCode() <= b->getCode();
		}
		else if (a->getType() == Type::player && b->getType() == Type::player)
		{
			return a->getSecAgility() < b->getSecAgility();
		}
	}
	else
	{
		return a->getAgility() < b->getAgility();
	}
	
}

void Gloomhaven::playerManipUp(Player& p, CharacterActCard card)
{
	//move
	if (card.topMove.size() != 0)
	{
		for (int i = 0; i < card.topMove.size(); i++)
		{
			string moveStr;
			cout << "Please enter the moving command: " << endl;
			while (1)
			{
				cin >> moveStr;
				if (moveStr.length() > card.topMove.at(i))
				{
					cout << "The length of the command is not correct (you should input less than or equals to " << card.topMove[i] << " character(s))" << endl;
					cout << "Please re-enter the moving command: " << endl;
					continue;
				}
				else if (isValidMovingCommand(moveStr) == false)
				{
					cout << "One the character of the command is not valid. " << endl;
					cout << "Please re-enter the moving command: " << endl;
					continue;
				}
				else if (move(p, moveStr) == -1)
				{
					cout << "error move!!!" << endl;
					cout << "Please re-enter the moving command: " << endl;
					continue;
				}
				else
				{
					break;
				}
			}
			showMap(availableRegions);
			cout << endl;
		}
	}
	if (card.topAttack.size() != 0)
	{
		//attack
		for (int i = 0; i < card.topAttack.size(); i++)
		{
			bool haveAvailableMonster = false;
			int tempForRange = 0;
			for (Monster monster : monsters)
			{
				if (isInRegions(monster.getPosition(), availableRegions))
				{
					if (card.topRange == 0)
						tempForRange = 1;
					else
						tempForRange = card.topRange;
					if (inRange(p, monster, tempForRange))
					{
						haveAvailableMonster = true;
						break;
					}
				}
			}
			if (haveAvailableMonster == false)
			{
				cout << "There's no target near you. (Enter 0 to continue)" << endl;
				string zero;
				cin >> zero;
				while (zero != "0")
				{
					cout << "Invalid input. " << endl;
					cout << "There's no target near you. (Enter 0 to continue)" << endl;
					cin >> zero;
				}
				continue;
			}

			p.setAttack(card.topAttack.at(i));
			//set range
			if (card.topRange <= 0)
			{
				p.setRange(1);
			}
			else
			{
				p.setRange(card.topRange);
			}
			//get target
			char target;
			cout << "Please input the target: (0 to abandon attack)" << endl;
			cin >> target;
			bool valid = false;
			//if not abandon
			if (target != '0')
			{
				while (valid == false)
				{
					for (int j = 0; j < monsters.size(); j++)
					{
						//if found and valid
						if (monsters.at(j).getCode() == target && monsters.at(j).open == true && inRange(monsters.at(j), p, p.getRange()) == true)
						{
							monsters.at(j).attacked(p.getAttack());
							cout << endl;
							cout << p.getCode() << " attack " << monsters.at(j).getCode() << " " << p.getAttack() << " damage, ";
							cout << monsters.at(j).getCode() << " shield " << monsters.at(j).getShield() << ", " << monsters.at(j).getCode() << " remain " << monsters.at(j).getHP() << " hp" << endl;
							//if dead
							if (monsters.at(j).getHP() <= 0)
							{
								monsters.at(j).dead = true;
								cout << endl << monsters.at(j).getCode() << " was killed" << endl << endl;
								monsters.erase(monsters.begin() + j);
								showMap(availableRegions);
								cout << endl;
							}
							valid = true;
							break;
						}
					}
					if (valid == false)
					{
						cout << "Invalid target. " << endl;
						cout << "Please re-enter the target: (0 to abandon attack)" << endl;
						cin >> target;
						if (target == '0')
						{
							break;
						}
					}
				}
			}
			else
			{
				cout << "Abandon attack. " << endl;
			}
		}
	}
	if (card.topShield.size() != 0)
	{
		for (int i = 0; i < card.topShield.size(); i++)
		{
			p.setShield(card.topShield.at(i));
			cout << p.getCode() << " shield " << card.topShield.at(i) << " this turn." << endl;
		}
	}
	if (card.topHeal.size() != 0)
	{
		for (int i = 0; i < card.topHeal.size(); i++)
		{
			p.setHeal(card.topHeal.at(i));
			cout << p.getCode() << " heal " << card.topHeal.at(i) << ",and now the hp is " << p.getHP() << endl;
		}
	}
}

void Gloomhaven::playerManipBot(Player& p, CharacterActCard card)
{
	if (card.botMove.size() != 0)
	{
		for (int i = 0; i < card.botMove.size(); i++)
		{
			string moveStr;
			cout << "Please enter the moving command: " << endl;
			while (1)
			{
				cin >> moveStr;
				if (moveStr.length() > card.botMove.at(i))
				{
					cout << "The length of the command is not correct (you should input less than or equals to " << card.botMove[i] << " character(s))" << endl;
					cout << "Please re-enter the moving command: " << endl;
					continue;
				}
				else if (isValidMovingCommand(moveStr) == false)
				{
					cout << "One the character of the command is not valid. " << endl;
					cout << "Please re-enter the moving command: " << endl;
					continue;
				}
				else if (move(p, moveStr) == -1)
				{
					cout << "error move!!!" << endl;
					cout << "Please re-enter the moving command: " << endl;
					continue;
				}
				else
				{
					break;
				}
			}
			showMap(availableRegions);
			cout << endl;
		}
	}
	if (card.botAttack.size() != 0)
	{
		for (int i = 0; i < card.botAttack.size(); i++)
		{
			bool haveAvailableMonster = false;
			int tempForRange = 0;
			for (Monster monster : monsters)
			{
				if (isInRegions(monster.getPosition(), availableRegions))
				{
					if (card.botRange == 0)
						tempForRange = 1;
					else
						tempForRange = card.botRange;
					if (inRange(p, monster, tempForRange))
					{
						haveAvailableMonster = true;
						break;
					}
				}
			}
			if (haveAvailableMonster == false)
			{
				cout << "There's no target near you. (Enter 0 to continue)" << endl;
				string zero;
				cin >> zero;
				while (zero != "0")
				{
					cout << "Invalid input. " << endl;
					cout << "There's no target near you. (Enter 0 to continue)" << endl;
					cin >> zero;
				}
				continue;
			}

			p.setAttack(card.botAttack.at(i));
			if (card.botRange <= 0)
			{
				p.setRange(1);
			}
			else
			{
				p.setRange(card.botRange);
			}
			char target;
			cout << "Please input the target: (0 to abandon attack)" << endl;
			cin >> target;
			if (target != '0')
			{
				bool valid = false;
				while (valid == false)
				{
					for (int j = 0; j < monsters.size(); j++)
					{
						if (monsters.at(j).getCode() == target && monsters.at(j).open == true && inRange(monsters.at(j), p, p.getRange()) == true)
						{
							monsters.at(j).attacked(p.getAttack());
							cout << endl;
							cout << p.getCode() << " attack " << monsters.at(j).getCode() << " " << p.getAttack() << " damage, ";
							cout << monsters.at(j).getCode() << " shield " << monsters.at(j).getShield() << ", " << monsters.at(j).getCode() << " remain " << monsters.at(j).getHP() << " hp" << endl;
							if (monsters.at(j).getHP() <= 0)
							{
								monsters.at(j).dead = true;
								cout << endl << monsters.at(j).getCode() << " was killed" << endl << endl;
								monsters.erase(monsters.begin() + j);
								showMap(availableRegions);
								cout << endl;
							}
							valid = true;
							break;
						}
					}
					if (valid == false)
					{
						cout << "Invalid target. " << endl;
						cout << "Please re-enter the target: (0 to abandon attack)" << endl;
						cin >> target;
						if (target == '0')
						{
							break;
						}
					}
				}
			}
			else
			{
				cout << "Abandon attack. " << endl;
			}
		}
	}
	if (card.botShield.size() != 0)
	{
		for (int i = 0; i < card.botShield.size(); i++)
		{
			p.setShield(card.botShield.at(i));
			cout << p.getCode() << " shield " << card.botShield.at(i) << " this turn." << endl;
		}
	}
	if (card.topHeal.size() != 0)
	{
		for (int i = 0; i < card.botHeal.size(); i++)
		{
			p.setHeal(card.botHeal.at(i));
			cout << p.getCode() << " heal " << card.botHeal.at(i) << ", and now the hp is " << p.getHP() << endl;
		}
	}
}

bool Gloomhaven::isValidMovingCommand(string command)
{
	bool result = true;
	for (char c : command)
	{
		if (c != 'W' && c != 'w' && c != 'A' && c != 'a' && c != 'S' && c != 's' && c != 'D' && c != 'd' && c != 'E' && c != 'e')
		{
			result = false;
			break;
		}
	}
	return result;
}

bool Gloomhaven::isNumber(string str)
{
	bool result = true;
	for (char ch : str)
	{
		if (ch > '9' || ch < '0')
		{
			result = false;
			break;
		}
	}
	return result;
}

void Gloomhaven::setColor(int color)
{
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
}