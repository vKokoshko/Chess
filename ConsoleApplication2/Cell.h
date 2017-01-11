#pragma once
#include "Units.h"

class Cell
{
	sf::Vector2f spriteXY;
	int mark;
	Unit *unit;
	sf::Texture *texture;
	sf::Sprite *sprite;
	static myImage image;
public:
	Cell();
	Cell(const Cell&);
	~Cell();
	Cell& operator=(const Cell&);
	void markCell(int);
	void unmarkCell();
	friend class Map;
};


