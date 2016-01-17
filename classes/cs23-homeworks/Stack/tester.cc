#include <stdlib.h>
#include <iostream.h>
#include <assert.h>
#include "Stack.h"

//
// Test an IntStack implementation with a # of known members.
// Works better with a smaller number of #'s.
//
void TestA(IntStack *toTest, int howMany);

//
// Test an IntStack implementation with a # of Random members.
// Works better with a larger # of #'s.
//
void TestB(IntStack *toTest, int howMany);

int main(void) {
IntStackA *StackA;
  IntStackB *StackB;
  StackA = new IntStackA(10);
  StackB = new IntStackB;
  cout << "Test Small Array-based Stack" << endl;
  TestA(StackA, 10);
  cout << "Test Small List-based Stack" << endl;
  TestA(StackB, 10);
  delete StackA;
  delete StackB;
  StackA = new IntStackA(20000);
  StackB = new IntStackB;
  cout << "Test Large Array-based Stack" << endl;
  TestB(StackA, 20000);
  cout << "Test Large List-based Stack" << endl;
  TestB(StackB, 20000);
  cout << "Testing underflow on array based stack. (You should see an error message and a 0)" << endl;
  cout << StackA->Pop() << endl;
  cout << "Testing underflow on list based stack. (You should see an error message and a 0)" << endl;
  cout << StackB->Pop() << endl;
  delete StackA;
  delete StackB;
  StackA = new IntStackA(2);
  cout << "Testing overflow on list based stack. (You should see an error message)" << endl;
  StackA->Push(5);
  StackA->Push(10);
  StackA->Push(15);
  cout << "Checking remains after overflow (you should see 10, then 5)" << endl;
  cout << StackA->Pop() << endl;
  cout << StackA->Pop() << endl;
  delete StackA;
  cout << "Test complete." << endl;
  cout << "Note: if this test goes off the screen, please pipe to more using 'tester |& more'" << endl;
  cout << "or error messages, which are displayed to STDERR will appear in incorrect order." << endl;
  return(0);
  }

//
// Test an IntStack implementation with a # of known members.
// Works better with a smaller number of #'s.
//
void TestA(IntStack *toTest, int howMany){
  int i, j;
  cout << "Pushing " << howMany << " #'s onto the stack in order." << endl;
  for ( i = 0; i < howMany; i++ ) {
    toTest->Push(i);
    cout << i << ", ";
    }
  cout << "Done." << endl << endl;
  cout << "Popping elements off the stack..." << endl;
  cout << "Should be in reverse order." << endl;
  for ( i = (howMany-1); i >= 0; i--) {
    j = toTest->Pop();
    if (j!=i) cout << "(ERROR) ";
    cout << j << ", ";
    }
  cout << "Done." << endl << endl;
  }

//
// Test an IntStack implementation with a # of Random members.
// Works better with a larger # of #'s.
//
void TestB(IntStack *toTest, int howMany){
  int i, j, *testValues;
  testValues = new int[howMany];
  if (testValues == NULL) {
    cout << "** ERROR: Couldn't allocate memory for test values!" << endl;
    return;
    }
  cout << "Pushing " << howMany << " random #'s onto the stack." << endl;
  for ( i = 0; i < howMany; i++ ) {
    j = rand();
    testValues[i] = j;
    toTest->Push(j);
    }
  cout << "Done." << endl << endl;
  cout << "Popping elements off the stack..." << endl;
  cout << "Should be in reverse order." << endl;
  for ( i = (howMany-1); i >= 0; i--) {
    j = toTest->Pop();
    if (j!=testValues[i]) break;
    }
  if (i > 0) cout << "Failed." << endl << endl;
  else cout << "Success." << endl << endl;
  }

