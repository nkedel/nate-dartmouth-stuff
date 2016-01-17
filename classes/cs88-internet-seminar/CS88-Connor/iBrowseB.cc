#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
extern "C" int errno;

void browse(FILE* index, int start);

int main (int argc, char** argv) 
{
//  char specifiedFilename[1024];
//  strcpy(specifiedFilename, getenv("WHATEVER_THE_FILENAME_ENVIRONMENT_VARIABLE_IS_CALLED"));
//  FILE* file = fopen(specifiedFilename, "rt");

//index.dat just a placeholder for env variable which will later be called.
  char *q_str = getenv("QUERY_STRING");
    char *q_pts[9], *q_val[9];              // 9 params + 9 val strings
  int val[9];                             // and 9 values
  int i, n_tok, p_err;                    
      int s = -1;
      
      
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
	if (n_tok >= 9) break;
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
	if (n_tok >= 9) break;
    }
  
	for (i = 0; i<n_tok; i++) {
    
    
    switch(*q_pts[i]) {  // parameter should be case ins.
    
      // q: JPEG Quality - Must be between 25 and 100
      //    (q<25 not recommended by IJG, q>100 undefined)
      case 's': if (s!=-1) { p_err=1; break; }
                s = val[i];
                if (s<1 || s>30) { p_err=2; break; }
                break;
      
      default:
      	printf("Something's wrong with the token!");
      	exit(1);
		}
	}
	
  FILE* file = fopen("index.dat", "rt"); 
  if (file != NULL)
  {
    browse(file, s);
  }
  else
  {
    printf("File index.dat not found!\n");
  }

}

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
			if (start - imagesPerPage < 0)
			  	{
			  	prevPage = 0;
				}		
			else
			 	{
				prevPage = start - imagesPerPage;
			  	}
//  printf("<CENTER>(<A HREF=\"http://sigmanu.dartmouth.edu/\
//">Click here to return to the main page</A>)<BR>\n")

			printf("</TR>");
			printf("</TABLE>");
			printf("<CENTER>(<A HREF=\"c.cgi?s=%d\">Click here for previous %d images</A>)<BR>\n", prevPage, imagesPerPage);
			printf("<CENTER>(<A HREF=\"http://sigmanu.dartmouth.edu/\">Click here to return to the main page</A>)<BR>\n");
			printf("Page spun from iBrowse CGI - Connor Smith<BR>\n");
			printf("</BODY></HTML>\n\n"); 
			
			return;
	  		}
	  	else
	  		{	
	  			printf("</TR>");
			  	printf("</TABLE>");
			  	printf("<CENTER>(<A HREF=\"http://sigmanu\">Click here to return to main page</A>)<BR>");
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
				if (start - imagesPerPage < 0)
				  	prevPage = 0;
				else 
					prevPage = start - imagesPerPage;
		
				  	
				printf("</TR>");
				printf("</TABLE>");
				printf("<CENTER>(<A HREF=\"c.cgi?s=%d\">Click here for previous %d images</A>)<BR>\n", prevPage, imagesPerPage);
				printf("<CENTER>(<A HREF=\"c.cgi?s=%d\">Click here for next %d images</A>)<BR>\n", nextPage, imagesPerPage);
				printf("<CENTER>(<A HREF=\"http://sigmanu\">Click here to return to main page</A>)<BR>\n");
				printf("Page spun from iBrowse CGI - Connor Smith<BR>\n");
				printf("</BODY></HTML>\n\n"); 
				  	
				return;
				}
		  	else
		  		{	
		  			printf("</TR>");
				  	printf("</TABLE>");
					printf("<CENTER>(<A HREF=\"c.cgi?s=%d\">Click here for next %d images</A>)<BR>\n", nextPage, imagesPerPage);				  	printf("<B><AHREF=\"sigmanu\">Click here to return to main page</A></B>");
					printf("<CENTER>(<A HREF=\"http://sigmanu\">Click here to return to main page</A>)<BR>\n");
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
			      printf("<A HREF=\"http://sigma-nu.dartmouth.edu/cgi-bin/iview.cgi?file=%s.IRF&n=3&q=75\">\n", fileName);
			      printf("<IMG SRC=\"http://sigma-nu.dartmouth.edu/cgi-bin/iblaster.cgi?f=%s&x=150&q=75\"></A><BR>\n", fileName);
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