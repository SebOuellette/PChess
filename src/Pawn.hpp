#ifndef PAWN_HPP
#define PAWN_HPP

#include <vector>
#include "Piece.hpp"


// Pawn class
template <typename T>
class _Pawn : public _Piece<T> {
public:
	_Pawn(bool colour, const short int* tileSize) : _Piece<T>(colour, PAWN, tileSize) {
		
	}

	virtual std::vector<int*> getValidSquares(T * board) {
		short int* x = &this->pos[0];
		short int* y = &this->pos[1];

		std::cout << *x << " " << *y << std::endl;

		return std::vector<int*>{1};
	}
};

#endif