//
// Created by Jon on 1/5/2018.
//

#ifndef NATIVE_ACTIVITY_UTIL_H
#define NATIVE_ACTIVITY_UTIL_H

#include <android/log.h>
#include <GLES3/gl3.h>


#define DEBUG 1
#define LOG_TAG "GLES3JNI"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#if DEBUG
#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#else
#define ALOGV(...)
#endif


#define MAX_INSTANCES_PER_SIDE 16
#define MAX_INSTANCES   (MAX_INSTANCES_PER_SIDE * MAX_INSTANCES_PER_SIDE)
#define TWO_PI          (2.0 * M_PI)
#define MAX_ROT_SPEED   (0.3 * TWO_PI)



struct Vertex {
    GLfloat pos[2];
};

const Vertex QUAD[4] = {
        // Square with diagonal < 2 so that it fits in a [-1 .. 1]^2 square
        // regardless of rotation.
        {{-0.9f, -0.9f}},
        {{ 0.9f, -0.9f}},
        {{-0.9f,  0.9f}},
        {{ 0.9f,  0.9f}},
};

bool checkGlError(const char* funcName);

GLuint createShader(GLenum shaderType, const char* src);

GLuint createProgram(const char* vtxSrc, const char* fragSrc);

#endif //NATIVE_ACTIVITY_UTIL_H
