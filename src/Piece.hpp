#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

enum PieceID {
	pKing = 0,
	pQueen = 1,
	pBishop = 2,
	pKnight = 3,
	pRook = 4,
	pPawn = 5
};

// Piece parent class
class Piece {
private:
	const short int * tileSize;
	bool isWhite = true;
	PieceID pieceID;
	sf::Texture texture;
	sf::Sprite sprite;

protected:
	short int pos[2];

public:
	Piece(bool, PieceID, const short int*);
	Piece();
	sf::Sprite * getSprite();
	sf::Texture * getTexture();
	sf::Vector2i getPosition();
	void setPosition(short int, short int);
	void setPosition(sf::Vector2i);
	int getType();
	bool isPieceWhite();
	virtual std::vector<int*> getValidSquares(Piece* (*)[8]);
};
