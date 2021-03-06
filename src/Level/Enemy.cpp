#include "Level/Enemy.h"
#include "Level/MOBBehavior/MovingBehaviors/EnemyMovingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/EnemyAttackingBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/AllyBehavior.h"
#include "Level/MOBBehavior/AttackingBehaviors/NeutralBehavior.h"
#include "Level/MOBBehavior/ScriptedBehavior/ScriptedBehavior.h"
#include "Level/Level.h"
#include "Level/LevelMainCharacter.h"
#include "Screens/LevelScreen.h"
#include "ObjectFactory.h"
#include "GameObjectComponents/InteractComponent.h"
#include "Enums/EnumNames.h"
#include "Level/DamageNumbers.h"
#include "GlobalResource.h"

using namespace std;

const float Enemy::HP_BAR_HEIGHT = 3.f;
const float Enemy::PICKUP_RANGE = 100.f;

Enemy::Enemy(const Level* level, Screen* screen) : LevelMovableGameObject(level) {
	m_mainChar = dynamic_cast<LevelScreen*>(screen)->getMainCharacter();
	m_attributes = ZERO_ATTRIBUTES;
	m_screen = screen;
	m_spellManager = new SpellManager(this);

	m_buffBar = new EnemyBuffBar(this);

	sf::Texture* cursorTexture = g_resourceManager->getTexture(GlobalResource::TEX_GUI_CURSOR);
	m_targetSprite.setTexture(*cursorTexture);
	m_targetSprite.setOrigin(sf::Vector2f(0.5f * cursorTexture->getSize().x, 0.5f * cursorTexture->getSize().y));
}

Enemy::~Enemy() {
	delete m_lootWindow;
	delete m_buffBar;
	delete m_scriptedBehavior;
}

void Enemy::load(int skinNr) {
	loadResources();
	loadAnimation(skinNr);
	loadAttributes();
	loadSpells();
	loadBehavior();
	m_spellManager->setSpellsAllied(m_isAlly);

	m_interactComponent = new InteractComponent(g_textProvider->getText(EnumNames::getEnemyName(getEnemyID()), "enemy"), this, m_mainChar);
	m_interactComponent->setInteractRange(PICKUP_RANGE);
	m_interactComponent->setInteractText("ToLoot");
	m_interactComponent->setOnInteract(std::bind(&Enemy::loot, this));
	m_interactComponent->setInteractable(false);
	m_interactComponent->setTooltipHeight(getConfiguredDistanceToHPBar() - GUIConstants::CHARACTER_SIZE_S);
	addComponent(m_interactComponent);

	delete m_damageNumbers;
	m_damageNumbers = new DamageNumbers(this->isAlly());

	if (!m_spellManager->getSpellMap().empty() && m_movingBehavior != nullptr) {
		const SpellData& spellData = m_spellManager->getSpellMap().at(0)->getSpellData();
		m_movingBehavior->setDefaultFightAnimation(spellData.fightingTime, spellData.fightAnimation);
	}
}

bool Enemy::getFleeCondition() const {
	return false;
}

void Enemy::onHit(Spell* spell) {
	if (m_isDead) {
		if (spell->getSpellID() == SpellID::RaiseTheDead)
			spell->execOnHit(this);
		return;
	}

	const MovableGameObject* owner = spell->getOwner();
	if (m_isHPBarVisible && owner != nullptr && owner->getConfiguredType() == GameObjectType::_LevelMainCharacter) {
		m_mainChar->setLastHitEnemy(this);
	}

	LevelMovableGameObject::onHit(spell);
	m_chasingTime = getConfiguredChasingTime();
	m_recoveringTime = getConfiguredRecoveringTime();
}

void Enemy::renderAfterForeground(sf::RenderTarget& renderTarget) {
	LevelMovableGameObject::renderAfterForeground(renderTarget);
	m_buffBar->render(renderTarget);
	if (m_isHPBarVisible) {
		renderTarget.draw(m_hpBar);
	}
	if (m_isTargetedEnemy) {
		renderTarget.draw(m_targetSprite);
	}
	if (m_showLootWindow && m_lootWindow != nullptr) {
		m_lootWindow->render(renderTarget);
		m_showLootWindow = false;
	}
}

