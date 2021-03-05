/***
          Problem statement:          Solution:
                                       1 2 3 4 5 6 7 8
          |_|_|_|_|_|_|_|_| 3       1 |_|X|X|X|_|_|_|_| 3           
          |_|_|_|_|_|_|_|_| 2 1     2 |X|X|_|X|_|_|_|_| 2 1         
          |_|_|_|_|_|_|_|_| 3 2     3 |_|X|X|X|_|_|X|X| 3 2         
          |_|_|_|_|_|_|_|_| 2 2     4 |_|_|X|X|_|_|X|X| 2 2         
          |_|_|_|_|_|_|_|_| 6       5 |_|_|X|X|X|X|X|X| 6           
          |_|_|_|_|_|_|_|_| 1 5     6 |X|_|X|X|X|X|X|_| 1 5         
          |_|_|_|_|_|_|_|_| 6       7 |X|X|X|X|X|X|_|_| 6           
          |_|_|_|_|_|_|_|_| 1       8 |_|_|_|_|X|_|_|_| 1           
          |_|_|_|_|_|_|_|_| 2       9 |_|_|_|X|X|_|_|_| 2           
           1 3 1 7 5 3 4 3             1 3 1 7 5 3 4 3
           2 1 5 1                     2 1 5 1                      

   This problem can be stated as the two lists:
   [[3],[2,1],[3,2],[2,2],[6],[1,5],[6],[1],[2]] and 
   [[1,2],[3,1],[1,5],[7,1],[5],[3],[4],[3]] which give the
   "solid" lengths of the rows and columns, top-to-bottom and
   left-to-right, respectively.
***/

