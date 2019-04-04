(* FiniteAutomata module body *)

(*
Aluno 1: 56554 mandatory to fill
Aluno 2: ????? mandatory to fill

Comment:

?????????????????????????
?????????????????????????
?????????????????????????
?????????????????????????
?????????????????????????
?????????????????????????

*)

(*
01234567890123456789012345678901234567890123456789012345678901234567890123456789
   80 columns
*)

type symbol = char;;       (* our symbols are represented by chars *)
type word = symbol list;;  (* our words are represented by lists of symbols *)

type state = string;;      (* our states are represented by strings *)
type states = state list;;

type transition =
    state   (* state *)
  * symbol  (* consumed input symbol *)
  * state   (* next state *)
;;
type transitions = transition list;;

type fAutomaton = {
    initialState: state;       (* Initial state *)
    transitions: transitions;  (* Transition relation *)
    acceptStates: states       (* Accept states *)
};;


(* PRIVATE DEFINITIONS *)

let abc = {
    initialState = "START" ;
    transitions = [
            ("START",'a',"A"); ("START",'b',"START"); ("START",'c',"START");
                                                      ("START",'d',"START");
            ("A",'a',"A"); ("A",'b',"AB"); ("A",'c',"START"); ("A",'d',"START");
            ("AB",'a',"A"); ("AB",'b',"START"); ("AB",'c',"SUCCESS");
                                                    ("AB",'d',"START");
            ("SUCCESS",'a',"SUCCESS"); ("SUCCESS",'b',"SUCCESS");
                         ("SUCCESS",'c',"SUCCESS"); ("SUCCESS",'d',"SUCCESS")
        ];
    acceptStates = ["SUCCESS"]
};;

let abcND = {
    initialState = abc.initialState ;
    transitions = abc.transitions @ [
            ("SUCCESS",'a',"UNPRODUCTIVE");
            ("UNREACHABLE",'a',"SUCCESS");
            ("SUCCESS",'e',"UNPRODUCTIVE"); ("UNPRODUCTIVE",'a',"UNPRODUCTIVE")
        ];
    acceptStates = abc.acceptStates
};;

let canonical l =
    List.sort_uniq compare l
;;

let addAll symb =
    List.map (fun l -> symb::l)
;;

let flatMap f l =
    List.flatten (List.map f l)
;;

let getFirstAndThird triple = match triple with
                       (x,_,z) -> [x;z]
;;

let getFirst triple = match triple with
                       (x,_,_) -> x
;;

let getSecond triple = match triple with
                       (_,y,_) -> y
;;

let getThird triple = match triple with
                       (_,_,z) -> z
;;

(* split a list into a tuple of lists using predicate p *)
let rec filterSplit p l = match l with
                  [] -> ([],[])
                | x::xs -> let splitList = filterSplit p xs in
                           if p x then (x::(fst splitList),snd splitList)
                           else (fst splitList, x::(snd splitList))
;;





(* PUBLIC FUNCTIONS *)

let getStates fa =
    canonical (fa.initialState::(fa.acceptStates @ (flatMap getFirstAndThird
      fa.transitions)))
;;

let getAlphabet fa =
    canonical (List.map getSecond fa.transitions)
;;

(* creates for a state a tuple of two diffenret lists:
first list includes all transitions, that have s as a start state
second list includes all other transitions *)
let gcut s ts =
    filterSplit (fun x -> s = getFirst x) ts
;;


(*---------------------------
help fuctions for determinism
----------------------------*)

(*checks if a transistion t is determinitic*)
let rec transitionDeterministic t ts = match ts with
                  [] -> true
                | (_,y,z)::xs -> ((not (y = (getSecond t))) || (z =
                                 (getThird t)))
                                 && transitionDeterministic t xs
;;

(*checks if transitions in list ts are deterministic*)
let rec transitionsDeterministic ts = match ts with
                   [] -> true
                 | t::xs -> transitionDeterministic t xs
                            && transitionsDeterministic xs
;;

(*checks if states in l are deterministic with transitions ts*)
let rec statesDeterministic l ts = match l with
                   [] -> true
                 | s::ss -> let tuple = gcut s ts in
                            transitionsDeterministic (fst tuple) &&
                            statesDeterministic ss (snd tuple)
;;


(* an automaton is deterministic IF:
   for each pair of transitions T1=(A,s,B) and T2=(C,t,D) holds:
    if A=B and s=t then B=D *)
let determinism fa =
    statesDeterministic (getStates fa) fa.transitions