void Enemy::update(const sf::Time& frameTime) {
	updateEnemyState(frameTime);
	LevelMovableGameObject::update(frameTime);
	if (m_scriptedBehavior != nullptr) {
		m_scriptedBehavior->update(frameTime);
	}
	updateHpBar();
	if (m_showLootWindow && m_lootWindow != nullptr) {
		sf::Vector2f pos(getBoundingBox()->left + getBoundingBox()->width, getBoundingBox()->top - m_lootWindow->getSize().y + 10.f);
		m_lootWindow->setPosition(pos);
	}
	m_showLootWindow = m_showLootWindow || g_inputController->isKeyActive(Key::ToggleTooltips);
	m_buffBar->update(frameTime);
	m_targetSprite.setPosition(getBoundingBox()->left + 0.5f * getBoundingBox()->width, getBoundingBox()->top + 0.5f * getBoundingBox()->height);
}

void Enemy::updateHpBar() {
	if (!m_isHPBarVisible) return;
	m_hpBar.setPosition(getBoundingBox()->left, getBoundingBox()->top - getConfiguredDistanceToHPBar());
	m_hpBar.setSize(sf::Vector2f(getBoundingBox()->width * (static_cast<float>(m_attributes.currentHealthPoints) / m_attributes.maxHealthPoints), HP_BAR_HEIGHT));
}

void Enemy::loadBehavior() {
	LevelMovableGameObject::loadBehavior();
	// cast the two behavior components so that we only have to cast them once.
	m_enemyAttackingBehavior = dynamic_cast<EnemyAttackingBehavior*>(m_attackingBehavior);
	m_enemyMovingBehavior = dynamic_cast<EnemyMovingBehavior*>(m_movingBehavior);
	// hp bar fill color is dependent on attacking behavior
	m_hpBar.setFillColor(m_enemyAttackingBehavior ? m_enemyAttackingBehavior->getConfiguredHealthColor() : sf::Color::Yellow);
	updateHpBar();
}

sf::Time Enemy::getConfiguredRecoveringTime() const {
	return sf::milliseconds(400);
}

sf::Time Enemy::getConfiguredWaitingTime() const {
	return sf::seconds(1.f);
}

sf::Time Enemy::getConfiguredRandomDecisionTime() const {
	int r = rand() % 1500 + 200;
	return sf::milliseconds(r);
}

sf::Time Enemy::getConfiguredFearedTime() const {
	return sf::seconds(6);
}

sf::Time Enemy::getConfiguredChasingTime() const {
	return sf::seconds(1);
}

GameObjectType Enemy::getConfiguredType() const {
	return GameObjectType::_Enemy;
}

void Enemy::updateEnemyState(const sf::Time& frameTime) {
	// handle dead
	if (m_enemyState == EnemyState::Dead) return;

	// update times
	updateTime(m_recoveringTime, frameTime);
	updateTime(m_waitingTime, frameTime);
	updateTime(m_chasingTime, frameTime);
	updateTime(m_decisionTime, frameTime);

	// handle stunned
	if (m_stunnedTime > sf::Time::Zero) {
		m_enemyState = EnemyState::Stunned;
		return;
	}

	// handle fear
	if (m_fearedTime > sf::Time::Zero) {
		m_enemyState = EnemyState::Fleeing;
		return;
	}

	// handle recovering
	if (m_recoveringTime > sf::Time::Zero) {
		m_enemyState = EnemyState::Recovering;
		return;
	}

	// handle chasing
	if (m_chasingTime > sf::Time::Zero) {
		m_enemyState = EnemyState::Chasing;
		return;
	}

	// the state must have been chasing a frame before. Wait now.
	if (m_enemyState == EnemyState::Chasing) {
		m_waitingTime = getConfiguredWaitingTime();
		if (m_waitingTime == sf::Time::Zero) return;
	}

	// handle waiting
	if (m_waitingTime > sf::Time::Zero) {
		m_enemyState = EnemyState::Waiting;
	}
	else {
		m_enemyState = EnemyState::Idle;
	}

	if ((m_enemyState == EnemyState::Waiting || m_enemyState == EnemyState::Idle) &&
		m_decisionTime == sf::Time::Zero) {
		// decide again
		m_decisionTime = getConfiguredRandomDecisionTime();
		m_enemyMovingBehavior->makeRandomDecision();
	}
}

