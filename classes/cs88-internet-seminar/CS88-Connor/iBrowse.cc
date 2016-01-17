#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
extern "C" int errno;

void browse(FILE*, int, int);

int main (int argc, char** argv) 
{
//  char specifiedFilename[1024];
//  strcpy(specifiedFilename, getenv("WHATEVER_THE_FILENAME_ENVIRONMENT_VARIABLE_IS_CALLED"));
//  FILE* file = fopen(specifiedFilename, "rt");

//index.dat just a placeholder for env variable which will later be called.
  FILE* file = fopen("index.dat", "rt"); 
  if (file != NULL)
  {
    browse(file, 3, 15);
  }
  else
  {
    printf("File index.dat not found!\n");
  }
}

void browse(FILE* index, int start, int end)
{
  int rowBreak = 4;
 int imagesPerPage = 16;
 
  // Just to be safe...
  assert(index != NULL);
  
  printf("Content-type: text/html\n\n");
  printf("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 3.2 Final//EN\">\n");
  printf("<HTML><HEAD><TITLE>ImageBlaster Browser!</TITLE></HEAD>\n");
  printf("<BODY BGCOLOR=\"#000000\" TEXT=\"#FFFF00\" LINK=\"#8080FF\" VLINK=\"#80FFFF\" ALINK=\"#FF8080\" >");
  printf("<CENTER><IMG SRC=\"http://sigmanu.dartmouth.edu/ib-logo.gif\" ALT=\"ImageBlaster\"><BR>Demonstration Edition<BR>");
  printf("<CENTER>(<A HREF=\"http://sigmanu.dartmouth.edu/\">Click here to return to the main page</A>)<BR>\n"); 
  printf("<TABLE>");
  printf("<TR>");

  char buffer[1024];
  
  int indexNum = 0;
  int breakCount = 1;
    
  while (fgets(buffer, 1024, index) != NULL && indexNum <= end)
  { 
    char* fileName = NULL;
 
    // The first call to strtok establishes the string.
    // The second call will use the old string (strtok keeps a static pointer to the string
    // last passed and advances to the next delimited substring if you just pass it NULL for
    // the second argument).
    fileName = strtok(buffer, " ");
    
    // If we're past (or on) the record we're supposed to start on, print it out.
    if (indexNum >= start)
    {
      printf("<TD>");
      printf("<A HREF=\"http://sigma-nu.dartmouth.edu/cgi-bin/iview.cgi?f=%s&n=3&q=75\">\n", fileName);
      printf("<IMG SRC=\"http://sigma-nu.dartmouth.edu/cgi-bin/iblast-dev.cgi?f=%s&x=150&q=75\"></A><BR>\n", fileName);
      printf("File: %s.IRF\n", fileName);
      printf("</TD>\n");

    if (!(breakCount % rowBreak) && (breakCount)) {  //HACKISH!!  Fix later!
      printf("</TR>");
      printf("<TR>");
      }
    breakCount++;   
    }
      
    // Now we're going to the next record...
    indexNum++;
    }	

  printf("</TR>");
  printf("</TABLE>");
 
  printf("Page spun from iBrowse CGI - Connor Smith<BR>\n");
  printf("</BODY></HTML>\n\n"); 
}