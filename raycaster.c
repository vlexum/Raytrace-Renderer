#include "raycaster.h"

// test for ray intersection with sphere
float sphereIntersect(Object *sphere, float *origin, float *direction) {
    float oMinusP[3];

    v3_subtract(oMinusP, origin, sphere->position);
    
    float b = 2 * ((direction[0] * oMinusP[0]) + 
                   (direction[1] * oMinusP[1]) + 
                   (direction[2] * oMinusP[2])
                  );

     float c = pow(oMinusP[0], 2) +
               pow(oMinusP[1], 2) +
               pow(oMinusP[2], 2) - 
               pow(sphere->radius, 2);
        
    float discriminant = pow(b, 2) - (4 * c);

    if (discriminant > 0) {
        float tZero = (-b - sqrt(discriminant)) / 2.0;
        float tOne = (-b + sqrt(discriminant)) / 2.0;

        if (tZero > 0.0 ) {
            return tZero;
        }
        if (tOne > 0.0) {
            return tOne;
        }
    }
    return NO_INTERSECTION;
}

// test for ray intersection with plane
float planeIntersect(Object *plane, float *origin, float *direction) {
    float *position = plane->position;
    float *normal = plane->normal;

    float product = v3_dot_product(normal, direction);

    if (product != 0) {
        float dVal = -v3_dot_product(normal, position);

        float tVal = -(v3_dot_product(normal, origin) + dVal) / product;

        if ( tVal > 0) {
            return tVal;
        }
    }

  return NO_INTERSECTION;
}

// find the closest object(if any) and sets color appropriately
// black(background) if no intersection
void raycast(Object *objects, float *origin, float *direction, float *color, int recursionLimit, Object *skipObj) {
    Object *closest = NULL;
    // no intersection means background (black)
    v3_setZeros(color);

    if (recursionLimit >= 0) {
        // check if camera ray on pixel with object
        float tVal = findClosestIntersection(objects, origin, direction, &closest, skipObj);

        // if there was an object on the pixel
        if (closest != NULL) {
            // color the pixel with respect to the light in scene
            illuminate(objects, closest, direction, origin, color, tVal, recursionLimit);
        }
    }
}

// checks for intersection with a ray's origin and direction
// skipObj is the object we want to skip if called from illuminate
// skipObj will be null if called from raycast
float findClosestIntersection(Object *objects, float *origin, float *direction, Object **closest, Object *skipObj) {
    float tVal = NO_INTERSECTION;
    float tMin = INFINITY;
    Object *nearObject = NULL;
    Object *current = NULL;    

    for(int objectIndex = 0; objectIndex < MAX_OBJECTS; objectIndex++) {
        //set pointer to current object
        current = &objects[objectIndex];
        
        if (skipObj != current) {
            // stop if no more objects
            if (current->kind == NO_OBJECT) {
                break;
            }
            // check if object to be skipped
            else if (current->kind == SPHERE) {
                // intersect test
                tVal = sphereIntersect(current, origin, direction);
            }
            // Must be Plane
            else if (current->kind == PLANE) {
                // intersect test
                tVal = planeIntersect(current, origin, direction);
            }
            // if not plane or sphere - no intersection
            else {
                tVal = NO_INTERSECTION;
            }

            // check if closest
            if (tVal != NO_INTERSECTION && tVal < tMin) { 
                tMin = tVal;
                nearObject = current;
            }
        }
    }
    // report intersection was found
    // set the pointer to object that pixel will represent
    if (nearObject != NULL) {
        *closest = nearObject;
        return tMin;
    }
    // no intersection happened
    return NO_INTERSECTION;
}

