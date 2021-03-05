% Recebe o tamanho de uma linha e o board
% E retorna uma lista com o board "mapeado"
% Em linhas.
rows(_, [], []).
rows(RowLength, Board, [Row|T]) :-
    append(Row, NGrid, Board),
    length(Row, RowLength),
    rows(RowLength, NGrid, T).

% Predicado auxiliar ao columns e cols.
% Percorre o index desejado em todas as linhas
% De forma a construir a coluna
oneCol(_, [], []).
oneCol(I, [Row|Rows], [H|T]) :-
    nth1(I,Row,H),
    oneCol(I, Rows, T).

% Utiliza o predicado oneCol para construir todas
% vai repetir este processo até que o número de colunas feitas
% seja igual ao tamanho de uma linha
allCols(RowLength, RowLength, _, []).
allCols(RowLength, I, Rows, [Col|T]) :-
    Ni is I + 1,
    oneCol(Ni, Rows, Col),
    allCols(RowLength, Ni, Rows, T).

% Recebe o tamanho de uma linha e o conjunto de linhas do Board
% Retornando o conjunto de colunas, criado apartir das linhas
columns(Size, Rows, Cols) :- 
    allCols(Size, 0, Rows, Cols).

valid([],[]).
valid([Ch|Ct], [Lh|Lt]) :-
    % Verifica se a linha/coluna cumpre a restrição
    followsRules(Ch, Lh),
    valid(Ct,Lt).

followsRules([], []).
followsRules(R, [Lh|Lt]) :- 
    Lh = 0,
    followsRules(R, Lt).
followsRules([Rh|Rt], L) :- 
    % Irá pintar as casas do tabuleiro de acordo com a restricao
    sqtFill(Rh, L, Ls),
    followsRules(Rt, Ls).

% Recebe a restrição da respetiva linha ou coluna
% Juntamente com a linha ou coluna inicial
% E retorna a linha ou coluna após estar pintada
sqtFill(0,[],[]).
sqtFill(0,[Lh|Lt], L) :-
    Lh = 0,
    L = Lt.
sqtFill(R,[Lh|Lt], L) :-
    % Irá pintar casas no tabuleiro até que o número de 
    % quadrados por pintar na restrição seja igual a zero
    Nr is R - 1,
    Lh = 1,
    sqtFill(Nr, Lt, L).

% Print da solucao
stringify([], []).
stringify([1|T], [H|T2]) :-
    H = "X",
    stringify(T,T2).
stringify([0|T], [H|T2]) :-
    H = ".",
    stringify(T,T2).

formatRow([]) :-
    format("\n", []).
formatRow([H|T]) :-
    format("%c ", H),
    formatRow(T).

formatRows([]).
formatRows([Row|T1]) :-
    stringify(Row, SRow),
    formatRow(SRow),
    formatRows(T1).

% Predicado "main"
puzzle([RowsConstraints,ColumnsConstraints| []]) :-
    % O comprimento de uma coluna é o número de linhas
    % E o mesmo raciocionio para as linhas
    length(RowsConstraints, ColumnLength),
    length(ColumnsConstraints, RowLength),
    BoardSize is ColumnLength * RowLength,
    % Cria um "tabuleiro" com todas as posicoes necessarias
    length(Board, BoardSize),

    rows(RowLength, Board, Rows),
    columns(RowLength, Rows, Cols),

    % Onde o tabuleiro irá começar a ser pintado
    valid(RowsConstraints, Rows),
    valid(ColumnsConstraints, Cols),

    format("\n", []),
    formatRows(Rows).
