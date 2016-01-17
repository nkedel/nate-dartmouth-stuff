// TreeNode.h
// Declaration of TreeNode class
// originally provided by Prof. Cormen,
// modified N. Edel

class TreeNode;
struct sListNode;
class List;

typedef TreeNode *pTreeNode;
typedef struct sListNode *pNode;
typedef List *pList;

extern char *yytext;

typedef struct sListNode {
  pNode next; 
  pTreeNode child;
  } ListNode;

class List {
public:
  List();
  ~List();
  void append(pTreeNode x);        // append child to end of list;
  pTreeNode get(int n);            // get child -- returns null if no child n 
  int len(void);                   // get # of children;
private:
  pNode head;
  };

class TreeNode {
public:
  TreeNode(int type, char *s = "");   // constructor
  void addChild(pTreeNode x);                // make x the next child
  void addSib(pTreeNode x);              // make x the sibling
  pTreeNode getSib();                        // return the sibling
  void Print(int level);                     // print the TreeNode

private:
  int type;            // type of TreeNode
  pTreeNode sib;          // right sibling
  char *val; 
  pList kids;
  };

