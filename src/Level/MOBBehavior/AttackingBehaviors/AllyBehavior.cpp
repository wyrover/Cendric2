#include "Level/MOBBehavior/AttackingBehaviors/AllyBehavior.h"

AllyBehavior::AllyBehavior(Enemy* enemy) :
	EnemyAttackingBehavior(enemy),
	AttackingBehavior(enemy) {
}

void AllyBehavior::update(const sf::Time& frameTime) {
	EnemyAttackingBehavior::update(frameTime);
	GameObject::updateTime(m_timeToLive, frameTime);
	if (m_hasTimeToLive) {
		if (m_timeToLive == sf::Time::Zero) {
			m_enemy->setDead();
		}
		if (m_timeToLive < sf::seconds(5)) {
			m_enemy->setSpriteColor(sf::Color(255, 255, 255, m_timeToLive.asMilliseconds() / 20), sf::seconds(1));
		}
	}
}

void AllyBehavior::setTimeToLive(const sf::Time& ttl) {
	if (ttl <= sf::Time::Zero) {
		m_hasTimeToLive = false;
		return;
	}
	m_hasTimeToLive = true;
	m_timeToLive = ttl;
}

sf::Color AllyBehavior::getConfiguredHealthColor() const {
	return sf::Color::Green;
}

void AllyBehavior::updateAggro() {
	if (m_enemy->getEnemyState() == EnemyState::Chasing && m_enemy->getFleeCondition()) {
		m_enemy->setFleeing();
		return;
	}
	if (m_currentTarget == nullptr || m_currentTarget->isDead() || m_currentTarget->isDisposed()) {
		m_currentTarget = nullptr;
	}
	if (m_enemy->getEnemyState() != EnemyState::Idle) return;

	bool isInAggroRange = false;

	// search for new target
	Enemy* nearest = nullptr;
	float nearestDistance = 10000.f;
	for (auto& go : *m_enemies) {
		if (!go->isViewable()) continue;
		Enemy* enemy = dynamic_cast<Enemy*>(go);
		if (enemy->isDead() || enemy->isAlly()) continue;
		sf::Vector2f dist = go->getCenter() - m_enemy->getCenter();
		float distance = sqrt(dist.x * dist.x + dist.y * dist.y);
		if (distance < nearestDistance) {
			nearestDistance = distance;
			nearest = enemy;
		}
	}
	if (nearest == nullptr || nearestDistance > m_aggroRange) {
		m_currentTarget = nullptr;
		m_enemy->setWaiting();
		return;
	}
	m_currentTarget = nearest;
	m_enemy->setChasing();
}

