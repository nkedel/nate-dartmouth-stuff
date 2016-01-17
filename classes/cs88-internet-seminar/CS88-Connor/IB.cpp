#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "browse.h"
extern "C" int errno;



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
    
    int s = -1;
    
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
