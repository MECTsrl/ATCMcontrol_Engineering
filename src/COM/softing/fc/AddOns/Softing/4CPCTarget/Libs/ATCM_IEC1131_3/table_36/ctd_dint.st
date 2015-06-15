
(*[[
    UserURL= "ATCM:CTD_DINT.htm"
]]*)
(* 
 *  IEC-1131-3 Standard Functionblock CTD for data type DINT  
 *  Purpose: Down Counter
 *)
FUNCTION_BLOCK CTD_DINT
VAR_INPUT  (* Inputs *)
    CD         : BOOL R_EDGE; (* count down if rising edge     *)
    LD         : BOOL;        (* if true, copy PV to CV        *)
    PV         : DINT;        (* start value                   *)
END_VAR
VAR_OUTPUT (* Outputs *)
    Q          : BOOL;        (* TRUE if CV <= 0               *)
    CV         : DINT;        (* counter output                *)
END_VAR


  IF LD THEN 
	CV := PV;
  ELSIF CD AND (CV > FC_MIN_DINT) THEN 
	CV:=CV-1;
  END_IF;

  Q := (CV <= 0); 

END_FUNCTION_BLOCK
