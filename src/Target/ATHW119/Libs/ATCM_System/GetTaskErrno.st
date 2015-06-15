(*[[
    UserURL= "ATCMfile:GetTaskErrno.htm"
]]*)
(* 
 *  ATCMControl Standard Function GetTaskErrno
 *  Purpose: returns the last error number of the 
 *           calling task. 
 *           Return values <>0 signal an error. 
 *           See also the FC_ERRNO_* values 
 *           defined in ATCM_System.cst. 
 *)
FUNCTION GetTaskErrno : DINT

inline{STDLIB(GetTaskErrno)}

END_FUNCTION
