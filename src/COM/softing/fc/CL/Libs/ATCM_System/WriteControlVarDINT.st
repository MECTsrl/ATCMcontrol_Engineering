(*[[
    UserURL= "ATCMFile:WriteControlVarXXXX.htm"
]]*)
(* 
 * ATCMControl standard function WriteControlVarDINT
 * ----------------------------------------------------------------------------
 * PURPOSE: write to a named control variable.
 * 
 * EXAMPLE: bOk := WriteControlVarDINT("Prog1.myFB.aDint", "42");
 * 
 * RETURN:  if the function fails, a corresponding 
 *          OutputDebugString is made and return is FALSE.
 *          If successfull the return value is TRUE. 
 * 
 * WARNING: use this function only rarely, it is not efficient.
 *)
FUNCTION WriteControlVarDINT : BOOL
VAR_INPUT
   sPath : WSTRING;       (* full qualified name of the variable to write *)
   iVal  : DINT;          (* value to set *)
END_VAR

    ;(* add target specific implementaion here *)
    WriteControlVarDINT := FALSE;

END_FUNCTION


