(*[[
]]*)
(* 
 * ATCMControl standard function ReadControlVarLREAL
 * ----------------------------------------------------------------------------
 * PURPOSE: read from a named control variable.
 * 
 * EXAMPLE: aLreal := ReadControlVarLREAL("Prog1.myFB.aLreal");
 * 
 * RETURN:  if the function fails, a OutputDebugString
 *          is made and return is 0.
 * 
 * WARNING: use this function only rarely, it is not efficient.
 *)
FUNCTION ReadControlVarLREAL : LREAL
VAR_INPUT
   sPath : WSTRING;     (* full qualified name of the variable to read *)
END_VAR

    ReadControlVarLREAL := 0.0;

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
            if (o instanceof FcDouble)
            {
                _readcontrolvarlreal = ((FcDouble)o).getDouble();
            }
            else
            {
                DebugManager.print("4C_Control(ReadControlVarLREAL): variable '" + _spath + "' is not a JAVA double.");
            }
        } 
        else
        {
            DebugManager.print("4C_Control(ReadControlVarLREAL): variable '" + _spath + "' does not exist.");
        }
    END_NATIVE;
END_FUNCTION
