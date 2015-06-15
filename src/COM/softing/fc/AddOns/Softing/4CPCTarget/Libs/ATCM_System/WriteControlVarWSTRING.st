(*[[
]]*)
(* 
 * ATCMControl standard function WriteControlVarWSTRING
 * ----------------------------------------------------------------------------
 * PURPOSE: write to a named control variable.
 * 
 * EXAMPLE: bOk := WriteControlVarWSTRING("Prog1.myFB.aStr, "42");
 * 
 * RETURN:  if the function fails, a OutputDebugString
 *          is made and return is FALSE.
 * 
 * WARNING: use this function only rarely, it is not efficient.
 *)
FUNCTION WriteControlVarWSTRING : BOOL
VAR_INPUT
   sPath : WSTRING;       (* full qualified name of the variable to write *)
   sVal  : WSTRING;       (* value to set *)
END_VAR

    WriteControlVarWSTRING  := FALSE;

    NATIVE
        int    i = 0;
        Object o = null;
        java.util.StringTokenizer st = new java.util.StringTokenizer(_spath.toLowerCase(), ".");

        while (st.hasMoreTokens()) 
        {
            if (i == 0)
                o = ((ObjectDirectory)((FcObject)tc).getParent()).getObject(st.nextToken());
            else 
                o = ((ObjectDirectory)o).getObject(st.nextToken());
            
            if (o == null)
                break;
            
            i++;
        }
        
        if (o != null)
        {
            if (o instanceof FcString)
            {
                ((FcString)o).putString(_sval);
                _writecontrolvarwstring = true;
            }
            else
            {
                DebugManager.print("4C_Control(WriteControlVarWSTRING): variable '" + _spath + "' is not a JAVA String.");
            }
        } 
        else
        {
            DebugManager.print("4C_Control(WriteControlVarWSTRING): variable '" + _spath + "' does not exist.");
        }
    END_NATIVE;
END_FUNCTION


