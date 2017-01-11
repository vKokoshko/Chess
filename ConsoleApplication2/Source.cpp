#define _CRT_SECURE_NO_WARNINGS
#include "Map.h"
#include "Units.h"
using namespace std;
using namespace sf;

/*
class Engine
{
	Vector2i cursorPos;
	Vector2i tileXY;
	Unit **currentUnit;
	Map *map;
	vector<Unit*> *units;
	RenderWindow *window;
public:
	void draw()
	{
		window->draw(map->getBgSprite());
		window->draw(map->getGridSprite());
		for (int y = 0; y < 8; y++)
		{
			for (int x = 0; x < 8; x++)
			{
				if (map->getCellSprite(x, y))
					window->draw(*map->getCellSprite(x, y));
				if (map->getCellUnit(x, y))
					window->draw(map->getCellUnit(x, y)->getSprite());
			}
			if ((*currentUnit) && ((*currentUnit)->isBusy() == true))
				window->draw((*currentUnit)->getSprite());
		}
	}
};
*/
// переписать начисто


class Player
{
	string side;
	string name;
public:
	Player(){}
	Player(char *s, char *n)
	{
		side = s;
			name = n;
	}
	string getName()
	{
		return name;
	}
	string getSide()
	{
		return side;
	}
};


// запихнуть в engine
void draw(RenderWindow &obj, Unit *unit, Map &map)
{
	obj.draw(map.getBgSprite());
	obj.draw(map.getGridSprite());
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			if (map.getCellSprite(x, y))
				obj.draw(*map.getCellSprite(x, y));
			if (map.getCellUnit(x, y))
				obj.draw(map.getCellUnit(x, y)->getSprite());
		}
		if ((unit) && (unit->getAnimation() == 1) && (y == (((int)unit->getSprite().getPosition().y - 25) / 55)))
			obj.draw(unit->getSprite());
	}
}

class Figures
{
	vector<Unit*> units;
public:
	Figures()
	{
		units.resize(32);
		for (int i = 0; i < 8; i++)
		{
			string tmp = "pawn" + (i + 1);
			units[i] = new Pawn(tmp, string("light"), Vector2i(1, i));
			units[i+8] = new Pawn(tmp, string("dark"), Vector2i(6, i));
		}
		units[16] = new Knight(string("knight1"), string("light"), Vector2i(0, 1));
		units[17] = new Knight(string("knight2"), string("light"), Vector2i(0, 6));
		units[18] = new Knight(string("knight1"), string("dark"), Vector2i(7, 1));
		units[19] = new Knight(string("knight2"), string("dark"), Vector2i(7, 6));
		units[20] = new Rook(string("rook1"), string("light"), Vector2i(0, 0));
		units[21] = new Rook(string("rook2"), string("light"), Vector2i(0, 7));
		units[22] = new Rook(string("rook1"), string("dark"), Vector2i(7, 0));
		units[23] = new Rook(string("rook2"), string("dark"), Vector2i(7, 7));
		units[24] = new Bishop(string("bishop1"), string("light"), Vector2i(0, 2));
		units[25] = new Bishop(string("bishop2"), string("light"), Vector2i(0, 5));
		units[26] = new Bishop(string("bishop1"), string("dark"), Vector2i(7, 2));
		units[27] = new Bishop(string("bishop2"), string("dark"), Vector2i(7, 5));
		units[28] = new King(string("king"), string("light"), Vector2i(0, 3));
		units[29] = new King(string("king"), string("dark"), Vector2i(7, 3));
		units[30] = new Queen(string("queen"), string("light"), Vector2i(0, 4));
		units[31] = new Queen(string("queen"), string("dark"), Vector2i(7, 4));
	}
	~Figures()
	{
		for (int i = 0; i < units.size(); i++)
			delete units[i];
	}
	Unit* getUnit(int i)
	{
		return units[i];
	}
	int getSize()
	{
		return units.size();
	}
};


