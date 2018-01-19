//
// Created by Jon on 1/15/2018.
//

#ifndef NATIVE_ACTIVITY_TETRISGAME_H
#define NATIVE_ACTIVITY_TETRISGAME_H


#include <array>
#include <iostream>
#include <string>
#include <sstream>

constexpr size_t nrows = 16;
constexpr size_t ncols = 9;

//template<size_t R, size_t C>
class TetrisGame {
public:
    using Grid = std::array<std::array<size_t,ncols>,nrows>;

    TetrisGame() {
        for(auto& r : m_grid) {
            r.fill(0);
        }
    }

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

    const Grid grid() { return m_grid; }
    const size_t x() { return m_xpos; }
    const size_t y() { return m_ypos; }

private:
    Grid m_grid;

    int m_xpos {4};
    int m_ypos {15};
};


#endif //NATIVE_ACTIVITY_TETRISGAME_H
