#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define INFO_LOG_BUFFER_SIZE 1024

typedef enum
{
    VERTEX_SHADER,
    FRAGMENT_SHADER,
} shader_type_t;

static int compilation_status = 0;
static char info_log[INFO_LOG_BUFFER_SIZE];
static bool wireframe_mode = false;

static long        get_stream_char_count(FILE *fp);
static const char *parse_shader(const char *shader_path);
static void        check_compilation_error(const char *shader, shader_type_t type);
static void        check_shader_program_compilation_error(unsigned int sp);

int          glad_init(void);
void         frame_buffer_size_callback(GLFWwindow *window, int xscale, int yscale);
void         toggle_wireframe_mode(void);
void         key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
unsigned int create_vbo(unsigned int vertex_data_size, float *vertex_data);
unsigned int create_vao(void);
unsigned int create_ebo(unsigned int index_data_size, unsigned int *index_data);
unsigned int create_shader_program(const char *vshader_src_path, const char *fshader_src_path);
void         use_shader_program(unsigned int sp);

#ifdef UTIL_IMPLEMENTATION

static long get_stream_char_count(FILE *fp)
{
    fseek(fp, 0, SEEK_END);
    long count = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    return count;
}

static const char *parse_shader(const char *shader_path)
{
    FILE *fp = fopen(shader_path, "rb");
    if (fp == NULL)
    {
        printf("error: cannot find shader source code\n");
        return(EXIT_FAILURE);
    }

    long size = get_stream_char_count(fp);
    char *shader_src = (char *) malloc(size + 1);

    fread(shader_src, size, 1, fp);
    shader_src[size] = '\0';

    fclose(fp);

    return shader_src;
}

static void check_compilation_error(const char *shader, shader_type_t type)
{
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compilation_status);
    if (compilation_status == false)
    {
        glGetShaderInfoLog(shader, 512, NULL, info_log);

        if (type == VERTEX_SHADER)
            printf("vertex_shader::error: compilation failed\n%s\n", info_log);
        if (type == FRAGMENT_SHADER)
            printf("fragment_shader::error: compilation failed\n%s\n", info_log);

        return(EXIT_FAILURE);
    }
}

static void check_shader_program_compilation_error(unsigned int sp)
{
    glGetShaderiv(sp, GL_COMPILE_STATUS, &compilation_status);

    if (!compilation_status)
    {
        glGetProgramInfoLog(sp, 512, NULL, info_log);
        return(EXIT_FAILURE);
    }

    return;
}

int glad_init(void)
{
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        printf("error: failed to initialize GLAD\n");
        return(EXIT_FAILURE);
    }

    return(EXIT_SUCCESS);
}

void frame_buffer_size_callback(GLFWwindow *window, int xscale, int yscale)
{
    glViewport(0, 0, xscale, yscale);

    return;
}

void toggle_wireframe_mode(void)
{
    if (wireframe_mode)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        wireframe_mode = false;
    } else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        wireframe_mode = true;
    }

    return;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key == GLFW_KEY_X && action == GLFW_PRESS)
        toggle_wireframe_mode();
}

unsigned int create_vbo(unsigned int vertex_data_size, float *vertex_data)
{
    unsigned int vertex_buffer_object = 0;

    glGenBuffers(1, &vertex_buffer_object);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
    glBufferData(GL_ARRAY_BUFFER, vertex_data_size, vertex_data, GL_STATIC_DRAW);

    return vertex_buffer_object;
}

unsigned int create_vao(void)
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

unsigned int create_ebo(unsigned int index_data_size, unsigned int *index_data)
{
    unsigned int elementBufferObject = 0;

    glGenBuffers(1, &elementBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_data_size, index_data, GL_STATIC_DRAW);

    return elementBufferObject;
}

unsigned int create_shader_program(const char *vshader_src_path, const char *fshader_src_path)
{ 
    const char *vshader_src = parse_shader(vshader_src_path);
    const char *fshader_src = parse_shader(fshader_src_path);

    // Vertex Shader
    unsigned int vshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vshader, 1, &vshader_src, NULL);
    glCompileShader(vshader);

    check_compilation_error(vshader, VERTEX_SHADER);

    // Fragment Shader
    unsigned int fshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fshader, 1, &fshader_src, NULL);
    glCompileShader(fshader);

    check_compilation_error(fshader, FRAGMENT_SHADER);

    // Shader Program
    unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, vshader);
    glAttachShader(shader_program, fshader);
    glLinkProgram(shader_program);

    check_shader_program_compilation_error(shader_program);

    glDeleteShader(vshader);
    glDeleteShader(fshader);

    return shader_program;
}

void use_shader_program(unsigned int sp)
{
    glUseProgram(sp);

    return;
}

#endif