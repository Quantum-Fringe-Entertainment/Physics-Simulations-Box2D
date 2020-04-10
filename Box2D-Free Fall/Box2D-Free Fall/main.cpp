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
//Include Box2D here
#include <box2d/box2d.h>
#include "b2GLDraw.hpp"


#define WIDTH 500
#define HEIGHT 500
int screenWidth, screenHeight;
//Since Box2D uses MKS system hame proper conversion karna hai pixels se meters aour peche se bhi.
const float M2P = 30;
const float P2M = 1/M2P;
//Define the gravity vector.
b2Vec2 gravity(0.0f, -5.0f);
// Construct a world object, which will hold and simulate the rigid bodies.
b2World world(gravity);

// MARK: - Box2D object creation Function
b2Body* CreateBox2DRect(double x, double y, double hx, double hy, bool isDynamic, double density, double friction)
{
     b2BodyDef bodydef;
     bodydef.position.Set(x*P2M,y*P2M);

     if(isDynamic)
             bodydef.type = b2_dynamicBody;
    b2Body* body = world.CreateBody(&bodydef);
    
     b2PolygonShape shape;
     shape.SetAsBox(P2M*hx/2,P2M*hy/2);
    
     b2FixtureDef fixturedef;
     fixturedef.shape = &shape;
     fixturedef.density = density;
     fixturedef.friction = friction;
     body->CreateFixture(&fixturedef);
    
    return body;
}


// MARK: - OpenGl object Rendering Function
void RenderRect(b2Vec2* points,b2Vec2 center,float angle)//Rendering is done using coordinate data from the box2d engone aur opegl to show the graphics implementation of it.
{
    glColor3f(1,1,1);
        glPushMatrix();
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

                glTranslatef(center.x ,center.y,0);

                glRotatef(angle*180.0/M_PI,0,0,1);
                    glBegin(GL_QUADS);
                        for(int i=0;i<4;i++)
                            glVertex2f(points[i].x ,points[i].y );
                glEnd();
        glPopMatrix();
}



// MARK: - Display Function
void Display()
{
    glLoadIdentity();
    b2Body* tmp = world.GetBodyList();
    b2Vec2 points[4];
    while(tmp)
    {
        for(int i=0;i<4;i++)
        {
             points[i]=((b2PolygonShape*)tmp->GetFixtureList()->GetShape())->m_vertices[i];
        }
        //Here we render all the objects from linked lilsts and ge the position from th ephysics engine
//        float angle = tmp->GetWorldCenter().y;
//        std::cout << angle << std::endl;
        RenderRect(points, tmp->GetWorldCenter(), tmp->GetAngle());
        tmp=tmp->GetNext();
    }
}

static void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);

// MARK: - Main Function


int main(){
    // MARK: - OpenGL Initiliasation and Setup

    //Initialise glfw here
    glfwInit();
    
    
    //chalo now let's init GLFW here
    glfwInit();


    
    //now time to create the window
    
    //Make a window instance here
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Free Fall Simulation", NULL, NULL);
    
    if(window == NULL)
    {
        std::cout << "Failed to create GLFW Window" << std::endl;
        glfwTerminate();
        
        return -1;
    }
    
    
    // Make the window's context current
    glfwMakeContextCurrent( window );
    
    //What this line of code actually does is it gets the actual width of the screen window itself, relative to the density of the screen.
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
    //Input handlinn and call habacks here
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetCursorPosCallback(window, CursorPositionCallback);
    
    
    // MARK: - Box2D Physics Bodies Initilialisation
    //Ground Plane
    CreateBox2DRect(0, -25, 50, 5, false, 1, 0.2);
    //create dynami box here
    CreateBox2DRect(0, 25, 5, 5, false, 1, 0.2);
    CreateBox2DRect(24, 24, 5, 5, false, 1, 0.2);

    b2GLDraw debugInstance;
         world.SetDebugDraw(&debugInstance);
         uint32_t flags = 0;
         flags += b2Draw::e_shapeBit;
//         flags += b2Draw::e_jointBit;
//         flags += b2Draw::e_aabbBit;
//         flags += b2Draw::e_pairBit;
//         flags += b2Draw::e_centerOfMassBit;
         debugInstance.SetFlags(flags);
    
    // MARK: - Game Loop
    
    glfwSetCursorPosCallback(window, CursorPositionCallback);

   while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
    glfwWindowShouldClose(window) == 0 ){
           // Check if any events have been activiated (key pressed,
           //mouse moved etc.) and call corresponding response functions

        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
     
       
       //  Instruct the world to perform a single step of simulation.
       //         It is generally best to keep the time step and iterations fixed.
               float timeStep = 1.0f / 60.0f;
               int32 velocityIterations = 6;
               int32 positionIterations = 2;
       
               world.Step(timeStep, velocityIterations, positionIterations);//Physics Update
// MARK: Render Graphics and Update Physics here
       
       world.DebugDraw();
       Display();
     

 
          glfwSwapBuffers(window);//IDK why this is used.
        //This function swaps the front and back buffers of the specified window. If the swap interval is greater than zero, the GPU driver waits the specified number of screen updates before swapping the buffers.
        //https://www.glfw.org/docs/3.0/group__context.html#ga15a5a1ee5b3c2ca6b15ca209a12efd14
       glfwPollEvents();

    }
    glfwTerminate();

    return 0;
}

static void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
//    CreateBox2DRect( (10) , (10), 1, 1, true, 1, 0.2);
    CreateBox2DRect((xpos - (WIDTH/2))/10 * 2, -(ypos - (HEIGHT/2))/10 , 1, 1, true, 1, 0.2);
    std::cout << xpos << ", " << ypos << std::endl;
    std::cout << (xpos - (WIDTH/2))/10 * 2<< " : " << -(ypos - (HEIGHT/2))/10 << std::endl;
//    std::cout << (xpos - (WIDTH/2))*P2M << ", " << (ypos - (HEIGHT/2))*P2M  << std::endl;
    
    
    
}
