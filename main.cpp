//
//  main.cpp
//  3D Object Drawing
//
//  Created by Nazirul Hasan on 4/9/23.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "basic_camera.h"

#include <iostream>
#include <stdlib.h>
#include<windows.h>  

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void Table(Shader ourshader, glm::mat4 moveMatrix);
void Chair(Shader ourShader, glm::mat4 moveMatrix);
void SideWall(Shader ourShader, glm::mat4 moveMatrix);
void FrontWall(Shader ourShader, glm::mat4 moveMatrix);
void Roof(Shader ourShader, glm::mat4 moveMatrix, glm::vec4 color);
void Floor(Shader ourShader, glm::mat4 moveMatrix, glm::vec4 color);
void BlackBoard(Shader ourShader, glm::mat4 moveMatrix);
void Fan1(Shader ourShader);
void Fan2(Shader ourShader);


// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 600;

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 0.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;

// camera
Camera camera(glm::vec3(0.0f, 1.0f, 2.7f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 1.0, eyeZ = 2.7;
float lookAtX = 0.0, lookAtY = 1.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);

// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;


//rotate
bool isRotate = false;
float fanSpeed = 2.7f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------


    //// For axis
    //float axisVertices[] = {
    //    // X axis          // Color
    //    -1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,            //Red
    //    1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,

    //    // Y axis
    //    0.0f, -1.0f, 0.0f,  0.0f, 1.0f, 0.0f,            //Green
    //    0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,

    //    // Z axis
    //    0.0f, 0.0f, -1.0f,  0.0f, 0.0f, 1.0f,            //Blue
    //    0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f
    //};

    /*float cube_vertices[] = {
        0.0f, 0.0f, 0.0f, 0.3f, 0.8f, 0.5f,
        0.5f, 0.0f, 0.0f, 0.5f, 0.4f, 0.3f,
        0.5f, 0.5f, 0.0f, 0.2f, 0.7f, 0.3f,
        0.0f, 0.5f, 0.0f, 0.6f, 0.2f, 0.8f,
        0.0f, 0.0f, 0.5f, 0.8f, 0.3f, 0.6f,
        0.5f, 0.0f, 0.5f, 0.4f, 0.4f, 0.8f,
        0.5f, 0.5f, 0.5f, 0.2f, 0.3f, 0.6f,
        0.0f, 0.5f, 0.5f, 0.7f, 0.5f, 0.4f
    };*/
    /*float cube_vertices[] = {
        0.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
        0.0f, 0.0f, 0.5f,
        0.5f, 0.0f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.0f, 0.5f, 0.5f
    };*/
    float cube_vertices[] = {
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,

        0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 0.5f, 1.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f,

        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.5f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.5f, 1.0f, 0.0f, 1.0f
    };
    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };
    /*unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        1, 2, 6,
        6, 5, 1,

        5, 6, 7,
        7 ,4, 5,

        4, 7, 3,
        3, 0, 4,

        6, 2, 3,
        3, 7, 6,

        1, 5, 4,
        4, 0, 1
    };*/
    /*float cube_vertices[] = {
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
    };*/
    // world space positions of our cubes
    /*glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };*/
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // position attribute
   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);



    ////For Axis
    //unsigned int axisVBO, axisVAO;
    //glGenVertexArrays(1, &axisVAO);
    //glGenBuffers(1, &axisVBO);
    //glBindVertexArray(axisVAO);
    //glBindBuffer(GL_ARRAY_BUFFER, axisVBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(axisVertices), axisVertices, GL_STATIC_DRAW);
    //// position attribute
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);
    //// color attribute
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);


    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);



    //ourShader.use();


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // activate shader
        ourShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        // camera/view transformation
        //glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);




        ////For axis
        //ourShader.use();
        //glm::mat4 identity = glm::mat4(1.0f); // identity matrix
        //ourShader.setMat4("model", identity);
        //glBindVertexArray(axisVAO);
        //glDrawArrays(GL_LINES, 0, 6);


        glBindVertexArray(VAO);


        // Starting
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, scaleMatrix;
        glm::vec4 color1;

        float chairZ = 0.4f;

        float fixY = 0.45f;

        float chairX = 0.1f;


        // Middle column
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f + fixY, 0.0f));
        Table(ourShader, translateMatrix);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f + chairX, 0.0f + fixY, 1.0f - chairZ));
        Chair(ourShader, translateMatrix);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f + fixY, -1.35f));
        Table(ourShader, translateMatrix);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f + chairX, 0.0f + fixY, -0.35f - chairZ));
        Chair(ourShader, translateMatrix);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f + fixY, -2.7f));
        Table(ourShader, translateMatrix);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f + chairX, 0.0f + fixY, -1.7f - chairZ));
        Chair(ourShader, translateMatrix);

        /*translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f + fixY, -4.05f));
        Table(ourShader, translateMatrix);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f + fixY, -3.05f - chairZ));
        Chair(ourShader, translateMatrix);*/



        //Right column
        translateMatrix = glm::translate(identityMatrix, glm::vec3(1.5f, 0.0f + fixY, 0.0f));
        Table(ourShader, translateMatrix);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(1.5f + chairX, 0.0f + fixY, 1.0f - chairZ));
        Chair(ourShader, translateMatrix);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(1.5f, 0.0f + fixY, -1.35f));
        Table(ourShader, translateMatrix);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(1.5f + chairX, 0.0f + fixY, -0.35f - chairZ));
        Chair(ourShader, translateMatrix);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(1.5f, 0.0f + fixY, -2.7f));
        Table(ourShader, translateMatrix);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(1.5f + chairX, 0.0f + fixY, -1.7f - chairZ));
        Chair(ourShader, translateMatrix);

        /*translateMatrix = glm::translate(identityMatrix, glm::vec3(1.5f, 0.0f + fixY, -4.05f));
        Table(ourShader, translateMatrix);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(1.5f - 0.1f, 0.0f + fixY, -3.05f - chairZ));
        Chair(ourShader, translateMatrix);*/



        //Left column
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.5f, 0.0f + fixY, 0.0f));
        Table(ourShader, translateMatrix);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.5f, 0.0f + fixY, 1.0f - chairZ));
        Chair(ourShader, translateMatrix);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.5f, 0.0f + fixY, -1.35f));
        Table(ourShader, translateMatrix);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.5f, 0.0f + fixY, -0.35f - chairZ));
        Chair(ourShader, translateMatrix);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.5f, 0.0f + fixY, -2.7f));
        Table(ourShader, translateMatrix);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.5f, 0.0f + fixY, -1.7f - chairZ));
        Chair(ourShader, translateMatrix);

        /*translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.5f, 0.0f + fixY, -4.05f));
        Table(ourShader, translateMatrix);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.5f + 0.2f, 0.0f + fixY, -3.05f - chairZ));
        Chair(ourShader, translateMatrix);*/



        //Walls
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
        SideWall(ourShader, translateMatrix);       //Left

        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.5f, 0.0f, 0.0f));
        SideWall(ourShader, translateMatrix);       //Right

        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
        FrontWall(ourShader, translateMatrix);       //Front

        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 7.0f));
        FrontWall(ourShader, translateMatrix);       //Back

        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 2.0f));
        color1 = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f);
        Roof(ourShader, translateMatrix, color1);       //Roof

        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, -1.7f, 2.0f));
        color1 = glm::vec4(0.38f, 0.3f, 0.3f, 1.0f);
        Floor(ourShader, translateMatrix, color1);       //Floor

        

        //BlackBoard
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
        BlackBoard(ourShader, translateMatrix);


        //Fan
        Fan1(ourShader);


        // render boxes
        //for (unsigned int i = 0; i < 10; i++)
        //{
        //    // calculate the model matrix for each object and pass it to shader before drawing
        //    glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        //    model = glm::translate(model, cubePositions[i]);
        //    float angle = 20.0f * i;
        //    model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        //    ourShader.setMat4("model", model);

        //    glDrawArrays(GL_TRIANGLES, 0, 36);
        //}

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {                 //Forward
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {                 //Backward
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {                 //Left
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {                 //Right
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)                   //Down
    {
        /*if (rotateAxis_X) rotateAngle_X -= 0.1;
        else if (rotateAxis_Y) rotateAngle_Y -= 0.1;
        else rotateAngle_Z -= 0.1;*/

        camera.ProcessKeyboard(DOWN, deltaTime);


    }

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)                   //Pitch positive
    {
        /*rotateAngle_X += 1;
        rotateAxis_X = 1.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 0.0;*/
        camera.ProcessYPR(0.0f, 3.0f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)                   //Pitch negative
    {
        /*rotateAngle_X += 1;
        rotateAxis_X = 1.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 0.0;*/
        camera.ProcessYPR(0.0f, -3.0f, 0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)                   //Yaw positive
    {
        /*rotateAngle_Y += 1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 1.0;
        rotateAxis_Z = 0.0;*/
        camera.ProcessYPR(3.0f, 0.0f, 0.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)                   //Yaw negative
    {
        /*rotateAngle_Y += 1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 1.0;
        rotateAxis_Z = 0.0;*/
        camera.ProcessYPR(-3.0f, 0.0f, 0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)                   //Roll positive
    {
        /*rotateAngle_Z += 0.1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 1.0;*/
        camera.ProcessYPR(0.0f, 0.0f, 0.5f);

    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)                   //Roll negative
    {
        /*rotateAngle_Z += 0.1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 1.0;*/
        camera.ProcessYPR(0.0f, 0.0f, -0.5f);

    }

    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
    {
        eyeX += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)                   //Rotate camera around a look at point
    {
        /*eyeX -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);*/


    }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    {
        eyeZ += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)                   //Rotate Fan
    {
        /*eyeZ -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);*/
        isRotate ^= true;
        cout << isRotate << endl;
        Sleep(100);

    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        eyeY += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)                   //Up
    {
        /*eyeY -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);*/
        camera.ProcessKeyboard(UP, deltaTime);

    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        lookAtX += 2.5 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        lookAtX -= 2.5 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        lookAtY += 2.5 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    {
        lookAtY -= 2.5 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
    {
        lookAtZ += 2.5 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
    {
        lookAtZ -= 2.5 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
    {
        basic_camera.changeViewUpVector(glm::vec3(1.0f, 0.0f, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
    {
        basic_camera.changeViewUpVector(glm::vec3(0.0f, 1.0f, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
    {
        basic_camera.changeViewUpVector(glm::vec3(0.0f, 0.0f, 1.0f));
    }

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
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

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void Table(Shader ourShader, glm::mat4 moveMatrix)
{

    float tableY = 0.05f;
    // Top
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix, model;

    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f + tableY, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5f, 0.1f, 1.0f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix*model);
    ourShader.setVec4("color", glm::vec4(0.5f, 1.0f, 1.0f, 1.0f));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    // Leg 1
    glm::mat4 identityMatrix1 = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix1, scaleMatrix1, model1;
    translateMatrix1 = glm::translate(identityMatrix1, glm::vec3(0.02f, -0.46f + tableY, 0.01f));
    scaleMatrix1 = glm::scale(identityMatrix1, glm::vec3(0.1f, 1.0f, 0.1f));
    model1 = translateMatrix1 * scaleMatrix1;
    ourShader.setMat4("model", moveMatrix * model1);
    ourShader.setVec4("color", glm::vec4(1.5f, 0.0f, 0.0f, 1.0f));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    // Leg 2
    glm::mat4 identityMatrix2 = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix2, scaleMatrix2, model2;
    translateMatrix2 = glm::translate(identityMatrix2, glm::vec3(0.02f, -0.46f + tableY, 0.43f));
    scaleMatrix2 = glm::scale(identityMatrix2, glm::vec3(0.1f, 1.0f, 0.1f));
    model2 = translateMatrix2 * scaleMatrix2;
    ourShader.setMat4("model", moveMatrix * model2);
    ourShader.setVec4("color", glm::vec4(1.5f, 0.0f, 0.0f, 1.0f));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    // Leg 3
    //glm::mat4 translateMatrix1, rotateXMatrix1, rotateYMatrix1, rotateZMatrix1, scaleMatrix1, model1;
    translateMatrix1 = glm::translate(identityMatrix1, glm::vec3(0.69f, -0.46f + tableY, 0.01f));
    scaleMatrix1 = glm::scale(identityMatrix1, glm::vec3(0.1f, 1.0f, 0.1f));
    model1 = translateMatrix1 * scaleMatrix1;
    ourShader.setMat4("model", moveMatrix * model1);
    ourShader.setVec4("color", glm::vec4(1.5f, 0.0f, 0.0f, 1.0f));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    // Leg 4
    //glm::mat4 translateMatrix2, rotateXMatrix2, rotateYMatrix2, rotateZMatrix2, scaleMatrix2, model2;
    translateMatrix2 = glm::translate(identityMatrix2, glm::vec3(0.69f, -0.46f + tableY, 0.43f));
    scaleMatrix2 = glm::scale(identityMatrix2, glm::vec3(0.1f, 1.0f, 0.1f));
    model2 = translateMatrix2 * scaleMatrix2;
    ourShader.setMat4("model", moveMatrix * model2);
    ourShader.setVec4("color", glm::vec4(1.5f, 0.0f, 0.0f, 1.0f));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void Chair(Shader ourShader, glm::mat4 moveMatrix)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix, model;

    float fixLeg = 0.7f;
    float fixY = 0.1f;
    float fixYWhole = -0.08f;

    // Top
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.11f, 0.0f - fixY + fixYWhole, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.75f, 0.07f, 0.7f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("color", glm::vec4(0.9f, 0.3f, 0.0f, 1.0f));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    // Leg 1
    glm::mat4 identityMatrix1 = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix1, scaleMatrix1, model1;
    translateMatrix1 = glm::translate(identityMatrix1, glm::vec3(0.13f, -0.32f + fixYWhole, 0.01f));
    scaleMatrix1 = glm::scale(identityMatrix1, glm::vec3(0.07f, 0.7f * fixLeg, 0.07f));
    model1 = translateMatrix1 * scaleMatrix1;
    ourShader.setMat4("model", moveMatrix * model1);
    ourShader.setVec4("color", glm::vec4(0.9f, 0.9f, 1.0f, 1.0f));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    // Leg 2
    glm::mat4 identityMatrix2 = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix2, scaleMatrix2, model2;
    translateMatrix2 = glm::translate(identityMatrix2, glm::vec3(0.13f, -0.32f + fixYWhole, 0.28f));
    scaleMatrix2 = glm::scale(identityMatrix2, glm::vec3(0.07f, 0.7f * fixLeg, 0.07f));
    model2 = translateMatrix2 * scaleMatrix2;
    ourShader.setMat4("model", moveMatrix * model2);
    ourShader.setVec4("color", glm::vec4(0.9f, 0.9f, 1.0f, 1.0f));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    //// Leg 3
    //glm::mat4 translateMatrix1, rotateXMatrix1, rotateYMatrix1, rotateZMatrix1, scaleMatrix1, model1;
    translateMatrix1 = glm::translate(identityMatrix1, glm::vec3(0.43f, -0.32f + fixYWhole, 0.01f));
    scaleMatrix1 = glm::scale(identityMatrix1, glm::vec3(0.07f, 0.7f * fixLeg, 0.07f));
    model1 = translateMatrix1 * scaleMatrix1;
    ourShader.setMat4("model", moveMatrix * model1);
    ourShader.setVec4("color", glm::vec4(0.9f, 0.9f, 1.0f, 1.0f));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    // Leg 4
    //glm::mat4 translateMatrix2, rotateXMatrix2, rotateYMatrix2, rotateZMatrix2, scaleMatrix2, model2;
    translateMatrix2 = glm::translate(identityMatrix2, glm::vec3(0.43f, -0.32f + fixYWhole, 0.28f));
    scaleMatrix2 = glm::scale(identityMatrix2, glm::vec3(0.07f, 0.7f * fixLeg, 0.07f));
    model2 = translateMatrix2 * scaleMatrix2;
    ourShader.setMat4("model", moveMatrix * model2);
    ourShader.setVec4("color", glm::vec4(0.9f, 0.9f, 1.0f, 1.0f));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    //Left up 
    translateMatrix2 = glm::translate(identityMatrix2, glm::vec3(0.17f, 0.01f - fixY + fixYWhole, 0.29f));
    scaleMatrix2 = glm::scale(identityMatrix2, glm::vec3(0.07f, 0.25f, 0.07f));
    model2 = translateMatrix2 * scaleMatrix2;
    ourShader.setMat4("model", moveMatrix * model2);
    ourShader.setVec4("color", glm::vec4(0.9f, 0.9f, 1.0f, 1.0f));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    //Right up
    translateMatrix2 = glm::translate(identityMatrix2, glm::vec3(0.39f, 0.01f - fixY + fixYWhole, 0.29f));
    scaleMatrix2 = glm::scale(identityMatrix2, glm::vec3(0.07f, 0.25f, 0.07f));
    model2 = translateMatrix2 * scaleMatrix2;
    ourShader.setMat4("model", moveMatrix * model2);
    ourShader.setVec4("color", glm::vec4(0.9f, 0.9f, 1.0f, 1.0f));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //Back support
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.15f, 0.09f - fixY + fixYWhole, 0.28f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.6f, 0.5f, 0.1f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("color", glm::vec4(0.9f, 0.3f, 1.0f, 1.0f));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void SideWall(Shader ourShader, glm::mat4 moveMatrix)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix, model;

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.0f, 0.0f, -4.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 2.5f * 1.6f, 14.0f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("color", glm::vec4(0.97f, 0.65f, 0.53f, 1.0f));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void FrontWall(Shader ourShader, glm::mat4 moveMatrix)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix, model;

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.0f, 0.0f, -4.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(9.3f, 2.5f * 1.6f, 0.1f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("color", glm::vec4(0.8f, 0.9f, 0.7f, 1.0f));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void Roof(Shader ourShader, glm::mat4 moveMatrix, glm::vec4 color)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix, model;

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.0f, 2.0f, -6.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(9.3f, 0.1f, 14.0f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("color", color);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void Floor (Shader ourShader, glm::mat4 moveMatrix, glm::vec4 color)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix, model;

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.0f, 1.25f + 0.45f, -6.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(9.3f, 0.1f, 14.0f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("color", color);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void BlackBoard(Shader ourShader, glm::mat4 moveMatrix)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix, model;

    float fixY = 0.4f;

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.3f, 0.2f + fixY, -3.99f));        //Frame
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(6.3f, 1.5f, 0.1f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("color", glm::vec4(0.4f, 0.16f, 0.0f, 1.0f));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.23f, 0.26f + fixY, -3.98f));        //Board
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(6.0f, 1.3f, 0.1f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("color", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void Fan1(Shader ourShader)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateToPivot, translateFromPivot, rotateYMatrix, scaleMatrix, model, moveModel;

    //Handle
    moveModel = glm::translate(identityMatrix, glm::vec3(0.0f, 1.77f, 0.0f));
    //rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.5f, 0.1f));
    model = moveModel * scaleMatrix;
    ourShader.setMat4("model",  model);
    ourShader.setVec4("color", glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    if (isRotate)
    {
        rotateAngle_Y += fanSpeed;
        rotateAngle_Y = fmod(rotateAngle_Y, 360);
    }
    


    //Center
    translateToPivot = glm::translate(identityMatrix, glm::vec3(-0.125f, -0.075f, -0.125f));
    translateFromPivot = glm::translate(identityMatrix, glm::vec3(0.125f, 0.075f, 0.125f));
    moveModel = glm::translate(identityMatrix, glm::vec3(-0.1f, 1.65f, -0.1f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5f, 0.3f, 0.5f));
    model = moveModel * translateFromPivot * rotateYMatrix * translateToPivot * scaleMatrix;
    //model = moveModel * rotateYMatrix * scaleMatrix;
    //model =  translateFromPivot * rotateYMatrix * translateToPivot;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.2f, 1.0f, 0.2f, 1.0f));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //One hand
    translateToPivot = glm::translate(identityMatrix, glm::vec3(-0.7f, -0.025f, -0.075f));
    translateFromPivot = glm::translate(identityMatrix, glm::vec3(0.7f, 0.025f, 0.075f));
    moveModel = glm::translate(identityMatrix, glm::vec3(-0.65f, 1.67f, -0.05f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.8f, 0.1f, 0.3f));
    model = moveModel * translateFromPivot * rotateYMatrix * translateToPivot * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.2f, 0.2f, 1.0f, 1.0f));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //Second hand
    translateToPivot = glm::translate(identityMatrix, glm::vec3(-0.075f, -0.025f, -0.7f));
    translateFromPivot = glm::translate(identityMatrix, glm::vec3(0.075f, 0.025f, 0.7f));
    moveModel = glm::translate(identityMatrix, glm::vec3(-0.05f, 1.67f, -0.65f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.1f, 2.8f));
    model = moveModel * translateFromPivot * rotateYMatrix * translateToPivot * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.2f, 0.2f, 1.0f, 1.0f));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

}

void Fan2(Shader ourShader)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateToPivot, translateFromPivot, rotateYMatrix, scaleMatrix, model, moveModel;

    //Handle
    moveModel = glm::translate(identityMatrix, glm::vec3(0.0f, 1.77f, 0.0f));
    //rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.5f, 0.1f));
    model = moveModel * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    if (isRotate)
    {
        rotateAngle_Y += 2.7f;
        rotateAngle_Y = fmod(rotateAngle_Y, 360);
    }


    translateToPivot = glm::translate(identityMatrix, glm::vec3(-0.25f, -0.25f, -0.25f));
    translateFromPivot = glm::translate(identityMatrix, glm::vec3(0.25f, 0.25f, 0.25f));

    //Another way

    float fixX = 0.0;
    float fixZ = 0.0f;

    //Center
    moveModel = glm::translate(identityMatrix, glm::vec3(-0.1f, 1.65f, -0.1f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5f, 0.3f, 0.5f));
    model = moveModel * scaleMatrix * translateFromPivot * rotateYMatrix * translateToPivot;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.2f, 1.0f, 0.2f, 1.0f));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //hand 1
    moveModel = glm::translate(identityMatrix, glm::vec3(0.0f + fixX, 1.67f, -0.05f + fixZ));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(-1.5f, 0.1f, 0.3f));
    model = moveModel * rotateYMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.2f, 0.2f, 1.0f, 1.0f));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //hand 2
    moveModel = glm::translate(identityMatrix, glm::vec3(0.0f + fixX, 1.67f, -0.05f + fixZ));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5f, 0.1f, 0.3f));
    model = moveModel * rotateYMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.2f, 0.2f, 1.0f, 1.0f));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //hand 3
    moveModel = glm::translate(identityMatrix, glm::vec3(-0.05f + fixX, 1.67f, 0.0f + fixZ));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.1f, -1.5f));
    model = moveModel * rotateYMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.2f, 0.2f, 1.0f, 1.0f));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //hand 4
    moveModel = glm::translate(identityMatrix, glm::vec3(-0.05f + fixX, 1.67f, 0.0f + fixZ));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.1f, 1.5f));
    model = moveModel * rotateYMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("color", glm::vec4(0.2f, 0.2f, 1.0f, 1.0f));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}