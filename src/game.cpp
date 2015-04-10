#include <SFML/System.hpp>
#include <iostream>

#include "game.h"
#include "board.h"
#include "ai.h"

Game::Game() {
  board = new Board;
  ai = new AI(board);
  currentTurn = 2;
  gameOver = false;
  blacks = 0;
  whites = 0;
  skipped = false;
  possibleMoves();
}

Board* Game::getBoard() {
  return board;
}

int Game::getCurrentTurn() {
  return currentTurn;
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

void Game::nextTurn() {
  currentTurn = currentTurn == 1 ? 2 : 1;
  possibleMoves();
  if (currentTurn == 1) {
    sf::Vector2<int> played = ai->makeMove();
    flip(played.x, played.y);
    nextTurn();
  }
}

bool Game::isValidMove(int row, int col) {

  // Tile is taken.
  if (board->get(row, col) > 0) {
    return false;
  }

  return flipCount(row, col) > 0;
}

void Game::click(int row, int col) {
  if (isValidMove(row, col)) {
    std::cout << "human playing " << row << ", " << col << "\n";
    board->set(row, col, currentTurn);
    flip(row, col);
    nextTurn();
  }
}

int Game::flipCount(int row, int col) {
  if (row >= 8 || col >= 8 || row < 0 || col < 0) {
    return 0;
  }

  // `dirX` and `dirY` are each -1, 0, or 1 to change the direction of the
  // search. This way, every possible move is considered.
  //
  // dist is for the length of the search
  // dist = 1 searches one slot away, dist = 2 searches 2 slots away
  //
  // curX and curY are temporary variables to store the temporary positions
  // being searched while looping through the possibilities
  int dirX;
  int dirY;
  int dist = 1;
  int curX = col;
  int curY = row;
  int flipCount = 0;

  // these nested for loops consider -1, 0, and 1 for dir_x and dir_y
  for (dirX = -1; dirX < 2; dirX++) {
    for (dirY = -1; dirY < 2; dirY++) {
      // Set initial dist to 1 and increment only if the slot tested before
      // is the opponent's stone.
      dist = 1;

      // Oh no, a `while (true)` loop!
      while (true) {
        // dirX and dirY only set the direction (-1, 0, or 1)
        curX = col + (dist * dirX);
        curY = row + (dist * dirY);

        // Break current loop if it goes off board.
        if (curX < 0 || curY < 0 || curX >= 8 || curY >= 8){
          break;
        }

        // Break current loop if the slot is blank since no slots after will be
        // able to be taken anyway.
        if (board->get(curY, curX) < 1) {
          break;
        }

        // If the stone is the current player's stone, increment flip count by
        // the distance minus one since the slots preceding the current slot.
        // would be flipped
        if (board->get(curY, curX) == currentTurn) {
          flipCount += dist - 1;
          break;
        }

        // Break if (dirX, dirY) = (0,0) since it'll go into an infinite loop.
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

void Game::possibleMoves() {
  int moves = 0;
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
      else {
        board->set(i, j, 0);
      }

      if (flipCount(i, j) > 0) {
        board->set(i, j, -1);
        skipped = false;
        moves++;
      }
    }
  }

  if (moves == 0 && !skipped) {
    std::cout << "no possible moves for " << currentTurn << "\n";
    skipped = true;
    nextTurn();
  }
  else if (moves == 0 && skipped) {
    std::cout << "2no possible moves for " << currentTurn << "\n";
    gameOver = true;
  }
}
