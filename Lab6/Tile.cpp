#include "Tile.h"

Tile::Tile(TileType t_type, sf::Vector2f t_position, sf::Vector2f t_size, sf::Font& t_font)
	:
	m_type{ t_type },
	m_position{ t_position },
	m_size{ t_size },
	showText{ false },
	showFlowField{ true },
	m_cost{ -1 },
	m_flowField{ Direction::None },
	m_flowFieldColour{ sf::Color(192,192,192,255) }
{
	initTile();
	initText(t_font);
	for (int index = 0; index < 2; index++)
	{
		m_flowFieldLine[DIRECTION] = sf::VertexArray(sf::LinesStrip, 2);
		m_flowFieldLine[ARROW] = sf::VertexArray(sf::LinesStrip, 3);
	}
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
	if (showText) t_window.draw(m_costText);
	if (showFlowField)
	{
		for (int index = 0; index < 2; index++)
		{
			t_window.draw(m_flowFieldLine[index]);
		}
	}
}

sf::Vector2f Tile::getSize()
{
	return m_size;
}

TileType Tile::getType()
{
	return m_type;
}

int Tile::getCost()
{
	return m_cost;
}

void Tile::updateCost(int t_cost)
{
	m_cost = t_cost;
	std::string costString = m_cost == INT_MAX ? std::to_string(999) : std::to_string(m_cost);
	m_costText.setString(costString);
}


sf::Vector2f Tile::getCentreOfTile()
{
	return m_position + (m_size /2.0f);
}

void Tile::updateColour(sf::Color t_colour)
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
		m_colour = sf::Color::Yellow;
		break;
	default:
		m_colour = sf::Color::Blue;
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
	m_flowFieldLine[0][DIRECTION].position = sf::Vector2f(m_position.x + m_size.x / 2.0f, m_position.y + m_size.x / 2.0f);


	for (int index = 0; index < 2; index++)
	{
		m_flowFieldLine[0][index].color = m_flowFieldColour;
		m_flowFieldLine[1][index].color = m_flowFieldColour;
	}

	switch (m_flowField)
	{
	case Direction::Up:
		m_flowFieldLine[1][DIRECTION].position = sf::Vector2f(m_position.x + m_size.x / 2.0f, m_position.y);
		break;
	case Direction::Down:
		m_flowFieldLine[1][DIRECTION].position = sf::Vector2f(m_position.x + m_size.x / 2.0f, m_position.y + m_size.y);
		break;
	case Direction::Left:
		m_flowFieldLine[1][DIRECTION].position = sf::Vector2f(m_position.x, m_position.y + m_size.y / 2.0f);
		break;
	case Direction::Right:
		m_flowFieldLine[1][DIRECTION].position = sf::Vector2f(m_position.x + m_size.x , m_position.y + m_size.y / 2.0f);
		break;
	case Direction::TopLeft:
		m_flowFieldLine[1][DIRECTION].position = m_position;
		break;
	case Direction::TopRight:
		m_flowFieldLine[1][DIRECTION].position = sf::Vector2f(m_position.x + m_size.x, m_position.y);
		break;
	case Direction::BottomLeft:
		m_flowFieldLine[1][DIRECTION].position = sf::Vector2f(m_position.x, m_position.y + m_size.y);
		break;
	case Direction::BottomRight:
		m_flowFieldLine[1][DIRECTION].position = m_position + m_size;
		break;
	case Direction::None:
		m_flowFieldLine[1][DIRECTION].position = m_flowFieldLine[0][DIRECTION].position;
		break;
	default:
		m_flowFieldLine[1][DIRECTION].position = m_flowFieldLine[0][DIRECTION].position;
		break;
	}

}
