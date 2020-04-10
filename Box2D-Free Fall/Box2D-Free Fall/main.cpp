//
//  main.cpp
//  Box2D-Free Fall
//
//  Created by phani srikar on 10/04/20.
//  Copyright © 2020 phani srikar. All rights reserved.
//

#include <iostream>

//GLEW
#include <GL/glew.h>
//GLEW - V3
#include <GLFW/glfw3.h>


#define WIDTH 600
#define HEIGHT 800

int main(){
    
    //Initialise glfw here
    glfwInit();
    
    
    //chalo now let's configure GLFW here
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    
    //now time to create the window
    
    //Make a window instance here
    GLFWwindow* window = glfwCreateWindow(HEIGHT, WIDTH, "Free Fall Simulation", NULL, NULL);
    
    if(window == NULL)
    {
        std::cout << "Failed to create GLFW Window" << std::endl;
        glfwTerminate();
        
        return -1;
    }
    
    
    // Make the window's context current
    glfwMakeContextCurrent( window );
    
    //What this line of code actually does is it gets the actual width of the screen window itself, relative to the density of the screen.
    int screenWidth, screenHeight;
    glfwGetFramebufferSize( window, &screenWidth, &screenHeight );
    
    
    // Next set gler experimental featues to true to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    
    // abhi et's use g;fw to initiliase the actual window
    glewInit();
    if (GLEW_OK != glewInit())
    {
          std::cout << "Failed to initialize GLEW" << std::endl;
          return -1;
    }
    
     glViewport(0, 0, screenWidth, screenHeight);
    
    // Game loop
    while (!glfwWindowShouldClose(window))
    {
           // Check if any events have been activiated (key pressed,
           //mouse moved etc.) and call corresponding response functions
           glfwPollEvents();

        glClearColor(1, 1, 1, 1)
        glClear(GL_COLOR_BUFFER_BIT);
     
          // Draw Here
        
     
     
          glfwSwapBuffers(window);//IDK why this is used.
        //This function swaps the front and back buffers of the specified window. If the swap interval is greater than zero, the GPU driver waits the specified number of screen updates before swapping the buffers.
        //https://www.glfw.org/docs/3.0/group__context.html#ga15a5a1ee5b3c2ca6b15ca209a12efd14
    }
    glfwTerminate();

    return 0;
}
