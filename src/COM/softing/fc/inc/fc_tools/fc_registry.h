/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/inc/fc_tools/fc_registry.h 1     28.02.07 19:01 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: fc_registry.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/inc/fc_tools/fc_registry.h $
 *
 * =PROJECT             CAK1020  ATCMControl V2.0
 *
 * =SWKE                CMN
 *
 * =COMPONENT           fc_tools
 *
 * =CURRENT             $Date: 28.02.07 19:01 $
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
 *== Helper functions for registry operations
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  03.01.01  EF      File created
 *  see history at end of file !
 *==
 *******************************************************************************
 H<<*/

#ifndef FC_REGISTRY_H_
#define FC_REGISTRY_H_

#include <windows.h>
#include "fc_defines.h"

#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------*
/**
 * FC_RegSetStringVal sets a string value for a key, if the key does
 * exist it is created. This is a wrapper for RegCreateKeyEx and RegSetValueEx.
 *
 * @param           hKey: e.g. HKEY_CLASSES_ROOT.
 * @param           pszSubKey: e.g. "CLASSES\\{bf5d1650-e168-11d4-a535-0008c779c600}"
 * @param           pszValName: name of value or NULL or "" if unnamed.
 * @param           pszVal: the string value to set, must not be NULL
 *
 * @return          ERROR_SUCCESS or one of the error codes in winerror.h
 * @see             win32 RegCreateKeyEx and RegSetValueEx
*/
FC_TOAPI LONG FC_TODECL FC_RegSetStringVal(
    HKEY         hKey, 
    const TCHAR* pszSubKey, 
    const TCHAR* pszValName, 
    const TCHAR* pszVal
);

//------------------------------------------------------------------*
/**
 * FC_RegSetDwordVal sets a dword value for a key, if the key does
 * exist it is created. This is a wrapper for RegCreateKeyEx and RegSetValueEx.
 *
 * @param           hKey: e.g. HKEY_CLASSES_ROOT.
 * @param           pszSubKey: e.g. "CLASSES\\{bf5d1650-e168-11d4-a535-0008c779c600}"
 * @param           pszValName: name of value or NULL or "" if unnamed.
 * @param           dw: the DWORD value to set
 *
 * @return          ERROR_SUCCESS or one of the error codes in winerror.h
 * @see             win32 RegCreateKeyEx and RegSetValueEx
*/
FC_TOAPI LONG FC_TODECL FC_RegSetDwordVal(
    HKEY          hKey, 
    const TCHAR*  pszSubKey, 
    const TCHAR*  pszValName, 
    DWORD         dw
);



//------------------------------------------------------------------*
/**
 * FC_RegGetStringVal gets a null terminated string value for a key. 
 * This is a wrapper for RegCreateKeyEx and RegQueryValueEx.
 * This function cannot be used to determine the size of the string
 * buffer needed, it's intended to be used for the 'simple' cases
 * such as szVal[MAX_PATH].
 *
 * @param           hKey: e.g. HKEY_CLASSES_ROOT.
 * @param           pszSubKey: e.g. "CLASSES\\{bf5d1650-e168-11d4-a535-0008c779c600}"
 * @param           pszValName: name of value or NULL or "" for default value.
 * @param           szVal: pointer to TCHAR szVal[nBuff] to receive the result
 *                  If the function fails function fails szVal is undefined.
 * @param           nBuff: size of result buffer, must be >0.
 *
 * @return          ERROR_SUCCESS or one of the error codes in winerror.h.
 *                  If the value's data type is not REG_SZ or REG_EXPAND_SZ 
 *                  this function fails and returns ERROR_INVALID_DATA.
 *                  If nBuff was to small ERROR_MORE_DATA is returned.
 * @see             win32 RegOpenKeyEx and RegQueryValueEx
*/
FC_TOAPI LONG FC_TODECL FC_RegGetStringVal(
    HKEY           hKey, 
    const TCHAR*   pszSubKey, 
    const TCHAR*   pszValName, 
    TCHAR          szVal[/*nBuff*/],
    size_t         nBuff  
);


//------------------------------------------------------------------*
/**
 * FC_RegGetDwordVal gets a dword value for a key. This is a wrapper for 
 * RegOpenKeyEx and RegQueryValueEx.
 *
 * @param           hKey: e.g. HKEY_CLASSES_ROOT.
 * @param           pszSubKey: e.g. "CLASSES\\{bf5d1650-e168-11d4-a535-0008c779c600}"
 * @param           pszValName: name of value or NULL or "" for default value.
 * @param           pDWVal: pointer to DWORD to receive the result. If the 
 *                  function fails *pDWVal is undefined.
 *
 * @return          ERROR_SUCCESS or one of the error codes in winerror.h.
 *                  If the value's data type is not REG_DWORD this function
 *                  fails and returns ERROR_INVALID_DATA.
 * @see             win32 RegOpenKeyEx and RegQueryValueEx
*/
FC_TOAPI LONG FC_TODECL FC_RegGetDwordVal(
    HKEY           hKey, 
    const TCHAR*   pszSubKey, 
    const TCHAR*   pszValName, 
    DWORD*         pDWVal
);



