#include "Pawn.hpp"

Pawn::Pawn(bool isWhite, const short int * tileSize) : Piece(isWhite, pPawn, tileSize) {
	
}

std::vector<int*> Pawn::getValidSquares(Piece * board[][8]) {
    short int* x = &this->pos[0];
    short int* y = &this->pos[1];

    std::cout << *x << " " << *y << std::endl;

    return std::vector<int*>{1};
}