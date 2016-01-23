#include "Level/DynamicTiles/ShiftableBlockTile.h"
#include "Spell.h"
#include "Spells/WindGustSpell.h"
#include "Registrar.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::ShiftableBlock, ShiftableBlockTile)

ShiftableBlockTile::ShiftableBlockTile(Level* level) : LevelDynamicTile(level), MovableGameObject() {
}

void ShiftableBlockTile::init() {
	setSpriteOffset(sf::Vector2f(-1.f, 0.f));
	setBoundingBox(sf::FloatRect(0.f, 0.f, static_cast<float>(m_tileSize.x) - 2.f, static_cast<float>(m_tileSize.y)));
}

void ShiftableBlockTile::load(int skinNr) {
	m_isCollidable = true;

	Animation idleAnimation(sf::seconds(10.f));
	idleAnimation.setSpriteSheet(g_resourceManager->getTexture(ResourceID::Texture_tile_shiftableblock));
	idleAnimation.addFrame(sf::IntRect(BORDER, BORDER + ((skinNr - 1) * (m_tileSize.x + 2 * BORDER)), m_tileSize.x, m_tileSize.y));

	addAnimation(GameObjectState::Idle, idleAnimation);

	// initial values
	m_state = GameObjectState::Idle;
	setCurrentAnimation(getAnimation(GameObjectState::Idle), false);
	playCurrentAnimation(true);
}

void ShiftableBlockTile::update(const sf::Time& frameTime) {
	setAcceleration(sf::Vector2f(m_pushAcceleration, GRAVITY_ACCELERATION));
	sf::Vector2f nextPosition;
	calculateNextPosition(frameTime, nextPosition);
	checkCollisions(nextPosition);
	MovableGameObject::update(frameTime);
	m_pushAcceleration = 0.f;
}

void ShiftableBlockTile::onHit(Spell* spell) {
	switch (spell->getSpellID()) {
	case SpellID::WindGust: {
		float pushAcceleration = dynamic_cast<WindGustSpell*>(spell)->getPushAcceleration();
		// determine the direction of the windgust by the position of its owner.
		if (spell->getOwner()->getPosition().x < getPosition().x) {
			m_pushAcceleration = pushAcceleration;
		}
		else {
			m_pushAcceleration = -pushAcceleration;
		}
		break;
	}
	default:
		break;
	}
}

GameObjectType ShiftableBlockTile::getConfiguredType() const {
	return LevelDynamicTile::getConfiguredType();
}

void ShiftableBlockTile::calculateUnboundedVelocity(const sf::Time& frameTime, sf::Vector2f& nextVel) const {
	// distinguish damping in the air and at the ground
	float dampingPerSec = (getVelocity().y == 0.0f) ? DAMPING_GROUND : DAMPING_AIR;
	// don't damp when there is active acceleration 
	if (getAcceleration().x != 0.0f) dampingPerSec = 0;
	nextVel.x = (getVelocity().x + getAcceleration().x * frameTime.asSeconds()) * pow(1 - dampingPerSec, frameTime.asSeconds());
	nextVel.y = getVelocity().y + getAcceleration().y * frameTime.asSeconds();
}

void ShiftableBlockTile::checkCollisions(const sf::Vector2f& nextPosition) {
	const sf::FloatRect& bb = *getBoundingBox();
	sf::FloatRect nextBoundingBoxX(nextPosition.x, bb.top, bb.width, bb.height);
	sf::FloatRect nextBoundingBoxY(bb.left, nextPosition.y, bb.width, bb.height);

	bool isMovingDown = nextPosition.y > bb.top; // the mob is always moving either up or down, because of gravity. There are very, very rare, nearly impossible cases where they just cancel out.
	bool isMovingX = nextPosition.x != bb.left;
	bool isMovingRight = nextPosition.x > bb.left;

	// check for collision on x axis
	bool collidesX = isMovingX && m_level->collides(nextBoundingBoxX, this, false, false);

	if (collidesX) {
		setAccelerationX(0.0f);
		setVelocityX(0.0f);
		if (isMovingRight) {
			setPositionX(m_level->getNonCollidingLeft(nextBoundingBoxX, this, false, false));
		}
		else {
			setPositionX(m_level->getNonCollidingRight(nextBoundingBoxX, this, false, false));
		}
	}
	else {
		nextBoundingBoxY.left = nextPosition.x;
	}

	// check for collision on y axis
	bool collidesY = m_level->collides(nextBoundingBoxY, this, false, false);
	if (collidesY) {
		setAccelerationY(0.0);
		setVelocityY(0.0f);
		if (isMovingDown) {
			setPositionY(m_level->getNonCollidingTop(nextBoundingBoxY, this, false, false));
		}
		else {
			setPositionY(m_level->getNonCollidingBottom(nextBoundingBoxY, this, false, false));
		}
	}
}