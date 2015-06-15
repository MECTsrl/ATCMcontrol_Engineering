(*[[
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

    ReadControlVarLINT := 0;

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
            if (o instanceof FcLong)
            {
                _readcontrolvarlint = ((FcLong)o).getLong();
            }
            else
            {
                DebugManager.print("4C_Control(ReadControlVarLINT): variable '" + _spath + "' is not a JAVA long.");
            }
        } 
        else
        {
            DebugManager.print("4C_Control(ReadControlVarLINT): variable '" + _spath + "' does not exist.");
        }
    END_NATIVE;

END_FUNCTION
