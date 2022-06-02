#pragma once
#include <SFML/Graphics.hpp>
#include "Piece.hpp"

// Queen class
class Queen : public Piece {
public:
	Queen(bool, const short int*);
};
