#include <stdlib.h>
#include "parser.tab.h"
#include "ast.h"
#include "ast_print.h"

extern int yylex_destroy();

int main(void) {
  ast *p_ast = NULL;
  if (yyparse(&p_ast) == 0) ast_print(p_ast);
  if (p_ast != NULL) ast_free(&p_ast);
  yylex_destroy();
}
