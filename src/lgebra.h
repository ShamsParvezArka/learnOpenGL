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
LGEBRA void mat4_rotate(mat4_t *mat_a, float deg, plane_t p);
LGEBRA void mat4_scale(mat4_t *mat_a, vec3_t v);

#ifdef LGEBRA_IMPLEMENTATION

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
}

LGEBRA void mat4_scale(mat4_t *mat_a, vec3_t v)
{
    mat_a->m[0] *= v.x;
    mat_a->m[5] *= v.y;
    mat_a->m[10] *= v.z;
}

#endif