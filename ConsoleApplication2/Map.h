#pragma once
#include "Cell.h"
//#include <iterator>

class Map
{
	sf::Texture bg_texture, grid_texture;
	sf::Sprite bg_sprite, grid_sprite;
	Cell **cell;
public:
	Map();
	Map(const Map&);
	~Map();
	void mark(int, int, int);
	void unmark(int, int);
	void unmarkAll();
	int getCellMark(int, int);
	Unit* getCellUnit(int, int);
	void setCellUnit(int, int, Unit*);
	sf::Sprite& getBgSprite();
	sf::Sprite& getGridSprite();
	sf::Sprite* getCellSprite(int, int);
	void highlight(Unit*);
};