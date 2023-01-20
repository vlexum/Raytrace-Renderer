#include "ppmrw.h"

// Functions

// Gets Header Information
int parseHeader (FILE *fh) {
    int resultVal;
    bool parseFlag = true;
    bool intFlag = true;

    int character = fgetc(fh);
   
    //get through whitespace and comments and if EOF then we got an error
    while (parseFlag) {
      // Skip comments
      if ( character == '#') {
         do {
            character = fgetc(fh);
         } while ( character != EOF && character != '\r' && character != '\n');
      }
      // Skip white space
      else if ( isspace( character )) {
         character = fgetc(fh);
      }
      // If EOF in header there is error
      else if ( character == EOF ) {
         fprintf(stderr, "Error : formatting error in header");
         exit(1);
      }
      // Stop parsing we have a valid charachter
      else {
         parseFlag = false;
      }
    }

    resultVal = character;

    // Expecting int here 
    if ( resultVal <= '9' && resultVal >= '0') {
      resultVal -= '0';
      while (intFlag) {
         character = fgetc(fh);

         if ( isspace(character) ) {
            ungetc(character, fh);
            intFlag = false;
         }

         else {
            resultVal = (resultVal * 10) + (character - '0');
         }
      }
    }
    
    return resultVal;
}

// Builds header and calls appropriate read function for the image data 
// and return built image
Image *readPPM( FILE *fh ) {
    Header *ppmHeader;
    Image *inImaage;
    int ppmType;

    int character = parseHeader(fh);
    
    // Allocate memory
    ppmHeader = (Header *)malloc( sizeof( Header ));
    
    // Formatting check 
    if ( ( character != 'P') ) {
      fprintf(stderr,"Formatting Error: Does not start with P\n");
      exit(1);
    }
    
    // assign header vals
    ppmHeader->magicNumber = ppmType = parseHeader(fh);
    ppmHeader->width = parseHeader(fh);
    ppmHeader->height = parseHeader(fh);
    ppmHeader->maxVal = parseHeader(fh);

    // make sure maxVal is appropriate
    if (ppmHeader->maxVal < 0 || ppmHeader->maxVal > 255) {
      fprintf(stderr, "Error: MaxVal not in appropriate range\n");
      exit(1);
    }

    // read image data
    if (ppmType == 3){
       return readP3(fh, ppmHeader );
    }
    // Assume P6 since we already made sure the val is either 3 or 6
    else {
       return readP6(fh, ppmHeader );
    }
}

Image *readP3( FILE *fh, Header *headerPtr ) {
    Image *p3Image;
    Pixel *pixels;
    int numPixels = headerPtr->height * headerPtr->width;
    int index;

    p3Image = (Image *)malloc(sizeof(Image));
    
    pixels = (Pixel *)malloc(sizeof(Pixel) * numPixels);

    p3Image->header = headerPtr;
    p3Image->body = pixels;

    for (index = 0; index < numPixels; index++) {
      uint8_t val;
      // Red
      fscanf(fh, "%d", &val);
      p3Image->body[index].red = val;
      // Green
      fscanf(fh, "%d", &val);
      p3Image->body[index].green = val;
      // Blue
      fscanf(fh, "%d", &val);
      p3Image->body[index].blue = val;
      
    }
  
  return p3Image;
}

Image *readP6( FILE *fh, Header *headerPtr ) {
   Image *p6Image;
   Pixel *pixels;
   int numPixels = headerPtr->height * headerPtr->width;
   int index;

   p6Image = (Image *)malloc(sizeof(Image));
   
   pixels = (Pixel *)malloc(sizeof(Pixel) * numPixels);

   p6Image->header = headerPtr;
   p6Image->body = pixels;
   
   int testNext = fgetc(fh);
   if (! isspace(testNext)) {
      ungetc(testNext, fh);
   }

   fread(p6Image->body, sizeof(Pixel), numPixels, fh);
   
  
  return p6Image;
}

void writeP3( FILE *fh, Image *image) {
    int index;
    int numPixels = image->header->height * image->header->width;

    fprintf(fh, "P3\n%d %d\n%d\n", image->header->width,
            image->header->height, image->header->maxVal);
    
    for (index = 0; index < numPixels; index++) {
       fprintf(fh, "%d %d %d\n", image->body[index].red, image->body[index].green, 
                                                             image->body[index].blue);
    }
}

void writeP6( FILE *fh, Image *image) {
   int index;
   int numPixels = image->header->height * image->header->width;

   fprintf(fh, "P6\n%u %u\n%u\n", image->header->width,
            image->header->height, image->header->maxVal);
    
   fwrite(image->body, 3 * image->header->width, image->header->height, fh);
}

void freeMem(Image *image) {
   free(image->body);
   free(image->header);
   free(image);
}

