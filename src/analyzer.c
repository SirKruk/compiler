#include <stdlib.h>
#include "analyzer.h"
#include "ast_types.h"
#include "obj_type.h"

void analyze(ast *p_ast)
{
  if (obj_type_is_unset(p_ast->anno)) {
      obj_type_free(&(p_ast->anno));
      p_ast->anno = analyze_ast(p_ast);
    }

}

obj_type* analyze_ast(ast *p_ast)
{
  obj_type *ret = NULL;
  switch (p_ast->type) {
  case AST_INT: ret = new_int_type(); break;
  case AST_DOUBLE: ret = new_flt_type(); break;
  case AST_STRING: ret = new_str_type(); break;
  case AST_UNIT: ret = new_sth_type(); break;
  case AST_PRED_TRUE:
  case AST_PRED_FALSE: ret = new_bool_type(); break;
  case AST_LIST: ret = analyze_list(p_ast); break;
  default: ret = new_nth_type();
  }
  return ret;
}

/*
 * [] => NIL = LST[NTH]
 * [i:T] => LST[T]
 * [i:T1, j:T2] => if exist T: T1<:T and T2<:T then LST[T] else NTH
 */
obj_type* analyze_list(ast *p_ast)
{
  ast *head = CAST_LIST(p_ast)->head;
  if (head->type == AST_UNIT);
    // to do: a type hierarchy
  return NULL;
}
