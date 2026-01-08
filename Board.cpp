#include "Board.h"
#include "Piece.h"
#include <iostream>
#include <algorithm>
#include <cstdlib>

void clearScreen(){
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

Board::Board(){
    positions_.resize(24, -1);
    spots_.resize(24);
    for (int i = 0; i < 24; ++i) spots_[i] = Spot(i);

    mills_ = {
    {0,1,2}, {3,4,5}, {6,7,8},
    {0,9,21}, {3,10,18}, {6,11,15},
    {1,4,7}, {16,19,22}, {8,12,17},
    {5,13,20}, {2,14,23}, {9,10,11},
    {12,13,14}, {15,16,17}, {18,19,20},
    {21,22,23}
};

    adjacency_ = {
    {1,9}, {0,2,4}, {1,14},
    {4,10}, {1,3,5,7}, {4,13},
    {7,11}, {4,6,8}, {7,12},
    {0,10,21}, {3,9,11,18}, {6,10,15},
    {8,13,17}, {5,12,14,20}, {2,13,23},
    {11,16}, {15,17,19}, {12,16},
    {10,19}, {16,18,20,22}, {13,19},
    {9,22}, {19,21,23}, {14,22}
    };
}

void Board::validatePosition(int pos) const {
    if (pos < 0 || pos >= 24) {
        throw std::out_of_range("Invalid board position (must be [1-24])");
    }
}

void Board::placePiece(int playerColor, int pos) {
    validatePosition(pos);
    if (!isPositionEmpty(pos)) throw std::runtime_error("Position already occupied");
    positions_[pos] = playerColor;
}

void Board::movePiece(int from, int to) {
    validatePosition(from);
    validatePosition(to);

    if (isPositionEmpty(from)) throw std::runtime_error("No piece at source position");
    if (!isPositionEmpty(to)) throw std::runtime_error("Target position occupied");
    if (!isAdjacent(from, to)) throw std::runtime_error("Positions are not adjacent");

    int player = positions_[from];
    Spot* fromSpot = getSpot(from);
    Spot* toSpot = getSpot(to);
    Piece* movingPiece = fromSpot->getPiece();
    if (!movingPiece) throw std::runtime_error("Internal error: source spot has no piece");
    fromSpot->removePiece();
    toSpot->placePiece(movingPiece);
    movingPiece->place(toSpot);
    positions_[from] = -1;
    positions_[to] = player;
}

void Board::removePiece(int pos) {
    validatePosition(pos);
    if (isPositionEmpty(pos)) throw std::runtime_error("No piece to remove");
    Spot* spot = getSpot(pos);
    if (Piece* piece = spot->getPiece()) {piece->removeFromBoard();}
    spot->removePiece(); 
    positions_[pos] = -1;
}

bool Board::canFly(int playerColor) const {
    int playerPieceCount = std::count(positions_.begin(), positions_.end(), playerColor);
    return playerPieceCount == 3;
}

bool Board::isValidMove(int from, int to, int playerColor, bool isFlying) const {
    validatePosition(from);
    validatePosition(to);

    if (!isPositionOwnedBy(from, playerColor)) return false;
    if (!isPositionEmpty(to)) return false;
    return (isFlying || canFly(playerColor)) ? true : isAdjacent(from, to);
}

bool Board::isMillFormed(int lastMovePos, int playerColor) {
    bool millFound = false;
    for (const auto& mill : mills_) {
        if (std::find(mill.begin(), mill.end(), lastMovePos) != mill.end()) {
            bool completeMill = true;
            for (int pos : mill){
                if (!isPositionOwnedBy(pos, playerColor)) {
                    completeMill = false;
                    break;
                }
            }
            if (completeMill) {
                millFound = true;
                for (int pos : mill) {
                    Spot* spot = getSpot(pos);
                    if (spot && spot->getPiece()) {
                        spot->getPiece()->setMillStatus(true);
                    }
                }
            }
        }
    }
    return millFound;
}

void Board::displayBoardWithReference() const {
    clearScreen();
    const std::vector<int>& board = positions_;
    auto getChar = [](int val) -> char{
        if (val == -1) return '.';
        return val == 0 ? 'O' : 'X';
    };
    
    std::cout << "==================== NINE MEN'S MORRIS ====================\n";
    std::cout <<
        "         ""Game Board""                     ""Position Reference \n"
        "   " << getChar(board[0]) << "---------"<< getChar(board[1]) <<"---------"<< getChar(board[2]) <<"              1---------2---------3\n"
        "   |         |         |              |         |         |\n"
        "   |  "<< getChar(board[3]) <<"------"<< getChar(board[4]) <<"------"<< getChar(board[5]) <<"  |              |  4------5------6  |\n"
        "   |  |      |      |  |              |  |      |      |  |\n"
        "   |  |  "<< getChar(board[6]) <<"---"<< getChar(board[7]) <<"---"<< getChar(board[8]) <<"  |  |              |  |  7---8---9  |  |\n"
        "   |  |  |       |  |  |              |  |  |       |  |  |\n"
        "   "<< getChar(board[9]) <<"--"<< getChar(board[10]) <<"--"<< getChar(board[11]) <<"       "<< getChar(board[12]) <<"--"<< getChar(board[13]) <<"--"<< getChar(board[14]) <<"             10-11-12      13-14-15\n"
        "   |  |  |       |  |  |              |  |  |       |  |  |\n"
        "   |  |  "<< getChar(board[15]) <<"---"<< getChar(board[16]) <<"---"<< getChar(board[17]) <<"  |  |              |  |  16--17--18 |  |\n"
        "   |  |      |      |  |              |  |      |      |  |\n"
        "   |  "<< getChar(board[18]) <<"------"<< getChar(board[19]) <<"------"<< getChar(board[20]) <<"  |              |  19-----20-----21 |\n"
        "   |         |         |              |         |         |\n"
        "   "<< getChar(board[21]) <<"---------"<< getChar(board[22]) <<"---------"<< getChar(board[23]) <<"              22--------23-------24\n";

    std::cout << "\nPieces: O = Player 1 (Light), X = Player 2 (Dark), . = Empty Position\n";
    std::cout << "Quit Game: Type 'exit' at any time to leave.\n\n";
}

bool Board::isPositionEmpty(int pos) const {
    validatePosition(pos);
    return positions_[pos] == -1;
}

bool Board::isPositionOwnedBy(int pos, int playerColor) const {
    validatePosition(pos);
    return positions_[pos] == playerColor;
}

bool Board::isAdjacent(int from, int to) const {
    validatePosition(from);
    validatePosition(to);
    const auto& adjacent = adjacency_[from];
    return std::find(adjacent.begin(), adjacent.end(), to) != adjacent.end();
}

const std::vector<int>& Board::getAdjacentPositions(int pos) const {
    validatePosition(pos);
    return adjacency_[pos];
}

const std::vector<int>& Board::getPositions() const {return positions_;}

void Board::setPositions(const std::vector<int>& positions) {
    if (positions.size() != 24) throw std::runtime_error("Invalid board state");
    positions_ = positions;
}

Spot* Board::getSpot(int pos) {
    validatePosition(pos);
    return &spots_[pos];
}

const Spot* Board::getSpot(int pos) const {
    validatePosition(pos);
    return &spots_[pos];
} 

std::vector<int> Board::getRemovableOpponentPieces(int opponentColor) const {
    std::vector<int> removablePieces;

    for (int pos = 0; pos < 24; ++pos) {
        if (positions_[pos] == opponentColor) {
            const Spot* spot = getSpot(pos);
            if (spot && spot->getPiece() && !spot->getPiece()->isInMill()) {
                removablePieces.push_back(pos);
            }
        }
    }

    if (removablePieces.empty()) {
        for (int pos = 0; pos <= 24; ++pos) {
            if (positions_[pos] == opponentColor) {
                removablePieces.push_back(pos);
            }
        }
    }
    return removablePieces;
}

const std::vector<std::vector<int>>& Board::getAllMills() const{
    return mills_;
} 
