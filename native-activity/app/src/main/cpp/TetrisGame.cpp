//
// Created by Jon on 1/15/2018.
//

#include "TetrisGame.h"
#include "util.h"

#include <algorithm>
#include <chrono>
#include <tuple>
#include <random>

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

void update_board(Grid& grid) {
    for (int i = grid.size() - 1; i >= 0; --i) {
        if(auto it = std::find(std::begin(grid[i]), std::end(grid[i]), 0); it == std::end(grid[i])) {
            // Move all rows above 1 step down
            for(size_t j = i; j < grid.size() - 1; ++j) {
                grid[j] = grid[j+1];
            }
        }
    }
}

std::random_device* rd;
std::mt19937* gen;
std::uniform_int_distribution<uint16_t>*  dis;

void init_rng() {
    rd = new std::random_device();
    gen = new std::mt19937( (*rd)() );
    dis = new std::uniform_int_distribution<uint16_t>(0,6);
}

std::tuple<Item,uint16_t, Item, uint16_t> gen_item(uint16_t next) {
    const static Item items[] = {
            {{ 0,0, 1,0, -1,-1,  0,-1}},
            {{-1,0, 0,0,  1, 0,  2, 0}},
            {{-1,0, 0,0,  0,-1,  1,-1}},
            {{-1,0, 0,0,  1, 0,  1,-1}},
            {{-1,0, 0,0,  1, 0, -1,-1}},
            {{ 0,0, 1,0,  0,-1,  1,-1}},
            {{-1,0, 0,0,  1, 0,  0,-1}},
    };

    auto type = next;
    auto next_type = (*dis)(*gen);
    return std::tuple(items[type], type, items[next_type], next_type);
}

Game::Game() {
    for (auto& r : m_grid) {
        r.fill(0);
    }

    init_rng();
    m_next_type = (*dis)(*gen);

    std::tie(m_item, m_type,m_next_item,m_next_type) = gen_item(m_next_type);
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
    float time_step = m_going_down ? 100 : 500;

    auto t1 = cs::high_resolution_clock::now();
    auto dT = cs::duration_cast<cs::milliseconds>(t1 - g_t0).count();

    // Temporary
    m_score += dT;

    elapsed += dT;
    if(elapsed > time_step) {
        elapsed = elapsed - time_step;

        int next_y = m_ypos - 1;
        if(check_collision(m_grid,m_item,m_xpos,next_y)) {
            for (size_t i = 0; i < 8; i += 2) {
                int xp = m_xpos + m_item[i];
                int yp = m_ypos + m_item[i + 1];
                m_grid[yp][xp] = m_type + 1;
            }

            // Check for full rows and update accordingly
            update_board(m_grid);

            // Reset
            m_ypos = 15;
            m_xpos = 4;
            std::tie(m_item,m_type,m_next_item,m_next_type) = gen_item(m_next_type);
        } else {
            m_ypos = next_y;
        }
    }

    g_t0 = t1;
}

Item rotate_item(const Item& item) {
    return {{ -item[1], item[0],
              -item[3], item[2],
              -item[5], item[4],
              -item[7], item[6] }};
}

void Game::rotate() {
    auto rot = rotate_item(m_item);
    if(check_collision(m_grid,rot,m_xpos,m_ypos)) {
        return;
    }
    m_item = rot;
}

} // namespace::tetris