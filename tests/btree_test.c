#include <aion.h>
#include <stdlib.h>

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
      if (!btree_check_leaf(node->child[i], bound, node->key[i])) {
        return false;
      }
    } else {
      if (!btree_check_node(node->child[i], height - 1, bound, node->key[i])) {
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
      btree_node_print(n->child[index], height - 1);

      printf("\"node%lu\":f%u -> \"node%lu\"\n", id, index,
             (u64)(void *)n->child[index]);
    }
  }
}
static void btree_print(struct btree *tree) {
#define BOX  QUOTE(COLOR_LION)
#define TEXT QUOTE(COLOR_COCOA)
#define EDGE QUOTE(COLOR_OLIVINE)
  // clang-format off
  printf("digraph g{\n"
      "node [shape = record,height=.05,fontcolor="TEXT",color="BOX"];\n"
      "edge [color="EDGE"];\n"
      "bgcolor=\"transparent\";\n");
  // clang-format on

  btree_node_print(&tree->root, tree->height);

  printf("}\n");
}

static s32 cmp(const void *a, const void *b) { return *(u32 *)a - *(u32 *)b; }

s32 main() {
  CHECK_START();

  {
    struct btree tree = {0};
    btree_insert(&tree, 1);
    btree_insert(&tree, 3);
    btree_insert(&tree, 2);
    btree_insert(&tree, 4);
    btree_insert(&tree, 6);
    btree_print(&tree);
    btree_insert(&tree, 12);
    btree_insert(&tree, 6);
    btree_insert(&tree, 7);
    btree_insert(&tree, 8);
    btree_insert(&tree, 13);
    btree_insert(&tree, 15);
    btree_insert(&tree, 16);
    btree_insert(&tree, 17);
    btree_insert(&tree, 18);
    CHECK(btree_check(&tree), ==, true);

    // struct btree_iterator it;
    // btree_iterator(&tree, &it);
    // CHECK(btree_iterator_next(&it), ==, true);
    // CHECK(btree_iterator_key(&it), ==, 1);
    // CHECK(btree_iterator_next(&it), ==, true);
    // CHECK(btree_iterator_key(&it), ==, 2);
    // CHECK(btree_iterator_next(&it), ==, true);
    // CHECK(btree_iterator_key(&it), ==, 3);
    // CHECK(btree_iterator_next(&it), ==, true);
    // CHECK(btree_iterator_key(&it), ==, 4);
    // CHECK(btree_iterator_next(&it), ==, true);
    // CHECK(btree_iterator_key(&it), ==, 6);
    // CHECK(btree_iterator_next(&it), ==, true);
    // CHECK(btree_iterator_key(&it), ==, 7);
    // CHECK(btree_iterator_next(&it), ==, true);
    // CHECK(btree_iterator_key(&it), ==, 8);
    // CHECK(btree_iterator_next(&it), ==, true);
    // CHECK(btree_iterator_key(&it), ==, 12);
    // CHECK(btree_iterator_next(&it), ==, true);
    // CHECK(btree_iterator_key(&it), ==, 13);
    // CHECK(btree_iterator_next(&it), ==, true);
    // CHECK(btree_iterator_key(&it), ==, 15);
    // CHECK(btree_iterator_next(&it), ==, true);
    // CHECK(btree_iterator_key(&it), ==, 16);
    // CHECK(btree_iterator_next(&it), ==, true);
    // CHECK(btree_iterator_key(&it), ==, 17);
    // CHECK(btree_iterator_next(&it), ==, true);
    // CHECK(btree_iterator_key(&it), ==, 18);
    // CHECK(btree_iterator_next(&it), ==, false);
  }

  if (0) {
    static u32 key[2305] = {0};
    srand(7);
    struct btree tree = {0};
    for (u32 i = 0; COUNT(key) != i; ++i) {
      key[i] = rand();
      btree_insert(&tree, key[i]);
    }

    qsort(key, COUNT(key), sizeof(*key), cmp);

    u32 i = 0;
    struct btree_iterator it = {0};
    btree_iterator(&tree, &it);
    while (btree_iterator_next(&it)) {
      CHECK(i, !=, COUNT(key));
      if (i == COUNT(key)) {
        break;
      }

      u32 k = btree_iterator_key(&it);
      CHECK(key[i++], ==, k);
    }
    CHECK(i, ==, COUNT(key));
  }

  CHECK_END();
}
