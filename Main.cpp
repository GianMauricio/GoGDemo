#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Board.h"
#include "Tile.h"
#include "Pieces.h"
#include "Marker.h"

using namespace sf;
using namespace std;

enum RANKS {
	FLAG = 0, PRIV = 1, SARGE = 2, LT2 = 3, LT1 = 4, CAPT = 5, MAJ = 6, LTCOL = 7, COL = 8,
	GEN1 = 9, GEN2 = 10, GEN3 = 11, GEN4 = 12, GEN5 = 13,
	SPY = 14};

enum class STATES {
	START = 0, P1SET = 1, P2SET = 3, GAME = 4, END = 5
};

void processEvents(Event* event, MARKER* marker, vector<TILE*> tiles, vector<PIECE*> Allies, vector<PIECE*> Enemies, 
	STATES* currState, Vector2f mousePos, bool* select, int* selectindex, int* turn, int* deadAllies, int* deadEnemies) {

	//Board Setup THIS IS BEFORE START OF GAME
	if (event->type == sf::Event::KeyReleased && event->key.code == Keyboard::Space)/*Force skip a state*/ {
		switch (*currState) {
		case::STATES::START:
			*currState = STATES::P1SET;
			break;
		case::STATES::P1SET:
			*currState = STATES::P2SET;
			break;
		case::STATES::P2SET:
			*currState = STATES::GAME;
			break;
		case::STATES::GAME:
			*currState = STATES::END;
			break;
		}
	}

	if (*currState == STATES::START) {

	}

	//Player 1 setup
	if (*currState == STATES::P1SET) {
		for(PIECE* x : Enemies){
			x->setInivisble();
		}

		if (event->type == sf::Event::MouseButtonReleased && event->key.code == Mouse::Left && (*select == false)) {
			//Select the piece that was clicked
			for (int i = 0; i < Allies.size(); i++) {
				if (Allies[i]->getbounds().contains(mousePos)) {
					cout << Allies[i]->getname() << endl;
					*selectindex = i;
					marker->setposition(Allies[*selectindex]->getposition());
					*select = true;
					Allies[*selectindex]->place();
					cout << *selectindex << endl;
					break;
				}
			}
		}

		if (event->type == sf::Event::MouseButtonReleased && event->key.code == Mouse::Left && (*select == true)) {
			//Attempt to place piece
			bool placed = false;
			for (int i = 0; i < tiles.size(); i++) {
				if (tiles[i]->getbounds().contains(mousePos) && i >= 42) {
					if (tiles[i]->isEmpty()) {
						Allies[*selectindex]->setposition(tiles[i]->getposition());
						tiles[i]->setPiece();
						*select = false;
						placed = true;
						break;
					}
				}
			}

			//Check if player 1 is done setting up
			bool p1AllPlaced = true;
			for (PIECE* x : Allies) {
				if (!x->checkStatus()) { p1AllPlaced = false; }
			}

			if (p1AllPlaced && *currState == STATES::P1SET && placed) {
				*currState = STATES::P2SET;
			}
		}
	}

	//Player 2 Setup
	if (*currState == STATES::P2SET) {
		for (PIECE* x : Allies) {
			x->setInivisble();
		}

		for (PIECE* x : Enemies) {
			x->setVisible();
		}
		if (event->type == sf::Event::MouseButtonReleased && event->key.code == Mouse::Left && (*select == false)) {
			//Select the piece that was clicked
			for (int i = 0; i < Enemies.size(); i++) {
				if (Enemies[i]->getbounds().contains(mousePos)) {
					*selectindex = i;
					marker->setposition(Enemies[*selectindex]->getposition());
					*select = true;
					Enemies[*selectindex]->place();
					break;
				}
			}
		}

		if (event->type == sf::Event::MouseButtonReleased && event->key.code == Mouse::Left && (*select == true)) {
			//Attempt to place piece
			bool placed = false;
			for (int i = 0; i < tiles.size(); i++) {
				if (tiles[i]->getbounds().contains(mousePos) && i >= 42) {
					if (tiles[i]->isEmpty()) {
						Enemies[*selectindex]->setposition(tiles[i]->getposition());
						*select = false;
						placed = true;
						tiles[i]->setPiece();
						break;
					}
				}
			}

			//Check if player 2 is done setting up
			bool p2AllPlaced = true;
			for (PIECE* x : Enemies) {
				if (!x->checkStatus()) { p2AllPlaced = false; }
			}

			if (p2AllPlaced && *currState == STATES::P2SET && placed) {
				*currState = STATES::GAME;
			}
		}
	}

	//Game proper
	if (*currState == STATES::GAME) {
		if (*turn == 0)/*Player 1 turn*/ {
			if (event->type == sf::Event::MouseButtonReleased && event->key.code == Mouse::Left && (*select == false)) {
				//Select the piece that was clicked
				for (int i = 0; i < Allies.size(); i++) {
					if (Allies[i]->getbounds().contains(mousePos)) {
						cout << Allies[i]->getname() << endl;
						*selectindex = i;
						marker->setposition(Allies[*selectindex]->getposition());
						*select = true;
						break;
					}
				}
			}

			else if (event->type == sf::Event::MouseButtonReleased && event->key.code == Mouse::Left && (*select == true)) {
				//Attempt to place piece
				for (int i = 0; i < tiles.size(); i++) {
					if (tiles[i]->getbounds().contains(mousePos) && i >= 42) {
						Vector2f diffVec = Vector2f(abs(Allies[*selectindex]->getposition().x) - abs(mousePos.x),
							abs(Allies[*selectindex]->getposition().y) - abs(mousePos.y));

						if (tiles[i]->isEmpty()) {
							for (int i = 0; i < tiles.size(); i++) {
								if (tiles[i]->getbounds().contains(Allies[*selectindex]->getposition()) && i >= 42) {
									tiles[i]->removePiece();
								}
							}

							Allies[*selectindex]->setposition(tiles[i]->getposition());
							tiles[i]->setPiece();
							*select = false;
							*turn = *turn + 1;
							break;
						}

						else {
							//if piece was placed onto another piece of the same team
							for (PIECE* curr : Allies) {
								if (curr->getbounds().contains(mousePos)) {
									cout << "Illegal move" << endl;
								}
							}

							for (PIECE* curr : Enemies) {
								cout << "Attempt to kill" << endl;
								if (curr->getbounds().contains(mousePos)) {
									//Attempt to eat
									if (Allies[*selectindex]->getvalue() == 14 && Enemies[i]->getvalue() == 1)/*Spy gets eaten*/ {
										cout << "Lost spy" << endl;
										Allies[*selectindex]->setposition(tiles[*deadAllies]->getposition());
										(*deadAllies)++;
									}

									else if (Allies[*selectindex]->getvalue() == 1 && Enemies[i]->getvalue() == 14) /*Private eats spy*/ {
										cout << "Killed Spy" << endl;
										Enemies[*selectindex]->setInivisble();
										Enemies[*selectindex]->setposition(tiles[*deadEnemies]->getposition());
										(*deadEnemies)++;

										Allies[*selectindex]->setposition(tiles[i]->getposition());
										tiles[i]->setPiece();
										*select = false;
										*turn = *turn + 1;
									}

									else if (Allies[*selectindex]->getvalue() > Enemies[i]->getvalue())/*Normal eat*/ {
										cout << "Kill" << endl;
										Enemies[i]->setInivisble();
										Enemies[*selectindex]->setposition(tiles[*deadEnemies]->getposition());
										(*deadEnemies)++;

										Allies[*selectindex]->setposition(tiles[i]->getposition());
										tiles[i]->setPiece();
										*select = false;
										*turn = *turn + 1;
									}

									else if (Allies[*selectindex]->getvalue() < Enemies[i]->getvalue())/*Normal get eaten*/ {
										cout << "Death" << endl;
										Allies[*selectindex]->setposition(tiles[*deadAllies]->getposition());
										(*deadAllies)++;
									}
								}
							}
						}
					}
				}
			}
		}

		else if (*turn == 1)/*Player 2 turn*/ {
			if (event->type == sf::Event::MouseButtonReleased && event->key.code == Mouse::Left && (*select == false)) {
				//Select the piece that was clicked
				for (int i = 0; i < Enemies.size(); i++) {
					if (Enemies[i]->getbounds().contains(mousePos)) {
						*selectindex = i;
						marker->setposition(Enemies[*selectindex]->getposition());
						*select = true;
						break;
					}
				}
			}

			else if (event->type == sf::Event::MouseButtonReleased && event->key.code == Mouse::Left && (*select == true)) {
				//Attempt to place piece
				for (int i = 0; i < tiles.size(); i++) {
					if (tiles[i]->getbounds().contains(mousePos) && i >= 42) {
						Vector2f diffVec = Vector2f(abs(Enemies[*selectindex]->getposition().x) - abs(mousePos.x),
							abs(Enemies[*selectindex]->getposition().y) - abs(mousePos.y));

						if (tiles[i]->isEmpty()) {

							for (int i = 0; i < tiles.size(); i++) {
								if (tiles[i]->getbounds().contains(Enemies[*selectindex]->getposition()) && i >= 42) {
									tiles[i]->removePiece();
								}
							}

							Enemies[*selectindex]->setposition(tiles[i]->getposition());
							*select = false;
							*turn = *turn - 1;
							break;
						}

						else {
							//if piece was placed onto another piece of the same team
							for (PIECE* curr : Enemies) {
								if (curr->getbounds().contains(mousePos)) {
									cout << "Illegal move" << endl;
								}
							}

							for (PIECE* curr : Allies) {
								if (curr->getbounds().contains(mousePos)) {
									cout << "Attempt to kill" << endl; 
									//Attempt to eat
									if (Enemies[*selectindex]->getvalue() == 14 && Allies[i]->getvalue() == 1)/*Spy gets eaten*/  {
										cout << "Lost spy" << endl;
										Enemies[*selectindex]->setInivisble();
										Enemies[*selectindex]->setposition(tiles[*deadEnemies]->getposition());
										(*deadEnemies)++;
									}

									else if (Enemies[*selectindex]->getvalue() == 1 && Allies[i]->getvalue() == 14) /*Private eats spy*/ {
										cout << "Killed spy" << endl;
										Allies[*selectindex]->setInivisble();
										Allies[*selectindex]->setposition(tiles[*deadAllies]->getposition());
										(*deadAllies)++;
									}

									else if (Enemies[*selectindex]->getvalue() > Allies[i]->getvalue())/*Normal eat*/ {
										cout << "Kill" << endl;
										Allies[i]->setInivisble();
										Allies[*selectindex]->setposition(tiles[*deadAllies]->getposition());
										(*deadAllies)++;
									}

									else if (Enemies[*selectindex]->getvalue() < Allies[i]->getvalue())/*Normal get eaten*/ {
										cout << "Death" << endl;
										Enemies[i]->setInivisble();
										Enemies[*selectindex]->setposition(tiles[*deadEnemies]->getposition());
										(*deadEnemies)++;
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void update(Window* window, MARKER* marker, vector<TILE*> tiles, vector<PIECE*> Allies, vector<PIECE*> Enemies) {

}

int main()
{
	sf::RenderWindow window(sf::VideoMode(1600, 900), "Games of The Generals!");
	BOARD board({ 800,450 });
	Mouse mouse;
	Vector2f mouseposition;

	MARKER marker({ -10,-10 });
	TILE* tile;
	vector <TILE*> tiles;
	PIECE* piece;
	vector <PIECE*> vAlliedPieces;/*Allied Pieces*/
	vector <PIECE*> vEnemyPieces;/*Enemy Pieces*/
	STATES currState = STATES::START;

	int tilecounter = 0;
	float x = 10;
	float y = 35;
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 3; j++) {
			tile = new TILE({ x,y });
			tiles.push_back(tile);
			x += 105;
			tilecounter++;
		}
		x = 10;
		y += 105;
	}
	x = 1285;
	y = 35;

	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 3; j++) {
			tile = new TILE({ x,y });
			tiles.push_back(tile);
			x += 105;
			tilecounter++;
		}
		x = 1285;
		y += 105;
	}
	x = 330;
	y = 35;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 9; j++) {
			tile = new TILE({ x,y });
			tiles.push_back(tile);
			x += 105;
		}
		x = 330;
		y += 105;
	}

	window.setKeyRepeatEnabled(false);
	bool select = false;
	int selectindex = 0;
	int turn = 0;
	int deadAllies = 0;
	int deadEnemies = 20;

	//Initialize all ally pieces (Player1)
	for (int i = 0; i < 21; i++) {
		if (i == 0) {
			//Flag	
			piece = new PIECE("Texture/Enemy.png", "Texture/Flag.png", tiles[i]->getposition(), RANKS::FLAG, "Flag");
			vAlliedPieces.push_back(piece);
		}

		else if (i > 0 && i < 7) {
			//Private
			piece = new PIECE("Texture/Enemy.png", "Texture/Private.png", tiles[i]->getposition(), RANKS::PRIV, "Private");
			vAlliedPieces.push_back(piece);
		}
		else if (i == 7) {
			//Sergeant
			piece = new PIECE("Texture/Enemy.png", "Texture/Serg.png", tiles[i]->getposition(), RANKS::SARGE, "Sergeant");
			vAlliedPieces.push_back(piece);
		}
		else if (i == 8) {
			//2nd Lieutenant
			piece = new PIECE("Texture/Enemy.png", "Texture/Lieut2.png", tiles[i]->getposition(), RANKS::LT2, "2nd Lieutenant");
			vAlliedPieces.push_back(piece);
		}
		else if (i == 9) {
			//1st Lieutenant
			piece = new PIECE("Texture/Enemy.png", "Texture/Lieut1.png", tiles[i]->getposition(), RANKS::LT1, "1st Lieutenant");
			vAlliedPieces.push_back(piece);
		}
		else if (i == 10) {
			//Captain
			piece = new PIECE("Texture/Enemy.png", "Texture/Capt.png", tiles[i]->getposition(), RANKS::CAPT, "Captain");
			vAlliedPieces.push_back(piece);
		}
		else if (i == 11) {
			//Major
			piece = new PIECE("Texture/Enemy.png", "Texture/Major.png", tiles[i]->getposition(), RANKS::MAJ, "Major");
			vAlliedPieces.push_back(piece);
		}
		else if (i == 12) {
			//Lieutenant Colonel
			piece = new PIECE("Texture/Enemy.png", "Texture/Lieutcol.png", tiles[i]->getposition(), RANKS::LTCOL, "Lieutenant Colonel");
			vAlliedPieces.push_back(piece);
		}
		else if (i == 13) {
			//Colonel
			piece = new PIECE("Texture/Enemy.png", "Texture/Col.png", tiles[i]->getposition(), RANKS::COL, "Colonel");
			vAlliedPieces.push_back(piece);
		}

		else if (i == 14) {
			//Gen 1
			piece = new PIECE("Texture/Enemy.png", "Texture/Gen1.png", tiles[i]->getposition(), RANKS::GEN1, "1 Star General");
			vAlliedPieces.push_back(piece);
		}
		else if (i == 15) {
			//Gen 2
			piece = new PIECE("Texture/Enemy.png", "Texture/Gen2.png", tiles[i]->getposition(), RANKS::GEN2, "2 Star General");
			vAlliedPieces.push_back(piece);
		}
		else if (i == 16) {
			//Gen 3
			piece = new PIECE("Texture/Enemy.png", "Texture/Gen3.png", tiles[i]->getposition(), RANKS::GEN3, "3 Star General");
			vAlliedPieces.push_back(piece);
		}
		else if (i == 17) {
			//Gen 4
			piece = new PIECE("Texture/Enemy.png", "Texture/Gen4.png", tiles[i]->getposition(), RANKS::GEN4, "4 Star General");
			vAlliedPieces.push_back(piece);
		}
		else if (i == 18) {
			//Gen 5
			piece = new PIECE("Texture/Enemy.png", "Texture/Gen5.png", tiles[i]->getposition(), RANKS::GEN5, "5 Star General");
			vAlliedPieces.push_back(piece);
		}
		else if (i == 19 || i == 20) {
			//Spy
			piece = new PIECE("Texture/Enemy.png", "Texture/Spy.png", tiles[i]->getposition(), RANKS::SPY, "Spy");
			vAlliedPieces.push_back(piece);
		}
	}

	//Initialize all Enemy Pieces (Player2)
	for (int i = 0; i < 21; i++) {
		if (i == 0) {
			//Flag	
			piece = new PIECE("Texture/Enemy.png", "Texture/Flag.png", tiles[i]->getposition(), RANKS::FLAG, "Flag");
			vEnemyPieces.push_back(piece);
		}
		else if (i > 0 && i < 7) {
			//Private
			piece = new PIECE("Texture/Enemy.png", "Texture/Private.png", tiles[i]->getposition(), RANKS::PRIV, "Private");
			vEnemyPieces.push_back(piece);
		}
		else if (i == 7) {
			//Sergeant
			piece = new PIECE("Texture/Enemy.png", "Texture/Serg.png", tiles[i]->getposition(), RANKS::SARGE, "Sergeant");
			vEnemyPieces.push_back(piece);
		}
		else if (i == 8) {
			//2nd Lieutenant
			piece = new PIECE("Texture/Enemy.png", "Texture/Lieut2.png", tiles[i]->getposition(), RANKS::LT2, "2nd Lieutenant");
			vEnemyPieces.push_back(piece);
		}
		else if (i == 9) {
			//1st Lieutenant
			piece = new PIECE("Texture/Enemy.png", "Texture/Lieut1.png", tiles[i]->getposition(), RANKS::LT1, "1st Lieutenant");
			vEnemyPieces.push_back(piece);
		}
		else if (i == 10) {
			//Captain
			piece = new PIECE("Texture/Enemy.png", "Texture/Capt.png", tiles[i]->getposition(), RANKS::CAPT, "Captain");
			vEnemyPieces.push_back(piece);
		}
		else if (i == 11) {
			//Major
			piece = new PIECE("Texture/Enemy.png", "Texture/Major.png", tiles[i]->getposition(), RANKS::MAJ, "Major");
			vEnemyPieces.push_back(piece);
		}
		else if (i == 12) {
			//Lieutenant Colonel
			piece = new PIECE("Texture/Enemy.png", "Texture/Lieutcol.png", tiles[i]->getposition(), RANKS::LTCOL, "Lieutenant Colonel");
			vEnemyPieces.push_back(piece);
		}
		else if (i == 13) {
			//Colonel
			piece = new PIECE("Texture/Enemy.png", "Texture/Col.png", tiles[i]->getposition(), RANKS::COL, "Colonel");
			vEnemyPieces.push_back(piece);
		}

		else if (i == 14) {
			//Gen 1
			piece = new PIECE("Texture/Enemy.png", "Texture/Gen1.png", tiles[i]->getposition(), RANKS::GEN1, "1 Star General");
			vEnemyPieces.push_back(piece);
		}
		else if (i == 15) {
			//Gen 2
			piece = new PIECE("Texture/Enemy.png", "Texture/Gen2.png", tiles[i]->getposition(), RANKS::GEN2, "2 Star General");
			vEnemyPieces.push_back(piece);
		}
		else if (i == 16) {
			//Gen 3
			piece = new PIECE("Texture/Enemy.png", "Texture/Gen3.png", tiles[i]->getposition(), RANKS::GEN3, "3 Star General");
			vEnemyPieces.push_back(piece);
		}
		else if (i == 17) {
			//Gen 4
			piece = new PIECE("Texture/Enemy.png", "Texture/Gen4.png", tiles[i]->getposition(), RANKS::GEN4, "4 Star General");
			vEnemyPieces.push_back(piece);
		}
		else if (i == 18) {
			//Gen 5
			piece = new PIECE("Texture/Enemy.png", "Texture/Gen5.png", tiles[i]->getposition(), RANKS::GEN5, "5 Star General");
			vEnemyPieces.push_back(piece);
		}
		else if (i == 19 || i == 20) {
			//Spy
			piece = new PIECE("Texture/Enemy.png", "Texture/Spy.png", tiles[i]->getposition(), RANKS::SPY, "Spy");
			vEnemyPieces.push_back(piece);
		}
	}

	for (PIECE* x : vEnemyPieces) {
		Vector2f temp = x->getposition();
		temp.x += 1270;
		x->setposition(temp);
	}

	while (window.isOpen())
	{
		sf::Event event;
		mouseposition = window.mapPixelToCoords(Mouse::getPosition(window));

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			processEvents(&event, &marker, tiles, vAlliedPieces, vEnemyPieces, &currState, mouseposition, &select, &selectindex, &turn, &deadAllies, &deadEnemies);
		}

		window.clear();
		Sprite MenuScreen;
		Sprite Play;
		Sprite Quit;
		Texture Menu, play, quit;

		Menu.loadFromFile("Texture/Menu.png");
		play.loadFromFile("Texture/Start.png");
		quit.loadFromFile("Texture/Quit.png");

		Play.setPosition({ 800, 600 });
		Quit.setPosition({ 800, 740 });

		MenuScreen.setTexture(Menu);
		if (currState == STATES::START) {
			window.draw(Play);
			window.draw(Quit);
			window.draw(MenuScreen);
		}

		else if (currState == STATES::P1SET || currState == STATES::P2SET || currState == STATES::GAME) {
			window.draw(board.getboard());
			for (int i = 0; i < tiles.size(); i++) {
				window.draw(tiles[i]->getshape());
			}
			if (select == true) {
				window.draw(marker.getsprite());
			}
			for (int i = 0; i < vAlliedPieces.size(); i++) {
				window.draw(vAlliedPieces[i]->getsprite());
			}
			for (PIECE* x : vEnemyPieces) {
				window.draw(x->getsprite());
			}
		}

		else {

		}
		
		window.display();
	}

	return 0;
}