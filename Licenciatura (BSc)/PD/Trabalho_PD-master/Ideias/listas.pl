len_listaX([]).
len_listaX(L):-
    length(L, Len),
    write('len= '), write(Len), nl.

soma_lista([]).
soma_lista([H|T]) :-
    write(H), nl,
    nl, write('------ Erro 1 -------'), nl,
    sum_list(T,S),
    nl, write('------ Erro 2 -------'), nl,
    write('Sum = '), write(S), nl,
    len_listaX(T).


auxListaX([]).
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5
auxListaX(L) :-
    append(B, L, B),
    nl, write('Lista Esquerda: '), nl,
    write(B), write(' | '), write(B), nl,
    soma_lista(B).

listaX([]).
listaX([Head|Tail]) :-
    nl, write('Lista Direita: '), nl,
    soma_lista([Head|Tail]),
    auxListaX(Head), %%%%%%%%%%%

    nl, nl, write('-------------'), nl,
    listaX(Tail).


%  nl, write('------ Erro 1 -------'), nl,