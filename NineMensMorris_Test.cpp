#include "NineMensMorris.h"
#include "Board.h"
#include "Player.h"
#include "Spot.h"
#include <iostream>
#include <cassert>
#include <stdexcept>

#define TEST_CASE(name) std::cout << "Test: " << name << "... ";
#define PASSED() std::cout << "Passed\n"
#define FAILED() std::cout << "Failed\n"

void testBoardPlacement() {
    TEST_CASE("Board Placement");
    Board board;

    for (int position = 0; position < 24; ++position) {
        if (!board.isPositionEmpty(position)) {
            std::cerr << "Error: Position " << position << " should be empty at initialization.\n";
            FAILED();
            return;
        }
    }

    board.placePiece(0, 0);
    if (board.isPositionEmpty(0) || !board.isPositionOwnedBy(0, 0)){
        std::cerr << "Error: Piece placement failed at position 1 for player 0.\n";
        FAILED();
        return;
    }

    try {
        board.placePiece(1, 0);
        std::cerr << "Error: Duplicate placement on occupied position not detected.\n";
        FAILED();
    } catch (const std::runtime_error&) {
        PASSED();
    }
}

void testMillDetection() {
    TEST_CASE("Mill Detection");
    Board board;

    board.placePiece(0, 0);
    board.placePiece(0, 1);
    board.placePiece(0, 2);

    assert(board.isMillFormed(2, 0));
    PASSED();
}

void testPlayerPieces() {
    TEST_CASE("Player Pieces");
    Player player("Test", 0);
    Spot spot(1);

    assert(player.activePieces() == 0);
    assert(player.placePiece(&spot));
    assert(player.activePieces() == 1);
    PASSED();
}

void testMoveValidation() {
    TEST_CASE("Move Validation");
    Board board;

    board.placePiece(0, 0);
    board.placePiece(1, 1);

    try {
        board.movePiece(0, 1);
        std::cerr << "Error: Invalid move (to occupied position) was not caught.\n";
        FAILED();
    } catch (const std::runtime_error&) {
        PASSED();
    }
}

void testFlyingMove() {
    TEST_CASE("Flying Move");
    Board board;

    board.placePiece(0, 0);
    board.placePiece(0, 3);
    board.placePiece(0, 6);
    board.removePiece(3);
    board.placePiece(0, 9);

    assert(board.isValidMove(0, 2, 0, true));
    PASSED();
}

void testInvalidMillRemoval() {
    TEST_CASE("Invalid Mill Removal");
    Player player1("P1", 0);
    Player player2("P2", 1);
    Spot spot(0);

    player2.placePiece(&spot);
    Piece* piece = spot.getPiece();
    piece->setMillStatus(true);

    assert(!player1.capturePiece(piece));
    PASSED();
}    

void testPhaseTransitionToFlying(){
    TEST_CASE("Phase Transition to FLYING");
    Player player("Test", 0);
    Board board;

    for (int pos :{0,1,2}){
        Spot*spot = board.getSpot(pos);
        player.placePiece(spot);
        board.placePiece(0, pos);
    }
    assert(player.canFly());
    PASSED();
}

void testMillFormationAndCapture(){
    TEST_CASE("Mill Formation and Capture");
    Board board;
    Player p1("p1", 0);
    Player p2("p2", 1);

    for (int pos: {0,1,2}){
        Spot* spot = board.getSpot(pos);
        p1.placePiece(spot);
        board.placePiece(0, pos);
    }
    assert(board.isMillFormed(2,0));
    Spot* opponentSpot = board.getSpot(5);
    p2.placePiece(opponentSpot);
    board.placePiece(1,5);
    Piece*target = opponentSpot->getPiece();
    bool captured = p1.capturePiece(target);
    board.removePiece(5);
    assert(captured);
    PASSED();
}

void testWinContditionSimulation(){
    TEST_CASE("Simulation Win Condition");
    Player p1("p1", 0);
    Player p2("p2", 1);
    for(int i = 0; i < 7; ++i)
        p2.incrementCaptured();
    assert(p2.activePieces() < 3);
    PASSED();
}

void testPlacingToMovingPhase(){
    TEST_CASE("Phase Transition: PLACING to MOVING");
    Player player("Test", 0);
    Board board;

    for (int i = 0; i < 9; ++i){
        Spot*spot = board.getSpot(i);
        player.placePiece(spot);
        board.placePiece(player.getColor(), i);
    }
    assert(player.availableToPlace() == 0);
    PASSED();
}

void testMovingToFlyingPhase(){
    TEST_CASE("Phase Transition: MOVING to FLYING");
    Player player("Test", 0);
    Board board;

    for (int pos : {0,1,2}){
        Spot*spot = board.getSpot(pos);
        player.placePiece(spot);
        board.placePiece(player.getColor(), pos);
    }
    for (int i = 0; i < 6; ++i)
        player.incrementCaptured();
    assert(player.canFly());
    PASSED();
}

int main() {
    std::cout << "=== NINE MEN'S MORRIS TEST SUITE ===\n\n";

    testBoardPlacement();
    testMillDetection();
    testPlayerPieces();
    testMoveValidation();
    testFlyingMove();
    testInvalidMillRemoval();
    testPhaseTransitionToFlying();
    testMillFormationAndCapture();
    testWinContditionSimulation();
    testPlacingToMovingPhase();
    testMovingToFlyingPhase();

    std::cout << "\nAll tests completed!\n";
    return 0;
} 
