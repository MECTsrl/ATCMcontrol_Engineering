(*[[
    UserURL= "ATCMFile:SignalError.htm"
]]*)
(* 
 *  ATCMControl Standard Functionblock SignalError
 *  Purpose: Send Task to Error State
 *
 *  if enable=TRUE and errno<>0 then
 *    the current task is stopped.
 *)
FUNCTION_BLOCK SignalError

VAR_INPUT
    ENABLE     : BOOL:=TRUE;  (* enable this inst *)
    ERRNO      : DINT:=0;     (* error number *)
END_VAR

IF ENABLE AND ERRNO<>0 THEN
;(* add target specific implementaion here *)
END_IF;

END_FUNCTION_BLOCK
