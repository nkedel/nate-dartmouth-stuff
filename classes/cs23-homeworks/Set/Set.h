// CS23 Spring 1996 
// John M. Danskin (Mostly lifted from David Kotz).
// HW1 

// Declaration of an abstract data type for sets of ints.

class Set {
public:
  Set(void);			// construct an empty set
  Set(int x);			// construct a set with one element, x
  Set(const Set& s);		// construct a set, identical to another set s
  ~Set(void);

  Set& operator=(const Set& s); // assignment operator

  bool isElement(int x);	// is the given element in the set?
  void print(ostream& s);	// print this set to s, like "(2,3,6,7)"
				// An empty set looks like "()".
				// No endl is output.


  void Add(int x);		// add the element x to the set
  void Rm(int x);		// remove the element x from the set

  // Returns a set that represents the intersection of the two given sets.
  // This is a friend of class Set.
  friend Set Intersection(const Set& a, const Set& b);

  // for the future:
  //    friend Set Union(const Set& a, const Set& b);
  //    friend Set Difference(const Set& a, const Set& b);

private:

  // One implementation of a set is as an array of integers, in order

  int count;			// number of members
  int *members;			// [count]; except NULL for count==0
};
