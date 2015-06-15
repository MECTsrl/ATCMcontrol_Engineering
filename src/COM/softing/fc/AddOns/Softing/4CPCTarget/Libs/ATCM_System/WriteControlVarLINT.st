(*[[
]]*)
(* 
 * ATCMControl standard function WriteControlVarLINT
 * ----------------------------------------------------------------------------
 * PURPOSE: write to a named control variable.
 * 
 * EXAMPLE: bOk := WriteControlVarLINT("Prog1.myFB.aLint", "42");
 * 
 * RETURN:  if the function fails, a corresponding 
 *          OutputDebugString is made and return is FALSE.
 *          If successfull the return value is TRUE. 
 * 
 * WARNING: use this function only rarely, it is not efficient.
 *)
FUNCTION WriteControlVarLINT : BOOL
VAR_INPUT
   sPath : WSTRING;       (* full qualified name of the variable to write *)
   iVal  : LINT;          (* value to set *)
END_VAR

    WriteControlVarLINT := FALSE;

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
                ((FcLong)o).putLong(_ival);
                _writecontrolvarlint = true;
            }
            else
            {
                DebugManager.print("4C_Control(WriteControlVarLINT): variable '" + _spath + "' is not a JAVA int.");
            }
        } 
        else
        {
            DebugManager.print("4C_Control(WriteControlVarLINT): variable '" + _spath + "' does not exist.");
        }
    END_NATIVE;

END_FUNCTION


