#ifndef _CLI_VIEW_H_
#define _CLI_VIEW_H_

#include <locale.h>
#include <ncurses.h>
#include <string.h>

#include <string>

#include "../../brick_game/api/api.h"

namespace brick_game {

/// @brief Draws a game in the terminal
class CliView {
 private:
  void print_rectangle(int top_y, int bottom_y, int left_x, int right_x);
  void viewOverlay();

 public:
  CliView();
  ~CliView();

  void startWindow();              ///< Draws the game selection menu
  void viewClear();                ///< Clears the field
  void viewGame(GameInfo_t info);  ///< Draws the game
  void gameOver(bool win);         ///< Draws gameover message
  int getAct();                    ///< Returns user actions
};

}  // namespace brick_game

#define MAIN_FIELD_WIDTH 20
#define MAIN_FIELD_HEIGHT 20

#define NEXT_FIELD_WIDTH 10
#define NEXT_FIELD_HEIGHT 5

#define START_MESSAGE "Press enter to start game"

#define MAX_FIGURE_SIZE 4

#define GO_MESSAGE "Game Over."
#define GO_MESSAGE_LEN 10
#define GET_USER_INPUT getch()

#define PRINT_FROG(x, y) mvprintw(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), "@")
#define MVPRINTW(y, x, ...) \
  mvprintw(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), __VA_ARGS__)
#define MVADDCH(y, x, c) mvaddch(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), c)
#define CLEAR_BACKPOS(y, x) mvaddch(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), ' ')

#define INTRO_MESSAGE_LEN 21

#define BOARDS_BEGIN 2

#define SUCCESS 0
#define ERROR 1

#define NO_INPUT -1

#define ESCAPE 27
#define ENTER_KEY 10

#endif