# Nine Men’s Morris – Strategy Game
Nine Men's Morris is a strategic game coded in C++ for two players. Players take turns placing, moving, and eventually "flying" their pieces across a 24-position board. 
The objective is to form "mills" (three pieces in a row) to remove the opponent's pieces. A player wins when the opponent has fewer than three pieces or cannot make a legal move.
## Game Features
- *Text-based interface* (no graphics or GUI)
- *Two-player local mode* – both players share the same computer
- *Three-phase gameplay*: Placing → Moving → Flying
- *Automatic mill detection and piece capture*
- *Game ends* when a player is reduced to fewer than 3 pieces or has no valid moves
## Gameplay Phases
1. *Placing* – Players alternate placing 9 pieces on the board.
2. *Moving* – Players move pieces to adjacent spots.
3. *Flying* – With only 3 pieces left, players may move anywhere.
*Form a mill (three in a row)* to remove an opponent's piece.
*Win by* reducing your opponent to <3 pieces or blocking all moves.
## Main Classes
- **Piece** – Player token, tracks position and mill status.
- **Player** – Handles name, ID, and piece logic.
- **Board** – 24-spot board, manages moves, mills, adjacency.
- **NineMensMorris** – Game engine: turns, phases, input/output.
## Requirements
- C++11 or higher
- Terminal or command line (tested on Windows)
- VS Code or any C++-capable IDE
## Limitations
- No computer AI (human vs. human only)
- No network or online play
- No graphical interface (console-only)
## How to Compile & Run  
If you're using *VS Code with g++*, open your terminal in the project directory and run:
```bash
# Run the Nine Men's Merris
g++ ./NineMensMorris.cpp ./Board.cpp ./Piece.cpp ./Player.cpp
./a.exe.
# Run the Tests
g++ ./NineMensMorris_Test.cpp ./Board.cpp ./Piece.cpp ./Player.cpp
./a.exe.
