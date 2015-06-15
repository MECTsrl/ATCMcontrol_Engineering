(*[[
    UserURL= "ATCMFile:WriteControlVarXXXX.htm"
]]*)
(* 
 * ATCMControl standard function WriteControlVarBOOL
 * ----------------------------------------------------------------------------
 * PURPOSE: write to a named control variable.
 * 
 * EXAMPLE: bOk := WriteControlVarBOOL("Prog1.myFB.aBool", TRUE);
 * 
 * RETURN:  if the function fails, a corresponding 
 *          OutputDebugString is made and return is FALSE.
 *          If successfull the return value is TRUE. 
 * 
 * WARNING: use this function only rarely, it is not efficient.
 *)
FUNCTION WriteControlVarBOOL : BOOL
VAR_INPUT
   sPath : WSTRING;       (* full qualified name of the variable to write *)
   bVal  : BOOL;          (* value to set *)
END_VAR

    ;(* add target specific implementaion here *)
    WriteControlVarBOOL := FALSE;
    
END_FUNCTION


