#pragma once
#include "myImage.h"
#include <vector>
#include <string>

enum highlights {hl_selected = 1, hl_move, hl_attack, hl_range};
enum animations {an_moving = 1, an_atk_straight, an_atk_up, an_atk_down, an_hit, an_defence, an_death, an_spell};
enum directions {dir_dnleft = 1, dir_down, dir_dnright, dir_left, dir_all, dir_right, dir_upleft, dir_up, dir_upright};

class Unit
{
protected:
	std::string name;
	std::string type;
	std::string side;
	int hp;
	int damage;
	bool selected, dead;
	int animation; // 1 движение, 2 атака прямо (3,4 - верх,низ), 5 получение урона, 6 защита, 7 смерть
	sf::Vector2i gameXY;
	sf::Vector2f spriteXY;
	int direction; // 1 вправо, -1 влево
	float curFrame, timer;
	sf::Texture texture;
	sf::Sprite sprite;
	static sf::Vector2f destination;
public:
	Unit(std::string, std::string, std::string, int, int, sf::Vector2i, const sf::Image&);
	Unit(const Unit&);
	std::string getType();
	std::string getSide();
	bool isSelected();
	bool isDead();
	void setSelected(bool s);
	bool isBusy();
	int getAnimation();
	sf::Vector2i getGameXY();
	sf::Sprite& getSprite();
	void move(sf::Vector2i dest);
	void attack(Unit*);
	void hit(int);
	virtual void update(float);
	virtual std::vector<int[3]> select() = 0;
protected:
	void setPattern(int*, int, int, int);
};

class Pawn : public Unit
{
	static myImage imgL;
	static myImage imgD;
public:
	Pawn(std::string, std::string, sf::Vector2i);
	void update(float);
	std::vector<int[3]> select();
};

class Knight : public Unit
{
	static myImage imgL;
	static myImage imgD;
public:
	Knight(std::string, std::string, sf::Vector2i);
	void update(float);
	std::vector<int[3]> select();
};

class Rook : public Unit
{
	static myImage imgL;
	static myImage imgD;
public:
	Rook(std::string, std::string, sf::Vector2i);
	void update(float);
	std::vector<int[3]> select();
};

class Bishop : public Unit
{
	static myImage imgL;
	static myImage imgD;
public:
	Bishop(std::string, std::string, sf::Vector2i);
	void update(float);
	std::vector<int[3]> select();
};

class King : public Unit
{
	static myImage imgL;
	static myImage imgD;
public:
	King(std::string, std::string, sf::Vector2i);
	void update(float);
	std::vector<int[3]> select();
};

class Queen : public Unit
{
	static myImage imgL;
	static myImage imgD;
public:
	Queen(std::string, std::string, sf::Vector2i);
	void update(float);
	std::vector<int[3]> select();
};