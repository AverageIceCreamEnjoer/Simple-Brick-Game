#ifndef TEST_T_H
#define TEST_T_H

#include <gtest/gtest.h>

#include "../brick_game/tetris/tetrisModel.h"

TEST(Tetris, test_game_init1) {
  brick_game::TetrisModel t;
  ASSERT_TRUE(t._info.field != nullptr);
  ASSERT_TRUE(t._field != nullptr);
  ASSERT_EQ(t._state, brick_game::GameState_t::START);
}

TEST(Tetris, test_figure1) {
  brick_game::TetrisModel t;
  ASSERT_TRUE(t._current.field != nullptr);
  ASSERT_TRUE(t._next.field != nullptr);
}

TEST(Tetris, test_figure2) {
  brick_game::TetrisModel t;
  for (int i = 0; i < 25; ++i) t._current.update();
}

TEST(Tetris, test_spawn1) {
  brick_game::TetrisModel t;
  while (!t.ticked()) t.userInput(10);
  t.sigact();
  ASSERT_EQ(t._state, brick_game::GameState_t::SPAWN);
}

TEST(Tetris, test_movedown1) {
  brick_game::TetrisModel t;
  while (!t.ticked()) t.userInput(10);
  t.sigact();
  while (t._state != brick_game::GameState_t::GAMEOVER) {
    t.userInput(258);
    t.sigact();
  }
  ASSERT_EQ(t._state, brick_game::GameState_t::GAMEOVER);
}

TEST(Tetris, test_pause) {
  brick_game::TetrisModel t;
  while (!t.ticked()) t.userInput(10);
  t.sigact();
  while (!t.ticked()) t.userInput(258);
  t.sigact();
  while (!t.ticked()) t.userInput('p');
  t.sigact();
  ASSERT_EQ(t._state, brick_game::GameState_t::PAUSE);
  while (!t.ticked()) t.userInput('p');
  t.sigact();
  ASSERT_EQ(t._state, brick_game::GameState_t::MOVING);
}

TEST(Tetris, test_userInput) {
  brick_game::TetrisModel t;
  while (!t.ticked()) t.userInput(27);
  t.sigact();
  ASSERT_EQ(t._state, brick_game::GameState_t::GAMEOVER);
}

TEST(Tetris, test_updatecurrentstate) {
  brick_game::TetrisModel t;
  t._info.score = 1000;
  ASSERT_EQ(t.updateCurrentState().score, 1000);
}

TEST(Tetris, gameover) {
  brick_game::TetrisModel t;
  t._state = brick_game::GameState_t::GAMEOVER;
  t.gameOver();
  ASSERT_EQ(t._state, brick_game::GameState_t::GAMEOVER);
}
#endif