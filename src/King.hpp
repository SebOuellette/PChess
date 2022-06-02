#pragma once
#include <SFML/Graphics.hpp>
#include "Piece.hpp"

// King class
class King : public Piece {
public:
	King(bool, const short int*);
};
