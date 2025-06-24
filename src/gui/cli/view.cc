#include "view.h"

namespace brick_game {

CliView::CliView() {
  initscr();
  noecho();
  clear();
  curs_set(0);
  keypad(stdscr, TRUE);
  timeout(5);
  setlocale(LC_ALL, "");
}

CliView::~CliView() { endwin(); }

void CliView::viewClear() { clear(); }

void CliView::startWindow() {
  viewOverlay();
  MVPRINTW(NEXT_FIELD_HEIGHT + 10, MAIN_FIELD_WIDTH + 2,
           "Press \'s\' to play snake");
  MVPRINTW(NEXT_FIELD_HEIGHT + 12, MAIN_FIELD_WIDTH + 2,
           "Press \'t\' to play tetris");
  MVPRINTW(NEXT_FIELD_HEIGHT + 14, MAIN_FIELD_WIDTH + 2,
           "Press \'q\' to quit game");
}

void CliView::viewOverlay() {
  print_rectangle(0, MAIN_FIELD_HEIGHT + 1, 0, MAIN_FIELD_WIDTH + 1);
  print_rectangle(0, NEXT_FIELD_HEIGHT + 9, MAIN_FIELD_WIDTH + 2,
                  MAIN_FIELD_WIDTH + NEXT_FIELD_WIDTH + 6);

  print_rectangle(6, 6 + NEXT_FIELD_HEIGHT, MAIN_FIELD_WIDTH + 3,
                  MAIN_FIELD_WIDTH + NEXT_FIELD_WIDTH + 3);

  MVPRINTW(1, MAIN_FIELD_WIDTH + 3, "SCORE");
  MVPRINTW(3, MAIN_FIELD_WIDTH + 3, "HI-SCORE");
  MVPRINTW(5, MAIN_FIELD_WIDTH + 2 + (NEXT_FIELD_WIDTH / 2), "NEXT");
  MVPRINTW(NEXT_FIELD_HEIGHT + 7, MAIN_FIELD_WIDTH + 3, "LEVEL");
}

void CliView::print_rectangle(int top_y, int bottom_y, int left_x,
                              int right_x) {
  MVADDCH(top_y, left_x, ACS_ULCORNER);

  int i = left_x + 1;

  for (; i < right_x; i++) MVADDCH(top_y, i, ACS_HLINE);
  MVADDCH(top_y, i, ACS_URCORNER);

  for (int i = top_y + 1; i < bottom_y; i++) {
    MVADDCH(i, left_x, ACS_VLINE);
    MVADDCH(i, right_x, ACS_VLINE);
  }

  MVADDCH(bottom_y, left_x, ACS_LLCORNER);
  i = left_x + 1;
  for (; i < right_x; i++) MVADDCH(bottom_y, i, ACS_HLINE);
  MVADDCH(bottom_y, i, ACS_LRCORNER);
}

void CliView::viewGame(GameInfo_t info) {
  const static char* blocks[4] = {"..", "{}", "[]", "()"};
  viewOverlay();
  for (size_t i = 0; i < MAX_FIGURE_SIZE; i++)
    for (size_t j = 0; j < MAX_FIGURE_SIZE; j++)
      if (info.next[i][j]) {
        MVADDCH(7 + i, MAIN_FIELD_WIDTH + 4 + j * 2, blocks[2][0]);
        MVADDCH(7 + i, MAIN_FIELD_WIDTH + 5 + j * 2, blocks[2][1]);
      }
  for (size_t i = 0; i < MAIN_FIELD_HEIGHT; i++)
    for (size_t j = 0; j < MAIN_FIELD_WIDTH / 2; j++) {
      MVADDCH(i + 1, j * 2 + 1, blocks[info.field[i][j]][0]);
      MVADDCH(i + 1, j * 2 + 2, blocks[info.field[i][j]][1]);
    }
  MVPRINTW(2, MAIN_FIELD_WIDTH + 3, "%d", info.score);
  MVPRINTW(4, MAIN_FIELD_WIDTH + 3, "%d", info.high_score);
  MVPRINTW(NEXT_FIELD_HEIGHT + 8, MAIN_FIELD_WIDTH + 3, "%d", info.level);
  if (info.pause)
    MVPRINTW(NEXT_FIELD_HEIGHT + 12, MAIN_FIELD_WIDTH + 2, "Game is paused.");
}

void CliView::gameOver(bool win) {
  if (win)
    MVPRINTW(MAIN_FIELD_HEIGHT / 2,
             (MAIN_FIELD_WIDTH - GO_MESSAGE_LEN - 2) / 2 + 1, "You won!");
  else
    MVPRINTW(MAIN_FIELD_HEIGHT / 2, (MAIN_FIELD_WIDTH - GO_MESSAGE_LEN) / 2 + 1,
             GO_MESSAGE);
  timeout(-1);
  getch();
  viewClear();
}

int CliView::getAct() {
  timeout(5);
  return getch();
}

}  // namespace brick_game