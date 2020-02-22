#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "Marker.h"

using namespace sf;
using namespace std;

//Constructor (based on position(?))
MARKER::MARKER(Vector2f newposition) {
	this->texture.loadFromFile("Texture/Marker.png");
	this->sprite.setTexture(this->texture);
	this->sprite.setPosition(newposition);
	this->bounds = this->sprite.getGlobalBounds();
}

MARKER::~MARKER() {}

//Return current global position
FloatRect MARKER::getbounds(){
	return this->sprite.getGlobalBounds();
}

//Return current position
Vector2f MARKER::getposition(){
	return this->sprite.getPosition();
}

//Set new position
void MARKER::setposition(Vector2f newposition){
	this->sprite.setPosition(newposition);
}

//For drawing purposes
Sprite MARKER::getsprite() {
	return this->sprite;
}
