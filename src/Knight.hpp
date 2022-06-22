#ifndef KNIGHT_HPP
#define KNIGHT_HPP

#include "Piece.hpp"

// Knight class
template <typename T>
class _Knight : public _Piece<T> {
public:
	_Knight(bool colour, const short int* tileSize) : _Piece<T>(colour, KNIGHT, tileSize) {
		
	}
	// virtual std::vector<int*> getValidSquares(T* board) {

	// }
};

#endif