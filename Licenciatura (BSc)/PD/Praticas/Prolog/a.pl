cust(tom, smith, 20.55).
cust(sally, smith, 120.55).

get_cust_bal(FName, LName) :-
    cust(FName, LName, Bal),
    % write(FName), tab(1),
    format('~w ~w owes us $~2f ~n', [FName, LName, Bal]).