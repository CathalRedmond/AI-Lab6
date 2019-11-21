#include "Grid.h"

Grid::Grid()
	:
	m_leftButtonPressed{ false },
	m_rightButtonPressed{ false },
	m_goalIndex{ -1 },
	m_startIndex{ -1 },
	m_highestCost{50}
{
	if (!m_font.loadFromFile("ASSETS/FONTS/ariblk.ttf"))
	{
		std::cout << "Error Loading Font" << std::endl;
	}
}

void Grid::setWindowSize(float t_windowSize)
{
	m_windowSize = t_windowSize;
	m_tileSize = sf::Vector2f(m_windowSize / 50, m_windowSize / 50);
	initGrid();
}

void Grid::processEvent(sf::Event t_event, sf::RenderWindow & t_window)
{
	
	sf::Vector2f mousePosition = sf::Vector2f(sf::Mouse::getPosition(t_window).x, sf::Mouse::getPosition(t_window).y);
	int tileIndex = getIndexByPosition(mousePosition);
	if (tileIndex != -1)
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !m_leftButtonPressed)
		{
			bool doCostUpdate = false;
			if (m_tileGrid[tileIndex].getType() == TileType::Obstacle) doCostUpdate = true;
			updateTile(tileIndex, TileType::Start);
			m_leftButtonPressed = true;
			if (m_goalIndex != 1 && doCostUpdate)
			{
				updateTileCost();
				createPath();
			}
		}
		else if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && !m_rightButtonPressed)
		{
			updateTile(tileIndex, TileType::Goal);
			m_rightButtonPressed = true;
			updateTileCost();
			if (m_startIndex != -1) createPath();
		}
		else if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
		{
			updateTile(tileIndex, TileType::Obstacle);
		}

		if (!sf::Mouse::isButtonPressed(sf::Mouse::Left) && m_leftButtonPressed)
		{
			m_leftButtonPressed = false;
		}
		else if (!sf::Mouse::isButtonPressed(sf::Mouse::Right) && m_rightButtonPressed)
		{
			m_rightButtonPressed = false;
		}
		else if (!sf::Mouse::isButtonPressed(sf::Mouse::Middle))
		{
			if (m_goalIndex != -1) updateTileCost();
			if (m_goalIndex != -1 && m_startIndex != -1) createPath();
		}
	}
	if (t_event.type == sf::Event::KeyReleased)
	{
		if (t_event.key.code == sf::Keyboard::Q)
		{
			for (int index = 0; index < NUMBER_OF_TILES; index++)
			{
				m_tileGrid[index].toggleText();
			}
		}
		else if (t_event.key.code == sf::Keyboard::R)
		{
			resetBoard();
		}
	}
}

void Grid::render(sf::RenderWindow& t_window)
{
	for (int index = 0; index < NUMBER_OF_TILES; index++)
	{
		m_tileGrid[index].render(t_window);
	}
}

void Grid::initGrid()
{
	sf::Vector2f position = sf::Vector2f();
	for (int index = 0; index < NUMBER_OF_TILES; index++)
	{
		m_tileGrid.push_back(Tile(TileType::Empty, position, m_tileSize, m_font));
		position.x += m_tileSize.x;
		if ((index + 1) % 50 == 0)
		{
			position.y += m_tileSize.y;
			position.x = 0;
		}
	}
}

void Grid::updateTileCost()
{
	for (int index = 0; index < NUMBER_OF_TILES; index++)
	{
		int cost = m_tileGrid[index].getType() == TileType::Obstacle ? INT_MAX : -1;
		m_tileGrid[index].setCost(cost);
	}
	m_highestCost = -1;
	m_tileGrid[m_goalIndex].setCost(0);
	std::vector<int> neighbours = getValidNeighbours(m_goalIndex);
	while (!neighbours.empty())
	{
		std::vector<int> newNeighbours = getValidNeighbours(neighbours[0]);
		neighbours.insert(neighbours.end(), newNeighbours.begin(), newNeighbours.end());
		neighbours.erase(neighbours.begin());
	}

	for (int index = 0; index < NUMBER_OF_TILES; index++)
	{
		if (m_tileGrid[index].getCost() == -1)
		{
			m_tileGrid[index].setCost(INT_MAX);
		}
		float blue;
		float cost = m_tileGrid[index].getCost();
		if (cost == INT_MAX)
		{
			blue = 30;
		}
		else
		{
			blue = 255 - 255 * ((cost + 1) / (m_highestCost * 2.0f));
		}


		if (blue < 45) blue = 45;
		m_tileGrid[index].setEmptyTileColour(sf::Color{ 0,0,sf::Uint8(blue),255 });
		handleFlowFieldNeighbours(index);
	}


	
}

