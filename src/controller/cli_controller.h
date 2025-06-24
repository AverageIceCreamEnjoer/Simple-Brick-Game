#ifndef _CLI_CONTROLLER_
#define _CLI_CONTROLLER_

#include <chrono>

#include "../brick_game/api/api.h"

namespace brick_game {
/// @brief Controller class
/// @tparam Model of the game
/// @tparam View which renders the game
template <class Model, class View>
class CliController {
 private:
  Model _model;
  View *_view;

 public:
  CliController(View *v) : _model(), _view(v) {}

  ~CliController() { _view = nullptr; }

  /// @brief Run the game
  void run() {
    while (_model.getState() != GAMEOVER) {
      _view->viewClear();
      _model.sigact();
      _view->viewGame(_model.updateCurrentState());
      auto start = std::chrono::steady_clock::now();
      auto end = start;
      int act = -1, tmp;
      while (!_model.ticked()) {
        tmp = _view->getAct();
        if (tmp > 0 && act == -1) act = tmp;
        end = std::chrono::steady_clock::now();
      }
      _model.userInput(act);
    }
    _view->gameOver(_model.isWin());
  }
};
}  // namespace brick_game

#endif