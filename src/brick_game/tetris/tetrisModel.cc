#include "tetrisModel.h"

namespace brick_game {

TetrisModel::Figure::Figure(bool random) : x(3), y(0) {
  field = new int *[4 * 4 + 4];
  for (int i = 0; i < 4; ++i) {
    field[i] = (int *)(field + 4 + 4 * i);
    for (int j = 0; j < 4; ++j) field[i][j] = 0;
  }
  if (random) {
    update();
  } else {
    width = 4;
    height = 4;
  }
}

TetrisModel::Figure::~Figure() { delete[] field; }

void TetrisModel::Figure::update() {
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j) field[i][j] = 0;
  typedef void (Figure::*init_f)();
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dist(0, 6);
  int n = dist(gen);
  const static init_f init[] = {
      &TetrisModel::Figure::F1, &TetrisModel::Figure::F2,
      &TetrisModel::Figure::F3, &TetrisModel::Figure::F4,
      &TetrisModel::Figure::F5, &TetrisModel::Figure::F6,
      &TetrisModel::Figure::F7};
  (this->*init[n])();
}

void TetrisModel::Figure::F1() {
  static const int g[2][2] = {{1, 1}, {1, 1}};
  for (int i = 0; i < 2; ++i)
    for (int j = 0; j < 2; ++j) field[i][j] = g[i][j];
  height = 2;
  width = 2;
}

void TetrisModel::Figure::F2() {
  static const int g[3][3] = {{0, 1, 1}, {0, 1, 0}, {0, 1, 0}};
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j) field[i][j] = g[i][j];
  height = 3;
  width = 3;
}

void TetrisModel::Figure::F3() {
  static const int g[3][3] = {{1, 1, 0}, {0, 1, 0}, {0, 1, 0}};
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j) field[i][j] = g[i][j];
  height = 3;
  width = 3;
}

void TetrisModel::Figure::F4() {
  static const int g[4][4] = {
      {0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}};
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j) field[i][j] = g[i][j];
  height = 4;
  width = 4;
}

void TetrisModel::Figure::F5() {
  static const int g[3][3] = {{1, 0, 0}, {1, 1, 0}, {0, 1, 0}};
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j) field[i][j] = g[i][j];
  height = 3;
  width = 3;
}

void TetrisModel::Figure::F6() {
  static const int g[3][3] = {{0, 0, 1}, {0, 1, 1}, {0, 1, 0}};
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j) field[i][j] = g[i][j];
  height = 3;
  width = 3;
}

void TetrisModel::Figure::F7() {
  static const int g[3][3] = {{0, 1, 0}, {0, 1, 1}, {0, 1, 0}};
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j) field[i][j] = g[i][j];
  height = 3;
  width = 3;
}

TetrisModel::Figure::Figure(const Figure &other) {
  field = new int *[20];
  x = other.x;
  y = other.y;
  height = other.height;
  width = other.width;
  for (unsigned int i = 0; i < 4; ++i) {
    field[i] = (int *)(field + 4 + 4 * i);
    for (unsigned int j = 0; j < 4; ++j)
      field[i][j] = (i < height && j < width) ? other.field[i][j] : 0;
  }
}

TetrisModel::Figure TetrisModel::Figure::operator=(
    const TetrisModel::Figure &other) {
  x = other.x;
  y = other.y;
  height = other.height;
  width = other.width;
  for (unsigned int i = 0; i < 4; ++i) {
    field[i] = (int *)(field + 4 + 4 * i);
    for (unsigned int j = 0; j < 4; ++j)
      field[i][j] = (i < height && j < width) ? other.field[i][j] : 0;
  }
  return *this;
}

int TetrisModel::readHighScore() {
  int tmp = 0;
  FILE *f = fopen(TETRIS_HIGH_SCORE_FILE, "r");
  if (f) {
    fscanf(f, "%d", &tmp);
    fclose(f);
  }
  return tmp;
}

