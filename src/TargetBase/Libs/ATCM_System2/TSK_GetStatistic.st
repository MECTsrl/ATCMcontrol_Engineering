(*
 * ATCMControl Embedded - System Library
 * ----------------------------------------------------------------------------
 *
 
 *)

(*[[
    UserURL= "ATCMfile:TSK_GetStatistic.htm"
]]*)

(* TSK_GetStatistic
 * ----------------------------------------------------------------------------
 * Gets statistic information of the corresponding task.
 *
 * in   sTask       Task name.
 * out  TaskStat    Task statistics of corresponding task.
 * ret              OK if successful else error number (see ErrNo.cst).
 *)
FUNCTION TSK_GetStatistic : WORD

VAR_INPUT
    sTask    : STRING;
END_VAR

VAR_OUTPUT
    TaskStat : TSK_Stat;
END_VAR


inline{STDLIB(TSK_GetStatistic)}

END_FUNCTION
