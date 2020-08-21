#include <iostream>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

void glfw_initialisation_error(int error, const char* description){
    std::cerr << "ERROR::INITIALISATIO::GLFW::" << error << "::DESCRIPTION::" << description << std::endl;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        std::cout << "Escape Key Pressed..." << '\n';
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}
int main(int argc, char const *argv[]) {

    const int Width = 800, Height = 600;

    glfwSetErrorCallback(glfw_initialisation_error);
    if(!glfwInit())
        return EXIT_FAILURE;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window;
    window = glfwCreateWindow(Width, Height, "Exercise 6", NULL, NULL);
    if(!window){
        std::cerr << "Could Not initialise Window" << '\n';
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable V-Sync

    glfwSetKeyCallback(window, key_callback);

    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK){
        std::cerr << "Could Not initialise GLEW" << '\n';
        return EXIT_FAILURE;
    }

    glViewport(0, 0, Width, Height);


    while (!glfwWindowShouldClose(window)) {

        glfwPollEvents();

        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);


        glfwSwapBuffers(window);
    }
    glfwTerminate();

    return EXIT_SUCCESS;
}
