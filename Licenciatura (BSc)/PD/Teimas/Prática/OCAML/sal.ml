type 'a abp = | V | N of 'a abp * 'a * 'a abp;;

let rec insert x = function
    V -> N (V, x, V)
    | N (l, y, r) ->
        if x=y then N (l, y, r)
        else if x < y then N (insert x l, y, r)
        else N (l, y, insert x r);;