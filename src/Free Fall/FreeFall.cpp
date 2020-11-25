#include <stdlib.h>

#include <Renderer.h>
// Box2D
#include <box2d/box2d.h>

//------------------------------------------------------------------------------
// Global Variable
//------------------------------------------------------------------------------
const int Width = 800, Height = 600;

glm::vec2 cursorPosition;
// View Projection
glm::mat4 view(1.0f);
glm::mat4 projection(1.0f);

Renderer renderer(view, projection);

std::vector<Rigidbody2D> boxes;
std::vector<glm::vec4> boxes_colors;
//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void mouse_position_callback(GLFWwindow* window, double xpos, double ypos);
template <class T>
T inline clamp(T number, T min, T max, T maxX, T minX)
{
    return (max - min) * ((number - minX) / (maxX - minX)) + min;
}
//------------------------------------------------------------------------------
// Main Funciton
//------------------------------------------------------------------------------
int main(int argc, char const *argv[])
{
    srand(1);

    if(!glfwInit())
        return EXIT_FAILURE;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window;
    window = glfwCreateWindow(Width, Height, "Free Fall Simulation", NULL, NULL);
    if(!window){
        std::cerr << "Could Not initialise Window" << '\n';
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);

    /* GLFW Callbacks */
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, mouse_position_callback);

    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK){
        std::cerr << "Could Not initialise GLEW" << '\n';
        return EXIT_FAILURE;
    }

    glViewport(0, 0, Width, Height);
//------------------------------------------------------------------------------
// OpenGL Data and Setup
//------------------------------------------------------------------------------

    float quadVertices[] = {
        -1.0f, -1.0f,
        -1.0f,  1.0f,
         1.0f,  1.0f,
         1.0f, -1.0f
    };

    unsigned int quadIndices[]=
    {
        0, 1, 2,
        2, 3, 0
    };

    Shader defaultShader("res/shaders/default.vert", "res/shaders/default.frag");

    VertexArray quad_VAO;
    VertexBuffer quad_VBO(quadVertices, sizeof(quadVertices));
    IndexBuffer quad_IBO(quadIndices, 6);
    VertexBufferLayout quad_layout;
    quad_layout.Push<float>(2);
    quad_VAO.Bind();
    quad_VBO.Bind();
    quad_IBO.Bind();
    quad_VAO.AddBuffer(quad_VBO, quad_layout);
//------------------------------------------------------------------------------
    Transform box_Transform;
    box_Transform.scale = glm::vec3(10, 10, 1);
    Rigidbody2D box(glm::vec3(0, 100, 0), glm::vec2(10, 10), 1.0f, 0.3f, Dynamic);

    Transform plank_Transform;
    plank_Transform.scale = glm::vec3(80, 5, 1);
    Rigidbody2D plank(glm::vec3(0, -60, 0), glm::vec2(80, 5), 1.0f, 1.0f, Static);

//------------------------------------------------------------------------------
    projection  = glm::ortho(-160.0f, +160.0f, -120.0f, +120.0f, -1.0f, +1.0f);

    GLfloat currentFrame = 0.0f;
    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;
    GLint FPS = 0;

//------------------------------------------------------------------------------
// Game Loop
//------------------------------------------------------------------------------
    while (!glfwWindowShouldClose(window))
    {
        float timeStep = 1.0f / 20.0f;
        int velocityIterations = 4;
        int positionIterations = 3;

        PhysicsWorld.Step(timeStep, velocityIterations, positionIterations);

        box_Transform.position = box.GetPositionInPixels();
        box_Transform.rotation = glm::vec3(0, 0, box.GetRotation());

        plank_Transform.position = plank.GetPositionInPixels();
        plank_Transform.rotation = glm::vec3(0, 0, plank.GetRotation());

        glfwPollEvents();

        glClearColor(0.13, 0.13, 0.13, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        defaultShader.SetUniform4f("u_Color", glm::vec4(1, 0, 0, 1));
        renderer.draw_raw_indices(box_Transform, defaultShader, quad_VAO, quad_IBO);
        defaultShader.SetUniform4f("u_Color", glm::vec4(1, 1, 0, 1));
        renderer.draw_raw_indices(plank_Transform, defaultShader, quad_VAO, quad_IBO);

        for (int i = 0; i < boxes.size(); i++)
        {
            defaultShader.SetUniform4f("u_Color", boxes_colors[i]);
            Transform rb_Transform(boxes[i].GetPositionInPixels(), glm::vec3(0, 0, boxes[i].GetRotation()), glm::vec3(10, 10, 1));
            renderer.draw_raw_indices(rb_Transform, defaultShader, quad_VAO, quad_IBO);
        }

        glfwSwapBuffers(window);
    }
    glfwTerminate();

    return EXIT_SUCCESS;
}
//------------------------------------------------------------------------------
// Function Definitions
//------------------------------------------------------------------------------
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        Rigidbody2D rb(glm::vec3(cursorPosition.x, cursorPosition.y, 0), glm::vec2(10, 10), 0.6f, 1.0f, Dynamic);
        boxes.push_back(rb);
        boxes_colors.push_back(glm::vec4(((float) rand() / RAND_MAX), ((float) rand() / RAND_MAX), ((float) rand() / RAND_MAX), 1.0f));
    }
}
void mouse_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    cursorPosition.x = clamp(xpos, -160.0, 160.0, 800.0, 0.0);
    cursorPosition.y = clamp(ypos, -120.0, 120.0, 0.0, 600.0);
}
//------------------------------------------------------------------------------
