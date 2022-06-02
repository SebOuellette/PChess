#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Piece.hpp"


// Pawn class
class Pawn : public Piece {
public:
	Pawn(bool, const short int*);
	virtual std::vector<int*> getValidSquares(Piece* (*)[8]);
};