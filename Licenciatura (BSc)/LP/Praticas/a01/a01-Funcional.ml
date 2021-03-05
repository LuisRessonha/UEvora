(*  Maximo divisor comum *)

let rec mdc a b =
    if a=b then a else
          if b>a then (mdc a (b-a)) else (mdc (a-b) b);;