(* FiniteAutomata module interface *)
(* LAP (AMD 2019) *)

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

val getStates : fAutomaton -> state list
val getAlphabet : fAutomaton -> symbol list
val gcut : state -> transitions -> transitions * transitions
val determinism : fAutomaton -> bool
val reachable: fAutomaton -> states
val productive: fAutomaton -> states
val accept : word -> fAutomaton -> bool     (* pre: fa is deterministic *)
val generate : int -> fAutomaton -> word list
val accept2 : word -> fAutomaton -> bool
