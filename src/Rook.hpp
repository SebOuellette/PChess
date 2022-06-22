#ifndef ROOK_HPP
#define ROOK_HPP

#include <SFML/Graphics.hpp>
#include "Piece.hpp"

// Rook class
template <typename T>
class _Rook : public _Piece<T> {
public:
	_Rook(bool colour, const short int* tileSize) : _Piece<T>(colour, ROOK, tileSize) {
		
	}
	// virtual std::vector<int*> getValidSquares(T* board) {

	// }
};

#endif