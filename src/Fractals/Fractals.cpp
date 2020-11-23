#include <iostream>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
// Custom
#include "Shader.h"



int Width = 800, Height = 600;
double cx = -1.0, cy = -0.5, zoom = 1.0; /* Camera position and zoom value */
int iterations = 300; /* Number of iterations */

bool keys[1024] = { 0 };

void glfw_initialisation_error(int error, const char* description){
    std::cerr << "ERROR::INITIALISATIO::GLFW::" << error << "::DESCRIPTION::" << description << std::endl;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    const double d = 0.1 / zoom;

	if(action == GLFW_PRESS) {
		keys[key] = true;
	} else if(action == GLFW_RELEASE) {
		keys[key] = false;
	}

	if(keys[GLFW_KEY_ESCAPE]) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	} else if(keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) {
		cx -= d;
	} else if(keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) {
		cx += d;
	} else if(keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) {
		cy += d;
	} else if(keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) {
		cy -= d;
	} else if(keys[GLFW_KEY_MINUS] &&
		  iterations < std::numeric_limits <int>::max() - 10) {
		iterations += 10;
	} else if(keys[GLFW_KEY_EQUAL]) {
		iterations -= 10;
		if(iterations <= 0) {
			iterations = 0;
		}
	}
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	zoom += yoffset * 0.1 * zoom;
	if(zoom < 0.1) {
		zoom = 0.1;
	}
}

int main(int argc, char const *argv[]) {


    glfwSetErrorCallback(glfw_initialisation_error);
    if(!glfwInit())
        return EXIT_FAILURE;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window;
    window = glfwCreateWindow(Width, Height, "Fractals", NULL, NULL);
    if(!window){
        std::cerr << "Could Not initialise Window" << '\n';
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);
	glfwSetScrollCallback(window, scroll_callback);

    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK){
        std::cerr << "Could Not initialise GLEW" << '\n';
        return EXIT_FAILURE;
    }

    glViewport(0, 0, Width, Height);



    Shader mandlebrotShader("default.vert", "mandlebrot.frag");

    GLfloat points[] = {
       -1.0f,  1.0f,  0.0f,
       -1.0f,  -1.0f,  0.0f,
       1.0f,  -1.0f,  0.0f,
       1.0f,  1.0f,  0.0f
    };

    GLint indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



    while (!glfwWindowShouldClose(window)) {

        glfwPollEvents();
        glfwGetWindowSize(window, &Width, &Height);



        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUniform2d(glGetUniformLocation(mandlebrotShader.Program, "screen_size"), (double)Width, (double)Height);
        glUniform1d(glGetUniformLocation(mandlebrotShader.Program, "screen_ratio"), (double)Width / (double)Height);
        glUniform2d(glGetUniformLocation(mandlebrotShader.Program, "center"), cx, cy);
        glUniform1d(glGetUniformLocation(mandlebrotShader.Program, "zoom"), zoom);
        glUniform1i(glGetUniformLocation(mandlebrotShader.Program, "itr"), iterations);
        mandlebrotShader.Use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }
    glfwTerminate();

    return EXIT_SUCCESS;
}
