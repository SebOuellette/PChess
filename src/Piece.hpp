#ifndef PIECE_HPP
#define PIECE_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

enum PieceID {
	KING = 0,
	QUEEN = 1,
	BISHOP = 2,
	KNIGHT = 3,
	ROOK = 4,
	PAWN = 5
};

// Piece parent class
template <typename T>
class _Piece {
private:
	const short int * tileSize;
	bool isWhite = true;
	PieceID pieceID;
	sf::Texture texture;
	sf::Sprite sprite;

protected:
	short int pos[2];

public:
	_Piece(bool isWhite, PieceID pieceID, const short int * tileSize) : tileSize(tileSize) {
		this->isWhite = isWhite;
		this->pieceID = pieceID;
		
		// Convert the isWhite boolean to an integer
		int colourBit = (int)(!this->isWhite);
		
		// Convert the piece ID enum to an integer
		int pieceType = (int)this->pieceID;

		if (!this->texture.loadFromFile("images/pixel_spritesheet.png", sf::IntRect(pieceType*14, 0, 14, 14))) {
			std::cout << "Error loading piece texture" << std::endl;
		}
		this->texture.setSmooth(false);

		this->sprite.setTexture(this->texture);
		this->sprite.setScale(*tileSize / this->sprite.getLocalBounds().width, *tileSize / this->sprite.getLocalBounds().height);
	}

	_Piece() {
		this->isWhite = true;
	}

	sf::Sprite * getSprite() {
		return &(this->sprite);
	}

	sf::Texture * getTexture() {
		return &this->texture;
	}

	sf::Vector2i getPosition() {
		return sf::Vector2i(this->pos[0], this->pos[1]);
	}
	
	void setPosition(short int x, short int y) {
		this->sprite.setPosition(sf::Vector2f(*this->tileSize * x, *this->tileSize * y));
		this->pos[0] = x;
		this->pos[1] = y;
	}

	void setPosition(sf::Vector2i newPos) {
		this->setPosition(newPos.x, newPos.y);
	}

	int getType() {
		// pKing = 0,
		// pQueen = 1,
		// pBishop = 2,
		// pKnight = 3,
		// pRook = 4,
		// pPawn = 5

		return this->pieceID;
	}

	bool isPieceWhite() {
		return this->isWhite;
	}

	virtual std::vector<int*> getValidSquares(T * board) {
		return std::vector<int*>{};
	}
};

#endif