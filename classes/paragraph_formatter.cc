// para.cc
//
// neatly pringing paragraph program -- CLR 16-2 problem solution

#include <iostream.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

const int MAX_WORD_LENGTH=20;
const int LINE_LENGTH =   75;
const int NUM_WORDS =     100;
const int FLAG =          0 - LINE_LENGTH;
const int MCUBED =        LINE_LENGTH * LINE_LENGTH * LINE_LENGTH;

int l[NUM_WORDS+1]; // length of each word
int c[NUM_WORDS+1]; // optimal cost of paragraph with n words
int w[NUM_WORDS+1]; // number of words in prior component for paragraph w/ n words
int p_a[NUM_WORDS+1][NUM_WORDS+1];  // cost by line for line with words i..j
int s_a[NUM_WORDS+1][NUM_WORDS+1];  // extra spaces on line with words i..j  

// define extra spaces function/table
inline int s(int i, int j) {
  if (s_a[i][j] != FLAG) 
        return s_a[i][j]; // if we've already computed this, return it.
  int k, sum=0;
  for (k=i;k<=j;k++) sum += l[k];      // if not, sum the lengths of the words... 
  return s_a[i][j] = LINE_LENGTH - j + i - sum;  // then compute the extra spaces based
  }                                              // on the line length
  
// define penalty function/table
inline int p(int i,int j) {
  if (p_a[i][j] != FLAG) 
    return p_a[i][j];       // if we've already computed this, return it.
  int x;
  x = s(i,j);
  if (x<0) 
    return p_a[i][j] = MCUBED; // if the line is too long, tag it
  if (j == NUM_WORDS && x >= 0) 
    return p_a[i][j]=0;     // return 0 if this is the last line
  return p_a[i][j]=x*x*x;   // return cube of extra spaces
  }

int main(int argc, char **argv) {
  int x, y;
  // generate random words
  cout << "Generating " << NUM_WORDS << " random words.\n";
  for (x=0;x<=NUM_WORDS;x++){
    l[x]= rand() % (MAX_WORD_LENGTH-1) + 2;
    cout << "Word " << x << " is " << l[x] << " letters long.\n";
    for (y=0;y<=NUM_WORDS;y++) {
      p_a[x][y]=FLAG;
      s_a[x][y]=FLAG;
      } 
    }
    
  // assign a random length from 2 to MAX_WORD_LENGTH to each word, and tag our
  // penalty table and extra length tables as unfilled.
  
  int i,j;
  int min, ptr; // min tracks minimum, ptr keeps value of i at min
  c[0]=0;
  for (j=1;j<NUM_WORDS;j++) { // calculate costs for each possibility
    min = RAND_MAX;
    for (i=j;i>0;i--) { 
      if (p(i,j) == MCUBED) break;  // no sense in going on if the line being tested is too long
      x = c[i-1] + p(i,j);
      if (x < min) { 
        min = x;
        ptr = i;      
        }
      }
    c[j] = min;
    w[j] = ptr;
    cout << "J=" << j << ": min= "<<min <<"@ i= "<<ptr<<endl;
    }
  
  // display solution (sort of)
  int n_lines=0;
  x = NUM_WORDS-1;
  while (w[x] > 0) { // count # lines
    n_lines++;
    x=w[x]-1;
    }  
  cout << "Of " << n_lines << " lines, line:" << endl;
  x = NUM_WORDS-1;
  y = 0;
  while (w[x] > 0) { // output values
    cout << n_lines - y << " has words " << w[x] << " to " << x << " with " << s(w[x],x) << " extra spaces." << endl;
    x = w[x]-1;
    y++;
    }  
  }
