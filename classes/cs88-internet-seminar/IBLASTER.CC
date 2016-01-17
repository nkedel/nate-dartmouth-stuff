// iblaster.cc
// n edel -- cs88
//
// receives image #, quality & size (fixed divisor for now) from CGI GET,
// checks if generated image exists (and if so, if it is newer than the 
// corresponding source image)... if not, generate new .jpg; in either case,
// then deliver "image/jpeg" data to stdout.
//
// most errors result in output to stderr (to be caught by apache's log) and
// an exit(#) as any error message to stdout will be ignored by the browser,
// since it will be expecting an image.

#include <unistd.h>
#include <ctype.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "include/jpeglib.h"



// IRF image header; I don't think we're going to want to use 
// interleaving after all, but since this is the header all our 
// test images use, the extra pair of unsigned shorts is harmless.
// (also note that this is rather non-portable; this is intended to be
//  run on an Intel system running Linux and would probably should be 
//  rewritten more portably)
struct sImageHeader { 
  char     name[48];
  unsigned short x, y;
  unsigned short yDegInt;   // degrees of vertical interleaving
  unsigned short xDegInt;   // degrees of horizontal interleaving
  bool     isColor;         // should always be true; false would indicate
  };                        // a grayscale image, if we support those... 

// Pixel structure for IRF. Should probably be rewritten more portably.
struct sPixel {
  unsigned char R;
  unsigned char G;
  unsigned char B;
  };

// structs necessary for libjpeg-6
struct jpeg_compress_struct cinfo;
struct jpeg_error_mgr jerr;

// the compiler was being picky about 
// "typedef struct sImageHeader { ... } ImageHeader" 
// and same for sPixel/Pixel so these are here
typedef struct sImageHeader ImageHeader;
typedef struct sPixel Pixel;

// our global image info, to same passing all of this between a lot 
// of functions...
ImageHeader iImage;    // IRF image header
JSAMPROW *imgLines, *tagLines;    // image raster array
int x, y, q, n, tag;       // jpeg image parameters

// iCompress(FILE *oFile): 
//   compress the raster array using libjpeg; oFile is a C stream 
//   indicating the output file
//   Global 'q' indicates quality/degree of compression.
bool iCompress(FILE *oFile);

// iLoadScale(int iFile):
//   loads an IRF image whose header has already been loaded (into the 
//   global ImageHeader Image) into a new malloc'd raster array pointed
//   to by (global) arLines; scale image by integer divisor (global) 'n'
//   iFile is a file descriptor to the input file.
bool iLoadScale(int iFile);
bool TagOverlay(void);

// iLoadHdr(int
//   loads an image header has into the global ImageHeader structure 
//   'Image'; iFile is a file descriptor to the input file.
//   returns the name of the original image which had been converted 
//   to IRF
char *iLoadHdr(int iFile, ImageHeader &Image);

extern int errno;


// file name and full path to file; not 100% sure these still need to be 
// globals (they did under the original version)
char ifName[128];

// fSize("filename"): return size of a file, or -1 for error
//                    and dump error to log
int fSize(const char *sFile) {
  struct stat sBuf;
  int i = stat(sFile, &sBuf);
  if (i == -1) {
    fprintf(stderr,"{%s: %s}",sFile, strerror(errno)); 
    return -1;
    };
  return sBuf.st_size;
  }   

// isNewer("file1", "file2"): return true if file1 is newer than file2, 
//   or if file1 exists, but file2 does not. Print error to log and exit(-1)
//   if file1 does does not exist or its inode is unreadable.
bool isNewer(char *sFile1, char *sFile2) {
  struct stat sBuf1, sBuf2;
  int i = stat(sFile1, &sBuf1);
  if (i == -1) {
    fprintf(stderr, "isNewer F1: {%s: %s}",sFile1, strerror(errno)); 
    exit(-1);
    };
  i = stat(sFile2, &sBuf2);
  if (i == -1) {
    // if File2 doesn't exist, which is the most likely reason we've 
    // gotten an error, we have a cache miss and need to generate file2
    // so let's call File1 newer.
    return true;
    };
    
  // if file1 is newer than file2, the source has been updated and we need
  // to re-generate 
  if (sBuf1.st_mtime > sBuf2.st_mtime) return true;
  
  // otherwise we have a newer jpeg and thus a "cache hit"
  return false;
  }
  
