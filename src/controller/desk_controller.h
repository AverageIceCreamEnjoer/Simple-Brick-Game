#ifndef DESK_CONTROLLER_H
#define DESK_CONTROLLER_H

#include <QtWidgets>

#include "../brick_game/api/api.h"
#include "../gui/desktop/desk.h"

namespace brick_game {
/// @brief Describes desktop controller in Qt
class DeskController : public QWidget {
  Q_OBJECT

 public:
  DeskController(Model *model, QWidget *w = 0);
 public slots:
  void viewGame();  ///< Draws a frame from signal

 private:
  Model *_model;
  DeskViewer _viewer;
  QTimer _timer;

 public:
  void keyPressEvent(QKeyEvent *e);  ///< Processes user action
  void sendEvent(int act);           ///< Send users action to game model
  void paintEvent(QPaintEvent *e);   ///< Redrawing view
};
}  // namespace brick_game
#endif