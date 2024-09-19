#pragma once

#include <math.h>

#ifndef LGEBRA
    #define LGEBRA static inline
#endif

#define PI 3.1415926535897932384626
#define DEG_TO_RAD(theta) (float) (((theta) * PI) / 180)

typedef struct
{
    float x;
    float y;
    float z;
} vec3_t;

typedef struct
{
    float m[9];
} mat3_t;

typedef struct
{
    float m[16];
} mat4_t;

typedef enum
{
    EMPTY,
    IDENTITY,
} mat_type_t;

typedef enum
{
    X_PLANE,
    Y_PLANE,
    Z_PLANE
} plane_t;

LGEBRA mat4_t mat4(mat_type_t type);
LGEBRA mat3_t mat3(mat_type_t type);
LGEBRA void mat4_rotate(mat4_t *mat_a, float deg, plane_t p);
LGEBRA void mat4_scale(mat4_t *mat_a, vec3_t v);
LGEBRA void mat4_ortho(mat4_t *mat_a, float left, float right, float bottom, float top, float near, float far);
LGEBRA void mat4_perspective(mat4_t *mat_a, float fov, float aspect, float near, float far);
LGEBRA void mat4_translate(mat4_t *mat_a, vec3_t t);

#ifdef LGEBRA_IMPLEMENTATION

LGEBRA mat3_t mat3(mat_type_t type)
{
    switch (type)
    {
    case EMPTY:
        return (mat3_t)
        {
            0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f
        };

    case IDENTITY:
        return (mat3_t)
        {
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 
            0.0f, 0.0f, 1.0f

        };

    default:
        return (mat3_t)
        {
            0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f
        };
    }
}

LGEBRA mat4_t mat4(mat_type_t type)
{
    switch (type)
    {
    case EMPTY:
        return (mat4_t)
        {
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f
        };

    case IDENTITY:
        return (mat4_t)
        {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };

    default:
        return (mat4_t)
        {
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f
        };
    }
}

LGEBRA void mat4_rotate(mat4_t *mat_a, float deg, plane_t p)
{
    switch (p)
    {
    case X_PLANE:
        mat_a->m[5] = cosf(DEG_TO_RAD(deg));
        mat_a->m[6] = -sinf(DEG_TO_RAD(deg));
        mat_a->m[9] = sinf(DEG_TO_RAD(deg));
        mat_a->m[10] = cosf(DEG_TO_RAD(deg));
        break;

    case Y_PLANE:
        mat_a->m[0] = cosf(DEG_TO_RAD(deg));
        mat_a->m[2] = sinf(DEG_TO_RAD(deg));
        mat_a->m[8] = -sinf(DEG_TO_RAD(deg));
        mat_a->m[10] = cosf(DEG_TO_RAD(deg));
        break;

    case Z_PLANE:
        mat_a->m[0] = cosf(DEG_TO_RAD(deg));
        mat_a->m[1] = -sinf(DEG_TO_RAD(deg));
        mat_a->m[4] = sinf(DEG_TO_RAD(deg));
        mat_a->m[5] = cosf(DEG_TO_RAD(deg));
        break;

    default:
        break;
    }

    return;
}

LGEBRA void mat4_scale(mat4_t *mat_a, vec3_t v)
{
    mat_a->m[0] *= v.x;
    mat_a->m[5] *= v.y;
    mat_a->m[10] *= v.z;

    return;
}

LGEBRA void mat4_ortho(mat4_t *mat_a, float left, float right, float bottom, float top, float near, float far)
{
    mat_a->m[0] = 2 / (right - left);
    mat_a->m[5] = 2 / (top - bottom);
    mat_a->m[10] = 2 / (far - near);

    mat_a->m[3] = -(right + left) / (right - left);
    mat_a->m[7] = -(top + bottom) / (top - bottom);
    mat_a->m[11] = -(far + near) / (far - near);

    return;
}

LGEBRA void mat4_perspective(mat4_t *mat_a, float fov, float aspect, float near, float far)
{
    mat_a->m[0] = 1 / (aspect * tanf(DEG_TO_RAD(fov * 0.5f)));
    mat_a->m[5] = 1 / tanf(DEG_TO_RAD(fov * 0.5f));
    mat_a->m[10] = -(far + near) / (far - near);
    mat_a->m[11] = -(2 * far * near) / (far - near);
    mat_a->m[14] = -1.0f;

    return;
}

LGEBRA void mat4_translate(mat4_t *mat_a, vec3_t t)
{
    mat_a->m[3] = t.x;
    mat_a->m[7] = t.y;
    mat_a->m[11] = t.z;
}

#endif

// todo: implement mat3_rotate fn