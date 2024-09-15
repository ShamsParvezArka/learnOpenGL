#include <stdio.h>
#include <stdbool.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
        
#define CHFN_IMPLEMENTATION
#include "glutil.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

    chfnGladInit();
    glfwSetFramebufferSizeCallback(window, chfnFramebufferSizeCallback);

    float vertices[] = 
    {
        // positions       // colors         //texture coords
        0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
       -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
       -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f // top left
    };

    unsigned int indices[] = 
    { 
        0, 1, 2,
        0, 2, 3
    };

    unsigned int shaderProgram = chfnCreateShaderProgram("src/vert.glsl", "src/frag.glsl");
    
    //Vertex buffer object, Vertex array object, Element buffer object
    unsigned int VBO = chfnCreateVBO(sizeof(vertices), vertices);
    unsigned int VAO = chfnCreateVAO();

    unsigned int EBO = chfnCreateEBO(sizeof(indices), indices);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    unsigned int texture = 0;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int imageWidth, imageHeight, nrChannel;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *imageData = stbi_load("honeypie.png", &imageWidth, &imageHeight, &nrChannel, 0);

    if (imageData)
    {
        //glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else
    {
        printf("error: failed to load texture\n");
        return(EXIT_FAILURE);
    }

    stbi_image_free(imageData);

    unsigned int textureUni = glGetUniformLocation(shaderProgram, "ourTexture");
    chfnUseShaderProgram(shaderProgram);
    glUniform1i(textureUni, 0);

    glfwSetKeyCallback(window, chfnKeyCallback);

    while (!glfwWindowShouldClose(window)) 
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        chfnUseShaderProgram(shaderProgram);

        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    
    glfwDestroyWindow(window);
    glfwTerminate();

    return(EXIT_SUCCESS);
}