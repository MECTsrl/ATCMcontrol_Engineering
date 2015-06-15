/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_tools/fc_regdel.c 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: fc_regdel.c $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_tools/fc_regdel.c $
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
 *==Helper function to delete a registry key and it's subkeys recursively
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  04.01.01  EF      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/


#include <tchar.h>
#include "fc_tools\fc_registry.h"



FC_TOAPI LONG FC_TODECL FC_RegRecursiveDelete(
    HKEY          hKey, 
    const TCHAR*  pszSubKey
)
{
    LONG      lret;
    LONG      lret2;
    DWORD     cSubKeys;
    DWORD     dwIndex;
    TCHAR     szSubKey[MAX_PATH];
    DWORD     dwMP;
    FILETIME  lmft;
    HKEY      hKey2;

    if(!(hKey && pszSubKey))
        return E_INVALIDARG;

    lret = RegOpenKeyEx(
      hKey,                   // handle to open key
      pszSubKey,              // address of name of subkey to open
      0,                      // reserved
      KEY_ENUMERATE_SUB_KEYS|KEY_QUERY_VALUE, // security access mask
      &hKey2                  // address of handle to open key
    );
    if(lret!=ERROR_SUCCESS)
        return lret;

    lret = FC_RegCountSubKeys(hKey2, &cSubKeys);
    if(lret!=ERROR_SUCCESS)
    {
        RegCloseKey(hKey2);
        return lret;
    }

    for(dwIndex=0; lret==ERROR_SUCCESS && dwIndex<cSubKeys;dwIndex++)
    {
        szSubKey[0]=0;
        dwMP = FC_ARRAY_LEN(szSubKey);
        lret = RegEnumKeyEx(
          hKey2,      // handle to key to enumerate
          0,          // index of subkey to enumerate
          szSubKey,   // address of buffer for subkey name
          &dwMP,      // address for size of subkey buffer
          NULL,       // reserved
          NULL,       // address of buffer for class string
          0,          // address for size of class buffer
          &lmft       // address for time key last written to
        );
        //recurse:
        if(lret==ERROR_SUCCESS)
            lret = FC_RegRecursiveDelete(hKey2, szSubKey);
    }
    
    if(lret==ERROR_SUCCESS)
        lret = RegDeleteKey(hKey,  pszSubKey);
       
    lret2 = RegCloseKey(hKey2);
    return lret==ERROR_SUCCESS?lret2:lret;
}



/*
 *----------------------------------------------------------------------------*
 *  $History: fc_regdel.c $
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
