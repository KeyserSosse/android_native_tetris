//
// Created by Jon on 1/15/2018.
//

#include "TetrisGame.h"
#include "util.h"

#include <algorithm>
#include <chrono>
#include <tuple>

namespace tetris {

bool check_collision(const Grid& grid, const Item& item, int x, int y) {
    for (size_t i = 0; i < 8; i += 2) {
        int xp = x + item[i];
        int yp = y + item[i + 1];
        if(grid[yp][xp]) {
            return true;
        }
        if(yp < 0) { return true; }
        if(xp < 0 || xp > ncols - 1) { return true; }
    }
    return false;
}

std::pair<Item,int> gen_item() {
    const static Item items[] = {
            {{0,0, 1,0, -1,-1, 0,-1}},
            {{0,0, 1,0, 2,0, 3,0}},
            {{-1,0, 0,0, 0,-1, 1,-1}},
            {{0,0, 1,0, 2,0, 2,-1}},
            {{0,0, 1,0, 2,0, 0,-1}},
            {{0,0, 1,0, 0,-1, 1,-1}},
            {{0,0, 1,0, 2,0, 1,-1}},
    };

    uint8_t type = 0;
    return {items[type], type};
}

Game::Game() {
    for(auto& r : m_grid) {
        r.fill(0);
    }
    std::tie(m_item,m_col) = gen_item();
}

void Game::move_left() {
    auto next_x = m_xpos - 1;
    if(check_collision(m_grid,m_item,next_x,m_ypos)) {
        return;
    }
    m_xpos = next_x;
}

void Game::move_right() {
    auto next_x = m_xpos + 1;
    if(check_collision(m_grid,m_item,next_x,m_ypos)) {
        return;
    }
    m_xpos = next_x;
}
namespace cs = std::chrono;
auto g_t0 = cs::high_resolution_clock::now();
void Game::update() {
    static float elapsed = 0.f;
    constexpr float time_step = 500;

    auto t1 = cs::high_resolution_clock::now();
    auto dT = cs::duration_cast<cs::milliseconds>(t1 - g_t0).count();
    elapsed += dT;
    if(elapsed > time_step) {
        elapsed = elapsed - time_step;

        int next_y = m_ypos - 1;
        if(check_collision(m_grid,m_item,m_xpos,next_y)) {
            for (size_t i = 0; i < 8; i += 2) {
                int xp = m_xpos + m_item[i];
                int yp = m_ypos + m_item[i + 1];
                m_grid[yp][xp] = m_col + 1;
            }

            // Reset
            m_ypos = 15;
            std::tie(m_item,m_col) = gen_item();
        } else {
            m_ypos = next_y;
        }
    }

    g_t0 = t1;
}

void Game::move_down() {

}

void Game::rotate() {

}

} // namespace::tetris