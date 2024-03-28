#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "shader_s.h"
#include "FunctionDrawer.h"
#include "MyFunctions.h"

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
    GLFWwindow* window = glfwCreateWindow(800, 600, "2.1 Intersection", nullptr, nullptr);
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

    Shader ourShader("assets/shader/vertexShader.glsl", "assets/shader/fragmentShader.glsl");
    ourShader.use();

    int samples = 101;

    FunctionDrawer func1(samples, gamma_x, gamma_y);
    FunctionDrawer func2(samples, eta_x, eta_y);

    func1.init(0,1);
    func2.init(0,1);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.use();
        ourShader.setVec4("ourColor",1.0f,0.0f,0.0f,1.0f);
        func1.draw();
        ourShader.setVec4("ourColor",0.0f,0.0f,1.0f,1.0f);
        func2.draw();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}