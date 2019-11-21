#include "Tile.h"

Tile::Tile(TileType t_type, sf::Vector2f t_position, sf::Vector2f t_size, sf::Font& t_font)
	:
	m_type{t_type},
	m_position{t_position},
	m_size{t_size},
	showText{ false },
	showFlowField{true},
	m_cost{-1},
	m_flowField{Direction::None}
{
	initTile();
	initText(t_font);
	m_flowfieldLine = sf::VertexArray(sf::LinesStrip, 2);
	setFlowField();
}

void Tile::updateType(TileType t_newType)
{
	m_type = t_newType;
	setColour();
}

void Tile::render(sf::RenderWindow& t_window)
{
	t_window.draw(m_tile);

	if (showText)
	{
		t_window.draw(m_costText);
	}
	if (showFlowField)
	{
		t_window.draw(m_flowfieldLine);
	}
}

sf::Vector2f Tile::getSize()
{
	return m_size;
}

bool Tile::tileTouched(sf::Vector2f t_position)
{
	if (t_position.x > m_position.x && t_position.x < m_position.x + m_size.x
		&& t_position.y > m_position.y && t_position.y < m_position.y + m_size.y)
	{
		return true;
	}
	return false;
}

TileType Tile::getType()
{
	return m_type;
}

void Tile::setCost(int t_cost)
{
	m_cost = t_cost;
	std::string costString = m_cost == INT_MAX ? std::to_string(-1) : std::to_string(m_cost);
	m_costText.setString(costString);
}

int Tile::getCost()
{
	return m_cost;
}

sf::Vector2f Tile::getPosition()
{
	sf::Vector2f tileCentre;
	tileCentre.x = m_position.x + m_tile.getGlobalBounds().width / 2.0f;
	tileCentre.y = tileCentre.x;
	return tileCentre;
}

void Tile::setEmptyTileColour(sf::Color t_colour)
{
	if (m_type == TileType::Empty)
	{
		m_colour = t_colour;
		m_tile.setFillColor(m_colour);
	}
	else
	{
		setColour();
	}
}

void Tile::toggleText()
{
	showText = !showText;
}

void Tile::toggleFlowField()
{
	showFlowField = !showFlowField;
}

void Tile::updateFlowField(Direction t_direction)
{
	m_flowField = t_direction;
	setFlowField();
}

Direction Tile::getDirection()
{
	return m_flowField;
}

void Tile::initTile()
{
	setColour();
	m_tile.setSize(m_size);
	m_tile.setPosition(m_position);
	m_tile.setOutlineColor(sf::Color::Black);
	m_tile.setOutlineThickness(-1);
}

void Tile::setColour()
{
	switch (m_type)
	{
	case TileType::Empty:
		m_colour = sf::Color::Blue;
		break;
	case TileType::Obstacle:
		m_colour = sf::Color::Black;
		break;
	case TileType::Start:
		m_colour = sf::Color::Green;
		break;
	case TileType::Goal:
		m_colour = sf::Color::Red;
		break;
	case TileType::Path:
		m_colour = sf::Color::Cyan;
		break;
	default:
		break;
	}
	m_tile.setFillColor(m_colour);
}

void Tile::initText(sf::Font & t_font)
{
	m_costText.setFont(t_font);
	m_costText.setFillColor(sf::Color::White);
	m_costText.setString(std::to_string(m_cost));
	m_costText.setCharacterSize(15);
	m_costText.setPosition(m_position);
}

void Tile::setFlowField()
{
	m_flowfieldLine[0].position = sf::Vector2f(m_position.x + m_size.x / 2.0f, m_position.y + m_size.x / 2.0f);
	m_flowfieldLine[0].color = sf::Color(192, 192, 192, 255);
	m_flowfieldLine[1].color = sf::Color(192, 192, 192, 255);
	switch (m_flowField)
	{
	case Direction::Up:
		m_flowfieldLine[1].position = sf::Vector2f(m_position.x + m_size.x / 2.0f, m_position.y);
		break;
	case Direction::Down:
		m_flowfieldLine[1].position = sf::Vector2f(m_position.x + m_size.x / 2.0f, m_position.y + m_size.y);
		break;
	case Direction::Left:
		m_flowfieldLine[1].position = sf::Vector2f(m_position.x, m_position.y + m_size.y / 2.0f);
		break;
	case Direction::Right:
		m_flowfieldLine[1].position = sf::Vector2f(m_position.x + m_size.x , m_position.y + m_size.y / 2.0f);
		break;
	case Direction::TopLeft:
		m_flowfieldLine[1].position = m_position;
		break;
	case Direction::TopRight:
		m_flowfieldLine[1].position = sf::Vector2f(m_position.x + m_size.x, m_position.y);
		break;
	case Direction::BottomLeft:
		m_flowfieldLine[1].position = sf::Vector2f(m_position.x, m_position.y + m_size.y);
		break;
	case Direction::BottomRight:
		m_flowfieldLine[1].position = m_position + m_size;
		break;
	case Direction::None:
		m_flowfieldLine[1].position = m_flowfieldLine[0].position;
		break;
	default:
		break;
	}

}
