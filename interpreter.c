#include "hw_ast.h"
#include <stdio.h>
#include <stdlib.h>

int eval_ast(struct AST_node *node) {
  if (node->type == NODE_LIT) {
    // fprintf(stderr, "Literal %d\n", node->literal);
    return node->literal;
  }
  if (node->type == NODE_NEG) {
    int right_val = eval_ast(node->right);
    // fprintf(stderr, "negating %d -> %d\n", right_val, -right_val);
    return -right_val;
  }
  int left_val = eval_ast(node->left);
  int right_val = eval_ast(node->right);
  if (node->type == NODE_ADD) {
    // fprintf(stderr, "%d + %d = %d\n", left_val, right_val,
    //         left_val + right_val);
    return left_val + right_val;
  }

  if (node->type == NODE_MUL) {
    // fprintf(stderr, "%d * %d = %d\n", left_val, right_val,
    // left_val * right_val);
    return left_val * right_val;
  }

  fprintf(stderr, "Error: Unknown node type %d\n", node->type);
  exit(1);
}