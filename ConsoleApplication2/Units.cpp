#include "Units.h"

// class Unit
Unit::Unit(std::string n, std::string t, std::string s, int h, int d, sf::Vector2i xy, const sf::Image& img)
{
	name = n;
	type = t;
	side = s;
	hp = (h < 0) ? 0 : h;
	damage = d;
	selected = false;
	dead = false;
	animation = 0;
	gameXY = xy;
	spriteXY.x = gameXY.x * 80 + 80;
	spriteXY.y = gameXY.y * 55 + 25;
	direction = (side == "light") ? 1 : -1;
	curFrame = timer = 0.0f;
	texture.loadFromImage(img);
	sprite.setTexture(texture);
	if (side == "light")
		sprite.setTextureRect(sf::IntRect(0, 0, 80, 130));
	else
		sprite.setTextureRect(sf::IntRect(80, 0, -80, 130));
	sprite.setPosition(spriteXY);
}
Unit::Unit(const Unit &obj)
{
	name = obj.name;
	type = obj.type;
	side = obj.side;
	hp = obj.hp;
	damage = obj.damage;
	animation = obj.animation;
	selected = obj.selected;
	gameXY = obj.gameXY;
	spriteXY = obj.spriteXY;
	direction = obj.direction;
	curFrame = obj.curFrame;
	timer = obj.timer;
	texture = obj.texture;
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(0, 0, 80, 130));
	sprite.setPosition(spriteXY);
}
std::string Unit::getType()
{
	return type;
}
std::string Unit::getSide()
{
	return side;
}
bool Unit::isSelected()
{
	return selected;
}
bool Unit::isDead()
{
	return dead;
}
void Unit::setSelected(bool s)
{
	selected = s;
}
bool Unit::isBusy()
{
	return animation;
}
int Unit::getAnimation()
{
	return animation;
}
sf::Vector2i Unit::getGameXY()
{
	return gameXY;
}
sf::Sprite& Unit::getSprite()
{
	return sprite;
}
void Unit::move(sf::Vector2i dest)
{
	destination.x = dest.x * 80 + 80;
	destination.y = dest.y * 55 + 25;
	gameXY = dest;
	animation = an_moving;
	selected = false;
}
void Unit::attack(Unit *enemy)
{
	if (type == "queen")
		animation = an_atk_straight;
	else
	{
		if (gameXY.y > enemy->gameXY.y)
			animation = an_atk_up;
		else if (gameXY.y < enemy->gameXY.y)
			animation = an_atk_down;
		else
			animation = an_atk_straight;
	}
	std::cout << type << " (" << hp << ") attaks (" << damage << ") " << enemy->type << " (" << enemy->hp << ")" << std::endl;
	enemy->hit(damage);
	selected = false;
}
void Unit::hit(int dmg)
{
	animation = an_defence;
	std::cout << hp << " - " << dmg << " = " << hp - dmg << std::endl;
	hp -= dmg;
	if (hp <= 0)
	{
		animation = an_death;
	}
}
void Unit::update(float time)
{
	if (dead) return;
	if (animation == an_moving)
	{
		float temp = (destination.x - spriteXY.x) * (destination.x - spriteXY.x) - (destination.y - spriteXY.y) * (destination.y - spriteXY.y);
		if (temp < 0) temp *= -1;
		float distance = sqrt(temp);
		if (distance > 1)
		{
			spriteXY.x += 0.1 * time * (destination.x - spriteXY.x) / distance;
			spriteXY.y += 0.1 * time * (destination.y - spriteXY.y) / distance;
		}
		else
		{
			animation = 0;
			if (side == "light")
				sprite.setTextureRect(sf::IntRect(0, 0, 100, 130));
			else
				sprite.setTextureRect(sf::IntRect(80, 0, -80, 130));
			spriteXY = destination;
			destination.x = 0.0f;
			destination.y = 0.0f;
		}
//		if (animation)
//			spriteXY.x -= 40;
		sprite.setPosition(spriteXY);
//		if (animation)
//			spriteXY.x += 40;
	}
	else if (animation)
	{
		timer += time;
		if (timer > 1400)
		{
			if (animation == an_death)
			{
				dead = true;
				sprite.setTextureRect(sf::IntRect(100, 0, 100, 130));
			}
			else if (side == "light")
				sprite.setTextureRect(sf::IntRect(0, 0, 100, 130));
			else
				sprite.setTextureRect(sf::IntRect(80, 0, -80, 130));
			animation = 0;
			curFrame = timer = 0.0f;
		}
	}
}
void Unit::setPattern(int* mas, int _type, int _dir, int _range)
{
	*mas = _type;
	*(mas + 1) = _dir;
	*(mas + 2) = _range;
}
sf::Vector2f Unit::destination(0.0f, 0.0f);

