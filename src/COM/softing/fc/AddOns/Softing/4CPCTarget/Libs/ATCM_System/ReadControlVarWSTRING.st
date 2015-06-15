(*[[
]]*)
(* 
 * ATCMControl standard function ReadControlVarWSTRING
 * ----------------------------------------------------------------------------
 * PURPOSE: read from a named control variable.
 * 
 * EXAMPLE: bStr := ReadControlVarWSTRING("Prog1.myFB.aStr");
 * 
 * RETURN:  if the function fails, a OutputDebugString
 *          is made and return is "".
 * 
 * WARNING: use this function only rarely, it is not efficient.
 *)
FUNCTION ReadControlVarWSTRING : WSTRING
VAR_INPUT
   sPath : WSTRING;     (* full qualified name of the variable to read *)
END_VAR



   ReadControlVarWSTRING  := "";

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
                _readcontrolvarwstring = ((FcString)o).getString();
            }
            else
            {
                DebugManager.print("4C_Control(ReadControlVarWSTRING): variable '" + _spath + "' is not a JAVA String.");
            }
        } 
        else
        {
            DebugManager.print("4C_Control(ReadControlVarWSTRING): variable '" + _spath + "' does not exist.");
        }
    END_NATIVE;
END_FUNCTION


(* $Header: /ABK4191 Liebherr Pruefstand-Modernisierung/4C-Basis/4C_Control/ReadControlVarWSTRING.st 4     13.02.02 11:15 Go $
*----------------------------------------------------------------------------*
*
* =FILENAME            $Workfile: ReadControlVarWSTRING.st $
*                       $Logfile: /ABK4191 Liebherr Pruefstand-Modernisierung/4C-Basis/4C_Control/ReadControlVarWSTRING.st $
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
