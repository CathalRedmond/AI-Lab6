#pragma once
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