// reader.cc
// Nate Edel 
// first written 02 May 99 
//
// read tiff file into memory and output as IRF (interleaved-raw format)
// a lot of this code is loosely based on libtiff sample code.
//
// inputs the tif (filename supplied on the command line)
// outputs IRF to a file (tifname)+".IRF"
// returns 0 on success, nonzero on error


#include <tiffio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

extern "C" int errno;

struct sImageHeader { 
  char     name[48];
  unsigned short x, y;
  unsigned short yDegInt;   // degrees of vertical interleaving
  unsigned short xDegInt;   // degrees of horizontal interleaving
  bool     isColor;         // should always be true; false would indicate
  };            //   a grayscale image, if we support those... 

struct sPixel {
  unsigned char R;
  unsigned char G;
  unsigned char B;
  };

typedef struct sImageHeader ImageHeader;
typedef struct sPixel Pixel;

ImageHeader *outImage;
uint32 *raster, n_pixels, height, width;
Pixel *line;
size_t ofSize;
unsigned char *ofMap;

int main(int argc, char **argv) {
  TIFF *inFile;
  int   outFile;
  char *tmem;
  int  lval;
  
  if (argc != 2) { 
    fprintf(stderr,"reader [input filename.tif]\n");
    exit(0);
    }
  assert(argv[1] != NULL);
  assert(strlen(argv[1]) != 0);

// this block snarfed and rewritten from libtiff dox
  inFile = TIFFOpen(argv[1], "r");
  if (inFile == NULL) {
    fprintf(stderr,"TIFFOpen failed. Sorry.\n");
    exit(127);
    }
  TIFFGetField(inFile, TIFFTAG_IMAGEWIDTH,  &width);
  TIFFGetField(inFile, TIFFTAG_IMAGELENGTH, &height);
  n_pixels = width * height;
  printf("Source image size: %d x %d\n",width,height);
  if (n_pixels == 0 || width < 128 || width > 2048 || height < 128 || height > 2048) {
    fprintf(stderr,"TIFF dimensions %d x %d are illegal...\nMust be between 128x128 and 2048x2048", width, height);
    exit(126);
    }
  raster = (uint32 *) _TIFFmalloc(n_pixels * sizeof(unsigned int)); 
  if (raster == NULL) {
    fprintf(stderr,"Unable to allocate memory for TIFF input image.");
    exit(125);
    }

  // copy tiff data to memory
  if (TIFFReadRGBAImage(inFile, width, height, raster, 0) == 0) {
    fprintf(stderr, "Error reading TIFF image data into memory.");
    exit(124);
    }
  
  // assemble the output filename
  tmem = (char *) malloc(sizeof(char) * (strlen(argv[1]) + 5));
  assert(tmem != NULL);
  strcpy(tmem, argv[1]);
  strcat(tmem, ".IRF");
  
  // open the output file
  outFile = open(tmem, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU );
  if (outFile<1) {
    printf("Error #%d (%s) opening output file.\n", errno, strerror(errno));
    exit(123);
    }
  free(tmem);
  
  outImage = (ImageHeader *) malloc(sizeof(ImageHeader));
// set our header info
  strcpy(outImage->name,argv[1]);
  outImage->x = (unsigned short) width;
  outImage->y = (unsigned short) height;
  outImage->yDegInt = 0;    // degrees of vertical interleaving NOT USED YET; 0 = no interleaving
  outImage->xDegInt = 0;    // ditto, but horizontal
  outImage->isColor = true; // no greyscale supprt yet, must be true
  lval = write(outFile, (void *)outImage, sizeof(ImageHeader));
  if (lval<0) {
    printf("Error #%d (%s) writing image header.\n", errno, strerror(errno));
    exit(123);
    }
  
  printf("Written header.\n");
  line = (Pixel *) malloc(sizeof(Pixel) * width);
  assert(line != NULL);  

  printf("Copying lines:\n");
  for (int i = height; i-- > 0 ; ) {
    register uint32 tPel;
    for (register int j = 0; j < width; j++) {
      tPel = raster[(i*width)+j];
      line[j].R = TIFFGetR(tPel);
      line[j].G = TIFFGetG(tPel);
      line[j].B = TIFFGetB(tPel);
      }
    printf("Writing line #%d\r",1000-i);
    lval = write(outFile, (void *)line, sizeof(Pixel[width]));
    if (lval<0) {
      printf("Error #%d (%s) writing line #%d.\n", errno, strerror(errno),i);
      exit(123);
      }
    }

// if we don't do interleaving up above, add another pass to do 
// interleaving here

  close(outFile);
     
  _TIFFfree(raster); 
  TIFFClose(inFile);
  printf("\n\n");
  }
