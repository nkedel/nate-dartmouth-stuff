// writer.cc
// Nate Edel 
// first written 03 May 99 
//
// read IRF file into memory and output as JPG
// a lot of this code is loosely based on libjpeg sample code.
//
// inputs the tif (filename supplied on the command line)
// outputs jpeg to a file (original name)+".jpg"
// returns 0 on success, nonzero on error

#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "include/jpeglib.h"

bool iCompress(FILE *oFile);
bool iLoadScale(int iFile);
char *iLoadHdr(int iFile);

extern int errno;

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

struct jpeg_compress_struct cinfo;
struct jpeg_error_mgr jerr;

typedef struct sImageHeader ImageHeader;
typedef struct sPixel Pixel;

ImageHeader Image;
Pixel *inLine;
JSAMPROW *arLines;
JSAMPROW outLine;
int x, y, q, n;

unsigned int fSize;
unsigned char *ofMap;
struct stat stBuf;

int main(int argc, char **argv) {
  int inFile;
  FILE *outFile;
  char *ofName;
  
  n=-1;
  
  if (argc != 5) { 
    fprintf(stderr,"reader [input filename.IRF] [qual] [x] [y]\n");
    exit(0);
    }
  assert(argv[4] != NULL);
  assert(strlen(argv[4]) != 0);
  assert(argv[3] != NULL);
  assert(strlen(argv[3]) != 0);
  assert(argv[2] != NULL);
  assert(strlen(argv[2]) != 0);
  assert(argv[1] != NULL);
  assert(strlen(argv[1]) != 0);

  q = atoi(argv[2]);
  if (q<0 || q>100) {
    fprintf(stderr,"reader [input filename.IRF] [qual] [x] [y]\n");
    fprintf(stderr,"qual. must be between 0 and 100.\n");
    exit(0);
    }
  
  x = atoi(argv[3]);
  if (x<-1 || x>4096) {
    fprintf(stderr,"reader [input filename.IRF] [qual] [x] [y]\n");
    fprintf(stderr,"x must be between 10 and 4096.\n");
    exit(0);
    }

  y = atoi(argv[4]);
  if (y<-1 || y>4096) {
    fprintf(stderr,"reader [input filename.IRF] [qual] [x] [y]\n");
    fprintf(stderr,"y must be between 10 and 4096.\n");
    exit(0);
    }

  inFile = open(argv[1], O_RDONLY, 0 );
  if (inFile < -1) {
    fprintf(stderr, "Error #%d (%s) opening input file.\n", errno, strerror(errno));
    exit(127);
    }
  
  // load header
  ofName = iLoadHdr(inFile);

  // open the output file
  outFile = fopen(ofName, "wb");
  if (outFile == NULL) {
    fprintf(stderr,"Error #%d (%s) opening output file.\n", errno, strerror(errno));
    exit(124);
    }

  iLoadScale(inFile);
  iCompress(outFile);
  
  fclose(outFile);
  close(inFile);
  fprintf(stderr,"\n\n");
  }

// iLoadScale(int iFile):
//   loads an IRF image whose header has already been loaded (into the 
//   global ImageHeader Image) into a new malloc'd raster array pointed
//   to by (global) arLines; scale image by integer divisor (global) 'n'
//   iFile is a file descriptor to the input file.
bool iLoadScale(int iFile) {  
  size_t lwidthb, lwidthp;    // # bytes for jpeg raster array and for pixel array lines
  int w_x, w_y;               // working counter for x&y for n.n. scaling
  int tlr;                    // total lines read
  double f_x, f_y;             // x and y scale factors 
  Pixel *inLine;
  JSAMPROW outLine;
  
  // copy image  to memory
  // produce our scale factors
  f_x = (double) Image.x / (double) x;
  f_y = (double) Image.y / (double) y;

  fprintf(stderr,"iLoadScale called %d.%d -> %d.%d (%3f,%3f)\n",Image.x,Image.y,x,y,f_x,f_y);
    
  arLines = (JSAMPROW *) malloc((y + 1) * sizeof(JSAMPROW));
  if (arLines == NULL) {
    fprintf(stderr,"error malloc'ing raster array %d (%s)\n", errno, strerror(errno));
    exit(-3);
    }
  
  lwidthb = 3 * (x + 1) * sizeof(JSAMPLE);
  lwidthp = sizeof(Pixel) * Image.x;
  
  inLine = (Pixel *) malloc(lwidthp);
  if (inLine == NULL) {
    fprintf(stderr,"error malloc'ing input line %d (%s)\n", errno, strerror(errno));
    exit(-3);
    }

  tlr = 0;   
  for (int i = 0; i < y; i++) {
    w_y = (int)((double) i * f_y);         
    while (w_y > tlr-1) {
      if(read(iFile, inLine, lwidthp) < lwidthp) {
        // either we have an incomplete file, or we have a read error
        fprintf(stderr,"error reading image line %d: %d (%s)\n", i, errno, strerror(errno));
        exit(-3);
        }
      tlr++;
//      fprintf(stderr,"read ln %d [to %d] (output ln %d)\r",tlr,w_y,i);
      }
    // found nearest line, copy it to raster array
    arLines[i] = (JSAMPLE *) malloc(lwidthb);
    if (arLines[i] == NULL) {
      fprintf(stderr,"error malloc'ing raster line %d: %d (%s)\n", i, errno, strerror(errno));
      exit(-3);
      }
//    fprintf(stderr, "\nAllocated line %d at %d\n", i, arLines[i]);
    outLine=arLines[i];
    for (int j = 0; j < x; j++) {
      w_x = (int) ((double) j * f_x);            // find nearest neighbor
      outLine[j*3]   = (JSAMPLE) inLine[w_x].R;
      outLine[j*3+1] = (JSAMPLE) inLine[w_x].G;
      outLine[j*3+2] = (JSAMPLE) inLine[w_x].B;
      }
    }
  fprintf(stderr,"\n");  
  fprintf(stderr,"Original x:%d, y:%d\n",Image.x, Image.y);
  fprintf(stderr,"Final x:%d, y:%d\n",x, y);
  return true;
  }

