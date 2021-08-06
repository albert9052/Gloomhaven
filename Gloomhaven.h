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

	// ��Jplayers����T(���ܿ�X�]�]��F�A���L�i�H�A�u��)
	void inputPlayerData();

	// ��Xmaps�ɮש��U�Ҧ���map���W��
	void outputAllMapsName();

	// �]�w��w��map�W��
	void setMapName();

	// ���J�a���ɩҦ�����T
	void loadMap();

	// ��X��getMap������a��
	void showMap(vector<int> regions);

	// �ھڴ��Ѫ�regions��ܧ��㪺�a��
	vector<string> getMap(vector<int> regions);

	// ��X��getBlankMap������a��
	void showBlankMap(vector<int> regions, bool withInitialPosition = false);

	// �ھڴ��Ѫ�regions��ܥu����P�a�O���a�ϡA��withInitialPosition�i�M�w�O�_�n��ܪ�l��m
	vector<string> getBlankMap(vector<int> regions, bool withInitialPosition);

	// ��ܰ_�l��m(�p������w�g��n�åB�׸��A�|�M�λPmove�P�˪��W�h)
	void chooseStartPosition();

	// �ϥΩ�getFirstPosition()����sort
	static bool compareForGetFirstPosition(const Position& pos_1, const Position& pos_2);

	// �̷�command���ʩҵ������ͪ��A�p�G player ���ʥ��ѫh�^�� -1�C��L���p�Ҧ^�� 1
	// creature (Creature&): �i��Jplayer �� monster �Ӷi�沾��
	// command (string): ���ʪ����O(�i�]�A�j�p�g��WASD)
	int move(Creature& creature, string command);

	// �P�_�Ө���O�_�b���W�A�æ^�ǸӪ��� Position
	// player (Player&): �n�P�_������
	Position onDoor(Player& player);

	// ���}���ݦ�m�����A�üW�[�s�ϰ�
	// position (Position): �Ӫ�����m
	void openTheDoor(Position position);

	// �γ\�b�S�w�ϰ줺���Ǫ��ƶq
	// regions (vector<int>&): �ϰ�
	int getMonstersNum(const vector<int>& regions);

	// ��position�O�_�b������regions(vector<int>)��
	bool isInRegions(Position position, vector<int> regions);

	// �ϥΩ�loadMap()�̡A��U�Ӱϰ찵�s��
	void separateRegion();

	// �ϥΩ�separateRegion()�̡A�Χ@���j�s��
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