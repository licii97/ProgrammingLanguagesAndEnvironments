(* FiniteAutomata module body *)

(* 
AMD SOLUTION

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
            ("SUCCESS",'e',"UNPRODUTIVE"); ("UNPRODUTIVE",'a',"UNPRODUTIVE")
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


(* PUBLIC FUNCTIONS *)

(* --- *)
let getAlphabet fa =
    canonical (List.map (fun (_,symb,_) -> symb) fa.transitions)
;;


(* --- *)
let getStates fa =
    canonical (
        fa.initialState
        :: List.map (fun (s,_,_) -> s) fa.transitions
        @ List.map (fun (_,_,s) -> s) fa.transitions
        @ fa.acceptStates
    )
;;


(* --- *)
let gcut s ts =
    List.partition (fun (a,b,c) -> a = s) ts
;;


(* --- *)
let stateAndSymbols fa =
    List.map (fun (s,symb,_) -> (s,symb)) fa.transitions
;;

let noRepetition l =
    List.length l = List.length (canonical l)
;;

let determinism fa =
    noRepetition (stateAndSymbols fa)
;;


(* --- *)
let rec reachableX s ts =
    let (x, xs) = gcut s ts in
        s::flatMap (fun (_,_,s) -> reachableX s xs) x
;;            

let reachable fa =
    canonical (reachableX fa.initialState fa.transitions)
;;

(* alternative for reachableX

let rec reachableX s g =
    let (x, xs) = gcut s g in
        s::lreachableX x xs
and 
    lreachableX l gl =
        match l with
            [] -> []
          | (_,_,s2)::ts -> reachableX s2 gl  @  lreachableX ts gl
;;
*)


(* --- *)
let rec intersection l1 l2 =
    List.filter (fun x -> List.mem x l2) l1
;;

let productive fa =
    let allStates = getStates fa in
        let reachAccepted s = intersection fa.acceptStates (reachableX s fa.transitions) <> [] in
            List.filter reachAccepted allStates
;;


(* --- *)
let transitionsFor (s,symb) fa =
    List.filter (fun (s1,symb1,_) -> (s,symb) = (s1,symb1)) fa.transitions
;;

let rec acceptX s w fa =
    match w with
          [] ->
            List.mem s fa.acceptStates
        | x::xs ->
            match transitionsFor (s,x) fa with
                   [] -> false
                 | (_,_,s)::_ -> acceptX s xs fa
;;

let accept w fa =
    acceptX fa.initialState w fa
;;


(* --- *)
let rec generateX n s ts a =
    if n = 0 then
        if List.mem s a then [[]] else []
    else
        let (x,_) = gcut s ts in
            flatMap (fun (_,symb,s) -> addAll symb (generateX (n-1) s ts a)) x
;;

let generate n fa =
    canonical (generateX n fa.initialState fa.transitions fa.acceptStates)
;;


(* --- *)
let rec accept2X s w fa =
    match w with
          [] ->
            List.mem s fa.acceptStates
        | x::xs ->
            let l = transitionsFor (s,x) fa in
                List.exists (fun (_,_,s) -> accept2X s xs fa) l
;;

let accept2 w fa =
    accept2X fa.initialState w fa
;;



