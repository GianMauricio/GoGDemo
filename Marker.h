#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>

using namespace sf;
using namespace std;

class MARKER : private sf::NonCopyable {
public:
	MARKER(Vector2f newposition);
	~MARKER();
	FloatRect getbounds();
	Vector2f getposition();
	void setposition(Vector2f newposition);
	Sprite getsprite();

private:
	Texture texture;
	Sprite sprite;
	FloatRect bounds;
	Vector2f position;
};