TetrisModel::TetrisModel()
    : _state(GameState_t::START),
      _act(UserAction_t::Action),
      _current(false),
      _next(true),
      _won(false) {
  _field = new int *[HEIGHT * WIDTH + HEIGHT];
  for (int i = 0; i < HEIGHT; ++i) {
    _field[i] = (int *)(_field + HEIGHT + WIDTH * i);
    for (int j = 0; j < WIDTH; ++j) _field[i][j] = 0;
  }
  _info.field = new int *[HEIGHT * WIDTH + HEIGHT];
  for (int i = 0; i < HEIGHT; ++i) {
    _info.field[i] = (int *)(_info.field + HEIGHT + WIDTH * i);
    for (int j = 0; j < WIDTH; ++j) _info.field[i][j] = 0;
  }
  _info.next = new int *[20];
  for (unsigned int i = 0; i < 4; ++i) {
    _info.next[i] = (int *)(_info.next + 4 + 4 * i);
    for (unsigned int j = 0; j < 4; ++j)
      _info.next[i][j] =
          (i < _next.height && j < _next.width) ? _next.field[i][j] : 0;
  }
  _info.score = 0;
  _info.high_score = readHighScore();
  _info.level = 1;
  _info.pause = 0;
  _info.speed = DOWN_SPEED;
  _timer = std::chrono::steady_clock::now();
  _timer_down = std::chrono::steady_clock::now();
}

TetrisModel::~TetrisModel() {
  FILE *f = fopen(TETRIS_HIGH_SCORE_FILE, "w");
  if (f) {
    fprintf(f, "%d", _info.high_score);
    fclose(f);
  }
  delete[] _field;
  _field = nullptr;
  delete[] _info.field;
  _info.field = nullptr;
  delete[] _info.next;
  _info.next = nullptr;
  _current.field = nullptr;
  _next.field = nullptr;
}

void TetrisModel::moveFigure() {
  if (canMoveFig()) {
    shift(_current);
    if (_act == UserAction_t::Action)
      _timer_down = std::chrono::steady_clock::now();
    _state = GameState_t::MOVING;
  } else if (isCollided(_current))
    _state = GameState_t::GAMEOVER;
  else if (isAttached()) {
    connectToField();
    attaching();
  }
}

bool TetrisModel::canMoveFig() {
  Figure ftest(_current);
  shift(ftest);
  bool result = !isCollided(ftest);
  return result;
}

void TetrisModel::shift(Figure &figure) {
  if (_act == UserAction_t::Left)
    figure.x -= 1;
  else if (_act == UserAction_t::Right)
    figure.x += 1;
  else if (_act == UserAction_t::Start) {
    Figure ftest(figure);
    for (unsigned int i = 0; i < figure.height; ++i)
      for (unsigned int j = 0; j < figure.width; ++j)
        figure.field[j][figure.width - i - 1] = ftest.field[i][j];
  } else
    figure.y += 1;
}

bool TetrisModel::isCollided(Figure figure) {
  bool result = (figure.y >= HEIGHT);
  for (unsigned int i = 0; i < figure.height && !result; ++i)
    for (unsigned int j = 0; j < figure.width && !result; ++j)
      if (figure.field[i][j] != 0) {
        result = (i + figure.y) >= HEIGHT || (j + figure.x) >= WIDTH ||
                 _field[i + figure.y][j + figure.x] != Block_t::Empty;
      }
  return result;
}

bool TetrisModel::isAttached() {
  Figure ftest(_current);
  ftest.y += 1;
  return isCollided(ftest);
}

void TetrisModel::connectToField() {
  for (unsigned int i = 0; i < _current.height; ++i)
    for (unsigned int j = 0; j < _current.width; ++j)
      if (_current.field[i][j] != 0) {
        _field[i + _current.y][j + _current.x] = Block_t::Filled;
      }
}

int TetrisModel::deleteRows() {
  int result = 0;
  for (int i = 0; i < HEIGHT; ++i) {
    bool filled = true;
    for (int j = 0; j < WIDTH && filled; ++j)
      filled = _field[i][j] != Block_t::Empty;
    if (filled) {
      for (int j = i; j > 0; --j)
        for (int k = 0; k < WIDTH; ++k) _field[j][k] = _field[j - 1][k];
      for (int j = 0; j < WIDTH; ++j) _field[0][j] = 0;
      ++result;
    }
  }
  return result;
}

