(*[[
    UserURL= "ATCMfile:GetLocalTaskErrno.htm"
]]*)
(* 
 *  ATCMControl Standard Function GetLocalTaskErrno
 *  Purpose: get the task error number of the named task.
 *           NOTE: name must be given in "lower case".
 *           Return values <>0 signal an error. 
 *           See also the FC_ERRNO_* values 
 *           defined in ATCM_System.cst. 
 *)
FUNCTION GetLocalTaskErrno : DINT
VAR_INPUT
    NAME : ANY_STRING;
END_VAR

overload DINT(STRING)  inline {STDLIB(GetLocalTaskErrno)}

END_FUNCTION