//------------------------------------------------------------------*
/**
 * FC_RegGetStrByPath gets a null terminated string value for a key. 
 * This is a wrapper for  * RegOpenKeyEx and RegQueryValueEx.
 * This function cannot be used to determine the size of the string
 * buffer needed, it's intended to be used for the 'simple' cases
 * such as szVal[MAX_PATH]. The HKEY to be used as root is encoded as the 
 * first path field of pszPath and the value name is the last path field.
 * To address the default value pszPath must end in a simple back slash.
 *
 * If the type of data is not a string it is converted into a string using 
 * TODO: add info how it is converted (in the moment only DWORD is converted to)
 *
 * The possible names for HKEY roots are either long or short form and case 
 * insensitive, see FC_RegHYKYFromPath for details.
 *
 * @param           pszPath: name of HKEY\<sub key path>\[<value name] 
 *                           Example: HKLM\Software\Softing\4ControlV2\Path
 *                           Example: HKLM\Software\Softing\4ControlV2\
 * @param           flags:   reserved, must be 0 
 * @param           szVal:   pointer to TCHAR szVal[nBuff] to receive the result
 *                           If the function fails function fails szVal is undefined.
 * @param           nBuff:   size of result buffer, must be >0.
 *
 * @return          ERROR_SUCCESS or one of the error codes in winerror.h.
 *                  If nBuff was to small ERROR_MORE_DATA is returned.
 * @see             win32 RegOpenKeyEx and RegQueryValueEx, FC_RegHYKYFromPath
*/
FC_TOAPI LONG FC_TODECL FC_RegGetStrByPath(
    const TCHAR*   pszPath, 
    long           flags,
    TCHAR          szVal[/*nBuff*/],
    size_t         nBuff  
);

//------------------------------------------------------------------*
/**
 * FC_RegHYKYFromPath parses the first field of a path and matches the 
 * path names to HKEY handle values.
 * long:                         short:
 * HKEY_CLASSES_ROOT             HKCR
 * HKEY_CURRENT_USER             HKCU
 * HKEY_LOCAL_MACHINE            HKLM
 * HKEY_USERS                    HKUS
 * HKEY_PERFORMANCE_DATA         HKPD
 * HKEY_CURRENT_CONFIG           HKCC
 * HKEY_DYN_DATA                 HKDD
 * The HKEY names are case insensitive. 
 *
 * @param           pszPath: name of HKEY\<key path>[\[<subKey]] 
 *                           Example: HKLM\Software\Softing\4ControlV2\Path
 *                           Example: HKLM\Software\Softing\4ControlV2\
 * @param           ppszEnd: if not NULL: receives a pointer to the next path field
 *                           after a following \/ or if no \/ a pointer to the 0-char.
 *                           If the function returtn NULL *ppszEnd = pszPath.
 * 
 * @return          One of the predefined HKEYs or NULL if no match
*/
FC_TOAPI HKEY FC_TODECL FC_RegHYKYFromPath(
    const TCHAR*   pszPath, 
    const TCHAR**  ppszEnd   
);




//------------------------------------------------------------------*
/**
 * FC_RegCountSubKeys returns the number of subkeys, this is a wrapper
 * for win32 RegQueryInfoKey with all parameters NULL except lpcSubKeys.  
 *
 * @param           hKey: input the handle to the key to query
 * @param           pcSubKeys: output the number of subkeys
 * @return          ERROR_SUCCESS if no error (same as RegQueryInfoKey).
 * @see             win32 RegQueryInfoKey
*/
__inline LONG FC_RegCountSubKeys(
    HKEY   hKey, 
    DWORD* pcSubKeys
)
{
    return RegQueryInfoKey(
      hKey,    // handle to key to query
      NULL,    // address of buffer for class string
      NULL,    // address of size of class string buffer
      NULL,    // reserved
      pcSubKeys,// address of buffer for number of subkeys
      NULL,    // address of buffer for longest subkey name length
      NULL,    // address of buffer for longest class string length
      NULL,    // address of buffer for number of value entries
      NULL,    // address of buffer for longest value name length
      NULL,    // address of buffer for longest value data length
      NULL,    // address of buffer for security descriptor length
      NULL     // address address of buffer for last write time
     );
}


