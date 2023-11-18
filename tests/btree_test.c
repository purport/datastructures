
#include <aion.h>

static bool btree_check_leaf(struct btree_node *node, u32 min, u32 max) {
  u32 bound = node->key[0];
  if (min > bound) {
    return false;
  }
  for (u32 i = 1; i != node->length; ++i) {
    if (bound >= node->key[i]) {
      return false;
    }
    bound = node->key[i];
  }
  if (bound > max) {
    return false;
  }
  return true;
}

static bool btree_check_node(struct btree_node *node, u32 height, u32 min,
                             u32 max) {
  u32 bound = min;
  for (u32 i = 0; i != node->length; ++i) {
    if (bound > node->key[i]) {
      return false;
    }
    if (height == 1) {
      if (!btree_check_leaf(node->children[i], bound, node->key[i])) {
        return false;
      }
    } else {
      if (!btree_check_node(node->children[i], height - 1, bound,
                            node->key[i])) {
        return false;
      }
    }
    bound = node->key[i] + 1;
  }
  if ((bound - 1) > max) {
    return false;
  }
  return true;
}

static bool btree_check(struct btree *tree) {
  if (tree->height == 0) {
    return btree_check_leaf(&tree->root, 0, U32_MAX);
  } else {
    return btree_check_node(&tree->root, tree->height, 0, U32_MAX);
  }
}

static void btree_node_print(struct btree_node *n, u32 height) {
  u64 id = (u64)(void *)n;

  printf("node%lu[label= \"{|{", id);
  for (u32 index = 0; index != BTREE_MAX; ++index) {
    printf("<f%u> |", index);
    if (index < n->length) {
      printf("%d", n->key[index]);
    }
    printf(" |");
  }
  printf("}}\"]\n");
  if (height != 0) {
    for (u32 index = 0; index != n->length; ++index) {
      btree_node_print(n->children[index], height - 1);

      printf("\"node%lu\":f%u -> \"node%lu\"\n", id, index,
             (u64)(void *)n->children[index]);
    }
  }
}
static void btree_print(struct btree *tree) {
  printf("digraph g{\nnode [shape = record, height=.05];\n");

  btree_node_print(&tree->root, tree->height);

  printf("}\n");
}

s32 main() {
  CHECK_START();

  {
    struct btree tree = {0};
    btree_insert(&tree, 1, NULL);
    btree_insert(&tree, 3, NULL);
    btree_insert(&tree, 2, NULL);
    btree_insert(&tree, 4, NULL);
    btree_insert(&tree, 12, NULL);
    btree_insert(&tree, 6, NULL);
    btree_insert(&tree, 7, NULL);
    btree_insert(&tree, 8, NULL);
    btree_insert(&tree, 13, NULL);
    btree_insert(&tree, 15, NULL);
    btree_insert(&tree, 16, NULL);
    btree_insert(&tree, 17, NULL);
    btree_insert(&tree, 18, NULL);
    CHECK(btree_check(&tree), ==, true);
    btree_print(&tree);
    // CHECK(btree_check(&tree), ==, true);
    // btree_insert(&tree, 4, NULL);
    // CHECK(btree_check(&tree), ==, true);
  }

  CHECK_END();
}
