(*[[
    UserURL= "ATCMFile:GetTaskErrno.htm"
]]*)
(* 
 *  ATCMControl Standard Function GetTaskErrno
 *  Purpose: returns the last error number of the 
 *           calling task. 
 *           Return values <>0 signal an error. 
 *           See also the FC_ERRNO_* values 
 *           defined in 4C_System.cst. 
 *)
FUNCTION GetTaskErrno : DINT

;(* add target specific implementaion here *)
GetTaskErrno := 0;

END_FUNCTION