// class Pawn
Pawn::Pawn(std::string name, std::string side, sf::Vector2i xy) :
	Unit(name, std::string("pawn"), side, 100, 50, xy, (side == "light") ? imgL.img : imgD.img) {}
void Pawn::update(float time)
{
	Unit::update(time);
	if (animation)
	{
		curFrame += 0.005f * time;
		int frames = (animation == an_moving) ? 9 : 7;
		if (curFrame > frames) curFrame -= frames;
		int x1 = 100 * int(curFrame) + ((side == "light") ? 0 : 100);
		int y1 = 130 * animation;
		int x2 = 100 * direction;
		int y2 = 130;
		sprite.setTextureRect(sf::IntRect(x1, y1, x2, y2));
	}
	/*
	switch (animation)
	{
	case an_moving: // анимация движения
	{
		if (curFrame > 9) curFrame -= 9;
		sprite.setTextureRect(sf::IntRect(100 * int(curFrame) + ((side == "light") ? 0 : 100), 130, 100 * direction, 130));
		break;
	}
	case an_atk_up: // анимация атаки вверх
	{
		if (curFrame > 7) curFrame -= 7;
		sprite.setTextureRect(sf::IntRect(100 * int(curFrame) + ((side == "light") ? 0 : 100), 260, 100 * direction, 130));
		break;
	}
	case an_atk_down: // анимация атаки вниз
	{
		if (curFrame > 7) curFrame -= 7;
		sprite.setTextureRect(sf::IntRect(100 * int(curFrame) + ((side == "light") ? 0 : 100), ((curFrame > 2) && (curFrame < 6)) ? 390 : 260, 100 * direction, 130));
		break;
	}
	case an_defence: // анимация защиты
	{
		if (curFrame > 7) curFrame -= 7;
		if (curFrame < 4)
			sprite.setTextureRect(sf::IntRect(200 + 100 * int(curFrame) + ((side == "light") ? 0 : 100), 0, 100 * direction, 130));
		else
			sprite.setTextureRect(sf::IntRect(500 - 100 * int(curFrame - 3) + ((side == "light") ? 0 : 100), 0, 100 * direction, 130));
		break;
	}
	case an_death: // анимация смерти
	{
		if (curFrame > 7) curFrame -= 7;
		if (curFrame >= 2)
			sprite.setTextureRect(sf::IntRect(100 * int(curFrame - 2) + ((side == "light") ? 0 : 100), 520, 100 * direction, 130));
		break;
	}
	}
	*/
}
std::vector<int[3]> Pawn::select()
{
	setSelected(true);
	std::vector<int[3]> result(5);	// паттерн возможных действий (тип действия, направление, количество клеток/дальность)
	setPattern(result[0], hl_move, (direction == 1) ? dir_right : dir_left, 2);	// движение вперед на 2 клетки
	setPattern(result[1], hl_move, dir_up, 1);	// движение вверх на 1 клетку
	setPattern(result[2], hl_move, dir_down, 1);	// движение вниз на 1 клетку
	setPattern(result[3], hl_attack, (direction == 1) ? dir_upright : dir_upleft, 1);	// атака вверх
	setPattern(result[4], hl_attack, (direction == 1) ? dir_dnright : dir_dnleft, 1);	// атака вниз
	return result;
}
myImage Pawn::imgL("images/light_pawn.png");
myImage Pawn::imgD("images/dark_pawn.png");

// class Knight
Knight::Knight(std::string name, std::string side, sf::Vector2i xy):
	Unit(name, std::string("knight"), side, 150, 100, xy, (side == "light") ? imgL.img : imgD.img) {}
