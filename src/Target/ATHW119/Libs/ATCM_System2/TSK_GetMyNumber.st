(*
 * ATCMControl Embedded - System Library
 * ----------------------------------------------------------------------------
 *
 
 *)

(*[[
    UserURL= "ATCMfile:TSK_GetMyNumber.htm"
]]*)

(* TSK_GetMyNumber
 * ----------------------------------------------------------------------------
 * Gets the index number of the calling task.
 *
 * ret              Task index number of calling task.
 *)
FUNCTION TSK_GetMyNumber : INT

inline{STDLIB(TSK_GetMyNumber)}

END_FUNCTION
