#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

void framebufferSizeCallback(GLFWwindow *window, int xscale, int yscale)
{
    glViewport(0, 0, xscale, yscale);

    return;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    return;
}

const char *parseShader(const char *shaderPath)
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

static int gladInit(void)
{
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        printf("error: failed to initialize GLAD\n");
        return(EXIT_FAILURE);
    }

    return(EXIT_SUCCESS);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const int windowWidth = 800;
    const int windowHeight = 600;
    const char *windowTitle = "Learning OpenGL";

    GLFWwindow *window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, NULL, NULL);
    
    if (window == NULL) 
    {
        printf("error: cannot create GLFW window\n");
        return(EXIT_FAILURE);
    }
    
    glfwMakeContextCurrent(window);

    gladInit();
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 
        -0.25f, 0.0f, 0.0f, 0.6f, 0.0f, 0.0f,
        0.25f, 0.0f, 0.0f, 0.4f, 0.0f, 0.0f,
        0.0f, -0.5f, 0.0f, 0.0, 0.0f, 1.0f
    };

    unsigned int indices[] = 
    {
        0, 5, 3,
        5, 1, 4,
        3, 4, 2
    };
    
    const char *vertexShaderSource = parseShader("vshader.glsl");
    const char *fragmentShaderSource = parseShader("fshader.glsl");

    // Vertex Shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success = 0;
    char infoLog[512];
    
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

    //Vertex buffer object, Vertex array object, Element buffer object
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    // color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window)) 
    {
        processInput(window);

        glClearColor(204.0f / 255.0f, 202.0f / 255.0f, 202.0f / 255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    
    glfwDestroyWindow(window);
    glfwTerminate();

    return(EXIT_SUCCESS);
}
