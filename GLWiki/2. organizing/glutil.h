#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

static bool isWireframeModeOn = false;

int gladInit(void);
static const char *parseShader(const char *shaderPath);
void framebufferSizeCallback(GLFWwindow *window, int xscale, int yscale);
void toggleWireframeMode(void);
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
unsigned int createShaderProgram(const char *vertexShaderSourcePath, const char *fragmentShaderSourcePath);
void useShaderProgram(unsigned int shaderProgram);

#ifdef GLUTIL_IMPLEMENTATION
int gladInit(void)
{
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        printf("error: failed to initialize GLAD\n");
        return(EXIT_FAILURE);
    }

    return(EXIT_SUCCESS);
}

static const char *parseShader(const char *shaderPath)
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

void framebufferSizeCallback(GLFWwindow *window, int xscale, int yscale)
{
    glViewport(0, 0, xscale, yscale);

    return;
}

void toggleWireframeMode(void)
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

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key == GLFW_KEY_X && action == GLFW_PRESS)
        toggleWireframeMode();
}

unsigned int createShaderProgram(const char *vertexShaderSourcePath, const char *fragmentShaderSourcePath)
{
    int success = 0;
    char infoLog[512];

    const char *vertexShaderSource = parseShader(vertexShaderSourcePath);
    const char *fragmentShaderSource = parseShader(fragmentShaderSourcePath);

    // Vertex Shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (success == false)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("error: shader compilation failed\n%s\n", infoLog);
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
        printf("error: shader compilation failed\n%s\n", infoLog);
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

void useShaderProgram(unsigned int shaderProgram)
{
    glUseProgram(shaderProgram);

    return;
}

#endif