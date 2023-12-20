#ifndef ROOK_HPP
#define ROOK_HPP

#include <SFML/Graphics.hpp>
#include "Piece.hpp"

// Rook class
template <typename T>
class _Rook : public _Piece<T> {
public:
	_Rook(bool colour, const short int* tileSize, sf::Vector2f boardPos, short int startx, short int starty) : _Piece<T>(colour, ROOK, tileSize, boardPos, startx, starty) {
		
	}
	
	virtual std::vector<sf::Vector2i> getValidSquares(T * board) {
		std::vector<sf::Vector2i> validSquares;
		const sf::Vector2i thisPos = this->getPosition();

		for (int y=-1;y<=1;y++) {
			for (int x=-1;x<=1;x++) {
				if (x != 0 && y != 0 || (x == 0 && y == 0))
					continue;

				// X and Y point in the direction of where the rook should search for legal moves
				int searchCount = 0;
				while (true) {
					searchCount++;

					sf::Vector2i newPos = thisPos + sf::Vector2i(x, y) * searchCount;

					// Check if search out of bounds
					if (newPos.x < 0 || newPos.x > 7 || newPos.y < 0 || newPos.y > 7)
						break;

					// Check if found a target
					if (board->getPiece(newPos.x, newPos.y) != nullptr) {
						if (board->getPiece(newPos.x, newPos.y)->isPieceWhite() != this->isPieceWhite()) // IF the pieces are different colours, the piece can be taken
							validSquares.push_back(newPos);

						break;
					}

					// Add the position to the list
					validSquares.push_back(newPos);
				}
			}
		}

		return validSquares;
	}
};

#endif