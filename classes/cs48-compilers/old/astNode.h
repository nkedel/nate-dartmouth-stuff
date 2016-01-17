// ast.h
// Declaration of abstract syntax tree class 
// based on "astNode" class originally provided by Prof. Cormen,
// modified N. Edel

class ast;

typedef astNode *pAstNode;
typedef struct sListNode *pNode;

extern char *yytext;

typedef struct sListNode {
  pNode next; 
  pAstNode child;
  } ListNode;

class List {
public:
  List();
  ~List();
  void append(pAstNode x);        // append child to end of list;
  pAstNode get(int n);            // get child -- returns null if no child n 
  int len(void);                   // get # of children;
private:
  pNode head;
  };

class astNode {
public:
  astNode(int type, char *s = "");   // constructor
  void addChild(pAstNode x);                // make x the next child
  void addSib(pAstNode x);              // make x the sibling
  pAstNode getSib();                        // return the sibling
  void Print(int level);                     // print the astNode

private:
  const int max_n_kids = 5;
  int type;              // type of astNode
  int n_kids;            // number of Child Nodes
  pAstNode sib;          // right sibling
  char *val;              
  int nval;
  double fval;
  pAstNode kid[5];
  };

