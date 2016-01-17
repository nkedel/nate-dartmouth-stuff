// iview.cc
// n edel -- cs88 
// (last version before ownership of this part of the project passed to connor smith)
//
// receives image #, quality & size (fixed divisor for now) from CGI GET
// generates an HTML page w/ a form to call itself, and an inline image call to
// iblaster.cc to display the actual image
//
// most errors result in a short HTML page indicating the error, and possible
// including a dump of environment variables (probably no longer needed, but I
// was using it early in the process)

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


// iLoadHdr(int
//   loads an image header has into the global ImageHeader structure
//   'Image'; iFile is a file descriptor to the input file.
//   returns the name of the original image which had been converted
//   to IRF
char *iLoadHdr(int iFile);

extern int errno;

// IRF header. Used to load image statistics...
struct sImageHeader { 
  char     name[48];
  unsigned short x, y;
  unsigned short yDegInt;   // degrees of vertical interleaving
  unsigned short xDegInt;   // degrees of horizontal interleaving
  bool     isColor;         // should always be true; false would indicate
  };            //   a grayscale image, if we support those... 

typedef struct sImageHeader ImageHeader;

ImageHeader Image;
int x, y, q, n;

// ifName: input file name
// ifName1: ditto, with full path... don't reveal path to remote users
char ifName[128], ifName1[128];

unsigned char *ofMap;
struct stat stBuf;

// error strings for parse errors
const char *pErrStr[] = {"No Error", "Token repeated", "Invalid Token Value", "Unsupported Token"};

extern char **environ;


// fSize("filename"): return size of a file, or -1 for error
//                    and dump error to log
int fSize(char *sFile) {
  struct stat sBuf;
  int i = stat(sFile, &sBuf);
  if (i == -1) {
    printf("{%s: %s}",sFile, strerror(errno)); 
    return -1;
    };
  return sBuf.st_size;
  }   

// dumperr(): dump environment
void dumperr(void) {
  // this is redundant (but simple ugly looking, nor harmful) on some calls
  printf("Content-type: text/plain\n\n");
  int c =0;
  printf("There was an error processing the HTML GET command. Here is the environment:\n");
  while(environ[c] != NULL) { 
    printf("EV #%d: %s\n",c,environ[c]);
    c++;
    }
  exit(0);
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
    printf("Content-type: text/plain\n\nNo query string\n");
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
      printf("Content-type: text/plain\n\nMalformed token #%d\n", i);
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
  ifName[0] = -1;
  
  for (i = 0; i<n_tok; i++) {
    switch(toupper(*q_pts[i])) {  // parameter should be case ins.
    
      // q: JPEG Quality - Must be between 25 and 100
      //    (q<25 not recommended by IJG, q>100 undefined)
      case 'Q': if (q!=-1) { p_err=1; break; }
                q = val[i];
                if (q<25 || q>100) { p_err=2; break; }
                break;
      
      // n: image size - must be between 1 and 10
      //    integer divisor / should not be combined with x or y          
      case 'N': if (n!=-1) { p_err=1; break; }
                n = val[i];
                if (n<1 || n>10) { p_err=2; break; }
                break;
                
      // f: File # ... converted to 4 digits w/ leading 0s,+.IRF
      case 'F': if (ifName[i] != -1) {p_err=1; break; }
                if (val[i] <= 0 || val[i] >= 999) {p_err=2; break;}
                sprintf(ifName, "%04d.IRF", val[i]);
                sprintf(ifName1,"/home/httpd/images/%04d.IRF", val[i]);
                break;
      
      case 'X': // not implemented yet; note that when they are, either
      case 'Y': // (X || Y) && !N or N && !(X || Y) will be the forms supprted.
      default:  p_err=3;
                break;
      }
    if (p_err != 0) break;
    }
  
  // if we got an error, dump out parameters to the log and exit
  if (p_err != 0) {
    printf("Content-type: text/plain\n\nParse error#%d in token %d\n",p_err,i);
    for (i=0; i<n_tok; i++) 
      printf("Content-type: text/plain\n\nT%d: \"%s\" = %04d\n", i, q_pts[i], val[i]);
    exit(-1);
    }
  
  if ((i=open(ifName1, O_RDONLY, 0 )) == -1) {
    printf("Content-type: text/plain\n\nError opening %s: %s\n",ifName,strerror(errno));
    exit(-2);
    }
  return i;
  }

    