// parse_query(): parse CGI GET query, taken from environment
//   break apart x=y&x=y into separate strings, then check for 
//   valid values and put into global parameter variables (ifName,
//   n, q at present; will add x,y when I implement arbitrary 
//   scaling, also some flags for other features)
int parse_query(void) {
  char *q_str = getenv("QUERY_STRING");   // our query
  char *q_pts[9], *q_val[9];              // 9 params + 9 val strings
  int val[9];                             // and 9 values
  int i, n_tok, p_err;                    
  
  i = 0; n_tok = 0; p_err=0;
  q_pts[0] = q_str;                       // grab our first parameter
  if (q_pts[0] == NULL) {                 // check if it's a good string
    fprintf(stderr, "No query string\n");
    exit(-1);
    }
  n_tok++;
    
  while(1) {                              // scan through for more tokens
    q_pts[n_tok] = strchr(q_pts[n_tok-1],'&');
    if (q_pts[n_tok] == NULL) break;      // and if we find them, grab the 
    *q_pts[n_tok]=0;                      // next parameter and terminate
    q_pts[n_tok]++;                       // the previous string.
    n_tok++;
    }
  
  while(i<n_tok) {                        // then scan through and separate
    q_val[i] = strchr(q_pts[i],'=');      // each parameter from its value
    if (q_val[i] == NULL) {
      fprintf(stderr,"Malformed token #%d\n", i);
      exit(-1);
      }
    *q_val[i]=0;
    q_val[i]++;
    val[i] = (int) strtol(q_val[i],NULL,10);
    i++;
    }

  // guard values.
  q = -1;
  n = -1;
  x = -1;
  y = -1;
  tag = -1;
  ifName[0] = -1;
  
  for (i = 0; i<n_tok; i++) {
    switch(toupper(*q_pts[i])) {  // parameter should be case ins.
    
      // q: JPEG Quality - Must be between 25 and 100
      //    (q<25 not recommended by IJG, q>100 undefined)
      case 'Q': if (q!=-1) { p_err=1; break; }
                q = val[i];
                if (q<25 || q>100) { p_err=2; break; }
                break;

      case 'T': if (tag!=-1) { p_err=1; break; }
                tag = val[i];
                if (tag<10 || tag>50) { p_err=2; break; }
                break;

      
      // n: image size - must be between 1 and 10
      //    integer divisor / should not be combined with x or y        
      //    if we wanted to be really studly, we could replace this
      //    with a FP value...  
      case 'N': if (n!=-1 || x!=-1 || y!=-1) { p_err=1; break; }
                n = val[i];
                if (n<1 || n>10) { p_err=2; break; }
                break; 
                              
      // f: File # ... converted to 4 digits w/ leading 0s,+.IRF
      case 'F': if (ifName[i] != -1) {p_err=1; break; }
                if (val[i] <= 0 || val[i] >= 999) {p_err=2; break;}
                sprintf(ifName,"/home/httpd/images/%04d.IRF", val[i]);
                break;
      
                // (X || Y) && !N or N && !(X || Y) will be the forms supprted.
                // X and Y must be between 10 pixels (arbitrary, and purely to
                // prevent garbage values) and 4096 pixels (as anything larger
                // is likely to be garbage as well, and at 48MB+, a 4096x4096
                // is likely to choke the system)
      case 'X': if (n!=-1) { p_err=4; break; }
                if (x!=-1) { p_err=1; break; }
                x = val[i];
                if (x < 10 || x > 4096) { p_err=2; break; }
                break;
      case 'Y': if (n!=-1) { p_err=4; break; }
                if (y!=-1) { p_err=1; break; }
                y = val[i];
                if (y < 10 || y > 4096) { p_err=2; break; }
                break;
                
      case 'K': // timed key from other CGI script... not supported yet
      case 'P': // password from other CGI script... not supported yet
      default:  p_err=3;
                break;
      }
    if (p_err != 0) break;
    }
  
  // if we got an error, dump out parameters to the log and exit
  if (p_err != 0) {
    fprintf(stderr,"Parse error#%d in token %d\n",p_err,i);
    for (i=0; i<n_tok; i++) 
      fprintf(stderr,"T%d: \"%s\" = %04d\n", i, q_pts[i], val[i]);
    exit(-1);
    }
  
  if ((i=open(ifName, O_RDONLY, 0 )) == -1) {
    fprintf(stderr,"Error opening %s: %s\n",ifName,strerror(errno));
    exit(-2);
    }
  return i;
  }

  
