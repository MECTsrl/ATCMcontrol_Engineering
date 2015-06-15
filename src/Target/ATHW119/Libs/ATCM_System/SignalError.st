(*[[
    UserURL= "ATCMfile:SignalError.htm"
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

inline{STDLIB(SignalError)}

END_FUNCTION_BLOCK
