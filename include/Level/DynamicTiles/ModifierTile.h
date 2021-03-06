#pragma once

#include "global.h"
#include "Level/LevelDynamicTile.h"
#include "Structs/SpellModifier.h"
#include "LightObject.h"
#include "Particles/ParticleSystem.h"

class ModifierTile : public LevelDynamicTile {
public:
	ModifierTile(LevelScreen* levelScreen);
	virtual ~ModifierTile();

	void render(sf::RenderTarget& target) override;
	void update(const sf::Time& frameTime) override;

	void init() override;
	void loadAnimation(int skinNr) override;
	void onHit(Spell* spell) override;
	void onHit(LevelMovableGameObject* mob) override;

	void setPosition(const sf::Vector2f& pos) override;

	void setModifier(const SpellModifier& modififer);
	LevelDynamicTileID getDynamicTileID() const override { return LevelDynamicTileID::Modifier; }

private:
	SpellModifier m_modifier;
	sf::Time m_particleTime = sf::seconds(3.f);

	particles::TextureParticleSystem* m_ps = nullptr;
	particles::AngledVelocityGenerator* m_velGenerator = nullptr;
	particles::ParticleSpawner* m_particleSpawner = nullptr;

	void addModifier();

	void loadParticleSystem();

	void updateParticleSystemPosition();
};