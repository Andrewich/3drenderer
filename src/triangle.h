#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "vector.h"

typedef struct {
    int a;
    int b;
    int c;
} face_t;

typedef struct {
    vec2_t points[3];
} triangle_t;

#endif  // TRIANGLE_H_
