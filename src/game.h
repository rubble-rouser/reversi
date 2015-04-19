#ifndef GAME_H
#define GAME_H

#include "ai.h"
#include "board.h"

class Game {
  public:
    Game();
    ~Game();
    Board* getBoard();
    void click(int row, int col);
    int flipCount(int row, int col);
    int getCurrentTurn();
    bool isOver();
    int blackScore();
    int whiteScore();
    void setAI(bool on);
    void playAI();
    bool getAIOn();

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
