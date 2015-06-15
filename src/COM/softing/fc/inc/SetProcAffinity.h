/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/inc/SetProcAffinity.h 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: SetProcAffinity.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/inc/SetProcAffinity.h $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                CMN
 *
 * =COMPONENT           INC
 *
 * =CURRENT      $Date: 28.02.07 19:01 $
 *               $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *
 


 

 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *== Helper function to set the processor affinity mask, include this into our main.cpp
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  23.01.2007  EF      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/



//------------------------------------------------------------------*
/**
 * SetProcAffinity.
 *
 * @param          lPreferedProc: the prefered processor where this 
 *                 process should run a value 1 or 2. 
 *                 If there is no second CPU 1 is assumed and this function
 *                 does nothing.
 * @return         DWORD: the actual selected processor affinity mask or 0
 *                 if any error       
 * @exception       -
 * @see            GetProcessAffinityMask, SetProcessAffinityMask  
*/
static DWORD FC_SetProcAffinity(long lPreferedProc)
{
    DWORD   processAffinityMask;
    DWORD   systemAffinityMask;
    DWORD   procId;
    HANDLE  hProcess;


    procId = GetCurrentProcessId();

    hProcess = OpenProcess(
        PROCESS_ALL_ACCESS,  // access flag
        FALSE,               // handle inheritance option
        procId               // process identifier
    );
    if(!hProcess)
        return 0;

    if(!GetProcessAffinityMask(hProcess, &processAffinityMask, &systemAffinityMask))
    {
        CloseHandle(hProcess);
        return 0;
    }
   
    //printf("processAffinityMask = 0x%x\n",processAffinityMask);
    //printf("systemAffinityMask  = 0x%x\n",systemAffinityMask);
    
    if((systemAffinityMask & 0x3)==0x3 && (processAffinityMask & 0x3)==0x3)
    {
        if(lPreferedProc == 1)
            processAffinityMask = 0x1;
        else
            processAffinityMask = 0x2;

        if(!SetProcessAffinityMask(hProcess, processAffinityMask))
            processAffinityMask = 0;
    }
    //else: seems to be single processor system anyway, nothing todo...
    //TODO: consider for example the case systemAffinityMask == 0xf0 ??

    CloseHandle(hProcess);
    return processAffinityMask;
}



/*
 *----------------------------------------------------------------------------*
 *  $History: SetProcAffinity.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/inc
 * 
 * *****************  Version 1  *****************
 * User: Ef           Date: 23.01.07   Time: 11:38
 * Created in $/4Control/COM/softing/fc/inc
 *==
 *----------------------------------------------------------------------------*
*/
