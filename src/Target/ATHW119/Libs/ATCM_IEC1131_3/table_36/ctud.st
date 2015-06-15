(*[[
    UserURL= "ATCMfile:TP.htm"
]]*)
(* 
 *  IEC-1131-3 Standard Functionblock CTUD for data type DINT  
 *  Purpose: Up/Down Counter
 *)
FUNCTION_BLOCK CTUD
VAR_INPUT (* Inputs *)
    CU         : BOOL R_EDGE; (* count up if rising edge   *)
    CD         : BOOL R_EDGE; (* count down if rising edge *)
    R          : BOOL;        (* Reset, set CV to 0        *)
    LD         : BOOL;        (* if true, copy PV to CV    *)
    PV         : INT;         (* start value               *)
END_VAR
VAR_OUTPUT (* Outputs *)
    QU         : BOOL;        (* TRUE if CV >= PV          *)
    QD         : BOOL;        (* TRUE if CV <= 0           *)
    CV         : INT;         (* counter output            *)
END_VAR

inline { STDLIB(ctud) }

END_FUNCTION_BLOCK
