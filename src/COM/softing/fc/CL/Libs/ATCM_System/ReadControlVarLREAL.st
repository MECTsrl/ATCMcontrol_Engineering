(*[[
    UserURL= "ATCMFile:ReadControlVarXXXX.htm"
]]*)
(* 
 * ATCMControl standard function ReadControlVarLREAL
 * ----------------------------------------------------------------------------
 * PURPOSE: read from a named control variable.
 * 
 * EXAMPLE: aLreal := ReadControlVarLREAL("Prog1.myFB.aLreal");
 * 
 * RETURN:  if the function fails, a corresponding 
 *          OutputDebugString is made and return is 0.0.
 * 
 * WARNING: use this function only rarely, it is not efficient.
 *)
FUNCTION ReadControlVarLREAL : LREAL
VAR_INPUT
   sPath : WSTRING;     (* full qualified name of the variable to read *)
END_VAR

    ;(* add target specific implementaion here *)
    ReadControlVarLREAL := 0.0;

END_FUNCTION
