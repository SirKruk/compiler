#ifndef OBJ_TYPE
#define OBJ_TYPE

#include "obj_types.h"

typedef struct obj_type {
  obj_types type;
  struct obj_type *p_inner_type;
} obj_type;

obj_type* new_complex_type(obj_types type, obj_type *p_inner_type);
obj_type* new_type(obj_types type);
obj_type* new_unset_type();
obj_type* new_int_type();
obj_type* new_flt_type();
obj_type* new_str_type();
obj_type* new_bool_type();
obj_type* new_sth_type();
obj_type* new_nth_type();

void obj_type_free(obj_type **pp_obj_type);

int obj_type_is_unset(obj_type *p_obj_type);
char* obj_type_to_str(obj_type *p_obj_type);
#endif
