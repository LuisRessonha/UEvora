guess_num :- loop(start).

loop(15) :- write('You guessed it').

loop(X) :-
    X \= 15,
    write('Guess number '),
    read(Guess),
    write(Guess), 
    write(' is not the number'), nl,
    loop(Guess).