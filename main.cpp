#include <iostream>
#include <string>
#include <vector>
#include <assert.h>

enum class Color{red=0, black=1, dblack=2};

#define isred(node) (node->color==Color::red)
#define isblack(node) (node->color==Color::black)
#define isdblack(node) (node->color==Color::dblack)
#define isleaf(node) (!node->left&&!node->right)
#define isfull(node) (node->left && node->right)


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

    NodePtr_t rotate_right(NodePtr_t lhs, NodePtr_t rhs) {
        NodePtr_t rhs_f = rhs->father;
        NodePtr_t tmp_r = lhs->right;
        rhs->left = tmp_r;
        lhs->right = rhs;
        if(rhs_f) {
            if(rhs==rhs_f->left)  rhs_f->left = lhs;
            else rhs_f->right = lhs;
        }
        else
            this->root = lhs;
        return lhs;
    }

    NodePtr_t rotate_left(NodePtr_t lhs, NodePtr_t rhs) {
        NodePtr_t lhs_f = lhs->father;
        NodePtr_t tmp_l = rhs->left;
        lhs->right = tmp_l;
        rhs->left = lhs;
        if(lhs_f) {
            if(lhs==lhs_f->left)  lhs_f->left = rhs;
            else lhs_f->right = rhs;
        }else
            this->root = rhs;
        return rhs;
    }

    void ascend_fatherIsRed(NodePtr_t inserted_node, NodePtr_t father) {
      // father node is red
      NodePtr_t gf = father->father; // grand father must be black
      NodePtr_t uncle = (father==gf->left)?gf->right:gf->left;
      // uncle node is red
      if(uncle && isred(uncle)) {
          father->color = Color::black;
          uncle->color = Color::black;
          gf->color = Color::red;
          if(gf==this->root) { gf->color = Color::black; return;}
          else if(isblack(gf->father)) return;
          else ascend_fatherIsRed(gf, gf->father);
      }
      // uncle node is black
      else{
          // same direction
          if(inserted_node == father->left && father == gf->left) {
              father->color = Color::black;
              gf->color = Color::red;
              rotate_right(father, gf);
          }
          else if(inserted_node==father->right && father==gf->right) {
              father->color = Color::black;
              gf->color = Color::red;
              rotate_left(gf, father);
          }
          // not same direction
          else if(inserted_node==father->right && father==gf->left) {
              auto p = rotate_left(father, inserted_node);
              p->color = Color::black;
              gf->color = Color::red;
              rotate_right(p, gf);
          }
          else {
              auto p = rotate_right(inserted_node, father);
              p->color = Color::black;
              gf->color = Color::red;
              rotate_left(gf, p);
          }
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

    NodePtr_t findNodeToRemove(int value) {
        NodePtr_t p = this->root;
        while(p) {
            int cur_value = p->value;
            if(value==cur_value)
                return p;
            else if(value<cur_value)  p=p->left;
            else p=p->right;
        }
        return NULL;
    }

    void black_black(NodePtr_t father, NodePtr_t brother) {
        if(brother==father->right) {
            if(brother->right && isred(brother->right)) {
                Color tmp = father->color;
                father->color = Color::black;
                brother->color = tmp;
                rotate_left(father, brother);
                brother->right->color = Color::black;
            }
            else if(brother->left && isred(brother->left)) {
                brother->left->color = Color::black;
                brother->color = Color::red;
                auto p = rotate_right(brother->left, brother);
                Color tmp = father->color;
                father->color = Color::black;
                p->color = tmp;
                rotate_left(father, p);
                p->right->color = Color::black;
            }
            else if(isred(father)) {
                father->color = Color::black;
                brother->color = Color::red;
            }
            else {
                
            }
        }else {

        }
    }

    void remove_atleastonenull(NodePtr_t node_to_remove) {
        // red leaf node
        if(isred(node_to_remove) && isleaf(node_to_remove)) {
            auto father = node_to_remove->father;
            if(node_to_remove==father->left)
                father->left = NULL;
            else
                father->right = NULL;
        }
        // black leaf node
        else if(isblack(node_to_remove) && isleaf(node_to_remove)) {
            if(node_to_remove==this->root) {this->root=NULL;return;}
            assert(node_to_remove->father!=NULL);
            NodePtr_t father = node_to_remove->father;
            NodePtr_t brother = NULL;
            if(node_to_remove==father->left) {
                father->left = NULL;
                brother = father->right;
            }else {
                father->right = NULL;
                brother = father->left;
            }
            assert(brother!=NULL);
            if(brother->color==Color::black) {
                black_black(father, brother);
            }else if(brother==father->right) {
                brother->color = Color::black;
                father->color = Color::red;
                rotate_left(father, brother);
                black_black(father, father->right);
            }
            else{
                brother->color = Color::black;
                father->color = Color::red;
                rotate_right(brother, father);
                black_black(father, father->left);
            }
        }
        // black node with one red leaf node
        else {
            assert(node_to_remove->color==Color::black);
            NodePtr_t child = (node_to_remove->left)?node_to_remove->left:node_to_remove->right;
            assert(isred(child));
            node_to_remove->value = child->value;
            if(child==node_to_remove->left)  node_to_remove->left=NULL;
            else  node_to_remove->right=NULL;
        }
    }

    NodePtr_t findSuccessor(NodePtr_t node) {
        assert(node->right!=NULL);
        NodePtr_t p = node->right;
        while(p->left) 
            p = p->left;
        return p;
    }

    bool remove(int value) {
        NodePtr_t node_to_remove = findNodeToRemove(value);
        if(!node_to_remove)
            return false;
        if(!isfull(node_to_remove)) {
            remove_atleastonenull(node_to_remove);
            return true;
        }
        else {
            // find successor node first
            NodePtr_t successor_node = findSuccessor(node_to_remove);
            node_to_remove->value = successor_node->value;
            successor_node->color = node_to_remove->color;
            remove_atleastonenull(successor_node); // successor's left is NULL
        }
    }

    void inorder_traverse(NodePtr_t root) {
        if(root) {
            inorder_traverse(root->left);
            std::cout << root->value << " ";
            inorder_traverse(root->right);
        }
    }

    void print() {
        inorder_traverse(this->root);
        std::cout<<"\n";
    }
};

int main() {
    RBTree tree(3);
    tree.insert(1);
    tree.insert(4);
    tree.insert(0);
    tree.insert(2);
    tree.print();
    return 0;
}
