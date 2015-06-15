(*[[
    UserURL= "ATCMFile:EnableTaskStatistic.htm"
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

;(* add target specific implementaion here *)

END_FUNCTION
