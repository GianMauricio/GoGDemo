#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "Tile.h"

using namespace sf;
using namespace std;

TILE::TILE(Vector2f newposition) {
	this->shape.setFillColor(Color::Transparent);
	this->shape.setOrigin({ this->shape.getGlobalBounds().width / 2, this->shape.getGlobalBounds().height / 2 });
	this->shape.setSize({ 100.f,100.f });
	this->shape.setPosition(newposition);
	this->bounds = this->shape.getGlobalBounds();
	this->hasPiece = false;
}

TILE::~TILE() {}

FloatRect TILE::getbounds()
{
	return this->shape.getGlobalBounds();
}

Vector2f TILE::getposition()
{
	return this->shape.getPosition();
}

RectangleShape TILE::getshape() {
	return this->shape;
}

void TILE::setPiece()
{
	hasPiece = true;
}

void TILE::removePiece()
{
	hasPiece = false;
}

bool TILE::isEmpty()
{
	return (!hasPiece);
}
