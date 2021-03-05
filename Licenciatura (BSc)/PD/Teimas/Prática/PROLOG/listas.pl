% -----------------------------------Listas----------------------------
/* Aula 2019.10.08-09 Parte B*/
% lista tem duas partes [CABEÇA|CAUDA]
% caso base
lista([]).
% caso recursivo
lista([_|L]):- lista(L).

membro(X, [X|_]).
membro(X, [_, L]):- membro(X, L).

% uma lista vazia é prefixo de qql outra
prefixo([], _).
% uma lista A é prefixo de uma lista B se a cabeça de A for a msm que B e a cauda de A for prefixo da cuda de B
prefixo([X|A], [X,B]):- prefixo(A, B).
% qualquer lista é sufixo dela mesma
sufixo(A, A).
% Uma lista A é sufixo de B se A for sufixo da cauda de B
sufixo(A, [_|B]):- sufixo(A, B).
% uma sublista é um prefixo de um sufixo e vice-versa
sublista(S, L):- prefixo(P, L), sufixo(S, P).
% catenar a lista A com uma vazia resulta em A
catena([], A, A).
% catenar uma lista que começa com X
catena([X|Xt], L, [X|Y]):- catena(Xt, L, Y).
% definir o ultimo elemento de uma lista
ultimo(X, Y):- catena(_, [X], Y).

% Naive reverse
% se a lista for vazia, tmb é a inversa
nrev([], []).
% se a lista começar por X, invertemos a cauda da lista e ao resulato catenamos a lista X
nrev([X|A], B):- nrev(A, AR), catena(AR, [X], B).

% Reverse Com Acumulador
rev(L, R):- rev(L, [], R).
rev([], R, R).
rev([A|B], X, R):- rev(B, [A|X], R).

% contagem de elementos da lista
compr([], 0).
compr([_|T], X):- compr(T, Y), X is Y+1.

% remover elemento da lista especificando o elemento, a lista e retornando a lista
sel(E, [E|L], L).
sel(E, [X|L], [X|M]):- sel(E, L, M).

% Soma de Uma lista com recursiva terminal
somalista(L, SOMA):- somalista(L, 0, SOMA).
somalista([], SI, SI).
somalista([HEAD|RESTO], St,  SOMA):- S is St+HEAD,
                                     somalista(RESTO, S, SOMA).

% permutacao (Não percebo bem)
perm([], []).
perm(L, [X|LP]):- sel(X, L, LX),
                  perm(LX, LP).

% verifica ordenação
ord([]).
ord([_]).
ord([A|B], X):- A<B, ord([B|X]).

% insertion sort com acumulacao
isort(I, S):- isort(I, [], S).
isort([], S, S).
isort([X|Xs], SI, SO):- insord(X, SI, SX),
isort(Xs, SX, SO).

insord(X, [], [X]).
insord(X, [A|As], [X, A|As]):- X=<A.
insord(X, [A|As], [A|AAs]):- X>A, insord(X, As, AAs).
