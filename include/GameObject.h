#pragma once

#include "global.h"
#include "InputController.h"

#include "Enums/GameObjectState.h"
#include "Enums/GameObjectType.h"

class Screen;

// A game object with position, bounding box, game state that can be added to a screen.
class GameObject {
public:
	GameObject() {}
	virtual ~GameObject() {}

	virtual void update(const sf::Time& frameTime);
	virtual void render(sf::RenderTarget& renderTarget);
	// used for tooltips and loot windows
	virtual void renderAfterForeground(sf::RenderTarget& renderTarget);
	// gets checked & called in the update loop. default implementation does nothing.
	// checks for the bounding box 
	virtual void onMouseOver();
	// gets checked & called in the update loop. default implementation does nothing.
	// checks for the bounding box 
	// right click is used to interact with game objects
	virtual void onRightClick();
	// left click is used to fire spells / press buttons
	virtual void onLeftClick();
	virtual void onRightJustPressed();
	virtual void onLeftJustPressed();
	virtual void onInteractKey();

	void setBoundingBox(const sf::FloatRect& rect);
	virtual void setPosition(const sf::Vector2f& pos);
	virtual void setPositionX(float posX);
	virtual void setPositionY(float posY);
	virtual void setDisposed();
	
	virtual void setState(GameObjectState state);
	virtual void setScreen(Screen* screen);
	void setInputInDefaultView(bool value);
	virtual void setViewable(bool value);
	
	virtual const sf::Vector2f& getPosition() const;
	const sf::FloatRect* getBoundingBox() const;
	sf::Vector2f getCenter() const;
	// is the object currently visible inside this view + margin?
	bool isViewable() const;
	// returns whether the game object should be deleted
	// if this is set, the game object gets deleted in the next game loop
	bool isDisposed() const;
	virtual GameObjectType getConfiguredType() const = 0;
	Screen* getScreen() const;
	GameObjectState getGameObjectState() const;

	// convenience method for updating time values (used in many GOs). 
	// The frame time is subtracted from the time but 
	// if the time falls below sf::Time::Zero, it is set to sf::Time::Zero.
	static void updateTime(sf::Time &time, const sf::Time &frameTime);
	virtual void setDebugBoundingBox(const sf::Color &debugColor);

protected:
	GameObjectState m_state;

	Screen* m_screen = nullptr;
	sf::RectangleShape m_debugBox;

	bool m_isDisposed = false;
	bool m_isViewable = true;

	sf::FloatRect m_boundingBox;
	// absolute position as seen from the upper left corner
	sf::Vector2f m_position;

	bool m_isDrawBoundingBox = false;
	bool m_isInputInDefaultView = false;
};