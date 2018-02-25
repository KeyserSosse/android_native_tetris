//
// Created by Jon on 2/25/2018.
//

#ifndef NATIVE_ACTIVITY_DIGITRENDERER_H
#define NATIVE_ACTIVITY_DIGITRENDERER_H

#include <cstdlib>
#include <array>
#include <string>

#include "util.h"

class DigitRenderer {
public:
    DigitRenderer();
    ~DigitRenderer();

    void draw_string_number(const std::string& s, float x, float y, float sx, float sy);

private:
    std::array<GLuint,  10> m_vertex_arrays;
    std::array<GLsizei, 10> m_line_counts;

    GLuint m_program {0};
    GLint m_u_offset{-1};
    GLint m_u_scale{-1};

    void draw(int num, float x, float y, float sx, float sy);
};


#endif //NATIVE_ACTIVITY_DIGITRENDERER_H
