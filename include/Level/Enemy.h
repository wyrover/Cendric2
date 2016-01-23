#pragma once

#include "global.h"
#include "LevelMovableGameObject.h"
#include "GUI/LootWindow.h"

#include "Structs/AttributeData.h"

#include "Enums/EnemyID.h"
#include "Enums/SpellID.h"
#include "Enums/DamageType.h"
#include "Enums/EnemyState.h"
#include "Enums/EnemyAttitude.h"

#include "Level/EnemyBuffBar.h"

class Level;
class LevelMainCharacter;
class Spell;
class EnemyMovingBehavior;
class EnemyAttackingBehavior;

// An enemy in a level
class Enemy : public virtual LevelMovableGameObject {
public:
	Enemy(Level* level, Screen* screen);
	virtual ~Enemy();

	void load(EnemyID id);

	void renderAfterForeground(sf::RenderTarget& target) override;
	void onRightClick() override;
	void onMouseOver() override;
	void update(const sf::Time& frameTime) override;

	void onHit(Spell* spell) override;
	// the enemy flees for the given time
	void setFeared(const sf::Time& fearedTime);
	// the enemy is stunned for the given time
	void setStunned(const sf::Time& stunnedTime);
	void addDamageOverTime(const DamageOverTimeData& data) override;
	void setLoot(const std::map<std::string, int>& items, int gold);
	void setQuestTarget(const std::pair<std::string, std::string>& questtarget);
	void setDead() override;
	// an enemy that is persistent will not be marked as dead and respawn with every level reset
	void setPersistent(bool value);
	// the object ID in the level enemy object layer.
	void setObjectID(int id);
	// sets the enemy as ally. The enemy dies after the time to live has run out.
	void setAlly(const sf::Time& ttl);

	GameObjectType getConfiguredType() const override;
	EnemyID getEnemyID() const;
	EnemyAttitude getAttitude() const;
	EnemyState getEnemyState() const;
	// a level, ranges from 0 to 4. An enemy can only be feared / stunned / resurrected, if the level of its spell is high enough.
	// default is 0. A enemy with level 4 can never be feared / stunned / controlled or affected in any other way!!
	virtual int getMentalStrength() const;
	// determines the distance from the top of the enemies bounding box to its
	// hp bar. Default is 20px.
	virtual float getConfiguredDistanceToHPBar() const;
	// returns whether the enemy is an ally of cendric
	bool isAlly() const;
	// sets the waiting time to the configured waiting time
	void setWaiting();
	// sets the chasing time to the configured chasing time
	void setChasing();
	// sets the feared time to the configured feared time
	void setFleeing();
	const LevelMovableGameObject* getCurrentTarget() const;

	// returns false as a default. can be anything, for example if the enemy hp drops below some limit
	virtual bool getFleeCondition() const;

	virtual void insertDefaultLoot(std::map<std::string, int>& loot, int& gold) const = 0;

protected:
	LevelMainCharacter* m_mainChar;
	// loads attributes and adds immune spells + enemies. all attributes are set to zero before that call. default does nothing.
	virtual void loadAttributes() = 0;
	// loads spells and adds them to the spell manager. default does nothing.
	virtual void loadSpells() = 0;
	// loads the animation
	virtual void loadAnimation() = 0;
	// loads/updates hp bar
	virtual void updateHpBar();
	// updates the hp bar after loading the behavior
	void loadBehavior() override;

	EnemyID m_id = EnemyID::VOID;
	int m_objectID = -1;
	bool m_isPersistent = false;
	// spells of these damage types won't hurt. default is empty.
	std::vector<DamageType> m_immuneDamageTypes;

	// AI
	EnemyState m_enemyState = EnemyState::Idle;
	virtual void updateEnemyState(const sf::Time& frameTime);
	EnemyAttackingBehavior* m_enemyAttackingBehavior = nullptr;
	EnemyMovingBehavior* m_enemyMovingBehavior = nullptr;
	
	// time until the enemy can attack after it has taken a hit
	sf::Time m_recoveringTime = sf::Time::Zero;
	// time stunned
	sf::Time m_stunnedTime = sf::Time::Zero;
	// time feared
	sf::Time m_fearedTime = sf::Time::Zero;
	// time while the enemy does nothing because it tried to get to the main char and it failed
	sf::Time m_waitingTime = sf::Time::Zero;
	// time until next random desicion in idle state
	sf::Time m_decisionTime = sf::Time::Zero;
	// time the enemy will chase the main char anyway (even if it is out of aggro range)
	sf::Time m_chasingTime = sf::Time::Zero;

	virtual sf::Time getConfiguredRecoveringTime() const;
	virtual sf::Time getConfiguredRandomDecisionTime() const;
	// time feared after the fear condition is true (has nothing to do with spells)
	virtual sf::Time getConfiguredFearedTime() const;
	virtual sf::Time getConfiguredWaitingTime() const;
	virtual sf::Time getConfiguredChasingTime() const;

private:
	sf::RectangleShape m_hpBar;
	static const float HP_BAR_HEIGHT;

	// lootable items 
	std::map<std::string, int> m_lootableItems;
	int m_lootableGold;
	LootWindow *m_lootWindow = nullptr;
	bool m_showLootWindow = false;

	// is this enemy a quest target?
	std::pair<std::string, std::string> m_questTarget;

	// the enemy can only be looted if the main char is in this range
	static const float PICKUP_RANGE;

	EnemyBuffBar* m_buffBar = nullptr;
};