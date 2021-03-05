CC = gcc
CFLAGS = -g
OUTFILE = ya
LIBS = -lm

all: lexer.c parser.c apt.c
	$(CC) $(CFLAGS) parser.c lexer.c apt.c -o $(OUTFILE) $(LIBS)


lexer.c: ya.lex
	flex -o lexer.c ya.lex

parser.c: ya.y
	bison -y -d ya.y -o parser.c

clean: 
	rm -rf parser.c lexer.c parser.h
	rm -f *~
	rm -f *.o
	rm -f ya