/* On backtracking, finds and displays all solutions to problem N */
nonogram(N):-
    problem(N, Rows, Cols),
    length(Rows, Height),
    length(Cols, Width),
    initialize_domains(Rows, Width, RowVDs),
    initialize_domains(Cols, Height, ColVDs),
    forward_checking(RowVDs, ColVDs, RowVVs, _),
    sort(RowVVs, SortedRowVVs),
    show_solution(SortedRowVVs, Width),
    fail.
  nonogram(_).
  
  /* Initializes the domain for each variable */
  /* e.g. initialize_domains([[3],[2,1]], 5, VDs),
          VDs=[vd(1, [[1,2,3],[2,3,4],[3,4,5]]), 
               vd(2, [[1,2,4],[1,2,5],[2,3,5]])].       */
  initialize_domains(Rules, Size, VDs):-
    initialize_domains_1(Rules, Size, 1, VDs).
   
  initialize_domains_1([], _, _, []).
  initialize_domains_1([Rule|Rules], Size, Var, [vd(Var,Domain)|VDs]):-
    findall(Pattern, pattern_nd(Rule, Size, Pattern), Domain),
    Var1 is Var + 1,
    initialize_domains_1(Rules, Size, Var1, VDs).
  
  /* Generates a Pattern corresponding to the Rule */
  /* e.g. pattern_nd([2,3,4], 12, Pattern). */
  pattern_nd(Rule, Size, Pattern):-
    sum(Rule, Sum),
    length(Rule, Length),
    Sum1 is Sum + Length - 1,
    pattern_nd_1(Rule, Sum1, 1, Size, [], Pattern).
  
  pattern_nd_1([0], _, _, _, _, [0]):-!.
  pattern_nd_1([N], _, LowerBound, Size, Pattern0, Pattern):-!,
    UpperBound is Size - N + 1,
    for(LowerBound, UpperBound, First),
    Last is First + N - 1,
    integers(First, Last, Pattern1),
    append(Pattern0, Pattern1, Pattern).
  pattern_nd_1([N|Rule], Sum, LowerBound, Size, Pattern0, Pattern):-
    UpperBound is Size - Sum + 1,
    for(LowerBound, UpperBound, First),
    Last is First + N - 1,
    integers(First, Last, Pattern1),
    Sum1 is Sum - N - 1,
    LowerBound1 is Last + 2,
    append(Pattern0, Pattern1, Pattern2),
    pattern_nd_1(Rule, Sum1, LowerBound1, Size, Pattern2, Pattern).
  
  /* forward_checking(RowVDs, ColVDs, RowVVs, ColVVs) is true if for each    */
  /*   element vd(Xi,Di) of the list RowVDs the corresponding element of the */
  /*   list RowVVs is vv(Xi,Vi) where Vi is an element of Di, such that the  */
  /*   elements of RowVVs satisfy the constraints (and similarly for the     */
  /*   lists ColVDs and ColVVs). On backtracking, all solutions will be      */
  /*   found.                                                                */
  forward_checking([], [], [], []):-!.
  forward_checking([], ColVDs, RowVVs, ColVVs):-
    forward_checking(ColVDs, [], ColVVs, RowVVs).
  forward_checking([RowVD|RowVDs], ColVDs, [vv(Xi,Vi)|RowVVs], ColVVs):-
    shortest_and_rest(RowVDs, RowVD, vd(Xi,Di), RowVDs1),
    member(Vi, Di),
    update_domains(ColVDs, vv(Xi,Vi), ColVDs1),
    forward_checking(ColVDs1, RowVDs1, ColVVs, RowVVs).
  
  /* shortest_and_rest(VDs, VD, Shortest, Rest) is true if Shortest is the   */
  /*   shortest element of [VD|VDs] and Rest contains the other elements of  */
  /*   [VD|VDs] (not necessarily in the same order).                         */
  shortest_and_rest([], Shortest, Shortest, []).
  shortest_and_rest([vd(Xh,Dh)|VDs], vd(Xi,Di), Shortest, [vd(Xi,Di)|Rest]):-
    is_shorter(Dh, Di), !,
    shortest_and_rest(VDs, vd(Xh,Dh), Shortest, Rest).
  shortest_and_rest([VD|VDs], BestSoFar, Shortest, [VD|Rest]):-
    shortest_and_rest(VDs, BestSoFar, Shortest, Rest).
  
  /* update_domains(VDs0, vv(Xi,Vi), VDs) is true if: each element vd(Xh,Dk) */
  /*   of VDs is the same as the corresponding element vd(Xh,Dh) of VDs0     */
  /*   except that each value Vh is removed if vv(Xi,Vi) and vv(Xh,Vh) are   */
  /*   not consistent; and no domain Dk is empty.                            */
  /* e.g. update_domains([vd(2,[1,2,3,4]),vd(3,[1,2,3,4]),vd(4,[1,2,3,4])],  */
  /*                     vv(1,1),                                            */
  /*                     [vd(2,[3,4]),vd(3,[2,4]),vd(4,[2,3])]).             */
  update_domains([], _, []).
  update_domains([vd(Xh,Dh)|VDs0], VV, [vd(Xh,Dk)|VDs]):-
    update_domain(Dh, Xh, VV, Dk),
    Dk=[_|_],
    update_domains(VDs0, VV, VDs).
  
  /* update_domain(Dh, Xh, vv(Xi,Vi), Dk) is true if the domain Dk is the    */
  /*   same as the domain Dh except that each value Vh is removed if         */
  /*   vv(Xi,Vi) and vv(Xh,Vh) are not consistent.                           */
  update_domain([], _, _, []).
  update_domain([Vh|Dh], Xh, VVi, Dk):-
    /* \+... helps to avoid heap overflow but is slightly slower */
    /*   than putting the test in the next clause                   */
    \+ consistent(vv(Xh,Vh), VVi), !,
    update_domain(Dh, Xh, VVi, Dk).
  update_domain([Vh|Dh], Xh, VVi, [Vh|Dk]):-
    update_domain(Dh, Xh, VVi, Dk).
  
  /* consistent(vv(RowNo,Row), vv(ColumnNo,Column)) is true if the Row       */
  /*   placed at RowNo is consistent with the Column placed at ColumnNo.     */
  /* e.g. consistent(vv(2,[1,2,3]), vv(1,[2])).                              */
  consistent(vv(RowNo,Row), vv(ColumnNo,Column)):-
    ord_member(ColumnNo, Row), !,
    ord_member(RowNo, Column).
  consistent(vv(RowNo,_), vv(_,Column)):-
    \+ ord_member(RowNo, Column).
  
  /* Used by insertion_sort/2 */
  lt(vv(X,_), vv(Y,_)):-X < Y.
  
  /* Displays a solution */
  show_solution([], _):-nl.
  show_solution([vv(_,Vals)|VVs], Width):-
    show_row(1, Width, Vals), nl,
    show_solution(VVs, Width).
  
  /* Displays a row of the solution */
  show_row(N, Width, _):-
    N > Width, !.
  show_row(N, Width, [N|Vals]):-!,
    write('#'),
    N1 is N + 1,
    show_row(N1, Width, Vals).
  show_row(N, Width, Vals):-
    write('-'),
    N1 is N + 1,
    show_row(N1, Width, Vals).

/* 9x8  1 solution: */
problem(1, [[3],[2,1],[3,2],[2,2],[6],[1,5],[6],[1],[2]],
           [[1,2],[3,1],[1,5],[7,1],[5],[3],[4],[3]]).