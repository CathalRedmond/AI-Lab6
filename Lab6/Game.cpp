#include "Game.h"

Game::Game():
	m_windowSize{sf::VideoMode::getDesktopMode().height - 100.0f}
{
	m_window.create(sf::VideoMode(m_windowSize, m_windowSize), "Lab 6", sf::Style::None);
	m_grid.setWindowSize(m_windowSize);
}

void Game::run()
{	
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps);
	while (m_window.isOpen())
	{
		processEvents(); 
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents();
			update(timePerFrame);
		}
		render();
	}
}

void Game::processEvents()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if ( sf::Event::KeyPressed == event.type) 
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				m_window.close();
			}
		}
		if (sf::Event::Closed == event.type)
		{
			m_window.close();
		}
		m_grid.processEvent(event, m_window);
	}
}

void Game::update(sf::Time t_deltaTime)
{

}

void Game::render()
{
	m_window.clear(sf::Color::White);
	m_grid.render(m_window);
	m_window.display();
}