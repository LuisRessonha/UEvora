(* include Array *)

(* input teste *)
let pista = [[[1;2;3];[4;5;6]; [3]];[[1;2];[3;4]]];;
(* input teste *)


(*Devolve a lista das linhas*)
let lines = function 
  | [] -> []
  | h::t -> h;;

(*Devolve a lista das colunas*)
let rec columns = function 
  | [] -> []
  | h::[] -> h
  | h::t -> columns t;;

(* Converte as listas para array *)
let arr_lines = Array.of_list (lines pista);; 
let arr_columns = Array.of_list (columns pista);;

(* Tamanho do tabuleiro *)
let n_lines = Array.length arr_lines;;
let n_columns = Array.length arr_columns;;

(* somas as pistas de uma determinada linha ou coluna *)
let rec soma_pistas = function
  | [] -> -1
  | h::t -> h + 1 + soma_pistas t;;

(* soma as todas as pista e os espacos entre elas *)
let soma_lines = soma_pistas arr_lines.(0);;

(* verifica se existem sobreposicoes em cada linha *)
let sobreposicao soma n_columns = 
  if soma_lines > (n_columns/2) then 
      true
  else false;;


let tab_linha = Array.make n_columns 5;;
let tabuleiro = Array.make n_lines 6;;

let pos = 0;;
let rec criar_tabuleiro tabuleiro pos = 
  if pos = (n_lines-1) then 
    tabuleiro.(pos) <- tab_linha
  else
    tabuleiro.(pos) <- tab_linha; 
    criar_tabuleiro tabuleiro (pos + 1);;

Array.iter print_int tab_linha;;
Array.iter print_int tabuleiro;;

(*   

(* recebe lines ou columns *)
let rec jogar = function
  | [] -> 0
  | h::t -> sobre(soma_pistas h) ; jogar t;;
 *)


(* 

 let soma_line = soma_pistas lines;;

let soma_columns = soma_pistas columns;;



val make_matrix : n_lines n_columns;;
let tab = make_matrx;; 

*)
