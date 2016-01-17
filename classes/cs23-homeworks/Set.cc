// Set.cc
// Nate Edel
// CS 23 Spring 96

// Inplementation of the class Set as defined in Set.h
// This implementation of class Set implements an ADT for sets of ints 
// using a sorted, dynamically allocated array of int.

// This set will contain unique elements only; attempts to add a duplicate 
// element will be ignored.

#include <iostream.h>
#include <stdlib.h>
#include "Set.h"
#include <assert.h>

#define DEBUG

// Constructs an empty set.
Set::Set(void) {
  count = 0;
  members = NULL;
}

// Constructs a new set with a single integer as a member
Set::Set(int x) {
  count = 1;
  members = new int[1];
  assert(members != NULL);          // make sure we actually got a pointer
  members[0] = x;
}

// Constructs a new set as an exact copy of set s
Set::Set(const Set& s) {
  count = s.count;
  members = new int[count];
  assert(members != NULL);          // make sure we actually got a pointer
  for (int i=0; i<count; i++)       // copy all members of s
    members[i] = s.members[i];      // ...one at a time
}

// Destructor: simpy deallocates the dynamically allocated array, if any
Set::~Set(void) {
  if (members) delete [] members;
  }

// Copy operator: overwrite target set with an exact copy of set s
Set& Set::operator= (const Set& s) {
  if (members) delete [] members;  // don't delete a nonexistant array
  count = s.count;
  members = new int[count];        // create enough space for the other set
  assert(members != NULL);         // make sure we actually got a pointer
  for (int i=0; i < count; i++)    // copy all members of s one at a time
    members[i] = s.members[i];          
  return *this;
}

// Determine if int x is a member of the set using a linear search.
bool Set::isElement(int x) {
  if (count == 0) return FALSE;    // if it's an empty set, it can't have members
  else 
    for (int i=0; i < count; i++)   // otherwise, do a linear search
      if (members[i] == x) return TRUE;  // and stop if we find something
  return FALSE;
  }

// dumps a minimally formatted copy of the set to the standard output
void Set::print(ostream& s) {
  s << "( ";           
  if (count) {                        // don't loop through an empty set 	
    for (int i = 0; i < count-1; i++) // run through the numbers...
      s << members[i] << ", ";        // ...printing them one by one
    s << members[count-1] << " ";
  }
  s << ")" << endl;
}

// Add an element into its sorted order in the set, via an insertion sort.
// This will ignore request to add multiple identical elements in the set
void Set::Add(int x) {
  int *oldArray,                         // create some pointers for our arrays
      *newArray,                         
      i = 0;                             // a counter
  if (isElement(x)) return;		 // if the element exists, quit
  newArray = new int[count+1];           // create a new Array to hold 1 more than the current number of elements
  assert(newArray != NULL);              // make sure we actually got a pointer
  oldArray = members;                    // point the old array at the current members
  while (i < count) {                    // scan through for a successor (or equal) to our element
    if (oldArray[i] >= x) break;         // stop scanning here if we found something
    newArray[i] = oldArray[i]; i++;      // ...and and if not, copy the element which we scanned 
  }
  newArray[i] = x;                       // assign our element to insert to our current point
  while (i < count) {                    // *IF* we have elements left, copy them over
    newArray[i+1] = oldArray[i]; i++;
  }
  count++;                               // increment count to reflect our insertion
  members = newArray;                    // point members to our new array
  delete [] oldArray;                    // and get rid of our old array
}

// Remove an element from the set if it is a member; otherwise do nothing
void Set::Rm(int x) {
  int *oldArray,                         // create pointers for our temporary arrays
      *newArray,                         
      i = 0;
  if (isElement(x)) {                    // if it's NOT an element, forget it
    oldArray = members;                  // point oldArray to our old members array
    newArray = new int[count-1];         // create a new array of the right size for our reduced array
    assert(newArray != NULL);            // make sure we actually got a pointer
    while (i < count) {                  // scan through our array until we find our item to erase
      if (oldArray[i] == x) break;       // ...stop scanning when we find it
      newArray[i] = oldArray[i]; i++;    // ...otherwise, make copy the element
    } 
    i++;                                 // advance past the element to delete
    while (i < count) {                  // if there are any elements after it, scan through
      newArray[i-1] = oldArray[i]; i++;  // and copy them...
    }
    members = newArray;                  // assign the new array to be our list of members
    delete [] oldArray;                  // and delete the old list of members
    count--;                             // and decrement the count value to be accurate
  }
}

// Intersection creates a new set and returns a new set containing all elements which are contained in both
// input sets.
Set Intersection(const Set& a, const Set& b) {
  Set returnSet;                         // create an empty set to return
  int last = 0,                          // create an integer to hold the number we're checking
      i = 0;
  if ((a.count == 0) || (b.count == 0))  // can't intersect an empty set
    return returnSet;
  while (i < a.count) {
    last = a.members[i];                 // get our item
    for (int j=0; j < b.count; j++) {    // do a linear search for its pal
      if (b.members[j] == last) {
        returnSet.Add(last);             // if our item is also in b, copy it
        break;                           // and stop if we find something
        }
    }                             
    i++;                                                    // move to the next item
    while ( (i < a.count) && (last == a.members[i]) ) i++;  // skip any duplicates
  }
  return returnSet;
}