int main(int argc, char **argv) {
  int inFile;
  FILE *outFile;
  char *ofName, ofName1[128];

  int i;

  // parse our query and open the input file;
  inFile = parse_query();
  
  // load header
  ofName = iLoadHdr(inFile, iImage);

  // send our Content-type header: browsers seem to time out less if we send this 
  //   immediately, before doing any image processing  
  printf("Content-type: image/jpeg\n");

  // generate path to possible cached file
  sprintf(ofName1,"/home/httpd/images/dyn/%s",ofName);  

  // if out of date or not in cache, generate new cached file
  if (isNewer(ifName,ofName1)) {
    iLoadScale(inFile);
    
    if (tag != -1) TagOverlay();
    
    // open the output file
    outFile = fopen(ofName1, "wb");
    if (outFile == NULL) {
      fprintf(stderr,"Error #%d (%s) opening output file.\n", errno, strerror(errno));
      exit(124);
      }
    iCompress(outFile);
    fclose(outFile);
    }
  close(inFile);
  
  // deliver cached image
  int inC = fSize(ofName1);
  
  int outC = 0, tC=0, buf_sz = inC % 8192;
  char *buf;
  buf = (char *)malloc(buf_sz);
  if (buf == NULL) {
    fprintf(stderr,"error malloc'ing read buffer %d (%s)\n", errno, strerror(errno));
    exit(-3);
    }
    
  printf("Content-length: %d\n\n", inC);
  outFile = fopen(ofName1, "rb");

  // read-write loop.
  while (outC < inC) {
    errno = 0;
    tC = (int) fread(buf, sizeof(char), buf_sz, outFile);
    outC += (int) fwrite(buf, sizeof(char), tC, stdout);
    if (errno != 0) {
      fprintf(stderr, "error writing or reading %d (%s)\n", errno, strerror(errno));
      exit(-4);
      }
    usleep(2000); // throttle output by waiting 2msec... 
    }  
  fclose(outFile);
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
  f_x = (double) iImage.x / (double) x;
  f_y = (double) iImage.y / (double) y;

//  fprintf(stderr,"iLoadScale called %d.%d -> %d.%d (%3f,%3f)\n",iImage.x,iImage.y,x,y,f_x,f_y);
    
  imgLines = (JSAMPROW *) malloc((y + 1) * sizeof(JSAMPROW));
  if (imgLines == NULL) {
    fprintf(stderr,"error malloc'ing raster array %d (%s)\n", errno, strerror(errno));
    exit(-3);
    }
  
  lwidthb = 3 * (x + 1) * sizeof(JSAMPLE);
  lwidthp = sizeof(Pixel) * iImage.x;
  
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
    imgLines[i] = (JSAMPLE *) malloc(lwidthb);
    if (imgLines[i] == NULL) {
      fprintf(stderr,"error malloc'ing raster line %d: %d (%s)\n", i, errno, strerror(errno));
      exit(-3);
      }
//    fprintf(stderr, "\nAllocated line %d at %d\n", i, imgLines[i]);
    outLine=imgLines[i];
    for (int j = 0; j < x; j++) {
      w_x = (int) ((double) j * f_x);            // find nearest neighbor
      outLine[j*3]   = (JSAMPLE) inLine[w_x].R;
      outLine[j*3+1] = (JSAMPLE) inLine[w_x].G;
      outLine[j*3+2] = (JSAMPLE) inLine[w_x].B;
      }
    }
//  fprintf(stderr,"\n");  
//  fprintf(stderr,"Original x:%d, y:%d\n",iImage.x, iImage.y);
//  fprintf(stderr,"Final x:%d, y:%d\n",x, y);
  return true;
  }

