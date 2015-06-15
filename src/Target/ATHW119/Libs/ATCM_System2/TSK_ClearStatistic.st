(*
 * ATCMControl Embedded - System Library
 * ----------------------------------------------------------------------------
 *
 
 *)

(*[[
    UserURL= "ATCMfile:TSK_ClearStatistic.htm"
]]*)

(* TSK_ClearStatistic
 * ----------------------------------------------------------------------------
 * Clears the statistic information of the corresponding task.
 *
 * in   sTask       Task name.
 * ret              OK if successful else error number (see ErrNo.cst).
 *)
FUNCTION TSK_ClearStatistic : WORD

VAR_INPUT
    sTask    : STRING;
END_VAR

inline{STDLIB(TSK_ClearStatistic)}

END_FUNCTION