void Knight::update(float time)
{
	Unit::update(time);
	
	switch (animation)
	{
	case an_moving: // анимация движения
	{
		curFrame += 0.005f * time;
		if (curFrame > 9) curFrame -= 9;
		sprite.setTextureRect(sf::IntRect(100 * int(curFrame) + ((side == "light") ? 0 : 100), 130, 100 * direction, 130));
		break;
	}
	case an_atk_up: // анимация атаки вверх
	case an_atk_down: // анимация атаки вниз
	{
		curFrame += 0.005f * time;
		if (curFrame > 7) curFrame -= 7;
		sprite.setTextureRect(sf::IntRect(100 * int(curFrame) + ((side == "light") ? 0 : 100), ((curFrame > 2) && (curFrame < 6)) ? 390 : 260, 100 * direction, 130));
		break;
	}
	case an_defence: // анимация защиты
	{
		curFrame += 0.005f * time;
		if (curFrame > 7) curFrame -= 7;
		int darkX = ((curFrame >= 3) && (side == "dark")) ? 20 : 0;
		if (curFrame < 4)
			sprite.setTextureRect(sf::IntRect(200 + 100 * int(curFrame) + ((side == "light") ? 0 : 100) + darkX, 0, 100 * direction + darkX, 130));
		else
			sprite.setTextureRect(sf::IntRect(500 - 100 * int(curFrame - 3) + ((side == "light") ? 0 : 100), 0, 100 * direction, 130));
		break;
	}
	case an_death: // анимация смерти
	{
		curFrame += 0.005f * time;
		if (curFrame > 7) curFrame -= 7;
		if (curFrame >= 2)
			sprite.setTextureRect(sf::IntRect(100 * int(curFrame - 2) + ((side == "light") ? 0 : 100), 520, 100 * direction, 130));
		break;
	}
	}
	
}
std::vector<int[3]> Knight::select()
{
	setSelected(true);
	std::vector<int[3]> result(5);	// паттерн возможных действий (тип действия, направление, количество клеток/дальность)
	setPattern(result[0], hl_move, dir_all, 2);	// движение в любую клетку на расстоянии до 2х клеток
	setPattern(result[1], hl_attack, dir_up, 1);	// атака вверх
	setPattern(result[2], hl_attack, dir_down, 1);	// атака вниз
	setPattern(result[3], hl_attack, dir_right, 1);	// атака вперед
	setPattern(result[4], hl_attack, dir_left, 1);	// атака назад
	return result;
}
myImage Knight::imgL("images/light_knight.png");
myImage Knight::imgD("images/dark_knight.png");

// class Rook
Rook::Rook(std::string name, std::string side, sf::Vector2i xy):
	Unit(name, std::string("rook"), side, 200, 100, xy, (side == "light") ? imgL.img : imgD.img) {}
void Rook::update(float time)
{
	Unit::update(time);
	// animation
}
std::vector<int[3]> Rook::select()
{
	setSelected(true);
	std::vector<int[3]> result(3);	// паттерн возможных действий (тип действия, направление, количество клеток/дальность)
	setPattern(result[0], hl_move, dir_up, 3);	// движение вверх на 3 клетки
	setPattern(result[1], hl_move, dir_down, 3);	// движение вниз на 3 клетки
	setPattern(result[2], hl_attack, (direction == 1) ? dir_right : dir_left, 7);	// атака вперед через всю карту
	return result;
}
myImage Rook::imgL("images/light_rook.png");
myImage Rook::imgD("images/dark_rook.png");

// class Bishop
Bishop::Bishop(std::string name, std::string side, sf::Vector2i xy) :
	Unit(name, std::string("bishop"), side, 50, 50, xy, (side == "light") ? imgL.img : imgD.img) {}
void Bishop::update(float time)
{
	Unit::update(time);
	// animation
}
std::vector<int[3]> Bishop::select()
{
	setSelected(true);
	std::vector<int[3]> result(2);	// паттерн возможных действий (тип действия, направление, количество клеток/дальность)
	setPattern(result[0], hl_move, dir_all, 1);	// движение в любую сторону на 1 клетку
	setPattern(result[1], hl_attack, dir_all, 3);	// атака в любую клетку на расстоянии до 3х клеток
	return result;
}
myImage Bishop::imgL("images/light_bishop.png");
myImage Bishop::imgD("images/dark_bishop.png");

// class King
King::King(std::string name, std::string side, sf::Vector2i xy):
	Unit(name, std::string("king"), side, 300, -50, xy, (side == "light") ? imgL.img : imgD.img) {}
void King::update(float time)
{
	Unit::update(time);
	// animation
}
std::vector<int[3]> King::select()
{
	setSelected(true);
	std::vector<int[3]> result(2);	// паттерн возможных действий (тип действия, направление, количество клеток/дальность)
	setPattern(result[0], hl_move, dir_all, 1);	// движение в любую сторону на 1 клетку
	setPattern(result[1], hl_attack, dir_all, 7);	// лечение в любую клетку на карте
	return result;
}
myImage King::imgL("images/light_king.png");
myImage King::imgD("images/dark_king.png");

// class Queen
Queen::Queen(std::string name, std::string side, sf::Vector2i xy) :
	Unit(name, std::string("queen"), side, 200, 50, xy, (side == "light") ? imgL.img : imgD.img) {}
void Queen::update(float time)
{
	Unit::update(time);
	// animation
}
std::vector<int[3]> Queen::select()
{
	setSelected(true);
	std::vector<int[3]> result(2);
	setPattern(result[0], hl_move, dir_all, 1);
	setPattern(result[1], hl_attack, dir_all, 3);
	return result;
}
myImage Queen::imgL("images/light_queen.png");
myImage Queen::imgD("images/dark_queen.png");
