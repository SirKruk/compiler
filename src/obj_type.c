#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "obj_type.h"
#include "util.h"

#define PREFIX_OBJ "OBJ_"

obj_type* new_complex_type(obj_types type, obj_type *p_inner_type)
{
  obj_type* ret = malloc(sizeof(struct obj_type));
  ret->type = type;
  ret->p_inner_type = p_inner_type;
  return ret;
}

obj_type* new_type(obj_types otype)
{
  return new_complex_type(otype, NULL);
}

obj_type* new_unset_type()
{
  return new_type(OBJ_UNSET);
}

obj_type* new_int_type()
{
  return new_type(OBJ_INT);
}

obj_type* new_flt_type()
{
  return new_type(OBJ_FLT);
}

obj_type* new_str_type()
{
  return new_type(OBJ_STR);
}

obj_type* new_bool_type()
{
  return new_type(OBJ_BOOL);
}

obj_type* new_sth_type()
{
  return new_type(OBJ_STH);
}

obj_type* new_nth_type()
{
  return new_type(OBJ_NTH);
}

void obj_type_free(obj_type **pp_obj_type)
{
  assert(*pp_obj_type != NULL);

  if ((*pp_obj_type)->p_inner_type != NULL)
    obj_type_free(&((*pp_obj_type)->p_inner_type));

  free(*pp_obj_type);
}

int obj_type_is_unset(obj_type *p_obj_type)
{
  return p_obj_type->type == OBJ_UNSET;
}

char* obj_type_to_str(obj_type *p_obj_type)
{
  const char *obj_string = obj_type_strings[p_obj_type->type];
  char *inner_str = strip_prefix(obj_string, PREFIX_OBJ);
  int len = (strlen(inner_str) + strlen("[] ") + 1) * sizeof(char);
  char *ret = malloc(len);
  if (obj_type_is_unset(p_obj_type)) sprintf(ret, "");
  else sprintf(ret, "[%s] ", inner_str);
  free(inner_str);
  return ret;
}
