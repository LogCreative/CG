#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "shader_s.h"
#include "../IncrPoly/IncrPoly-cli.h"
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
    GLFWwindow* window = glfwCreateWindow(800, 600, "Incremental algorithm for parabola", nullptr, nullptr);
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

    float a, b, c;
    int x_b, x_e;
    prepare_input(a, b, c, x_b, x_e);
    IncrPoly poly(a, b,c);
    int length = x_e - x_b + 1;
    int* y = new int[length];
    poly.getRangeValue(x_b, x_e, y);

    int coordLength = length + length;
    int* vertices = new int[coordLength];
    int x = x_b;
    int* y_ptr = y;
    int* vertices_ptr = vertices;
    for (int i = 0; i < length; ++i) {
        std::cout << "(" << x << " " << *y_ptr << ") ";
        *vertices_ptr++ = x++;
        *vertices_ptr++ = *y_ptr++;
    }
    delete[] y;

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*vertices) * coordLength, vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_INT, GL_FALSE, 2 * sizeof(int), (void*)0);
    glEnableVertexAttribArray(0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
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
    delete[] vertices;

    glfwTerminate();
    return 0;
}