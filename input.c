#include "input.h"

// populate object array from scene input
int parseInput(FILE *fh, Object *objects) {
    char inputStr[200];
    char objectType[30];
    char currentWord[30];
    int inputLength = 0;
    int cameraIndex = 0;
    int length = 0;
    int totalLength = 0;
    int index = 0;
    Object *current = NULL;

    // Loop while there are object lines in input
    while (fgets(inputStr, 200, fh) != NULL) {
        // length of full object properties string 
        inputLength = strlen(inputStr);
        // get object type
        sscanf(inputStr, "%s%n", objectType, &totalLength);

        current = &objects[index];
        
        // set everything to defaults
        v3_setZeros(current->diffuseColor);
        v3_setZeros(current->position);
        v3_setZeros(current->specularColor);
        current->reflectivity = 0;
        /*
        current->specularColor[0] = 1;
        current->specularColor[1] = 1;
        current->specularColor[2] = 1;
        */
        if (strcmp(objectType, "camera,") == 0) {
            current->height = 0;
            current->width = 0;
            current->kind = CAMERA;
        }
        else if (strcmp(objectType, "sphere,") == 0) {
            current->radius = 0;
            current->kind = SPHERE;
        }
        else if (strcmp(objectType, "plane,") == 0) {
            v3_setZeros(current->normal);
            current->kind = PLANE;
        }
        else if (strcmp(objectType, "light,") == 0) {
            v3_setZeros(current->direction);
            current->angularA0 = 0;
            current->radialA0 = 0;
            current->radialA1 = 0;
            current->radialA2 = 0;
            current->kind = LIGHT;
        }

        // loop while we still have string to parse 
        while(totalLength < inputLength) {
            // Get the Object property keyword
            sscanf(inputStr + totalLength, "%s%n", currentWord, &length); 

            // update how far in the string we are
            totalLength += length + 1;
 
            // ASSIGN OBJECT PROPERTIES 
            // Camera
            if (strcmp(objectType, "camera,") == 0) {
                if (strcmp(currentWord, "width:") == 0) {
                    cameraIndex = index;
                    sscanf(inputStr + totalLength, "%f,%n", &current->width, &length);
                }
                else if (strcmp(currentWord, "height:") == 0) {
                    sscanf(inputStr + totalLength, "%f,%n", &current->height, &length);
                }
            }
            // Sphere
            else if (strcmp(objectType, "sphere,") == 0) {
                if (strcmp(currentWord, "position:") == 0) {
                    sscanf(inputStr + totalLength,
                        "[%f, %f, %f],%n",
                        &current->position[0],
                        &current->position[1],
                        &current->position[2],
                        &length
                        );
                }

                else if (strcmp(currentWord, "radius:") == 0) {
                    sscanf(inputStr + totalLength, "%f,%n", &current->radius, &length);
                }

                else if (strcmp(currentWord, "reflectivity:") == 0) {
                    sscanf(inputStr + totalLength, "%f,%n", &current->reflectivity, &length);
                }

                else if (strcmp(currentWord, "diffuse_color:") == 0) {
                    sscanf(inputStr + totalLength,
                            "[%f, %f, %f],%n",
                            &current->diffuseColor[0],
                            &current->diffuseColor[1],
                            &current->diffuseColor[2],
                            &length
                            );
                }
                else if (strcmp(currentWord, "specular_color:") == 0) {  
                    sscanf(inputStr + totalLength,
                            "[%f, %f, %f],%n",
                            &current->specularColor[0],
                            &current->specularColor[1],
                            &current->specularColor[2],
                            &length
                            );
                }
            }
            // Plane
            else if (strcmp(objectType, "plane,") == 0) {
                if (strcmp(currentWord, "position:") == 0) {
                    sscanf(inputStr + totalLength,
                        "[%f, %f, %f],%n",
                        &current->position[0],
                        &current->position[1],
                        &current->position[2],
                        &length
                        );
                }

                else if (strcmp(currentWord, "reflectivity:") == 0) {
                    sscanf(inputStr + totalLength, "%f,%n", &current->reflectivity, &length);
                }

                else if (strcmp(currentWord, "normal:") == 0) {
                    sscanf(inputStr + totalLength,
                           "[%f, %f, %f],%n",
                            &current->normal[0],
                            &current->normal[1],
                            &current->normal[2],
                            &length
                           );
                }
                else if (strcmp(currentWord, "diffuse_color:") == 0) {  
                    sscanf(inputStr + totalLength,
                            "[%f, %f, %f],%n",
                            &current->diffuseColor[0],
                            &current->diffuseColor[1],
                            &current->diffuseColor[2],
                            &length
                            );
                }
                else if (strcmp(currentWord, "specular_color:") == 0) {  
                    sscanf(inputStr + totalLength,
                            "[%f, %f, %f],%n",
                            &current->specularColor[0],
                            &current->specularColor[1],
                            &current->specularColor[2],
                            &length
                            );
                }
            }

            else if (strcmp(objectType, "light,") == 0) {
                if (strcmp(currentWord, "position:") == 0) {
                    sscanf(inputStr + totalLength,
                        "[%f, %f, %f],%n",
                        &current->position[0],
                        &current->position[1],
                        &current->position[2],
                        &length
                        );
                }
                else if (strcmp(currentWord, "color:") == 0) {  
                    sscanf(inputStr + totalLength,
                            "[%f, %f, %f],%n",
                            &current->diffuseColor[0],
                            &current->diffuseColor[1],
                            &current->diffuseColor[2],
                            &length
                            );
                }
                else if (strcmp(currentWord, "direction:") == 0) { 
                    sscanf(inputStr + totalLength,
                            "[%f, %f, %f],%n",
                            &current->direction[0],
                            &current->direction[1],
                            &current->direction[2],
                            &length
                            );
                }
                else if (strcmp(currentWord, "theta:") == 0) {
                    sscanf(inputStr + totalLength, "%f,%n", &current->theta, &length);
                }
                else if (strcmp(currentWord, "radial-a0:") == 0) {
                    sscanf(inputStr + totalLength, "%f,%n", &current->radialA0, &length);
                }
                else if (strcmp(currentWord, "radial-a1:") == 0) {
                    sscanf(inputStr + totalLength, "%f,%n", &current->radialA1, &length);
                }
                else if (strcmp(currentWord, "radial-a2:") == 0) {
                    sscanf(inputStr + totalLength, "%f,%n", &current->radialA2, &length);
                }
                else if (strcmp(currentWord, "angular-a0:") == 0) {
                    sscanf(inputStr + totalLength, "%f,%n", &current->angularA0, &length);
                }
            }

            else {
                fprintf(stderr, "Unrecognized Object\nMust be of type: Camera, Sphere, Plane, Light\n");
                exit(1);
            }

            totalLength += length;
        }
        index++;
    }
  // if we are under 128 objects set the index after the last object to NO_OBJECT
  // so we can know when looping when to stop
  if (index < MAX_OBJECTS) {
    objects[index].kind = NO_OBJECT;
  }

  return cameraIndex;
}

