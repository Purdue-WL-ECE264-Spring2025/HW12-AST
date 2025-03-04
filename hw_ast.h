#ifndef HW_AST
#define HW_AST

/*

1 - 2

expr = term + expr | term - expr | term
term = factor | factor * term
factor = int | `(` expr `)`
int = ...

A = B C | B
C = x y z | x y w

(1 + 2) * 3

expr := term expr_rest
expr_rest := '+' expr | '-' expr | {}

term := factor term_rest
term_rest = '*' term | {}

factor := int | '(' expr ')'
*/

#include <stddef.h>

enum parse_result { PARSE_SUCCESS, PARSE_FAILURE };
enum token_type {
  TOK_LPAREN,
  TOK_RPAREN,
  TOK_BINOP,
  TOK_UNOP,
  TOK_LITERAL,
  TOK_EOF
};

struct stream {
  const char *text;
  int pos;
  size_t length;
};

struct token {
  enum token_type type;
  const char *start;
  size_t length;
};

enum node_type { NODE_ADD, NODE_MUL, NODE_NEG, NODE_LIT };

struct AST_node {
  enum node_type type;
  union {
    struct {
      struct AST_node *left;
      struct AST_node *right;
    };
    int literal;
  };
};

struct token peek(struct stream *s);
struct token next(struct stream *s);

void skip_whitespace(struct stream *s);

struct AST_node *parse_expr(struct stream *s);
struct AST_node *parse_term(struct stream *s);
struct AST_node *parse_factor(struct stream *s);

void free_AST(struct AST_node *node);
struct AST_node *make_lit(int literal);
struct AST_node *make_op(struct AST_node *left, struct AST_node *right,
                         enum node_type op);

int eval_ast(struct AST_node *node);

#endif // HW_AST