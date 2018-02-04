//
// Created by Jon on 1/5/2018.
//

#include "renderer.h"

#include <algorithm>
#include <math.h>
#include <stdlib.h>
#include <string.h>
//#include <time.h>
#include <chrono>

#include "TetrisGame.h"

#define STR(s) #s
#define STRV(s) STR(s)

#define POS_ATTRIB 0
//#define SCALEROT_ATTRIB 1
#define COLOR_ATTRIB 1
#define OFFSET_ATTRIB 2

static const char VERTEX_SHADER[] =
        "#version 300 es\n"
                "layout(location = " STRV(POS_ATTRIB) ") in vec2 pos;\n"
                "layout(location=" STRV(COLOR_ATTRIB) ") in vec4 color;\n"
                "layout(location=" STRV(OFFSET_ATTRIB) ") in vec2 offset;\n"
                "out vec4 vColor;\n"
                "uniform vec2 scale;"
                "void main() {\n"
                "    mat2 sr = mat2(0.111111, 0, 0, 0.062500);\n"
                "    gl_Position = vec4(sr*pos + offset, 0.0, 1.0);\n"
                "    vColor = color;\n"
                "}\n";

static const char FRAGMENT_SHADER[] =
        "#version 300 es\n"
                "precision mediump float;\n"
                "in vec4 vColor;\n"
                "out vec4 outColor;\n"
                "void main() {\n"
                "    outColor = vColor;\n"
                "}\n";


static const float blue_color[] = {0.f, 0.f, 255.f, 1.f};
static const float red_color[] = {255.f, 0.f, 0.f, 1.f};
static const float green_color[] = {0.f, 255.f, 0.f, 1.f};
static const float yellow_color[] = {255.f, 255.f, 0.f, 1.f};
static const float cyan_color[] = {0.f, 255.f, 255.f, 1.f};
static const float purple_color[] = {255.f, 0.f, 255.f, 1.f};
static const float orange_color[] = {255.f, 255.f, 240.f, 1.f};

static const float* color_pointers[] = { std::begin(green_color),
                                         std::begin(red_color),
                                         std::begin(blue_color),
                                         std::begin(yellow_color),
                                         std::begin(cyan_color),
                                         std::begin(purple_color),
                                         std::begin(orange_color) };

RendererES3* createES3Renderer() {
    RendererES3* renderer = new RendererES3;
    if (!renderer->init()) {
        delete renderer;
        return NULL;
    }
    return renderer;
}

RendererES3::RendererES3()
        :   mEglContext(eglGetCurrentContext()),
            mProgram(0),
            mVBState(0),
            mNumInstances(0),
            m_offsets(tetris::nrows * tetris::ncols * 2, 0)
{
    for (int i = 0; i < VB_COUNT; i++)
        mVB[i] = 0;
}

bool RendererES3::init() {
    mProgram = createProgram(VERTEX_SHADER, FRAGMENT_SHADER);
    if (!mProgram)
        return false;

    glGenBuffers(VB_COUNT, mVB);
    glBindBuffer(GL_ARRAY_BUFFER, mVB[VB_INSTANCE]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(QUAD), &QUAD[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, mVB[VB_SCALEROT]);
    glBufferData(GL_ARRAY_BUFFER, MAX_INSTANCES * 4*sizeof(float), NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, mVB[VB_OFFSET]);
    glBufferData(GL_ARRAY_BUFFER, MAX_INSTANCES * 2*sizeof(float), NULL, GL_STATIC_DRAW);

    glGenVertexArrays(1, &mVBState);
    glBindVertexArray(mVBState);

    glBindBuffer(GL_ARRAY_BUFFER, mVB[VB_INSTANCE]);
    glVertexAttribPointer(POS_ATTRIB, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, pos));
    glEnableVertexAttribArray(POS_ATTRIB);

    glBindBuffer(GL_ARRAY_BUFFER, mVB[VB_SCALEROT]);
    glVertexAttribPointer(COLOR_ATTRIB, 4, GL_FLOAT, GL_FALSE, 4*sizeof(float), 0);
    glEnableVertexAttribArray(COLOR_ATTRIB);
    glVertexAttribDivisor(COLOR_ATTRIB, 1);

    glBindBuffer(GL_ARRAY_BUFFER, mVB[VB_OFFSET]);
    glVertexAttribPointer(OFFSET_ATTRIB, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);
    glEnableVertexAttribArray(OFFSET_ATTRIB);
    glVertexAttribDivisor(OFFSET_ATTRIB, 1);

    ALOGV("Using OpenGL ES 3.0 renderer");
    return true;
}

RendererES3::~RendererES3() {
    /* The destructor may be called after the context has already been
     * destroyed, in which case our objects have already been destroyed.
     *
     * If the context exists, it must be current. This only happens when we're
     * cleaning up after a failed init().
     */
    if (eglGetCurrentContext() != mEglContext)
        return;
    glDeleteVertexArrays(1, &mVBState);
    glDeleteBuffers(VB_COUNT, mVB);
    glDeleteProgram(mProgram);
}

