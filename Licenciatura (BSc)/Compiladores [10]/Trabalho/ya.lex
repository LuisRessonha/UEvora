%{
#include <stdlib.h>
#include <string.h>
#include "apt.h"
#include "parser.h"

%}

INT [0-9]+                                           
FLOAT [0-9]*\.[0-9]+([eE][\+\-]?[0-9]+)?             
TYPE_VAR "int"|"float"|"string"|"bool"|"void"          
ID [A-Za-z]+[0-9A-Za-z]*
BOOLEAN "true"|"false"
STRING \"[^\"]*\"

%%

{INT}   {yylval.intlit = atoi(yytext); return INT; }
{FLOAT} {yylval.fltlit = atof(yytext); return FLOAT; }
{BOOLEAN}    { return BOOLEAN; } 
{TYPE_VAR}  {yylval.str = strdup(yytext); return TYPE_VAR; }


";"       { return SEMICOLON; }
"("       { return LPAR; }
")"       { return RPAR; }
"{"       { return LBRA; }
"}"       { return RBRA; }
","       { return COMMA; }
":"       { return COLON; }
"="       { return ASSIGN;}


"+"       { return ADD;}
"-"       { return SUB;}
"*"       { return MUL;}
"/"       { return DIV;}
"^"       { return EXP;}

"=="      { return EQUAL;}
"<"       { return LESS;}
">"       { return GREAT;}
"<="      { return LESSEQUAL;}
">="      { return GREATEQUAL;}
"!="      { return NEQUAL;}

"mod"     { return MOD;}
"and"     { return AND;}
"or"      { return OR;}
"not"     { return NOT;}

"define"  { return DEFINE;}
"if"      { return IF;}
"then"    { return THEN;}
"else"    { return ELSE;}
"while"   { return WHILE;}
"do"      { return DO;}

"return"  { return RETURN;}
"break"   { return BREAK;}
"next"    { return NEXT;}

{ID}   {yylval.str = strdup(yytext); return ID; }
{STRING}   {yylval.str = strdup(yytext); return STRING; }

#.*     ; /*ignore comments */
[ \t\n]+  ; /* ignore space */

%%
int yywrap()
{
    return 1;
}
