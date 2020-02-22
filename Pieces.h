#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;


class PIECE : private sf::NonCopyable{
public:
	PIECE(string filename1, string filename, Vector2f position, int newvalue, string newname);
	~PIECE();
	void place();
	Sprite getsprite();
	FloatRect getbounds();
	void makeenemy(Vector2f position);
	int getvalue();
	string getname();
	void setposition(Vector2f newposition);
	Vector2f getposition();
	bool checkStatus();
	void setInivisble();
	void setVisible();

private:
	
	Sprite sprite;
	Texture texture, basetexture;
	int value;
	bool isenemy;
	bool isselected;
	bool isPlaced;
	FloatRect bounds;

	string name;

	Text text;
	Font font;

	PIECE* piece;
	vector<PIECE*> pieces;
	
};


//Board Notes:
//330px width start and 100px away
// 35px height start and 100px away