(*[[
    UserURL= "ATCMFile:ReadControlVarXXXX.htm"
]]*)
(* 
 * ATCMControl standard function ReadControlVarLINT
 * ----------------------------------------------------------------------------
 * PURPOSE: read from a named control variable.
 * 
 * EXAMPLE: aLreal := ReadControlVarLINT("Prog1.myFB.aLint");
 * 
 * RETURN:  if the function fails, a corresponding 
 *          OutputDebugString is made and return is 0.
 * 
 * WARNING: use this function only rarely, it is not efficient.
 *)
FUNCTION ReadControlVarLINT : LINT
VAR_INPUT
   sPath : WSTRING;     (* full qualified name of the variable to read *)
END_VAR

    ;(* add target specific implementaion here *)
    ReadControlVarLINT := 0;

END_FUNCTION
