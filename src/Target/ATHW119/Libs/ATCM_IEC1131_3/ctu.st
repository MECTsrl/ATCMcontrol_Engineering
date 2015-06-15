(*[[
    UserURL= "ATCMfile:TON.htm"
]]*)
(* 
 *  IEC-1131-3 Standard Functionblock CTU for data type INT
 *  Purpose: Up Counter
 *)
FUNCTION_BLOCK CTU
VAR_INPUT (* Inputs *)
    CU         : BOOL R_EDGE; (* count up if rising edge *)
    R          : BOOL;        (* if true,  CV to zero    *)
    PV         : INT;         (* end value               *)
END_VAR
VAR_OUTPUT (* Outputs *)
    Q          : BOOL;        (* TRUE if CV >= PV        *)
    CV         : INT;         (* counter output          *)
END_VAR

inline { STDLIB(ctu) }

END_FUNCTION_BLOCK
