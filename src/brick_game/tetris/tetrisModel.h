#ifndef _TETRIS_H_
#define _TETRIS_H_

#include <chrono>
#include <iostream>
#include <random>

#include "../api/api.h"

#define TETRIS_HIGH_SCORE_FILE "tetris_high_score.txt"
#define ACTION_BUTTONS 8
#define HEIGHT 20
#define WIDTH 10
#define PLAYER_SPEED 100
#define DOWN_SPEED 500

namespace brick_game {

/// @brief Class of the Tetris game
class TetrisModel : public Model {
 public:
  class Figure {
   public:
    void F1();
    void F2();
    void F3();
    void F4();
    void F5();
    void F6();
    void F7();

   public:
    Figure(bool random = false);
    Figure(const Figure& other);
    ~Figure();

    Figure operator=(const Figure& other);

    void update();

    unsigned int height, width;
    int x, y;
    int** field;
  };

  GameInfo_t _info;
  GameState_t _state;
  UserAction_t _act;
  Figure _current, _next;
  int** _field;
  std::chrono::steady_clock::time_point _timer;
  std::chrono::steady_clock::time_point _timer_down;
  bool _won;

  int readHighScore();

  void start();
  void spawn();
  void move();
  void attaching();
  void pause();
  void gameOver();

  void moveFigure();
  bool canMoveFig();
  bool isCollided(Figure figure);
  bool isAttached();
  void shift(Figure& figure);
  void connectToField();

  int deleteRows();

 public:
  TetrisModel();

  ~TetrisModel();

  void sigact()
      override;  ///< Function that should be called every tick of the game

  void userInput(int act) override;  ///< Accepts the player's action

  GameState_t getState() override;  ///< Returns the game state

  bool ticked()
      override;  ///< Returns whether the game's speed-dependent tick has passed

  GameInfo_t updateCurrentState()
      override;  ///< Updates and returns information needed to render the game

  bool isWin() override;
};
}  // namespace brick_game

#endif