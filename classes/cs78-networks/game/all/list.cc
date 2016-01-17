
#include "list.h"
#include <iostream.h>
#include <assert.h>
#include <new.h>

Item::Item()
{
  playerok = false;
  sock = -1;       // Socket is invalid
  misc = false;    // Stopper is false
  userlevel = 3;   // Access level is 2
  cserver = false; // Is it using our client?
}

Item::~Item()
{
  // HAHA!
}

// Node constructor
Node::Node()
{
misc = -1;
}


// Node destructor
Node::~Node()
{
}

// Splices a node into a list
void Node::Splice()
{
  anext->aprev = aprev->anext = this; 
}


// Slices node out of A list
void Node::Slice()
{
  anext->aprev = aprev;
  aprev->anext = anext;
}


// Findes the tknumber of the next node in the list
int Node::Prior()
{
  Node *temp = this->anext;

#ifdef debug
  cout << "Commencing prior!" << endl;
#endif

  // Searches for the next node in the node or the sentinel
   while (temp->misc != -42)
      temp = temp->anext;

  if (temp->misc == -42)
    {
#ifdef debug
      cout << "There is no Prior" << endl;
#endif
      // Returns -1 if it's the sentinel
      return -1;
    }
  else
    {
#ifdef debug
      cout << "Prior is " << temp->element->GetTknum() << endl;
#endif
      // Return the tknumber if it's an acceptable node
      // fix me!
      return -1; //(temp->element->GetTknum());
    }
}


// Constructor

List::List ()
{
  // Create the sentinel
  if ((sentinel = new Node) == NULL)
    {
    cerr << "List::List: out of memory, exiting\n";
    exit(1);
    }
  sentinel->anext = sentinel->aprev = sentinel;

  // Set sentinel marker
  sentinel->misc = -42;
  numitems = 0;
}


// Destructor

List::~List ()
{
  MakeEmpty();
  delete sentinel;
}

// Is list empty (does sentinal point to itself)
bool
List::Empty ()
{
  return (sentinel -> anext == sentinel);
}

Node * List::Current()
{
  return (curpos -> anext);
}


// Go to the begining of the list (the sentinal)

void
List::Start ()
{
  curpos = sentinel;
}


// If there is a next item on the list, go to it

void
List::Advance ()
{
  if (Isitem())
    curpos = Current();
}


// If there is a previous item on the list, go to it
	
void
List::Prev ()
{
  curpos = curpos->aprev;
}


// Checks if the current item is valid

bool
List::Isitem ()
{
  assert(curpos != NULL);
  assert(curpos->anext != NULL);
  assert(curpos->aprev != NULL);
  return ((Current() != sentinel) && (Current() != NULL));
}

// Deletes current item

void
List::Delete ()
{
if (Isitem())
  // If the current item exists
  {
    Node *temp = Current();
    temp->Slice();
    // Slice out the item
    delete temp->element;
    delete temp;
    numitems--;
    // Delete it
  }
}


// Adds item to the front of the list

Node *
List::InsertFront (Item* newitem, int slide)
{
  Node *temp = new Node;     // Create a new node

  if (temp == NULL)	     // make sure memory was properly allocated
    {
      cerr << "List::Add: out of memory, exiting\n";
      exit(1);
    }

  Node *find = sentinel->anext;

  temp->element = newitem;
  // adds the shape
  temp->anext = find;
  // sets next node
  temp->aprev = find->aprev;
  // sets prev node to sentinel

  temp->Splice();

  assert(sentinel->anext != sentinel);
  numitems++;
  return temp;
}

Node *
List::InsertBack (Item* newitem, int slide)
{
  Node *temp = new Node;
  // Create a new node

  if (temp == NULL)
    // make sure memory was properly allocated
    {
      cerr << "List::Add: out of memory, exiting\n";
      exit(1);
    }

  Node * find = sentinel->aprev;

  temp->element = newitem;
  // adds the shape

  temp->anext = find->anext;
  // sets next node
  temp->aprev = find;
  // sets prev node to sentinel

  temp->Splice();
  // Splices in the node
  numitems++;
  return temp;
}
		
// Inserts item right after current

void
List::Insert (Item newitem)
{
  Node *temp = new Node;
  // Create a new node

  if (temp == NULL)
    // make sure memory was properly allocated
    {
      cerr << "List::Add: out of memory, exiting\n";
      exit(1);
    }

   Item * nitem;
   nitem = new Item;
//   nitem = newitem;

   temp->element = nitem;		 // adds the shape
   temp->anext = curpos->anext;	         // sets next node
   temp->aprev = curpos;		 // sets prev node to sentinel
   numitems++;
   temp->Splice();			 // Splices in the node
}

// Used for debugging
// Prints info on all shapes

/*
bool
List::Find(Item itemToFind)
{
  for (Start(); Isitem(); Advance())
	if (Current()->Element() == itemToFind)
		return true;
  return false;
}
*/

/*
Item
List::Retrieve(Item itemToFind)
{
  for (Start(); Isitem(); Advance())
	if (Current()->Element() == itemToFind)
		return Current()->element;
  return Current()->Element();
}
*/


void
List::Print ()
{
  Node *temp = curpos;
  // Store original position

  // Parse the list and display information about all nodes
  for (Start(); Isitem(); Advance())
    {
      cout << "------------------" << endl;
      cout << "STUFF... " << endl;
     // Traverse list displaying item information
    }
  curpos = temp;
  // Return to original position
}


// Deletes all items in a list
void
List::MakeEmpty ()
{
  // Go to the beginning of the list
  Start();
  // Sequencially delete EVERYTHING
  while (!Empty())
	Delete();
}

// Returns the number of nodes that are alive
int
List::Nalive()
{
  return numitems;
}
