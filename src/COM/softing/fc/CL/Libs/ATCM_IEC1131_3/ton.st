(*[[
    UserURL= "ATCMFile:TON.htm"
]]*)
FUNCTION_BLOCK TON

VAR_INPUT   (* Inputs *)
    IN : BOOL := FALSE; (* Input        *)
    PT : TIME := T#0s;  (* Preset time 	*)
END_VAR

VAR_OUTPUT  (* Outputs *)
    Q  : BOOL := FALSE; (* TRUE if ET >= PT *)
    ET : TIME := T#0s;  (* Elapsed time     *)
END_VAR

VAR NON_RETAIN
    StartTime : TIME;
    Trig : BOOL := FALSE;
END_VAR

IF PT > T#0s THEN
    IF NOT IN THEN
        Q := FALSE;
        ET := T#0s;
		Trig := FALSE;
    ELSIF NOT Trig THEN
        Trig := TRUE;
        StartTime := getTimeSinceSystemBoot();
    ELSIF NOT Q THEN
        ET := getTimeSinceSystemBoot() - StartTime;
        IF ET >= PT THEN
            Q := TRUE;
            ET := PT;
        END_IF;
    END_IF;
ELSE
	(* error handling *)
    Q := IN;
    ET := T#0s;
    Trig := FALSE;
END_IF;

END_FUNCTION_BLOCK
