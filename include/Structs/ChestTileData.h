#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "Structs/LightData.h"

struct ChestTileData {
	int objectID;
	sf::Vector2f spawnPosition;
	int skinNr;
	int chestStrength = 0;
	bool isOpen = false;
	bool isPermanent = false;
	std::string keyItemID = "";
	std::string tooltipText = "";
	LightData lightData;
	std::pair<std::map<std::string, int>, int> loot;
	std::pair<std::string, std::string> conditionProgress;
};