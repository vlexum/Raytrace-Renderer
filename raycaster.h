#ifndef RAYCASTER_H
#define RAYCASTER_H

#define PI 3.1415926535897
#define NS 20

#include "ppmrw.h"
#include "v3math.h"
#include "input.h"

typedef enum {
    NO_INTERSECTION 
} Errors;

// Function Declarations
float sphereIntersect(Object *sphere, float *origin, float *direction);
float planeIntersect(Object *sphere, float *origin, float *direction);
float clamp(float value, float min, float max);
void raycast(Object *objects, float *origin, float *direction, float *color, int recursionLimit, Object *skipObj);
float findClosestIntersection(Object *objects, float *origin, float *direction, Object **closest, Object *skipObj);
void illuminate(Object *objects, Object *closest, float *direction, float *origin, float *color, float tVal, int recursionLimit);
void findDiffuse(Object *light, Object *object, float *lightVec, float *normal, float *diffuse);
void findSpecular(Object *light, Object *object, float *lightVec, float *normal, float *rVec, float *specular, float *camDir);
float radialAttenuation(Object *light, float *lightVec);
float angularAttenuation(Object *light, float *lightVec);
#endif