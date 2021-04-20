#ifndef ANALYZER_H
#define ANALYZER_H

#include "ast.h"

void analyze(ast *p_ast);

obj_type* analyze_ast(ast *p_ast);

obj_type* analyze_list(ast *p_list);

#endif
