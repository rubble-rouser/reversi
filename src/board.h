#ifndef BOARD_H
#define BOARD_H

class Board {
  public:
    Board();
    int get(int row, int col);
    void set(int row, int col, int value);

  private:
    int board[8][8];
};

#endif
