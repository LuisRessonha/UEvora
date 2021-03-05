#Verifica se um elemento é par
let par x = (x mod 2) = 0;;

#Verifica os elementos da lista que são pares
let rec pares_lista l =
    match l with
    [] -> []
    | n::rl when par n -> n :: pares_lista rl
    | n::rl -> pares_lista rl;;

#filtra a lista segundo um paramentro boleano
let rec filtrar p l =
    match l with
    [] -> []
    | x::rl when p x -> x::filtrar p rl
    | x::rl -> filtrar p rl;;