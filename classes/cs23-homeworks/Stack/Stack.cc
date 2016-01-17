//
// Stack.cc --- Integer Stack Implementations
//

#include <iostream.h>
#include  <assert.h>
#include "Stack.h"

// Creates a new instance of the Array based stack with max size maxSZ
// maxSZ must be greater than 0, or create will fail.
// will fail (assert) if unable to allocate the array.
IntStackA::IntStackA(int maxSZ) {
   assert(maxSZ > 0);
	stackSize = 0;
   stackMax = maxSZ;
	stackArray = new int[maxSZ];
   assert(stackArray != NULL);
	};

// Destructor.
IntStackA::~IntStackA() {
   delete [] stackArray;
	}

// Pushes element x onto the stack, unless stack is full, in which case
// we output an error and do nothing.
void IntStackA::Push(int x) {
   if (stackSize < stackMax) {
		stackArray[stackSize] = x;
      stackSize++;
   	}
   else cerr << "** Error: Push to Full Stack!" << endl;
	};

// Pops top element off the stack and return it, unless stack is empty,
// in which case we output an error and return 0.
int IntStackA::Pop() {
   int retVal = 0;
   if (stackSize) {
      stackSize--;
      retVal = stackArray[stackSize];
   	}
   else cerr << "** Error: Pop from Empty Stack!" << endl;
   return retVal;
	}


// Creates an empty instance of a list-based stack.
IntStackB::IntStackB() {
   stackTop = NULL;
	}

// Destructor: deletes list of stackNodes
IntStackB::~IntStackB() {
   stackNode *doomedNode;
   while (stackTop != NULL) {
      doomedNode = stackTop;
      stackTop = stackTop -> prevNode;
      delete doomedNode;
   	}
	}

// Push value x onto the stack by creating a new node
// and linking that to the stack top
// Will output an error and do nothing if a new node cannot be created.
void IntStackB::Push(int x) {
   stackNode *newNode = new stackNode;
   if (newNode != NULL) {
      newNode->prevNode = stackTop;
      stackTop = newNode;
      stackTop->value = x;
   	}
   else cerr << "** Error: Node Create Failed; Couldn't Push." << endl;
	}

// Pop the top value off the stack and return its value, unless
// the stack is empty, in which case outputs an error and does nothing.
int IntStackB::Pop() {
   stackNode *myNode;
   int retVal = 0;
   if (stackTop != NULL) {
      myNode = stackTop;
      stackTop = myNode->prevNode;
      retVal = myNode->value;
      delete myNode;
   	}
   else cerr << "** Error: Pop from Empty Stack!" << endl;
   return retVal;
	}

