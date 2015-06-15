/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_tools/fc_regenum.c 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: fc_regenum.c $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_tools/fc_regenum.c $
 *
 * =PROJECT             CAK1020  ATCMControl V2.0
 *
 * =SWKE                fc
 *
 * =COMPONENT           fc_tools
 *
 * =CURRENT             $Date: 28.02.07 19:00 $
 *                      $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *

 



 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==Helper functions to enumerate the registry with a call back
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  04.01.01  EF      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/


#include <tchar.h>
#include "fc_tools\fc_registry.h"



FC_TOAPI LONG FC_TODECL FC_RegEnumKeys(
    HKEY               hKey, 
    const TCHAR*       pszSubKey, 
    FC_RegEnumCallBack pCallBack,
    void*              pUserData
)
{
    LONG      lret;
    LONG      lret2;
    int       iAction;
    DWORD     dwIndex;
    TCHAR     szSubKey[MAX_PATH];
    DWORD     dwMP;
    FILETIME  lmft;
    HKEY      hKey2;

    if(!(hKey && pCallBack))
        return E_INVALIDARG;

    lret = RegOpenKeyEx(
      hKey,                   // handle to open key
      pszSubKey,              // address of name of subkey to open
      0,                      // reserved
      KEY_READ,               // security access mask
      &hKey2                  // address of handle to open key
    );

    if(lret!=ERROR_SUCCESS)
        return lret;

    dwIndex = 0;
    do
    {
        szSubKey[0]=0;
        dwMP = FC_ARRAY_LEN(szSubKey);
        lret = RegEnumKeyEx(
          hKey2,      // handle to key to enumerate
          dwIndex,    // index of subkey to enumerate
          szSubKey,   // address of buffer for subkey name
          &dwMP,      // address for size of subkey buffer
          NULL,       // reserved
          NULL,       // address of buffer for class string
          0,          // address for size of class buffer
          &lmft       // address for time key last written to
        );
        dwIndex++;

        if(lret==ERROR_SUCCESS)
            iAction = pCallBack(hKey2, szSubKey, pUserData);
        else if(lret==ERROR_NO_MORE_ITEMS)
            iAction = pCallBack(hKey2, NULL, pUserData);

        if(iAction<0)//iAction<0 stop immediately
            lret = ERROR_OPERATION_ABORTED;

        //recurse or skip this one if iAction=0;
        if(lret==ERROR_SUCCESS && iAction>0)
            lret = FC_RegEnumKeys(hKey2, szSubKey, pCallBack, pUserData);

    }while(lret==ERROR_SUCCESS);
    
    lret2 = RegCloseKey(hKey2);
    if(lret2!=ERROR_SUCCESS)
        return lret2;

    return lret==ERROR_NO_MORE_ITEMS ? ERROR_SUCCESS : lret;
}

/*
 *----------------------------------------------------------------------------*
 *  $History: fc_regenum.c $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CMN/fc_tools
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CMN/fc_tools
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 28.01.02   Time: 17:52
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * various bugfixes
 * 
 * *****************  Version 1  *****************
 * User: Ef           Date: 5.01.01    Time: 17:09
 * Created in $/4Control/COM/softing/fc/CMN/fc_tools
 * from V1.3 tools.dll and some new features
 *==
 *----------------------------------------------------------------------------*
*/
