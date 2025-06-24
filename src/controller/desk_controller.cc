#include "desk_controller.h"

namespace brick_game {
DeskController::DeskController(Model *model, QWidget *w)
    : _model(model), QWidget(w) {
  QVBoxLayout *l = new QVBoxLayout();
  l->addWidget(&_viewer);
  setLayout(l);
  setFixedSize(400, 400);
  QObject::connect(&_timer, SIGNAL(timeout()), this, SLOT(viewGame()));
  _timer.start(500);
}

void DeskController::viewGame() {
  _model->sigact();
  GameInfo_t info = _model->updateCurrentState();
  _viewer.viewGameInfo(info);
  _timer.setInterval(((double)info.speed) / 1000);
  if (_model->getState() == GameState_t::GAMEOVER)
    _viewer.viewGameOver(_model->isWin());
}

void DeskController::keyPressEvent(QKeyEvent *e) {
  int act = e->key();
  sendEvent(act);
}

void DeskController::paintEvent(QPaintEvent *e) {
  Q_UNUSED(e);
  _viewer.viewGameInfo(_model->updateCurrentState());
}

void DeskController::sendEvent(int act) {
  if (act == Qt::Key_Return)
    act = 10;
  else if (act == Qt::Key_Up)
    act = 259;
  else if (act == Qt::Key_Down)
    act = 258;
  else if (act == Qt::Key_Left)
    act = 260;
  else if (act == Qt::Key_Right)
    act = 261;
  else if (act == Qt::Key_Escape)
    act = 27;
  _model->userInput(act);
  _model->sigact();
  if (_model->getState() == GameState_t::GAMEOVER) _timer.stop();
}
}  // namespace brick_game