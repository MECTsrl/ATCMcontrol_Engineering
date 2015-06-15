(*[[
    UserURL= "ATCMFile:ReadControlVarXXXX.htm"
]]*)
(* 
 * ATCMControl standard function ReadControlVarBOOL
 * ----------------------------------------------------------------------------
 * PURPOSE: read from a named control variable.
 * 
 * EXAMPLE: bValue := ReadControlVarBOOL("Prog1.myFB.aBool");
 * 
 * RETURN:  if the function fails, a corresponding 
 *          OutputDebugString is made and return is FALSE.
 * 
 * WARNING: use this function only rarely, it is not efficient.
 *)
FUNCTION ReadControlVarBOOL : BOOL
VAR_INPUT
   sPath : WSTRING;     (* full qualified name of the variable to read *)
END_VAR

    ;(* add target specific implementaion here *)
    ReadControlVarBOOL := FALSE;
END_FUNCTION
