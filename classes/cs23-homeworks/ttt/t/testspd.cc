#include <time.h>
#include <unistd.h>
#include <iostream.h>

int main(void) {
   char *c;
   c = new char [1000];
   int x, y;
   time(&x);
   for ( int i = 0; i < 20000000 ; i++)  
      if (c[i % 1000] == 0) c[i%1000]++;
   time(&y);
   cout << "Start: " << x << " End: " << y << endl;
   cout << "Took :" << (y - x) << "seconds." << endl;
   }
