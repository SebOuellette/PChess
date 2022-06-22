#ifndef KING_HPP
#define KING_HPP

#include "Piece.hpp"

// King class
template <typename T>
class _King : public _Piece<T> {
public:
	_King(bool colour, const short int* tileSize) : _Piece<T>(colour, KING, tileSize) {
		
	}
	// virtual std::vector<int*> getValidSquares(T* board) {

	// }
};

#endif