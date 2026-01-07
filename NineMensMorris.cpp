#include "NineMensMorris.h"
#include <algorithm>
#include <iostream>
#include <limits>

NineMensMorris::NineMensMorris()
    : currentPlayer_(0),
      currentPhase_(Phase::PLACING),
      board_(),
      players_{ Player("Player 1", 0), Player("Player 2", 1) },
      lastMovePos_(-1) {}

void NineMensMorris::startGame() {
    while (!checkWinCondition()) {
        board_.displayBoardWithReference();
        std::cout << "\nCurrent player: " << getCurrentPlayer().getName()
                  << " (" << (getCurrentPlayer().getColor() == 0 ? "Light" : "Dark") << ")\n";

        std::string action;
        switch (currentPhase_) {
            case Phase::PLACING: action = "Place a piece"; break;
            case Phase::MOVING:  action = "Move a piece"; break;
            case Phase::FLYING:  action = "Fly a piece"; break;
        }
        std::cout << "Action: " << action << "\n";

        try {
            switch (currentPhase_) {
                case Phase::PLACING: handlePlacingPhase(); break;
                case Phase::MOVING:  handleMovingPhase(); break;
                case Phase::FLYING:  handleFlyingPhase(); break;
            }

            if (board_.isMillFormed(lastMovePos_, currentPlayer_)) {
                handleMillFormation();
            } else {
                switchPlayer();
            }

            updateGamePhase();

        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    announceWinner();
}

void NineMensMorris::handlePlacingPhase() {
    while (true) {
        std::cout << "Place a piece (1-24): ";
        int pos = getValidInput(1, 24) - 1;

        if (!board_.isPositionEmpty(pos)) {
            std::cout << "That spot is already occupied! Try a different position.\n";
            continue;
        }

        Spot* spot = board_.getSpot(pos);

        if (!players_[currentPlayer_].placePiece(spot)) {
            throw std::runtime_error("No pieces left to place");
        }

        board_.placePiece(currentPlayer_, pos);
        lastMovePos_ = pos;
        break;
    }
}

void NineMensMorris::handleMovingPhase() {
    std::cout << "Move a piece (from to) [1-24]: ";
    int from = getValidInput(1, 24) - 1;
    int to = getValidInput(1, 24) - 1;

    if (!board_.isValidMove(from, to, currentPlayer_, false)) {
        throw std::runtime_error("Invalid move");
    }

    board_.movePiece(from, to);
    lastMovePos_ = to;
}

void NineMensMorris::handleFlyingPhase() {
    std::cout << "Fly a piece (from to) [1-24]: ";
    int from = getValidInput(1, 24) - 1;
    int to = getValidInput(1, 24) - 1;

    if (!board_.isValidMove(from, to, currentPlayer_, true)) {
        throw std::runtime_error("Invalid fly move");
    }

    board_.movePiece(from, to);
    lastMovePos_ = to;
}

void NineMensMorris::handleMillFormation() {
    int opponentColor = (currentPlayer_ + 1) % 2;
    std::vector<int> removable = board_.getRemovableOpponentPieces(opponentColor);

    if (removable.empty()) {
        std::cout << "Opponent's pieces can't be removed.\n";
        return;
    }

    while (true) {
        std::cout << "MILL FORMED! Select opponent's piece to remove: ";
        int pos = getValidInput(1, 24) - 1;

        try {
            if (std::find(removable.begin(), removable.end(), pos) == removable.end()) {
                throw std::runtime_error("Piece is in mill or invalid. Choose another.");
            }

            Spot* spot = board_.getSpot(pos);
            Piece* target = spot->getPiece();

            if (!target) {
                throw std::runtime_error("No piece found at selected position.");
            }

            if (!players_[opponentColor].capturePiece(target)) {
                throw std::runtime_error("Failed to capture piece.");
            }

            board_.removePiece(pos);
            players_[currentPlayer_].incrementCaptured();

            break;
        } catch (const std::exception& e) {
            std::cout << "ERROR: " << e.what() << " Try again.\n";
        }
    }

    updateGamePhase();
    switchPlayer();
}

void NineMensMorris::switchPlayer() {
    currentPlayer_ = (currentPlayer_ + 1) % 2;
}

void NineMensMorris::updateGamePhase() {
    Player& current = getCurrentPlayer();

    bool allPlaced = (current.availableToPlace() == 0);
    int piecesPlaced = 9 - current.availableToPlace();
    int capturedPieces = current.getCaptured();
    int totalPiecesToPlace = 9 - capturedPieces;

    if (currentPhase_ == Phase::PLACING) {
        if (piecesPlaced >= totalPiecesToPlace) {
            currentPhase_ = Phase::MOVING;
            std::cout << "[PHASE] " << current.getName() << " placed all available pieces. Switching to MOVING phase.\n";
        } else {
            std::cout << "[DEBUG] " << current.getName()
                      << " placed: " << piecesPlaced
                      << ", captured: " << capturedPieces
                      << ", needed: " << totalPiecesToPlace << "\n";
        }
    }

    if (currentPhase_ == Phase::MOVING && current.canFly()) {
        currentPhase_ = Phase::FLYING;
    }
}

bool NineMensMorris::checkWinCondition() const {
    if (currentPhase_ == Phase::PLACING) return false;

    const Player& opponent = players_[(currentPlayer_ + 1) % 2];
    return opponent.activePieces() < 3 || !hasValidMoves(opponent);
}

bool NineMensMorris::hasValidMoves(const Player& player) const {
    if (player.canFly()) {
        for (int i = 0; i < 24; ++i)
            if (board_.isPositionEmpty(i)) return true;
    } else {
        for (const auto& piece : player.getPieces()) {
            if (piece.isPlaced()) {
                int pos = piece.getPosition()->getPosition();
                for (int adj : board_.getAdjacentPositions(pos))
                    if (board_.isPositionEmpty(adj)) return true;
            }
        }
    }
    return false;
}

int NineMensMorris::getValidInput(int min, int max) {
    int input;
    while (true) {
        std::string inputStr;
        std::cin >> inputStr;
        if (inputStr == "exit") std::exit(0);

        try {
            input = std::stoi(inputStr);
            if (input >= min && input <= max) return input;
        } catch (...) {}

        std::cout << "Invalid input. Enter a number between " << min << " and " << max << ": ";
    }
}

void NineMensMorris::announceWinner() const {
    if (checkWinCondition()) {
        std::cout << getCurrentPlayer().getName() << " wins the game!\n";
        std::exit(0);
    }
}

Player& NineMensMorris::getCurrentPlayer() { return players_[currentPlayer_]; }
const Player& NineMensMorris::getCurrentPlayer() const { return players_[currentPlayer_]; }

#ifndef RUN_TESTS
int main() {
    while (true) {
        std::cout << "==================== NINE MEN'S MORRIS ====================\n";
        std::cout << "1. Start Game\n";
        std::cout << "2. Exit\n";
        std::cout << "Choose a section please (1 or 2): ";

        std::string choiceStr;
        std::cin >> choiceStr;

        if (choiceStr == "1") {
            NineMensMorris game;
            game.startGame();
        } else if (choiceStr == "2") {
            break;
        } else {
            std::cout << "Invalid input. Please choose section 1 or 2.\n";
        }
    }
    return 0;
} 
#endif
