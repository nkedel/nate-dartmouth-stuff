// TreeNode.h
// Declaration of TreeNode class

enum TreeNodeType {
  OP_ASSN, OP_ADD, OP_SUB, OP_MULT, OP_NEG, NUM_NODE, ID_NODE };

class TreeNode;
typedef TreeNode *TreeNodePtr;

class TreeNode {
public:
  TreeNode(TreeNodeType t);	// constructor
  void AddLeft(TreeNodePtr x);	// make x the left child
  void AddRight(TreeNodePtr x);	// make x the right child
  void AddSibling(TreeNodePtr x); // make x the sibling
  TreeNodePtr GetSibling();	// return the sibling
  void Print(int level);	// print the TreeNode

private:
  TreeNodeType type;		// type of TreeNode
  TreeNodePtr sibling;		// right sibling
  union {
    char *idName;		// if an ID_NODE, the id's name
    int value;			// if a NUM_NODE, the number's integer value
    struct {
      TreeNodePtr left, right;	// if an OP node, left and right children
    } children;
  };
};
