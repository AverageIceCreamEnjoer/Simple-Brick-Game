#include <QtWidgets>

#include "../brick_game/snake/snake.h"
#include "../brick_game/tetris/tetrisModel.h"
#include "../controller/desk_controller.h"

namespace brick_game {
/// @brief Describes the game selection menu
class Desk : public QWidget {
 private:
  QHBoxLayout *_mainLayout;
  QLabel *_menu;
  Model *_model;
  DeskController *_controller;

 public:
  Desk(QWidget *w = 0)
      : QWidget(w),
        _menu(nullptr),
        _controller(nullptr),
        _model(nullptr),
        _mainLayout(nullptr) {
    initMenu();
    setFixedSize(450, 450);
    setStyleSheet("background-color: gray;");
  }

  /// @brief Processes user action
  /// @param e user action
  void keyPressEvent(QKeyEvent *e) {
    int key = e->key();
    if (_model != nullptr && _model->getState() == GameState_t::GAMEOVER) {
      delete _model;
      _model = nullptr;
      delete _controller;
      _controller = nullptr;
      setNewLayout(_menu);
    } else if (_model != nullptr)
      _controller->keyPressEvent(e);
    else if (tolower(key) == 's') {
      _model = new SnakeModel();
      _controller = new DeskController(_model);
      setNewLayout(_controller);
    } else if (tolower(key) == 't') {
      _model = new TetrisModel();
      _controller = new DeskController(_model);
      setNewLayout(_controller);
    } else if (tolower(key) == 'q')
      QApplication::quit();
  }

 private:
  void initMenu() {
    delete _menu;
    _menu = new QLabel();
    QFont newFont("Terminal", 13, QFont::Bold);
    _menu->setFont(newFont);
    _menu->setText(
        "Press 't' - Start tetris\nPress 's' - Start snake\nPress 'q' - Quit");
    setNewLayout(_menu);
  }

  void setNewLayout(QWidget *w) {
    if (_mainLayout != nullptr) delete _mainLayout;
    _mainLayout = new QHBoxLayout();
    _mainLayout->addWidget(w);
    _mainLayout->setAlignment(Qt::AlignCenter);
    setLayout(_mainLayout);
  }
};
}  // namespace brick_game

int main(int argc, char **argv) {
  QApplication app(argc, argv);
  brick_game::Desk game;
  game.show();
  return app.exec();
}
