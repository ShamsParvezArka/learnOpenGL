#include <stdio.h>
#include <stdbool.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#define UTIL_IMPLEMENTATION
#include "../include/util.h"

#define LGEBRA_IMPLEMENTATION
#include "../include/lgebra.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "Learning OpenGL"

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
    
#if 0
    float vertices[] =
    { //     COORDINATES     /        COLORS      /   TexCoord  //
        -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
        -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
         0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
         0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
         0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
    };

    // Indices for vertices order
    GLuint indices[] =
    {
        0, 1, 2,
        0, 2, 3,
        0, 1, 4,
        1, 2, 4,
        2, 3, 4,
        3, 0, 4
    };
#endif

#if 1
    float vertices[] = 
    {
    -0.3f, -0.3f, -0.3f,  0.0f, 0.0f,
     0.3f, -0.3f, -0.3f,  1.0f, 0.0f,
     0.3f,  0.3f, -0.3f,  1.0f, 1.0f,
     0.3f,  0.3f, -0.3f,  1.0f, 1.0f,
    -0.3f,  0.3f, -0.3f,  0.0f, 1.0f,
    -0.3f, -0.3f, -0.3f,  0.0f, 0.0f,

    -0.3f, -0.3f,  0.3f,  0.0f, 0.0f,
     0.3f, -0.3f,  0.3f,  1.0f, 0.0f,
     0.3f,  0.3f,  0.3f,  1.0f, 1.0f,
     0.3f,  0.3f,  0.3f,  1.0f, 1.0f,
    -0.3f,  0.3f,  0.3f,  0.0f, 1.0f,
    -0.3f, -0.3f,  0.3f,  0.0f, 0.0f,

    -0.3f,  0.3f,  0.3f,  1.0f, 0.0f,
    -0.3f,  0.3f, -0.3f,  1.0f, 1.0f,
    -0.3f, -0.3f, -0.3f,  0.0f, 1.0f,
    -0.3f, -0.3f, -0.3f,  0.0f, 1.0f,
    -0.3f, -0.3f,  0.3f,  0.0f, 0.0f,
    -0.3f,  0.3f,  0.3f,  1.0f, 0.0f,

     0.3f,  0.3f,  0.3f,  1.0f, 0.0f,
     0.3f,  0.3f, -0.3f,  1.0f, 1.0f,
     0.3f, -0.3f, -0.3f,  0.0f, 1.0f,
     0.3f, -0.3f, -0.3f,  0.0f, 1.0f,
     0.3f, -0.3f,  0.3f,  0.0f, 0.0f,
     0.3f,  0.3f,  0.3f,  1.0f, 0.0f,

    -0.3f, -0.3f, -0.3f,  0.0f, 1.0f,
     0.3f, -0.3f, -0.3f,  1.0f, 1.0f,
     0.3f, -0.3f,  0.3f,  1.0f, 0.0f,
     0.3f, -0.3f,  0.3f,  1.0f, 0.0f,
    -0.3f, -0.3f,  0.3f,  0.0f, 0.0f,
    -0.3f, -0.3f, -0.3f,  0.0f, 1.0f,

    -0.3f,  0.3f, -0.3f,  0.0f, 1.0f,
     0.3f,  0.3f, -0.3f,  1.0f, 1.0f,
     0.3f,  0.3f,  0.3f,  1.0f, 0.0f,
     0.3f,  0.3f,  0.3f,  1.0f, 0.0f,
    -0.3f,  0.3f,  0.3f,  0.0f, 0.0f,
    -0.3f,  0.3f, -0.3f,  0.0f, 1.0f
};
#endif

#if 0
    float vertices[] =
    {
        // positions       // colors         //texture coords
        0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
       -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
       -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
    };
#endif

#if 1
    unsigned int indices[] = 
    { 
        0, 1, 2,
        0, 2, 3
    };
#endif

    unsigned int shader_program = create_shader_program("src/main_vert.glsl", "src/main_frag.glsl");
    
    unsigned int VBO = create_vbo(sizeof(vertices), vertices);
    unsigned int VAO = create_vao();
    //unsigned int EBO = create_ebo(sizeof(indices), indices);

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    // color
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    //glEnableVertexAttribArray(1);
    // texture
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    unsigned int texture = load_texture("container.jpg", 1);

    glfwSetKeyCallback(window, key_callback);

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) 
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, texture);

        use_shader_program(shader_program);

        //mat4_perspective(&projection, 90, (float) WINDOW_WIDTH / (float) WINDOW_HEIGHT, 0.1f, 100.0f);
        mat4_t projection = mat4(IDENTITY);
        mat4_t view = mat4(IDENTITY);
        mat4_t model = mat4(IDENTITY);

        mat4_rotate(&model, glfwGetTime() * 50, (vec3_t) { 0.3f, 1.0f, 0.0f });
        mat4_scale(&model, (vec3_t) { 1.0f, 1.0f, 1.0f });
        mat4_translate(&view, (vec3_t) { 0.0f, 0.0f, 0.3f }); 

        unsigned int texture_loc = glGetUniformLocation(shader_program, "_our_texture");
        use_shader_program(shader_program);
        glUniform1i(texture_loc, 0);

        unsigned int model_loc = glGetUniformLocation(shader_program, "_model");
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, model.m);

        unsigned int projection_loc = glGetUniformLocation(shader_program, "_projection");
        glUniformMatrix4fv(projection_loc, 1, GL_FALSE, projection.m);

        unsigned int view_loc = glGetUniformLocation(shader_program, "_view");
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, view.m);

        glBindVertexArray(VAO);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    
    glfwDestroyWindow(window);
    glfwTerminate();

    return(EXIT_SUCCESS);
}