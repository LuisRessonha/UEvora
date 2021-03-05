%{
#include <stdio.h>
#include <string.h>
#include "apt.h"

int yylex (void);
void yyerror (char const *);
%}

%union {
    int intlit;
    float fltlit;
	bool boollit;
    char *str;
    declist declist;
    decl decl;
	exprlist exprlist;
    expr expr;
    stmlist stmlist;
    stm stm;
    arglist arglist;
    arg arg;
    idlist idlist;
    type type;
}

/* Bison declarations */

%left 				SEMICOLON
%token <intlit>		INT
%token <fltlit>		FLOAT
%token <boollit>    BOOLEAN
%token <str>		TYPE_VAR STRING ID

%token COLON DEFINE IF THEN ELSE WHILE DO RETURN BREAK NEXT 

%right 		ASSIGN 
%left 		OR
%left 		AND
%left 		NOT
%nonassoc	EQUAL LESS LESSEQUAL GREAT GREATEQUAL NEQUAL

%left 	SUB ADD
%left 	MUL DIV MOD
%right 	EXP
%left 	NEG

%token	LPAR RPAR COMMA LBRA RBRA 

%type   <declist>   declist
%type   <decl>      decl
%type   <exprlist>  exprlist
%type   <expr>      expr
%type   <stmlist>   stmlist
%type   <stm>       stm
%type   <arglist>   arglist
%type   <arg>       arg
%type   <idlist>    idlist
%type   <type>      type

%%
prgrm: declist	{ /*analisedeclist($1);*/ }
;

declist:	decl SEMICOLON			{ $$ = declistnode($1, NULL); }
		|	decl SEMICOLON declist	{ $$ = declistnode($1, $3); }
;

decl:	idlist COLON type				{ $$ = vardeclnode($1, $3, NULL); }
	|	idlist COLON type ASSIGN expr	{ $$ = vardeclnode($1, $3, $5); }
	|	ID LPAR RPAR COLON type LBRA stmlist RBRA	{ $$ = funcdeclnode($1, NULL, $5, $7); }
	|	ID LPAR arglist RPAR COLON type LBRA stmlist RBRA 	{ $$ = funcdeclnode($1, $3, $6, $8); }
	|	DEFINE ID type		{ $$ = newtypedeclnode($2, $3); }
;

stmlist:	stm SEMICOLON 	{ $$ = stmlistnode($1, NULL); }
		|	stm SEMICOLON stmlist	{ $$ = stmlistnode($1, $3); }
;

stm:    decl            { $$ = declstmnode($1); }
    |   expr            { $$ = exprstmnode($1); }
    |   IF expr THEN LBRA stmlist RBRA      { $$ = ifstmnode($2, $5, NULL); }
    |   IF expr THEN LBRA stmlist RBRA ELSE LBRA stmlist RBRA   { $$ = ifstmnode($2, $5, $9); }
    |   WHILE expr DO LBRA stmlist RBRA     { $$ = whilestmnode($2, $5); }
    |   RETURN expr     { $$ = returnstmnode($2); }
;

exprlist:   expr        { $$ = exprlistnode($1, NULL); }
        |   expr COMMA exprlist { $$ = exprlistnode($1, $3); }
;

expr:       INT         { $$ = intexprnode($1); }
    |       FLOAT       { $$ = floatexprnode($1); }
    |       BOOLEAN     { $$ = boolexprnode($1); }
    |       STRING      { $$ = stringexprnode($1); }
    |       ID          { $$ = idexprnode($1); }
    |       expr ADD expr { $$ = binopexprnode("+", $1, $3); }
    |       expr SUB expr { $$ = binopexprnode("-", $1, $3); }
    |       expr MUL expr { $$ = binopexprnode("*", $1, $3); }
    |       expr DIV expr { $$ = binopexprnode("/", $1, $3); }
    |       expr EXP expr { $$ = binopexprnode("^", $1, $3); }
    |       expr MOD expr { $$ = binopexprnode("%", $1, $3); }
    |       expr AND expr { $$ = binopexprnode("&&", $1, $3); }
    |       expr OR expr { $$ = binopexprnode("||", $1, $3); }
    |       NOT expr { $$ = unopexprnode("!", $2); }
    |       SUB expr %prec NEG { $$ = unopexprnode("-", $2); }
    |       expr EQUAL expr { $$ = binopexprnode("==", $1, $3); }
    |       expr LESS expr { $$ = binopexprnode("<", $1, $3); }
    |       expr LESSEQUAL expr { $$ = binopexprnode("<=", $1, $3); }
    |       expr GREAT expr { $$ = binopexprnode(">", $1, $3); }
    |       expr GREATEQUAL expr { $$ = binopexprnode(">=", $1, $3); }
    |       expr NEQUAL expr { $$ = binopexprnode("!=", $1, $3); }       
    |       ID ASSIGN expr  { $$ = assignexprnode($1, $3); }
    |       LPAR expr RPAR { $$ = $2; }
    |       ID LPAR RPAR { $$ = fcallexprnode($1, NULL); }
    |       ID LPAR exprlist RPAR { $$ = fcallexprnode($1, $3); }
;

arglist:    arg     { $$ = arglistnode($1, NULL); }
        |   arg COMMA arglist { $$ = arglistnode($1, $3); }
;

arg:        ID COLON type   { $$ = argnode($1, $3); }
;

idlist:     ID { $$ = idlistnode($1, NULL); }
    |       ID COMMA idlist { $$ = idlistnode($1, $3); }
;

type:       TYPE_VAR    { $$ = typenode($1); }
    |       ID          { $$ = typenode($1); }
;
           

%%
void yyerror (char const *s){
	fprintf (stderr, "%s\n", s);
}
int main (void){
	return yyparse();
}