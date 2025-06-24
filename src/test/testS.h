#ifndef TEST_S_H
#define TEST_S_H

#include <gtest/gtest.h>

#include "../brick_game/snake/snake.h"

TEST(Snake, test_game_init1) {
  brick_game::SnakeModel t;
  ASSERT_TRUE(t._info.field != nullptr);
  ASSERT_TRUE(t._field != nullptr);
  ASSERT_EQ(t._state, brick_game::GameState_t::START);
}

TEST(Snake, test_spawn1) {
  brick_game::SnakeModel t;
  while (!t.ticked()) t.userInput(10);
  t.sigact();
  ASSERT_EQ(t._state, brick_game::GameState_t::SPAWN);
}

TEST(Snake, test_movedown1) {
  brick_game::SnakeModel t;
  while (!t.ticked()) t.userInput(10);
  t.sigact();
  while (t._state != brick_game::GameState_t::GAMEOVER) {
    t.userInput(258);
    t.sigact();
  }
  ASSERT_EQ(t._state, brick_game::GameState_t::GAMEOVER);
}

TEST(Snake, test_pause) {
  brick_game::SnakeModel t;
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

TEST(Snake, test_userInput) {
  brick_game::SnakeModel t;
  while (!t.ticked()) t.userInput(27);
  t.sigact();
  ASSERT_EQ(t._state, brick_game::GameState_t::GAMEOVER);
}

TEST(Snake, test_updatecurrentstate) {
  brick_game::SnakeModel t;
  t._info.score = 1000;
  ASSERT_EQ(t.updateCurrentState().score, 1000);
}

TEST(Snake, boost) {
  brick_game::SnakeModel t;
  while (t._state != brick_game::GameState_t::GAMEOVER) {
    t.userInput(10);
    t.sigact();
  }
  ASSERT_EQ(t._state, brick_game::GameState_t::GAMEOVER);
}

TEST(Snake, gameover) {
  brick_game::SnakeModel t;
  t._state = brick_game::GameState_t::GAMEOVER;
  t.gameOver();
  ASSERT_EQ(t._state, brick_game::GameState_t::GAMEOVER);
}
#endif