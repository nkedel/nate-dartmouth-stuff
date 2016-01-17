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

extern char **environ;
const char qs_text[] = "QUERY_STRING=";

void dumperr(int argc, char **argv) {
  printf("Content-type: text/plain\n\n");
  int c = argc;
  printf("There was an error processing the HTML GET command. Here is the system status:\n");
  while (c>0) { 
    printf("Arg #%d: %s\n",c,argv[c]);
    c--;
    }
  while(environ[c] != NULL) { 
    printf("EV #%d: %s\n",c,environ[c]);
    c++;
    }
  exit(0);
  }
  
int find_query(void) {
  int i = 0, qs_len = strlen(qs_text);
  bool found = false;
  while(environ[i] != NULL && found == false) {
     if (strlen(environ[i]) >= qs_len) {
      found = true;    
      for (int j=0; j < qs_len; j++) 
        if (environ[i][j] != qs_text[j]) {
          found=false; 
          break;
          }   
      if (found==true) return i;
      i++;
      }
    }
  return -1;
  }

int parse_query(int i) {
  char *q_str = environ[i];
  char *q_pts[3], *q_val[3];
  
  q_pts[0] = strchr(q_str,'=');
  if (q_pts[0] == NULL) {
    printf("Content-type: text/plain\n\nMissed first token.\n");
    return -1;
    }

  *q_pts[0]=0;
  q_pts[0]++;
 
  q_pts[1] = strchr(q_pts[0],'&');
  if (q_pts[1] == NULL) {
    printf("Content-type: text/plain\n\nMissed second token.\n");
    return -1;
    }

  *q_pts[1]=0;
  q_pts[1]++;

  q_pts[2] = strchr(q_pts[1],'&');
  if (q_pts[2] == NULL) {
    printf("Content-type: text/plain\n\nMissed third token.\n");
    return -1;
    } 
     
  *q_pts[2]=0;
  q_pts[2]++;

  q_val[0] = strchr(q_pts[0],'=');
  if (q_val[0] == NULL) {
    printf("Content-type: text/plain\n\nMissed first value.\n");
    return -1;
    }

  *q_val[0]=0;
  q_val[0]++;

  q_val[1] = strchr(q_pts[1],'=');
  if (q_val[1] == NULL) {
    printf("Content-type: text/plain\n\nMissed second value.\n");
    return -1;
    }

  *q_val[1]=0;
  q_val[1]++;

  q_val[2] = strchr(q_pts[2],'=');
  if (q_val[2] == NULL) {
    printf("Content-type: text/plain\n\nMissed first value.\n");
    return -1;
    }

  *q_val[2]=0;
  q_val[2]++;
  
  q = atoi(q_val[1]);
  if (q<0 || q>100) return -1;
  
  n = atoi(q_val[2]);
  if (n<1 || n>10)  return -1;

  return (open(q_val[0], O_RDONLY, 0 ));
  }
  
int main(int argc, char **argv) {
  int inFile;
//  FILE *outFile;
  char *ofName;

  int i = find_query();
  if (i == -1) dumperr(argc, argv);
  if ((inFile = parse_query(i)) == -1) 
    dumperr(argc, argv);
      
  // load header
  iLoadScale(inFile);
  printf("Content-type: image/jpeg\n\n");
  iCompress(stdout);
  
//  fclose(outFile);
  close(inFile);
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
    if(read(iFile, inLine, lwidthp) < lwidthp) {
      exit(123);   
      }
    if (i % n == 0) {
      arLines[w_y] = (JSAMPLE *) malloc(lwidthb);
      if (arLines[w_y] == NULL) {
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
    printf("\n\n");
    jpeg_destroy_compress(&cinfo);
    }

char *iLoadHdr(int iFile) {
  char *tmem;
  char *ofName = (char *)malloc(sizeof(char) * 48);    
  tmem = (char *) malloc(sizeof(ImageHeader));
  if (read(iFile, tmem, sizeof(ImageHeader)) != sizeof(ImageHeader) )  {
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
