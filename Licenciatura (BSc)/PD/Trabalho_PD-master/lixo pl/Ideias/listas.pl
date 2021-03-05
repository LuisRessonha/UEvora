

len_listaX([]).
len_listaX(Tail):-
    length(Tail, Len),
    write('length = '), write(Len), nl.

soma_lista([]).
soma_lista([Tail]) :-
    sum_list(Tail,S),
    write('Soma = '), write(S), nl,
    len_listaX(Tail).



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
auxListaX([]).
auxListaX([Head_aux|Tail_aux]) :-
    write(Head_aux), write(' | '), write(Tail_aux), nl,
    soma_lista([Tail_aux]).
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

count_up(Low, High) :-
    between(Low, High, Y),
    Z is Y + Low,
    write(Z), nl.



listaX([], J).
listaX([Head|Tail], J) :-
    nl, write('Lista Direita: '), nl,
    soma_lista([Tail]),

    nl, write('Lista Esquerda: '), nl,
    write(Head), write(' | '), write(Tail), nl,
    plus(K, J, Head),
    plus(L, K, 1),
    write(L), nl,
    

    nl, nl, write('-------------'), nl,
    listaX(Tail, L).


%  nl, write('------ Erro 1 -------'), nl,
/*
append([], listaX, A),
    auxListaX(A),

*/