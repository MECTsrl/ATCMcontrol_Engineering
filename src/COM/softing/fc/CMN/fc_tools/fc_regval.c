/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_tools/fc_regval.c 1     28.02.07 19:00 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: fc_regval.c $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CMN/fc_tools/fc_regval.c $
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
 *==Set/get value helper functions for registry operations
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  04.01.01  EF      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/


#include <tchar.h>
#include "fc_tools\fc_registry.h"
#include "fc_tools\fc_string.h"


FC_TOAPI LONG FC_TODECL FC_RegSetStringVal(
    HKEY         hKey, 
    const TCHAR* pszSubKey, 
    const TCHAR* pszValName, 
    const TCHAR* pszVal
)
{
    LONG  lret;
    LONG  lret2;
    HKEY  hkeySub = NULL;
    DWORD dwDisposition;

    if(!(hKey && pszSubKey && pszVal))
        return E_INVALIDARG;

    lret = RegCreateKeyEx(hKey, pszSubKey, 0, _T(""), 
               REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, 
		       &hkeySub, &dwDisposition);

    if(lret==ERROR_SUCCESS) 
    {
        lret = RegSetValueEx(hkeySub, pszValName, 0, REG_SZ, 
               (CONST BYTE*)pszVal, (_tcslen(pszVal)+1)*sizeof(TCHAR));
    }

    if(hkeySub)
    {
        lret2 = RegCloseKey(hkeySub);
        if(lret==ERROR_SUCCESS)
            lret = lret2;
    }
    return lret;
}


FC_TOAPI LONG FC_TODECL FC_RegSetDwordVal(
    HKEY          hKey, 
    const TCHAR*  pszSubKey, 
    const TCHAR*  pszValName, 
    unsigned long dwVal
)
{
    LONG  lret;
    LONG  lret2;
    HKEY  hkeySub = NULL;
    DWORD dwDisposition;

    if(!(hKey && pszSubKey))
        return E_INVALIDARG;

    lret = RegCreateKeyEx(hKey, pszSubKey, 0, _T(""), 
               REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, 
		       &hkeySub, &dwDisposition);

    if(lret==ERROR_SUCCESS) 
    {
        lret = RegSetValueEx(hkeySub, pszValName, 0, REG_DWORD, 
               (CONST BYTE*)&dwVal, sizeof(unsigned long));
    }

    if(hkeySub)
    {
        lret2 = RegCloseKey(hkeySub);
        if(lret==ERROR_SUCCESS)
            lret = lret2;
    }
    return lret;
}



FC_TOAPI LONG FC_TODECL FC_RegGetStringVal(
    HKEY           hKey, 
    const TCHAR*   pszSubKey, 
    const TCHAR*   pszValName, 
    TCHAR          szVal[/*nBuff*/],
    size_t         nBuff  
)
{
    LONG  lret;
    LONG  lret2;
    HKEY  hkeySub = NULL;
    DWORD dwType;
    DWORD cbData = nBuff*sizeof(TCHAR);


    if(!(hKey && pszSubKey && szVal && nBuff>0))
        return E_INVALIDARG;

    lret = RegOpenKeyEx(hKey, pszSubKey, 0, KEY_READ, &hkeySub);

    if(lret==ERROR_SUCCESS) 
    {
        //check the data type:
        lret = RegQueryValueEx(
          hkeySub,        // handle to key
          pszValName,     // value name
          NULL,           // reserved
          &dwType,        // type buffer
          NULL,           // data buffer
          NULL            // size of data buffer
        );
        if(lret==ERROR_SUCCESS && dwType!=REG_SZ && dwType!=REG_EXPAND_SZ)
            lret = ERROR_INVALID_DATA;

        if(lret==ERROR_SUCCESS)
        {
            lret = RegQueryValueEx(
              hkeySub,        // handle to key
              pszValName,     // value name
              NULL,           // reserved
              &dwType,        // type buffer
              (LPBYTE)szVal,  // data buffer
              &cbData         // size of data buffer
            );
        }
    }

    if(hkeySub)
    {
        lret2 = RegCloseKey(hkeySub);
        if(lret==ERROR_SUCCESS)
            lret = lret2;
    }
    return lret;
}



FC_TOAPI LONG FC_TODECL FC_RegGetDwordVal(
    HKEY           hKey, 
    const TCHAR*   pszSubKey, 
    const TCHAR*   pszValName, 
    DWORD*         pDWVal
)
{
    LONG  lret;
    LONG  lret2;
    HKEY  hkeySub = NULL;
    DWORD dwType;
    DWORD cbData = sizeof(DWORD);


    if(!(hKey && pszSubKey && pDWVal))
        return E_INVALIDARG;

    lret = RegOpenKeyEx(hKey, pszSubKey, 0, KEY_READ, &hkeySub);

    if(lret==ERROR_SUCCESS) 
    {
        //check the data type:
        lret = RegQueryValueEx(
          hkeySub,        // handle to key
          pszValName,     // value name
          NULL,           // reserved
          &dwType,        // type buffer
          NULL,           // data buffer
          NULL            // size of data buffer
        );
        if(lret==ERROR_SUCCESS && dwType!=REG_DWORD)
            lret = ERROR_INVALID_DATA;

        if(lret==ERROR_SUCCESS)
        {
            lret = RegQueryValueEx(
              hkeySub,        // handle to key
              pszValName,     // value name
              NULL,           // reserved
              &dwType,        // type buffer
              (LPBYTE)pDWVal, // data buffer
              &cbData         // size of data buffer
            );
        }
    }

    if(hkeySub)
    {
        lret2 = RegCloseKey(hkeySub);
        if(lret==ERROR_SUCCESS)
            lret = lret2;
    }
    return lret;
}





