#pragma once

#include <vector>
#include <stdexcept>
#include "Spot.h"

class Board {
public:
    Board(); 
    bool isValidMove(int from, int to, int playerColor, bool isFlying) const;
    bool isMillFormed(int lastMovePos, int playerColor);
    bool canFly(int playerColor) const;
    bool isPositionEmpty(int pos) const;
    bool isPositionOwnedBy(int pos, int playerColor) const;
    bool isAdjacent(int from, int to) const;

    void placePiece(int playerColor, int pos);
    void movePiece(int from, int to);
    void removePiece(int pos);

    const std::vector<int>& getAdjacentPositions(int pos) const;
    const std::vector<std::vector<int>>& getAllMills() const;
    const std::vector<int>& getPositions() const;

    std::vector<int> getRemovableOpponentPieces(int opponentColor) const;
    void displayBoardWithReference() const;
    void setPositions(const std::vector<int>& positions);
    Spot* getSpot(int pos);
    const Spot* getSpot(int pos) const;

private:
    std::vector<int> positions_;                
    std::vector<std::vector<int>> mills_;           
    std::vector<std::vector<int>> adjacency_;       
    std::vector<Spot> spots_;                       
    void validatePosition(int pos) const;
}; 
