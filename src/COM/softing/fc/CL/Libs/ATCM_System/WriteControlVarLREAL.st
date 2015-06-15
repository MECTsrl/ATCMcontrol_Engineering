(*[[
    UserURL= "ATCMFile:WriteControlVarXXXX.htm"
]]*)
(* 
 * ATCMControl standard function WriteControlVarLREAL
 * ----------------------------------------------------------------------------
 * PURPOSE: write to a named control variable.
 * 
 * EXAMPLE: bOk := WriteControlVarLREAL("Prog1.myFB.aLreal", "42.0");
 * 
 * RETURN:  if the function fails, a corresponding 
 *          OutputDebugString is made and return is FALSE.
 *          If successfull the return value is TRUE. 
 * 
 * WARNING: use this function only rarely, it is not efficient.
 *)
FUNCTION WriteControlVarLREAL : BOOL
VAR_INPUT
   sPath : WSTRING;       (* full qualified name of the variable to write *)
   lVal  : LREAL;         (* value to set *)
END_VAR

    ;(* add target specific implementaion here *)
    WriteControlVarLREAL := FALSE;

END_FUNCTION


