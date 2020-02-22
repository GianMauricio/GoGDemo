#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "Board.h"

using namespace sf;
using namespace std;

//Create board based on position (preset)
BOARD::BOARD(Vector2f newposition) {
	this->texture.loadFromFile("Texture/BOARD2.png");
	this->sprite.setTexture(this->texture);
	this->sprite.setOrigin({ this->sprite.getGlobalBounds().width / 2, this->sprite.getGlobalBounds().height / 2 });
	this->sprite.setPosition(newposition);
}

BOARD::~BOARD() {}

Sprite BOARD::getboard() {
	return this->sprite;
}
