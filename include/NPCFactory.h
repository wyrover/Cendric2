#pragma once

#include <vector>
#include <map>

#include "global.h"

#include "Structs/NPCBean.h"

// contains information for each and every npc and fills npc beans.
// will be ported to a db-like structure (.csv)
class NPCFactory
{
public:
	// fills the npc bean for the item with id 'id'. 
	// this method assumes the referenced bean is already initialized with default values (DEFAULT_NPC)
	void loadNPCBean(NPCBean& npc, const std::string& id);
};