//
// Created by Jon on 1/15/2018.
//

#ifndef NATIVE_ACTIVITY_TETRISGAME_H
#define NATIVE_ACTIVITY_TETRISGAME_H



#include <array>
#include <iostream>
#include <string>
#include <sstream>

namespace tetris {

constexpr size_t nrows = 16;
constexpr size_t ncols = 9;

using Grid = std::array<std::array<size_t,ncols>,nrows>;
using Item = std::array<int,8>;

class Game {
public:
    Game();

    void move_left() ;
    void move_right();
    void rotate();
    void push_down() {m_going_down = true;};
    void release_down() {m_going_down = false;};

    void update();

    const Grid& grid() const { return m_grid; }
    const Item& item() const { return m_item; }
    const uint16_t col() const { return m_col; }
    const int x() const { return m_xpos; }
    const int y() const { return m_ypos; }
    const bool going_down() const { return m_going_down; }

private:
    Grid        m_grid;
    Item        m_item;
    uint16_t    m_col;
    bool        m_going_down {false};

    int         m_xpos {ncols / 2};
    int         m_ypos {nrows - 1};
};

} // namespace tetris

#endif //NATIVE_ACTIVITY_TETRISGAME_H
