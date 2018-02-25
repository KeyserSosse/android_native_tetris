//
// Created by Jon on 2/25/2018.
//

#include "DigitRenderer.h"

#include <vector>

static const char digit_vertex_shader[] =
                "#version 300 es\n"
                "layout(location = 0) in vec2 pos;\n"
                "uniform vec2 u_scale;"
                "uniform vec2 u_offset;"
                "void main() {\n"
                "    mat2 sr = mat2(u_scale.x, 0, 0, u_scale.y);\n"
                "    gl_Position = vec4(sr*pos + u_offset, 0.0, 1.0);\n"
                "}\n";

static const char digit_fragment_shader[] =
                "#version 300 es\n"
                "precision mediump float;\n"
                "out vec4 outColor;\n"
                "void main() {\n"
                "    outColor = vec4(1.f, 1.f, 1.f, 1.f);\n"
                "}\n";

DigitRenderer::DigitRenderer() {
    const std::vector<float> digit_vertex_data[] = {
            {1.f, 1.f,  0.f, 1.f,  0.f, 0.f,  0.f, -1.f,  1.f, -1.f,  1.f, 0.f,  1.f, 1.f},
            {1.f, 1.f,  1.f, 0.f,  1.f, -1.f}, // 1)
            {0.f, 1.f,  1.f, 1.f,  1.f, 0.f,  0.f, 0.f,  0.f, -1.f,  1.f, -1.f},
            {0.f, 1.f,  1.f, 1.f,  1.f, 0.f,  0.f, 0.f,  1.f, 0.f,  1.f, -1.f,  0.f, -1.f},
            {0.f, 1.f,  0.f, 0.f,  1.f, 0.f,  1.f, 1.f,  1.f, 0.f,  1.f, -1.f  },
            {1.f, 1.f,  0.f, 1.f,  0.f, 0.f,  1.f, 0.f,  1.f, -1.f, 0.f, -1.f },
            {1.f, 1.f,  0.f, 1.f,  0.f, 0.f,  0.f, -1.f,  1.f, -1.f,  1.f, 0.f,  0.f, 0.f},
            {0.f, 1.f,  1.f, 1.f,  1.f, 0.f,  1.f, -1.f},
            {1.f, 1.f,  0.f, 1.f,  0.f, 0.f,  1.f, 0.f,  1.f, -1.f,  0.f, -1.f,  0.f, 0.f, 1.f, 0.f,  1.f, 1.f},
            {1.f, -1.f, 1.f, 0.f,  1.f, 1.f,  0.f, 1.f,  0.f, 0.f,  1.f, 0.f}
    };

    glGenVertexArrays(m_vertex_arrays.size(), m_vertex_arrays.data());

    for(size_t i = 0; i < m_vertex_arrays.size(); ++i) {
        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, digit_vertex_data[i].size() * sizeof(float), digit_vertex_data[i].data(), GL_STATIC_DRAW);

        glBindVertexArray(m_vertex_arrays[i]);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (GLvoid*)0);
        glEnableVertexAttribArray(0);

        m_line_counts[i] = digit_vertex_data[i].size() / 2;
    }

    m_program = createProgram(digit_vertex_shader,digit_fragment_shader);
    if(!m_program) {
        ALOGE("Failed to create digit program");
    }

    m_u_offset = glGetUniformLocation(m_program, "u_offset");
    m_u_scale = glGetUniformLocation(m_program, "u_scale");
}

DigitRenderer::~DigitRenderer() {
    glDeleteVertexArrays(m_vertex_arrays.size(), m_vertex_arrays.data());
    //glDeleteBuffers(VB_COUNT, mVB);
    glDeleteProgram(m_program);
}

void DigitRenderer::draw(int num, float x, float y, float sx, float sy) {
    glUniform2f(m_u_offset, x, y);
    glUniform2f(m_u_scale, sx, sy);

    glBindVertexArray(m_vertex_arrays[num]);
    glLineWidth(3.f);
    glDrawArrays(GL_LINE_STRIP, 0, m_line_counts[num]);
}

void DigitRenderer::draw_string_number(const std::string &s, float x, float y, float sx, float sy) {
    glUseProgram(m_program);

    size_t i = s.size() - 1;
    for(const auto& c : s) {
        int d = c - '0';
        draw(d, x - (sx + 0.05) * i--, y, sx, sy);
    }
}

