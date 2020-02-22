#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>

using namespace sf;
using namespace std;

class TILE : private sf::NonCopyable {
public:
	TILE(Vector2f newposition);
	~TILE();
	FloatRect getbounds();
	Vector2f getposition();
	RectangleShape getshape();
	void setPiece();
	void removePiece();
	bool isEmpty();
private:
	RectangleShape shape;
	FloatRect bounds;
	Vector2f position;
	Vector2f boardPos;
	bool hasPiece;
};
