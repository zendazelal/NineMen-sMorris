#pragma once

#include <string>
#include "Board.h"
#include "Player.h"
#include "Piece.h"

class NineMensMorris {
public:
    enum class Phase {
        PLACING, 
        MOVING,  
        FLYING 
    };

    NineMensMorris();     
    void startGame();       
    void saveGameToFile(const std::string& filename);
    void loadGameFromFile(const std::string& filename);

private:
    int currentPlayer_;     
    Phase currentPhase_;         
    Board board_;             
    Player players_[2];           
    int lastMovePos_;

    void handlePlacingPhase();
    void handleMovingPhase();
    void handleFlyingPhase();
    bool checkWinCondition() const;
    void updateGamePhase();
    void switchPlayer();
    void handleMillFormation();
    bool hasValidMoves(const Player& player) const;
    int getValidInput(int min, int max);
    void displayBoard() const;
    void announceWinner() const;
    std::string phaseToString(Phase phase) const;
    char getPieceChar(int pos) const;
    Player& getCurrentPlayer();
    const Player& getCurrentPlayer() const;
}; 
