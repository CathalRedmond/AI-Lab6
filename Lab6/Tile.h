#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "ScreenSize.h"
#include "Enums.h"

class Tile
{
public:
	Tile(TileType t_type, sf::Vector2f t_position, sf::Vector2f t_size,sf::Font & t_font);
	void render(sf::RenderWindow& t_window);

	sf::Vector2f getSize();
	sf::Vector2f getCentreOfTile();
	TileType getType();
	Direction getDirection();
	int getCost();

	void updateColour(sf::Color t_colour);
	void updateType(TileType t_newType);
	void updateCost(int t_cost);
	void updateFlowField(Direction t_direction);

	void toggleText();
	void toggleFlowField();
private:
	void initTile();
	void setColour();
	void initText(sf::Font& t_font);
	void setFlowField();

	sf::Color m_colour;
	const sf::Color m_flowFieldColour;
	sf::Vector2f m_position;
	sf::Vector2f m_size;
	sf::RectangleShape m_tile;
	sf::VertexArray m_flowFieldLine[2];
	const int DIRECTION = 0;
	const int ARROW = 1;
	sf::Text m_costText;

	TileType m_type;
	Direction m_flowField;

	int m_cost;
	bool showText;
	bool showFlowField;
};

