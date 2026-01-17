#pragma once

#include <vector>
#include <string>
#include "Piece.h"
#include "Spot.h"

class Player {
public:
    Player(const std::string& name, int color);

    std::string getName() const;
    int getColor() const;

    bool canFly() const; 
    int activePieces() const; 

    int availableToPlace() const;
    int totalActivePieces() const; 

    bool placePiece(Spot* spot); 
    bool capturePiece(Piece* target); 

    const std::vector<Piece>& getPieces() const; 
    std::vector<Piece>& getPieces(); 

    void incrementCaptured();
    int getCaptured() const;

private:
    std::string name_;
    int color_;
    std::vector<Piece> pieces_;
    int capturedPieces_ = 0; 
};
