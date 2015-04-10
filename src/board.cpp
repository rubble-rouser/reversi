#include "board.h"

Board::Board() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      board[i][j] = 0;
    }
  }

  board[3][3] = 2;
  board[4][4] = 2;
  board[3][4] = 1;
  board[4][3] = 1;
}

int Board::get(int row, int col) {
  return board[row][col];
}

void Board::set(int row, int col, int value) {
  board[row][col] = value;
}
