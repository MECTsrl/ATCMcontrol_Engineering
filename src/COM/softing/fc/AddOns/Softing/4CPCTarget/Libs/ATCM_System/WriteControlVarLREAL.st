(*[[
]]*)
(* 
 * ATCMControl standard function WriteControlVarLREAL
 * ----------------------------------------------------------------------------
 * PURPOSE: write to a named control variable.
 * 
 * EXAMPLE: bOk := WriteControlVarLREAL("Prog1.myFB.aStr, "42.0");
 * 
 * RETURN:  if the function fails, a OutputDebugString
 *          is made and return is FALSE.
 * 
 * WARNING: use this function only rarely, it is not efficient.
 *)
FUNCTION WriteControlVarLREAL : BOOL
VAR_INPUT
   sPath : WSTRING;       (* full qualified name of the variable to write *)
   lVal  : LREAL;         (* value to set *)
END_VAR

    WriteControlVarLREAL := FALSE;

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
                ((FcDouble)o).putDouble(_lval);
                _writecontrolvarlreal = true;
            }
            else
            {
                DebugManager.print("4C_Control(WriteControlVarLREAL): variable '" + _spath + "' is not a JAVA double.");
            }
        } 
        else
        {
            DebugManager.print("4C_Control(WriteControlVarLREAL): variable '" + _spath + "' does not exist.");
        }
    END_NATIVE;
END_FUNCTION


