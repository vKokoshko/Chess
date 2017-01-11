#include "myImage.h"

myImage::myImage()
{
	img.create(80, 55, sf::Color(255, 255, 255, 50));
}

myImage::myImage(char *file)
{
	img.loadFromFile(file);
}

myImage::~myImage()
{
}
