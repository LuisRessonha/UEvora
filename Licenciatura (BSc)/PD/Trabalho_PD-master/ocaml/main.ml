let pista = [[[1;2;3];[4;5;6]];[[1;2];[3;4]]];;

(*Devolve a lista das linhas*)
let lines = function 
  | [] -> []
  | h::t -> h;;

let arr_lines = Array.of_list (lines pista);; (* Converte a lista para array *)

(*Devolve a lista das colunas*)
let rec columns = function 
  | [] -> []
  | h::[] -> h
  | h::t -> columns t;;

let arr_columns = Array.of_list (columns pista);; (* Converte a lista para array *)

(* Tamanho do tabuleiro *)
let n_lines = Array.length arr_lines;;
let n_columns = Array.length arr_columns;;

let rec soma_pistas = function
  | [] -> -1
  | h::t -> h + 1 + soma_pistas t;;

let pos = 0;;
let soma_lines = soma_pistas arr_lines.(pos);;

let sobre soma n_columns = 
  if soma_lines > (n_columns) then true
  else false;;

let tem_sobre = sobre soma_lines n_columns;;


(*   

let sobre soma =
  if  soma > ((size (columns pista))/2) then true
  else false

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
