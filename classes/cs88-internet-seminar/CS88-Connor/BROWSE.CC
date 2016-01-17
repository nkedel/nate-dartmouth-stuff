#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <browse.h>
extern "C" int errno;

void browse(FILE* index, int start)
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
  int nextPage = 0;
  int prevPage = 0;
  
  
  while (1)
    {
	  if (fgets(buffer, 1024, index) == NULL)
	  	{
		if (start != 0)
			{
			if (start - imagesPerPage < 1)
			  	prevPage = 1;
			else {
				prevPage = start - imagesPerPage;
			  	
			  	printf("</TR>");
			  	printf("</TABLE>");
			  	printf("<B><AHREF=\"cgi-bin/iview.cgi?s=%d\">Click here for previous %d images</A></B>\n", prevPage, imagesPerPage);
			  	printf("<B><AHREF=\"sigmanu\">Click here to return to main page</A></B>");
			  	printf("Page spun from iBrowse CGI - Connor Smith<BR>\n");
			  	printf("</BODY></HTML>\n\n"); 
				}
			return;
			}
	  	else
	  		{	
	  			printf("</TR>");
			  	printf("</TABLE>");
			  	printf("<B><AHREF=\"sigmanu\">Click here to return to main page</A></B>");
			  	printf("Page spun from iBrowse CGI - Connor Smith<BR>\n");
			  	printf("</BODY></HTML>\n\n"); 
	  		return;
	  		}

		}
		
	  else if (breakCount > imagesPerPage)
				{
				nextPage = (start + imagesPerPage);
				
				if (start != 0)
					{
					if (start - imagesPerPage < 1)
					  	prevPage = 1;
					else {
						prevPage = start - imagesPerPage;
					  	
					  	printf("</TR>");
					  	printf("</TABLE>");
					  	printf("<B><AHREF=\"cgi-bin/iview.cgi?s=%d\">Click here for previous %d images</A></B>\n", prevPage, imagesPerPage);
					  	printf("<B><AHREF=\"cgi-bin/iview.cgi?s=%d\">Click here for next %d images</A></B>\n", nextPage, imagesPerPage);
					  	printf("<B><AHREF=\"sigmanu\">Click here to return to main page</A></B>");
					  	printf("Page spun from iBrowse CGI - Connor Smith<BR>\n");
					  	printf("</BODY></HTML>\n\n"); 
						}
					return;
					}
			  	else
			  		{	
			  			printf("</TR>");
					  	printf("</TABLE>");
					  	printf("<B><AHREF=\"cgi-bin/iview.cgi?s=%d\">Click here for next %d images</A></B>\n", nextPage, imagesPerPage);
					  	printf("<B><AHREF=\"sigmanu\">Click here to return to main page</A></B>");
					  	printf("Page spun from iBrowse CGI - Connor Smith<BR>\n");
					  	printf("</BODY></HTML>\n\n"); 
			  		return;
		  			}	  	
				}  		

	  else	   //&& breakCount <= imagesPerPage)
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
	}
}