#pragma once
#include "Piece.h"

class Spot{
public:
    Spot(int position = -1) : position_(position), placedPiece_(nullptr) {}

    int getPosition() const { return position_; }

    Piece* getPiece() const { return placedPiece_; }

    bool isEmpty() const { return placedPiece_ == nullptr; }

    void placePiece(Piece* piece) { placedPiece_ = piece; }

    void removePiece() { placedPiece_ = nullptr; }

private:
    int position_;
    Piece* placedPiece_;
}; 
