#include "Spells/RaiseTheDeadSpell.h"
#include "Level/Enemy.h"
#include "ObjectFactory.h"
#include "GlobalResource.h"

RaiseTheDeadSpell::~RaiseTheDeadSpell() {
	delete m_ps;
}

void RaiseTheDeadSpell::load(const SpellData& bean, LevelMovableGameObject* mob, const sf::Vector2f& target) {
	Spell::load(bean, mob, target);
	loadParticleSystem();
}

void RaiseTheDeadSpell::update(const sf::Time& frameTime) {
	m_ps->update(frameTime);
	Spell::update(frameTime);
	updateParticleSystemPosition();
}

void RaiseTheDeadSpell::execOnHit(LevelMovableGameObject* target) {
	if (!target->isDead()) return;
	Enemy* enemy = dynamic_cast<Enemy*>(target);
	if (enemy == nullptr) return;
	if (enemy->getMentalStrength() >= m_data.strength) {
		setDisposed();
		return;
	}

	AttributeData attributes = ZERO_ATTRIBUTES;
	attributes.damagePhysical = m_data.damage;
	attributes.damageFire = m_data.damage;
	attributes.damageIce = m_data.damage;
	attributes.damageLight = m_data.damage;
	attributes.damageShadow = m_data.damage;

	// add an allied copy of that mob.
	Enemy* copy = ObjectFactory::Instance()->createEnemy(enemy->getEnemyID(), m_level, m_screen);
	copy->load(0);
	copy->setAlly(m_data.duration);
	copy->addAttributes(m_data.duration, attributes);
	copy->setPosition(enemy->getPosition());
	m_screen->addObject(copy);
	setDisposed();
}

bool RaiseTheDeadSpell::checkCollisionsWithEnemies(const sf::FloatRect* boundingBox) {
	// this method is overridden to guarantee that the spell only hits once
	// and that the iterator is not invalidated (we change the enemy vector size on the fly)
	for (auto& go : *m_enemies) {
		if (!go->isViewable()) continue;
		Enemy* enemy = dynamic_cast<Enemy*>(go);
		if (enemy != nullptr && (enemy->getBoundingBox()->intersects(*boundingBox))) {
			enemy->onHit(this);
			return true;
		}
	}
	return false;
}

void RaiseTheDeadSpell::render(sf::RenderTarget& target) {
	Spell::render(target);
	m_ps->render(target);
}

void RaiseTheDeadSpell::loadParticleSystem() {
	m_ps = new particles::TextureParticleSystem(50, g_resourceManager->getTexture(GlobalResource::TEX_PARTICLE_BLOB));
	m_ps->additiveBlendMode = true;
	m_ps->emitRate = 50.0f;

	// Generators
	auto spawner = m_ps->addSpawner<particles::BoxSpawner>();
	spawner->center = sf::Vector2f(getPosition().x + getBoundingBox()->width / 2.f, getPosition().y + getBoundingBox()->height / 2.f);
	spawner->size = sf::Vector2f(getBoundingBox()->width, 0.f);
	m_particleSpawner = spawner;

	auto sizeGen = m_ps->addGenerator<particles::SizeGenerator>();
	sizeGen->minStartSize = 10.f;
	sizeGen->maxStartSize = 20.f;
	sizeGen->minEndSize = 2.f;
	sizeGen->maxEndSize = 6.f;

	auto colGen = m_ps->addGenerator<particles::ColorGenerator>();
	colGen->minStartCol = sf::Color(31, 86, 28, 100);
	colGen->maxStartCol = sf::Color(44, 132, 57, 150);
	colGen->minEndCol = sf::Color(0, 255, 0, 0);
	colGen->maxEndCol = sf::Color(200, 255, 200, 0);

	auto velGen = m_ps->addGenerator<particles::AngledVelocityGenerator>();
	velGen->minAngle = 0.f;
	velGen->maxAngle = 360.f;
	velGen->minStartSpeed = 5.f;
	velGen->maxStartSpeed = 10.f;

	auto timeGen = m_ps->addGenerator<particles::TimeGenerator>();
	timeGen->minTime = 0.5f;
	timeGen->maxTime = 1.f;

	// Updaters
	m_ps->addUpdater<particles::TimeUpdater>();
	m_ps->addUpdater<particles::ColorUpdater>();
	m_ps->addUpdater<particles::EulerUpdater>();
	m_ps->addUpdater<particles::SizeUpdater>();
}

void RaiseTheDeadSpell::updateParticleSystemPosition() {
	m_particleSpawner->center.x = getPosition().x + getBoundingBox()->width / 2;
	m_particleSpawner->center.y = getPosition().y + getBoundingBox()->height / 2;
}