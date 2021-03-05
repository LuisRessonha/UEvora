/* Aula 2019.10.08-09  Parte A*/
num(z).
num(s(X)):-num(X).

num(z, 0).
num(s(X), Y):-num(X, Y1), Y is Y1+1.

% true se B = A+1
mais1(X, s(X)).

% soma (A. B. R), A+B=R
soma(z, X, X).
soma(s(X), Y, s(Z)):- soma(X, Y, Z).

% substrai (A, B, R), A-B=R
sub(X, Y, Z):-soma(Z, Y, X).

% less or equal
le(X, Y):- soma(X, _, Y).

% less than
lt(X, Y):- le(s(X), Y).

% dobro (A, B), B=A*x
dobro(X, Y):- soma(X, X, Y).

/*mult (A, B, C), A*B=C
A*B=X=(A-1)*B+B*/
mult(z, _, z).
mult(s(X), Y, Z):-mult(X, Y, A),
                  soma(A, Y, Z).

% div inteira x/y=z <=> x=z*y
div(X, Y, Z):-mult(Z, Y, X).

% div com resto
div(X, Y, Q, R):- mult(Q, Y, Z),
                  soma(Z, R, X).

% pot(X, Y, Z), X^Y = Z
pot(_, z, s(z)).
pot(X, s(Y), Z) :- pot(X, Y, W),
                   mult(W, X, Z).

% quadrado(A, B)
quadrado(X, Y):- mult(X, X, Y).