bool Enemy::isAlly() const {
	return m_isAlly;
}

bool Enemy::isBoss() const {
	return m_isBoss;
}

const LevelMovableGameObject* Enemy::getCurrentTarget() const {
	return m_enemyAttackingBehavior->getCurrentTarget();
}

float Enemy::getConfiguredDistanceToHPBar() const {
	return 20.f;
}

int Enemy::getMentalStrength() const {
	return 0;
}

EnemyState Enemy::getEnemyState() const {
	return m_enemyState;
}

void Enemy::setWaiting() {
	m_waitingTime = getConfiguredWaitingTime();
}

void Enemy::setChasing() {
	m_chasingTime = getConfiguredChasingTime();
}

void Enemy::setFleeing() {
	m_fearedTime = getConfiguredFearedTime();
}

void Enemy::setLoot(const std::map<string, int>& items, int gold) {
	m_lootableItems = items;
	m_lootableGold = gold;
	delete m_lootWindow;
	m_lootWindow = nullptr;
	if (items.empty() && gold <= 0) return;
	m_lootWindow = new LootWindow();
	m_lootWindow->setLoot(items, gold);
}

void Enemy::addQuestTarget(const std::pair<std::string, std::string>& questtarget) {
	m_questTargets.push_back(questtarget);
}

void Enemy::setQuestCondition(const std::pair<std::string, std::string>& questcondition) {
	m_questCondition = questcondition;
}

void Enemy::setObjectID(int id) {
	m_objectID = id;
}

void Enemy::setAlly(const sf::Time& ttl) {
	m_isAlly = true;
	delete m_movingBehavior;
	m_movingBehavior = createMovingBehavior(true);
	m_enemyMovingBehavior = dynamic_cast<EnemyMovingBehavior*>(m_movingBehavior);

	delete m_attackingBehavior;
	m_attackingBehavior = createAttackingBehavior(true);
	AllyBehavior* allyBehavior = dynamic_cast<AllyBehavior*>(m_attackingBehavior);

	if (allyBehavior == nullptr) {
		g_logger->logError("Enemy::setAlly", "Enemy can't be risen as an ally, no AllyBehavior or NeutralBehavior created.");
		return;
	}

	allyBehavior->setTimeToLive(ttl);
	m_enemyAttackingBehavior = allyBehavior;
	m_hpBar.setFillColor(m_enemyAttackingBehavior->getConfiguredHealthColor());
	m_isAlwaysUpdate = true;

	m_spellManager->setSpellsAllied(true);

	setDebugBoundingBox(COLOR_GOOD);
}

void Enemy::setUnique(bool value) {
	m_isUnique = value;
}

void Enemy::setFeared(const sf::Time& fearedTime) {
	if (m_isDead) return;
	LevelMovableGameObject::setFeared(fearedTime);
	m_buffBar->addFeared(fearedTime);
}

void Enemy::setStunned(const sf::Time& stunnedTime) {
	if (m_isDead) return;
	LevelMovableGameObject::setStunned(stunnedTime);
	m_buffBar->addStunned(stunnedTime);
}

void Enemy::addDamageOverTime(DamageOverTimeData& data) {
	if (m_isDead || data.damageType == DamageType::VOID) return;
	m_buffBar->addDotBuff(data.duration, data.damageType);
	LevelMovableGameObject::addDamageOverTime(data);
}

