#pragma once
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <fstream>
#include "MonsterActCard.h"
#include "MonsterData.h"

vector<MonsterData> readMonsterData(string fileName);
bool compareMonsterCard(MonsterActCard a, MonsterActCard b);