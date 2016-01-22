#pragma once

#include "global.h"
#include "Level/EnemyBehavior/AttackingBehavior.h"

// An allied enemy attacking behavior. These enemies help cendric and attack unfriendly enemies
class AllyBehavior : public AttackingBehavior {
public:
	AllyBehavior(Enemy* enemy) : AttackingBehavior(enemy) {};
	~AllyBehavior() {};

	void update(const sf::Time& frameTime) override;
	void updateAggro() override;

	// sets the time to live for an allied enemy
	void setTimeToLive(const sf::Time& ttl);

	EnemyAttitude getAttitude() const override;
	sf::Color getConfiguredHealthColor() const override;

private:
	sf::Time m_timeToLive = sf::Time::Zero;
};