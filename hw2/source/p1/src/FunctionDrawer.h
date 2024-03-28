#ifndef INCRPOLY_OPENGL_FUNCTIONDRAWER_H
#define INCRPOLY_OPENGL_FUNCTIONDRAWER_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"

class FunctionDrawer {
    int samples;
    float (*func_x)(float t);
    float (*func_y)(float t);

    float *vertices;
    unsigned int VAO;
    unsigned int VBO;
public:
    FunctionDrawer(int samples, float (*f_x)(float t), float (*f_y)(float t)) :
            samples(samples), func_x(f_x), func_y(f_y) {}

    void init(float start, float end) {
        float t = start;
        float interval = (end - start) / (samples - 1);
        int coordLength = samples + samples;
        vertices = new float[coordLength];
        float *vptr = vertices;
        for (int i = 0; i < coordLength; i += 2) {
            *vptr++ = func_x(t);
            *vptr++ = func_y(t);
            t += interval;
        }

        glGenBuffers(1, &VBO);
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(*vertices) * coordLength, vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);
    }

    void draw() {
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINE_STRIP, 0, samples);
    }

    ~FunctionDrawer() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        delete[] vertices;
    }
};

#endif //INCRPOLY_OPENGL_FUNCTIONDRAWER_H
