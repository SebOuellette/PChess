#ifndef KING_HPP
#define KING_HPP

#include "Piece.hpp"

// King class
template <typename T>
class _King : public _Piece<T> {
public:
	_King(bool colour, const short int* tileSize, short int startx, short int starty) : _Piece<T>(colour, KING, tileSize, startx, starty) {
		
	}

	virtual std::vector<sf::Vector2i> getValidSquares(T * board) {
		std::vector<sf::Vector2i> validSquares;
		const sf::Vector2i thisPos = this->getPosition();

		for (int y=-1;y<=1;y++) {
			for (int x=-1;x<=1;x++) {
				if (x == 0 && y == 0)
					continue;

				sf::Vector2i newPos = thisPos + sf::Vector2i(x, y);

				// Check if search out of bounds
				if (newPos.x < 0 || newPos.x > 7 || newPos.y < 0 || newPos.y > 7)
					continue;

				// Check if found a target
				if (board->getPiece(newPos.x, newPos.y) != nullptr) {
					if (board->getPiece(newPos.x, newPos.y)->isPieceWhite() != this->isPieceWhite()) // IF the pieces are different colours, the piece can be taken
						validSquares.push_back(newPos);

					continue;
				}
				
				// Add the position to the list
				validSquares.push_back(newPos);
			}
		}

		return validSquares;
	}
};

#endif