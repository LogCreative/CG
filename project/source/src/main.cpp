#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader_m.h"
#include "camera.h"

int screenWidth = 800;
int screenHeight = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = screenWidth / 2.0f;
float lastY = screenHeight / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// meshes
unsigned int planeVAO;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

//    Make sure Working Directory is the root directory.
    Shader planeShader("assets/shader/planeVertexShader.glsl", "assets/shader/planeFragmentShader.glsl");
    Shader lightingShader("assets/shader/vertexShader.glsl", "assets/shader/fragmentShader.glsl");
    Shader lightCubeShader("assets/shader/lightCubeVertexShader.glsl", "assets/shader/lightCubeFragmentShader.glsl");

    glm::vec3 pointLightPositions[] = {
            glm::vec3( 1.5f,  0.7f,  2.0f),
            glm::vec3( -1.5f, 0.7f, 2.0f)
    };

    /// Plane

    float planeVertices[] = {
            25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
            -25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
            -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

            25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
            -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
            25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
    };
    unsigned int planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindVertexArray(0);
    unsigned int woodTexture = loadTexture("assets/art/wood.png");
    planeShader.use();
    planeShader.setInt("texture1", 0);


    /// Cube (polyhedron)

    float cubeVertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    glBindVertexArray(cubeVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);


    /// Cylinder

    float cylinder_height = 1.0f,
            cylinder_radius = 1.0f;
    int nr_of_points_cylinder = 256;
    float delta_degree = 2 * M_PI / nr_of_points_cylinder;
    int number_of_vertices_cylinder = (nr_of_points_cylinder + 1) * 4 + 2;
    float* cylinderVertices = new float[number_of_vertices_cylinder * 6];
    // top
    const int offset_top_begin = 0;
    cylinderVertices[0] = 0;
    cylinderVertices[1] = cylinder_height;
    cylinderVertices[2] = 0;
    cylinderVertices[3] = 0;
    cylinderVertices[4] = 1;
    cylinderVertices[5] = 0;
    const int offset_top = offset_top_begin + 6;
    for (int i = 0; i <= nr_of_points_cylinder; i++) {
        cylinderVertices[offset_top + i * 6] = glm::cos(delta_degree * i) * cylinder_radius;
        cylinderVertices[offset_top + i * 6 + 1] = cylinder_height;
        cylinderVertices[offset_top + i * 6 + 2] = glm::sin(delta_degree * i) * cylinder_radius;
        cylinderVertices[offset_top + i * 6 + 3] = 0;
        cylinderVertices[offset_top + i * 6 + 4] = 1;
        cylinderVertices[offset_top + i * 6 + 5] = 0;
    }
    // bottom
    const int offset_bottom_begin = offset_top + (nr_of_points_cylinder + 1) * 6;
    cylinderVertices[offset_bottom_begin] = 0;
    cylinderVertices[offset_bottom_begin + 1] = 0;
    cylinderVertices[offset_bottom_begin + 2] = 0;
    cylinderVertices[offset_bottom_begin + 3] = 0;
    cylinderVertices[offset_bottom_begin + 4] = -1;
    cylinderVertices[offset_bottom_begin + 5] = 0;
    const int offset_bottom = offset_bottom_begin + 6;
    for (int i = 0; i <= nr_of_points_cylinder; i++) {
        cylinderVertices[offset_bottom + i * 6] = glm::cos(delta_degree * i) * cylinder_radius;
        cylinderVertices[offset_bottom + i * 6 + 1] = 0;
        cylinderVertices[offset_bottom + i * 6 + 2] = glm::sin(delta_degree * i) * cylinder_radius;
        cylinderVertices[offset_bottom + i * 6 + 4] = 0;
        cylinderVertices[offset_bottom + i * 6 + 3] = -1;
        cylinderVertices[offset_bottom + i * 6 + 5] = 0;
    }
    // side
    const int offset_side = offset_bottom + (nr_of_points_cylinder + 1) * 6;
    float normal_side_x = 0.0f, normal_side_z = 0.0f;
    for (int i = 0; i <= nr_of_points_cylinder; i++) {
        normal_side_x = glm::cos(delta_degree * i);
        normal_side_z = glm::sin(delta_degree * i);

        cylinderVertices[offset_side + i * 12] = glm::cos(delta_degree * i) * cylinder_radius;
        cylinderVertices[offset_side + i * 12 + 1] = cylinder_height;
        cylinderVertices[offset_side + i * 12 + 2] = glm::sin(delta_degree * i) * cylinder_radius;
        cylinderVertices[offset_side + i * 12 + 4] = normal_side_x;
        cylinderVertices[offset_side + i * 12 + 3] = 0;
        cylinderVertices[offset_side + i * 12 + 5] = normal_side_z;
        cylinderVertices[offset_side + i * 12 + 6] = glm::cos(delta_degree * i) * cylinder_radius;
        cylinderVertices[offset_side + i * 12 + 7] = 0;
        cylinderVertices[offset_side + i * 12 + 8] = glm::sin(delta_degree * i) * cylinder_radius;
        cylinderVertices[offset_side + i * 12 + 9] = normal_side_x;
        cylinderVertices[offset_side + i * 12 + 10] = 0;
        cylinderVertices[offset_side + i * 12 + 11] = normal_side_z;
    }
    unsigned int cylinderVBO, cylinderVAO;
    glGenVertexArrays(1, &cylinderVAO);
    glGenBuffers(1, &cylinderVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cylinderVBO);
    glBufferData(GL_ARRAY_BUFFER, number_of_vertices_cylinder * 6 * sizeof(float), cylinderVertices, GL_STATIC_DRAW);
    glBindVertexArray(cylinderVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    /// Cone
    float cone_height = 1.0f,
            cone_radius = 1.0f;
    int nr_of_points_cone = 72;
    float delta_degree_cone = 2 * M_PI / nr_of_points_cone;
    int number_of_vertices_cone = (nr_of_points_cylinder + 1) * 2 + 2;
    float* coneVertices = new float[number_of_vertices_cylinder * 6];
    // top
    const int offset_top_begin_cone = 0;
    coneVertices[0] = 0;
    coneVertices[1] = cone_height;
    coneVertices[2] = 0;
    coneVertices[3] = 0;
    coneVertices[4] = 1;
    coneVertices[5] = 0;
    const float normal_top_z = cone_radius / cone_height;
    const int offset_top_cone = offset_top_begin_cone + 6;
    for (int i = 0; i <= nr_of_points_cone; i++) {
        coneVertices[offset_top_cone + i * 6] = glm::cos(delta_degree_cone * i) * cone_radius;
        coneVertices[offset_top_cone + i * 6 + 1] = 0;
        coneVertices[offset_top_cone + i * 6 + 2] = glm::sin(delta_degree_cone * i) * cone_radius;
        coneVertices[offset_top_cone + i * 6 + 3] = glm::cos(delta_degree_cone * i);
        coneVertices[offset_top_cone + i * 6 + 4] = normal_top_z;
        coneVertices[offset_top_cone + i * 6 + 5] = glm::sin(delta_degree_cone * i);
    }
    // bottom
    const int offset_bottom_begin_cone = offset_top_cone + (nr_of_points_cone + 1) * 6;
    coneVertices[offset_bottom_begin_cone] = 0;
    coneVertices[offset_bottom_begin_cone + 1] = 0;
    coneVertices[offset_bottom_begin_cone + 2] = 0;
    coneVertices[offset_bottom_begin_cone + 3] = 0;
    coneVertices[offset_bottom_begin_cone + 4] = -1;
    coneVertices[offset_bottom_begin_cone + 5] = 0;
    const int offset_bottom_cone = offset_bottom_begin_cone + 6;
    for (int i = 0; i <= nr_of_points_cone; i++) {
        coneVertices[offset_bottom_cone + i * 6] = glm::cos(delta_degree_cone * i) * cone_radius;
        coneVertices[offset_bottom_cone + i * 6 + 1] = 0;
        coneVertices[offset_bottom_cone + i * 6 + 2] = glm::sin(delta_degree_cone * i) * cone_radius;
        coneVertices[offset_bottom_cone + i * 6 + 3] = 0;
        coneVertices[offset_bottom_cone + i * 6 + 4] = -1;
        coneVertices[offset_bottom_cone + i * 6 + 5] = 0;
    }
    unsigned int coneVBO, coneVAO;
    glGenVertexArrays(1, &coneVAO);
    glGenBuffers(1, &coneVBO);
    glBindBuffer(GL_ARRAY_BUFFER, coneVBO);
    glBufferData(GL_ARRAY_BUFFER, number_of_vertices_cone * 6 * sizeof(float), coneVertices, GL_STATIC_DRAW);
    glBindVertexArray(coneVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);


    /// Sphere
    float sphere_radius = 1.0f;
    int nr_of_points_sphere = 64;
    int nr_of_stacks_sphere = 64;
    float delta_theta = 2 * M_PI / nr_of_points_sphere;
    float delta_phi = M_PI / nr_of_stacks_sphere;
    int number_of_vertices_sphere = (1 + nr_of_points_sphere + 1) * 2 + (nr_of_stacks_sphere - 1) * (nr_of_points_sphere + 1) * 2;
    float* sphereVertices = new float[number_of_vertices_sphere * 6];
    // north pole
    const int offset_north_pole_begin = 0;
    sphereVertices[0] = 0;
    sphereVertices[1] = sphere_radius;
    sphereVertices[2] = 0;
    sphereVertices[3] = 0;
    sphereVertices[4] = 1;
    sphereVertices[5] = 0;
    const int offset_north_pole = offset_north_pole_begin + 6;
    for (int i = 0; i <= nr_of_points_sphere; i++) {
        sphereVertices[offset_north_pole + i * 6] = glm::cos(delta_theta * i) * glm::sin(delta_phi) * sphere_radius;
        sphereVertices[offset_north_pole + i * 6 + 1] = glm::cos(delta_phi) * sphere_radius;
        sphereVertices[offset_north_pole + i * 6 + 2] = glm::sin(delta_theta * i) * glm::sin(delta_phi) * sphere_radius;
        sphereVertices[offset_north_pole + i * 6 + 3] = glm::cos(delta_theta * i) * glm::sin(delta_phi);
        sphereVertices[offset_north_pole + i * 6 + 4] = glm::cos(delta_phi);
        sphereVertices[offset_north_pole + i * 6 + 5] = glm::sin(delta_theta * i) * glm::sin(delta_phi);
    }
    // south pole
    const int offset_south_pole_begin = offset_north_pole + (nr_of_points_sphere + 1) * 6;
    sphereVertices[offset_south_pole_begin] = 0;
    sphereVertices[offset_south_pole_begin + 1] = -sphere_radius;
    sphereVertices[offset_south_pole_begin + 2] = 0;
    sphereVertices[offset_south_pole_begin + 3] = 0;
    sphereVertices[offset_south_pole_begin + 4] = -1;
    sphereVertices[offset_south_pole_begin + 5] = 0;
    const int offset_south_pole = offset_south_pole_begin + 6;
    for (int i = 0; i <= nr_of_points_sphere; i++) {
        sphereVertices[offset_south_pole + i * 6] = glm::cos(delta_theta * i) * glm::sin(M_PI - delta_phi) * sphere_radius;
        sphereVertices[offset_south_pole + i * 6 + 1] = glm::cos(M_PI - delta_phi) * sphere_radius;
        sphereVertices[offset_south_pole + i * 6 + 2] = glm::sin(delta_theta * i) * glm::sin(M_PI - delta_phi) * sphere_radius;
        sphereVertices[offset_south_pole + i * 6 + 3] = glm::cos(delta_theta * i) * glm::sin(M_PI - delta_phi);
        sphereVertices[offset_south_pole + i * 6 + 4] = glm::cos(M_PI - delta_phi);
        sphereVertices[offset_south_pole + i * 6 + 5] = glm::sin(delta_theta * i) * glm::sin(M_PI - delta_phi);
    }
    // side
    const int offset_side_sphere = offset_south_pole + (nr_of_points_sphere + 1) * 6;
    for (int i = 1; i < nr_of_stacks_sphere; i++) {
        for (int j = 0; j <= nr_of_points_sphere; j++) {
            sphereVertices[offset_side_sphere + (i * (nr_of_points_sphere + 1) + j) * 12] = glm::cos(delta_theta * j) * glm::sin(delta_phi * i) * sphere_radius;
            sphereVertices[offset_side_sphere + (i * (nr_of_points_sphere + 1) + j) * 12 + 1] = glm::cos(delta_phi * i) * sphere_radius;
            sphereVertices[offset_side_sphere + (i * (nr_of_points_sphere + 1) + j) * 12 + 2] = glm::sin(delta_theta * j) * glm::sin(delta_phi * i) * sphere_radius;
            sphereVertices[offset_side_sphere + (i * (nr_of_points_sphere + 1) + j) * 12 + 3] = glm::cos(delta_theta * j) * glm::sin(delta_phi * i);
            sphereVertices[offset_side_sphere + (i * (nr_of_points_sphere + 1) + j) * 12 + 4] = glm::cos(delta_phi * i);
            sphereVertices[offset_side_sphere + (i * (nr_of_points_sphere + 1) + j) * 12 + 5] = glm::sin(delta_theta * j) * glm::sin(delta_phi * i);

            sphereVertices[offset_side_sphere + (i * (nr_of_points_sphere + 1) + j) * 12 + 6] = glm::cos(delta_theta * j) * glm::sin(delta_phi * (i + 1)) * sphere_radius;
            sphereVertices[offset_side_sphere + (i * (nr_of_points_sphere + 1) + j) * 12 + 7] = glm::cos(delta_phi * (i + 1)) * sphere_radius;
            sphereVertices[offset_side_sphere + (i * (nr_of_points_sphere + 1) + j) * 12 + 8] = glm::sin(delta_theta * j) * glm::sin(delta_phi * (i + 1)) * sphere_radius;
            sphereVertices[offset_side_sphere + (i * (nr_of_points_sphere + 1) + j) * 12 + 9] = glm::cos(delta_theta * j) * glm::sin(delta_phi * (i + 1));
            sphereVertices[offset_side_sphere + (i * (nr_of_points_sphere + 1) + j) * 12 + 10] = glm::cos(delta_phi * (i + 1));
            sphereVertices[offset_side_sphere + (i * (nr_of_points_sphere + 1) + j) * 12 + 11] = glm::sin(delta_theta * j) * glm::sin(delta_phi * (i + 1));
        }
    }
    unsigned int sphereVBO, sphereVAO;
    glGenVertexArrays(1, &sphereVAO);
    glGenBuffers(1, &sphereVBO);
    glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
    glBufferData(GL_ARRAY_BUFFER, number_of_vertices_sphere * 6 * sizeof(float), sphereVertices, GL_STATIC_DRAW);
    glBindVertexArray(sphereVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    /// Light Cubes
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 线框模式
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    /// Render Loop
    while(!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        /// Plane rendering
        planeShader.use();
        planeShader.setMat4("projection", projection);
        planeShader.setMat4("view", view);
        // set light uniforms
        planeShader.setVec3("viewPos", camera.Position);
        planeShader.setVec3("pointLights[0].position", pointLightPositions[0]);
        planeShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        planeShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        planeShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        planeShader.setFloat("pointLights[0].constant", 1.0f);
        planeShader.setFloat("pointLights[0].linear", 0.09f);
        planeShader.setFloat("pointLights[0].quadratic", 0.032f);

        planeShader.setVec3("pointLights[1].position", pointLightPositions[1]);
        planeShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        planeShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        planeShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        planeShader.setFloat("pointLights[1].constant", 1.0f);
        planeShader.setFloat("pointLights[1].linear", 0.09f);
        planeShader.setFloat("pointLights[1].quadratic", 0.032f);
        // floor
        glBindVertexArray(planeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, woodTexture);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        /// Light Shader for Cube, Cylinder, Cone, Sphere
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);
        lightingShader.setVec3("material.ambient",  1.0f, 0.5f, 0.31f);
        lightingShader.setVec3("material.diffuse",  1.0f, 0.5f, 0.31f);
        lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
        lightingShader.setFloat("material.shininess", 32.0f);

        lightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
        lightingShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights[0].constant", 1.0f);
        lightingShader.setFloat("pointLights[0].linear", 0.09f);
        lightingShader.setFloat("pointLights[0].quadratic", 0.032f);

        lightingShader.setVec3("pointLights[1].position", pointLightPositions[1]);
        lightingShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights[1].constant", 1.0f);
        lightingShader.setFloat("pointLights[1].linear", 0.09f);
        lightingShader.setFloat("pointLights[1].quadratic", 0.032f);

        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        /// Cube rendering
        glm::mat4 cube_model = glm::mat4(1.0f);
        lightingShader.setMat4("model", cube_model);
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        /// Cylinder rendering
        glBindVertexArray(cylinderVAO);
        lightingShader.setVec3("material.diffuse",  0.0f, 0.5f, 0.5f);
        glm::mat4 cylinder_model = glm::mat4(1.0f);
        cylinder_model = glm::scale(cylinder_model, glm::vec3(0.5f, 2.0f, 0.5f));
        cylinder_model = glm::translate(cylinder_model, glm::vec3(-2.5f, -0.25f, 0.0f));
        lightingShader.setMat4("model", cylinder_model);
        glDrawArrays(GL_TRIANGLE_FAN, 0, nr_of_points_cylinder + 2);
        glDrawArrays(GL_TRIANGLE_FAN, nr_of_points_cylinder + 2, nr_of_points_cylinder + 2);
        glDrawArrays(GL_TRIANGLE_STRIP, nr_of_points_cylinder * 2 + 4, nr_of_points_cylinder * 2 + 2);

        /// Cone rendering
        glBindVertexArray(coneVAO);
        lightingShader.setVec3("material.diffuse",  0.5f, 0.5f, 0.0f);
        glm::mat4 cone_model = glm::mat4(1.0f);
        cone_model = glm::scale(cone_model, glm::vec3(0.5f, 1.0f, 0.5f));
        cone_model = glm::translate(cone_model, glm::vec3(2.5f, -0.5f, 0.0f));
        lightingShader.setMat4("model", cone_model);
        glDrawArrays(GL_TRIANGLE_FAN, 0, nr_of_points_cone + 2);
        glDrawArrays(GL_TRIANGLE_FAN, nr_of_points_cone + 2, nr_of_points_cone + 2);

        /// Sphere rendering
        glBindVertexArray(sphereVAO);
        lightingShader.setVec3("material.diffuse",  0.5f, 0.0f, 0.5f);
        glm::mat4 sphere_model = glm::mat4(1.0f);
        sphere_model = glm::scale(sphere_model, glm::vec3(0.5f, 0.5f, 0.5f));
        sphere_model = glm::translate(sphere_model, glm::vec3(0.0f, 2.0f, 0.0f));
        lightingShader.setMat4("model", sphere_model);
        glDrawArrays(GL_TRIANGLE_FAN, 0, nr_of_points_sphere + 2);
        glDrawArrays(GL_TRIANGLE_FAN, nr_of_points_sphere + 2, nr_of_points_sphere + 2);
        glDrawArrays(GL_TRIANGLE_STRIP, (nr_of_points_sphere + 1) * 2, number_of_vertices_sphere - (nr_of_points_sphere + 1) * 2);

        /// Light Cube rendering
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);

        glm::mat4 lightCube_model = glm::mat4(1.0f);
        glBindVertexArray(lightCubeVAO);
        for (unsigned int i = 0; i < 2; i++)
        {
            lightCube_model = glm::mat4(1.0f);
            lightCube_model = glm::translate(lightCube_model, pointLightPositions[i]);
            lightCube_model = glm::scale(lightCube_model, glm::vec3(0.2f)); // Make it a smaller cube
            lightCubeShader.setMat4("model", lightCube_model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    /// Release resources
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteVertexArrays(1, &cylinderVAO);
    glDeleteVertexArrays(1, &coneVAO);
    glDeleteVertexArrays(1, &sphereVAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &cylinderVBO);
    glDeleteBuffers(1, &coneVBO);
    glDeleteBuffers(1, &sphereVBO);
    delete[] cylinderVertices;
    delete[] coneVertices;
    delete[] sphereVertices;

    glfwTerminate();
    return 0;
}