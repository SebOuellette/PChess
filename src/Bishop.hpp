#ifndef BISHOP_HPP
#define BISHOP_HPP

#include "Piece.hpp"

// Bishop class
template <typename T>
class _Bishop : public _Piece<T> {
public:
	_Bishop(bool colour, const short int* tileSize) : _Piece<T>(colour, BISHOP, tileSize) {
		
	}
};

#endif