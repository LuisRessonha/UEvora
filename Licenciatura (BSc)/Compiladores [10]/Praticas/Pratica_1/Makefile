# Makefile for the rpm calculator
#

rpncalc: lex.yy.c
		gcc -o $@ $<

lex.yy.c: rpm.flex
		flex $<

clean:
		rm -f lex.yy.c

clean_all clean-all: clean
		rm -f rpncalc
