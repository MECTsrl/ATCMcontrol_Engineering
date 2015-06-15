
(*[[
    UserURL= "ATCM:ClearTaskErrno.htm"
]]*)
(* 
 *  ATCMControl Standard Function ClearTaskErrno
 *  Purpose: Clears the error state variable of the 
 *           calling task. Error numbers <>0 signal an
 *           error. 
 *)
FUNCTION ClearTaskErrno

inline{tc.setErrorNumber(0)}

END_FUNCTION