// iCompress(FILE *oFile): 
//   compress the raster array using libjpeg; oFile is a C stream 
//   indicating the output file
//   Global 'q' indicates quality/degree of compression.
//   (note: snarfed almost verbatim from libjpeg docs)
bool iCompress(FILE *oFile) {  
//   Allocate and initialize a JPEG compression object
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);

//   Specify the destination for the compressed data (eg, a file)
    jpeg_stdio_dest(&cinfo, oFile);                              
    
//   Set parameters for compression, including image size & colorspace
    cinfo.image_width = x;      /* image width and height, in pixels */
    cinfo.image_height = y;
                                    // change this for greyscale
    cinfo.input_components = 3;     /* # of color components per pixel */
    cinfo.in_color_space = JCS_RGB; /* colorspace of input image */
    jpeg_set_defaults(&cinfo);     
    jpeg_set_quality(&cinfo, q, FALSE); // must be after set_defaults, this
                                        // is a user-supplied parameter
    jpeg_start_compress(&cinfo, TRUE);
    while (cinfo.next_scanline < cinfo.image_height)
      jpeg_write_scanlines(&cinfo, &arLines[cinfo.next_scanline], 1);
    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);
    return true;
    }

// iLoadHdr(int
//   loads an image header has into the global ImageHeader structure 
//   'Image'; iFile is a file descriptor to the input file.
//   returns the name of the original image which had been converted 
//   to IRF
char *iLoadHdr(int iFile) {
  char *ofName = (char *)malloc(sizeof(char) * 48);    
  if (ofName == NULL) {
    fprintf(stderr,"error malloc'ing ofName string %d (%s)\n", errno, strerror(errno));
    exit(-3);
    }
    
  char *tHdr = (char *) malloc(sizeof(ImageHeader));
  if (tHdr == NULL) {
    fprintf(stderr,"error malloc'ing image header %d (%s)\n", errno, strerror(errno));
    exit(-3);
    }

  if (read(iFile, tHdr, sizeof(ImageHeader)) != sizeof(ImageHeader) )  {
    fprintf(stderr, "error reading image header %d (%s)\n", errno, strerror(errno));
    exit(-3);
    }

  memcpy(&Image, tHdr, sizeof(ImageHeader)); 
  free(tHdr);
  if (y == -1 && x == -1 && n < 2) {
    y = Image.y;
    x = Image.x;
    }
  else if (y == -1 && x == -1 && n > 1) {
    y = Image.y/n;
    x = Image.x/n;
    }
  // if either x or y is valid, ignore n
  else if (y != -1 && x == -1) {
    double scf = (double) y / (double) Image.y; // scale factor for x
    x = (int) ((double) Image.x * scf);      
    }
  else if (y == -1 && x != -1) {
    double scf = (double) x / (double) Image.x; // scale factor for y
    y = (int) ((double) Image.y * scf);      
    }
  // else x != -1 && Y != -1 ergo x and y are cool.

  // we are not allowing upscaling, also catch errors.
  if (x < 10 || x > Image.x) { fprintf(stderr,"x not in 10<x<Image.x: %d, %d\n", x, Image.x); x = Image.x; }
  if (y < 10 || y > Image.y) { fprintf(stderr,"y not in 10<y<Image.y: %d, %d\n", y, Image.y); y = Image.y; }
  
  sprintf(ofName,"%s.%d.%d.%d.jpg",Image.name,x,y,q);
  return ofName;
  }

