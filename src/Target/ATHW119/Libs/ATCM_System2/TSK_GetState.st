(*
 * ATCMControl Embedded - System Library
 * ----------------------------------------------------------------------------
 *
 
 *)

(*[[
    UserURL= "ATCMfile:TSK_GetState.htm"
]]*)

(* TSK_GetState
 * ----------------------------------------------------------------------------
 * Gets the task state of the corresponding task.
 *
 * in   sTask       Task name.
 * out  ulState     Task state. (See System2.cst for possible values.)
 * ret              OK if successful else error number (see ErrNo.cst).
 *)
FUNCTION TSK_GetState : WORD

VAR_INPUT
    sTask   : STRING;
END_VAR

VAR_OUTPUT
    ulState : DINT;
END_VAR


inline{STDLIB(TSK_GetState)}

END_FUNCTION
