#include "hw_ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s [input]", argv[0]);
    return EXIT_FAILURE;
  }

  FILE *fp = fopen(argv[1], "r");
  if (fp == NULL) {
    printf("Failed to open file: %s\node", argv[1]);
    return EXIT_FAILURE;
  }

  char buf[1024];
  fgets(buf, 1024, fp);
  fclose(fp);

  struct stream s = {.text = buf, .length = strlen(buf), .pos = 0};
  struct AST_node *expr = parse_expr(&s);
  if (expr == NULL || peek(&s).type != TOK_EOF) {
    printf("Invalid\n");
  } else {
    int value = eval_ast(expr);
    printf("%d\n", value);
    free_AST(expr);
  }

  return 0;
}