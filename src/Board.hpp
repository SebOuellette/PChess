#ifndef BOARD_HPP
#define BOARD_HPP
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