bool TagOverlay(void) {  
  size_t lwidthb, lwidthp;    // # bytes for jpeg raster array and for pixel array lines
  Pixel *inLine;
  JSAMPROW *arLines;
  JSAMPROW outLine;
  ImageHeader tagHdr;
  char tagFName[127];
  int i, tFile, offX, offY;
  
  // copy image  to memory
  // produce our scale factors
  
  sprintf(tagFName,"/home/httpd/images/tag/%04d", tag);
  if ((i=open(tagFName, O_RDONLY, 0 )) == -1) {
    fprintf(stderr,"Error opening %s: %s\n",tagFName,strerror(errno));
    exit(-2);
    }
  tFile = i;

  char *tHdr = (char *) malloc(sizeof(ImageHeader));
  if (tHdr == NULL) {
    fprintf(stderr,"error malloc'ing image header %d (%s)\n", errno, strerror(errno));
    exit(-3);
    }

  if (read(tFile, tHdr, sizeof(ImageHeader)) != sizeof(ImageHeader) )  {
    fprintf(stderr, "error reading image header %d (%s)\n", errno, strerror(errno));
    exit(-3);
    }

  memcpy(&tagHdr, tHdr, sizeof(ImageHeader)); 
  free(tHdr);

  if (tagHdr.x > x || tagHdr.y > y) { 
    fprintf(stderr,"Error: tag %d size of %d.%d > image width of %d.%d\n",tag,tagHdr.x,tagHdr.y,x,y);
    exit(-2);
    }

  offX = (x - tagHdr.x);
  offY = (y - tagHdr.y);
  
  lwidthp = sizeof(Pixel) * tagHdr.x;
  inLine = (Pixel *) malloc(lwidthp);
  if (inLine == NULL) {
    fprintf(stderr,"error malloc'ing tag line: %d (%s)\n", errno, strerror(errno));
    exit(-3);
    }

  for (i = 0; i < tagHdr.y; i++) {
    if(read(tFile, inLine, lwidthp) < lwidthp) {
        // either we have an incomplete file, or we have a read error
        fprintf(stderr,"error reading tag line %d: %d (%s)\n", i, errno, strerror(errno));
        exit(-3);
        }
    // found nearest line, copy it to raster array
    outLine=imgLines[i+offY];
    for (int j = 0; j < tagHdr.x; j++) 
      if (inLine[j].R != 192 && inLine[j].G != 128 && inLine[j].B != 64) {
        outLine[(j+offX)*3]   = (JSAMPLE) inLine[j].R;
        outLine[(j+offX)*3+1] = (JSAMPLE) inLine[j].G;
        outLine[(j+offX)*3+2] = (JSAMPLE) inLine[j].B;
        } 
    }
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
      jpeg_write_scanlines(&cinfo, &imgLines[cinfo.next_scanline], 1);
    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);
    }

// iLoadHdr(int
//   loads an image header has into the global ImageHeader structure 
//   'Image'; iFile is a file descriptor to the input file.
//   returns the name of the original image which had been converted 
//   to IRF
char *iLoadHdr(int iFile, ImageHeader &Image) {
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
    float scf = (float) y / (float) Image.y; // scale factor for x
    x = (int) ((float) Image.x * scf);      
    }
  else if (y == -1 && x != -1) {
    float scf = (float) x / (float) Image.x; // scale factor for y
    y = (int) ((float) Image.y * scf);      
    }

  // else x != -1 && Y != -1 ergo x and y are cool.

  // we are not allowing upscaling, also catch errors.
  if (x < 10 || x > Image.x) { fprintf(stderr,"x not in 10<x<Image.x: %d, %d\n", x, Image.x); x = Image.x; }
  if (y < 10 || y > Image.y) { fprintf(stderr,"y not in 10<y<Image.y: %d, %d\n", y, Image.y); y = Image.y; }
  
  if (tag == -1) sprintf(ofName,"%s.%d.%d.%d.jpg",Image.name,x,y,q);
          else sprintf(ofName,"%s.%d.%d.%d.%d.jpg",Image.name,x,y,q,tag);
  return ofName;
  }

