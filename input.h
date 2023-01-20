#ifndef INPUT_H
#define INPUT_H

#define MAX_OBJECTS 128

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "v3math.h"

// Header Files
typedef struct {
    int kind;

    union {
        // Sphere
        struct {
            float radius;
        };
        // Plane
        struct { 
            float normal[3];
        };
        // Camera
        struct {
            float width;
            float height;
        };
        // Light
        struct {
            float theta;
            float radialA0;
            float radialA1;
            float radialA2;
            float angularA0;
            float direction[3];
        };
    };

    float position[3];

    // for camera and light it is just color
    float diffuseColor[3];

    // unused by light and camera
    float reflectivity;
    float specularColor[3];
} Object;

typedef enum {
    NO_OBJECT,
    LIGHT,
    CAMERA,
    SPHERE,
    PLANE
} ObjectTypes;

int parseInput(FILE *fh, Object *objects);
void printObjects(Object *objects);
void printObject(Object *object);
#endif