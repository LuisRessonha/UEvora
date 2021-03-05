
(*Junta 2 listas *)
let rec append l1 l2 =
    match l1 with
    [] -> l2
    | h::rl -> h::append rl l2;;

(*Verifica se x existe em l*)
let rec membro l x =
    match l with
    [] -> false
    | h::rl -> if h=x then true else membro rl x;;

(*Conta as ocorrencias de x em l*)
let rec conta x l =
    match l with
    [] -> 0
    | h::rl -> if h=x then 1 + conta x rl else conta x rl;;

(*Remove os elementos x de l*)
let rec remove x l =
    match l with
    [] -> []
    | h::rl -> if x=h then remove x rl else h::(remove x rl);;

(*Conta as ocorrencias de cada elemento*)
let rec contagem = function
    [] -> []
    | h::rl -> (h, (conta h rl)+1)::(contagem(remove h rl));;

type numeral = | Z
               | S of numeral;;

(*Converte numerais em inteiros*)
let rec valor = function
    | Z -> 0
    | S(n) -> 1 + valor n;;

(*Converte inteiros em numerais*)
let rec numeral x =
    match x with
    0 -> Z;
    | x -> S(numeral (x-1));;

(*Soma 2 numerais dados e retorna a soma em numeral *)
let soma n1 n2 = numeral((+) (valor n1) (valor n2));;

(*Aplica o primeiro argumento ao segundo*)
let apl1 = function x -> function y -> x y;;

(*Aplica o primeiro argumento ao segundo 2 vezes*)
let apl2 = function x -> function y -> x (x y);;

(*Aplica o segundo argumento N vezes ao terceiro argumento*)
let rec apl = function n -> function x -> function y -> if n>0 then apl (n-1) x (x y) else x;;


let rec pares = function
    [] -> []
    | h::t -> if (h mod 2) = 0 then h::pares t else pares t;; 

(*tipo abp*)
type 'a abp = N of ('a abp * 'a * 'a abp) | V;;

(*Procura na Arvore*)
let rec lookup n x =
    match n with
    V -> false;
    | N(l, v, r) -> if x=v then true 
                    else if x<v then (lookup l x)
                    else (lookup r x);;

(*Insere na arvore*)
let rec insert n x =
    match n with
    V -> N(V, x, V)
    | N(l, v, r) -> if x=v then N(l, x, r)
                    else if x<v then N((insert l x), v, r)
                    else N(l, v, (insert r x));;

let rec maxE a =
    match a with
    V->0
    | N(V, v, V) -> v
    | N(_,_,r) -> maxE r;;

(*remove da arvore*)
let rec delete n x =
    match n with
    V -> V;
    | N(l, v, r) -> if x<v then N((delete l x), v, r)
                    else if x>e then N(l, v, (delete r x))
                    else match (l,r) with
                     (V, V) -> V
                    |(l, V) -> l
                    |(V, r) -> r
                    |(_,_) -> N(delete l (maxE l), maxE l,r);;

