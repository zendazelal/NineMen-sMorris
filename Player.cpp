#include <iostream>
#include <algorithm>
#include "Player.h"

Player::Player(const std::string& name, int color)
    : name_(name), color_(color), pieces_(9, Piece(this)), capturedPieces_(0) {}

bool Player::placePiece(Spot* spot) {
    if (availableToPlace() <= 0 || !spot || !spot->isEmpty()) return false;

    for (auto& piece : pieces_) {
        if (!piece.isPlaced()) {
            piece.place(spot);
            spot->placePiece(&piece);
            return true;
        }
    }
    return false;
}

int Player::availableToPlace() const {
    int totalToPlace = 9 - capturedPieces_;
    if (totalToPlace < 0) totalToPlace = 0;

    int placedCount = std::count_if(pieces_.begin(), pieces_.end(),
                                    [](const Piece& p) { return p.isPlaced(); });

    return totalToPlace - placedCount;
}

int Player::totalActivePieces() const {
    int count = 0;
    for (const auto& piece : pieces_) {
        if (piece.isPlaced()) count++;
    }
    return count;
}

bool Player::capturePiece(Piece* target) {
    if (target->isInMill()) return false;
    Spot* spot = target->getPosition();
    target->removeFromBoard();
    if (spot) spot->removePiece();
    return true;
}

int Player::activePieces() const {
    return std::count_if(pieces_.begin(), pieces_.end(),
                         [](const Piece& p) { return p.isPlaced(); });
}

bool Player::canFly() const { return activePieces() <= 3; }
std::string Player::getName() const { return name_; }
int Player::getColor() const { return color_; }
const std::vector<Piece>& Player::getPieces() const { return pieces_; }
std::vector<Piece>& Player::getPieces() { return pieces_; }
void Player::incrementCaptured() {++capturedPieces_;}
int Player::getCaptured() const {return capturedPieces_;}