//------------------------------------------------------------------*
/**
 * FC_RegRecursiveDelete deletes hKey's pszSubKey and all it's child keys if any
 * Note: call this function with maximum care ! there is no undo. 
 *
 * @param           hKey: e.g. HKEY_CLASSES_ROOT.
 * @param           pszSubKey: e.g. "CLASSES\\{bf5d1650-e168-11d4-a535-0008c779c600}"
 *
 * @return          ERROR_SUCCESS or one of the error codes in winerror.h
*/
FC_TOAPI LONG FC_TODECL FC_RegRecursiveDelete(
    HKEY          hKey, 
    const TCHAR*  pszSubKey 
);









//------------------------------------------------------------------*
/**
 * FC_RegEnumCallBack called from FC_RegEnumKeys. The following example 
 * call back simply prints all key pathes from HKCR but skips HKCR\CLSID:
 *
    #include <stdio.h>
    #include "fc_todebug\fc_assert.h"
    #include "fc_tools\fc_string.h"
    #include "fc_tools\fc_registry.h"

    static int CALLBACK RegPrintReg(HKEY hKey, const TCHAR*  pszSubKey, void* pUserData)
    {
       FC_CString* pStr = (FC_CString*)pUserData;
       if(pszSubKey)
       {
           (*pStr)<<_T("\\")<<pszSubKey;
           printf("HKCR%s\n",pStr->psz());

           if(!strcmp(pStr->psz(), "\\CLSID"))
           {
               printf("      skip HKCR\\CLSID\n");
               pStr->cutAtLast(_T("\\"));
               return 0;
           }
       }
       else
       {
           pStr->cutAtLast(_T("\\"));
       }
       return 1;
    }
    static void TestRegistry()
    {
        FC_CString jot;
        LONG lret= FC_RegEnumKeys(HKEY_CLASSES_ROOT, NULL,RegPrintReg, &jot);
        FC_ASSERT_SYSERR(lret, "FC_RegEnumKeys");
    }
 *
 * @param         hKey:      an opended key, must not be closed by the callback
 * @param         pszSubKey: pszSubKey of the current hKey or NULL if hKey has 
 *                           no subkeys.
 *                           
 * @param         pUserData: user data pointer passed from FC_RegEnumKeys
 *
 * @return        <0:  FC_RegEnumKeys ends recursion immediately 
 *                FC_RegEnumKeys will return ERROR_OPERATION_ABORTED
 *                to propagate this event to the top level caller.
 *                =0:  Skip pszSubKey, FC_RegEnumKeys dosen't open it 
 *                but processes the next sub key, if any.
 *                
 *                >0:  Carry on enumerating the keys recursively 
*/
typedef int (CALLBACK* FC_RegEnumCallBack)(
    /*const*/HKEY hKey,      //an opended key, must not be closed by the callback
    const TCHAR*  pszSubKey, //pszSubKey of the current hKey
    void*         pUserData  //user data pointer passed from FC_RegEnumKeys
); 
//------------------------------------------------------------------*
/**
 * FC_RegEnumKeys can be used to recurse all subkeys of hKey->pszSubKey
 * and call a callback function FC_RegEnumCallBack for every key.
 *
 * @param           hKey: e.g. HKEY_CLASSES_ROOT.
 * @param           pszSubKey: e.g. "CLASSES\\{bf5d1650-e168-11d4-a535-0008c779c600}"
 * @param           pCallBack: the callback must not be NULL
 * @param           pUserData: user data for callback, may be NULL.
 * @return          ERROR_SUCSESS if all keys are processed or 
 *                  ERROR_OPERATION_ABORTED if the callback returns <0 to abort the recursion.
 *                  Or one of the error codes in winerror.h if any error.
*/
FC_TOAPI LONG FC_TODECL FC_RegEnumKeys(
    HKEY               hKey, 
    const TCHAR*       pszSubKey, 
    FC_RegEnumCallBack pCallBack,
    void*              pUserData
);


#ifdef __cplusplus
}
#endif


#endif //FC_REGISTRY_H_

/*
 *----------------------------------------------------------------------------*
 *  $History: fc_registry.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 19:01
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/inc/fc_tools
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:34
 * Created in $/4Control/COM/softing/fc/inc/fc_tools
 * 
 * *****************  Version 4  *****************
 * User: Ef           Date: 25.10.02   Time: 20:07
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * reg value by simple path, used for $reg(...)
 * 
 * *****************  Version 3  *****************
 * User: Ef           Date: 28.01.02   Time: 17:52
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * some new functions
 * 
 * *****************  Version 2  *****************
 * User: Ef           Date: 26.02.01   Time: 10:48
 * Updated in $/4Control/COM/softing/fc/inc/fc_tools
 * corrected SWKE name
 * 
 * *****************  Version 1  *****************
 * User: Ef           Date: 5.01.01    Time: 17:08
 * Created in $/4Control/COM/softing/fc/inc/fc_tools
 * from V1.3 tools.dll, and some new features
 *==
 *----------------------------------------------------------------------------*
*/
