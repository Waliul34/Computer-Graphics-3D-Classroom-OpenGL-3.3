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
#include "directionalLight.h"
#include "pointLight.h"
#include "spotLight.h"

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
void drawCube(Shader ourShader, glm::mat4 moveMatrix);

// settings
const unsigned int SCR_WIDTH = 1500;
const unsigned int SCR_HEIGHT = 800;

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

float eyeX = 0.0, eyeY = 1.0, eyeZ = 2.5;
float lookAtX = 0.0, lookAtY = 1.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);

// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;


// Light initialization
glm::vec3 lightPositions[] = {
        glm::vec3(2.25f, 1.6f, -1.7f),
        glm::vec3(-1.95f, 1.6f, -1.7f)
};

glm::vec3 lightDirections[] = {
    glm::vec3(0.4f, -0.2f, -1.0f)
};


DirectionalLight directionalLight(-lightPositions[1], glm::vec4(0.2f, 0.2f, 0.2f, 1.0f), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1);
PointLight pointLight(lightPositions[0], glm::vec4(0.2f, 0.2f, 0.2f, 1.0f), glm::vec4(0.9f, 0.9f, 0.9f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, 1);
SpotLight spotLight(lightPositions[1], lightDirections[0], 8.5f, 15.5f, glm::vec4(0.2f, 0.2f, 0.2f, 1.0f), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, 1);


//rotate
bool isRotating = false;
float fanSpeed = 8.0f;

bool lightingOn = false;
float ambientOn = 1.0;
float diffuseOn = 1.0; 
float specularOn = 1.0;
bool dark = false;

float directionalLightOn = 0.0;
float pointLightOn = 1.0;
float spotLightOn = 0.0;

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
    Shader lightCubeShader("lightVertexShader.vs", "lightFragmentShader.fs");

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
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, 0.0f, 0.0f, 0.0f, -1.0f,
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, -1.0f,

        0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 0.5f, -1.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        0.5f, 0.0f, 0.5f, 0.0f, -1.0f, 0.0f,
        0.0f, 0.0f, 0.5f, 0.0f, -1.0f, 0.0f
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
    unsigned int cubeVAO, cubeVBO, cubeEBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // position attribute
   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);

    //light's VAO
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);



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

        ////testing rotating around lookat point
        //const float radius = 2.0f;
        //float camX = sin(glfwGetTime()) * radius;
        //float camZ = cos(glfwGetTime()) * radius;
        //glm::mat4 view;
        //view = glm::lookAt(glm::vec3(camX, 1.0, camZ), glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

        // camera/view transformation
        //glm::mat4 view = basic_camera.createViewMatrix();
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);


        //**************lighting**************

        /*glm::vec3 lightColor;
        lightColor.x = sin(glfwGetTime() * 1.0f);
        lightColor.y = sin(glfwGetTime() * 0.35f);
        lightColor.z = sin(glfwGetTime() * 0.7f);
        glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);
        ourShader.setVec4("light.ambient", glm::vec4(ambientColor, 1.0f));
        ourShader.setVec4("light.diffuse", glm::vec4(diffuseColor, 1.0f));*/


        /*lightPositions[0].x = 1.0f + sin(glfwGetTime()) * 2.0f;
        lightPositions[0].y = sin(glfwGetTime() / 2.0f) * 1.0f;*/

        //Setting up Directional Light
        directionalLight.setUpLight(ourShader);
        if (!directionalLightOn)
            directionalLight.turnOff();
        if (!ambientOn)
            directionalLight.turnAmbientOff();
        if(!diffuseOn)
            directionalLight.turnDiffuseOff();
        if(!specularOn)
            directionalLight.turnSpecularOff();

        //Setting up Point Light
        pointLight.setUpLight(ourShader);
        if (!pointLightOn)
            pointLight.turnOff();
        if (!ambientOn)
            pointLight.turnAmbientOff();
        if (!diffuseOn)
            pointLight.turnDiffuseOff();
        if (!specularOn)
            pointLight.turnSpecularOff();

        //Setting up Spot Light
        spotLight.setUpLight(ourShader);
        if (!spotLightOn)
            spotLight.turnOff();
        if (!ambientOn)
            spotLight.turnAmbientOff();
        if (!diffuseOn)
            spotLight.turnDiffuseOff();
        if (!specularOn)
            spotLight.turnSpecularOff();

        //Setting up Camera and Others
        ourShader.setVec3("viewPos", camera.Position);
        ourShader.setBool("lightingOn", lightingOn);
        ourShader.setFloat("ambientOn", ambientOn);
        ourShader.setFloat("diffuseOn", diffuseOn);
        ourShader.setFloat("specularOn", specularOn);
        ourShader.setFloat("pointLightOn", pointLightOn);
        ourShader.setFloat("directionalLightOn", directionalLightOn);
        ourShader.setFloat("spotLightOn", spotLightOn);
        ourShader.setBool("dark", dark);

        ////For axis
        //ourShader.use();
        //glm::mat4 identity = glm::mat4(1.0f); // identity matrix
        //ourShader.setMat4("model", identity);
        //glBindVertexArray(axisVAO);
        //glDrawArrays(GL_LINES, 0, 6);


        glBindVertexArray(cubeVAO);


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
        color1 = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
        Roof(ourShader, translateMatrix, color1);       //Roof

        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, -1.7f, 2.0f));
        color1 = glm::vec4(0.38f, 0.3f, 0.3f, 1.0f);
        Floor(ourShader, translateMatrix, color1);       //Floor

        
        //BlackBoard
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
        BlackBoard(ourShader, translateMatrix);


        //Fan
        Fan1(ourShader);


        //Lights
        for (int i = 0; i < 2; i++)
        {
            lightCubeShader.use();

            /*glm::vec3 lightColor;
            lightColor.x = sin(glfwGetTime() * 1.0f);
            lightColor.y = sin(glfwGetTime() * 0.35f);
            lightColor.z = sin(glfwGetTime() * 0.7f);
            glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
            lightCubeShader.setVec4("bodyColor", glm::vec4(diffuseColor, 1.0f));*/

            glm::vec4 bodyColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

            //emissive
            glm::vec3 val = glm::vec3(0.5f);
            if (i == 0 and pointLightOn == 0.0)
                bodyColor = glm::vec4(val, 1.0f);
            if (i == 1 and spotLightOn == 0.0)
                bodyColor = glm::vec4(val, 1.0f);
            if (dark)
                bodyColor = glm::vec4(val, 1.0f);

            lightCubeShader.setVec4("bodyColor", bodyColor);
            glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            lightCubeShader.setMat4("projection", projection);
            glm::mat4 view = camera.GetViewMatrix();
            lightCubeShader.setMat4("view", view);
            glm::mat4 tempModel = glm::mat4(1.0f);
            tempModel = glm::translate(tempModel, lightPositions[i]);
            tempModel = glm::scale(tempModel, glm::vec3(0.5f));
            lightCubeShader.setMat4("model", tempModel);

            glBindVertexArray(lightCubeVAO);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        }


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
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeEBO);

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
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)                   //Pitch negative
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
    {                                                                   //Right
        /*eyeX -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);*/
        camera.RotateAroundLookAt(2.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)                   //Rotate camera around a look at point
    {                                                                   //Left
        /*eyeX -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);*/
        camera.RotateAroundLookAt(-2.0f);
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
        isRotating ^= true;
        cout << isRotating << endl;
        Sleep(100);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        eyeY += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)                   //Up
    {
        eyeY -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);

        camera.ProcessKeyboard(UP, deltaTime);

    }

    //***************Lighting***************

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)                   //Lighting On-Off
    {
        lightingOn ^= true;
        cout << lightingOn << endl;
        Sleep(100);
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)                   //Ambient On
    {
        ambientOn = 1.0;
        pointLight.turnAmbientOn();
        spotLight.turnAmbientOn();
        directionalLight.turnAmbientOn();
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)                   //Ambient Off
    {
        ambientOn = 0.0;
        pointLight.turnAmbientOff();
        spotLight.turnAmbientOff();
        directionalLight.turnAmbientOff();
    }

    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)                   //Diffuse On
    {
        diffuseOn = 1.0;
        pointLight.turnDiffuseOn();
        spotLight.turnDiffuseOn();
        directionalLight.turnDiffuseOn();

    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)                   //Diffuse Off
    {
        diffuseOn = 0.0;
        pointLight.turnDiffuseOff();
        spotLight.turnDiffuseOff();
        directionalLight.turnDiffuseOff();
    }
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)                   //Specular On
    {
        specularOn = 1.0;
        pointLight.turnSpecularOn();
        spotLight.turnSpecularOn();
        directionalLight.turnSpecularOn();
    }
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)                   //Specular Off
    {
        specularOn = 0.0;
        pointLight.turnSpecularOff();
        spotLight.turnSpecularOff();
        directionalLight.turnSpecularOff();
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)                   //Point Light On
    {
        pointLightOn = 1.0;
        pointLight.turnOn();
    }
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)                   //Point Light Off
    {
        pointLightOn = 0.0;
        pointLight.turnOff();
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)                   //Directional Light On
    {
        directionalLightOn = 1.0;
        directionalLight.turnOn();
    }
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)                   //Directional Light Off
    {
        directionalLightOn = 0.0;
        directionalLight.turnOff();
    }
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)                   //Spot Light On
    {
        spotLightOn = 1.0;
        spotLight.turnOn();
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)                   //Spot Light Off
    {
        spotLightOn = 0.0;
        spotLight.turnOff();
    }
    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)                   //Dark On-Off
    {
        dark ^= true;
        cout << dark << endl;
        Sleep(100);
    }





    //if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)               //Basic Camera rotate around lookat
    //{
    //    //lookAtX += 2.5 * deltaTime;
    //    //basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    //    float change = -0.3f;
    //    float x, y, z;
    //    x = eyeX * cos(glm::radians(change)) + eyeZ * sin(glm::radians(change));
    //    y = eyeY;
    //    z = - eyeX * sin(glm::radians(change)) + eyeZ * cos(glm::radians(change));

    //    eyeX = x, eyeY = y, eyeZ = z;
    //    basic_camera.changeEye(eyeX, eyeY, eyeZ);
    //}
    //if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    //{
    //    //lookAtX -= 2.5 * deltaTime;
    //    //basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);

    //    float change = 0.3f;
    //    float x, y, z;
    //    x = eyeX * cos(glm::radians(change)) + eyeZ * sin(glm::radians(change));
    //    y = eyeY;
    //    z = -eyeX * sin(glm::radians(change)) + eyeZ * cos(glm::radians(change));

    //    eyeX = x, eyeY = y, eyeZ = z;
    //    basic_camera.changeEye(eyeX, eyeY, eyeZ);
    //}
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



