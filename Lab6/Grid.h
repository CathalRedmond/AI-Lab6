#pragma once
#include "Tile.h"
#include <SFML/Graphics.hpp>
#include <iostream>
class Grid
{
public:
	Grid();
	void setWindowSize(float t_windowSize);
	void processEvent(sf::Event t_event, sf::RenderWindow& t_window);
	void render(sf::RenderWindow& t_window);
private:
	void initGrid();
	void updateTileCost();
	std::vector<int> findNeighboursIndex(int currentIndex);
	std::vector<Tile> m_tileGrid;
	sf::Vector2f m_tileSize;
	static const int NUMBER_OF_TILES = 2500;
	bool m_leftButtonPressed;
	bool m_rightButtonPressed;


	std::vector<std::vector<int>> m_tileNeighbours;

	int getIndexByPosition(sf::Vector2f t_mousePosition);


	void updateTile(int t_tileIndex, TileType t_type);

	int m_goalIndex;
	int m_startIndex;
	void clearGoal();
	void clearStart();

	float m_windowSize;

	std::vector<int> getValidNeighbours(int t_index);

	void handleFlowFieldNeighbours(int t_index);

	sf::Font m_font;

	void resetBoard();
	int m_highestCost;
	void toggleHeatMap();
	bool m_showHeatMap;

	void setFlowField();

	float distanceFormula(sf::Vector2f t_pointOne, sf::Vector2f t_pointTwo);

	void createPath();
};

