// TreeNode.h
// Declaration of TreeNode class

enum TreeNodeType {
  OP_ASSN, OP_ADD, OP_SUB, OP_MULT, OP_NEG, NUM_NODE, ID_NODE,
  IF_STMT, IF_ELSE_STMT };

class TreeNode;
typedef TreeNode *TreeNodePtr;

const int MAXTREENODEKIDS = 3;

class TreeNode {
public:
  TreeNode(TreeNodeType t);	// constructor
  void AddChild(TreeNodePtr x, int j); // make x the jth child
  void AddSibling(TreeNodePtr x); // make x the sibling
  TreeNodePtr GetSibling();	// return the sibling
  void Print(int level);	// print the TreeNode

private:
  TreeNodeType type;		// type of TreeNode
  TreeNodePtr sibling;		// right sibling
  union {
    char *idName;		// if an ID_NODE, the id's name
    int value;			// if a NUM_NODE, the number's integer value
    TreeNodePtr child[MAXTREENODEKIDS];
				// if an OP node, its children
  };
};
