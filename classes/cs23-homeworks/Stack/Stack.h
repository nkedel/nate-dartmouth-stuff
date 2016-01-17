//
// Stack.h --- Integer Stack interface and Implementations
//
// I have supplied the interfaces. You are supposed to supply the
// implementations for IntStackA and IntStackB.
//
// John M. Danskin, CS23,  4/8/96
//

//
// Modifications:
//		Added private members for the classes, and a stackNode
//		structure for use by the list based implementation
//		-- Nathan K. Edel. 4/13/96
//

#ifndef STACK_H
#define STACK_H

//
// This is our node structure for the list based implementation
// (NKE (4/13)
typedef struct tStackNode {
   struct tStackNode *prevNode;
   int value;
	} stackNode;

//
// This is the abstract class for IntStack, a stack of integers.
//
class IntStack {
public:
  virtual ~IntStack() {}
  virtual void Push(int x)=0;
  virtual int Pop()=0;
};

//
// This is the array based stack implementation.
// It uses an array with a maximum size.
//
class IntStackA : public IntStack {
public:
  IntStackA(int maxSZ);
  ~IntStackA();
  virtual void Push(int x);
  virtual int Pop();
private:
  int *stackArray;
  int stackSize, stackMax;
};

//
// This is the list based stack implementation
//     it uses a list with no maximum size.
//
class IntStackB : public IntStack {
public:
  IntStackB();
  ~IntStackB();
  virtual void Push(int x);
  virtual int Pop();
private:
	stackNode *stackTop;
};

#endif
