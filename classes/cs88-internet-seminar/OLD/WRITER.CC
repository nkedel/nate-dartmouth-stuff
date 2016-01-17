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
  
  if (argc != 4) { 
    fprintf(stderr,"reader [input filename.IRF] [qual] [div]\n");
    exit(0);
    }
  assert(argv[3] != NULL);
  assert(strlen(argv[3]) != 0);
  assert(argv[2] != NULL);
  assert(strlen(argv[2]) != 0);
  assert(argv[1] != NULL);
  assert(strlen(argv[1]) != 0);

  q = atoi(argv[2]);
  if (q<0 || q>100) {
    fprintf(stderr,"reader [input filename.IRF] [qual] [div]\n");
    fprintf(stderr,"qual. must be between 0 and 100.\n");
    exit(0);
    }
  
  n = atoi(argv[3]);
  if (n<1 || n>10) {
    fprintf(stderr,"reader [input filename.IRF] [qual] [div]\n");
    fprintf(stderr,"div must be between 1 and 10.\n");
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

bool iLoadScale(int iFile) {  
  size_t lwidthb, lwidthp;
  int w_x, w_y;
  // copy image  to memory
  
  w_x = 0;
  w_y = 0;
  
  arLines = (JSAMPROW *) malloc((y/n + 1) * sizeof(JSAMPROW));
  assert(arLines != NULL);
  
  lwidthb = 3 * (x/n + 1) * sizeof(JSAMPLE);
  lwidthp = sizeof(Pixel) * x;
  
  inLine = (Pixel *) malloc(lwidthp);
  assert(inLine != NULL);
  
  for (int i = 0; i < y; i++) {
    fprintf(stderr,"reading line %d\r",i);
    if(read(iFile, inLine, lwidthp) < lwidthp) {
      fprintf(stderr,"\nError #%d (%s) reading line %d.\n", errno, strerror(errno), i);
      exit(123);   
      }
    if (i % n == 0) {
      arLines[w_y] = (JSAMPLE *) malloc(lwidthb);
      if (arLines[w_y] == NULL) {
        fprintf(stderr,"\nMemory Allocate failed for line %d\n",i);
        exit(122);
        }
      outLine=arLines[w_y];
      w_x = 0;
      for (int j = 0; j < x; j++) {
        if (j % n == 0) {
          outLine[w_x*3]   = (JSAMPLE) inLine[j].R;
          outLine[w_x*3+1] = (JSAMPLE) inLine[j].G;
          outLine[w_x*3+2] = (JSAMPLE) inLine[j].B;
          ++w_x;
          }
        }
      ++w_y;
      }
    }
  fprintf(stderr,".........copied.\n");  
  fprintf(stderr,"Original x:%d, y:%d\n",x, y);
  fprintf(stderr,"Final x:%d, y:%d\n",w_x, w_y);
  x=w_x;
  y=w_y;
  return true;
  }

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
    jpeg_set_quality(&cinfo, q, FALSE);

    jpeg_start_compress(&cinfo, TRUE);
    while (cinfo.next_scanline < cinfo.image_height)
      jpeg_write_scanlines(&cinfo, &arLines[cinfo.next_scanline], 1);
    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);
    }

char *iLoadHdr(int iFile) {
  char *tmem;
  char *ofName = (char *)malloc(sizeof(char) * 48);    
  tmem = (char *) malloc(sizeof(ImageHeader));
  if (read(iFile, tmem, sizeof(ImageHeader)) != sizeof(ImageHeader) )  {
    fprintf(stderr,"Error #%d (%s) opening input file.\n", errno, strerror(errno));
    exit(125);
    }
  memcpy(&Image, tmem, sizeof(ImageHeader)); 
  assert(Image.name!= NULL);
  sprintf(ofName,"%s.%d.%d.jpg",Image.name,n,q);
  y = Image.y;
  x = Image.x;
  free(tmem);
  return ofName;
  }
