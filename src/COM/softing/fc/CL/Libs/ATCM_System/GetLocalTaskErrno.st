(*[[
    UserURL= "ATCMFile:GetLocalTaskErrno.htm"
]]*)
(* 
 *  ATCMControl Standard Function GetLocalTaskErrno
 *  Purpose: get the task error number of the named task.
 *           Return values <>0 signal an error. 
 *           See also the FC_ERRNO_* values 
 *           defined in ATCM_System.cst. 
 *)
FUNCTION GetLocalTaskErrno : DINT
VAR_INPUT
    NAME : ANY_STRING;
END_VAR

overload (STRING)  inline {(* add target specific implementaion here *)}
overload (WSTRING) inline {(* add target specific implementaion here *)}

END_FUNCTION
