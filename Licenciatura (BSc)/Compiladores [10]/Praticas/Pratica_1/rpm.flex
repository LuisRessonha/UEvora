%option noyywrap

%{
#include <stdio.h>

int result;
int element_1, element_2;

int stack[20];
int sp = 0;

int pop();
void push(int val);

%}

NUM [0-9]+

%%

{NUM}		{ push(atoi(yytext)); }
"+"			{ result = pop() + pop(); push(result); }
"*"			{ result = pop() * pop(); push(result); }
"-"			{ element_1 = pop();
			  element_2 = pop();
			  result = element_2 - element_1;
			  push(result);
			}
"/"			{ element_1 = pop();
			  element_2 = pop();
			  result = element_2 / element_1;
			  push(result);
			}

[\n\t ]+	{ /* ignore whitespace */ }

"show"		{ printf("(%d)\n", pop()); }
.			{ printf("Syntax error .\n"); }

%%

int main(int argc, char *argv[]) {
	while(1) {
		yylex();
	}
}

int pop() {
	if (sp == 0) {
		printf("Empty Stack!\n");
		return 0;
	}
	return stack[--sp];
}

void push(int val) {
	stack[sp++] = val;
}