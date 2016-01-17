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
                sprintf(ifName,"/home/httpd/images/%04d.IRF", val[i]);
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