FC_TOAPI LONG FC_TODECL FC_RegGetStrByPath(
    const TCHAR*   pszPath, 
    long           flags,
    TCHAR          szVal[/*nBuff*/],
    size_t         nBuff  
)
{
    HKEY         hKey;
    LONG         lret;
    LONG         lret2;
    HKEY         hkeySub;
    const TCHAR* pszValName;
    TCHAR        szSubKey[2*MAX_PATH];
    DWORD        dwType;
    DWORD        cbData;

    if(nBuff==0)
        lret = ERROR_BUFFER_OVERFLOW;

    szVal[0] = 0;

    hKey = FC_RegHYKYFromPath(pszPath, &pszValName);
    if(!hKey)
        return ERROR_PATH_NOT_FOUND;

    if(!FC_StringCopyToBuffer(szSubKey, FC_ARRAY_LEN(szSubKey), pszValName))
        return ERROR_BUFFER_OVERFLOW;

    pszValName = FC_StringGetFilePart(pszPath);
    FC_StringCutFilePart(szSubKey);

    lret = RegOpenKeyEx(hKey, szSubKey, 0, KEY_READ, &hkeySub);

    if(lret==ERROR_SUCCESS) 
    {
        //check the data type:
        cbData = nBuff*sizeof(TCHAR);
        lret = RegQueryValueEx(
          hkeySub,        // handle to key
          pszValName,     // value name
          NULL,           // reserved
          &dwType,        // type buffer
          (LPBYTE)szVal,  // data buffer
          &cbData         // size of data buffer
        );

        if(lret==ERROR_SUCCESS)
        {
            if(dwType==REG_SZ || dwType==REG_EXPAND_SZ)
            {
                ;
            }
            else if(dwType==REG_DWORD)
            {
                szSubKey[0] = '0';
                szSubKey[1] = 'x';
                _ultot(*(DWORD*)szVal, szSubKey+2, 16);
                if(!FC_StringCopyToBuffer(szVal, nBuff, szSubKey))
                    lret = ERROR_MORE_DATA;
            }
            else
            {
                lret = ERROR_INVALID_DATA;//TODO
            }
        }
    }

    if(hkeySub)
    {
        lret2 = RegCloseKey(hkeySub);
        if(lret==ERROR_SUCCESS)
            lret = lret2;
    }
    return lret;
}



FC_TOAPI HKEY FC_TODECL FC_RegHYKYFromPath(
    const TCHAR*   pszPath, 
    const TCHAR**  ppszEnd   
)
{
    HKEY hKey;

    if(FC_StringIncBeginI(&pszPath, _T("HKCR")) || 
       FC_StringIncBeginI(&pszPath, _T("HKEY_CLASSES_ROOT"))
      )
    {
        hKey = HKEY_CLASSES_ROOT;
    }
    else if(FC_StringIncBeginI(&pszPath, _T("HKEY_CURRENT_USER")) || 
       FC_StringIncBeginI(&pszPath, _T("HKCU"))
      )
    {
        hKey = HKEY_CURRENT_USER;
    }
    else if(FC_StringIncBeginI(&pszPath, _T("HKLM")) || 
       FC_StringIncBeginI(&pszPath, _T("HKEY_LOCAL_MACHINE"))
      )
    {
        hKey = HKEY_LOCAL_MACHINE;
    }
    else if(FC_StringIncBeginI(&pszPath, _T("HKEY_USERS")) || 
       FC_StringIncBeginI(&pszPath, _T("HKUS"))
      )
    {
        hKey = HKEY_USERS;
    }
    else if(FC_StringIncBeginI(&pszPath, _T("HKEY_PERFORMANCE_DATA")) || 
       FC_StringIncBeginI(&pszPath, _T("HKPD"))
      )
    {
        hKey = HKEY_PERFORMANCE_DATA;
    }
    else if(FC_StringIncBeginI(&pszPath, _T("HKEY_CURRENT_CONFIG")) || 
       FC_StringIncBeginI(&pszPath, _T("HKCC"))
      )
    {
        hKey = HKEY_CURRENT_CONFIG;
    }
    else if(FC_StringIncBeginI(&pszPath, _T("HKEY_DYN_DATA")) || 
       FC_StringIncBeginI(&pszPath, _T("HKDD"))
      )
    {
        hKey = HKEY_DYN_DATA;
    }
    else
    {
        hKey = NULL;
    }

    if(ppszEnd)
        *ppszEnd = (*pszPath=='\\' || *pszPath=='/') ? pszPath+1 : pszPath;

    return hKey;
}






/*
 *----------------------------------------------------------------------------*
 *  $History: fc_regval.c $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:00
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CMN/fc_tools
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:33
 * Created in $/4Control/COM/softing/fc/CMN/fc_tools
 * 
 * *****************  Version 4  *****************
 * User: Ef           Date: 25.10.02   Time: 20:08
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * reg value by simple path, used for $reg(...)
 * 
 * *****************  Version 3  *****************
 * User: Ef           Date: 11.03.02   Time: 15:08
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * removed dead code
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 26.03.01   Time: 11:47
 * Updated in $/4Control/COM/softing/fc/CMN/fc_tools
 * bug fix in reg get functions: use reg open and not reg create
 * 
 * *****************  Version 1  *****************
 * User: Ef           Date: 5.01.01    Time: 17:09
 * Created in $/4Control/COM/softing/fc/CMN/fc_tools
 * from V1.3 tools.dll and some new features
 *==
 *----------------------------------------------------------------------------*
*/
