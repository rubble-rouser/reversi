#include <SFML/System.hpp>
#include "ai.h"
#include "board.h"
#include "game.h"

Game::Game() {
  board = new Board;
  ai = new AI(board);
  currentTurn = 2;
  gameOver = false;
  blacks = 0;
  whites = 0;
  skipped = false;
  aiOn = false;
  possibleMoves();
}

Game::~Game() {
  delete board;
  delete ai;
}

void Game::setAI(bool on) {
  aiOn = on;
}

Board* Game::getBoard() {
  return board;
}

int Game::getCurrentTurn() {
  return currentTurn;
}

bool Game::getAIOn() {
  return aiOn;
}

bool Game::isOver() {
  return gameOver;
}

int Game::blackScore() {
  return blacks;
}

int Game::whiteScore() {
  return whites;
}

void Game::playAI() {
  sf::Vector2<int> played = ai->makeMove(currentTurn);
  flip(played.x, played.y);
  nextTurn();
}

void Game::nextTurn() {
  currentTurn = currentTurn == 1 ? 2 : 1;
  possibleMoves();

  if (gameOver) {
    return;
  }
}

bool Game::isValidMove(int row, int col) {
  // Tile does not exist.
  if (row < 0 || row > 7 || col < 0 || col > 7) {
    return false;
  }

  // Tile is taken.
  if (board->get(row, col) > 0) {
    return false;
  }

  return flipCount(row, col) > 0;
}

void Game::click(int row, int col) {
  if (isValidMove(row, col)) {
    board->set(row, col, currentTurn);
    flip(row, col);
    nextTurn();
  }
}

// Get the number of pieces the opponent will lose if the current player places
// a piece at row, col.
int Game::flipCount(int row, int col) {
  if (row >= 8 || col >= 8 || row < 0 || col < 0) {
    return 0;
  }

  // We must iterate in all eight directions from row, col.
  //
  // `dirX` and `dirY` are each -1, 0, or 1 to change the direction of the
  // search. This way, every possible move is considered.
  //
  // `dist` tracks the length of our search in a particular direction.
  // For example, `dist = 2` means that we are 2 tiles away from row, col.
  //
  // `curX` and `curY` are temporary variables to store the temporary positions
  // being searched while looping through the possibilities.
  int dirX;
  int dirY;
  int dist = 1;
  int curX = col;
  int curY = row;
  int flipCount = 0;

  // These nested for-loops consider -1, 0, and 1 for `dirX` and `dirY`.
  for (dirX = -1; dirX < 2; dirX++) {
    for (dirY = -1; dirY < 2; dirY++) {

      // Set initial dist to 1 and increment only if the slot tested before is
      // the opponent's piece.
      dist = 1;

      // Oh no, a `while (true)` loop! Don't worry, we `break` out of it
      // eventually.
      while (true) {

        // Again, `dirX` and `dirY` only set the direction (-1, 0, or 1). We
        // go `dist` times away from row, col in this direction.
        curX = col + (dist * dirX);
        curY = row + (dist * dirY);

        // Break current loop if it has gone off the board.
        if (curX < 0 || curY < 0 || curX > 7 || curY > 7) {
          break;
        }

        // Break current loop if the slot is blank since no slots after will be
        // able to be taken anyway.
        if (board->get(curY, curX) < 1) {
          break;
        }

        // If the stone is the current player's stone, increment flip count by
        // the distance minus one since the slots preceding the current slot
        // would be flipped
        if (board->get(curY, curX) == currentTurn) {
          flipCount += dist - 1;
          break;
        }

        // Break if (dirX, dirY) = (0, 0), otherwise we will enter an infinite
        // loop.
        if (dirX == 0 && dirY == 0) {
          break;
        }

        dist++;
      }
    }
  }

  return flipCount;
}

void Game::flip(int row, int col) {
  if (row >= 8 || col >= 8 || row < 0 || col < 0) {
    return;
  }

  int dirX;
  int dirY;
  int dist = 1;
  int curX = col;
  int curY = row;

  for (dirX = -1; dirX < 2; dirX++) {
    for (dirY = -1; dirY < 2; dirY++) {
      dist = 1;

      while (true) {
        curX = col + (dist * dirX);
        curY = row + (dist * dirY);

        if (curX < 0 || curY < 0 || curX >= 8 || curY >= 8){
          break;
        }

        if (board->get(curY, curX) < 1) {
          break;
        }

        if (board->get(curY, curX) == currentTurn) {
          // Backtrace moves and flip opponent's slots. Decrement `dist` and
          // flip until `dist` is 0.
          for (dist--; dist > 0; dist--) {
            curX = col + (dist * dirX);
            curY = row + (dist * dirY);
            board->set(curY, curX, currentTurn);
          }
          break;
        }

        if (dirX == 0 && dirY == 0) {
          break;
        }

        dist++;
      }
    }
  }
}

// Re-calculate what moves are possible, update score counts, and determine if
// game is over.
void Game::possibleMoves() {
  int moves = 0;

  // Since we are about to traverse the entire board, we might as well update
  // each player's scores while we are at it.
  blacks = 0;
  whites = 0;

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (board->get(i, j) > 0) {
        if (board->get(i, j) == 1) {
          whites++;
        }
        else {
          blacks++;
        }

        continue;
      }

      // Reset previously-possible moves to `0` for re-calculation.
      else {
        board->set(i, j, 0);
      }

      // If at least one piece can be flipped from this position, then this is a
      // possible move.
      if (flipCount(i, j) > 0) {
        board->set(i, j, -1);
        skipped = false;
        moves++;
      }
    }
  }

  // If no moves were found, pass the turn to the next player.
  if (moves == 0 && !skipped) {
    skipped = true;
    nextTurn();
  }

  // If no moves were found, and we have already passed the turn to the second
  // player, then there must be no moves remaining. The game is now over.
  else if (moves == 0 && skipped) {
    gameOver = true;
  }
}
