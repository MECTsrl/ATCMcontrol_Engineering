(*[[
    UserURL= "ATCMfile:SFCResetToInitialStep.htm"
]]*)
(* 
 *  ATCMControl Standard Function SFCResetToInitialStep
 *  Purpose: reset SFC logic
 *)
FUNCTION SFCResetToInitialStep

VAR_INPUT
    ENABLE : BOOL; (* reset only if ENABLE = TRUE *)
END_VAR

inline
{
    push1     %1
    POPD_IN_1    __resetSFC
}

END_FUNCTION
