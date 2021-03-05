#Soma os elementos de uma lista
let rec soma list =
    match list with
    [] -> 0
    | h::t -> h + soma t;;

#multiplica os elementos de uma lista
let rec produto list =
    match list with
    []->1
    | h::t -> h * produto t;;

#Reduz
let rec reduz f i l =
    match l with
    [] -> i
    | h::t -> reduz f (f i h) t;;


#Append Martelado
let rec listAppend l1 l2 =
    match l2 with
    [] -> []
    | h::rl -> h::(listAppend rl l1);;

#Verifica se X existe na lista
let rec membro list x =
    match list with
    [] -> false
    | h::rl -> if x=h then true else membro rl x;;

#FALTA O REMOVE

#Aplica a funcao f aos elementos da lista l
let rec map f l =
    match l with
    [] -> []
    | x::rl -> (f x)::(map f rl);;

let rec append l1 l2 =
    match l2 with
    [] -> []
    | h::rl -> l1::(append h rl);;

