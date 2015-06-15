(*[[
    UserURL= "ATCMFile:TP_EX.htm"
]]*)

FUNCTION_BLOCK TP_EX

VAR_INPUT   (* Inputs *)
    IN : BOOL := FALSE; (* Input       *)
    PT : TIME := T#0s;  (* Preset time *)
END_VAR

VAR_INPUT (* Parameters *)
    SCALEMIN : DINT := 0;   (* minimum of scaling *)
    SCALEMAX : DINT := 100; (* maximum of scaling *)
END_VAR

VAR_OUTPUT  (* Outputs *)
    Q  : BOOL := FALSE; (* FALSE if ET >= PT    *)
    ET : TIME := T#0s;  (* Elapsed time         *)
    ERROR : DINT;       (* Error                *)
END_VAR

VAR
    M         : BOOL := FALSE;
END_VAR

VAR NON_RETAIN
    Trig      : BOOL := FALSE;
    StartTime : TIME;
END_VAR

  ERROR := 0;
  IF (SCALEMIN >= SCALEMAX) THEN
     ERROR := -1;
  END_IF;

  IF PT > T#0s THEN
    IF NOT Trig THEN
        Q := FALSE;
        IF IN AND NOT M THEN
            (* initilize function block *)
            (* get start time *)
            Trig := TRUE;
            StartTime := getTimeSinceSystemBoot();
            ET := T#0s;
            Q := TRUE;
        ELSIF NOT IN THEN
            ET := T#0s;
        END_IF;
    ELSE (* IF Trig *)
        (* get elapsed time *)
        ET := getTimeSinceSystemBoot() - Starttime;

        IF ET >= PT THEN (* time reached *)
            Trig := FALSE;
            Q := FALSE;
            IF NOT IN THEN
              ET := T#0s;
            ELSE
              ET := PT;
            END_IF;
        END_IF;
	END_IF;

  ELSE
    (* error handling *)
    Q := FALSE;
    ET := T#0s;
    Trig := FALSE;
    ERROR := -1;
  END_IF;


  M := IN;

END_FUNCTION_BLOCK
