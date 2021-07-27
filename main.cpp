#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include <queue>
#include <string>

enum class Color{red=0, black=1, dblack=2};

#define isred(node) (node->color==Color::red)
#define isblack(node) (node->color==Color::black)
#define isdblack(node) (node->color==Color::dblack)
#define isleaf(node) (!node->left&&!node->right)
#define isfull(node) (node->left && node->right)

#define green(text) std::string("\e[0;32m")+std::string(text)+std::string("\e[0m")
#define red(text) std::string("\e[0;31m")+std::string(text)+std::string("\e[0m")

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
    int depth;
  public:
    RBTree(): root(NULL), depth(0) {}
    explicit RBTree(int value): root(new Node_t(value)), depth(0) {root->color = Color::black;}

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
        rhs->father = lhs;
        if(rhs_f) {
            if(rhs==rhs_f->left) rhs_f->left = lhs ;
            else rhs_f->right = lhs;
            lhs->father = rhs_f;
        }
        else {
            this->root = lhs;
            lhs->father = NULL;
        }
        return lhs;
    }

    NodePtr_t rotate_left(NodePtr_t lhs, NodePtr_t rhs) {
        NodePtr_t lhs_f = lhs->father;
        NodePtr_t tmp_l = rhs->left;
        lhs->right = tmp_l;
        rhs->left = lhs;
        lhs->father = rhs;
        if(lhs_f) {
            if(lhs==lhs_f->left)  lhs_f->left = rhs;
            else lhs_f->right = rhs;
            rhs->father = lhs_f;
        }else {
            this->root = rhs;
            rhs->father = NULL;
        }
        return rhs;
    }

    void ascend_fatherIsRed(NodePtr_t inserted_node, NodePtr_t father) {
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
      if(!father) {
        std::cerr << red("inserted value already in the tree\n");
        return false;
      }
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
        if(!brother) {
            if(isred(father) || father==this->root) {
                father->color = Color::black;
                return;
            }else {
                NodePtr_t tmp = father->father;
                NodePtr_t b = (father==tmp->left)?tmp->right:tmp->left;
                black_black(tmp, b);
            }
        }
        if(brother==father->right) {
            if(isblack(brother) && brother->right && isred(brother->right)) {
                Color tmp = father->color;
                father->color = Color::black;
                brother->color = tmp;
                rotate_left(father, brother);
                brother->right->color = Color::black;
            }
            else if(isblack(brother) && brother->left && isred(brother->left)) {
                brother->left->color = Color::black;
                brother->color = Color::red;
                auto p = rotate_right(brother->left, brother);
                Color tmp = father->color;
                father->color = Color::black;
                p->color = tmp;
                rotate_left(father, p);
                p->right->color = Color::black;
            }
            else {
                brother->color = Color::red;
                if(isred(father) || father==this->root)
                    father->color = Color::black;
                else {
                    NodePtr_t tmp = father->father;
                    NodePtr_t b = (father==tmp->left)?tmp->right:tmp->left;
                    black_black(tmp, b);
                }
            }
        }else {
            NodePtr_t prev = father->right;
            if(isblack(brother) && brother->left && isred(brother->left)) {
                Color tmp = father->color;
                father->color = Color::black;
                brother->color = tmp;
                rotate_right(brother, father);
                brother->left->color = Color::black;
            }
            else if(isblack(brother) && brother->right && isred(brother->right)) {
                brother->right->color = Color::black;
                brother->color = Color::red;
                auto p = rotate_left(brother, brother->right);
                Color tmp = father->color;
                father->color = Color::black;
                p->color = tmp;
                rotate_right(p, father);
                p->left->color = Color::black;
            }
            else {
                brother->color = Color::red;
                if(isred(father) || father==this->root)
                    father->color = Color::black;
                else {
                    NodePtr_t tmp = father->father;
                    NodePtr_t b = (father==tmp->left)?tmp->right:tmp->left;
                    black_black(tmp, b);
                }
            }
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
            if(node_to_remove==this->root) {delete node_to_remove; this->root=NULL;return;}
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
            delete node_to_remove;
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
            delete child;
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
        if(!node_to_remove) {
            std::cerr << red("Value to be removed not in the tree\n");
            return false;
        }
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
            return true;
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

    void dfs(NodePtr_t root, size_t depth) {
        if(!root)
            return;
        if(depth > this->depth)
            this->depth = depth;
        dfs(root->left, depth+1);
        dfs(root->right, depth+1);
    }

    void update_depth() {
        if(this->depth!=0)
            return;
        dfs(this->root, 1);
    }

    void display(size_t width) {
        update_depth();
        size_t whole_width = (this->depth-1)*2*width+1;
        std::queue<NodePtr_t> q;
        q.push(this->root);
        size_t level = 0;
        while(!q.empty()) {
            level++;
            size_t num = q.size();
            for(size_t i=0;i<(this->depth-level);++i)
                for(size_t j=0;j<width;++j)
                    std::cout<<" ";
            size_t space;
            if(level>1)
                space = (whole_width-num-width*2*(this->depth-level))/(num-1);
            else
                space = 0;
            while(num--) {
                auto ptr = q.front();
                q.pop();
                if(ptr)
                    if(ptr->color==Color::red)
                        std::cout<<"\e[0;31m"<<ptr->value<<"\e[0m";
                    else
                        std::cout<<ptr->value;
                else
                    std::cout<<" ";
                for(size_t j=0;j<space;++j)
                    std::cout<<" ";
                if(level<this->depth) {
                   if(ptr) {
                        q.push(ptr->left);
                        q.push(ptr->right);
                    }else {
                        q.push(NULL);
                        q.push(NULL);
                    }
                }
            }
            std::cout<<"\n";
        }
    }
};


int main() {
    RBTree tree;
    for(int i=0;i<5;++i)
        tree.insert(i);
    tree.insert(-1);
    tree.display(4);
    tree.remove(-1);
    tree.display(4);
    tree.remove(4);
    tree.remove(2);
    tree.display(4);
    tree.remove(3);
    tree.display(4);
    return 0;
}
