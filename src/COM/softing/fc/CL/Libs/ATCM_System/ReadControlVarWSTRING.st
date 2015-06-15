(*[[
    UserURL= "ATCMFile:ReadControlVarXXXX.htm"
]]*)
(* 
 * ATCMControl standard function ReadControlVarWSTRING
 * ----------------------------------------------------------------------------
 * PURPOSE: read from a named control variable.
 * 
 * EXAMPLE: bStr := ReadControlVarWSTRING("Prog1.myFB.aStr");
 * 
 * RETURN:  if the function fails, a corresponding 
 *          OutputDebugString is made and return is "".
 * 
 * WARNING: use this function only rarely, it is not efficient.
 *)
FUNCTION ReadControlVarWSTRING : WSTRING
VAR_INPUT
   sPath : WSTRING;     (* full qualified name of the variable to read *)
END_VAR

    ;(* add target specific implementaion here *)
    ReadControlVarWSTRING  := "";

END_FUNCTION


