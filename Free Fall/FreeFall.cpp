#include <iostream>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// Box2D
#include <box2d/box2d.h>
#include "../Shader.hpp"
// Since Box2D uses MKS System
const float M2P = 34;
const float P2M = 1/M2P;

b2Vec2 gravity(0.0f, -25.0f);
b2World world(gravity);

GLuint VAO, VBO;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);


b2Body* CreateBox2DRect(double x, double y, double hx, double hy, bool isDynamic, double density, double friction)
{
    b2BodyDef bodyDef;
    bodyDef.position.Set(x * P2M, y * P2M);

    if(isDynamic)
        bodyDef.type = b2_dynamicBody;

    b2Body* body = world.CreateBody(&bodyDef);

    b2PolygonShape shape;
    shape.SetAsBox(P2M * hx, P2M * hy);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    body->CreateFixture(&fixtureDef);

    return body;
}

void RenderPhysicsBodies(b2Vec2* points,b2Vec2 center,float angle, Shader* shader){

    GLfloat vertices[8] = {
        points[3].x, points[3].y,
        points[0].x, points[0].y,
        points[2].x, points[2].y,
        points[1].x, points[1].y
    };
    // for(int i = 0; i < 8 ; i++){
    //     vertices[i] = i % 2 == 0 ? points[(GLint)i / 2].x : points[(GLint)i / 2].y;
    // }
    // for(int i = 0; i < 8 ; i++){
    //     std::cout << vertices[i] << '\n';
    // }
    GLfloat quadVertices[] = {
        -0.5,   0.5,
        -0.5,  -0.5,
         0.5,   0.5,
         0.5,  -0.5
    };


    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_TRUE, 2 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Apply Transformations here
    glm::mat4 model(1.0f), view(1.0f), projection(1.0f);
    // std::cout << "X center is :" << center.x << "Y center is :" << center.y << '\n';
    model = glm::translate(model, glm::vec3(center.x, center.y, 0.0f));
    model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));
    // GLfloat radius = 10.0f;
    // GLfloat camX = sin(glfwGetTime()) * radius;
    // GLfloat camZ = cos(glfwGetTime()) * radius;
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    projection = glm::perspective(45.0f, (GLfloat)800 / (GLfloat)600, 0.1f, 100.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader->Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shader->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

}

void GetPhyBodiesToRender(Shader* shader){

    b2Body* currBody = world.GetBodyList();
    b2Vec2 points[4];
    while (currBody){
        for(int i=0;i<4;i++)
        {
            points[i] = ((b2PolygonShape*)currBody->GetFixtureList()->GetShape())->m_vertices[i];
        }
        RenderPhysicsBodies(points, currBody->GetWorldCenter(), currBody->GetAngle(), shader);
        currBody = currBody->GetNext();
    }
}



void glfw_initialisation_error(int error, const char* description){
    std::cerr << "ERROR::INITIALISATION::GLFW::" << error << "::DESCRIPTION::" << description << std::endl;
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
    window = glfwCreateWindow(Width, Height, "OpenGL Window", NULL, NULL);
    if(!window){
        std::cerr << "Could Not initialise Window" << '\n';
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);

    /* GLFW Callbacks */
    glfwSetKeyCallback(window, key_callback);

    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK){
        std::cerr << "Could Not initialise GLEW" << '\n';
        return EXIT_FAILURE;
    }

    glViewport(0, 0, Width, Height);


    Shader defaultShader("../default.vert", "../default.frag");


    CreateBox2DRect(20, 25, 10, 5, true, 1, 0.2);
    // CreateBox2DRect(0, 10, 10, 5, false, 1, 0.2);



    while (!glfwWindowShouldClose(window)) {

        float timeStep = 1.0f / 60.0f; // Physics time step
        int32 velocityIterations = 6;
        int32 positionIterations = 2;

        world.Step(timeStep, velocityIterations, positionIterations);//Physics Update

        GetPhyBodiesToRender(&defaultShader);

        glfwPollEvents();

        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);


        defaultShader.Use();


        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        // glBindVertexArray(0);

        glfwSwapBuffers(window);
    }
    glfwTerminate();

    return EXIT_SUCCESS;
}

// clang++ -std=c++17 -framework OpenGL -I /Users/phanisrika
// r/Desktop/Physics-Simulations-Box2D/ExternalLibraries/Box2D/include -I /Users/phanisrikar/Libraries
// -L /Users/phanisrikar/Desktop/Physics-Simulations-Box2D/ExternalLibraries/Box2D/lib -lglew -lglfw -l
// box2d FreeFall.cpp -o freefall &&./freefall
