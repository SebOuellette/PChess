#ifndef PAWN_HPP
#define PAWN_HPP

#include <SFML/System/Vector2.hpp>
#include <vector>
#include "Piece.hpp"


// Pawn class
template <typename T>
class _Pawn : public _Piece<T> {
public:
	_Pawn(bool colour, const short int* tileSize, sf::Vector2f boardPos, short int startx, short int starty) : _Piece<T>(colour, PAWN, tileSize, boardPos, startx, starty) {
		
	}

	virtual std::vector<sf::Vector2i> getValidSquares(T * board) {
		std::vector<sf::Vector2i> validSquares;
		const sf::Vector2i thisPos = this->getPosition();
		int forwardDir = this->isPieceWhite() ? -1 : 1;



		// Infront of pawn
		sf::Vector2i front = thisPos + sf::Vector2i(0, forwardDir);
		if (board->getPiece(front.x, front.y) == nullptr) {
			validSquares.push_back(front);
			
			// It can move one space forward...
			// now can it move two spaces forward?
			sf::Vector2i twoFront = front + sf::Vector2i(0, forwardDir);
			if (!this->hasPieceMoved() && board->getPiece(twoFront.x, twoFront.y) == nullptr)
				validSquares.push_back(twoFront);
		}

		// Pawn diagonals
		for (int i=-1;i<=1;i+=2) {
			// Get the location for the diagonal
			sf::Vector2i diagonal = front + sf::Vector2i(i, 0);
			
			// Check if there is a piece on the diagonal
			if (board->getPiece(diagonal.x, diagonal.y) != nullptr)
				validSquares.push_back(diagonal);
		}
		

		/// TODO: check if another piece en passanted

		return validSquares;
	}
};

#endif