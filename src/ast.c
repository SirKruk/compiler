#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "ast.h"
#include "ast_types.h"
#include "ast_print.h"

ast* new_ast_node(ast_types type, ast *p_left, ast *p_right)
{
  assert(is_valid_ast_type(type));
  ast *ret = malloc(sizeof(struct ast));
  ret->type = type;
  ret->anno = new_unset_type();
  ret->ref = 0;
  ret->p_left = p_left;
  ret->p_right = p_right;
  return ref(ret);
}

ast* new_u_node()
{
  return new_ast_node(AST_UNIT, NULL, NULL);
}

ast* new_i_node(int val)
{
  ast *ret = new_ast_node(AST_INT, NULL, NULL);
  (CAST_I(ret))->value = val;
  return ret;
}

ast* new_d_node(double val)
{
  ast *ret = new_ast_node(AST_DOUBLE, NULL, NULL);
  (CAST_D(ret))->value = val;
  return ret;
}

ast* new_s_node(char *newstr)
{
  assert(newstr != NULL);
  ast *ret = new_ast_node(AST_STRING, NULL, NULL);
  int len = strlen(newstr) + 1;
  (CAST_S(ret))->str = malloc(len * sizeof(char));
  strncpy((CAST_S(ret))->str, newstr, len);
  return ret;
}

ast* new_id_node(char *newstr)
{
  assert(newstr != NULL);
  ast *ret = new_s_node(newstr);
  ret->type = AST_ID;
  return ret;
}

ast* new_nil_node()
{
  return new_ast_node(AST_NIL, NULL, NULL);
}

ast* new_list_node(ast *p_exp)
{
  assert(p_exp != NULL);
  return new_ast_node(AST_LIST, p_exp, new_nil_node());
}

ast* add_to_list(ast *p_list, ast *p_exp)
{
  assert(is_list(p_list) && p_exp != NULL);
  ast *tail = CAST_LIST(p_list)->tail;
  if (tail->type == AST_NIL) {
    ast *new_tail = new_list_node(p_exp);
    ast_free(&tail);
    CAST_LIST(p_list)->tail = new_tail;
  } else add_to_list(tail, p_exp);
  return p_list;
}

ast* new_seq_node(ast *p_exp)
{
  assert(p_exp != NULL);
  return fix_list_type(new_list_node(p_exp), AST_SEQ);
}

ast* add_to_seq(ast *p_seq, ast *p_exp)
{
  assert(p_seq != NULL && p_seq->type == AST_SEQ && p_exp != NULL);
  add_to_list(p_seq, p_exp);
  return fix_list_type(p_seq, AST_SEQ);
}

ast* new_funcall_node(ast* p_id, ast *p_args)
{
  assert(p_id != NULL && p_args != NULL);
  return new_ast_node(AST_FUNCALL, p_id, p_args);
}

ast* new_fun_node(ast* p_idargs, ast *p_exp)
{
  assert(p_idargs != NULL && p_idargs->type == AST_LIST &&
         p_exp != NULL);
  return new_ast_node(AST_LAMBDA, p_idargs, p_exp);
}

ast* new_neg_node(ast *p_ast)
{
  assert(p_ast != NULL);
  return new_ast_node(AST_NEG, p_ast, NULL);
}

ast* new_not_node(ast *p_ast)
{
  assert(p_ast != NULL);
  return new_ast_node(AST_PRED_NOT, p_ast, NULL);
}

ast* new_true_node()
{
  return new_ast_node(AST_PRED_TRUE, NULL, NULL);
}

ast* new_false_node()
{
  return new_ast_node(AST_PRED_FALSE, NULL, NULL);
}

ast* new_if_node(ast* p_pred, ast *p_then)
{
  assert(p_pred != NULL && p_then != NULL);
  return new_ast_node(AST_IF, p_pred, p_then);
}

ast* new_then_node(ast* p_then)
{
  assert(p_then != NULL);
  return new_ast_node(AST_THEN, p_then, new_u_node());
}

ast* new_then_else_node(ast* p_then, ast *p_else)
{
  assert(p_then != NULL && p_else != NULL);
  return new_ast_node(AST_THEN, p_then, p_else);
}

