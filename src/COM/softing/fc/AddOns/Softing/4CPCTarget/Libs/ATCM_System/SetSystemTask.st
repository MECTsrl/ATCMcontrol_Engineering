
(*[[
    UserURL= "ATCM:SetSystemTask.htm"
]]*)
(* 
 *  ATCMControl Standard Function SetSystemTask
 *  Purpose: Declare this task to a system task.
 *           To be effective, this has to be the first
 *           statement of the program.
 *           System tasks are tasks that control
 *           other tasks via Start/StopLocalTask.
 *           A System Task will not be checked
 *           for dead-locks, so USE THIS CAREFULLY!
 *)
FUNCTION SetSystemTask

inline{tc.setSystemTask()}

END_FUNCTION
