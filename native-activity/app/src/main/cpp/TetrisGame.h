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

    std::string print_grid() const {
        std::stringstream ss;
        for(const auto& row : m_grid) {
            for (const auto& col : row) {
                ss << col << " ";
            }
            ss << "\n";
        }
        return ss.str();
    }

    void move_left() ;
    void move_right();
    void rotate();
    void move_down();

    void update();

    const Grid& grid() const { return m_grid; }
    const Item& item() const { return m_item; }
    const size_t x() const { return m_xpos; }
    const size_t y() const { return m_ypos; }

private:
    Grid    m_grid;
    Item    m_item;
    size_t  m_col;

    int     m_xpos {4};
    int     m_ypos {15};
};

} // namespace tetris

#endif //NATIVE_ACTIVITY_TETRISGAME_H
