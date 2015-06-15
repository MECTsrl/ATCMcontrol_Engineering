
(*[[
    UserURL= "ATCM:CTD_DINT_EX.htm"
]]*)
(* 
 *  IEC-1131-3 Standard Functionblock CTD for data type DINT  
 *  Purpose: Down Counter with visualisation scaling parameters
 *)
FUNCTION_BLOCK CTD_DINT_EX
VAR_INPUT  (* Inputs *)
    CD         : BOOL R_EDGE; (* count down if rising edge     *)
    LD         : BOOL;        (* if true, copy PV to CV        *)
    PV         : DINT;        (* start value                   *)
END_VAR
VAR_INPUT  (* Parameters *)
    SCALEMIN   : DINT:=0;     (* minimum of scaling            *)
    SCALEMAX   : DINT:=100;   (* maximum of scaling            *)
END_VAR
VAR_OUTPUT (* Outputs *)
    Q          : BOOL;        (* TRUE if CV <= 0               *)
    CV         : DINT;        (* counter output                *)
    ERROR      : DINT;        (* error                         *)
END_VAR


ERROR := 0;
IF (SCALEMIN >= SCALEMAX) THEN
   ERROR:=-1;
END_IF;

IF LD THEN 
   CV := PV;
ELSIF CD AND (CV > FC_MIN_DINT) THEN 
   CV:=CV-1;
END_IF;

Q := (CV <= 0);

END_FUNCTION_BLOCK