// print all objects
// used for debugging parser
void printObjects(Object *objects) {
    Object *current = NULL;

    for(int index = 0; index < MAX_OBJECTS; index++) {
        //set pointer to current object
        current = &objects[index];
        // stop if no more objects
        if (current->kind == NO_OBJECT) {
            break;
        }
        else if (current->kind == LIGHT) {
            printf("LIGHT\n");
            printf("Postion: [%f %f %f]\n", 
                    current->position[0],
                    current->position[1],
                    current->position[2]
                    );
            printf("Color: [%f %f %f]\n", 
                    current->diffuseColor[0],
                    current->diffuseColor[1],
                    current->diffuseColor[2]
                    );
            printf("Theta: %f\n", current->theta);
            printf("Radial-a0: %f\n", current->radialA0);
            printf("Radial-a1: %f\n", current->radialA1);
            printf("Radial-a2: %f\n", current->radialA2);
        }
        else if (current->kind == CAMERA) {
            printf("CAMERA\n");
            printf("Width: %f\n", current->width);
            printf("Height: %f\n", current->height);
        }
        else if (current->kind == SPHERE) {
            printf("SPHERE\n");
            printf("Postion: [%f %f %f]\n", 
                    current->position[0],
                    current->position[1],
                    current->position[2]
                    );
            printf("Diffuse Color: [%f %f %f]\n", 
                    current->diffuseColor[0],
                    current->diffuseColor[1],
                    current->diffuseColor[2]
                    );
            printf("Specular Color: [%f %f %f]\n", 
                    current->specularColor[0],
                    current->specularColor[1],
                    current->specularColor[2]
                    );
            printf("Radius: %f\n", current->radius);
            printf("Reflectivity: %f\n", current->reflectivity);
        }
        // Must be Plane
        else {
            printf("PLANE\n");
            printf("Position: [%f %f %f]\n", 
                    current->position[0],
                    current->position[1],
                    current->position[2]
                    );
            printf("Normal: [%f %f %f]\n", 
                    current->normal[0],
                    current->normal[1],
                    current->normal[2]
                    );
            printf("Diffuse Color: [%f %f %f]\n", 
                    current->diffuseColor[0],
                    current->diffuseColor[1],
                    current->diffuseColor[2]
                    );
            printf("Reflectivity: %f\n", current->reflectivity);
        }
    }
}

void printObject(Object *object) {
    Object *current = object;

    if (current->kind == LIGHT) {
        printf("LIGHT\n");
        printf("Postion: [%f %f %f]\n", 
                current->position[0],
                current->position[1],
                current->position[2]
                );
        printf("Color: [%f %f %f]\n", 
                current->diffuseColor[0],
                current->diffuseColor[1],
                current->diffuseColor[2]
                );
        printf("Theta: %f\n", current->theta);
        printf("Radial-a0: %f\n", current->radialA0);
        printf("Radial-a1: %f\n", current->radialA1);
        printf("Radial-a2: %f\n", current->radialA2);
    }
    else if (current->kind == CAMERA) {
        printf("CAMERA\n");
        printf("Width: %f\n", current->width);
        printf("Height: %f\n", current->height);
    }
    else if (current->kind == SPHERE) {
        printf("SPHERE\n");
        printf("Postion: [%f %f %f]\n", 
                current->position[0],
                current->position[1],
                current->position[2]
                );
        printf("Diffuse Color: [%f %f %f]\n", 
                current->diffuseColor[0],
                current->diffuseColor[1],
                current->diffuseColor[2]
                );
        printf("Specular Color: [%f %f %f]\n", 
                current->specularColor[0],
                current->specularColor[1],
                current->specularColor[2]
                );
        printf("Radius: %f\n", current->radius);
        printf("Reflectivity: %f\n", current->reflectivity);
    }
    // Must be Plane
    else {
        printf("PLANE\n");
        printf("Position: [%f %f %f]\n", 
                current->position[0],
                current->position[1],
                current->position[2]
                );
        printf("Normal: [%f %f %f]\n", 
                current->normal[0],
                current->normal[1],
                current->normal[2]
                );
        printf("Diffuse Color: [%f %f %f]\n", 
                current->diffuseColor[0],
                current->diffuseColor[1],
                current->diffuseColor[2]
                );
        printf("Reflectivity: %f\n", current->reflectivity);
    }
}