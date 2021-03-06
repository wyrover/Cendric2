#include "Screens/OptionsScreen.h"
#include "Screens/MenuScreen.h"
#include "Screens/KeyBindingsScreen.h"
#include "Screens/ScreenManager.h"

using namespace std;

OptionsScreen::OptionsScreen(CharacterCore* core) : Screen(core) {
}

void OptionsScreen::execUpdate(const sf::Time& frameTime) {
	if (g_inputController->isKeyActive(Key::Escape)) {
		onBack();
		return;
	}

	updateObjects(GameObjectType::_Button, frameTime);
	updateTooltipText(frameTime);
}

void OptionsScreen::render(sf::RenderTarget& renderTarget) {
	renderTarget.setView(renderTarget.getDefaultView());
	renderTarget.draw(*m_title);
	renderObjects(GameObjectType::_Button, renderTarget);
	renderTooltipText(renderTarget);
}

void OptionsScreen::execOnEnter(const Screen *previousScreen) {
	// title
	m_title = new BitmapText(g_textProvider->getText("Options"), TextStyle::Shadowed);
	m_title->setCharacterSize(GUIConstants::CHARACTER_SIZE_XXXL);
	m_title->setPosition(sf::Vector2f((WINDOW_WIDTH - m_title->getLocalBounds().width) / 2.f, 50.f));

	float distFromTop = 150.f;
	float distFromLeft = 150.f;

	// language
	m_languageSelector = new ArrowSelector();
	m_languageSelector->setLabelText("Language");
	for (int language = 1; language < static_cast<int>(Language::MAX); ++language) {
		m_languageSelector->addOption(EnumNames::getLanguageName(static_cast<Language>(language)));
	}
	m_languageSelector->setOptionIndex(static_cast<int>(g_resourceManager->getConfiguration().language) - 1);
	m_languageSelector->setPosition(sf::Vector2f(distFromLeft, distFromTop));
	addObject(m_languageSelector);

	distFromTop = distFromTop + 75;

	// displayMode
	m_displayModeSelector = new ArrowSelector();
	m_displayModeSelector->setLabelText("DisplayMode");
	for (int mode = 1; mode < static_cast<int>(Language::MAX); ++mode) {
		m_displayModeSelector->addOption(EnumNames::getDisplayModeName(static_cast<DisplayMode>(mode)));
	}
	m_displayModeSelector->setOptionIndex(static_cast<int>(g_resourceManager->getConfiguration().displayMode) - 1);
	m_displayModeSelector->setPosition(sf::Vector2f(distFromLeft, distFromTop));
	addObject(m_displayModeSelector);

	distFromTop = distFromTop + 150;

	m_volumeSoundSlider = new Slider(0, 100);
	string volumeText = g_textProvider->getText("SoundVolume");
	m_volumeSoundSlider->setTextRaw(volumeText);
	m_volumeSoundSlider->setUnit("%");
	m_volumeSoundSlider->setSliderPosition(g_resourceManager->getConfiguration().volumeSound);
	m_volumeSoundSlider->setPosition(sf::Vector2f(distFromLeft, distFromTop));
	addObject(m_volumeSoundSlider);

	distFromTop = distFromTop + 100;

	m_volumeMusicSlider = new Slider(0, 100);
	volumeText = g_textProvider->getText("MusicVolume");
	m_volumeMusicSlider->setTextRaw(volumeText);
	m_volumeMusicSlider->setUnit("%");
	m_volumeMusicSlider->setSliderPosition(g_resourceManager->getConfiguration().volumeMusic);
	m_volumeMusicSlider->setPosition(sf::Vector2f(distFromLeft, distFromTop));
	addObject(m_volumeMusicSlider);

	distFromTop = 150.f;
	distFromLeft = WINDOW_WIDTH / 2.f + 150.f;

	// quickcast
	m_quickCastCheckbox = new Checkbox();
	m_quickCastCheckbox->setPosition(sf::Vector2f(distFromLeft, distFromTop));
	m_quickCastCheckbox->setChecked(g_resourceManager->getConfiguration().isQuickcast);
	m_quickCastCheckbox->setText("Quickcast");
	addObject(m_quickCastCheckbox);

	distFromTop = distFromTop + 50;

	// display hints
	m_displayHintsCheckbox = new Checkbox();
	m_displayHintsCheckbox->setPosition(sf::Vector2f(distFromLeft, distFromTop));
	m_displayHintsCheckbox->setChecked(g_resourceManager->getConfiguration().isDisplayHints);
	m_displayHintsCheckbox->setText("DisplayHints");
	addObject(m_displayHintsCheckbox);

	distFromTop = distFromTop + 50;

	// display damage numbers
	m_displayDamageNumbersCheckbox = new Checkbox();
	m_displayDamageNumbersCheckbox->setPosition(sf::Vector2f(distFromLeft, distFromTop));
	m_displayDamageNumbersCheckbox->setChecked(g_resourceManager->getConfiguration().isDisplayDamageNumbers);
	m_displayDamageNumbersCheckbox->setText("DisplayDamageNumbers");
	addObject(m_displayDamageNumbersCheckbox);

	distFromTop = distFromTop + 50;

	// smoothing
	m_smoothingCheckbox = new Checkbox();
	m_smoothingCheckbox->setPosition(sf::Vector2f(distFromLeft, distFromTop));
	m_smoothingCheckbox->setChecked(g_resourceManager->getConfiguration().isSmoothing);
	m_smoothingCheckbox->setText("Smoothing");
	addObject(m_smoothingCheckbox);

	distFromTop = distFromTop + 50;

	// vsync
	m_vSyncCheckbox = new Checkbox();
	m_vSyncCheckbox->setPosition(sf::Vector2f(distFromLeft, distFromTop));
	m_vSyncCheckbox->setChecked(g_resourceManager->getConfiguration().isVSyncEnabled);
	m_vSyncCheckbox->setText("VSync");
	addObject(m_vSyncCheckbox);

	distFromTop = distFromTop + 50;

	// sound	
	m_soundCheckbox = new Checkbox();
	m_soundCheckbox->setPosition(sf::Vector2f(distFromLeft, distFromTop));
	m_soundCheckbox->setChecked(g_resourceManager->getConfiguration().isSoundOn);
	m_soundCheckbox->setText("Sound");
	addObject(m_soundCheckbox);

	distFromTop = distFromTop + 100;

	// keyboard mappings button
	Button* button = new Button(sf::FloatRect(distFromLeft, distFromTop, 200, 50), GUIOrnamentStyle::SMALL);
	button->setText("KeyBindings");
	button->setCharacterSize(12);
	button->setOnClick(std::bind(&OptionsScreen::onKeybindings, this));
	addObject(button);

	// back
	button = new Button(sf::FloatRect(60, WINDOW_HEIGHT - 80, 200, 50), GUIOrnamentStyle::SMALL);
	button->setText("Back");
	button->setOnClick(std::bind(&OptionsScreen::onBack, this));
	addObject(button);
	// apply
	button = new Button(sf::FloatRect(WINDOW_WIDTH - 260, WINDOW_HEIGHT - 80, 200, 50), GUIOrnamentStyle::SMALL);
	button->setText("Apply");
	button->setOnClick(std::bind(&OptionsScreen::onApply, this));
	addObject(button);
}

