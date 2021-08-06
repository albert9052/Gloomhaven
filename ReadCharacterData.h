#pragma once
#include <fstream>
#include "CharacterActCard.h"
#include "CharacterData.h"

vector<CharacterData> readCharacterData(string fileName);
bool compareCharacterCard(CharacterActCard a, CharacterActCard b);