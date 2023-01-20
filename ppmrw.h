#ifndef PPMRW_H
#define PPMRW_H

// Header Files
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

// Header information
typedef struct header {
    int magicNumber;
    int height;
    int width;
    int maxVal;
} Header;

// Represents an RGB pixel with integer values between 0-maxval
typedef struct pixel {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} Pixel;

// PPM Image Information
typedef struct image {
    Header *header;
    Pixel *body;
} Image;

// Function Prototypes
Image *readPPM(FILE *fh);
Image *readP3(FILE *fh, Header *headerPtr);
Image *readP6(FILE *fh, Header *headerPtr);
void writeP3(FILE *fh, Image *image);
void writeP6(FILE *fh, Image *image);
void freeMem(Image *image);
int parseHeader(FILE *fh);

#endif