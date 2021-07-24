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

    void ascend_fatherIsRed(NodePtr_t inserted_node, NodePtr_t father) {
      // father node is red
      NodePtr_t gf = father->father; // grand father must be black
      NodePtr_t uncle = (father==gf->left)?gf->right:gf->left;
      // uncle node is black
      if(isblack(uncle)) {
          // same direction
          if(inserted_node == father->left && father == father->father->left) {
          }
          else if(inserted_node==father->right && father==father->father->right) {
          }
          // not same direction
      }
      // uncle node is red
      else {
          father->color = Color::black;
          uncle->color = Color::black;
          gf->color = Color::red;
          if(gf==this->root) { gf->color = Color::black; return;}
          else if(isblack(gf->father)) return;
          else ascend_fatherIsRed(gf, gf->father);
      }
    
    }

    bool _insert(NodePtr_t root, int value) {
      auto [father, pptr] = findInsertion(root, value);
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
      else {
          ascend_fatherIsRed(new_node, father);
          return true;
      }
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
