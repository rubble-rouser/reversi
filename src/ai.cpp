#include <SFML/System.hpp>
#include "ai.h"
#include "board.h"
#include <iostream>

AI::AI(Board* board) {
  this->board = board;
}

sf::Vector2<int> AI::makeMove(int player) {
  int bestX;
  int bestY;
  int best = 0;
  bool done = false;
  for (int i = 0; i < 8; i++) {
    if (done) {
      break;
    }

    for (int j = 0; j < 8; j++) {
      if (board->get(i, j) == -1) {
        if ((i == 0 || i == 7) && (j == 0 || j == 7)) {
          bestX = i;
          bestY = j;
          done = true;
          break;
        }
        else if (flipCount(i, j, player) > best) {
          bestX = i;
          bestY = j;
        }
      }
    }
  }

  std::cout << "ai playing " << bestX << ", " << bestY << "\n";
  board->set(bestX, bestY, 1);
  return sf::Vector2<int>(bestX, bestY);
}

int AI::flipCount(int row, int col, int player) {
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
        if (board->get(curY, curX) == player) {
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
