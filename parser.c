#include "hw_ast.h"
#include <stdlib.h>
#include <string.h>

struct AST_node *parse_expr(struct stream *s) {
  struct AST_node *term = parse_term(s);
  if (term == NULL)
    return NULL;
  if (peek(s).type == TOK_BINOP && *peek(s).start == '+') {
    next(s);
    struct AST_node *right = parse_expr(s);
    if (right == NULL) {
      free_AST(term);
      return NULL;
    }

    return make_op(term, right, NODE_ADD);
  } else {
    return term;
  }
}

struct AST_node *parse_term(struct stream *s) {
  struct AST_node *factor = parse_factor(s);
  if (factor == NULL)
    return NULL;
  if (peek(s).type == TOK_BINOP && *peek(s).start == '*') {
    next(s);
    struct AST_node *right = parse_term(s);
    if (right == NULL) {
      free_AST(factor);
      return NULL;
    }

    return make_op(factor, right, NODE_MUL);
  } else {
    return factor;
  }
}

struct AST_node *parse_factor(struct stream *s) {
  if (peek(s).type == TOK_LPAREN) {
    next(s); // consume '('
    struct AST_node *expr = parse_expr(s);
    if (expr == NULL)
      return NULL;
    if (next(s).type != TOK_RPAREN) {
      free_AST(expr);
      return NULL;
    }
    return expr;
  } else if (peek(s).type == TOK_LITERAL) {
    struct token next_tok = next(s);
    char int_buf[8] = {0};
    strncpy(int_buf, next_tok.start, next_tok.length);
    return make_lit(atoi(int_buf));
  } else if (peek(s).type == TOK_UNOP) {
    next(s);
    struct AST_node *expr = parse_factor(s);
    return make_op(NULL, expr, NODE_NEG);
  } else {
    return NULL;
  }
}

void free_AST(struct AST_node *node) {
  if (node == NULL)
    return;
  if (node->type != NODE_LIT) {
    free_AST(node->left);
    free_AST(node->right);
  }
  free(node);
}

struct AST_node *make_lit(int literal) {
  struct AST_node *node = malloc(sizeof(struct AST_node));
  node->type = NODE_LIT;
  node->literal = literal;
  return node;
}

struct AST_node *make_op(struct AST_node *left, struct AST_node *right,
                         enum node_type op) {
  struct AST_node *node = malloc(sizeof(struct AST_node));
  node->type = op;
  node->left = left;
  node->right = right;
  return node;
}