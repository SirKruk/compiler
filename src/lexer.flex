%{
#include<string.h>
#include "parser.tab.h"
#define YY_NO_INPUT
#define BUF_SIZE_BASE 2 * sizeof(char)

  size_t strbuf_size = BUF_SIZE_BASE;
  char *p_strbuf;
  size_t str_idx;
%}

%s START
%x INLINE
%x STRREAD

%%

. { BEGIN INLINE; unput(*yytext); return INPUT_START; }

<INLINE>"true" { return TRUE; }
<INLINE>"false" { return FALSE; }
<INLINE>"if" { return IF; }
<INLINE>"else" { return ELSE; }
<INLINE>"while" { return WHILE; }


<INLINE>[0-9]+ { yylval.i = atoi(yytext); return INT; }
<INLINE>[0-9]+\.[0-9]+ { yylval.d = atof(yytext); return DOUBLE; }
<INLINE>[a-zA-Z][a-zA-Z0-9]* {
  yylval.s = malloc(sizeof(char) * (strlen(yytext) + 1));
  strcpy(yylval.s, yytext);
  return ID;
 }

<INLINE>"+"  { return ADD; }
<INLINE>"-"  { return SUB; }
<INLINE>"*"  { return MUL; }
<INLINE>"/"  { return DIV; }
<INLINE>"^"  { return PWR; }

<INLINE>"("  { return LP; }
<INLINE>")"  { return RP; }
<INLINE>"{"  { return LC; }
<INLINE>"}"  { return RC; }
<INLINE>"["  { return LB; }
<INLINE>"]"  { return RB; }

<INLINE>"<" { return LESS; }
<INLINE>">" { return MORE; }

<INLINE>"&" { return AND; }
<INLINE>"|" { return OR; }
<INLINE>"==" { return EQEQ; }
<INLINE>"!=" { return NEQ; }
<INLINE>"=" { return EQ; }
<INLINE>"!" { return NOT; }

<INLINE>";"  { return SEMI; }
<INLINE>","  { return COMMA; }

<INLINE>":"  { return CONS; }

<INLINE>"\"" { BEGIN STRREAD; yylval.s = malloc(strbuf_size); *yylval.s = '\0'; str_idx = 0; }
<STRREAD>[^\"]* {
  str_idx = strlen(yylval.s);
  int len = strlen(yytext) + 1;
  if (str_idx + len > strbuf_size) {
    while (str_idx + len > strbuf_size)
      strbuf_size *= 2;
    yylval.s = realloc(yylval.s, strbuf_size);
  }
  strcpy(yylval.s + str_idx, yytext);
 }
<STRREAD>"\""  { BEGIN INLINE; return STRING; }

<INLINE>"#".*$ { /*comment*/ }
<INLINE>[\ \t\n] { /*ignore*/ }

<INLINE><<EOF>> { BEGIN START; return INPUT_STOP; }

%%
