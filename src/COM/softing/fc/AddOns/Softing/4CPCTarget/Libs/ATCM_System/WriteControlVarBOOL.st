(*[[
]]*)
(* 
 * ATCMControl standard function WriteControlVarBOOL
 * ----------------------------------------------------------------------------
 * PURPOSE: write to a named control variable.
 * 
 * EXAMPLE: bOk := WriteControlVarBOOL("Prog1.myFB.aBool, TRUE);
 * 
 * RETURN:  if the function fails, a OutputDebugString
 *          is made and return is FALSE.
 * 
 * WARNING: use this function only rarely, it is not efficient.
 *)
FUNCTION WriteControlVarBOOL : BOOL
VAR_INPUT
   sPath : WSTRING;       (* full qualified name of the variable to write *)
   bVal  : BOOL;          (* value to set *)
END_VAR



    WriteControlVarBOOL := FALSE;
    
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
            if (o instanceof FcBool)
            {
                ((FcBool)o).putBool(_bval);
                _writecontrolvarbool = true;
            }
            else
            {
                DebugManager.print("4C_Control(WriteControlVarBOOL): variable '" + _spath + "' is not a JAVA boolean.");
            }
        } 
        else
        {
            DebugManager.print("4C_Control(WriteControlVarBOOL): variable '" + _spath + "' does not exist.");
        }
    END_NATIVE;
END_FUNCTION


(* $Header: /ABK4191 Liebherr Pruefstand-Modernisierung/4C-Basis/4C_Control/WriteControlVarBOOL.st 4     13.02.02 11:15 Go $
*----------------------------------------------------------------------------*
*
* =FILENAME            $Workfile: WriteControlVarBOOL.st $
*                       $Logfile: /ABK4191 Liebherr Pruefstand-Modernisierung/4C-Basis/4C_Control/WriteControlVarBOOL.st $
*
* =PROJECT             ABK4191 Liebherr Prüfstand-Modernisierung
*
* =COMPONENT           4C_Control Library
*
* =CURRENT      $Date: 13.02.02 11:15 $
*           $Revision: 4 $
*             $Author: Go $
*----------------------------------------------------------------------------*
*******************************************************************************
*)
