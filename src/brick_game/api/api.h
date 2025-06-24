//    \file File that the interface accesses
#ifndef _API_H_
#define _API_H_

#define STATES 5

namespace brick_game {

//    \brief enum of user actions
typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

//  \brief enum of block types
typedef enum { Empty, Head, Filled, Apple } Block_t;

//    \brief Struct that contains information about game for user interface
typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

//    \brief enum of game states
typedef enum { START = 0, PAUSE, SPAWN, MOVING, GAMEOVER } GameState_t;

// \brief Parent class of game models
class Model {
 public:
  Model() {}
  virtual ~Model() = default;

  virtual GameInfo_t
  updateCurrentState() = 0;  ///< Updates and returns information needed to
                             ///< render the game
  virtual void
  sigact() = 0;  ///< Function that should be called every tick of the game
  virtual void userInput(int act) = 0;  ///< Accepts the player's action
  virtual GameState_t getState() = 0;   ///< Returns the game state
  virtual bool
  ticked() = 0;  ///< Returns whether the game's speed-dependent tick has passed
  virtual bool isWin() = 0;
};
}  // namespace brick_game
#endif