#include "board.h"

// Internal representation of the state of the board.
//
//   -1 = valid move (as determined by `Game::possibleMoves`)
//    0 = empty space, invalid move
//    1 = white piece
//    2 = black piece
Board::Board() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      board[i][j] = 0;
    }
  }

  // Initial board begins with 2 blacks and 2 whites at the center. Blacks are
  // placed at north-west and south-east. Whites are placed at north-east and
  // south-west.
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
