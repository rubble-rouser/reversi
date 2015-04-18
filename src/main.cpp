#include <iostream>
#include <sstream>
#include <string>
#include <SFML/Graphics.hpp>
#include "game.h"

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

  sf::RectangleShape squares[8][8];
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      sf::RectangleShape square(sf::Vector2f(size, size));
      square.setFillColor(sf::Color::Green);
      square.setPosition(i * (size + 5) + 5, j * (size + 5) + 5);
      squares[i][j] = square;
    }
  }

  sf::Font font;
  font.loadFromFile("arial.ttf");

  sf::Text currentTurnText("Current turn:", font);
  currentTurnText.setCharacterSize(14);
  currentTurnText.setPosition(850,75);

  sf::Text turn("BLACK", font);
  turn.setCharacterSize(30);
  turn.setPosition(850, 100);

  sf::Text scoreText("Score: \nBlack-White",font);
  scoreText.setPosition(850,150);
  scoreText.setCharacterSize(16);

  sf::Text scores("——", font);
  scores.setCharacterSize(30);
  scores.setPosition(850, 200);

  sf::Text gameVersusText("New game with:", font);
  gameVersusText.setCharacterSize(16);
  gameVersusText.setPosition(810,475);

  sf::Text buttonText1("AI", font);
  buttonText1.setPosition(820,510);

  sf::Text buttonText2 ("Player", font);
  buttonText2.setPosition(820,610);

  sf::RectangleShape button1(sf::Vector2f(180, 100));
  sf::RectangleShape button2(sf::Vector2f(180, 100));

  button1.setPosition(810, 500);
  button2.setPosition(810, 600);
  button1.setFillColor(sf::Color::Blue);
  button2.setFillColor(sf::Color::Red);

  while (window.isOpen()) {
    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
      else if (event.type == sf::Event::Resized){
        resizeWidth = event.size.width / 1000.0;
        resizeHeight = event.size.height / 800.0;
      }
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


        if (game->getBoard()->get(i, j) > 0) {
          sf::CircleShape circle(radius);
          circle.setFillColor(game->getBoard()->get(i, j) == 1
            ? sf::Color::White
            : sf::Color::Black);
          circle.setPosition(i * (size + 5) + 10, j * (size + 5) + 10);
          window.pushGLStates();
          window.draw(circle);
          window.popGLStates();
        }
        else if (game->getBoard()->get(i, j) == -1) {
          sf::CircleShape circle(radius);
          circle.setFillColor(sf::Color(125, 125, 125));
          circle.setPosition(i * (size + 5) + 10, j * (size + 5) + 10);
          window.pushGLStates();
          window.draw(circle);
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
