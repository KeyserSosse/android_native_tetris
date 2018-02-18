//
// Created by Jon on 1/5/2018.
//

#ifndef NATIVE_ACTIVITY_RENDERER_H
#define NATIVE_ACTIVITY_RENDERER_H


#include "util.h"

#include <EGL/egl.h>
#include <vector>

class RendererES3 {
public:
    RendererES3();
    ~RendererES3();
    bool init();
    void resize(int w, int h);
    void render(int,int);
    void draw_instances(const std::vector<size_t>& indices, const std::vector<size_t>& colors);

private:
    enum {VB_INSTANCE, VB_COLOR, VB_OFFSET, VB_COUNT};

    float* mapOffsetBuf();
    void  unmapOffsetBuf();
    float* mapColorBuf();
    void unmapColorBuf();
    void draw(unsigned int numInstances);


    void calcSceneParams(unsigned int w, unsigned int h, float* offsets);


    const EGLContext mEglContext;
    GLuint mProgram;
    GLuint mVB[VB_COUNT];
    GLuint mVBState;

    unsigned int        mNumInstances;
    float               mScale[2];

    std::vector<float>  m_offsets;
};

#endif //NATIVE_ACTIVITY_RENDERER_H
