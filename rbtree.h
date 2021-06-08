enum color_t { BLACK, RED };

struct RBNode {
  RBNode *parent;
  RBNode *left;
  RBNode *right;
  enum color_t color;
  int key;
};

struct RBtree {
  RBNode *root;
};
