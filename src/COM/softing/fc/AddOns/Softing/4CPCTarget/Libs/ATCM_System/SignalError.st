
(*[[
    UserURL= "ATCM:SignalError.htm"
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
    NATIVE
        DebugManager.print("4CC : error E1000: SignalError(ERRNO=" + _errno.getInt() +")\n");
        Thread.dumpStack();
        tc.errorTask(_errno.getInt());
    END_NATIVE;
END_IF;

END_FUNCTION_BLOCK
