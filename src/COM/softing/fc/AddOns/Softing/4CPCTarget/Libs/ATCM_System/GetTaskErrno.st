
(*[[
    UserURL= "ATCM:GetTaskErrno.htm"
]]*)
(* 
 *  ATCMControl Standard Function GetTaskErrno
 *  Purpose: returns the last error number of the 
 *           calling task. Error numbers <>0 signal an
 *           error. 
 *)
FUNCTION GetTaskErrno : DINT

inline{tc.getErrorNumber()}

END_FUNCTION
