#ifndef BOARD_HPP
#define BOARD_HPP
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <algorithm>
#include "Piece.hpp"
#include "Pawn.hpp"
#include "Rook.hpp"
#include "Bishop.hpp"
#include "Knight.hpp"
#include "Queen.hpp"
#include "King.hpp"

#define AUDIO_SLIDE_PITCH 200.f

// class Board;

// typedef  _Piece<Board> Piece;
// typedef  _Pawn<Board> Pawn;
// typedef  _Rook<Board> Rook;
// typedef  _Knight<Board> Knight;
// typedef  _Bishop<Board> Bishop;
// typedef  _Queen<Board> Queen;
// typedef  _King<Board> King;

// Board class
class Board {
private:
	const short int tileSize;

	// The array of pieces on the board. The array just stores indexes
	_Piece<Board> *pieces[8][8] = {};

	sf::RenderTexture pieceOffsetTexture;
	
	sf::RenderTexture backgroundTexture;
	sf::Sprite background;
	sf::Color tile1 = sf::Color(241, 217, 181);
	sf::Color tile2 = sf::Color(181, 136, 99);

	sf::SoundBuffer slideBuffer;
	sf::SoundBuffer takeBuffer;

	// TODO
	sf::Vector2i position; // Position (in pixels) where the board is on the window  (mousepos must be adjusted for this offset)
	sf::Vector2i size; // Size of the board

	void genPieces();
public:
	sf::Shader pieceShader;
	sf::Shader pieceOffsetShader;
	// Sound effect
	sf::Sound slideSound;
	sf::Sound takeSound;


	Board(short int, sf::Window* window);
	~Board();
	sf::Sprite * getBackground();
	void drawPieces(sf::RenderWindow* window, _Piece<Board>* holdingPiece);
	//int tryMove(Piece* piece, short int x, short int y);

	// Setters / Getters
	_Piece<Board>* getPiece(int x, int y);
	_Piece<Board>* getPiece(sf::Vector2f pos);
	void setPiece(int x, int y, _Piece<Board>* newPiece);
	void setPiece(sf::Vector2f pos, _Piece<Board>* newPiece);
	sf::Vector2f getPosition();
	void setPosition(sf::Vector2f newPos);
	sf::Vector2i getSize();
	void setSize(sf::Vector2i newSize);

	// Used for mapping cursor pos to the board's coordinate system
	//  Board system is translated by position, then the Y axis is flipped across the center of the board. 
	sf::Vector2f mapShaderCoord(sf::Vector2f);
	sf::Vector2f mapWindowPixelToBoard(sf::Vector2f);


	void doFrame(
		sf::RenderWindow* window,
		sf::Vector2f lastClickedPiece, 
		sf::Vector2f mouseVelocity,
		sf::Vector2f hoveredTile,
		sf::Cursor* cursor,
		sf::Vector2f mousePos,
		float framerate
	);

};

#endif 