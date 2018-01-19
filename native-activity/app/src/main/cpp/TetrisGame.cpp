//
// Created by Jon on 1/15/2018.
//

#include "TetrisGame.h"

#include <algorithm>
#include <chrono>

void TetrisGame::move_left() {
    auto next_x = m_xpos - 1;
    if(next_x < 0) {
        return;
    }
    m_xpos = next_x;
}

void TetrisGame::move_right() {
    auto next_x = m_xpos + 1;
    if(next_x > ncols - 1) {
        return;
    }
    m_xpos = next_x;
}
namespace cs = std::chrono;
auto g_t0 = cs::high_resolution_clock::now();
void TetrisGame::update() {
    static float elapsed = 0.f;
    constexpr float time_step = 500;

    auto t1 = cs::high_resolution_clock::now();
    auto dT = cs::duration_cast<cs::milliseconds>(t1 - g_t0).count();
    elapsed += dT;
    if(elapsed > time_step) {
        elapsed = elapsed - time_step;
        m_ypos -= 1;
        if(m_ypos < 0) { m_ypos = 15; }
    }

    g_t0 = t1;
}

void TetrisGame::move_down() {

}

void TetrisGame::rotate() {

}