float* RendererES3::mapOffsetBuf() {
    glBindBuffer(GL_ARRAY_BUFFER, mVB[VB_OFFSET]);
    return (float*)glMapBufferRange(GL_ARRAY_BUFFER,
                                    0, MAX_INSTANCES * 2*sizeof(float),
                                    GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
}

void RendererES3::unmapOffsetBuf() {
    glUnmapBuffer(GL_ARRAY_BUFFER);
}

float* RendererES3::mapTransformBuf() {
    glBindBuffer(GL_ARRAY_BUFFER, mVB[VB_SCALEROT]);
    return (float*)glMapBufferRange(GL_ARRAY_BUFFER,
                                    0, MAX_INSTANCES * 4*sizeof(float),
                                    GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
}

void RendererES3::unmapTransformBuf() {
    glUnmapBuffer(GL_ARRAY_BUFFER);
}

void RendererES3::draw(unsigned int numInstances) {
    glUseProgram(mProgram);
    glBindVertexArray(mVBState);
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, numInstances);
}

void RendererES3::resize(int w, int h) {
    auto offsets = mapOffsetBuf();
    calcSceneParams(w, h, offsets);
    unmapOffsetBuf();

    // Auto gives a signed int :-(
    for (auto i = (unsigned)0; i < mNumInstances; i++) {
        //mAngles[i] = drand48() * TWO_PI;
        //mAngularVelocity[i] = MAX_ROT_SPEED * (2.0*drand48() - 1.0);
    }

    glViewport(0, 0, w, h);
}

void RendererES3::calcSceneParams(unsigned int w, unsigned int h,
                               float* offsets) {
    // number of cells along the larger screen dimension
    const float NCELLS_MAJOR = MAX_INSTANCES_PER_SIDE;
    // cell size in scene space
    const float CELL_SIZE = 2.0f / NCELLS_MAJOR;

    // Calculations are done in "landscape", i.e. assuming dim[0] >= dim[1].
    // Only at the end are values put in the opposite order if h > w.
    const float dim[2] = {fmaxf(w,h), fminf(w,h)};
    const float aspect[2] = {dim[0] / dim[1], dim[1] / dim[0]};
    const float scene2clip[2] = {1.0f, aspect[0]};
    const int ncells[2] = {
            static_cast<int>(NCELLS_MAJOR),
            (int)floorf(NCELLS_MAJOR * aspect[1])
    };

    float centers[2][MAX_INSTANCES_PER_SIDE];
    for (int d = 0; d < 2; d++) {
        auto offset = -ncells[d] / NCELLS_MAJOR; // -1.0 for d=0
        for (auto i = 0; i < ncells[d]; i++) {
            centers[d][i] = scene2clip[d] * (CELL_SIZE*(i + 0.5f) + offset);
        }
    }

    int major = w >= h ? 0 : 1;
    int minor = w >= h ? 1 : 0;
    // outer product of centers[0] and centers[1]
    for (int i = 0; i < ncells[0]; i++) {
        for (int j = 0; j < ncells[1]; j++) {
            int idx = i*ncells[1] + j;
            offsets[2*idx + major] = centers[0][i];
            offsets[2*idx + minor] = centers[1][j];
            ALOGV("offsets[%u] =  %f", 2*idx+major, centers[0][i]);
        }
    }

    std::copy(offsets, offsets + 2 * tetris::ncols * tetris::nrows, std::begin(m_offsets));
    //std::copy(offsets, offsets + 2 * 9 * 16, std::begin(m_offsets));

    mNumInstances = ncells[0] * ncells[1];
    mScale[major] = 0.5f * CELL_SIZE * scene2clip[0];
    mScale[minor] = 0.5f * CELL_SIZE * scene2clip[1];

    float* transforms = mapTransformBuf();
    for (unsigned int i = 0; i < mNumInstances; i++) {
        transforms[4*i + 0] = 0;
        transforms[4*i + 1] = 255;
        transforms[4*i + 2] = 0;
        transforms[4*i + 3] = 1.f;
    }
    unmapTransformBuf();

    ALOGV("mScale[%u] = %f",major,0.5f * CELL_SIZE * scene2clip[0]);
    ALOGV("mScale[%u] = %f",minor,0.5f * CELL_SIZE * scene2clip[1]);
}

void RendererES3::draw_instances(const std::vector<size_t>& indices, const std::vector<size_t>& colors) {
    auto n_instances = indices.size();

    auto offsets = mapOffsetBuf();
    if(offsets == nullptr) {
        ALOGV("offsets is null");
        unmapOffsetBuf();
        return;
    } else {
        for(size_t i = 0; i < n_instances; ++i) {
            offsets[i * 2] = m_offsets[indices[i] * 2];
            offsets[i * 2 + 1] = m_offsets[indices[i] * 2 + 1];
        }
        unmapOffsetBuf();
    }

    auto color_buffer = mapTransformBuf();
    if(color_buffer == nullptr) {
        ALOGV("colors is null");
        unmapTransformBuf();
        return;
    } else {
        for(size_t i = 0; i < n_instances; ++i) {
            const auto p_color = color_pointers[colors[i]];
            std::copy(p_color, p_color + 4, &color_buffer[i * 4]);
        }
        unmapTransformBuf();
    }

    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    draw(n_instances);
    checkGlError("Renderer::render");
}
