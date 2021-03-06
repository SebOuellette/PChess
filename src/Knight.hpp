#ifndef KNIGHT_HPP
#define KNIGHT_HPP

#include "Piece.hpp"

// Knight class
template <typename T>
class _Knight : public _Piece<T> {
public:
	_Knight(bool colour, const short int* tileSize, short int startx, short int starty) : _Piece<T>(colour, KNIGHT, tileSize, startx, starty) {
		
	}
	
	virtual std::vector<sf::Vector2i> getValidSquares(T * board) {
		std::vector<sf::Vector2i> validSquares;
		const sf::Vector2i thisPos = this->getPosition();

		for (int y=-1;y<=1;y++) {
			for (int x=-1;x<=1;x++) {
				if (x != 0 && y != 0 || (x == 0 && y == 0))
					continue;

				sf::Vector2i middlePos = thisPos + sf::Vector2i(x, y) * 2;
				

				for (int i=-1;i<=1;i+=2) {
					sf::Vector2i newPos = middlePos + sf::Vector2i(y, x) * i;

					// Check if search out of bounds
					if (newPos.x < 0 || newPos.x > 7 || newPos.y < 0 || newPos.y > 7)
						continue;

					// Check if found a target
					if (board->getPiece(newPos.x, newPos.y) != nullptr && board->getPiece(newPos.x, newPos.y)->isPieceWhite() == this->isPieceWhite())
						continue;

					// Add the position to the list
					validSquares.push_back(newPos);
				}
			}
		}

		return validSquares;
	}
};

#endif