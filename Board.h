#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>

using namespace sf;
using namespace std;

class BOARD : private sf::NonCopyable {
public:
	BOARD(Vector2f newposition);
	~BOARD();
	Sprite getboard();

private:
	Texture texture;
	Sprite sprite;
};
