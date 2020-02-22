#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "Pieces.h"

using namespace sf;
using namespace std;

//Constructor
PIECE::PIECE(string filename, string filename1, Vector2f position, int newvalue, string newname) {
	this->texture.loadFromFile(filename1);
	this->basetexture.loadFromFile(filename);
	this->sprite.setTexture(this->texture);
	this->sprite.setScale(0.2, 0.2);
	this->sprite.setPosition(position);
	this->value = newvalue;
	this->name = newname;
	this->isPlaced = false;
}

PIECE::~PIECE() {}

//For drawing
Sprite PIECE::getsprite() {
	return this->sprite;
}

//Get global bounds
FloatRect PIECE::getbounds() {
	return this->sprite.getGlobalBounds();
}

//Convert piece to enemy faction
void PIECE::makeenemy(Vector2f newposition) {
	this->texture.loadFromFile("Texture/Enemy.png");
	this->sprite.setTexture(this->texture);
	this->sprite.setScale(0.2, 0.2);
	this->sprite.setPosition(newposition);
	this->value *= -1;
}

//Return rank and allegiance
int PIECE::getvalue() {
	return this->value;
}

//Return Rank as title
string PIECE::getname() {
	return this->name;
}

//Set new position
void PIECE::setposition(Vector2f newposition) {
	this->sprite.setPosition(newposition);
}

//Return position
Vector2f PIECE::getposition() {
	return this->sprite.getPosition();
}

void PIECE::place() {
	isPlaced = true;
}

bool PIECE::checkStatus() {
	return isPlaced;
}

void PIECE::setInivisble()
{
	sprite.setTexture(basetexture);
}

void PIECE::setVisible()
{
	sprite.setTexture(texture);
}