int main(int argc, char **argv) {
  int inFile;
  FILE *outFile;

  int i;
  inFile = parse_query();
          
  // load header
  iLoadHdr(inFile);

  printf("Content-type: text/html\n\n");
  printf("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 3.2 Final//EN\">\n");
  printf("<HTML><HEAD><TITLE>ImageBlaster Results!</TITLE></HEAD>\n");
  printf("<BODY BGCOLOR=\"#000000\" TEXT=\"#FFFF00\" LINK=\"#8080FF\" VLINK=\"#80FFFF\" ALINK=\"#FF8080\" >");
  printf("<CENTER><IMG SRC=\"http://sigmanu.dartmouth.edu/ib-logo.gif\" ALT=\"ImageBlaster\"><BR>Demonstration Edition<BR>"); 
  printf("<CENTER>(<A HREF=\"http://sigmanu.dartmouth.edu/\">Click here to return to the main page</A>)<BR>\n"); 
  printf("Source File: %s (%d bytes) %dx%d IRF File<P>",ifName,fSize(ifName1),Image.x,Image.y);
  printf("<CENTER><IMAGE SRC=\"iblaster.cgi?file=%s&n=%d&q=%d\" ALT=\"Image: %s\"></CENTER>",ifName,n,q,ifName);
  printf("<FORM ACTION=\"/cgi-bin/iview.cgi\" METHOD=GET>PICTURE:<SELECT NAME=\"file\">");
  printf("<OPTION SELECTED>%s\n", ifName);
  printf("<OPTION>0002.IRF<OPTION>0003.IRF\n<OPTION>0004.IRF\n<OPTION>0005.IRF\n</SELECT>\n");
  printf("<BR>Select JPEG Quality:\n");
  printf("<SELECT NAME=\"q\">\n<OPTION SELECTED>%d\n",q);
  printf("<OPTION>99\n<OPTION>85\n<OPTION>75\n<OPTION>50\n<OPTION>35\n<OPTION>25\n</SELECT><BR>\n");
  printf("Pick a size (1/n of original)<SELECT NAME=\"n\"><OPTION SELECTED>%d\n",n);
  printf("<OPTION>1\n<OPTION>2\n<OPTION>3\n<OPTION>4\n<OPTION>5\n<OPTION>6\n<OPTION>7\n<OPTION>8\n<OPTION>9\n<OPTION>10\n</SELECT>\n");
  printf("<INPUT TYPE=\"submit\"></FORM><HR>\n");
  printf("<CENTER>(<A HREF=\"http://sigmanu.dartmouth.edu/\">Click here to return to the main page</A>)<BR>\n"); 
  printf("This page was automagically generated by the cgi script iview.cgi, written by n edel<BR>\n");
  printf("</BODY></HTML>\n\n");
  close(inFile);
  }

// iLoadHdr(int
//   loads an image header has into the global ImageHeader structure
//   'Image'; iFile is a file descriptor to the input file.
//   returns the name of the original image which had been converted
//   to IRF
char *iLoadHdr(int iFile) {
  char *ofName = (char *)malloc(sizeof(char) * 48);    
  if (ofName == NULL) {
    printf("Content-type: text/plain\n\nerror malloc'ing ofName string %d (%s)\n", errno, strerror(errno));
    exit(-3);
    }
    
  char *tHdr = (char *) malloc(sizeof(ImageHeader));
  if (tHdr == NULL) {
    printf("Content-type: text/plain\n\nerror malloc'ing image header %d (%s)\n", errno, strerror(errno));
    exit(-3);
    }

  if (read(iFile, tHdr, sizeof(ImageHeader)) != sizeof(ImageHeader) )  {
    printf("Content-type: text/plain\n\nerror reading image header %d (%s)\n", errno, strerror(errno));
    exit(-3);
    }

  memcpy(&Image, tHdr, sizeof(ImageHeader)); 
  free(tHdr);

  sprintf(ofName,"%s.%d.%d.jpg",Image.name,n,q);
  y = Image.y;
  x = Image.x;

  return ofName;
  }
