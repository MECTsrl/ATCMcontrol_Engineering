(*
 * ATCMControl Embedded - System Library
 * ----------------------------------------------------------------------------
 *
 
 *)

FUNCTION LD_Get : WORD

(* Warning: This function is not thread safe. Calling this function from
 * different tasks for the same load type may cause unpredictable results!
 *)

VAR_INPUT
    LoadType    : WORD;         (* See SysLoad.cst for details.     *)
    TaskID      : UINT;         (* See SysLoad.cst for details.     *)
END_VAR

VAR_OUTPUT
    str         : STRING;
END_VAR

inline {STDLIB(LD_Get)}

END_FUNCTION
