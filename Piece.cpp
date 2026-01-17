#include "Piece.h"

Piece::Piece(Player* owner) : owner_(owner), position_(nullptr), inMill_(false) {}

void Piece::place(Spot* newPosition) {
    position_ = newPosition;
    inMill_ = false;
}

void Piece::removeFromBoard() {
    position_ = nullptr;
    inMill_ = false;
}

void Piece::setMillStatus(bool inMill) {inMill_ = inMill;}
Player* Piece::getOwner() const {return owner_;}      
Spot* Piece::getPosition() const {return position_;} 
bool Piece::isPlaced() const {return position_ != nullptr;} 
bool Piece::isInMill() const {return inMill_;}              
