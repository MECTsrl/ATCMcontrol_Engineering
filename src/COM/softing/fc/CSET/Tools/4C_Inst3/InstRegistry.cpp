// InstRegistry.cpp: Implementierung der Klasse CInstRegistry.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "InstRegistry.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



BOOL DoesLocalMachineKeyExist(LPCSTR szKey)
{
    // checks if a key does exist in the HKEY_LOCAL_MACHINE section
    HKEY hKey;

    if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, szKey, 0, KEY_QUERY_VALUE, &hKey))
    {
        return FALSE;
    }

    RegCloseKey(hKey);
    return TRUE;
}

BOOL FindRegistryKey(HKEY hKey, LPCSTR pszRegistryKey, LPCSTR pszSubString,
                        LPSTR pValue, DWORD * pdwSize)
{
    // Finds a key in the registry and retruns its first subkey's name
    HKEY hKeyMs;
    BOOL bRet= TRUE;
	DWORD dwIndex = 0;
	LPSTR pszName = new CHAR[MAX_PATH];
	DWORD dwSize = MAX_PATH;

    if (ERROR_SUCCESS != RegOpenKeyEx(hKey, pszRegistryKey, 0, KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS, &hKeyMs))
    {
        bRet = FALSE;
    }
	else
	{
		while (ERROR_SUCCESS == RegEnumKeyEx(hKeyMs, dwIndex++, pszName, &dwSize, NULL, NULL, NULL, NULL))
		{
			dwSize = MAX_PATH;
			if (!strncmp(pszName ,pszSubString, strlen(pszSubString) - 1) )
			{
				HKEY hKeySdk;
				if (ERROR_SUCCESS != RegOpenKeyEx(hKeyMs, pszName, 0, KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS, &hKeySdk))
				{
					bRet = FALSE;
				}
				if (ERROR_SUCCESS != RegEnumKeyEx(hKeySdk, 0, pValue, &dwSize, NULL, NULL, NULL, NULL))
				{
					bRet = FALSE;
				}
				RegCloseKey(hKeySdk);
				break;
			}
		}
	}

	delete pszName;

    RegCloseKey(hKeyMs);
    return bRet;
}


BOOL QueryRegistryValue(HKEY hKey, LPCSTR pszRegistryKey, LPCSTR pszName, DWORD * pdwType, 
                        LPBYTE pValue, DWORD * pdwSize)
{
    // Reads a value from the registry
    HKEY hKeyReg;
    BOOL bRet= TRUE;

    if (ERROR_SUCCESS != RegOpenKeyEx(hKey, pszRegistryKey, 0, KEY_QUERY_VALUE, &hKeyReg))
    {
        return FALSE;
    }

    if (ERROR_SUCCESS != RegQueryValueEx(hKeyReg, pszName, 0, pdwType, pValue, pdwSize))
    {
        bRet = FALSE;
    }

    RegCloseKey(hKeyReg);
    return bRet;
}



