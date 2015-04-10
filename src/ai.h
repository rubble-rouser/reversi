#ifndef AI_H
#define AI_H

#include <SFML/System.hpp>
#include "board.h"

class AI {
  public:
    AI(Board* board);
    sf::Vector2<int> makeMove();

  private:
    Board* board;
    int flipCount(int row, int col);
};

#endif
