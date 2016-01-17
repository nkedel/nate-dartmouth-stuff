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

char ifName[128], ifName1[128];

unsigned char *ofMap;
struct stat stBuf;

extern char **environ;

int fSize(char *sFile) {
  struct stat sBuf;
  int i = stat(sFile, &sBuf);
  if (i == -1) {
    printf("{%s: %s}",sFile, strerror(errno)); 
    return -1;
    };
  return sBuf.st_size;
  }   

void dumperr(void) {
  printf("Content-type: text/plain\n\n");
  int c =0;
  printf("There was an error processing the HTML GET command. Here is the environment:\n");
  while(environ[c] != NULL) { 
    printf("EV #%d: %s\n",c,environ[c]);
    c++;
    }
  exit(0);
  }

int parse_query(void) {
  char *q_str = getenv("QUERY_STRING");
  char *q_pts[3], *q_val[3];
  int i;
  
  q_pts[0] = q_str;
  if (q_pts[0] == NULL) {
    printf("Content-type: text/plain\n\nMissed first token.\n");
    return -1;
    }

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

  if (q_val[0] == NULL || strlen(q_val[0]) == 0) return -1;
  strcpy(ifName, q_val[0]);
  strcpy(ifName1,"/home/httpd/images/");
  strcat(ifName1, ifName);
  
//  printf("Content-type: text/plain\n\n%s, %s\n",ifName,ifName1);
  if ((i=open(ifName1, O_RDONLY, 0 )) == -1) {
    printf("Content-type: text/plain\n\nError opening %s: %s\n",ifName1,strerror(errno));
    exit(1);
    }
  return i;
  }
  
int main(int argc, char **argv) {
  int inFile;
  FILE *outFile;
  char *ofName, ofName1[128], ofName2[128];

  int i;
  if ((inFile = parse_query()) == -1) dumperr();
          
  // load header

  // load header
  ofName = iLoadHdr(inFile);
  strcpy(ofName1,"http://sigmanu.dartmouth.edu/dynimage/");
  strcat(ofName1,ofName);
  strcpy(ofName2,"/home/httpd/html/dynimage/");
  strcat(ofName2,ofName);

// caching code here -- skip to next comment if we have a valid cached copy!
  iLoadScale(inFile);

  // open the output file
  outFile = fopen(ofName2, "wb");
  if (outFile == NULL) {
    fprintf(stderr,"Error #%d (%s) opening output file.\n", errno, strerror(errno));
    exit(124);
    }
  iCompress(outFile);
// end cache code

  printf("Content-type: text/html\n\n");
  printf("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 3.2 Final//EN\">\n");
  printf("<HTML><HEAD><TITLE>ImageBlaster Results!</TITLE></HEAD>\n");
  printf("<BODY BGCOLOR=\"#000000\" TEXT=\"#FFFF00\" LINK=\"#8080FF\" VLINK=\"#80FFFF\" ALINK=\"#FF8080\" >");
  printf("<CENTER><IMG SRC=\"http://sigmanu.dartmouth.edu/ib-logo.gif\" ALT=\"ImageBlaster\"><BR>Demonstration Edition<BR>"); 
  printf("<CENTER>(<A HREF=\"http://sigmanu.dartmouth.edu/\">Click here to return to the main page</A>)<BR>\n"); 
  printf("Source File: %s (%d bytes) %dx%d IRF File<P>",ifName,fSize(ifName1),Image.x,Image.y);
  printf("<CENTER><IMAGE SRC=\"%s\" ALT=\"Image: %s\"></CENTER>",ofName1,ofName);
  printf("Final  File: %s (%d bytes) %dx%d q %d JPEG File<P><HR>",ofName,fSize(ofName2),x,y,q);
  printf("<FORM ACTION=\"/cgi-bin/iblast.cgi\" METHOD=GET>PICK A PICTURE:<SELECT NAME=\"name\">");
  //<OPTION>0001.IRF
  printf("<OPTION>0002.IRF\n");
  printf("<OPTION SELECTED>0003.IRF<OPTION>0004.IRF<OPTION>0005.IRF");
  // <OPTION>0006.IRF<OPTION>0007.IRF");
  printf("</SELECT><BR>Select a quality (HIGHER# = HIGHER QUALITY):<SELECT NAME=\"q\"><OPTION>99<OPTION>85<OPTION SELECTED>75<OPTION>50<OPTION>35<OPTION>25</SELECT><BR>");
  printf("Pick a size divisor: (size = 1/n of original)<SELECT NAME=\"n\"><OPTION>1<OPTION>2<OPTION SELECTED>3<OPTION>4<OPTION>5<OPTION>6<OPTION>7<OPTION>8<OPTION>9<OPTION>10</SELECT>");
  printf("<INPUT TYPE=\"submit\"></FORM> <HR>");
  printf("<CENTER>(<A HREF=\"http://sigmanu.dartmouth.edu/\">Click here to return to the main page</A>)<BR>\n"); 
  printf("This page was automagically generated by the cgi script iblast.cgi, written by n edel<BR>\n");
  printf("</BODY></HTML>\n\n");
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
    fclose(oFile);
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

