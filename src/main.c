#include <stdio.h>
#include <stdbool.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
        
#define UTIL_IMPLEMENTATION
#include "util.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "Learning OpenGL"

typedef struct
{
    float container[16];
} mat4_t;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    
    if (window == NULL) 
    {
        printf("error: cannot create GLFW window\n");
        return(EXIT_FAILURE);
    }
    
    glfwMakeContextCurrent(window);

    glad_init();
    glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);

    float vertices[] = 
    {
        // positions       // colors         //texture coords
        0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
       -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
       -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
    };

    unsigned int indices[] = 
    { 
        0, 1, 2,
        0, 2, 3
    };

    unsigned int shaderProgram = create_shader_program("src/main_vert.glsl", "src/main_frag.glsl");
    
    //Vertex buffer object, Vertex array object, Element buffer object
    unsigned int VBO = create_vbo(sizeof(vertices), vertices);
    unsigned int VAO = create_vao();

    unsigned int EBO = create_ebo(sizeof(indices), indices);

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
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else
    {
        printf("error: failed to load texture\n");
        return(EXIT_FAILURE);
    }

    stbi_image_free(imageData);

    mat4_t trans =
    {
        0.0f, -1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    unsigned int textureUni = glGetUniformLocation(shaderProgram, "_our_texture");
    use_shader_program(shaderProgram);
    glUniform1i(textureUni, 0);

    unsigned int transformLoc = glGetUniformLocation(shaderProgram, "_transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, trans.container);

    glfwSetKeyCallback(window, key_callback);

    while (!glfwWindowShouldClose(window)) 
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        use_shader_program(shaderProgram);

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