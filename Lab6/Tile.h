#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "ScreenSize.h"
enum class TileType
{
	Empty,
	Obstacle,
	Start,
	Goal,
	Path
};

enum class Direction
{
	Up,
	Down,
	Left,
	Right,
	TopLeft,
	TopRight,
	BottomLeft,
	BottomRight,
	None
};

class Tile
{
public:
	Tile(TileType t_type, sf::Vector2f t_position, sf::Vector2f t_size,sf::Font & t_font);
	void updateType(TileType t_newType);
	void render(sf::RenderWindow& t_window);
	sf::Vector2f getSize();
	bool tileTouched(sf::Vector2f t_position);
	TileType getType();
	void setCost(int t_cost);
	int getCost();
	sf::Vector2f getPosition();
	void setEmptyTileColour(sf::Color t_colour);

	void toggleText();
	void toggleFlowField();
	void updateFlowField(Direction t_direction);
	Direction getDirection();
private:
	void initTile();
	void setColour();
	sf::Color m_colour;
	sf::Vector2f m_position;
	sf::Vector2f m_size;
	int m_cost;
	TileType m_type;
	sf::RectangleShape m_tile;

	sf::Text m_costText;
	void initText(sf::Font& t_font);

	bool showText;
	Direction m_flowField;
	sf::VertexArray m_flowfieldLine;

	void setFlowField();

	bool showFlowField;
};

