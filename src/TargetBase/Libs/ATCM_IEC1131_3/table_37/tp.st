(*[[
    UserURL= "ATCMfile:TP.htm"
]]*)

FUNCTION_BLOCK TP

VAR_INPUT   (* Inputs *)
    IN : BOOL := FALSE; (* Input       *)
    PT : TIME := T#0s;  (* Preset time *)
END_VAR

VAR_OUTPUT  (* Outputs *)
    Q  : BOOL := FALSE; (* FALSE if ET >= PT    *)
    ET : TIME := T#0s;  (* Elapsed time         *)
END_VAR

VAR NON_RETAIN
    StartTime : TIME;
    State     : INT := 0;
END_VAR

VAR
    M         : BOOL := FALSE;
END_VAR

inline { STDLIB(tp) }

END_FUNCTION_BLOCK
