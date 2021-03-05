% Nonogram solver by Klemens Schölhorn under GPLv2+
%
% Datastuctures and optimisation inspired by the example solution to number
% 98 of the 99 prolog problems by Werner Hett and Paul Singleton.
% (https://sites.google.com/site/prologsite/prolog-problems)

/**
 * nonogram(+RowConstr:list, +ColConstr:list, -Rows:list) is nondet.
 *
 * Solves the nonogram specified by the given row and column contrains
 * in the following form: [[4],[3,1],[2]] The solutions (if any) are returned
 * row-major, i.e. as a list of row-lists, and are built using ' ' and 'x'.
 */
nonogram(RowConstr, ColConstr, Rows) :-
    length(RowConstr, NRows),
    length(ColConstr, NCols),
    generate_matrix(NRows, NCols, Rows, Cols),
    append(RowConstr, ColConstr, LineConstr),
    append(Rows, Cols, Lines),
    solve(Lines, LineConstr).

%
% Generation of the datastuctures
%

/**
 * generate_matrix(+NRows:int, +NCols:int, -Rows:list, -Cols:list) is det.
 *
 * Generates a NRows x NCols matrix of free variables which can be accessed
 * through both Rows and Cols, which are lists of row-/col-lists. Example:
 *   ?- generate_matrix(2, 3, Rows, Cols).
 *   Rows = [[_G22, _G25, _G28], [_G31, _G34, _G37]],
 *   Cols = [[_G22, _G31], [_G25, _G34], [_G28, _G37]].
 */
generate_matrix(NRows, NCols, Rows, Cols) :-
    generate_rows(NRows, NCols, Rows),
    extract_columns(NCols, Rows, Cols).

/**
 * generate_rows(+NRows:int, +NCols:int, -Rows:list) is det.
 *
 * Generates a NRows x NCols matrix of free variables as a list of row-lists.
 */
generate_rows(NRows, NCols, Rows) :-
    length(Rows, NRows),
    CreateRowNCols =.. [create_row, NCols],
    maplist(CreateRowNCols, Rows).
create_row(NCols,Col) :-
    length(Col,NCols).

/**
 * extract_columns(+NCols:int, +Rows:list, -Cols:list) is det.
 *
 * Extracts the columns from the given Rows. Free variables are not bound
 * or changed, i.e. you can access the same variables through Rows and Cols.
 */
extract_columns(NCols, Rows, Cols) :-
    NCols1 is NCols + 1,
    extract_columns(NCols, NCols1, Rows, Cols).
extract_columns(0, _, _, []) :- !.
extract_columns(N, Max, Rows, [Col|Cols]) :-
    InvN is Max - N,
    generate_column(Rows, Col, InvN),
    N1 is N - 1,
    extract_columns(N1, Max, Rows, Cols).
generate_column(Rows, Col, N) :-
    % inspired by http://stackoverflow.com/a/5808507
    Check =.. [nth1, N],
    maplist(Check, Rows, Col).

%
% Nonogram solving algorithm
%

/**
 * solve(+Lines:list, +Constrs:list) is nondet.
 *
 * Solves the nonogram given through the lines and their constraints.
 * Uses an optimized algorithm that solves lines with few possibilities first.
 */
solve(Lines, Constrs) :-
    pack(Lines, Constrs, Pack),
    sort(Pack, SortedPack),
    solve(SortedPack).

solve([]).
solve([line(_, Line, Constr)|Rest]) :-
    check_line(Line, Constr),
    solve(Rest).

/**
 * pack(+Lines:list, +Constrs:list, -Result:list) is det.
 *
 * Packs a line and its constraints into a single term and adds the number of
 * possible line solutions given the line's length and constraints as the term's
 * first argument to enable sorting.
 */
pack([], [], []).
pack([Line|Lines], [Constr|Constrs], [line(Count, Line, Constr)|Result]) :-
    length(Line, LineLength),
    length(CheckLine, LineLength),
    findall(CheckLine, check_line(CheckLine, Constr), NCheckLine),
    length(NCheckLine, Count),
    pack(Lines, Constrs, Result).

/**
 * check_line(+Line:list ,+Constraints:list) is nondet.
 *
 * Checks if the given Line satisfies the Constraints. Can also generate all
 * valid lines if given a line with some or all members unbound. Examples:
 *   ?- check_line([x, ' ', x, ' '], [1,1]).
 *   true .
 *   ?- L = [_,_,_,_,_], check_line(L, [2,1]).
 *   L = [x, x, ' ', x, ' '] ;
 *   L = [x, x, ' ', ' ', x] ;
 *   L = [' ', x, x, ' ', x] .
 */
check_line([],[]) :- !.
check_line(Line, [Part|Rest]) :-
    Rest \= [],
    add_space(Line, Line2),
    check_part(Line2, Line3, Part),
    force_space(Line3, Line4),
    check_line(Line4, Rest).
check_line(Line, [Part|[]]) :-
    add_space(Line, Line2),
    check_part(Line2, Line3, Part),
    add_space(Line3, Line4),
    check_line(Line4, []).

force_space([' '|Line],Line).

add_space(Line, Line).
add_space([' '|Line],RestLine) :-
    add_space(Line, RestLine).

check_part(Line, Line, 0).
check_part(['x'|Line], RestLine, N) :-
    N > 0,
    N1 is N - 1,
    check_part(Line, RestLine, N1).

%
% Functions to produce a nice output
%

print_solution([]).
print_solution([Row|Rows]) :-
    print(' '),
    print_line(Row),
    nl,
    print_solution(Rows).

print_line([]).
print_line([Var|Vars]) :-
    print_var(Var),
    print(' '),
    print_line(Vars).

print_var(' ') :- print(' ').
print_var('x') :- print('*').