(*[[
]]*)
(* 
 * ATCMControl standard function WriteControlVarDINT
 * ----------------------------------------------------------------------------
 * PURPOSE: write to a named control variable.
 * 
 * EXAMPLE: bOk := WriteControlVarDINT("Prog1.myFB.aStr, "42");
 * 
 * RETURN:  if the function fails, a OutputDebugString
 *          is made and return is FALSE.
 * 
 * WARNING: use this function only rarely, it is not efficient.
 *)
FUNCTION WriteControlVarDINT : BOOL
VAR_INPUT
   sPath : WSTRING;       (* full qualified name of the variable to write *)
   iVal  : DINT;          (* value to set *)
END_VAR

    WriteControlVarDINT := FALSE;

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
            if (o instanceof FcInt)
            {
                ((FcInt)o).putInt(_ival);
                _writecontrolvardint = true;
            }
            else
            {
                DebugManager.print("4C_Control(WriteControlVarDINT): variable '" + _spath + "' is not a JAVA int.");
            }
        } 
        else
        {
            DebugManager.print("4C_Control(WriteControlVarDINT): variable '" + _spath + "' does not exist.");
        }
    END_NATIVE;
END_FUNCTION