void illuminate(Object *objects, Object *closest, float *direction, float *origin, float *color, float tVal, int recursionLimit) {
    // pointers
    Object *current = NULL;
    Object *intersectObj = NULL;
    // floats
    float angAttn = 1;
    float radAttn = 1;
    // float arrays
    float lightDir[3];
    float lightVec[3];
    float normal[3];
    float lightT;
    float rVec[3];
    float diffuseColor[3];
    float specularColor[3];
    float reflectedColor[3];
    float diffSpecSum[3];
    float intersect[3];
    float temp[3];
    float intersectVec[3];
    float reflectVec[3];
    float directionFlip[3];
    float calculatedColor[3];

    // values for reflective objects
    float reflectivity = closest->reflectivity;
    float reflection = 1 - reflectivity;

    v3_setZeros(calculatedColor);
    
    // copy direction to find intersect
    v3_copy(intersect, direction);
    
    // Scale the intersection by the tVal
    v3_scale(intersect, tVal);
    
    // origin is 0,0,0 for now
    v3_add(intersect, intersect, origin);

    // flipped cam view for specular calculation
    v3_copy(directionFlip, direction);
    v3_scale(directionFlip, -1);
    
    // set normal - depends on object type
    if (closest->kind == PLANE) {
        v3_copy(normal, closest->normal);
    }
    else if (closest->kind == SPHERE) {
        v3_subtract(temp, intersect, closest->position);
        v3_normalize(normal, temp);
    }

    // flip the ray from camera about the object normal and normalize
    v3_reflect(temp, direction, normal);
    v3_normalize(reflectVec, temp);
    
    // if an object is reflective
    if (reflectivity > 0.0) {
        // look for reflections - recursively
        raycast(objects, intersect, reflectVec, reflectedColor, recursionLimit - 1, closest);
    }

    // loop through objects and look for lights
    for(int index = 0; index < MAX_OBJECTS; index++) {
        //set pointer to current object
        current = &objects[index];
        
        // foundlight
        if (current->kind == LIGHT) {
            // get vector from intersect to light
            v3_from_points(lightDir, current->position, intersect);

            v3_from_points(temp, intersect, current->position);
            v3_normalize(intersectVec, temp); 

            // normalize for calculations
            v3_normalize(lightVec, lightDir);

            // get the reflection vector and normalize
            v3_reflect(temp, lightVec, normal);
            v3_normalize(rVec, temp);
            
            // look for an object
            // closeParam passes back closest object so the intersection test ignores it;
            lightT = findClosestIntersection(objects, intersect, intersectVec, &intersectObj, closest);
            
            // if nothing in the way 
            if (lightT == NO_INTERSECTION || lightT > v3_length(lightDir)) {
                // find radial and angular attenuation
                radAttn = radialAttenuation(current, lightDir);
                angAttn = angularAttenuation(current, lightVec);

                // find the diffuse and specular color for object with respect to the light
                findDiffuse(current, closest, lightVec, normal, diffuseColor);
                findSpecular(current, closest, lightVec, normal, rVec, specularColor, directionFlip);

                // add the diffuse and specular color 
                v3_add(diffSpecSum, diffuseColor, specularColor);
                
                // add to the object color for each light
                calculatedColor[0] += (diffSpecSum[0]);
                calculatedColor[1] += (diffSpecSum[1]);
                calculatedColor[2] += (diffSpecSum[2]);

                // Scale by angular and radial attenuation and reflection
                v3_scale(calculatedColor, (angAttn * radAttn * reflection));
            }
        }
        // stop if no more objects
        else if (current->kind == NO_OBJECT) {
            break;
        }
    }
    
    // add reflection and calculated color to object
    calculatedColor[0] += (reflectivity * reflectedColor[0]);
    calculatedColor[1] += (reflectivity * reflectedColor[1]);
    calculatedColor[2] += (reflectivity * reflectedColor[2]);

    // ensure values are between 0 and 1
    color[0] = clamp(calculatedColor[0], 0.0, 1.0);
    color[1] = clamp(calculatedColor[1], 0.0, 1.0);
    color[2] = clamp(calculatedColor[2], 0.0, 1.0);
}

void findDiffuse(Object *light, Object *object, float *lightVec, float *normal, float *diffuse) {
    // find dot product
    float dot = v3_dot_product(lightVec, normal);

    // set to black 
    v3_setZeros(diffuse);

    // diffuse calculation
    if (dot > 0) {   
        diffuse[0] = object->diffuseColor[0] * light->diffuseColor[0];
        diffuse[1] = object->diffuseColor[1] * light->diffuseColor[1];
        diffuse[2] = object->diffuseColor[2] * light->diffuseColor[2];

        v3_scale(diffuse, dot);
    }
}

void findSpecular(Object *light, Object *object, float *lightVec, float *normal, float *rVec, float *specular, float *camDir) {
    // get dot products
    float rDotCam = v3_dot_product(rVec, camDir);
    float lDotNorm = v3_dot_product(lightVec, normal);

    // set to black
    v3_setZeros(specular);

    // specular calculation
    if (rDotCam > 0 && lDotNorm > 0) {
        float spec = pow(rDotCam, NS);

        specular[0] = object->specularColor[0] * light->diffuseColor[0];
        specular[1] = object->specularColor[1] * light->diffuseColor[1];
        specular[2] = object->specularColor[2] * light->diffuseColor[2];

        v3_scale(specular, spec);
    }
}

float radialAttenuation(Object *light, float *lightVec){
    float a0 = light->radialA0;
    float a1 = light->radialA1;
    float a2 = light->radialA2;

    //d = normalized light vector
    float d = v3_length(lightVec);
    // radial attenuation
    float denom = a0 + (a1 * d) + (a2 * (d * d));

    return (1.0 / denom);
}

float angularAttenuation(Object *light, float *lightVec){
    float *vL = light->direction;
    float vO[3];

    // check if spotlight
    // if light has no direction then its not spotlight
    // also if theta is 0
    if (light->theta == 0 || vL[0] == 0 && vL[1] == 0 && vL[2] == 0) {
        return 1.0;
    }

    v3_copy(vO, lightVec);
    v3_scale(vO, -1);
    
    // angular attenuation
    float dot = v3_dot_product(vO, vL);
    float angAttenuation = pow(dot, light->angularA0);

    // 0 if light is outside cone
    if (acos(dot) * 180.0 / PI > light->theta) {
        angAttenuation = 0.0;
    }

    return angAttenuation;
}

// make sure values are between two values
float clamp(float value, float min, float max) {
    if (value < min) {
        return min;
    }
    else if (value > max) {
        return max;
    }
    return value;
}
