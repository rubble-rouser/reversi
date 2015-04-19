#ifndef GAME_H
#define GAME_H

#include "ai.h"
#include "board.h"

class Game {
  public:
    Game();
    ~Game();
    void setAI(bool on);
    Board* getBoard();
    int getCurrentTurn();
    bool getAIOn();
    bool isOver();
    int blackScore();
    int whiteScore();
    void playAI();
    void click(int row, int col);
    int flipCount(int row, int col);

  private:
    Board* board;
    AI* ai;
    int currentTurn;
    int blacks;
    int whites;
    bool gameOver;
    bool skipped;
    bool aiOn;
    void nextTurn();
    bool isValidMove(int row, int col);
    void flip(int row, int col);
    void possibleMoves();
};

#endif
