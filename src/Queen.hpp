#ifndef QUEEN_HPP
#define QUEEN_HPP

#include "Piece.hpp"

// Queen class
template <typename T>
class _Queen : public _Piece<T> {
public:
	_Queen(bool colour, const short int* tileSize) : _Piece<T>(colour, QUEEN, tileSize) {
		
	}
	// virtual std::vector<int*> getValidSquares(T* board) {

	// }
};

#endif