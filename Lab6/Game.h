#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "ScreenSize.h"
#include "Grid.h"

class Game
{
public:
	Game();
	void run();
private:
	void processEvents();
	void update(sf::Time t_deltaTime);
	void render();
	
	sf::RenderWindow m_window;
	Grid m_grid;
	float m_windowSize;
};



