#include <stdio.h>
#include <stdbool.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#define GLUTIL_IMPLEMENTATION
#include "glutil.h"

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
    
    
    unsigned int shaderProgram = createShaderProgram("src/vshader.glsl", "src/fshader.glsl");
    
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

    glfwSetKeyCallback(window, keyCallback);

    while (!glfwWindowShouldClose(window)) 
    {
        glClearColor(204.0f / 255.0f, 202.0f / 255.0f, 202.0f / 255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        useShaderProgram(shaderProgram);
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