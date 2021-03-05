% length(Rows, VSize),
% length(Cols, HSize),
% generate(RowCombos, Rows, HSize),
% generate(ColCombos, Cols, VSize).

%test-->combinate([1,2,3],9,X).


generate(LineCombos, Lines, Size) :- generate_Helper(1, LineCombos, Lines, Size).

generate_Helper(_, [], [], _).
generate_Helper(LineNumber, [line(LineNumber,Combos)|LineCombosTail], [Line|LinesTail], Size) :-
combinate(Line, Size, Combos),
 NextLineNumber is LineNumber + 1,
 generate_Helper(NextLineNumber, LineCombosTail, LinesTail, Size).


combinate(Line,Size,NComboList):-
						getFirst(Line,Size,ComboH,0),
						% bono(ComboH,Line,NComboH),
						length(Line,WhereInLine),
						addTo([ComboH],WhereInLine,Line,Size,ComboList),
						wanda(ComboList,Line,NComboList).
						
						
timmy(X,1,[X]).
timmy(X,N, [X|T]):- N>1,
           N1  is N-1,
           X1 is X+1,
           timmy(X1,N1,T).
           
bono([],[],[]).
bono([H1|T1],[H2|T2],A):- timmy(H1,H2,L1),
                          bono(T1,T2,L2),
                          append(L1,L2,A).
 wanda([],_,[]).
 wanda([H|T],Line,[A1|A2]):-bono(H,Line,A1),
				wanda(T,Line,A2).		
 


						
getFirst([],_,[],_).
 getFirst([LineH|LineT],Size,[ComboHH|ComboTT],X):-
				  ComboHH is X+1,
				  ComboHH =< Size,
				  X1 is LineH+ComboHH,
				  getFirst(LineT,Size,ComboTT,X1).
				  
				      
addTo(ComboList,0,_,_,ComboList).

addTo(ComboListSoFar , WhereInLine,Line,Size,ComboList):-
						  oneLengthCombos(ComboListSoFar,WhereInLine,Line,Size,ComboListSoFar,NewComboListSoFar),
						  WhereInLineNow is WhereInLine - 1,
						  addTo(NewComboListSoFar,WhereInLineNow,Line,Size,ComboList).
						  
						  
oneLengthCombos([],_,_,_,NCL,NCL).

oneLengthCombos([H|T] ,WhereInLine,Line ,Size,SameComboList,NCL ):-
				workingOn(WhereInLine,Line,WhichInLine),
				computeMaximumSize(Size,WhereInLine,Line,H,MaximumSize),
				workingOn(WhereInLine,H,OldPosition),
				NewPosition is OldPosition+1,
				checkIfFits(MaximumSize,NewPosition,WhichInLine),
				newCombo(H,WhereInLine,1,NewPosition,NH),
				append(T,[NH],NewT),
				append(SameComboList,[NH],ComboListModified),
				oneLengthCombos(NewT ,WhereInLine,Line ,Size,ComboListModified,NCL ).
				
				
oneLengthCombos([H|T] ,WhereInLine,Line ,Size,SameComboList,NCL ):-
				workingOn(WhereInLine,Line,WhichInLine),
				computeMaximumSize(Size,WhereInLine,Line,H,MaximumSize),
				workingOn(WhereInLine,H,OldPosition),
				NewPosition is OldPosition+1,
				\+checkIfFits(MaximumSize,NewPosition,WhichInLine),
				oneLengthCombos(T ,WhereInLine,Line ,Size,SameComboList,NCL ).
				
				
workingOn(1,[H|_],H).

workingOn(X,[_|T],Y):-
		      X>1,
		      X1 is X-1,
		      workingOn(X1,T,Y).
		      
		      
computeMaximumSize(Size,WhereInLine,Line,_,Size):-
						    length(Line,WhereInLine).
						    
computeMaximumSize(_,WhereInLine,Line,H,MaximumSize):-
							 \+length(Line,WhereInLine),
							 NWhereInLine is WhereInLine + 1,
							workingOn(NWhereInLine,H,MaximumSizePlus2),
							MaximumSize is MaximumSizePlus2-2.
							
							
workingOnPlus1(2,[H|_],H).

workingOnPlus1(X,[_|T],Y):-
		      X>2,
		      X1 is X-1,
		      workingOn(X1,T,Y).	
							
							
checkIfFits(MaximumSize,NewPosition,WhichInLine):-
						    Tmp is NewPosition + WhichInLine - 1,
						    Tmp =< MaximumSize.
						    
newCombo([_|T],WhereInLine,WhereInLine,NewPosition,[NewPosition|T]).

newCombo([H|T],WhereInLine,X,NewPosition,[H|NewT]):-
						    X < WhereInLine,
						    X1 is X+1,
						    newCombo(T,WhereInLine,X1,NewPosition,NewT).
					
getSolution([], _, []).
getSolution([line(RowNumber, [RowH|_])|RowsTail], Cols, [RowH|ST]) :-
		matchWithHead(RowNumber, RowH, Cols, MatchedCols), MatchedCols \= [], 
		replace(Cols, MatchedCols, ColsWithMatchedCols),
		getSolution(RowsTail, ColsWithMatchedCols, ST).
		
getSolution([line(RowNumber, [_|RowT])|RowsTail], Cols, Solution) :-
		getSolution([line(RowNumber, RowT)|RowsTail], Cols, Solution).


matchWithHead(_, [], _, []).
matchWithHead(LineNumber, [H|Tail], Cols, [MatchColsH|MatchColsT]) :-
									member(line(H, Combs), Cols),
									 hasRowNumber(LineNumber, Combs, SelectedCombs), 
									SelectedCombs \= [],
									MatchColsH = line(H, SelectedCombs),
									matchWithHead(LineNumber, Tail, Cols, MatchColsT).
 
 
hasRowNumber(_, [], []).
 
 hasRowNumber(LineNumber, [CombsH|CombsT], SelectedCombs) :-
							      member(LineNumber,CombsH),
							      SelectedCombs = [CombsH|CTail], 
							      hasRowNumber(LineNumber, CombsT, CTail).
						  
 hasRowNumber(LineNumber, [CombsH|CombsT], SelectedCombs) :-
							      \+member(LineNumber,CombsH),
							      hasRowNumber(LineNumber, CombsT, SelectedCombs).
							     							      
							      
replace(Rest,[],Rest).
replace([line(ColNumber, _)|ColsT],[line(ColNumber, NColT)|MColsT],[line(ColNumber, NColT)|Rest]):-
												      replace(ColsT,MColsT,Rest).
												      
replace([line(ColNumber, ColT)|ColsT],[line(AnotherColNumber, NColT)|MColsT],[line(ColNumber, ColT)|NewCols]):-
											ColNumber \= AnotherColNumber,
											replace(ColsT,[line(AnotherColNumber, NColT)|MColsT],NewCols).
						

  

		
 solve(Rows,Cols,Solution):-
			       length(Rows, VSize),
			       length(Cols, HSize),
			       generate(RowCombos, Rows, HSize),
			       generate(ColCombos, Cols, VSize),
			       getSolution(RowCombos, ColCombos, Solution),
			       printNonogram(Solution).
			       

			    

 
 
 
 
 
 printlist([]).
printlist([H|T]) :- number(H), swritef(S, '~` t#~%w|', [H]), format(S, []), printlist(T).

printNonogram([]).
printNonogram([H|T]):- printlist(H), nl, printNonogram(T).