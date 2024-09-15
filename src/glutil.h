#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define INFO_LOG_BUFFER_SIZE 512

static bool isWireframeModeOn = false;

static const char *chfnParseShader(const char *shaderPath);

int          chfnGladInit(void);
void         chfnFramebufferSizeCallback(GLFWwindow *window, int xscale, int yscale);
void         chfnToggleWireframeMode(void);
void         chfnKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
unsigned int chfnCreateVBO(unsigned int vertexDataSize, float *vertexData);
unsigned int chfnCreateVAO(void);
unsigned int chfnCreateEBO(unsigned int indexDataSize, unsigned int *indexData);
unsigned int chfnCreateShaderProgram(const char *vertexShaderSourcePath, const char *fragmentShaderSourcePath);
void         chfnUseShaderProgram(unsigned int shaderProgram);

#ifdef CHFN_IMPLEMENTATION
int chfnGladInit(void)
{
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        printf("error: failed to initialize GLAD\n");
        return(EXIT_FAILURE);
    }

    return(EXIT_SUCCESS);
}

static const char *chfnParseShader(const char *shaderPath)
{
    FILE *fp = fopen(shaderPath, "rb");

    if (fp == NULL)
    {
        printf("error: cannot find shader source code\n");
        return(EXIT_FAILURE);
    }

    fseek(fp, 0, SEEK_END);
    unsigned int size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *shaderSource = (char *) malloc(size + 1);

    fread(shaderSource, size, 1, fp);
    shaderSource[size] = '\0';

    fclose(fp);

    return shaderSource;
}

void chfnFramebufferSizeCallback(GLFWwindow *window, int xscale, int yscale)
{
    glViewport(0, 0, xscale, yscale);

    return;
}

void chfnToggleWireframeMode(void)
{
    if (isWireframeModeOn)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        isWireframeModeOn = false;
    } else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        isWireframeModeOn = true;
    }

    return;
}

void chfnKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key == GLFW_KEY_X && action == GLFW_PRESS)
        chfnToggleWireframeMode();
}

unsigned int chfnCreateVBO(unsigned int vertexDataSize, float *vertexData)
{
    unsigned int vertexBufferObject = 0;

    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, vertexDataSize, vertexData, GL_STATIC_DRAW);

    return vertexBufferObject;
}

unsigned int chfnCreateVAO(void)
{
    unsigned int vertexArrayObject = 0;

    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    // color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    return vertexArrayObject;
}

unsigned int chfnCreateEBO(unsigned int indexDataSize, unsigned int *indexData)
{
    unsigned int elementBufferObject = 0;

    glGenBuffers(1, &elementBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexDataSize, indexData, GL_STATIC_DRAW);

    return elementBufferObject;
}

unsigned int chfnCreateShaderProgram(const char *vertexShaderSourcePath, const char *fragmentShaderSourcePath)
{
    int success = 0;
    char infoLog[INFO_LOG_BUFFER_SIZE];

    const char *vertexShaderSource = chfnParseShader(vertexShaderSourcePath);
    const char *fragmentShaderSource = chfnParseShader(fragmentShaderSourcePath);

    // Vertex Shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (success == false)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("vertex shader::error: compilation failed\n%s\n", infoLog);
        return(EXIT_FAILURE);
    }

    // Fragment Shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (success == false)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("fragment shader::error: compilation failed\n%s\n", infoLog);
        return(EXIT_FAILURE);
    }

    // Shader Program
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_COMPILE_STATUS, &success);
    if (success == false)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        return(EXIT_FAILURE);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void chfnUseShaderProgram(unsigned int shaderProgram)
{
    glUseProgram(shaderProgram);

    return;
}

#endif