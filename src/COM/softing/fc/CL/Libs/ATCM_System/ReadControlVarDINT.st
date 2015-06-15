(*[[
    UserURL= "ATCMFile:ReadControlVarXXXX.htm"
]]*)
(* 
 * ATCMControl standard function ReadControlVarDINT
 * ----------------------------------------------------------------------------
 * PURPOSE: read from a named control variable.
 * 
 * EXAMPLE: aInt := ReadControlVarDINT("Prog1.myFB.aInt");
 * 
 * RETURN:  if the function fails, a corresponding 
 *          OutputDebugString is made and return is 0.
 * 
 * WARNING: use this function only rarely, it is not efficient.
 *)
FUNCTION ReadControlVarDINT : DINT
VAR_INPUT
   sPath : WSTRING;     (* full qualified name of the variable to read *)
END_VAR

    ;(* add target specific implementaion here *)
    ReadControlVarDINT := 0;
END_FUNCTION
