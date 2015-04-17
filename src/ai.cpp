#include <SFML/System.hpp>
#include "ai.h"
#include "board.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

#define cornerValue 6

AI::AI(Board* board) {
  this->board = board;
}

sf::Vector2<int> AI::makeMove(int player) {
  int bestX;
  int bestY;
  int best = 0;
  srand(time(NULL));
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (board->get(i, j) == -1) {
        int score = rateMove(i, j, player);
        if (score > best) {
          bestX = i;
          bestY = j;
          best = score;
        }
        else if (score == best && rand() % 2 == 1) {
          bestX = i;
          bestY = j;
          best = score;
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

int AI::rateMove(int x, int y, int player) {
  int eval = 0;
  if (x > 7 || y > 7 || x < 0 || y < 0) {
    return 0;
  }

  if (board->get(x, y) != -1) {
    return 0;
  }

  int temp = flipCount(x, y, player);

  // Favor corners
  if (x == 0 || x == 7) {
    temp += cornerValue;
  }
  if (y == 0 || y == 7) {
    temp += cornerValue;
  }

  // Disfavor giving corners
  if (x == 1 || x == 6) {
    temp -= cornerValue;
  }
  if (y == 1 || y == 6){
    temp -= cornerValue;
  }

  if (x == 1 && (y == 1 || y == 6)) {
    temp -= cornerValue;
  }
  if (y == 1 && (x == 1 || x == 6)) {
    temp -= cornerValue;
  }

  if (x == 0 && (y == 1 || y == 6)) {
    temp -= 5;
  }
  if (y == 0 && (x == 1 || x == 6)) {
    temp -= 5;
  }
  if (y == 0 && (x == 0 || x == 7)) {
    temp += cornerValue;
  }

  if (x == 0 && (y == 0 || y == 7)) {
    temp += cornerValue;
  }

  if (temp > eval) {
    eval = temp;
  }

  if (eval < 1) {
    eval = 1;
  }

  return eval;
}
