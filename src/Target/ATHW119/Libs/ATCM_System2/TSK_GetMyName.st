(*
 * ATCMControl Embedded - System Library
 * ----------------------------------------------------------------------------
 *
 
 *)

(*[[
    UserURL= "ATCMfile:TSK_GetMyName.htm"
]]*)

(* TSK_GetMyName
 * ----------------------------------------------------------------------------
 * Gets the name of the calling task.
 *
 * ret              Name of the calling task.
 *)
FUNCTION TSK_GetMyName : STRING

inline{STDLIB(TSK_GetMyName)}

END_FUNCTION
