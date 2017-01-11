#include "Map.h"

Map::Map()
{
	bg_texture.loadFromFile("images/background_001.png");
	bg_sprite.setTexture(bg_texture);
	grid_texture.loadFromFile("images/grid.png");
	grid_sprite.setTexture(grid_texture);
	cell = new Cell*[8];
	for (int i = 0; i < 8; i++)
		cell[i] = new Cell[8];
	for (int y = 0; y < 8; y++)
		for (int x = 0; x < 8; x++)
		{
			cell[y][x].spriteXY.x = 80.0f + 80.0f * x;
			cell[y][x].spriteXY.y = 100.0f + 55.0f * y;
		}
}
Map::Map(const Map &obj)
{
	bg_texture = obj.bg_texture;
	grid_texture = obj.grid_texture;
	bg_sprite.setTexture(bg_texture);
	bg_sprite.setPosition(0, 0);
	grid_sprite.setTexture(grid_texture);
	grid_sprite.setPosition(0, 0);
	cell = new Cell*[8];
	for (int i = 0; i < 8; i++)
		cell[i] = new Cell[8];
	for (int y = 0; y < 8; y++)
		for (int x = 0; x < 8; x++)
			cell[y][x] = obj.cell[y][x];
}
Map::~Map()
{
	for (int i = 0; i < 8; i++)
		delete[]cell[i];
	delete[]cell;
}
void Map::mark(int x, int y, int mode)
{
	cell[y][x].markCell(mode);
}
void Map::unmark(int x, int y)
{
	cell[y][x].unmarkCell();
}
void Map::unmarkAll()
{
	for (int y = 0; y < 8; y++)
		for (int x = 0; x < 8; x++)
			cell[y][x].unmarkCell();
}
int Map::getCellMark(int x, int y)
{
	return cell[y][x].mark;
}
Unit* Map::getCellUnit(int x, int y)
{
	return cell[y][x].unit;
}
void Map::setCellUnit(int x, int y, Unit *unit)
{
	cell[y][x].unit = unit;
}
sf::Sprite& Map::getBgSprite()
{
	return bg_sprite;
}
sf::Sprite& Map::getGridSprite()
{
	return grid_sprite;
}
sf::Sprite* Map::getCellSprite(int x, int y)
{
	return cell[y][x].sprite;
}
void Map::highlight(Unit *unit)
{
	std::vector<int[3]> v = unit->select();
	int x = unit->getGameXY().x;
	int y = unit->getGameXY().y;
	cell[y][x].markCell(1);
	for (int i = 0; i < v.size(); i++)
	{
		int actionType = v[i][0];
		int actionDir = v[i][1];
		int actionRange = v[i][2];
		if (actionDir == dir_all) // маркировка клеток для движения/стрельбы по территории
		{
			int width = (actionRange * 2 + 1);
			int _x = x - actionRange;
			int _y = y - actionRange;
			for (int j = 0; j < width; j++)
			{
				for (int k = 0; k < width; k++)
				{
					if ((_x + k <= 7) && (_x + k >= 0) && (_y + j <= 7) && (_y + j >= 0))
					{
						Cell* curCell = &cell[_y + j][_x + k];
						if ((actionType == hl_move) && (!curCell->unit || curCell->unit->isDead()))
							curCell->markCell(hl_move);
						if ((actionType == hl_attack) && (!curCell->unit || curCell->unit->isDead()))
							curCell->markCell(hl_range);
						if (unit->getType() == "king")
						{
							if ((actionType == hl_attack) && curCell->unit && !curCell->unit->isDead() && (curCell->unit->getSide() == unit->getSide()))
								curCell->markCell(hl_selected);
						}
						else if ((actionType == hl_attack) && curCell->unit && !curCell->unit->isDead() && (curCell->unit->getSide() != unit->getSide()))
							curCell->markCell(hl_attack);
					}
				}
			}
			continue;
		}
		// маркировка клеток для движения/стрельбы по направлению
		int dx = 0, dy = 0;
		switch (actionDir)
		{
		case dir_dnleft: dx = -1; dy = 1; break;
		case dir_down: dx = 0; dy = 1; break;
		case dir_dnright: dx = 1; dy = 1; break;
		case dir_left: dx = -1; dy = 0; break;
		case dir_right: dx = 1; dy = 0; break;
		case dir_upleft: dx = -1; dy = -1; break;
		case dir_up: dx = 0; dy = -1; break;
		case dir_upright: dx = 1; dy = -1; break;
		}
		for (int j = 1; j <= actionRange; j++)
		{
			if ((x + j * dx <= 7) && (x + j * dx >= 0) && (y + j * dy <= 7) && (y + j * dy >= 0))
			{
				Cell* curCell = &cell[y + j * dy][x + j * dx];
				if ((actionType == hl_attack) && (actionRange == 7))
				{
					if (curCell->unit && !curCell->unit->isDead() && (curCell->unit->getSide() != unit->getSide()))
					{
						curCell->markCell(hl_attack);
						break;
					}
					else
					{
						curCell->markCell(hl_range);
					}
				}
				else if (((actionType == hl_move) && (!curCell->unit || curCell->unit->isDead())) ||
					((actionType == hl_attack) && curCell->unit && !curCell->unit->isDead() && (curCell->unit->getSide() != unit->getSide())))
				{
					curCell->markCell(actionType);
				}
				else
					break;
			}
		}
	}
}