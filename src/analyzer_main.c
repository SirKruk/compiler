#include <stdlib.h>
#include "parser.tab.h"
#include "ast.h"
#include "ast_print.h"
#include "analyzer.h"

extern int yylex_destroy();

int main(void)
{
  ast *p_ret = NULL;
  if (yyparse(&p_ret) == 0) {
    analyze(p_ret);
    ast_print(p_ret);
  }
  if (p_ret != NULL) ast_free(&p_ret);
  yylex_destroy();
}
