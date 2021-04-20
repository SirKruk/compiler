#include <stdio.h>
#include "parser.tab.h"

extern int yylex();
extern int yylex_destroy();
extern char* yytext;

int main(void)
{
  int token;
  while((token = yylex())) {
    switch (token) {
    case INPUT_START: printf("INPUT_START"); break;
    case INPUT_STOP: printf("INPUT_STOP"); break;

    case TRUE: printf("TRUE"); break;
    case FALSE: printf("FALSE"); break;
    case IF: printf("IF"); break;
    case ELSE: printf("ELSE"); break;
    case WHILE: printf("WHILE"); break;

    case LP: printf("LP"); break;
    case RP: printf("RP"); break;
    case LC: printf("LC"); break;
    case RC: printf("RC"); break;
    case LB: printf("LB"); break;
    case RB: printf("RB"); break;

    case SEMI: printf("SEMI"); break;
    case COMMA: printf("COMMA"); break;
    case CONS: printf("CONS"); break;

    case EQ: printf("EQ"); break;
    case EQEQ: printf("EQEQ"); break;
    case NEQ: printf("NEQ"); break;
    case LESS: printf("LESS"); break;
    case MORE: printf("MORE"); break;

    case AND: printf("AND"); break;
    case OR: printf("OR"); break;
    case NOT: printf("NOT"); break;

    case ADD: printf("ADD"); break;
    case SUB: printf("SUB"); break;
    case MUL: printf("MUL"); break;
    case DIV: printf("DIV"); break;
    case PWR: printf("PWR"); break;

    case INT: printf("INT(%d)", yylval.i); break;
    case DOUBLE: printf("DOUBLE(%g)", yylval.d); break;
    case STRING: printf("STRING(%s)", yylval.s); break;
    case ID: printf("ID(%s)", yylval.s); break;

    default: printf("unknown token: \"%s\"\n", yytext);
    }
    printf("\n");
  }
  yylex_destroy();
}
