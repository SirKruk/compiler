#ifndef AST_PRINT
#define AST_PRINT

#include <stdio.h>
#include "ast.h"

#define PREFIX_AST "AST_"
#define PREFIX_OBJ "OBJ_"

#define INDENT_STR "  "

void ast_print(ast *p_ast);
void ast_fprint(FILE *file, ast *p_ast);
char* ast_to_str(ast *p_ast, int indent_lvl);

char* ast_node_to_str(ast *p_ast, int indent_lvl);
char* single_node_to_str(ast *p_ast, int indent_lvl);
char* i_node_to_str(ast *p_ast);
char* d_node_to_str(ast *p_ast);
char* s_node_to_str(ast *p_ast);
char* id_node_to_str(ast *p_ast);

char* ast_type_to_str(ast *p_ast);

#endif
