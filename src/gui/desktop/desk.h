#ifndef DESK_H
#define DESK_H

#include <QtWidgets>
#include <vector>

#include "../../brick_game/api/api.h"

#define HEIGHT 20
#define WIDTH 10

namespace brick_game {
/// Describes widget of labels: score, level, ...
class GameLabel : public QLabel {
 public:
  GameLabel(const QString &s, QWidget *w = 0);

 public:
  void setValue(int val);  ///< Updates value in label.

 private:
  QString _title;
};

/// DEscribes widget of game-field-block.
class FieldBlock final : public QLabel {
 public:
  FieldBlock(QWidget *w = 0);

 public:
  void updateStyle(int s);  ///< Updates color of the block.
};

/// Describes widget of game-application.
class DeskViewer final : public QWidget {
  Q_OBJECT
 public:
  DeskViewer(QWidget *w = 0);

 public:
  void viewGameInfo(const GameInfo_t info);  ///< Draws a game frame
  void viewGameOver(bool win);               ///< Draws gameover message
 private:
  int _rows;
  int _cols;
  std::vector<std::vector<FieldBlock *>> _blocks;
  std::vector<std::vector<FieldBlock *>> _nextBlocks;

  GameLabel *_score;
  GameLabel *_highScore;
  GameLabel *_level;
  QLabel *_nxt;
  QLabel *_pause;
  QWidget *_next;
  QLabel *_gameover;

  QHBoxLayout *_mainLayout;
  QGridLayout *_fieldLayout;
  QVBoxLayout *_infoLayout;
  QGridLayout *_nextLayout;

 private:
  QHBoxLayout *getMainLayout();
  QGridLayout *getFieldLayout();
  QVBoxLayout *getInfoLayout();
  QGridLayout *getNextLayout();
  QWidget *getNext();
};

}  // namespace brick_game

#endif
