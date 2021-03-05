type 'a abp = N of ('a abp * 'a * 'a abp) | V;;

let rec lookup = function
    V -> (function _->false)
    | N(l, v, r)->function x->
                    if x=v then true else
                    if x<v then (lookup l x) else (lookup r x);;

let rec insert = function
    V -> (function x->N(V,x,V))
    | N(l, v, r) ->function x-> 
                        if x=v then N(l, x, r) else
                        if x<v then N((insert l x), v, r) else
                                    N(l, v, (insert r x));;