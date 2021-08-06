#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <io.h>

#include "CharacterData.h"
#include "MonsterData.h"
#include "Player.h"
#include "Monster.h"
#include "Obstacle.h"
#include "Position.h"

using namespace std;

typedef struct tempStructForGetTargetSequence
{
	char code;
	int sequence;
	int agility;
	float range;
} TS;

class Gloomhaven
{
public:

	Gloomhaven(string characterFileName, string monsterFileName, int _debugMode = 0);

	// Print out all the text in the file
	// introductionFileName (string): the file you want to print out to the console
	void showIntroduction(string introductionFileName);

	// Start the game
	void startGame();



private:

	// 輸入players的資訊(提示輸出也包辦了，不過可以再優化)
	void inputPlayerData();

	// 輸出maps檔案底下所有的map的名稱
	void outputAllMapsName();

	// 設定選定的map名稱
	void setMapName();

	// 載入地圖檔所有的資訊
	void loadMap();

	// 輸出由getMap獲取的地圖
	void showMap(vector<int> regions);

	// 根據提供的regions顯示完整的地圖
	vector<string> getMap(vector<int> regions);

	// 輸出由getBlankMap獲取的地圖
	void showBlankMap(vector<int> regions, bool withInitialPosition = false);

	// 根據提供的regions顯示只有牆與地板的地圖，而withInitialPosition可決定是否要顯示初始位置
	vector<string> getBlankMap(vector<int> regions, bool withInitialPosition);

	// 選擇起始位置(如有角色已經選好並且擋路，會套用與move同樣的規則)
	void chooseStartPosition();

	// 使用於getFirstPosition()內的sort
	static bool compareForGetFirstPosition(const Position& pos_1, const Position& pos_2);

	// 依照command移動所給予的生物，如果 player 移動失敗則回傳 -1。其他情況皆回傳 1
	// creature (Creature&): 可放入player 或 monster 來進行移動
	// command (string): 移動的指令(可包括大小寫的WASD)
	int move(Creature& creature, string command);

	// 判斷該角色是否在門上，並回傳該門的 Position
	// player (Player&): 要判斷的角色
	Position onDoor(Player& player);

	// 打開所屬位置的門，並增加新區域
	// position (Position): 該門的位置
	void openTheDoor(Position position);

	// 或許在特定區域內的怪物數量
	// regions (vector<int>&): 區域
	int getMonstersNum(const vector<int>& regions);

	// 該position是否在給予的regions(vector<int>)內
	bool isInRegions(Position position, vector<int> regions);

	// 使用於loadMap()裡，對各個區域做編號
	void separateRegion();

	// 使用於separateRegion()裡，用作遞迴編號
	void placeNumber(int x, int y);

	// Get all the files within the directory
	// path (string): the path of the directory you want to search
	// files (vector<string>&): it will push all the files' name in this vector
	void getFiles(string path, vector<string>& files);

	// Get all the .txt files within the directory
	// path (string): the path of the directory you want to search
	// files (vector<string>&): it will push all the .txt files' name in this vector
	void getTxtFiles(string path, vector<string>& files);

	void eventDetermine(vector<Player>& players);

	void update(vector<Player>& players, vector<Monster>& monsters);

	static bool cmp(Creature* a, Creature* b);

	void playerManipUp(Player& p, CharacterActCard card);

	void playerManipBot(Player& p, CharacterActCard card);

	bool isValidMovingCommand(string command);

	bool inRange(Creature& creature1, Creature& creature2, float range);

	int getTargetSequence(Monster& monster, float range);

	static bool compareForGetTargetSequence(TS ts1, TS ts2);

	bool isNumber(string str);

	void setColor(int color = 7);

	int debugMode;

	int playersNum;

	int monstersNum;

	int totalRegionNum = 1;

	Position firstPosition;

	vector<Player> players;

	vector<Monster> monsters;

	string mapName;

	vector<string> map;

	vector<Position> initialPosition;

	vector<MonsterData> allMonsters;

	vector<CharacterData> allCharacters;

	vector<int> availableRegions;

	vector<Obstacle> obstacles;

	vector<Position> walls;

};