#include "Game.h"

Game::Game() : m_screenManager(new SplashScreen())
{
	m_mainWindow.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT + BOTTOM_BORDER), "Cendric");
	m_mainWindow.setFramerateLimit(MAX_FRAME_RATE);
	g_inputController->setWindow(&m_mainWindow);
	m_running = true;
}

void Game::run()
{
	sf::Clock frameClock;
	sf::Time frameTime;
	frameTime = frameClock.restart();
	while (m_running)
	{
		sf::Event e;
		sf::Time deltaTime;
		
		// don't count this loop into the frametime!
		deltaTime = frameClock.restart();
		while (m_mainWindow.pollEvent(e))
		{
			if (e.type == sf::Event::Closed || g_inputController->isKeyActive(Key::Escape))
			{
				m_running = false;
			}
			else if (e.type == sf::Event::Resized)
			{
				g_inputController->setCurrentWindowSize(e.size.width, e.size.height);
			}
		}
		
		// time
		frameClock.restart();
		// hard bound: dt should not exeed 50ms (20fps)
		frameTime = deltaTime.asMilliseconds() > 50 ? sf::milliseconds(50) : deltaTime;

		// input
		g_inputController->update();

		// game updates
		m_screenManager.update(frameTime);
		if (m_screenManager.getCurrentScreen()->isQuitRequested())
		{
			m_running = false;
		}
		if (g_resourceManager->pollError()->first != ErrorID::Void)
		{
			m_screenManager.setErrorScreen();
		}

		// render
		m_mainWindow.clear();
		m_screenManager.render(m_mainWindow);
		if (DEBUG_RENDERING)
		{
			showFPSText(m_mainWindow, frameTime.asSeconds());
		}
		
		m_mainWindow.display();
	}

	m_mainWindow.close();
}

void Game::showFPSText(sf::RenderTarget& target, float frameTimeSeconds)
{
	sf::View oldView = target.getView();
	target.setView(target.getDefaultView());
	m_fpsList.push_back(frameTimeSeconds);
	if (m_fpsList.size() > FPS_AVERAGE_NR)
	{
		m_fpsList.pop_front();
	}
	// calc average
	float sum = 0.f;
	for (float f : m_fpsList)
	{
		sum += f;
	}
	int fps = static_cast<int>(1.f / (sum / FPS_AVERAGE_NR));
	sf::Text fpsText = sf::Text(
		"FPS: " + std::to_string(fps),
		(*g_resourceManager->getFont(ResourceID::Font_copperplateGothicBold)));
	fpsText.setColor(sf::Color::Red);
	fpsText.setPosition(sf::Vector2f(1150.f, 10.f));
	fpsText.setCharacterSize(20);

	target.draw(fpsText);
	target.setView(oldView);
}