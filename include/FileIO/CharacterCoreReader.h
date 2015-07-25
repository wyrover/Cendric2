#pragma once

#include "global.h"
#include "FileIO/Reader.h"
#include "Logger.h"

#include "Enums/ItemType.h"
#include "Structs/CharacterCoreData.h"
#include "FileIO/CharacterCoreIO.h"

// a reader to read a savegame.
class CharacterCoreReader : public Reader, public CharacterCoreIO
{
public:
	bool readCharacterCore(const std::string& fileName, CharacterCoreData& data);

private:
	bool readTimePlayed(char* start, char* end, CharacterCoreData& data) const;
	bool readSavegameName(char* start, char* end, CharacterCoreData& data) const;
	bool readSavegameDate(char* start, char* end, CharacterCoreData& data) const;

	bool readMapID(char* start, char* end, CharacterCoreData& data) const;
	bool readMapPosition(char* start, char* end, CharacterCoreData& data) const;
	bool readLevelID(char* start, char* end, CharacterCoreData& data) const;
	bool readLevelPosition(char* start, char* end, CharacterCoreData& data) const;
	bool readLevelKilled(char* start, char* end, CharacterCoreData& data) const;
	bool readLevelLooted(char* start, char* end, CharacterCoreData& data) const;
	bool readQuestStates(char* start, char* end, CharacterCoreData& data) const;
	bool readNPCStates(char* start, char* end, CharacterCoreData& data) const;

	bool readAttributes(char* start, char* end, CharacterCoreData& data) const;
	
	bool readGold(char* start, char* end, CharacterCoreData& data) const;
	bool readItemID(char* start, char* end, CharacterCoreData& data) const;
	bool readEquippedItem(char* start, char* end, CharacterCoreData& data, ItemType type) const;

	// check map bean for validity before loading the savegame
	bool checkData(CharacterCoreData& data) const;
};