#pragma once

class Player;
class Spot;
class Piece {
public:
    explicit Piece(Player* owner = nullptr);
    Player* getOwner() const;
    Spot* getPosition() const;
    bool isPlaced() const;
    bool isInMill() const;
    void place(Spot* newPosition);
    void removeFromBoard();
    void setMillStatus(bool inMill);

private:
    Player* owner_; 
    Spot* position_;
    bool inMill_;       
}; 
