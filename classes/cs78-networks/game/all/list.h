// list10.h

#ifndef LIST_H
#define LIST_H
#include "String.h"
#include "sockaddr.h"

class Item
{
public:
  Item();
  ~Item();

  bool playerok;
  SString nickname;
  SString pass;
  SString charfile;
  struct sockaddr_in sa;
  SString servername;
  SString curstring;
  int sock;

  int userlevel;
  bool cserver;

  bool misc;

    friend bool operator ==(Item, Item)
      {return true;}
};


class Node
{
public:

  Node();
  ~Node();

  Item *element;
  // holds the shape class instance

  Item * Element() {return element;}

  int misc;
  // tag used to locate the sentinel

  void Splice();
  // Assuming the aprev and anext pointers of a node are pointed
  // to 2 other nodes, splice will insert the current node between
  // them.

  void Slice();
  // Slices the current node out of the list that it is in.

  int Prior();
  // Determines the TKnumber of the first living node after it
  // in the list or -1 if none exist.

  Node *anext;
  // pointer to the next node in the linked list of nodes

  Node *aprev;
  // pointer to the previous node in the linked list of nodes
};



class List
{
 public:

  // Number of items active and alive
  int numitems;
  int curlist;

  // Sentinel in doubly linked list
  Node * sentinel;

  // Pointer for parsing
  Node * curpos;



  List ();
  ~List ();

  // Removes the item from the list
  void Delete ();

  // Is the list empty?
  bool Empty ();

  // Returns the current node
  Node * Current();

  // Returns the sentinel
  Node * Sent() {return sentinel;}

  void Start ();
  // Sets current pointer to the start of the list

  void Advance ();
  // Advances current pointer to the next node

  bool Isitem ();
  // Is current pointer a node on the list?

  Node * InsertFront (Item* elementToInsert, int slide = -2000000042);
  // Create a node containing given shape at beginning of list

  void Print ();
  // Prints out debug information

  void Prev();
  // Advances current pointer to the previous node

  // Insert item at the back of the list
  Node * InsertBack (Item* elementToInsert, int slide = -2000000042);

  // Insert item at the front of the list
  void Insert(Item elementToInsert);

  // Clear all nodes out of the list
  void MakeEmpty ();

  // Number of nodes alive or active
  int Nalive();

  bool Find(Item itemToFind);
  Item Retrieve(Item itemToFind);

};

#endif // LIST_H
