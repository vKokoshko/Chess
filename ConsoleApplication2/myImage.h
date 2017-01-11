#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class myImage
{
	sf::Image img;
public:
	myImage();
	~myImage();
	myImage(char *file);
	friend class Cell;
	friend class Pawn;
	friend class Rook;
	friend class Knight;
	friend class Bishop;
	friend class King;
	friend class Queen;
};

