#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ast.h"
#include "ast_print.h"
#include "ast_types.h"
#include "util.h"

void ast_print(ast *p_ast)
{
  if (p_ast == NULL) printf("NULL");
  else ast_fprint(stdout, p_ast);
  printf("\n");
}

void ast_fprint(FILE *file, ast *p_ast)
{
  char *str = ast_to_str(p_ast, 0);
  fprintf(file, "%s", str);
  free(str);
}

char* unknown()
{
  char str[] = "#unknown#";
  char *ret = malloc((strlen(str) + 1) * sizeof(char));
  sprintf(ret, "%s", str);
  return ret;
}

char *mkindent_sep(int indent_lvl)
{
  int len = (strlen("\n") + (indent_lvl + 1) * strlen(INDENT_STR) + 1) * sizeof(char);
  char *ret = malloc(len);
  strcpy(ret, "\n");
  while (indent_lvl-- > 0) strcat(ret, INDENT_STR);
  return ret;
}

char* ast_to_str(ast *p_ast, int indent_lvl)
{
  if (p_ast == NULL) return NULL;
  char *inner_str;
  switch(p_ast->type) {
  case AST_UNIT:
  case AST_NIL:
  case AST_PRED_TRUE:
  case AST_PRED_FALSE: inner_str = ast_type_to_str(p_ast); break;
  case AST_INT: inner_str = i_node_to_str(p_ast); break;
  case AST_DOUBLE: inner_str = d_node_to_str(p_ast); break;
  case AST_STRING: inner_str = s_node_to_str(p_ast); break;
  case AST_ID: inner_str = id_node_to_str(p_ast); break;
  case AST_NEG:
  case AST_PRED_NOT: inner_str = single_node_to_str(p_ast, indent_lvl); break;
  default: inner_str = ast_node_to_str(p_ast, indent_lvl);
  }
  char *indent_sep = mkindent_sep(indent_lvl);
  int len = (strlen(inner_str) + strlen(indent_sep) + strlen("<>") + 1) * sizeof(char);
  char *ret = malloc(len);
  if (indent_lvl == 0) sprintf(ret, "<%s>", inner_str);
  else sprintf(ret, "%s<%s>", indent_sep, inner_str);
  free(inner_str);
  free(indent_sep);
  return ret;
}

char* ast_node_to_str(ast *p_ast, int indent_lvl)
{
  if (p_ast == NULL) return NULL;
  if (!is_valid_ast_type(p_ast->type)) return unknown();
  char *left_str = ast_to_str(p_ast->p_left, indent_lvl + 1);
  char *right_str = ast_to_str(p_ast->p_right, indent_lvl + 1);
  char *base_str = ast_type_to_str(p_ast);
  char *indent_sep = mkindent_sep(indent_lvl);
  int len = (strlen(base_str) +
             strlen(left_str) +
             strlen(right_str) +
             strlen(indent_sep) + 1) * sizeof(char);
  char *ret = malloc(len);
  sprintf(ret, "%s%s%s%s", base_str, left_str, right_str, indent_sep);
  free(left_str);
  free(right_str);
  free(base_str);
  free(indent_sep);
  return ret;
}

char* single_node_to_str(ast *p_ast, int indent_lvl)
{
  char *base_str = ast_type_to_str(p_ast);
  char *inner_str = ast_to_str(p_ast->p_left, indent_lvl + 1);
  char *indent_sep = mkindent_sep(indent_lvl);
  int len = (strlen(base_str) +
             strlen(inner_str) +
             strlen(indent_sep) + 1) * sizeof(char);
  char *ret = malloc(len);
  sprintf(ret, "%s%s%s", base_str, inner_str, indent_sep);
  free(base_str);
  free(inner_str);
  free(indent_sep);
  return ret;
}

char* i_node_to_str(ast *p_ast)
{
  int val = CAST_I(p_ast)->value;
  char *base_str = ast_type_to_str(p_ast);
  int number_len = snprintf(NULL, 0, "%d", val);
  int len = (strlen(base_str) + number_len + strlen(" ") + 1) * sizeof(char);
  char *ret = malloc(len);
  sprintf(ret, "%s %d", base_str, val);
  free(base_str);
  return ret;
}

char* d_node_to_str(ast *p_ast)
{
  double val = CAST_D(p_ast)->value;
  char *base_str = ast_type_to_str(p_ast);
  int number_len = snprintf(NULL, 0, "%g", val);
  int len = (strlen(base_str) + number_len + strlen(" ") + 1) * sizeof(char);
  char *ret = malloc(len);
  sprintf(ret, "%s %g", base_str, val);
  free(base_str);
  return ret;
}

char* s_node_to_str(ast *p_ast)
{
  char *str = CAST_S(p_ast)->str;
  char *base_str = ast_type_to_str(p_ast);
  int len = (strlen(base_str) + strlen(str) + strlen(" \"\"") + 1) * sizeof(char);
  char *ret = malloc(len);
  sprintf(ret, "%s \"%s\"", base_str, str);
  free(base_str);
  return ret;
}

char* id_node_to_str(ast *p_ast)
{
  char *str = CAST_S(p_ast)->str;
  char *base_str = ast_type_to_str(p_ast);
  int len = (strlen(base_str) + strlen(str) + 2) * sizeof(char);
  char *ret = malloc(len);
  sprintf(ret, "%s %s", base_str, str);
  free(base_str);
  return ret;
}

char* ast_type_to_str(ast *p_ast)
{
  char *ret;
  char *obj_str = obj_type_to_str(p_ast->anno);
  char *ast_str = strip_prefix(ast_type_strings[p_ast->type], PREFIX_AST);
  int len = (strlen(obj_str) + strlen(ast_str) + 1) * sizeof(char);
  ret = malloc(len);
  sprintf(ret,"%s%s", obj_str, ast_str);
  free(obj_str);
  free(ast_str);
  return ret;
}