int main()
{
	RenderWindow window(VideoMode(800, 556), "Might & Magic Chess v.0.105");
	Clock clock;
	Event event;
	float time;

	// возможно запихнуть в класс engine какой-нибудь вместе с глобальными методами
	Vector2i cursorPos;
	Vector2i tileXY;
	Unit *currentUnit = nullptr;
	
	Map gamedesk;

	Figures figures;
	// метод расстановки юнитов в карте 
	for (int i = 0; i < figures.getSize(); i++)
		gamedesk.setCellUnit(figures.getUnit(i)->getGameXY().x, figures.getUnit(i)->getGameXY().y, figures.getUnit(i));
	// queue для игроков (самому написать кольцевую простую)
	// возможно сделать список "мертвых юнитов", что бы сохранять положение тела и отображать после того как на него "наступили"
	
	Player p1("light", "Player 1");
	Player p2("dark", "Player 2");
	Player *currentPlayer = &p1;
	cout << currentPlayer->getName() << " turn" << endl;

	while (window.isOpen())
	{
		time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 800;
		cursorPos = Mouse::getPosition(window);

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::MouseButtonPressed)
				if (event.key.code == Mouse::Left)
				{
					if (currentUnit && currentUnit->isBusy()) {}
					else if ((cursorPos.x >= 80) && (cursorPos.x < 720) && (cursorPos.y >= 100) && (cursorPos.y < 540))
					{
						tileXY.x = (cursorPos.x - 80) / 80;
						tileXY.y = (cursorPos.y - 100) / 55;
						Unit* cellUnit = gamedesk.getCellUnit(tileXY.x, tileXY.y);
						int cellMark = gamedesk.getCellMark(tileXY.x, tileXY.y);
						if (!currentUnit)
						{
							if (cellUnit && !cellUnit->isDead() && cellUnit->getSide() == currentPlayer->getSide())
							{
								currentUnit = cellUnit;
								gamedesk.highlight(currentUnit);
							}
						}
						else
						{
							if (cellMark)
							{
								if (cellMark == hl_move)
								{
									gamedesk.setCellUnit(currentUnit->getGameXY().x, currentUnit->getGameXY().y, nullptr);
									currentUnit->move(tileXY);
									gamedesk.unmarkAll();
								}
								if ((cellMark == hl_attack) || ((cellMark == hl_selected) && (cellUnit != currentUnit)))
								{
									currentUnit->attack(cellUnit);
									if (currentUnit->getType() == "queen")
									{
										if (gamedesk.getCellUnit(tileXY.x - 1, tileXY.y))
											currentUnit->attack(gamedesk.getCellUnit(tileXY.x - 1, tileXY.y));
										if (gamedesk.getCellUnit(tileXY.x + 1, tileXY.y))
											currentUnit->attack(gamedesk.getCellUnit(tileXY.x + 1, tileXY.y));
										if (gamedesk.getCellUnit(tileXY.x, tileXY.y - 1))
											currentUnit->attack(gamedesk.getCellUnit(tileXY.x, tileXY.y - 1));
										if (gamedesk.getCellUnit(tileXY.x, tileXY.y + 1))
											currentUnit->attack(gamedesk.getCellUnit(tileXY.x, tileXY.y + 1));
									}
									gamedesk.unmarkAll();
								}
								if (((cellMark == hl_selected) && (cellUnit == currentUnit)) || (cellMark == hl_range)) {}
								else
								{
									currentPlayer = (currentPlayer == &p1) ? &p2 : &p1;
									cout << currentPlayer->getName() << " turn" << endl;
								}
							}
						}
					}
				}
			if (event.type == Event::MouseButtonPressed)
				if (event.key.code == Mouse::Right)
					if (currentUnit && !currentUnit->isBusy())
					{
						gamedesk.unmarkAll();
						currentUnit->setSelected(false);
					}
		}

		// реализовать в update попробовать
		if (currentUnit && !currentUnit->isSelected() && !currentUnit->isBusy())
		{
			gamedesk.setCellUnit(currentUnit->getGameXY().x, currentUnit->getGameXY().y, currentUnit);
			currentUnit = nullptr;
		}
		
		// глобальный метод, который будет обновлять все что обновляется (вызывать методы, можно исп for each)
		for (int i = 0; i < figures.getSize(); i++)
			figures.getUnit(i)->update(time);

		window.clear();
		draw(window, currentUnit, gamedesk);
		window.display();
	}
}