std::vector<int> Grid::getValidNeighbours(int t_index)
{
	std::vector<int> neighbours;
	int neighbourIndexArray[] =
	{
		t_index - 1,
		t_index + 1,
		t_index - 50,
		t_index + 50,
		t_index - 51,
		t_index + 49,
		t_index + 51,
		t_index - 49
	};

	for (int index = 0; index < 8; index++)
	{
		if (neighbourIndexArray[index] > -1 && neighbourIndexArray[index] < NUMBER_OF_TILES)
		{
			sf::Vector2f currentPosition = m_tileGrid[t_index].getPosition();
			sf::Vector2f neighbourPosition = m_tileGrid[neighbourIndexArray[index]].getPosition();
			float distanceBetween = sqrt(std::pow((neighbourPosition.x - currentPosition.x), 2) + std::pow((neighbourPosition.y - currentPosition.y), 2));
			if (distanceBetween <= m_tileGrid[neighbourIndexArray[index]].getSize().x * 1.5)
			{
				if (m_tileGrid[neighbourIndexArray[index]].getCost() == -1)
				{
					int cost = (m_tileGrid[t_index].getCost()) + 1;
					if (cost > m_highestCost)
					{
						m_highestCost = cost;
					}
					m_tileGrid[neighbourIndexArray[index]].setCost(cost);
					neighbours.push_back(neighbourIndexArray[index]);
				}
			}
		}
	}
	return neighbours;
}

void Grid::handleFlowFieldNeighbours(int t_index)
{
	int currentIndex = t_index;
	int neighbourArray[] =
	{
		/*t_index - 1,
		t_index + 1,
		t_index - 50,
		t_index + 50,
		t_index - 51,
		t_index + 49,
		t_index + 51,
		t_index - 49*/

		//Up
		t_index - 50,
		//Down
		t_index + 50,
		//Left
		t_index -1,
		//Right
		t_index +1,
		// Top Left
		t_index -51,
		//Top Right
		t_index -49,
		//Bottom Left
		t_index + 49,
		// Bottom Right
		t_index + 51
		
	};

	//int neighbourArray[8];
	//neighbourArray[]
	int cost = m_tileGrid[t_index].getCost();
	float heursticCost = std::numeric_limits<float>::max();
	int closestIndex = -1;
	if (cost > 0 && cost != -1 && cost != INT_MAX)
	{
		for (int index = 0; index < 8; index++)
		{
			if (neighbourArray[index] > -1 && neighbourArray[index] < NUMBER_OF_TILES)
			{
				sf::Vector2f currentPosition = m_tileGrid[t_index].getPosition();
				sf::Vector2f neighbourPosition = m_tileGrid[neighbourArray[index]].getPosition();
				float distanceBetween = sqrt(std::pow((neighbourPosition.x - currentPosition.x), 2) + std::pow((neighbourPosition.y - currentPosition.y), 2));
				if (distanceBetween <= m_tileGrid[neighbourArray[index]].getSize().x * 1.5)
				{
					int neighbourCost = m_tileGrid[neighbourArray[index]].getCost();
					if (neighbourCost != INT_MAX && neighbourCost != -1)
					{
						float closestCost = (neighbourCost * 100) + distanceFormula(m_tileGrid[m_goalIndex].getPosition(), m_tileGrid[neighbourArray[index]].getPosition());
						if (closestCost < heursticCost)
						{
							heursticCost = closestCost;
							closestIndex = index;
						}
					}
				}
			}
		}

		Direction fieldDirection = Direction::None;
		if (neighbourArray[closestIndex] == t_index - 50)
		{
			fieldDirection = Direction::Up;
		}
		else if (neighbourArray[closestIndex] == t_index + 50)
		{
			fieldDirection = Direction::Down;

		}
		else if (neighbourArray[closestIndex] == t_index - 1)
		{
			fieldDirection = Direction::Left;

		}
		else if (neighbourArray[closestIndex] == t_index + 1)
		{
			fieldDirection = Direction::Right;

		}
		else if (neighbourArray[closestIndex] == t_index - 51)
		{
			fieldDirection = Direction::TopLeft;

		}
		else if (neighbourArray[closestIndex] == t_index - 49)
		{
			fieldDirection = Direction::TopRight;

		}
		else if (neighbourArray[closestIndex] == t_index + 49)
		{
			fieldDirection = Direction::BottomLeft;

		}
		else if (neighbourArray[closestIndex] == t_index + 51)
		{
			fieldDirection = Direction::BottomRight;

		}
		m_tileGrid[t_index].updateFlowField(fieldDirection);
	}
	else
	{
		m_tileGrid[t_index].updateFlowField(Direction::None);
	}
}
void Grid::resetBoard()
{
	for (int index = 0; index < NUMBER_OF_TILES; index++)
	{
		if (m_tileGrid[index].getType() != TileType::Empty)
		{
			m_tileGrid[index].updateType(TileType::Empty);
		}
		m_tileGrid[index].setCost(-1);
		m_tileGrid[index].setEmptyTileColour(sf::Color::Blue);
		m_goalIndex = -1;
		m_startIndex = -1;
		m_tileGrid[index].updateFlowField(Direction::None);
	}
}

