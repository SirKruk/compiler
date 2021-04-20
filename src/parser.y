%{
#include <string.h>
#include <stdio.h>
#include "ast.h"
#include "ast_print.h"

int yylex();
void yyerror(ast **pp_ast, const char *s);
%}

%union {
    int i;
    double d;
    char *s;
    struct ast *p_ast;
}

%token <i> INT
%token <d> DOUBLE
%token <s> STRING
%token <s> ID

%token INPUT_START INPUT_STOP
%token SEMI
%token COMMA
%token LP RP
%token LC RC
%token LB RB

%token ADD SUB
%token MUL DIV
%token PWR
%token UNOP

%token TRUE FALSE
%token EQ NEQ EQEQ
%token LESS MORE
%token AND OR NOT
%token IF ELSE
%token WHILE

%token CONS

%type <p_ast> id exp seq binops terminal assign list items funcall lambda empty block select loop 

%precedence IFX
%precedence ELSE
%precedence WHILE
%right EQ
%right CONS
%right EQEQ NEQ
%right LESS MORE
%left OR
%left AND
%left ADD SUB
%left MUL DIV
%precedence UNOP
%right PWR

%parse-param {struct ast **pp_ast}


%%

program:        INPUT_START exp INPUT_STOP { *pp_ast = ast_clone($2); ast_free(&($2)); }
        |       INPUT_START seq INPUT_STOP { *pp_ast = ast_clone($2); ast_free(&($2)); }
        |       INPUT_START INPUT_STOP { *pp_ast = new_u_node(); }
                ;

exp:            block
        |       select
        |       loop
        |       binops
        |       list
        |       assign
        |       lambda
        |       id
        |       funcall
        |       LP exp RP { $$ = $2; }
        |       terminal
                ;

select:         IF LP exp RP exp %prec IFX { $$ = new_if_node($3, new_then_node($5)); }
        |       IF LP exp RP exp ELSE exp { $$ = new_if_node($3, new_then_else_node($5, $7)); }
                ;

loop:           WHILE LP exp RP block { $$ = new_while_node($3, $5); }
        |       WHILE LP exp RP SEMI { $$ = new_while_node($3, new_u_node()); }

                ;

block:          LC seq RC { $$ = $2; }
                ;


seq:            empty SEMI { $$ = new_seq_node($1); }
        |       seq empty SEMI { $$ = add_to_seq($1, $2); }
        |       exp SEMI { $$ = new_seq_node($1); }
        |       seq exp SEMI { $$ = add_to_seq($1, $2); }
                ;

binops:
                exp ADD exp { $$ = new_add_node($1, $3); }
        |       exp SUB exp { $$ = new_sub_node($1, $3); }
        |       exp MUL exp { $$ = new_mul_node($1, $3); }
        |       exp DIV exp { $$ = new_div_node($1, $3); }
        |       exp PWR exp { $$ = new_pwr_node($1, $3); }
        |       SUB exp %prec UNOP { $$ = new_neg_node($2); }
        |       ADD exp %prec UNOP { $$ = $2; }
        |       NOT exp %prec UNOP { $$ = new_not_node($2); }
        |       exp AND exp { $$ = new_and_node($1, $3); }
        |       exp OR exp { $$ = new_or_node($1, $3); }
        |       exp EQEQ exp { $$ = new_eqeq_node($1, $3); }
        |       exp NEQ exp { $$ = new_neq_node($1, $3); }
        |       exp MORE exp { $$ = new_more_node($1, $3); }
        |       exp MORE EQ exp { $$ = new_meq_node($1, $4); }
        |       exp LESS exp { $$ = new_less_node($1, $3); }
        |       exp LESS EQ exp { $$ = new_leq_node($1, $4); }
        |       exp CONS exp { $$ = new_cons_node($1, $3); }
                ;

terminal:       INT { $$ = new_i_node($1); }
        |       DOUBLE { $$ = new_d_node($1); }
        |       STRING { $$ = new_s_node($1); free($1); }
        |       TRUE { $$ = new_true_node(); }
        |       FALSE { $$ = new_false_node(); }
        |       LP RP { $$ = new_u_node(); }
                ;

id:             ID  { $$ = new_id_node($1); free($1); }
                ;

funcall:        id LP items RP { $$ = new_funcall_node($1, $3); }
        |       funcall LP items RP { $$ = new_funcall_node($1, $3); }
        |       LP lambda RP LP items RP { $$ = new_funcall_node($2, $5); }
        |       list LP items RP { $$ = new_funcall_node($1, $3); }
                ;

list:           LB items RB { $$ = $2; }
                ;

items:          exp { $$ = new_list_node($1); }
        |       empty { $$ = new_list_node($1); }
        |       items COMMA exp { $$ = add_to_list($1, $3); }
                ;

assign:         id EQ exp { $$ = new_assign_node($1, $3); }
                ;

lambda:         list SUB MORE exp { $$ = new_fun_node($1, $4); }
                ;

empty:          %empty { $$ = new_u_node(); }
                ;

%%

void yyerror(ast **pp_ast, const char *str)
{
    fprintf(stderr, "PARSE ERROR: %s\n", str);
    if (*pp_ast != NULL && (*pp_ast)->type != 0) {
        fprintf(stderr, "AST=");
        ast_fprint(stderr, *pp_ast);
        fprintf(stderr, "\n");
   }
}
