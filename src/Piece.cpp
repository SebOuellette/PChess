#include "Piece.hpp"

Piece::Piece(bool isWhite, PieceID pieceID, const short int * tileSize) : tileSize(tileSize) {
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

Piece::Piece() {
	this->isWhite = true;
}

sf::Sprite * Piece::getSprite() {
	return &(this->sprite);
}


void Piece::setPosition(short int x, short int y) {
	this->sprite.setPosition(sf::Vector2f(*this->tileSize * x, *this->tileSize * y));
	this->pos[0] = x;
	this->pos[1] = y;
}

void Piece::setPosition(sf::Vector2i newPos) {
	this->setPosition(newPos.x, newPos.y);
}

int Piece::getType() {
	// pKing = 0,
	// pQueen = 1,
	// pBishop = 2,
	// pKnight = 3,
	// pRook = 4,
	// pPawn = 5

	return this->pieceID;
}

std::vector<int*> Piece::getValidSquares(Piece * board[][8]) {
	return std::vector<int*>{};
}

bool Piece::isPieceWhite() {
	return this->isWhite;
}

sf::Texture * Piece::getTexture() {
	return &this->texture;
}

sf::Vector2i Piece::getPosition() {
	return sf::Vector2i(this->pos[0], this->pos[1]);
}