#include "Cell.h"

Cell::Cell()
{
	spriteXY.x = spriteXY.y = 0.0f;
	mark = 0;
	unit = nullptr;
	texture = nullptr;
	sprite = nullptr;
}

Cell::Cell(const Cell &obj)
{
	spriteXY.x = obj.spriteXY.x;
	spriteXY.y = obj.spriteXY.y;
	mark = obj.mark;
	unit = obj.unit;
	if (!obj.texture)
	{
		texture = nullptr;
		sprite = nullptr;
	}
	else
	{
		texture = new sf::Texture;
		*texture = *obj.texture;
		sprite = new sf::Sprite;
		sprite->setTexture(*texture);
		sprite->setPosition(spriteXY);
	}
}

Cell::~Cell()
{
	if (texture) delete texture;
	if (sprite) delete sprite;
}

Cell& Cell::operator=(const Cell &obj)
{
	if (this == &obj) return *this;
	if (texture) delete texture;
	if (sprite) delete sprite;
	spriteXY.x = obj.spriteXY.x;
	spriteXY.y = obj.spriteXY.y;
	mark = obj.mark;
	unit = obj.unit;
	if (!obj.texture)
	{
		texture = nullptr;
		sprite = nullptr;
	}
	else
	{
		texture = new sf::Texture;
		*texture = *obj.texture;
		sprite = new sf::Sprite;
		sprite->setTexture(*texture);
		sprite->setPosition(spriteXY);
	}
	return *this;
}

void Cell::markCell(int mode)
{
	if (mark) return;
	if ((mode < 1) || (mode > 4)) return;
	texture = new sf::Texture;
	texture->loadFromImage(image.img);
	sprite = new sf::Sprite;
	sprite->setTexture(*texture);
	sprite->setPosition(spriteXY);
	switch (mode)
	{
	case 1: sprite->setColor(sf::Color::Blue); mark = hl_selected;  break;	// выделенный юнит/поддерживающа€ маги€
	case 2: sprite->setColor(sf::Color::Green); mark = hl_move;  break;	// можно идти
	case 3: sprite->setColor(sf::Color::Red); mark = hl_attack;  break;		// можно атаковать/боева€ маги€
	case 4: sprite->setColor(sf::Color::Black); mark = hl_range;  break;	// зона дос€гаемости выстрела/заклинани€
	}
}

void Cell::unmarkCell()
{
	if (!mark) return;
	delete texture;
	delete sprite;
	texture = nullptr;
	sprite = nullptr;
	mark = 0;
}

myImage Cell::image;