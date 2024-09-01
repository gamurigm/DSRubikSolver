// OpenGL
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

// Rubik cube
#include "RubikCube.h"

// STD libraries
#include <iostream>
#include <random>
#include <vector>
// learnopenGL libraries
#include "shader.h"
#include "camera.h"
#include "defineRubik.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

float random(float a, float b);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool pressedSolve = false;
// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// rubik cube
RubikCube3x3::STATE_ANIMATION animation_state = RubikCube3x3::STATE_ANIMATION::NONE;
RubikCube3x3::PAINT_MODE paint_mode = RubikCube3x3::PAINT_MODE::STATIC;
enum class PROYECTION_TYPE { PERSPECTIVE, ORTHOGONAL };
PROYECTION_TYPE proy_type = PROYECTION_TYPE::PERSPECTIVE;

int main()
{
    // inicializar glfw
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // crear ventana con glfw 
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Rubik Cube", NULL, NULL);
    if (window == NULL) 
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // CONFIGURANDO GLFW PARA CAPTURAR EL MOUSE
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: cargar todos los punteros de GLAD
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    // set up vertex data (and buffer(s)) and configure vertex attributes
// ------------------------------------------------------------------
    GLfloat sun_vertices[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
    };

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);



    // build and compile our shader zprogram
    // ------------------------------------
    GLuint VBO[1], VAO[1], EBO[1];
    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);
    glGenBuffers(1, EBO);

    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_fill_vertices), cube_fill_vertices, GL_STATIC_DRAW);
    // atributo de posicion
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // atributo de color


    Shader program("vertexShader.glsl", "fragmentShader.glsl");
    RubikCube3x3 rubik(program);
    //asociamos el VAO correspondiente
    rubik.AssociateVAO(VAO[0]);
    
    glLineWidth(10.0f); // Delineado del cubo
    // loop de renderizado
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // Obtener delta time
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Procesar las entradas (teclado, raton etc)
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



        // camera/view transformaciones
        glm::mat4 view = camera.GetViewMatrix(); 
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        if(proy_type == PROYECTION_TYPE::ORTHOGONAL)
            projection = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, 0.1f, 100.0f);

        // dibujamos el cubo de rubik
        if (animation_state == RubikCube3x3::STATE_ANIMATION::SOLVE) 
            rubik.Solve(animation_state);
        rubik.HandleDrawing(view, projection, animation_state, paint_mode);

        glfwSwapBuffers(window); //Refrescar la pantalla
        glfwPollEvents();
    }

    // Limpiar recuersos utilizados al cerrar el programa
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, VAO);
    glDeleteBuffers(1, VBO);
    glDeleteBuffers(1, EBO);
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (animation_state == RubikCube3x3::STATE_ANIMATION::NONE) {
        if (key == GLFW_KEY_KP_0 && action == GLFW_PRESS)
            animation_state = RubikCube3x3::STATE_ANIMATION::F;
        if (key == GLFW_KEY_KP_1 && action == GLFW_PRESS)
            animation_state = RubikCube3x3::STATE_ANIMATION::f;
        if (key == GLFW_KEY_KP_2 && action == GLFW_PRESS)
            animation_state = RubikCube3x3::STATE_ANIMATION::R;
        if (key == GLFW_KEY_KP_3 && action == GLFW_PRESS)
            animation_state = RubikCube3x3::STATE_ANIMATION::r;
        if (key == GLFW_KEY_KP_4 && action == GLFW_PRESS)
            animation_state = RubikCube3x3::STATE_ANIMATION::U;
        if (key == GLFW_KEY_KP_5 && action == GLFW_PRESS)
            animation_state = RubikCube3x3::STATE_ANIMATION::u;
        if (key == GLFW_KEY_KP_6 && action == GLFW_PRESS)
            animation_state = RubikCube3x3::STATE_ANIMATION::B;
        if (key == GLFW_KEY_KP_7 && action == GLFW_PRESS)
            animation_state = RubikCube3x3::STATE_ANIMATION::b;
        if (key == GLFW_KEY_KP_8 && action == GLFW_PRESS)
            animation_state = RubikCube3x3::STATE_ANIMATION::L;
        if (key == GLFW_KEY_KP_9 && action == GLFW_PRESS)
            animation_state = RubikCube3x3::STATE_ANIMATION::l;
        if (key == GLFW_KEY_KP_ADD && action == GLFW_PRESS)
            animation_state = RubikCube3x3::STATE_ANIMATION::D;
        if (key == GLFW_KEY_KP_SUBTRACT && action == GLFW_PRESS)
            animation_state = RubikCube3x3::STATE_ANIMATION::d;
        if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
            animation_state = RubikCube3x3::STATE_ANIMATION::SOLVE;
    }

    if (key == GLFW_KEY_O && action == GLFW_PRESS)
        proy_type = PROYECTION_TYPE::ORTHOGONAL;
    if (key == GLFW_KEY_P && action == GLFW_PRESS)
        proy_type = PROYECTION_TYPE::PERSPECTIVE;
}

// glfw: Funcion que se llama cada vez que actualizamos el tamaño de la ventana
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    //actualizar el tamaño del viewport de openGl segun las nuevas dimensiones
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }


    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: se llama cada vez que el scroll del mouse se use
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

float random(float a, float b) 
{
    std::random_device rd;  
    std::mt19937 gen(rd()); 
    std::uniform_real_distribution<float> dis(a, b);
    return dis(gen);
}