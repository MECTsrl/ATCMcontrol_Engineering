
(*[[
    UserURL= "ATCM:EnableTaskStatistic.htm"
]]*)
(* 
 *  ATCMControl Standard Function EnableTaskStatistic
 *  Purpose: Enable/Disable the automatic task 
 *  statistic output of all tasks. 
 *  The default after a download or coldstart or 
 *  warmstart is enable=true.
 *  
 *  EnableTaskStatistic(true) : enable  task statistics
 *  EnableTaskStatistic(false): disable task statistics
 *)
FUNCTION EnableTaskStatistic
VAR_INPUT
    ENABLE     : BOOL;  (* enable/disable input *)
END_VAR

inline{tc.enableTaskStatistic(%1)}

END_FUNCTION
