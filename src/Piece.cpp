#include "Piece.hpp"

// template <typename T>
// _Piece<T>::_Piece(bool isWhite, PieceID pieceID, const short int * tileSize) : tileSize(tileSize) {
// 	this->isWhite = isWhite;
// 	this->pieceID = pieceID;
	
// 	// Convert the isWhite boolean to an integer
// 	int colourBit = (int)(!this->isWhite);
	
// 	// Convert the piece ID enum to an integer
// 	int pieceType = (int)this->pieceID;

// 	if (!this->texture.loadFromFile("images/pixel_spritesheet.png", sf::IntRect(pieceType*14, 0, 14, 14))) {
// 		std::cout << "Error loading piece texture" << std::endl;
// 	}
// 	this->texture.setSmooth(false);

// 	this->sprite.setTexture(this->texture);
// 	this->sprite.setScale(*tileSize / this->sprite.getLocalBounds().width, *tileSize / this->sprite.getLocalBounds().height);
// }

// template <typename T>
// _Piece<T>::_Piece() {
// 	this->isWhite = true;
// }

// template <typename T>
// sf::Sprite * _Piece<T>::getSprite() {
// 	return &(this->sprite);
// }

// template <typename T>
// void _Piece<T>::setPosition(short int x, short int y) {
// 	this->sprite.setPosition(sf::Vector2f(*this->tileSize * x, *this->tileSize * y));
// 	this->pos[0] = x;
// 	this->pos[1] = y;
// }

// template <typename T>
// void _Piece<T>::setPosition(sf::Vector2i newPos) {
// 	this->setPosition(newPos.x, newPos.y);
// }

// template <typename T>
// int _Piece<T>::getType() {
// 	// pKing = 0,
// 	// pQueen = 1,
// 	// pBishop = 2,
// 	// pKnight = 3,
// 	// pRook = 4,
// 	// pPawn = 5

// 	return this->pieceID;
// }

// template <typename T>
// std::vector<int*> _Piece<T>::getValidSquares(T * board) {
// 	return std::vector<int*>{};
// }

// template <typename T>
// bool _Piece<T>::isPieceWhite() {
// 	return this->isWhite;
// }

// template <typename T>
// sf::Texture * _Piece<T>::getTexture() {
// 	return &this->texture;
// }

// template <typename T>
// sf::Vector2i _Piece<T>::getPosition() {
// 	return sf::Vector2i(this->pos[0], this->pos[1]);
// }