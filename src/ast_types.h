#ifndef AST_TYPES_H
#define AST_TYPES_H

#define PROCESS_TYPES(F)  F(AST_MIN),           \
    F(AST_UNIT),                                \
    F(AST_NEG),                                 \
    F(AST_INT),                                 \
    F(AST_DOUBLE),                              \
    F(AST_STRING),                              \
    F(AST_ID),                                  \
    F(AST_ADD),                                 \
    F(AST_SUB),                                 \
    F(AST_MUL),                                 \
    F(AST_DIV),                                 \
    F(AST_PWR),                                 \
    F(AST_ASSIGN),                              \
    F(AST_SEQ),                                 \
    F(AST_LIST),                                \
    F(AST_NIL),                                 \
    F(AST_FUNCALL),                             \
    F(AST_LAMBDA),                              \
    F(AST_PRED_TRUE),                           \
    F(AST_PRED_FALSE),                          \
    F(AST_PRED_AND),                            \
    F(AST_PRED_OR),                             \
    F(AST_PRED_NOT),                            \
    F(AST_PRED_EQ),                             \
    F(AST_PRED_NEQ),                            \
    F(AST_PRED_MORE),                           \
    F(AST_PRED_MEQ),                            \
    F(AST_PRED_LESS),                           \
    F(AST_PRED_LEQ),                            \
    F(AST_IF),                                  \
    F(AST_THEN),                                \
    F(AST_WHILE),                               \
    F(AST_CONS),                                \
    F(AST_MAX)

#define TO_ITEM(x) x

#define TO_STRING(x) #x

static const char *ast_type_strings[] = {
  PROCESS_TYPES(TO_STRING)
};

typedef enum {
    PROCESS_TYPES(TO_ITEM)
} ast_types;

#undef PROCESS_TYPES
#undef TO_ITEM
#undef TO_STRING

#endif