;;



(*--------------------------
help functions for reachable
--------------------------*)
let rec addReachables ss ts acc = match ss with
  [] -> acc
  | s::xs -> addReachables xs ts
              (if List.mem s acc
              then acc @ (List.map getThird (fst (gcut s ts)))
              else acc)
;;

let rec reachableLoop ss ts acc =
               if canonical (addReachables ss ts acc) = canonical acc
               then acc
               else reachableLoop ss ts (addReachables ss ts acc)
;;


let reachable fa =
    canonical (reachableLoop (getStates fa) fa.transitions [fa.initialState])
;;


(*---------------------------
Help functions for productive
----------------------------*)
let prodCut s ts =
    filterSplit (fun x -> s = getThird x) ts
;;

let rec addProductives ss ts acc = match ss with
  [] -> acc
  | s::xs -> addProductives xs ts
              (if List.mem s acc
              then acc @ (List.map getFirst (fst (prodCut s ts)))
              else acc)
;;

let rec productiveLoop ss ts acc =
               if canonical (addProductives ss ts acc) = canonical acc
               then acc
               else productiveLoop ss ts (addProductives ss ts acc)
;;

let productive fa =
    canonical (productiveLoop (getStates fa) fa.transitions fa.acceptStates)
;;


(*-----------------------
Help functions for accept
-----------------------*)

(*gives nextState from current state s, first char c of the word and possible
transitions starting from s*)
let rec nextState s c ts =
  match ts with
      [] -> "NO_POSSIBLE_STATE"
      |y::ys -> if getSecond y = c then (getThird y)
              else nextState s c ys
;;


(*when word is empty, test if current state is acceptable*)
let testAcception s fa =
  List.mem s fa.acceptStates
;;

(*takes chars from the word until ir is empty and then tests if state is
acceptable*)
let rec consumeWord s w fa =
  match w with
  [] -> testAcception s fa
  | c::cs -> let newState = nextState s c (fst(gcut s fa.transitions)) in
    if (newState = "NO_POSSIBLE_STATE") then false
              else consumeWord newState cs fa
;;


let accept w fa =
    if w = [] then List.mem fa.initialState fa.acceptStates
      else consumeWord fa.initialState w fa
;;


(*-------------------------
Help functions for generate
-------------------------*)

(*gives all States as a list that are possible to reach from current state s 
with char c*)
let rec getNextStatesAsList s c ts =
  match ts with
      [] -> []
      |y::ys -> if((getFirst y) = s) && ((getSecond y) = c) 
                then [(getThird y)] @ getNextStatesAsList s c ys
                else getNextStatesAsList s c ys
;;

(*gives all chars in a list, that can be chosen from the current state s*)
let rec getChars s l = 
  match l with 
    [] -> []
    | t::ts ->  canonical ([getSecond t] @ getChars s ts)
;;

(*eliminates all transitions out of a given transition list 
that lead to or end in an unproductive or unreachable state*)
let rec eliminateTransitions l prod reach = 
  match l with 
    [] -> []
    (*first and third element of transition tuple has to be productive and reachable*)
    |t::ts -> if ( (List.mem (getFirst t) prod) && (List.mem (getFirst t) reach)
                  && (List.mem (getThird t) prod) && (List.mem (getThird t) reach) )
              then [t] @ eliminateTransitions ts prod reach
              else eliminateTransitions ts prod reach
;;

(*filters all transitions that lead to or end in 
an unproductive or unreachable state*)
let rec filterTransistions fa =
  let prod = productive fa in 
    let reach = reachable fa in 
      let l = fa.transitions in 
        eliminateTransitions l prod reach
;;


(*main generate function, if n>0 at the start with current state s*)
(*cw is current word, ts filtered transactions and acc acceptable states*)
let generateAfterStart s n cw ts acc = [[]]
(*)  if n=0 then List.mem s fa.acceptStates
  else 
    let cs = getChars s in 
      match cs with 
        [] -> []
        |a::as -> nextStatesAsList *)
;; 

let generate n fa =
  if n = 0
    then 
      if (List.mem fa.initialState fa.acceptStates) then [[]] 
      (*result: list with empty word*)
      else [] 
      (*result empty list*)
  else let ts = filterTransistions fa in 
    canonical (generateAfterStart fa.initialState n [] ts fa.acceptStates)
;;


(*------------------------
Help functions for accept2
------------------------*)

let accept2 w fa =
    if determinism fa then accept w fa
      else false;
;;
