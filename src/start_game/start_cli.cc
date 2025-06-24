#include "../brick_game/snake/snake.h"
#include "../brick_game/tetris/tetrisModel.h"
#include "../controller/cli_controller.h"
#include "../gui/cli/view.h"

/// @brief Game selection menu
int main(void) {
  brick_game::CliView view;
  view.startWindow();
  int act = view.getAct();
  while (act != 'q' && act != 'Q') {
    if (act == 's' || act == 'S') {
      brick_game::CliController<brick_game::SnakeModel, brick_game::CliView>
          ctrlr(&view);
      ctrlr.run();
    } else if (act == 't' || act == 'T') {
      brick_game::CliController<brick_game::TetrisModel, brick_game::CliView>
          ctrlr(&view);
      ctrlr.run();
    }
    view.startWindow();
    act = view.getAct();
  }
  return 0;
}