void TetrisModel::sigact() {
  typedef void (TetrisModel::*action)();
  const static action fsm_arr[] = {&TetrisModel::start, &TetrisModel::pause,
                                   &TetrisModel::spawn, &TetrisModel::move,
                                   &TetrisModel::gameOver};
  if (ticked()) {
    (this->*fsm_arr[_state])();
    _timer = std::chrono::steady_clock::now();
  }
}

void TetrisModel::start() {
  if (_act == UserAction_t::Terminate)
    _state = GameState_t::GAMEOVER;
  else if (_act == UserAction_t::Start) {
    _state = GameState_t::SPAWN;
    _act = UserAction_t::Action;
  }
}

void TetrisModel::spawn() {
  _current = _next;
  _next.update();
  _state = GameState_t::MOVING;
}

void TetrisModel::move() {
  if (_act == UserAction_t::Pause) {
    _info.pause = 1;
    _state = GameState_t::PAUSE;
    _act = UserAction_t::Action;
  } else if (_act == UserAction_t::Terminate) {
    _state = GameState_t::GAMEOVER;
  } else if (_act == UserAction_t::Down) {
    _act = UserAction_t::Action;
    while (canMoveFig()) shift(_current);
    moveFigure();
  } else {
    if (std::chrono::duration<double>(std::chrono::steady_clock::now() -
                                      _timer_down)
            .count() > ((double)_info.speed) / 1000) {
      UserAction_t tmp = _act;
      _act = UserAction_t::Action;
      moveFigure();
      _act = tmp;
    }
    if (_act != UserAction_t::Action) {
      moveFigure();
      _act = UserAction_t::Action;
    }
  }
}

void TetrisModel::attaching() {
  int deleted = deleteRows();
  if (deleted == 1)
    _info.score += 100;
  else if (deleted == 2)
    _info.score += 300;
  else if (deleted == 3)
    _info.score += 700;
  else if (deleted == 4)
    _info.score += 1500;
  _state = GameState_t::SPAWN;
  spawn();
}

void TetrisModel::pause() {
  if (_act == UserAction_t::Pause) {
    _info.pause = 0;
    _act = UserAction_t::Action;
    _state = GameState_t::MOVING;
  } else if (_act == UserAction_t::Terminate) {
    _info.pause = 0;
    _act = UserAction_t::Action;
    _state = GameState_t::GAMEOVER;
  }
}

void TetrisModel::userInput(int act) {
  if (act == 259)  // KEY_UP
    _act = UserAction_t::Up;
  else if (act == 258)  // KEY_DOWN
    _act = UserAction_t::Down;
  else if (act == 260)  // KEY_LEFT
    _act = UserAction_t::Left;
  else if (act == 261)  // KEY_RIGHT
    _act = UserAction_t::Right;
  else if (act == 27)  // ESCAPE
    _act = UserAction_t::Terminate;
  else if (act == 10)  // ENTER_KEY
    _act = UserAction_t::Start;
  else if (act == 'p' || act == 'P')
    _act = UserAction_t::Pause;
}

GameState_t TetrisModel::getState() { return _state; }

bool TetrisModel::ticked() {
  return std::chrono::duration<double>(std::chrono::steady_clock::now() -
                                       _timer)
             .count() > ((double)PLAYER_SPEED) / 1000;
}

GameInfo_t TetrisModel::updateCurrentState() {
  for (size_t i = 0; i < HEIGHT; ++i)
    for (size_t j = 0; j < WIDTH; ++j) _info.field[i][j] = _field[i][j];
  for (size_t i = 0; i < 4; ++i)
    for (size_t j = 0; j < 4; ++j) {
      if (_current.field[i][j] != 0)
        _info.field[i + _current.y][j + _current.x] = Block_t::Head;
      _info.next[i][j] = _next.field[i][j];
    }
  _info.high_score =
      (_info.score > _info.high_score) ? _info.score : _info.high_score;
  _info.level = (_info.score / 600 + 1) <= 10 ? _info.score / 600 + 1 : 10;
  _info.speed = DOWN_SPEED - (_info.level * 20);
  return _info;
}

void TetrisModel::gameOver() { _state = GameState_t::GAMEOVER; }

bool TetrisModel::isWin() { return _won; }
}  // namespace brick_game