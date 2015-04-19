#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <SFML/Graphics.hpp>
#include "game.h"

void delay(int seconds) {
  clock_t temp;
  temp = clock () + seconds * CLOCKS_PER_SEC;
  while (clock() < temp) {}
}

static inline std::string int2Str(int x) {
  std::stringstream type;
  type << x;
  return type.str();
}

int main() {
  Game* game = new Game;

  float resizeWidth = 1;
  float resizeHeight = 1;

  // Given that the padding between each square is 5, and the padding
  // between the edge squares and the window frame is also 5, the amount of
  // space taken up by padding is 9 * 5 = 45. For a 800x800 window, and an 8x8
  // grid, the remaining space for each tile is:
  const double size = (800 - 45) / 8.0;

  sf::RenderWindow window(sf::VideoMode(1000, 800), "REVERSI");
  window.setFramerateLimit(30);

  sf::Texture tile1;
  sf::Texture tile2;
  sf::Texture piece1;
  sf::Texture piece2;
  sf::Texture glow;
  sf::Texture wood;
  sf::Texture button;
  if (!tile1.loadFromFile("imgs/tile1.jpg")
      || !tile2.loadFromFile("imgs/tile2.jpg")
      || !piece1.loadFromFile("imgs/piece1.png")
      || !piece2.loadFromFile("imgs/piece2.png")
      || !glow.loadFromFile("imgs/glow.png")
      || !wood.loadFromFile("imgs/wood.jpg")
      || !button.loadFromFile("imgs/button.png")) {
    exit(1);
  }

  glow.setSmooth(true);

  sf::RectangleShape squares[8][8];
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      sf::RectangleShape square(sf::Vector2f(size, size));

      if (i == 0 && j % 2 == 1) {
        square.setTexture(&tile2);
      }
      else if (i == 0 && j % 2 == 0) {
        square.setTexture(&tile1);
      }
      else if (i % 2 == 1 && j % 2 == 1) {
        square.setTexture(&tile1);
      }
      else if (i % 2 == 0 && j % 2 == 1) {
        square.setTexture(&tile2);
      }
      else if (i % 2 == 1 && j % 2 == 0) {
        square.setTexture(&tile2);
      }
      else if (i % 2 == 0 && j % 2 == 0) {
        square.setTexture(&tile1);
      }

      square.setPosition(i * (size + 1) + 5, j * (size + 1) + 5);
      squares[i][j] = square;
    }
  }

  sf::Font font;
  font.loadFromFile("arial.ttf");

  sf::Text currentTurnText("Current turn:", font);
  currentTurnText.setCharacterSize(14);
  currentTurnText.setPosition(850, 75);

  sf::Text turn("BLACK", font);
  turn.setCharacterSize(30);
  turn.setPosition(850, 100);

  sf::Text scoreText("Score: \nBlack-White",font);
  scoreText.setPosition(850, 150);
  scoreText.setCharacterSize(16);

  sf::Text scores("-", font);
  scores.setCharacterSize(30);
  scores.setPosition(850, 200);

  sf::Text gameVersusText("New game with:", font);
  gameVersusText.setCharacterSize(16);
  gameVersusText.setPosition(810, 450);

  sf::Text buttonText1("AI", font);
  buttonText1.setPosition(865, 515);
  buttonText1.setColor(sf::Color::Black);

  sf::Text buttonText2("Player", font);
  buttonText2.setPosition(845, 635);
  buttonText2.setColor(sf::Color::Black);

  sf::RectangleShape background(sf::Vector2f(400, 770));
  sf::RectangleShape scorebg(sf::Vector2f(180, 300));
  sf::RectangleShape button1(sf::Vector2f(180, 100));
  sf::RectangleShape button2(sf::Vector2f(180, 100));

  background.setPosition(767, 0);
  background.setTexture(&wood);
  scorebg.setPosition(797, 10);
  button1.setPosition(797, 480);
  button2.setPosition(797, 600);
  scorebg.setFillColor(sf::Color(0, 0, 0, 125));
  button1.setTexture(&button);
  button2.setTexture(&button);

  while (window.isOpen()) {
    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
      else if (event.type == sf::Event::Resized) {
        resizeWidth = event.size.width / 1000.0;
        resizeHeight = event.size.height / 800.0;
      }
    }

    if (game->getAIOn() && game->getCurrentTurn() == 1 && !game->isOver()) {
      delay(1);
      game->playAI();
    }

    window.clear();

    sf::Vector2i localPosition = sf::Mouse::getPosition(window);
    int row = (localPosition.y / resizeHeight) / int(size + 5);
    int col = (localPosition.x / resizeWidth) / int(size + 5);

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && window.hasFocus()) {
      if (localPosition.x > 797 * resizeWidth
          && localPosition.x < 977 * resizeWidth) {
        if (localPosition.y > 480 * resizeHeight
            && localPosition.y < 580 * resizeHeight) {
          delete game;
          game = new Game();
          game->setAI(true);
        }
        else if (localPosition.y > 600 * resizeHeight
                 && localPosition.y < 700 * resizeHeight) {
          delete game;
          game = new Game();
          game->setAI(false);
        }
      }

      game->click(col, row);
    }

    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {

        window.pushGLStates();
        window.draw(squares[i][j]);
        window.popGLStates();

        float radius = (size - 10) / 2;
        float smallRadius = (size - 10) / 5;

        if (game->getBoard()->get(i, j) > 0) {
          sf::CircleShape circle(radius);
          piece1.setSmooth(true);
          piece2.setSmooth(true);
          circle.setTexture(game->getBoard()->get(i, j) == 1
            ? &piece1
            : &piece2);
          circle.setPosition(i * (size + 1) + 10, j * (size + 1) + 10);
          window.pushGLStates();
          window.draw(circle);
          window.popGLStates();
        }

        // Display small circles on possible moves if it is not white AI's turn.
        else if (game->getBoard()->get(i, j) == -1
                 && !(game->getAIOn()
                 && game->getCurrentTurn() == 1)) {
          sf::CircleShape circle2(smallRadius);
          circle2.setTexture(&glow);
          circle2.setFillColor(sf::Color(255, 255, 255));
          circle2.setPosition(i * (size + 1) + 35, j * (size + 1) + 35);
          window.pushGLStates();
          window.draw(circle2);
          window.popGLStates();
        }
      }
    }

    turn.setPosition(850, 100);
    turn.setCharacterSize(30);
    turn.setColor(sf::Color::White);
    turn.setStyle(sf::Text::Regular);

    if (game->isOver()) {
      if (game->whiteScore() > game->blackScore()) {
        turn.setString("GAME OVER:\n WHITE WON");
      }
      else if (game->whiteScore() < game->blackScore()) {
        turn.setString("GAME OVER:\n BLACK WON");
      }
      else {
        turn.setString("GAME OVER:\n TIED");
      }

      turn.setPosition(200, 300);
      turn.setCharacterSize(72);
      turn.setColor(sf::Color::Red);
      turn.setStyle(sf::Text::Bold);
    }
    else if (game->getCurrentTurn() == 2) {
      turn.setString("BLACK");
    }
    else {
      turn.setString("WHITE");
    }

    scores.setString(int2Str(game->blackScore()) + "-" + int2Str(game->whiteScore()));

    window.pushGLStates();
    window.draw(background);
    window.draw(scorebg);
    window.draw(turn);
    window.draw(scores);
    window.draw(button1);
    window.draw(button2);
    window.draw(gameVersusText);
    window.draw(currentTurnText);
    window.draw(scoreText);
    window.draw(buttonText1);
    window.draw(buttonText2);
    window.popGLStates();

    window.display();
  }

  return 0;
}
