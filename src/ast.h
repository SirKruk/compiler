#ifndef AST_H
#define AST_H

#include "ast_types.h"
#include "obj_type.h"

#define CAST(p) ((ast *)p)
#define CAST_U(p) ((u_node *)p)
#define CAST_I(p) ((i_node *)p)
#define CAST_D(p) ((d_node *)p)
#define CAST_S(p) ((s_node *)p)
#define CAST_ID(p) ((s_node *)p)
#define CAST_LIST(p) ((list_node *)p)

typedef struct ast {
  ast_types type;
  int ref;
  obj_type *anno;
  struct ast *p_left;
  struct ast *p_right;
} ast;

typedef struct {
  ast_types type;
  int ref;
  obj_type *anno;
} u_node;

typedef struct {
  ast_types type;
  int ref;
  obj_type *anno;
  int value;
} i_node;

typedef struct {
  ast_types type;
  int ref;
  obj_type *anno;
  double value;
} d_node;

typedef struct {
  ast_types type;
  int ref;
  obj_type *anno;
  char *str;
} s_node;

typedef struct {
  ast_types type;
  int ref;
  obj_type *anno;
  struct ast *head;
  struct ast *tail;
} list_node;

ast* new_ast_node(ast_types type, ast *p_left, ast *p_right);
ast* new_u_node();
ast* new_i_node(int val);
ast* new_d_node(double val);
ast* new_s_node(char *str);
ast* new_id_node(char *str);
ast* new_list_node(ast *p_exp);
ast* new_seq_node(ast *p_exp);
ast* new_nil_node();

ast* new_funcall_node(ast* p_id, ast *p_args);
ast* new_fun_node(ast* p_idargs, ast *p_exp);
ast* new_neg_node(ast *p_ast);
ast* new_not_node(ast *p_ast);
ast* new_true_node();
ast* new_false_node();
ast* new_if_node(ast* p_pred, ast *p_then);
ast* new_then_node(ast* p_then);
ast* new_then_else_node(ast* p_then, ast *p_else);
ast* new_while_node(ast* p_pred, ast *p_exp);

ast* new_add_node(ast* p_exp1, ast *p_exp2);
ast* new_sub_node(ast* p_exp1, ast *p_exp2);
ast* new_mul_node(ast* p_exp1, ast *p_exp2);
ast* new_div_node(ast* p_exp1, ast *p_exp2);
ast* new_pwr_node(ast* p_exp1, ast *p_exp2);

ast* new_and_node(ast* p_exp1, ast *p_exp2);
ast* new_or_node(ast* p_exp1, ast *p_exp2);
ast* new_eqeq_node(ast* p_exp1, ast *p_exp2);
ast* new_neq_node(ast* p_exp1, ast *p_exp2);
ast* new_more_node(ast* p_exp1, ast *p_exp2);
ast* new_meq_node(ast* p_exp1, ast *p_exp2);
ast* new_less_node(ast* p_exp1, ast *p_exp2);
ast* new_leq_node(ast* p_exp1, ast *p_exp2);

ast* new_cons_node(ast* p_item, ast *p_list);
ast* new_assign_node(ast* p_id, ast *p_exp);

ast* add_to_list(ast *p_seq, ast *p_exp);
ast* add_to_seq(ast *p_seq, ast *p_exp);

ast* ast_clone(ast *p_ast);
void ast_free(ast **pp_ast);
ast* ref(ast *p_ast);
ast* unref(ast *p_ast);

int is_valid_ast_type(ast_types type);
int is_list(ast *p_ast);
ast* fix_list_type(ast *p_list, ast_types type);

#endif
