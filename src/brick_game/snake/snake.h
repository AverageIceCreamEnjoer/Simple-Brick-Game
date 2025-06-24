#ifndef _SNAKE_H_
#define _SNAKE_H_

#include <chrono>
#include <deque>
#include <iostream>
#include <random>
#include <set>

#include "../api/api.h"

#define HIGH_SCORE_FILE "snake_high_score.txt"
#define ACTION_BUTTONS 8
#define HEIGHT 20
#define WIDTH 10
#define START_SPEED 300

namespace brick_game {

/// @brief enum of directions
typedef enum { L, R, U, D } Direction_t;

/// @brief Class of the Snake game
class SnakeModel : public Model {
 public:
  int **_field;
  std::set<std::pair<int, int>> _pixels;
  GameState_t _state;
  GameInfo_t _info;
  UserAction_t _act;
  std::deque<std::pair<int, int>> _snake;
  std::pair<int, int> _apple;
  Direction_t _direction;
  std::chrono::steady_clock::time_point _timer;
  std::chrono::steady_clock::time_point _bstimer;
  bool _won;

  int getRandNum(int d);

 public:
  SnakeModel();
  ~SnakeModel();

  void sigact()
      override;  ///< Function that should be called every tick of the game
  GameState_t getState() override;  ///< Returns the game state
  bool ticked()
      override;  ///< Returns whether the game's speed-dependent tick has passed
  void userInput(int act) override;  ///< Accepts the player's action
  GameInfo_t updateCurrentState()
      override;  ///< Updates and returns information needed to render the game
  bool isWin() override;

 public:
  int readHighScore();
  void writeHighScore();
  void shift();
  void boost();
  std::pair<int, int> getRandom();
  void startGame();
  void spawnApple();
  void gameOver();
  void move();
  void attaching();
  void pauseGame();
  bool isCollided();
  bool opposites(Direction_t x);
};

}  // namespace brick_game

#endif