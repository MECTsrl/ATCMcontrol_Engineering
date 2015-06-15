
(*[[
    UserURL= "ATCM:SFCResetToInitialStep.htm"
]]*)
(* 
 *  ATCMControl Standard Function SFCResetToInitialStep
 *  Purpose: reset SFC logic
 *)
FUNCTION SFCResetToInitialStep
VAR_INPUT
    ENABLE : BOOL; (* reset only if ENABLE = TRUE *)
END_VAR

inline{if(%1)this.resetToInitialStep()}

END_FUNCTION
