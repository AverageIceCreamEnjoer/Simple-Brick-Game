#include "snake.h"

namespace brick_game {

int SnakeModel::getRandNum(int d) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dist(0, d);
  return dist(gen);
}

std::pair<int, int> SnakeModel::getRandom() {
  std::pair<int, int> res(0, 0);
  if (_pixels.size() > 0) {
    int r = getRandNum(_pixels.size() - 1);
    auto it = _pixels.begin();
    for (int i = 0; i++ < r; ++it);
    std::pair<int, int> tmp = *it;
    res = std::make_pair(tmp.first, tmp.second);
    _pixels.erase(it);
  }
  return res;
}

void SnakeModel::shift() {
  const static int coords[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
  int x = coords[_direction][0], y = coords[_direction][1];
  if (_apple.first == _snake.front().first + y &&
      _apple.second == _snake.front().second + x) {
    _snake.push_front(std::make_pair(_apple.first, _apple.second));
    _apple = std::make_pair(-1, -1);
    _info.score++;
    if (_info.score != 200)
      _state = GameState_t::SPAWN;
    else {
      _state = GameState_t::GAMEOVER;
      _won = true;
    }
  } else {
    std::pair<int, int> tmp =
        std::make_pair(_snake.back().first, _snake.back().second);
    _pixels.insert(tmp);
    tmp = std::make_pair(_snake.front().first + y, _snake.front().second + x);
    _snake.pop_back();
    _snake.push_front(tmp);
    _pixels.erase(std::make_pair(tmp.first, tmp.second));
  }
}
SnakeModel::SnakeModel()
    : _pixels(), _state(START), _act(Action), _snake(4), _won(false) {
  _field = new int *[HEIGHT * WIDTH + HEIGHT];
  for (int i = 0; i < HEIGHT; ++i) {
    _field[i] = (int *)(_field + HEIGHT + WIDTH * i);
    for (int j = 0; j < WIDTH; ++j) {
      _field[i][j] = 0;
      _pixels.insert(std::make_pair(i, j));
    }
  }
  _info.field = _field;
  _info.high_score = readHighScore();
  _info.level = 1;
  _info.next = new int *[20];
  int g[4][4] = {{0, 1, 0, 0}, {1, 1, 0, 1}, {1, 0, 1, 1}, {0, 0, 1, 0}};
  for (int i = 0; i < 4; ++i) {
    _info.next[i] = (int *)(_info.next + 4 + 4 * i);
    for (int j = 0; j < 4; ++j) _info.next[i][j] = g[i][j];
  }
  _info.pause = 0;
  _info.score = 0;
  _info.speed = START_SPEED;
  int n = getRandNum(3), x = 0, y = 0;
  const int tmp[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
  x = tmp[n][0];
  y = tmp[n][1];
  _direction = (Direction_t)n;
  _snake[0] = std::make_pair((HEIGHT - 1) / 2, (WIDTH - 1) / 2);
  _pixels.erase(_snake[0]);
  for (int i = 1; i < 4; ++i) {
    _snake[i] =
        std::make_pair(_snake[i - 1].first + y, _snake[i - 1].second + x);
    _pixels.erase(_snake[i]);
  }
  _apple = std::make_pair(-1, -1);
  _timer = std::chrono::steady_clock::now();
}

SnakeModel::~SnakeModel() {
  writeHighScore();
  delete[] _field;
  _info.field = nullptr;
  delete[] _info.next;
}

typedef void (SnakeModel::*action)();

void SnakeModel::sigact() {
  const static action fsm_arr[STATES] = {
      &SnakeModel::startGame, &SnakeModel::pauseGame, &SnakeModel::spawnApple,
      &SnakeModel::move, &SnakeModel::gameOver};
  if (ticked()) {
    (this->*fsm_arr[_state])();
    _timer = std::chrono::steady_clock::now();
  }
}

void SnakeModel::startGame() {
  if (_act == Terminate)
    _state = GAMEOVER;
  else if (_act == Start) {
    _state = SPAWN;
    _act = Action;
  }
}

void SnakeModel::spawnApple() {
  _apple = getRandom();
  _state = GameState_t::MOVING;
}

void SnakeModel::move() {
  if (_act == UserAction_t::Pause) {
    _info.pause = 1;
    _state = GameState_t::PAUSE;
    _act = UserAction_t::Action;
  } else if (_act == UserAction_t::Terminate) {
    _state = GameState_t::GAMEOVER;
  } else {
    if (_act == UserAction_t::Start)
      boost();
    else if (UserAction_t::Terminate < _act && _act < UserAction_t::Action) {
      Direction_t tmp = (Direction_t)(_act - 3);
      if (!opposites(tmp)) _direction = tmp;
    }
    shift();
    _act = UserAction_t::Action;
    attaching();
  }
}

void SnakeModel::attaching() {
  if (isCollided()) {
    _state = GameState_t::GAMEOVER;
  } else if (_state != GameState_t::SPAWN)
    _state = GameState_t::MOVING;
}

void SnakeModel::pauseGame() {
  if (_act == UserAction_t::Pause) {
    _act = UserAction_t::Action;
    _info.pause = 0;
    _state = GameState_t::MOVING;
  } else if (_act == UserAction_t::Terminate) {
    _act = UserAction_t::Action;
    _info.pause = 0;
    _state = GameState_t::GAMEOVER;
  }
}

bool SnakeModel::isCollided() {
  int yh = _snake.front().first, xh = _snake.front().second;
  bool b = false;
  for (auto i = ++_snake.begin(); i != _snake.end() && !b; ++i)
    b = (yh == i->first && xh == i->second);
  return b || (yh < 0 || yh >= HEIGHT || xh < 0 || xh >= WIDTH);
}

bool SnakeModel::opposites(Direction_t x) {
  return (_direction == Direction_t::L && x == Direction_t::R) ||
         (_direction == Direction_t::R && x == Direction_t::L) ||
         (_direction == Direction_t::U && x == Direction_t::D) ||
         (_direction == Direction_t::D && x == Direction_t::U);
}

GameState_t SnakeModel::getState() { return _state; }

bool SnakeModel::ticked() {
  int b = (std::chrono::duration<double>(std::chrono::steady_clock::now() -
                                         _bstimer)
               .count() < ((double)_info.speed) / 1000)
              ? 2
              : 1;
  return std::chrono::duration<double>(std::chrono::steady_clock::now() -
                                       _timer)
             .count() > (((double)_info.speed) / 1000 / b);
}

void SnakeModel::userInput(int act) {
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

GameInfo_t SnakeModel::updateCurrentState() {
  for (size_t i = 0; i < HEIGHT; i++)
    for (size_t j = 0; j < WIDTH; j++) _info.field[i][j] = Block_t::Empty;
  if (_apple.first >= 0 && _apple.second >= 0)
    _info.field[_apple.first][_apple.second] = Block_t::Apple;
  if (_snake.front().first >= 0 && _snake.front().first < HEIGHT &&
      _snake.front().second < WIDTH && _snake.front().second >= 0)
    _info.field[_snake.front().first][_snake.front().second] = Block_t::Head;
  std::pair<int, int> tmp;
  for (auto i = ++_snake.begin(); i != _snake.end(); i++) {
    tmp = *i;
    _info.field[tmp.first][tmp.second] = Block_t::Filled;
  }
  _info.high_score =
      (_info.score > _info.high_score) ? _info.score : _info.high_score;
  _info.level = (_info.score / 5 + 1) <= 10 ? _info.score / 5 + 1 : 10;
  _info.speed = START_SPEED - (_info.level * 20);
  return _info;
}

void SnakeModel::writeHighScore() {
  FILE *f = fopen(HIGH_SCORE_FILE, "w");
  if (f) {
    fprintf(f, "%d",
            (_info.score > _info.high_score) ? _info.score : _info.high_score);
    fclose(f);
  }
}

int SnakeModel::readHighScore() {
  int tmp = 0;
  FILE *f = fopen(HIGH_SCORE_FILE, "r");
  if (f) {
    fscanf(f, "%d", &tmp);
    fclose(f);
  }
  return tmp;
}

void SnakeModel::boost() { _bstimer = std::chrono::steady_clock::now(); }

void SnakeModel::gameOver() { _state = GameState_t::GAMEOVER; }

bool SnakeModel::isWin() { return _won; }
}  // namespace brick_game