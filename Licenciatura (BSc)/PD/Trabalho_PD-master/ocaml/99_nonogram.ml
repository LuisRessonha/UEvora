type element = Empty | X (* ensure we do not miss cases in patterns *)
  
  (* Whether [row.(c)] for [col0 ≤ c < col1] are all set to [X]. *)
  let rec is_set_range row col0 col1 =
    col0 >= col1 || (row.(col0) = X && is_set_range row (col0 + 1) col1)
  
  (* Whether all [row.(ncol)] .. [row.(ncol + width - 1)] equal [X]. *)
  let is_set_sub row col0 width =
    col0 + width <= Array.length row
    && is_set_range row col0 (col0 + width)
  
  (* Check that [row.(col0 ..)] conforms the pattern [patt_row]. *)
  let rec check_row row col0 patt_row =
    if col0 >= Array.length row then
      patt_row = [] (* row exhausted, no pattern must remain *)
    else
      match row.(col0) with
      | Empty -> check_row row (col0 + 1) patt_row
      | X ->
         match patt_row with
         | [] -> false
         | nX :: tl ->
            if is_set_sub row col0 nX then
              let col0 = col0 + nX in
              (col0 >= Array.length row || row.(col0) = Empty)
              && check_row row (col0 + 1) tl
            else false
  
  (* Check that each row of the table conforms [patts_row].  It is
     assumed that the length of [patts_row] is equal to the number of
     lines of [table]. *)
  let rec check_rows table row0 patts_row =
    row0 >= Array.length table
    || (match patts_row with
       | patt_row :: tl -> check_row table.(row0) 0 patt_row
                          && check_rows table (row0 + 1) tl
       | [] -> assert false)
  
  let char_of_element = function
    | Empty -> '_'
    | X -> 'X'
  
  let print_tbl table =
    let print_row r =
      Array.iter (fun e -> print_char '|';
                         print_char(char_of_element e)) r;
      print_string "|\n" in
    Array.iter print_row table
  
  let solve patts_row patts_col =
    let height = List.length patts_row
    and width = List.length patts_col in
    let table = Array.make_matrix height width Empty in
    (* Generate all possibilities for columns and filter according
       to row patterns.  [patts_col] are the patterns left for the
       current column. *)
    let rec gen col row patts_col =
      if col >= width then (
        if check_rows table 0 patts_row then
          print_tbl table
      )
      else
        match patts_col with
        | [] :: rest_patt ->
           (* No pattern left for this column, go to the next one. *)
           gen (col + 1) 0 rest_patt
        | (nX :: tl) :: rest_patt ->
           assert(nX > 0);
           if row + nX <= height then (
             for r = row to row + nX - 1 do
               table.(r).(col) <- X
             done;
             gen col (row + nX + 1) (tl :: rest_patt);
             for r = row to row + nX - 1 do
               table.(r).(col) <- Empty
             done;
             (* Try the same pattern from next row: *)
             gen col (row + 1) patts_col;
           )
        | [] -> assert false
    in gen 0 0 patts_col;;