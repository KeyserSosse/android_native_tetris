//
// Created by Jon on 1/5/2018.
//

#ifndef NATIVE_ACTIVITY_RENDERER_H
#define NATIVE_ACTIVITY_RENDERER_H


#include "util.h"

#include <EGL/egl.h>

class RendererES3 {
public:
    RendererES3();
    ~RendererES3();
    bool init();
    void resize(int w, int h);
    void render();

private:
    enum {VB_INSTANCE, VB_SCALEROT, VB_OFFSET, VB_COUNT};

    float* mapOffsetBuf();
    void  unmapOffsetBuf();
    float* mapTransformBuf();
    void unmapTransformBuf();
    void draw(unsigned int numInstances);


    void calcSceneParams(unsigned int w, unsigned int h, float* offsets);
    void step();

    const EGLContext mEglContext;
    GLuint mProgram;
    GLuint mVB[VB_COUNT];
    GLuint mVBState;



    unsigned int mNumInstances;
    float mScale[2];
    float mAngularVelocity[MAX_INSTANCES];
    uint64_t mLastFrameNs;
    float mAngles[MAX_INSTANCES];
};

#endif //NATIVE_ACTIVITY_RENDERER_H
