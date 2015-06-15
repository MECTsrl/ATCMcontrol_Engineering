
(*[[
    UserURL= "ATCM:CTU_DINT_EX.htm"
]]*)
(* 
 *  IEC-1131-3 Standard Functionblock CTU for data type DINT  
 *  Purpose: Up Counter with visualisation scaling parameters
 *)
FUNCTION_BLOCK CTU_DINT_EX
VAR_INPUT  (* Inputs *)
    CU         : BOOL R_EDGE; (* count up if rising edge *)
    R          : BOOL;        (* if true,  CV to zero    *)
    PV         : DINT;        (* end value               *)
END_VAR
VAR_INPUT  (* Parameters *)
    SCALEMIN   : DINT:=0;     (* minimum of scaling      *)
    SCALEMAX   : DINT:=100;   (* maximum of scaling      *)
END_VAR
VAR_OUTPUT (* Outputs *)
    Q          : BOOL;        (* TRUE if CV >= PV        *)
    CV         : DINT;        (* counter output          *)
    ERROR      : DINT;        (* error                   *)
END_VAR


ERROR := 0;
IF (SCALEMIN >= SCALEMAX) THEN
    ERROR:=-1;
END_IF;

IF R THEN 
    CV := 0;
ELSIF CU AND (CV < FC_MAX_DINT) THEN 
    CV:=CV+1;
END_IF;

Q := (CV >= PV);

END_FUNCTION_BLOCK


