#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "shader_s.h"
#include "FunctionDrawer.h"
#include "NUNRB.h"

int main() {
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
    GLFWwindow *window = glfwCreateWindow(800, 300, "2.2 Non-Uniform NonRational B-Splines", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Intialize glad (loads the OpenGL functions) */
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        return -1;
    }

    /* Create the App */
    int w, h;
    glfwGetWindowSize(window, &w, &h);

    Shader ourShader("assets/shader/vertexShader.glsl", "assets/shader/fragmentShader.glsl");
    ourShader.use();

    int samples = 201;

    FunctionDrawer func01(samples, funcx, B01);
    FunctionDrawer func11(samples, funcx, B11);
    FunctionDrawer func21(samples, funcx, B21);
    FunctionDrawer func31(samples, funcx, B31);

    FunctionDrawer func02(samples, funcx, B02);
    FunctionDrawer func12(samples, funcx, B12);
    FunctionDrawer func22(samples, funcx, B22);

    FunctionDrawer func03(samples, funcx, B03);
    FunctionDrawer func13(samples, funcx, B13);

    FunctionDrawer func04(samples, funcx, B04);

    func01.init(-3, 8);
    func11.init(-3, 8);
    func21.init(-3, 8);
    func31.init(-3, 8);

    func02.init(-3, 8);
    func12.init(-3, 8);
    func22.init(-3, 8);

    func03.init(-3, 8);
    func13.init(-3, 8);

    func04.init(-3,8);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.use();
        ourShader.setVec4("ourColor", 1.0f, 0.6f, 0.6f, 1.0f);
        func01.draw();
        ourShader.setVec4("ourColor", 1.0f, 0.4f, 0.4f, 1.0f);
        func11.draw();
        ourShader.setVec4("ourColor", 1.0f, 0.2f, 0.2f, 1.0f);
        func21.draw();
        ourShader.setVec4("ourColor", 1.0f, 0.0f, 0.0f, 1.0f);
        func31.draw();

        ourShader.setVec4("ourColor", 0.6f, 1.0f, 0.6f, 1.0f);
        func02.draw();
        ourShader.setVec4("ourColor", 0.3f, 1.0f, 0.3f, 1.0f);
        func12.draw();
        ourShader.setVec4("ourColor", 0.0f, 1.0f, 0.0f, 1.0f);
        func22.draw();

        ourShader.setVec4("ourColor", 0.6f, 0.6f, 1.0f, 1.0f);
        func03.draw();
        ourShader.setVec4("ourColor", 0.0f, 0.0f, 1.0f, 1.0f);
        func13.draw();

        ourShader.setVec4("ourColor", 0.0f, 0.0f, 0.0f, 1.0f);
        func04.draw();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}