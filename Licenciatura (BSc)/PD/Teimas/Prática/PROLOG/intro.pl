/*Family Tree*/

/*Facts*/
homem(zé).
homem(daniel).
homem(celestino).
homem(osvaldo).
homem(luis).
homem(ruben).
homem(lourenço).

mulher(lucinda).
mulher(robertina).
mulher(maria).
mulher(fake).

progenitor(daniel, zé).
progenitor(robertina, zé).
progenitor(celestino, osvaldo).
progenitor(celestino, maria).
progenitor(lucinda, maria).
progenitor(lucinda, osvaldo).
progenitor(zé, luis).
progenitor(zé, ruben).
progenitor(maria, luis).
progenitor(maria, ruben).
progenitor(osvaldo, lourenço).
progenitor(osvaldo, fake).

/*Rules*/

irmao(X, Y):- homem(X),
              X \= Y,
              progenitor(A, X),
              progenitor(A, Y),
              progenitor(B, X),
              progenitor(B, Y).

irma(X, Y):-  mulher(X),
              X \= Y,
              progenitor(A, X),
              progenitor(A, Y),
              progenitor(B, X),
              progenitor(B, Y).

pai(X, Y):- homem(X),
            progenitor(X, Y).

mae(X, Y):- mulher(X),
            progenitor(X, Y).

filho(X, Y):- homem(Y),
              progenitor(Y, X).

filha(X, Y):- mulher(Y),
              progenitor(Y, X).

avo(X, Y):- progenitor(X, W),
            progenitor(W, Y).

primo(X, Y):- \+irmao(X, Y),
              \+irma(X, Y),
              avo(W, X),
              avo(W, Y).

prima_direita(X, Y):- mulher(X),
                      progenitor(A, X),
                      progenitor(B, Y),
                      irmao(A, B).

antepassado(X, Y):- progenitor(X, Y).
antepassado(X, Y):- progenitor(X, A),
                    antepassado(A, Y).

