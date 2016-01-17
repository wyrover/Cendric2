#include "Screens/MenuScreen.h"

using namespace std;

MenuScreen::MenuScreen(CharacterCore* core) : Screen(core) {
	m_screenSprite = sf::Sprite((*g_resourceManager->getTexture(ResourceID::Texture_screen_menu)));
}

Screen* MenuScreen::execUpdate(const sf::Time& frameTime) {
	if (m_startNewGame) {
		return new LoadingScreen(m_characterCore);
	}
	else if ((g_inputController->isKeyActive(Key::Escape) && m_characterCore == nullptr) || m_exitButton->isClicked()) {
		// end the game
		m_requestQuit = true;
	}
	else if ((m_resumeGameButton != nullptr && m_resumeGameButton->isClicked()) || (g_inputController->isKeyActive(Key::Escape) && m_characterCore != nullptr)) {
		// resume game
		return new LoadingScreen(m_characterCore);
	}
	else if (m_newGameButton->isClicked() && m_yesOrNoForm == nullptr) {
		if (m_characterCore == nullptr) {
			// we start a new game with an empty character core
			m_characterCore = new CharacterCore();
			m_characterCore->loadNew();
			return new LoadingScreen(m_characterCore);
		}
		else {
			m_newCharacterCore = new CharacterCore();
			m_newCharacterCore->loadNew();
			m_yesOrNoForm = new YesOrNoForm(sf::FloatRect(400, 350, 450, 200));
			m_yesOrNoForm->setMessage("QuestionStartNewGame");
			m_yesOrNoForm->setOnNoClicked(std::bind(&MenuScreen::onNo, this));
			m_yesOrNoForm->setOnYesClicked(std::bind(&MenuScreen::onStartNewGame, this));
			addObject(m_yesOrNoForm);
			setAllButtonsEnabled(false);
		}
	}
	else if (m_loadGameButton->isClicked()) {
		return new LoadGameScreen(m_characterCore);
	}
	else if (m_saveGameButton->isClicked() && m_characterCore != nullptr) {
		return new SaveGameScreen(m_characterCore);
	}
	else if (m_optionsButton->isClicked()) {
		return new OptionsScreen(m_characterCore);
	}
	else if (m_creditsButton->isClicked()) {
		return new CreditsScreen(m_characterCore);
	}
	updateTooltipText(frameTime);
	updateObjects(GameObjectType::_Undefined, frameTime);
	updateObjects(GameObjectType::_Button, frameTime);
	updateObjects(GameObjectType::_Form, frameTime);
	return this;
}

void MenuScreen::setAllButtonsEnabled(bool value) {
	Screen::setAllButtonsEnabled(value);
	m_saveGameButton->setEnabled(value && (m_characterCore != nullptr));
}

void MenuScreen::render(sf::RenderTarget &renderTarget) {
	renderTarget.setView(renderTarget.getDefaultView());
	renderTarget.draw(m_screenSprite);
	renderTarget.draw(m_versionText);
	renderObjects(GameObjectType::_Undefined, renderTarget);
	renderObjects(GameObjectType::_Button, renderTarget);
	renderObjects(GameObjectType::_Form, renderTarget);
	renderTooltipText(renderTarget);
}

void MenuScreen::execOnEnter(const Screen *previousScreen) {
	// add burning fire baskets
	FireBasket* fireBasket1 = new FireBasket();
	FireBasket* fireBasket2 = new FireBasket();
	fireBasket1->setPosition(sf::Vector2f(60.f, 120.f));
	fireBasket2->setPosition(sf::Vector2f(1028.f, 120.f));
	addObject(fireBasket1);
	addObject(fireBasket2);

	// add version nr
	m_versionText.setString("Cendric v" + std::string(CENDRIC_VERSION_NR));
	m_versionText.setCharacterSize(8);
	m_versionText.setColor(sf::Color::White);
	m_versionText.setPosition(
		(WINDOW_WIDTH - m_versionText.getLocalBounds().width) / 2,
		WINDOW_HEIGHT - 18.f);

	float buttonHeight = 50.f;
	float buttonWidth = 300.f;
	float xOffset = (WINDOW_WIDTH - buttonWidth) / 2.f;
	float yOffset = 200.f;
	float addYOffset = 70.f;

	// add buttons
	if (m_characterCore != nullptr) {
		m_resumeGameButton = new Button(sf::FloatRect(xOffset, yOffset, buttonWidth, buttonHeight), ButtonOrnamentStyle::MEDIUM);
		m_resumeGameButton->setText("Resume");
		addObject(m_resumeGameButton);
	}
	yOffset += addYOffset;
	m_newGameButton = new Button(sf::FloatRect(xOffset, yOffset, buttonWidth, buttonHeight), ButtonOrnamentStyle::MEDIUM);
	m_newGameButton->setText("NewGame");
	yOffset += addYOffset;
	m_loadGameButton = new Button(sf::FloatRect(xOffset, yOffset, buttonWidth, buttonHeight), ButtonOrnamentStyle::MEDIUM);
	m_loadGameButton->setText("LoadGame");
	yOffset += addYOffset;
	m_saveGameButton = new Button(sf::FloatRect(xOffset, yOffset, buttonWidth, buttonHeight), ButtonOrnamentStyle::MEDIUM);
	m_saveGameButton->setText("SaveGame");
	m_saveGameButton->setEnabled(m_characterCore != nullptr);
	yOffset += addYOffset;
	m_optionsButton = new Button(sf::FloatRect(xOffset, yOffset, buttonWidth, buttonHeight), ButtonOrnamentStyle::MEDIUM);
	m_optionsButton->setText("Options");
	yOffset += addYOffset;
	m_creditsButton = new Button(sf::FloatRect(xOffset, yOffset, buttonWidth, buttonHeight), ButtonOrnamentStyle::MEDIUM);
	m_creditsButton->setText("Credits");
	yOffset += addYOffset;
	m_exitButton = new Button(sf::FloatRect(xOffset, yOffset, buttonWidth, buttonHeight), ButtonOrnamentStyle::MEDIUM);
	m_exitButton->setText("Exit");
	addObject(m_newGameButton);
	addObject(m_loadGameButton);
	addObject(m_optionsButton);
	addObject(m_creditsButton);
	addObject(m_exitButton);
	addObject(m_saveGameButton);
}

void MenuScreen::execOnExit(const Screen *nextScreen) {
	g_resourceManager->deleteResource(ResourceID::Texture_screen_menu);
	g_resourceManager->deleteResource(ResourceID::Texture_screen_splash_fireanimation);
	delete m_newCharacterCore;
}

// <<< agents for the yes or no form >>>

void MenuScreen::onStartNewGame() {
	m_yesOrNoForm->setDisposed();
	m_yesOrNoForm = nullptr;
	delete m_characterCore;
	m_characterCore = m_newCharacterCore;
	m_newCharacterCore = nullptr;
	m_startNewGame = true;
}

void MenuScreen::onNo() {
	m_yesOrNoForm = nullptr;
	delete m_newCharacterCore;
	m_newCharacterCore = nullptr;
	setAllButtonsEnabled(true);
}