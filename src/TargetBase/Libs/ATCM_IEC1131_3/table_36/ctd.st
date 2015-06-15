(*[[
    UserURL= "ATCMfile:TOF.htm"
]]*)
(* 
 *  IEC-1131-3 Standard Functionblock CTD for data type INT
 *  Purpose: Down Counter
 *)
FUNCTION_BLOCK CTD
VAR_INPUT  (* Inputs *)
    CD         : BOOL R_EDGE; (* count down if rising edge     *)
    LD         : BOOL;        (* if true, copy PV to CV        *)
    PV         : INT;         (* start value                   *)
END_VAR
VAR_OUTPUT (* Outputs *)
    Q          : BOOL;        (* TRUE if CV <= 0               *)
    CV         : INT;         (* counter output                *)
END_VAR

inline { STDLIB(ctd) }

END_FUNCTION_BLOCK
