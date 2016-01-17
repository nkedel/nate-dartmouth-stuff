// tester.cc
// Nate Edel
// CS 23 Spring 96
//
// Tests the functions in set.h in the following order:
//     (Constructor), isElement, Add, Print (Copy constructor),
//     (Copy/=), Rm, Intersection, (1 Element Constructor), (Destructor)
//
// Also tests the ability of algorithms to avoid crashing with nice big
// data sets.
//
// Inputs:  random numbers seeded from system time
// Outputs: diagnostic messages and set contents
//
// Known bugs: None.
// Failure conditions: 
//	Out of memory errors, Architecture dependent limitations 
//      (ie, Obscene > producing array of over 65536 bytes on an Intel
//      machine), User break

#include <iostream.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include "Set.h"

#define DEBUG

const int Ints = 25,                              // Nice round # and an average printout will fit on 1 line 
          Obscene = 50000,                        // An obscene number of integers
          Medium = 500;                            // A medium amount of integers
   
// Generate a random set of numInts integers in tSet and in the corresponding unsorted array       
void CPair(Set& tSet, int *intArray, int numInts);

// Prints a pattern of *s and -s 
void TPair(Set& tSet, int *intArray, int numInts);

// Test our Set class 
int main() {
  int i;                                        // reusable counter variable  
  int anArray[Ints];                            // create an array of Ints integers
  Set Test1;                                    // create an empty set

  int tv;
  time(&tv);                                     // gets the timer value in seconds to...
  srand((unsigned) tv);                          // seed the random number generator

  CPair(Test1, anArray, Ints);                  // call CPair to create a set and a corresponding array
  TPair(Test1, anArray, Ints );                 // and call TPair to test that all elements in the array are in the set
  cout  << "You should see a row of *s just under the .s" << endl;

  cout << "Now we test the copy constructor by trying to create a copy of Test1" << endl;
  cout << "First, print a copy of the old set..." << endl;
  Test1.print(cout);
  Set Test2 = Test1;                            // create a copy of Test1
  cout << "Now, print a copy of the new set..." << endl;
  Test2.print(cout);
  
  cout << "And we test the new set against our array... (*s means it worked)" << endl;
  TPair(Test2, anArray, Ints);                  // Test the validity of the copy
  
  cout << "Now, let's try out the = operator by copying the new over the old, & vice versa" << endl;
  Test1 = Test2;
  Test2 = Test1;
  
  cout << "And print the hopefully still identical sets..." << endl;
  Test1.print(cout);
  Test2.print(cout);
  cout << endl;
  
  Set Test3;                                    // create a NEW empty set
  int moreInts[Ints];                            // and a NEW empty array
  cout << "We create a new random set of 25 and print it..." << endl;
  CPair(Test3, moreInts, Ints);
  Test3.print(cout);
  cout << endl; 
  
  cout << "And test it agains its own array, which should match..." << endl;
  TPair(Test3, moreInts, Ints);
  cout << endl;
  
  cout << "And against the old array, which should produce a random-like" << endl;
  cout << "pattern of *s and -s." << endl; 
  TPair(Test3, anArray, Ints);
  
  cout << "And for good measure, we'll test the old sets against the new array" << endl;
  TPair(Test1, moreInts, Ints);
  TPair(Test2, moreInts, Ints);
  cout << endl;
  
  cout << "And now we'll try removing every even numbered element" << endl;
  for (i = 0; i < Ints; i+=2)                 // remove every other element
    Test2.Rm(anArray[i % Ints]);              // making sure we don't run over the total # of ints
  TPair(Test2, anArray, Ints);
  cout << endl;

  cout << "And now we'll try removing the first half of the set" << endl;
  Test2 = Test1;                                  // reset Test1 to our original set
  for (i = 0; i < (Ints / 2); i++)                 // remove the first half of the set
    Test2.Rm(anArray[i]);          
  TPair(Test2, anArray, Ints);
  cout << endl;

  cout << "And now we try the intersection of the old set and the new sets" << endl;
  Set Test4; 
  Test4 = Intersection(Test1, Test3);
  Test4.print(cout);
  TPair(Test4, anArray, Ints);
  TPair(Test4, moreInts, Ints);
  cout << endl;
  
  cout << "And the intersection of THAT set with the first set, which should produce itself" << endl;
  Test2 = Intersection(Test1, Test4);
  Test2.print(cout);
  Test4.print(cout);
  cout << endl;
  
  cout << "And finally, try intersecting the first set with itself" << endl;
  Test2 = Intersection(Test1, Test1);
  cout << "Which should produce itself..." << endl;
  Test1.print(cout);
  Test2.print(cout);
  cout << endl;
  
  // Test a few remaining features, mostly to see if they crash
  Set *Test5, *Test6;                            // create a blank pointer to a Set
  Test5 = new Set(5);                            // create single-item sets to test 'em                               
  assert(Test5 != NULL);
  cout << "Should be 5: ";
  Test5->print(cout); 
  Test6 = new Set(6);                          
  assert(Test6 != NULL);
  cout << "Should be 6: "; 
  Test6->print(cout);
  
  cout << "Viewable tests done... please wait while I test a huge set." << endl;  
  
  for (i = 0; i < Obscene ; i++)                // Generate an obscene amount of numbers...
    Test5->Add(rand() % 1000);                  // and add them to Test5 
   for (i = 0; i < Medium ; i++)                  // and generate a more moderate number for
     Test6->Add(rand() % 1000);                  // Test6
  Test2 = Intersection(*Test5, *Test6);          // and produce their intersection
  
  delete Test5;
  delete Test6;
   
  cout << "If you see this, the really big set has not crashed the program." << endl;
  cout << "Automated testing is completed." << endl;
  cout << "If you've missed text, run \"tester | more\" or redirect output to a file" << endl;

  return(0);
}  

void CPair(Set& tSet, int *intArray, int numInts) {
// create a set of numInts UNIQUE random integers in the range 0-99
// also fill the provided array so the user can test this...
  int i;
  cout << "Adding " << numInts;                 // Tell viewer how many ints we're going to see 
  cout << " Random Integers to set."; 
  cout << endl;
  for (i = 0; i < numInts ; i++) {              // Generate the numbers...
    int myInt;
    bool element = TRUE;
    while (element) {                           // Wait for a UNIQUE number, so the size of our set
      myInt = (rand() % 100);                   // is known...
      element = tSet.isElement(myInt); 
      }         
    intArray[i]=myInt;                          // once we have it, save it in the array and the set
    tSet.Add(intArray[i]);
    cout << ".";                                // and output a period so we know it's running
    }
  cout << endl;
  }

void TPair(Set& tSet, int *intArray, int numInts) {
  for (int i = 0; i < numInts ; i++) {                // scan through the array
    if (tSet.isElement(intArray[i]))            // making sure all the numbers are also in the set
      cout << "*";                              // and output *s if they're there
    else 
      cout << "-";                              // and dashes if they aren't
    }
  cout << endl;
  }
