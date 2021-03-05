%  Maximo divisor comum

mdc(A, A, C) :-
    C=A.

mdc(A, B, C) :-
    B>A,
    X is B-A,
    mdc(A, X, C).

mdc(A, B, C) :-
    A>B,
    X is A-B,
    mdc(X, B, C).