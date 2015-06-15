(*
 * ATCMControl Embedded - System Library
 * ----------------------------------------------------------------------------
 *
 
 *)

(*[[
    UserURL= "ATCMfile:SFC_ResetToInitial.htm"
]]*)

(* SFC_ResetToInitial
 * ----------------------------------------------------------------------------
 * Resets the SFC logic.
 * in   bEnable         Reset only if TRUE.
 *)
FUNCTION SFC_ResetToInitial

VAR_INPUT
    bEnable : BOOL; (* reset only if bEnable = TRUE *)
END_VAR

inline
{
    push1        %1
    POPD_IN_1    __resetSFC
}

END_FUNCTION
