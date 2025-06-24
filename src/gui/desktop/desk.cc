#include "desk.h"

namespace brick_game {
GameLabel::GameLabel(const QString &s, QWidget *w) : QLabel(w), _title(s) {
  QFont newFont("Terminal", 13, QFont::Bold);
  setFont(newFont);
  setText(_title.arg(0));
}

void GameLabel::setValue(int val) { setText(_title.arg(QString::number(val))); }

FieldBlock::FieldBlock(QWidget *w) : QLabel(w) {
  setStyleSheet("QLabel {background-color: white;}");
  setFixedSize(20, 20);
}

void FieldBlock::updateStyle(int key) {
  static const std::map<int, QString> keyColor = {{Block_t::Empty, "white"},
                                                  {Block_t::Filled, "blue"},
                                                  {Block_t::Head, "green"},
                                                  {Block_t::Apple, "red"}};

  QString color = keyColor.at(key);
  setStyleSheet(QString("QLabel {background-color: %1;}").arg(color));

  setFrameStyle(QFrame::Box | QFrame::Plain);
  setLineWidth(1);
}

DeskViewer::DeskViewer(QWidget *w)
    : QWidget(w), _rows(HEIGHT), _cols(WIDTH), _blocks(HEIGHT), _nextBlocks(4) {
  _mainLayout = getMainLayout();
  setStyleSheet("background-color: gray;");
  setLayout(_mainLayout);
}

QHBoxLayout *DeskViewer::getMainLayout() {
  _fieldLayout = getFieldLayout();
  _infoLayout = getInfoLayout();
  _mainLayout = new QHBoxLayout();
  _mainLayout->setSpacing(0);
  _mainLayout->addLayout(_fieldLayout);
  _mainLayout->addLayout(_infoLayout);
  return _mainLayout;
}

QGridLayout *DeskViewer::getFieldLayout() {
  QGridLayout *l = new QGridLayout();
  l->setSpacing(0);

  for (int i = 0; i < _rows; ++i) {
    for (int j = 0; j < _cols; ++j) {
      FieldBlock *b = new FieldBlock();
      _blocks[i].push_back(b);
      l->addWidget(b, i, j);
    }
  }

  return l;
}

QGridLayout *DeskViewer::getNextLayout() {
  QGridLayout *l = new QGridLayout();
  l->setSpacing(0);

  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      FieldBlock *b = new FieldBlock();
      _nextBlocks[i].push_back(b);
      l->addWidget(b, i, j);
    }
  }

  return l;
}

QVBoxLayout *DeskViewer::getInfoLayout() {
  QVBoxLayout *l = new QVBoxLayout();
  _nxt = new QLabel(QString("NEXT"));
  _pause = new QLabel();
  _gameover = new QLabel();
  QFont newFont("Terminal", 13, QFont::Bold);
  _nxt->setFont(newFont);
  _pause->setFont(newFont);
  _gameover->setFont(newFont);
  _next = getNext();
  _score = new GameLabel(QString("SCORE: %0"));
  _highScore = new GameLabel(QString("HIGH SCORE: %0"));
  _level = new GameLabel(QString("LEVEL: %0"));
  l->setSpacing(0);
  l->setContentsMargins(0, 0, 0, 0);
  l->addWidget(_nxt);
  l->addWidget(_next);
  l->addWidget(_score);
  l->addWidget(_highScore);
  l->addWidget(_level);
  l->addWidget(_pause);
  l->addWidget(_gameover);
  return l;
}

void DeskViewer::viewGameInfo(const GameInfo_t info) {
  for (int i = 0; i < _rows; ++i) {
    for (int j = 0; j < _cols; ++j) {
      _blocks[i][j]->updateStyle(info.field[i][j]);
    }
  }

  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      _nextBlocks[i][j]->updateStyle(info.next[i][j]);
    }
  }

  _highScore->setValue(info.high_score);
  _score->setValue(info.score);
  _level->setValue(info.level);
  if (info.pause)
    _pause->setText("Game is paused.");
  else
    _pause->setText("");
}

void DeskViewer::viewGameOver(bool win) {
  if (win)
    _gameover->setText("You won!");
  else
    _gameover->setText("Game over.");
}

QWidget *DeskViewer::getNext() {
  _nextLayout = getNextLayout();

  QWidget *w = new QWidget();
  w->setLayout(_nextLayout);
  w->setFixedSize(100, 100);

  return w;
}
}  // namespace brick_game