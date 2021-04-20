#ifndef OBJ_TYPES_H
#define OBJ_TYPES_H

#define PROCESS_TYPES(F)  F(OBJ_MIN),                      \
    F(OBJ_UNSET),                                          \
    F(OBJ_BOOL),                                           \
    F(OBJ_INT),                                            \
    F(OBJ_FLT),                                            \
    F(OBJ_STR),                                            \
    F(OBJ_LST),                                            \
    F(OBJ_LAM),                                            \
    F(OBJ_STH),                                            \
    F(OBJ_NTH),                                            \
    F(OBJ_MAX)

#define TO_ITEM(x) x

#define TO_STRING(x) #x

static const char *obj_type_strings[] = {
  PROCESS_TYPES(TO_STRING)
};

typedef enum {
    PROCESS_TYPES(TO_ITEM)
} obj_types;

#undef PROCESS_TYPES
#undef TO_ITEM
#undef TO_STRING

#endif