void OptionsScreen::execOnExit(const Screen *nextScreen) {
	// delete texts (buttons are deleted automatically by the screen)
	delete m_title;
}

void OptionsScreen::onBack() {
	setNextScreen(new MenuScreen(m_characterCore));
}

void OptionsScreen::onApply() {
	ConfigurationData& config = g_resourceManager->getConfiguration();
	DisplayMode mode = static_cast<DisplayMode>(m_displayModeSelector->getChosenOptionIndex() + 1);

	bool screenChanged = config.displayMode != mode;
	screenChanged = screenChanged || config.isVSyncEnabled != m_vSyncCheckbox->isChecked();
	screenChanged = screenChanged || config.isSmoothing != m_smoothingCheckbox->isChecked();
	Language language = static_cast<Language>(m_languageSelector->getChosenOptionIndex() + 1);
	bool languageChanged = config.language != language;
	bool soundOn = m_soundCheckbox->isChecked();
	bool soundChanged = soundOn != config.isSoundOn;
	soundChanged = soundChanged || config.volumeSound != m_volumeSoundSlider->getSliderPosition();
	soundChanged = soundChanged || config.volumeMusic != m_volumeMusicSlider->getSliderPosition();

	config.language = language;
	config.displayMode = mode;
	config.isSoundOn = soundOn;
	config.isQuickcast = m_quickCastCheckbox->isChecked();
	config.isDisplayHints = m_displayHintsCheckbox->isChecked();
	config.isDisplayDamageNumbers = m_displayDamageNumbersCheckbox->isChecked();
	config.isSmoothing = m_smoothingCheckbox->isChecked();
	config.isVSyncEnabled = m_vSyncCheckbox->isChecked();
	config.volumeMusic = m_volumeMusicSlider->getSliderPosition();
	config.volumeSound = m_volumeSoundSlider->getSliderPosition();

	ConfigurationWriter writer;
	writer.saveToFile(config);
	g_textProvider->reload();
	setTooltipText("ConfigurationSaved", COLOR_GOOD, true);
	if (screenChanged) {
		config.isWindowReload = true;
	}
	if (languageChanged) {
		setNextScreen(new OptionsScreen(getCharacterCore()));
	}
	if (languageChanged || soundChanged) {
		m_screenManager->clearBackupScreen();
	}
}

void OptionsScreen::onKeybindings() {
	setNextScreen(new KeyBindingsScreen(m_characterCore));
}