ast* new_while_node(ast* p_pred, ast *p_exp)
{
  assert(p_pred != NULL && p_exp != NULL);
  return new_ast_node(AST_WHILE, p_pred, p_exp);
}

ast* new_add_node(ast* p_exp1, ast *p_exp2)
{
  return new_ast_node(AST_ADD, p_exp1, p_exp2);
}

ast* new_sub_node(ast* p_exp1, ast *p_exp2)
{
  return new_ast_node(AST_SUB, p_exp1, p_exp2);
}

ast* new_mul_node(ast* p_exp1, ast *p_exp2)
{
  return new_ast_node(AST_MUL, p_exp1, p_exp2);
}

ast* new_div_node(ast* p_exp1, ast *p_exp2)
{
  return new_ast_node(AST_DIV, p_exp1, p_exp2);
}

ast* new_pwr_node(ast* p_exp1, ast *p_exp2)
{
  return new_ast_node(AST_PWR, p_exp1, p_exp2);
}

ast* new_and_node(ast* p_exp1, ast *p_exp2)
{
  return new_ast_node(AST_PRED_AND, p_exp1, p_exp2);
}

ast* new_or_node(ast* p_exp1, ast *p_exp2)
{
  return new_ast_node(AST_PRED_OR, p_exp1, p_exp2);
}

ast* new_eqeq_node(ast* p_exp1, ast *p_exp2)
{
  return new_ast_node(AST_PRED_EQ, p_exp1, p_exp2);
}

ast* new_neq_node(ast* p_exp1, ast *p_exp2)
{
  return new_ast_node(AST_PRED_NEQ, p_exp1, p_exp2);
}

ast* new_more_node(ast* p_exp1, ast *p_exp2)
{
  return new_ast_node(AST_PRED_MORE, p_exp1, p_exp2);
}

ast* new_meq_node(ast* p_exp1, ast *p_exp2)
{
  return new_ast_node(AST_PRED_MEQ, p_exp1, p_exp2);
}

ast* new_less_node(ast* p_exp1, ast *p_exp2)
{
  return new_ast_node(AST_PRED_LESS, p_exp1, p_exp2);
}

ast* new_leq_node(ast* p_exp1, ast *p_exp2)
{
  return new_ast_node(AST_PRED_LEQ, p_exp1, p_exp2);
}

ast* new_cons_node(ast* p_item, ast *p_list)
{
  return new_ast_node(AST_CONS, p_item, p_list);
}

ast* new_assign_node(ast* p_id, ast *p_exp)
{
  return new_ast_node(AST_ASSIGN, p_id, p_exp);
}


ast* ast_clone(ast *p_ast)
{
  if (p_ast == NULL) return NULL;
  return ref(p_ast);
}

void ast_free(ast **pp_ast)
{
  assert(*pp_ast != NULL);
  unref(*pp_ast);

  if ((*pp_ast)->ref > 0)
    return;

  obj_type_free(&((*pp_ast)->anno));

  switch ((*pp_ast)->type) {
  case AST_UNIT:
  case AST_NIL:
  case AST_INT:
  case AST_DOUBLE:
  case AST_PRED_TRUE:
  case AST_PRED_FALSE: break;
  case AST_STRING:
  case AST_ID: free((CAST_ID(*pp_ast))->str); break;
  case AST_NEG:
  case AST_PRED_NOT: ast_free(&((*pp_ast)->p_left)); break;
  default:
    ast_free(&(*pp_ast)->p_left);
    ast_free(&(*pp_ast)->p_right);
  }
  free(*pp_ast);
}

ast* ref(ast *p_ast)
{
  if (p_ast == NULL) return NULL;
  p_ast->ref++;
  return p_ast;
}

ast* unref(ast *p_ast)
{
  assert(p_ast != NULL && p_ast->ref > 0);
  p_ast->ref--;
  return p_ast;
}

int is_valid_ast_type(ast_types type)
{
  return type > AST_MIN && type < AST_MAX;
}

int is_list(ast *p_ast)
{
  return p_ast != NULL && (p_ast->type == AST_LIST || p_ast->type == AST_SEQ);
}

ast* fix_list_type(ast *p_list, ast_types type)
{
  list_node *list = CAST_LIST(p_list);
  list->type = type;
  if (list->tail->type != AST_NIL) fix_list_type(list->tail, type);
  return p_list;
}
