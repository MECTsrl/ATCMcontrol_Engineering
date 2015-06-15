(*[[
    UserURL= "ATCMFile:WriteControlVarXXXX.htm"
]]*)
(* 
 * ATCMControl standard function WriteControlVarLINT
 * ----------------------------------------------------------------------------
 * PURPOSE: write to a named control variable.
 * 
 * EXAMPLE: bOk := WriteControlVarLINT("Prog1.myFB.aLint", "42");
 * 
 * RETURN:  if the function fails, a corresponding 
 *          OutputDebugString is made and return is FALSE.
 *          If successfull the return value is TRUE. 
 * 
 * WARNING: use this function only rarely, it is not efficient.
 *)
FUNCTION WriteControlVarLINT : BOOL
VAR_INPUT
   sPath : WSTRING;       (* full qualified name of the variable to write *)
   iVal  : LINT;          (* value to set *)
END_VAR

    ;(* add target specific implementaion here *)
    WriteControlVarLINT := FALSE;

END_FUNCTION


