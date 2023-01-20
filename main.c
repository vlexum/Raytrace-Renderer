#include "raycaster.h"

// Print information
void help() {
   printf("Example Usage: ./raycast width height input.json output.ppm \n");
   exit(1);
}

// Check the command line args for errors
void checkCMDError( int argc, char **argv) {
   // ensure we have only 5 args
   if ( argc != 5) {
      fprintf(stderr, "Error: received %d arguments and expected 5\n", argc);
      help();
   }

   // if valid filenames make sure the input file exists
   else { 
      FILE *fh = fopen(argv[3], "r");

      if (fh == NULL) {
         fprintf(stderr, "Error: %s not found", argv[3]);
         exit(1);
      }
   
      fclose(fh);
   }
}

int main(int argc, char **argv) {
   // Check Args 
   checkCMDError(argc, argv);

   FILE *in = fopen(argv[3], "r");
   FILE *out = fopen(argv[4], "wb");

   int imageWidth = atoi(argv[1]);
   int imageHeight = atoi(argv[2]);
   int pixIndex = 0;
   
   float pVector[3];
   float rdVector[3];
   float color[3];
   float rZero[3];

   v3_setZeros(pVector);
   v3_setZeros(rdVector);
   v3_setZeros(color);
   v3_setZeros(rZero);

   Object objects[128];
   int cameraIndex = parseInput(in, objects);

   Object camera = objects[cameraIndex];
   //v3_copy(rZero, camera.position);

   float pixWidth = camera.width / imageWidth;
   float pixHeight = camera.height / imageHeight;

   Pixel *pixels = (Pixel *)malloc(sizeof(Pixel) * imageHeight * imageWidth);

   int recursionLimit = 7;

   for (int y = 0; y < imageHeight; y++) {
      // ASSUMING CAMERA POSTION IS ALWAYS (0, 0, 0)
      pVector[1] = camera.height / 2.0 - pixHeight * (y + 0.5);

      for (int x = 0; x < imageWidth; x++) {
         // find P
         pVector[0] = -camera.width / 2.0 + pixWidth * (x + 0.5);
         pVector[2] = -1;

         // find R(d)
         //rd = normalize(p-r0)
         v3_normalize(rdVector, pVector);
         
         // check if object in pixel and illuminate properly
         raycast(objects, rZero, rdVector, color, recursionLimit, NULL);

         // set pixel color
         pixels[pixIndex].red = (color[0] * 255);
         pixels[pixIndex].green = (color[1] * 255);
         pixels[pixIndex].blue = (color[2] * 255);

         
         pixIndex++;
      }
   } 

   // output 
   Header header;
   header.height = imageHeight;
   header.width = imageWidth;
   header.maxVal = 255;
   header.magicNumber = 6;

   Image image;
   image.header = &header;
   image.body = pixels;

   writeP6(out, &image);

   free(pixels);

   fclose(in);
   fclose(out);
}