void Enemy::onMouseOver() {
	if (m_isLooted) return;
	LevelMovableGameObject::onMouseOver();
	if (m_isDead && !isAlly()) {
		setSpriteColor(COLOR_INTERACTIVE, sf::milliseconds(100));
		m_showLootWindow = true;
	}
}

void Enemy::loot() {
	if (m_isLooted) return;
	m_mainChar->lootItems(m_lootableItems);
	m_mainChar->addGold(m_lootableGold);
	notifyLooted();
	setDisposed();
}

void Enemy::onRightClick() {
	if (m_isDead && !isAlly()) {
		// check if the enemy body is in range
		if (dist(m_mainChar->getCenter(), getCenter()) <= PICKUP_RANGE) {
			loot();
		}
		else {
			m_screen->setTooltipText("OutOfRange", COLOR_BAD, true);
		}
	}
	else if (!m_isDead && !isAlly() && m_isHPBarVisible) {
		m_mainChar->setTargetEnemy(this);
	}
	g_inputController->lockAction();
}

void Enemy::setScriptedBehavior(const std::string& luaPath) {
	delete m_scriptedBehavior;
	m_scriptedBehavior = new ScriptedBehavior(luaPath, this);
	if (m_scriptedBehavior->isError()) {
		delete m_scriptedBehavior;
		m_scriptedBehavior = nullptr;
	}
	else {
		m_isAlwaysUpdate = true;
	}
}

void Enemy::setBoss(bool value) {
	m_isBoss = value;
}

void Enemy::setDead() {
	if (m_isImmortal || m_mainChar->isDead()) return;
	LevelMovableGameObject::setDead();
	m_buffBar->clear();

	if (m_isTargetedEnemy) {
		m_mainChar->setTargetEnemy(nullptr);
	}
	if (m_isLastHitEnemy) {
		m_mainChar->setLastHitEnemy(nullptr);
	}

	if (isAlly()) {
		setDisposed();
		return;
	}

	if (m_isUnique) {
		notifyKilled();
	}

	if (m_scriptedBehavior != nullptr) {
		m_scriptedBehavior->onDeath();
	}

	if (m_isBoss) {
		// loot (but without set disposed)
		m_mainChar->lootItems(m_lootableItems);
		m_mainChar->addGold(m_lootableGold);
		notifyLooted();
		m_interactComponent->setInteractable(false);

		// TODO: what if we have multiple bosses?
		dynamic_cast<LevelScreen*>(m_screen)->notifyBossKilled(m_lootableItems, m_lootableGold);
	}
	else {
		m_interactComponent->setInteractable(true);
	}
}

void Enemy::notifyLooted() {
	m_screen->getCharacterCore()->setEnemyLooted(m_mainChar->getLevel()->getID(), m_objectID);
	m_isLooted = true;
}

void Enemy::notifyKilled() {
	if (m_screen->getCharacterCore()->isEnemyKilled(m_mainChar->getLevel()->getID(), m_objectID)) return;
	m_screen->getCharacterCore()->setEnemyKilled(m_mainChar->getLevel()->getID(), m_objectID);
	for (auto& target : m_questTargets) {
		if (!target.first.empty()) {
			dynamic_cast<LevelScreen*>(m_screen)->notifyQuestTargetKilled(target.first, target.second);
		}
	}
	if (!m_questCondition.first.empty()) {
		dynamic_cast<LevelScreen*>(m_screen)->notifyQuestConditionFulfilled(m_questCondition.first, m_questCondition.second);
	}
}

void Enemy::setMovingTarget(int x, int y) {
	m_enemyMovingBehavior->setMovingTarget(x, y);
}

void Enemy::resetMovingTarget() {
	m_enemyMovingBehavior->resetMovingTarget();
}

void Enemy::setTargeted(bool targeted) {
	m_isTargetedEnemy = targeted;
}

void Enemy::setLastHit(bool lastHit) {
	m_isLastHitEnemy = lastHit;
}