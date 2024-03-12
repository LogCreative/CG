#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "shader_s.h"
#include "../IncrPoly/IncrPoly.hpp"

int main()
{
    /* Initialize the library */
    if (!glfwInit()) {
        return -1;
    }

    /* Create a windowed mode window and its OpenGL context */
#ifdef __APPLE__
    /* We need to explicitly ask for a 3.3 context on Mac */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Incremental algorithm for parabola", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Intialize glad (loads the OpenGL functions) */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        return -1;
    }

    /* Create the App */
    int w, h;
    glfwGetWindowSize(window, &w, &h);

    Shader ourShader("shader/vertexShader.glsl", "shader/fragmentShader.glsl");
    ourShader.use();

    float a = -0.05, b = 5, c = 10;
    IncrPoly poly(a, b,c);
    const int x_b = -30, x_e = 130;
    const int length = x_e - x_b + 1;
    int* y = new int[length];
    poly.getRangeValue(x_b, x_e, y);

    float vertices[length+length];
    int x = x_b;
    int* yptr = y;
    for (int i = 0; i < length+length; i+=2) {
        std::cout << "(" << x << " " << *yptr << ") ";
        vertices[i] = x++;
        vertices[i+1] = *yptr++;
    }
    delete[] y;

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINE_STRIP, 0, length);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}