(* 
 * ATCMControl standard function ColdStartInstance
 * ----------------------------------------------------------------------------
 * PURPOSE: initialises all program, function block or structure instance members 
 *          to their coldstart initial values. This includes also all nested 
 *          function blocks and structs.
     *          The function returns if the initialisation is complete.
 * 
 * EXAMPLE: StopLocalTask("Task1");
 *          ColdStartInstance("Prog1.myFB");
 *
 * NOTE:    It is recommended to stop the task(s) that operates on "Prog1.myFB"
 *          before calling ColdStartInstance.
 * 
 * WARNING: use this function only rarely, it may take a long time to coldstart a 
 *          huge and deeply nested instance, that could cause watchdog error in the 
 *          calling task. Typically this function is used in a task that also 
 *          starts/stops other Tasks and therefore SetSystemTask() was called to 
 *          disable the watchdog anyway.
 *)
FUNCTION ColdStartInstance
VAR_INPUT
    instPath : WSTRING;
END_VAR

NATIVE
    int    i = 0;
    Object o = null;
    java.util.StringTokenizer st = new java.util.StringTokenizer(_instpath.toLowerCase(), ".");
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
        
    if (o == null || !(o instanceof ObjectDirectoryEx))
    {
        DebugManager.print("error E1056: ColdStartInstance: instance path '" + _instpath + "' does not exist.");
        return;
    }
    ObjectDirectoryEx od = (ObjectDirectoryEx)o;

    try{
        for(i=0; ;i++)
        {
            FcObject          fobj = od.getObjectByID(i);
            ObjectDescriptor  odescr = fobj.getOD();
            String            init = odescr.getInitString();

            if(init != null)
            {
                //DebugManager.print("vorher="+init);
                if(init.length()==0)
                {
                    init = null;//should not happen...
                }
                else if(init.charAt(0)=='%' && !(fobj instanceof COM.softing.fc.CC.system.StringVar))
                {
                    if(init.charAt(1)=='I')
                        continue;

                    int iPos = init.indexOf('|');
                    if(iPos>0 && iPos+1<init.length())
                    {
                        init = init.substring(iPos+1);
                    }
                    else
                    {
                        init = null;
                    }
                }
                //DebugManager.print("nachher="+init);
            }
    

            if (fobj instanceof FcBool)
            {
                ((FcBool)fobj).putBool(init==null ? false : init.equalsIgnoreCase("true"));
            }
            else if (fobj instanceof FcInt)
            {
                ((FcInt)fobj).putInt(init==null ? 0 : Integer.parseInt(init));
            }
            else if (fobj instanceof FcLong)
            {
              ((FcLong)fobj).putLong(init==null ? 0 : Long.parseLong(init));
            }
            else if (fobj instanceof FcDouble)
            {
              ((FcDouble)fobj).putDouble(init==null ? 0 : (new Double(init)).doubleValue());
            }
            else if (fobj instanceof FcString)
            {
              ((FcString)fobj).putString(init==null ? "" : init);
            }
            else if (fobj instanceof ObjectDirectoryEx)
            {
              _F_coldstartinstance(_instpath + "."+ fobj.getOD().getName(), globals, tc);
            }
            else
            {
               throw new ClassCastException("4CC : fatal error E0041: ColdStartInstance: unknown basic type class");
            }
        }
    }catch(ArrayIndexOutOfBoundsException e)
    {
        //normal end for loop: because there is no od.getLength() !! 
    }
END_NATIVE;

END_FUNCTION
