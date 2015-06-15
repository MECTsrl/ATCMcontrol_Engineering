(*[[
    UserURL= "ATCMFile:WriteControlVarXXXX.htm"
]]*)
(* 
 * ATCMControl standard function WriteControlVarWSTRING
 * ----------------------------------------------------------------------------
 * PURPOSE: write to a named control variable.
 * 
 * EXAMPLE: bOk := WriteControlVarWSTRING("Prog1.myFB.aStr", "hello");
 * 
 * RETURN:  if the function fails, a corresponding 
 *          OutputDebugString is made and return is FALSE.
 *          If successfull the return value is TRUE. 
 * 
 * WARNING: use this function only rarely, it is not efficient.
 *)
FUNCTION WriteControlVarWSTRING : BOOL
VAR_INPUT
   sPath : WSTRING;       (* full qualified name of the variable to write *)
   sVal  : WSTRING;       (* value to set *)
END_VAR

    ;(* add target specific implementaion here *)
    WriteControlVarWSTRING  := FALSE;

END_FUNCTION


