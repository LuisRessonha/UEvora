/*
 * https://gist.github.com/larsmans/1146705
 *
 * Nonogram/paint-by-numbers solver in SWI-Prolog. Uses CLP(FD),
 * in particular the automaton/3 (finite-state/RE) constraint.
 * Copyright 2011, 2014 Lars Buitinck
 * Copyright 2014 Markus Triska
 * Do with this code as you like, but don't remove the copyright notice.
 */

:- use_module(library(clpfd)).

nono(RowSpec, ColSpec, Grid) :-
    maplist(row, RowSpec, Grid),
    transpose(Grid, GridT),
    maplist(row, ColSpec, GridT).

row(Ks, Row) :-
    sum(Ks,  #=, Ones),
    sum(Row, #=, Ones),
    phrase(arcs(Ks, Start, Final), Arcs),
    append(Row, [0], RowZ),
    automaton(RowZ, [source(Start), sink(Final)], [arc(Start,0,Start) | Arcs]).

% List of transition arcs for finite-state constraint.
arcs([], Final, Final) --> [].
arcs([K|Ks], CurState, Final) -->
    ( { K == 0 } ->
        [arc(CurState, 0, CurState), arc(CurState, 0, NextState)],
        arcs(Ks, NextState, Final)
    ;   [arc(CurState, 1, NextState)],
        { K1 #= K-1 },
        arcs([K1|Ks], NextState, Final)
    ).


make_grid(Grid, X, Y, Vars) :-
    length(Grid,X),
    make_rows(Grid, Y, Vars).
make_rows([], _, []).
make_rows([R|Rs], Len, Vars) :-
    length(R, Len),
    make_rows(Rs, Len, Vars0),
    append(R, Vars0, Vars).

print_row([]) :- nl.
print_row([X|R]) :-
    (X == 0 ->
        write(' ')
    ;
        write('x')
    ),
    print_row(R).

example1(23, 17,
                [[1,1], [2,1,2], [2,1,1,2], [5,1,1,5], [4,1,1,5], [3,2,4], [3,4,4],
                [11], [4], [3,4,3], [10], [6,2], [11], [2,6], [10], [2,4,2], [4],
                [2,2], [8], [2,1,1,2], [2,2,2,2], [4,4], [8]],

                [[2], [5,1,1], [7,1,2,1,1], [5,2,1,2,3], [3,2,1,1,1,2,2], [1,5,2,3],
                [2,17], [12,1,1], [12,1,1], [2,17], [1,5,2,3], [2,1,1,1,2,2],
                [6,2,1,2,3], [5,1,1,1,1], [7,1,1], [5], [2]])    .

go :-
    example1(X, Y, Rows, Cols),
    make_grid(Grid, X, Y, Vars),
    nono(Rows, Cols, Grid),
    label(Vars),
    maplist(print_row, Grid).