//OBJECTS

void drawCube(Shader ourShader, glm::mat4 moveMatrix)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, scaleMatrix, model;

    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);

    ourShader.setVec4("material.ambient", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    ourShader.setVec4("material.diffuse", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    ourShader.setVec4("material.specular", glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
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
    ourShader.setVec4("material.ambient", glm::vec4(0.5f, 1.0f, 1.0f, 1.0f));
    ourShader.setVec4("material.diffuse", glm::vec4(0.5f, 1.0f, 1.0f, 1.0f));
    ourShader.setVec4("material.specular", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    ourShader.setFloat("material.shininess", 128.0f);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    // Leg 1
    glm::mat4 identityMatrix1 = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix1, scaleMatrix1, model1;
    translateMatrix1 = glm::translate(identityMatrix1, glm::vec3(0.02f, -0.46f + tableY, 0.01f));
    scaleMatrix1 = glm::scale(identityMatrix1, glm::vec3(0.1f, 1.0f, 0.1f));
    model1 = translateMatrix1 * scaleMatrix1;
    ourShader.setMat4("model", moveMatrix * model1);
    ourShader.setVec4("material.ambient", glm::vec4(1.5f, 0.0f, 0.0f, 1.0f));
    ourShader.setVec4("material.diffuse", glm::vec4(1.5f, 0.0f, 0.0f, 1.0f));
    ourShader.setVec4("material.specular", glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    // Leg 2
    glm::mat4 identityMatrix2 = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix2, scaleMatrix2, model2;
    translateMatrix2 = glm::translate(identityMatrix2, glm::vec3(0.02f, -0.46f + tableY, 0.43f));
    scaleMatrix2 = glm::scale(identityMatrix2, glm::vec3(0.1f, 1.0f, 0.1f));
    model2 = translateMatrix2 * scaleMatrix2;
    ourShader.setMat4("model", moveMatrix * model2);
    ourShader.setVec4("material.ambient", glm::vec4(1.5f, 0.0f, 0.0f, 1.0f));
    ourShader.setVec4("material.diffuse", glm::vec4(1.5f, 0.0f, 0.0f, 1.0f));
    ourShader.setVec4("material.specular", glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    // Leg 3
    //glm::mat4 translateMatrix1, rotateXMatrix1, rotateYMatrix1, rotateZMatrix1, scaleMatrix1, model1;
    translateMatrix1 = glm::translate(identityMatrix1, glm::vec3(0.69f, -0.46f + tableY, 0.01f));
    scaleMatrix1 = glm::scale(identityMatrix1, glm::vec3(0.1f, 1.0f, 0.1f));
    model1 = translateMatrix1 * scaleMatrix1;
    ourShader.setMat4("model", moveMatrix * model1);
    ourShader.setVec4("material.ambient", glm::vec4(1.5f, 0.0f, 0.0f, 1.0f));
    ourShader.setVec4("material.diffuse", glm::vec4(1.5f, 0.0f, 0.0f, 1.0f));
    ourShader.setVec4("material.specular", glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    // Leg 4
    //glm::mat4 translateMatrix2, rotateXMatrix2, rotateYMatrix2, rotateZMatrix2, scaleMatrix2, model2;
    translateMatrix2 = glm::translate(identityMatrix2, glm::vec3(0.69f, -0.46f + tableY, 0.43f));
    scaleMatrix2 = glm::scale(identityMatrix2, glm::vec3(0.1f, 1.0f, 0.1f));
    model2 = translateMatrix2 * scaleMatrix2;
    ourShader.setMat4("model", moveMatrix * model2);
    ourShader.setVec4("material.ambient", glm::vec4(1.5f, 0.0f, 0.0f, 1.0f));
    ourShader.setVec4("material.diffuse", glm::vec4(1.5f, 0.0f, 0.0f, 1.0f));
    ourShader.setVec4("material.specular", glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
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
    ourShader.setVec4("material.ambient", glm::vec4(0.9f, 0.3f, 0.0f, 1.0f));
    ourShader.setVec4("material.diffuse", glm::vec4(0.9f, 0.3f, 0.0f, 1.0f));
    ourShader.setVec4("material.specular", glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    // Leg 1
    glm::mat4 identityMatrix1 = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix1, scaleMatrix1, model1;
    translateMatrix1 = glm::translate(identityMatrix1, glm::vec3(0.13f, -0.32f + fixYWhole, 0.01f));
    scaleMatrix1 = glm::scale(identityMatrix1, glm::vec3(0.07f, 0.7f * fixLeg, 0.07f));
    model1 = translateMatrix1 * scaleMatrix1;
    ourShader.setMat4("model", moveMatrix * model1);
    ourShader.setVec4("material.ambient", glm::vec4(0.9f, 0.9f, 1.0f, 1.0f));
    ourShader.setVec4("material.diffuse", glm::vec4(0.9f, 0.9f, 1.0f, 1.0f));
    ourShader.setVec4("material.specular", glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    // Leg 2
    glm::mat4 identityMatrix2 = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix2, scaleMatrix2, model2;
    translateMatrix2 = glm::translate(identityMatrix2, glm::vec3(0.13f, -0.32f + fixYWhole, 0.28f));
    scaleMatrix2 = glm::scale(identityMatrix2, glm::vec3(0.07f, 0.7f * fixLeg, 0.07f));
    model2 = translateMatrix2 * scaleMatrix2;
    ourShader.setMat4("model", moveMatrix * model2);
    ourShader.setVec4("material.ambient", glm::vec4(0.9f, 0.9f, 1.0f, 1.0f));
    ourShader.setVec4("material.diffuse", glm::vec4(0.9f, 0.9f, 1.0f, 1.0f));
    ourShader.setVec4("material.specular", glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    //// Leg 3
    //glm::mat4 translateMatrix1, rotateXMatrix1, rotateYMatrix1, rotateZMatrix1, scaleMatrix1, model1;
    translateMatrix1 = glm::translate(identityMatrix1, glm::vec3(0.43f, -0.32f + fixYWhole, 0.01f));
    scaleMatrix1 = glm::scale(identityMatrix1, glm::vec3(0.07f, 0.7f * fixLeg, 0.07f));
    model1 = translateMatrix1 * scaleMatrix1;
    ourShader.setMat4("model", moveMatrix * model1);
    ourShader.setVec4("material.ambient", glm::vec4(0.9f, 0.9f, 1.0f, 1.0f));
    ourShader.setVec4("material.diffuse", glm::vec4(0.9f, 0.9f, 1.0f, 1.0f));
    ourShader.setVec4("material.specular", glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    // Leg 4
    //glm::mat4 translateMatrix2, rotateXMatrix2, rotateYMatrix2, rotateZMatrix2, scaleMatrix2, model2;
    translateMatrix2 = glm::translate(identityMatrix2, glm::vec3(0.43f, -0.32f + fixYWhole, 0.28f));
    scaleMatrix2 = glm::scale(identityMatrix2, glm::vec3(0.07f, 0.7f * fixLeg, 0.07f));
    model2 = translateMatrix2 * scaleMatrix2;
    ourShader.setMat4("model", moveMatrix * model2);
    ourShader.setVec4("material.ambient", glm::vec4(0.9f, 0.9f, 1.0f, 1.0f));
    ourShader.setVec4("material.diffuse", glm::vec4(0.9f, 0.9f, 1.0f, 1.0f));
    ourShader.setVec4("material.specular", glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    //Left up 
    translateMatrix2 = glm::translate(identityMatrix2, glm::vec3(0.17f, 0.01f - fixY + fixYWhole, 0.29f));
    scaleMatrix2 = glm::scale(identityMatrix2, glm::vec3(0.07f, 0.25f, 0.07f));
    model2 = translateMatrix2 * scaleMatrix2;
    ourShader.setMat4("model", moveMatrix * model2);
    ourShader.setVec4("material.ambient", glm::vec4(0.9f, 0.9f, 1.0f, 1.0f));
    ourShader.setVec4("material.diffuse", glm::vec4(0.9f, 0.9f, 1.0f, 1.0f));
    ourShader.setVec4("material.specular", glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


    //Right up
    translateMatrix2 = glm::translate(identityMatrix2, glm::vec3(0.39f, 0.01f - fixY + fixYWhole, 0.29f));
    scaleMatrix2 = glm::scale(identityMatrix2, glm::vec3(0.07f, 0.25f, 0.07f));
    model2 = translateMatrix2 * scaleMatrix2;
    ourShader.setMat4("model", moveMatrix * model2);
    ourShader.setVec4("material.ambient", glm::vec4(0.9f, 0.9f, 1.0f, 1.0f));
    ourShader.setVec4("material.diffuse", glm::vec4(0.9f, 0.9f, 1.0f, 1.0f));
    ourShader.setVec4("material.specular", glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //Back support
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.15f, 0.09f - fixY + fixYWhole, 0.28f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.6f, 0.5f, 0.1f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", glm::vec4(0.9f, 0.3f, 1.0f, 1.0f));
    ourShader.setVec4("material.diffuse", glm::vec4(0.9f, 0.3f, 1.0f, 1.0f));
    ourShader.setVec4("material.specular", glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
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
    ourShader.setVec4("material.ambient", glm::vec4(0.97f, 0.65f, 0.53f, 1.0f));
    ourShader.setVec4("material.diffuse", glm::vec4(0.97f, 0.65f, 0.53f, 1.0f));
    ourShader.setVec4("material.specular", glm::vec4(0.1f, 0.1f, 0.1f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
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
    ourShader.setVec4("material.ambient", glm::vec4(0.8f, 0.9f, 0.7f, 1.0f));
    ourShader.setVec4("material.diffuse", glm::vec4(0.8f, 0.9f, 0.7f, 1.0f));
    ourShader.setVec4("material.specular", glm::vec4(0.1f, 0.1f, 0.1f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
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
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", glm::vec4(0.1f, 0.1f, 0.1f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
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
    ourShader.setVec4("material.ambient", color);
    ourShader.setVec4("material.diffuse", color);
    ourShader.setVec4("material.specular", glm::vec4(0.1f, 0.1f, 0.1f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);

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
    ourShader.setVec4("material.ambient", glm::vec4(0.4f, 0.16f, 0.0f, 1.0f));
    ourShader.setVec4("material.diffuse", glm::vec4(0.4f, 0.16f, 0.0f, 1.0f));
    ourShader.setVec4("material.specular", glm::vec4(0.1f, 0.1f, 0.1f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.23f, 0.26f + fixY, -3.98f));        //Board
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(6.0f, 1.3f, 0.1f));
    model = translateMatrix * scaleMatrix;
    ourShader.setMat4("model", moveMatrix * model);
    ourShader.setVec4("material.ambient", glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    ourShader.setVec4("material.diffuse", glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    ourShader.setVec4("material.specular", glm::vec4(0.6f, 0.5f, 0.5f, 0.5f));
    ourShader.setFloat("material.shininess", 128.0f);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void Fan1(Shader ourShader)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateToPivot, translateFromPivot, rotateYMatrix, scaleMatrix, model, moveModel;
    glm::vec4 translateToPivotOrg, translateFromPivotOrg;
    glm::vec3 pivot;

    //Handle
    moveModel = glm::translate(identityMatrix, glm::vec3(0.0f, 1.77f, 0.0f));
    //rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.5f, 0.1f));
    model = moveModel * scaleMatrix;
    ourShader.setMat4("model",  model);
    ourShader.setVec4("material.ambient", glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
    ourShader.setVec4("material.diffuse", glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
    ourShader.setVec4("material.specular", glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    if (isRotating)
    {
        rotateAngle_Y += fanSpeed;
        rotateAngle_Y = fmod(rotateAngle_Y, 360);
    }
    


    //Center
    pivot = glm::vec3(0.25, 0.25, 0.25);

    translateToPivotOrg = glm::vec4(-pivot, 0.0f);
    translateFromPivotOrg = glm::vec4(pivot, 0.0f);
    
    moveModel = glm::translate(identityMatrix, glm::vec3(-0.1f, 1.65f, -0.1f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5f, 0.3f, 0.5f));
    
    translateToPivotOrg = scaleMatrix * translateToPivotOrg;
    translateFromPivotOrg = scaleMatrix * translateFromPivotOrg;
    translateToPivot = glm::translate(identityMatrix, glm::vec3(translateToPivotOrg));
    translateFromPivot = glm::translate(identityMatrix, glm::vec3(translateFromPivotOrg));

    model = moveModel * translateFromPivot * rotateYMatrix * translateToPivot * scaleMatrix;
    //model = moveModel * rotateYMatrix * scaleMatrix;
    //model =  translateFromPivot * rotateYMatrix * translateToPivot;
    ourShader.setMat4("model", model);
    ourShader.setVec4("material.ambient", glm::vec4(0.2f, 1.0f, 0.2f, 1.0f));
    ourShader.setVec4("material.diffuse", glm::vec4(0.2f, 1.0f, 0.2f, 1.0f));
    ourShader.setVec4("material.specular", glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //One hand

    translateToPivotOrg = glm::vec4(-pivot, 0.0f);
    translateFromPivotOrg = glm::vec4(pivot, 0.0f);

    moveModel = glm::translate(identityMatrix, glm::vec3(-0.65f, 1.67f, -0.05f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.8f, 0.1f, 0.3f));

    translateToPivotOrg = scaleMatrix * translateToPivotOrg;
    translateFromPivotOrg = scaleMatrix * translateFromPivotOrg;
    translateToPivot = glm::translate(identityMatrix, glm::vec3(translateToPivotOrg));
    translateFromPivot = glm::translate(identityMatrix, glm::vec3(translateFromPivotOrg));

    model = moveModel * translateFromPivot * rotateYMatrix * translateToPivot * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("material.ambient", glm::vec4(0.2f, 0.2f, 1.0f, 1.0f));
    ourShader.setVec4("material.diffuse", glm::vec4(0.2f, 0.2f, 1.0f, 1.0f));
    ourShader.setVec4("material.specular", glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //Second hand

    translateToPivotOrg = glm::vec4(-pivot, 0.0f);
    translateFromPivotOrg = glm::vec4(pivot, 0.0f);

    moveModel = glm::translate(identityMatrix, glm::vec3(-0.05f, 1.67f, -0.65f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.1f, 2.8f));

    translateToPivotOrg = scaleMatrix * translateToPivotOrg;
    translateFromPivotOrg = scaleMatrix * translateFromPivotOrg;
    translateToPivot = glm::translate(identityMatrix, glm::vec3(translateToPivotOrg));
    translateFromPivot = glm::translate(identityMatrix, glm::vec3(translateFromPivotOrg));

    model = moveModel * translateFromPivot * rotateYMatrix * translateToPivot * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("material.ambient", glm::vec4(0.2f, 0.2f, 1.0f, 1.0f));
    ourShader.setVec4("material.diffuse", glm::vec4(0.2f, 0.2f, 1.0f, 1.0f));
    ourShader.setVec4("material.specular", glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
    ourShader.setFloat("material.shininess", 32.0f);
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
    ourShader.setVec4("objectColor", glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    if (isRotating)
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
    ourShader.setVec4("objectColor", glm::vec4(0.2f, 1.0f, 0.2f, 1.0f));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //hand 1
    moveModel = glm::translate(identityMatrix, glm::vec3(0.0f + fixX, 1.67f, -0.05f + fixZ));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(-1.5f, 0.1f, 0.3f));
    model = moveModel * rotateYMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("objectColor", glm::vec4(0.2f, 0.2f, 1.0f, 1.0f));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //hand 2
    moveModel = glm::translate(identityMatrix, glm::vec3(0.0f + fixX, 1.67f, -0.05f + fixZ));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5f, 0.1f, 0.3f));
    model = moveModel * rotateYMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("objectColor", glm::vec4(0.2f, 0.2f, 1.0f, 1.0f));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //hand 3
    moveModel = glm::translate(identityMatrix, glm::vec3(-0.05f + fixX, 1.67f, 0.0f + fixZ));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.1f, -1.5f));
    model = moveModel * rotateYMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("objectColor", glm::vec4(0.2f, 0.2f, 1.0f, 1.0f));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    //hand 4
    moveModel = glm::translate(identityMatrix, glm::vec3(-0.05f + fixX, 1.67f, 0.0f + fixZ));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.1f, 1.5f));
    model = moveModel * rotateYMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
    ourShader.setVec4("objectColor", glm::vec4(0.2f, 0.2f, 1.0f, 1.0f));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}