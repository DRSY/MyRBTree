#include <iostream>
#include <string>
#include <vector>
#include <assert.h>

enum class Color{red=0, black=1, dblack=2};

#define isred(node) (node->color==Color::red)
#define isblack(node) (node->color==Color::black)
#define isdblack(node) (node->color==Color::dblack)

typedef struct Node{
  int value;
  Color color; // 0 for red, 1 for black, 2 for double black
  struct Node* father;
  struct Node* left;
  struct Node* right;

  Node() { value=0; color=Color::red; father=NULL;left=NULL; right=NULL;}
  Node(int _value) { value=_value; color=Color::red; father=NULL; left=NULL; right=NULL;}
}Node_t, *NodePtr_t;

class RBTree {
  private:
    NodePtr_t root;
  public:
    RBTree(): root(new Node_t()) { root->color = Color::black; }
    RBTree(int value): root(new Node_t(value)) { root->color = Color::black; }

    bool _search(NodePtr_t root, int value) {
      if(!root)
        return false;
      if(root->value == value)
        return true;
      if(root->value > value)
        return _search(root->left, value);
      else return _search(root->right, value);
    }

    bool search(int value) {
      return _search(this->root, value);
    }

    std::pair<NodePtr_t, NodePtr_t*> findInsertion(NodePtr_t root, int value) {
      if(root->value == value)
        return std::pair<NodePtr_t, NodePtr_t*>(NULL, NULL);
      if(value < root->value && !root->left)
        return std::pair<NodePtr_t, NodePtr_t*>(root, &root->left);
      else if(value < root->value && root->left)
        return findInsertion(root->left, value);
      else if(value > root->value && root->right)
        return findInsertion(root->right, value);
      else
        return std::pair<NodePtr_t, NodePtr_t*>(root, &root->right);
    }

    void ascend_checking(NodePtr_t node, NodePtr_t father, NodePtr_t uncle, NodePtr_t gf) {
      
    }

    bool _insert(NodePtr_t root, int value) {
      auto [father, pptr] = findInsertion(this->root, value);
      if(!father)
        return false;
      NodePtr_t new_node = new Node_t(value);
      new_node->color = Color::red;
      new_node->father = father;
      (*pptr) = new_node;
      // father node is black
      if(isblack(father)) {
        return true;
      }
      // father node is red
      NodePtr_t gf = father->father; // grand father must be black
      NodePtr_t uncle = (father==gf->left)?gf->right:gf->left;
      // uncle node is also red
      if(isred(uncle)) {
        ascend_checking(new_node, father, uncle, gf);
      }
      // uncle node is black
      else {
      }
      return false;
    }

    bool insert(int value) {
      if(!root) {
        root = new Node_t(value);
        root->color = Color::black;
        return true;
      }
      return _insert(this->root, value);
    }
};


int main() {
  RBTree tree(3);
  return 0;
}
