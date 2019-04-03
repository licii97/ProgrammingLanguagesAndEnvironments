(* FiniteAutomata module body *)

(*
Aluno 1: ????? mandatory to fill
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
            ("AB",'a',"START"); ("AB",'b',"START"); ("AB",'c',"SUCCESS");
                                                    ("AB",'d',"START");
            ("SUCCESS",'a',"SUCCESS"); ("SUCCESS",'b',"SUCCESS");
                         ("SUCCESS",'c',"SUCCESS"); ("SUCCESS",'d',"SUCCESS")
        ];
    acceptStates = ["SUCCESS"]
};;

let abcND = {
    initialState = abc.initialState ;
    transitions = abc.transitions @ [
            ("SUCCESS",'a',"SUCCESS");
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

let gcut s ts =
    filterSplit (fun x -> s = getFirst x) ts
;;

(* an automaton is deterministic IF:
   for each pair of transitions T1=(A,s,B) and T2=(C,t,D) holds:
    if A=B and s=t then B=D *)
let determinism fa =
    false
;;

let reachable fa =
    canonical []
;;

let productive fa =
    canonical []
;;

let accept w fa =
    false
;;

let generate n fa =
    canonical []
;;

let accept2 w fa =
    false
;;
