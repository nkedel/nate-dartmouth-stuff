/* list code developed in cs 68 by Ron Boskovic for a group homework,
modified by Peter Horowitz for cs 49 homework further modified by Peter
Horowitz and Nate Edel for Cs78 */

#ifndef LIST_H
#define LIST_H

class node {
public:
  node(int socketNumber, const char* name, const char* dnsName) {
      sockNumber = socketNumber; 
      strcpy(userName,name); 
      strcpy(systemName, dnsName);
      next=NULL;
      }
      
  node() {next= NULL;}
  int seeMyOwn; 
  int sockNumber;  
  int sysop;
  node* next;     
  char userName[24]; 
  char systemName[48];
};


class list {
public:
list() { 
      head = NULL; 
      bookmark = NULL;
      current = NULL; 
      numberEntries = 0; 
      }

void dump() {// output for class in case needed for error checking
  node *oldCurrent;
  oldCurrent=current;
  current=head;
  if (current==NULL) {
    printf("Empty list.\n");
    current=oldCurrent;
    return;
    }
  while (current != NULL) {
    printf("* %d, %s, %s\n",current->sockNumber,current->userName,current->systemName);
    current = current->next;
    }
  current=oldCurrent;
  }

void insert(int sockNum, char *userName, char *dns) {
    // increment number of entries. insert new node at the head of the list
    numberEntries++;
    node *tNode;
    tNode=head;
    head = new node(sockNum, userName, dns);
    head->next = tNode;
    if (current==NULL) current=head;
    }


bool last() { 
    // determine if current is at the end of the list
    if (head == NULL) return TRUE;
    assert(current != NULL);
    if (current->next == NULL) return TRUE;
    return FALSE; 
    }
  
void GoTolast() {
    // goto the last element in the list
    assert(current != NULL);
    while(current->next != NULL) current=current->next;
    }

void rewind() {
  // go to the head of the list
  if (head!= NULL) current= head; }

int next() {
    // goto the next element in the list
    if (current == NULL) return -1;
    current = current->next;
    return 0;
    }
    
void remove() { 
    // remove the node from the list, decrease number of entries
    if (current== NULL) return; 
    node *toDelete;
    numberEntries--;
    if (current == head) {// delete the head of the list
      head = current->next;
      delete current;
      current = head;
      return;
      }
    // we are in the middle of the list therefore we need to find the predescessor node
    toDelete=current;
    current=head;
    while (current->next != toDelete)
      current = current-> next;
    assert (toDelete == current->next);
    // after cutting out the node make sure its predessor points to the right node
    current->next = toDelete->next;
    delete toDelete;
    }

int findSocket(int sockToFind) {
  // find the correct node by comparing socket numbers
  current = head;
  if (current == NULL) return -1;
  while (current->sockNumber != sockToFind) {
    if (current->next == NULL) return 0;
    current = current->next;
    }
  return current->sockNumber;
  }

bool empty() { 
  // check to see if the list is empty
  if (head == NULL) return TRUE; 
  assert(current != NULL);
  return FALSE; 
  } 

int getSocket() { return current->sockNumber; } 
char* getName() { return current->userName; }
void setName(const char *newname)  { strcpy(current->userName, newname); }
char* getSystem() { return current->systemName; }
int entries()   { 
    return numberEntries; 
    }

void setBookmark() { bookmark=current; }
void getBookmark() { if (bookmark!=NULL) current = bookmark; }

private:
  node* head;
  node* current;    // position pointer for list operations
  node* bookmark;
  int numberEntries;
};

#endif