void Grid::toggleHeatMap()
{
	m_showHeatMap = !m_showHeatMap;
}

void Grid::setFlowField()
{
	std::vector<int> neighbours;
	for (int index = 0; index < NUMBER_OF_TILES; index++)
	{
		neighbours = getValidNeighbours(index);
	}
}

float Grid::distanceFormula(sf::Vector2f t_pointOne, sf::Vector2f t_pointTwo)
{
	return sqrt(std::pow((t_pointOne.x - t_pointTwo.x), 2) + std::pow((t_pointOne.y - t_pointTwo.y), 2));
}

void Grid::createPath()
{
	for (int index = 0; index < NUMBER_OF_TILES; index++)
	{
		if (m_tileGrid[index].getType() == TileType::Path)
		{
			m_tileGrid[index].updateType(TileType::Empty);
		}
	}
	int currentIndex = m_startIndex;
	Direction movementDir ;
	while (currentIndex != m_goalIndex)
	{
		movementDir = m_tileGrid[currentIndex].getDirection();
		switch (movementDir)
		{
		case Direction::Up:
			currentIndex -= 50;
			break;
		case Direction::Down:
			currentIndex += 50;
			break;
		case Direction::Left:
			currentIndex -= 1;
			break;
		case Direction::Right:
			currentIndex += 1;
			break;
		case Direction::TopLeft:
			currentIndex -= 51;
			break;
		case Direction::TopRight:
			currentIndex -= 49;
			break;
		case Direction::BottomLeft:
			currentIndex += 49;
			break;
		case Direction::BottomRight:
			currentIndex += 51;
			break;
		case Direction::None:
			currentIndex = m_goalIndex;
			break;
		default:
			break;
		}
		if (m_tileGrid[currentIndex].getType() != TileType::Goal)
		{
			m_tileGrid[currentIndex].updateType(TileType::Path);
		}
	}
}

int Grid::getIndexByPosition(sf::Vector2f t_mousePosition)
{
	if (t_mousePosition.x < 0 || t_mousePosition.x > m_windowSize
		|| t_mousePosition.y < 0 || t_mousePosition.y > m_windowSize)
	{
		return -1;
	}
	float gridSize = m_tileGrid[0].getSize().x;
	int y = t_mousePosition.y / gridSize;
	int x = t_mousePosition.x / gridSize;
	return (y * 50) + x;
}

void Grid::updateTile(int t_tileIndex, TileType t_type)
{
	if (t_type == TileType::Goal)
	{
		clearGoal();
		m_goalIndex = t_tileIndex;
	}
	else if (t_type == TileType::Start)
	{
		clearStart();
		m_startIndex = t_tileIndex;
	}
	m_tileGrid[t_tileIndex].updateType(t_type);
}

void Grid::clearGoal()
{
	if (m_goalIndex != -1)
	{
		m_tileGrid[m_goalIndex].updateType(TileType::Empty);
	}
}

void Grid::clearStart()
{
	if (m_startIndex != -1)
	{
		m_tileGrid[m_startIndex].updateType(TileType::Empty);
	}
}

