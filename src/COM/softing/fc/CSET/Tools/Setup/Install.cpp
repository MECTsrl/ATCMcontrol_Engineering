/* ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: Install.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CSET/Tools/Setup/Install.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				CSET
 *
 * =COMPONENT			Tools\Setup
 *
 * =CURRENT 	 $Date: 31.01.08 19:26 $
 *			 $Revision: 2 $
 *
 * ------------------------------------------------------------------------------
 */
 
#include "stdafx.h"
#include "Install.h"
#include "SetupDef.h"
#include "Resource.h"
#include "PropBase.h"
#include "PropSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CInstall, CObject)

typedef UINT			(WINAPI *PMsiEnableLogA)(DWORD, LPCSTR, BOOL);    
typedef INSTALLUILEVEL	(WINAPI *PMsiSetInternalUI)(INSTALLUILEVEL, HWND *);
typedef UINT			(WINAPI *PMsiInstallProductA)(LPCSTR , LPCSTR);
typedef UINT			(WINAPI *PMsiConfigureProductA)(LPCSTR, int, INSTALLSTATE);
typedef UINT			(WINAPI *PMsiGetProductInfoA)(LPCSTR, LPCSTR, LPSTR,	DWORD *);


/* CInstall
 * ----------------------------------------------------------------------------
 */
CInstall::CInstall()
{
	m_bInstallAll		= FALSE;
	m_bUnInstallAll		= FALSE;
	m_bUnInstallOnly	= FALSE;
	m_bAutoClose		= FALSE;

	m_bDebugLog		= FALSE;

	m_hTrace		= NULL;
	m_bTrace		= FALSE;

	m_pListCtrl		= NULL;
	m_bDisable		= FALSE;

	m_dwToDo		= 0;
	m_wMaxKey		= 0;
}


/* ~CInstall
 * ----------------------------------------------------------------------------
 */
CInstall::~CInstall()
{
	DestroyLBContents();

	m_pListCtrl = NULL;
}


/* InitializeInstaller
 * ----------------------------------------------------------------------------
 */
void CInstall::InitializeInstaller()
{
	/* Initialize Trace mechanism
	 */
	InitTrace();

    /* Set Windows Text
	 */
    GetKeyFromIni(SET_SCT_GENERAL, SET_KEY_CAPTION, m_sTitle);

	/* Get Filename
	 */
	GetModuleFileName(NULL, m_sModule.GetBuffer(MAX_PATH), MAX_PATH);
	m_sModule.ReleaseBuffer();
	
    GetKeyFromIni(SET_SCT_GENERAL, SET_KEY_PNAME, m_sProduct);

	Trc("OnInitDialog", "", "Module = %s", m_sModule);
	Trc("OnInitDialog", "", "Product = %s", m_sProduct);
	Trc("OnInitDialog", "", "All = %d", m_bInstallAll);

	GetKeyFromIni(SET_SCT_GENERAL, SET_KEY_TYPE, m_sType);


	/* Get ToDo List from registry
	 */
	CString sRegKey;
	sRegKey = m_sType == SET_TYPE_PC ? SET_HVAL_4C_COMPONENTS : GetEmbeddedKey();

	m_dwToDo = 0;
	DWORD dwSize = sizeof(m_dwToDo);

	if (TestRegKey(HKEY_LOCAL_MACHINE, SET_HKEY_SETUP)
		&& QueryRegValue(HKEY_LOCAL_MACHINE, SET_HKEY_SETUP, sRegKey, (LPBYTE)&m_dwToDo, &dwSize))
	{
		; // nix!
	}
	else
	{
		// No registry entry, use default.
		m_dwToDo = GetDefaultToDo();
	}			

	
	/* Get install language from registry
	 */
	m_dwLanguage = 0;
	dwSize = sizeof(m_dwLanguage);

	if (TestRegKey(HKEY_LOCAL_MACHINE, SET_HKEY_SETUP) 
		&& QueryRegValue(HKEY_LOCAL_MACHINE, SET_HKEY_SETUP, SET_HVAL_LANGUAGE, (LPBYTE)&m_dwLanguage, &dwSize))
	{
		; // nix!
	}
	else
	{
		// No registry entry, use default
		if (GetLanguage() == LANG_GERMAN)
		{
			m_dwLanguage	= SET_LANGID_DE;
		}
		else
		{
			m_dwLanguage	= SET_LANGID_EN;
		}
	}


	/* Get install Path from registry
	 */
	m_sInstallPath.Empty();

	dwSize = MAX_PATH;
	char szPath[MAX_PATH];

	if (TestRegKey(HKEY_LOCAL_MACHINE, SET_HKEY_SETUP)
		&& QueryRegValue(HKEY_LOCAL_MACHINE, SET_HKEY_SETUP, SET_HVAL_INSTPATH, (LPBYTE)szPath, &dwSize))
	{
		m_sInstallPath = szPath;
	}
	else
	{
		if (m_sInstallPathCmdLine.IsEmpty())
		{
			CString sPath;
			if (GetInstallPath(sPath))
			{
				m_sInstallPath = sPath;
			}
		}
		else
		{
			m_sInstallPath = m_sInstallPathCmdLine;
		}
	}

	/* Create Image List
	 */
	CreateImageList();
}


/* SetInstallPath
 * ----------------------------------------------------------------------------
 */
void CInstall::SetInstallPath(LPCTSTR szInstallPath)
{
	m_sInstallPath = szInstallPath;
}


/* GetInstallPath
 * ----------------------------------------------------------------------------
 */
LPCTSTR CInstall::GetInstallPath()
{
	return m_sInstallPath;
}


/* SetInstallPathCmdLine
 * ----------------------------------------------------------------------------
 */
void CInstall::SetInstallPathCmdLine(LPCTSTR szInstallPath)
{
	m_sInstallPathCmdLine = szInstallPath;
}


/* SetInstallLang
 * ----------------------------------------------------------------------------
 */
void CInstall::SetInstallLang(DWORD dwInstallLang)
{
	m_dwLanguage = dwInstallLang;
}


/* GetTitle
 * ----------------------------------------------------------------------------
 */
LPCTSTR CInstall::GetTitle()
{
	return m_sTitle;
}


/* SetDebugLog
 * ----------------------------------------------------------------------------
 */
void CInstall::SetDebugLog(BOOL bOn)
{
    m_bDebugLog = bOn;
}


/* SetInstallAll
 * ----------------------------------------------------------------------------
 */
void CInstall::SetInstallAll(BOOL bInstallAll)
{
	m_bInstallAll = bInstallAll;
}


/* SetUnInstallAll
 * ----------------------------------------------------------------------------
 */
void CInstall::SetUnInstallAll(BOOL bUnInstallAll)
{
	m_bUnInstallAll = bUnInstallAll;
}


/* SetUnInstallOnly
 * ----------------------------------------------------------------------------
 */
void CInstall::SetUnInstallOnly(BOOL bUnInstallOnly)
{
	m_bUnInstallOnly = bUnInstallOnly;
}


/* GetInstallAll
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::GetInstallAll()
{
	return m_bInstallAll;
}


/* GetUnInstallAll
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::GetUnInstallAll()
{
	return m_bUnInstallAll;
}


/* GetUnInstallOnly
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::GetUnInstallOnly()
{
	return m_bUnInstallOnly;
}


/* SetAutoClose
 * ----------------------------------------------------------------------------
 */
void CInstall::SetAutoClose(BOOL bAutoClose)
{
	m_bAutoClose = bAutoClose;
}


/* GetAutoClose
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::GetAutoClose()
{
	return m_bAutoClose;
}


/* SetType
 * ----------------------------------------------------------------------------
 */
void CInstall::SetType(LPCSTR szType)
{
	m_sType = szType;
}


/* GetType
 * ----------------------------------------------------------------------------
 */
LPCTSTR	CInstall::GetType()
{
	return m_sType;
}



/* C O M M O N  H E L P E R  F U N C T I O N S
 * ----------------------------------------------------------------------------
 */

/* IsWindowsNTBased
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::IsWindowsNTBased(void)
{
    OSVERSIONINFO VI;

    VI.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&VI);

    return VI.dwPlatformId == VER_PLATFORM_WIN32_NT;
}

/* IsWindowsVista
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::IsWindowsVista(void)
{
    OSVERSIONINFO VI;
	
    VI.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&VI);

	if (VI.dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		return VI.dwMajorVersion == 6 && VI.dwMinorVersion <= 1 ; //== 0;
	}

	return FALSE;
}

/* IsWindows2K3
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::IsWindows2K3(void)
{
    OSVERSIONINFO VI;
	
    VI.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&VI);

	if (VI.dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		return VI.dwMajorVersion == 5 && VI.dwMinorVersion == 2;
	}

	return FALSE;
}

/* IsWindowsXP
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::IsWindowsXP(void)
{
    OSVERSIONINFO VI;
	
    VI.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&VI);

	if (VI.dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		return VI.dwMajorVersion == 5 && VI.dwMinorVersion == 1;
	}

	return FALSE;
}

/* IsWindows2K
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::IsWindows2K(void)
{
    OSVERSIONINFO VI;
	
    VI.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&VI);

	if (VI.dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		return VI.dwMajorVersion == 5 && VI.dwMinorVersion == 0;
	}

	return FALSE;
}

/* IsWindowsNT4
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::IsWindowsNT4(void)
{
    OSVERSIONINFO VI;
	
    VI.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&VI);

	if (VI.dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		return VI.dwMajorVersion == 4;
	}

	return FALSE;
}

/* IsAdmin
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::IsAdmin(void)
{
  SC_HANDLE hSC;

  if (IsWindowsNTBased())
  {
      hSC = OpenSCManager(NULL, NULL, GENERIC_READ | GENERIC_WRITE | GENERIC_EXECUTE);
      if (!hSC)
      {
        return FALSE;
      }

      CloseServiceHandle(hSC);
  }

  return TRUE;
}


/* IsDirectory
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::IsDirectory(LPCTSTR lpszPath)
{
	DWORD dwRet = GetFileAttributes(lpszPath);
	return (dwRet != 0xFFFFFFFF) && (dwRet & FILE_ATTRIBUTE_DIRECTORY);
}


/* GetFileVersion
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::GetFileVersion(LPCTSTR szPath, CString &sVersion)
{
    DWORD dwDummy;
    DWORD dwSize = GetFileVersionInfoSize((LPSTR)szPath, &dwDummy);

    if (dwSize == 0)
    {
        return FALSE;
    }

    LPVOID p = malloc(dwSize);
    if (NULL != p)
    {
	    VS_FIXEDFILEINFO *pVersion;
        
		GetFileVersionInfo((LPTSTR)szPath, 0, dwSize, p);
        VerQueryValue(p, "\\", (LPVOID *)&pVersion, (UINT *)&dwSize);
        
		sVersion.Format("%d.%d.%d.%d", HIWORD(pVersion->dwFileVersionMS), LOWORD(pVersion->dwFileVersionMS),
									   HIWORD(pVersion->dwFileVersionLS), LOWORD(pVersion->dwFileVersionLS));

        free(p);
    }

    return TRUE;
}


/* GetInstallPath
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::GetInstallPath(CString &sPath)
{
	sPath.Empty();

	char szPath[MAX_PATH];
	DWORD dwSize = MAX_PATH;

	if (! QueryRegValue(HKEY_LOCAL_MACHINE, SET_HKEY_INSTPATH, SET_HVAL_INSTPATH, (BYTE *)szPath, &dwSize))
	{
		dwSize = MAX_PATH;
		
		if (! QueryRegValue(HKEY_LOCAL_MACHINE, SET_HKEY_CURRVERSION, SET_HVAL_PROGDIR, (BYTE *)szPath, &dwSize))
		{
			Trc("GetInstallPath", "ERROR", "QueryRegValue - %d.", GetLastError());
			return FALSE;
		}

		sPath  = szPath;
		sPath += '\\';
		sPath += SET_DEFDIR;
		sPath += '\\';
	}
	else
	{
		sPath = szPath;
	}

    return TRUE;
}


/* CheckVersionNumber
 * ----------------------------------------------------------------------------
 */
UINT CInstall::CheckVersionNumber(LPCTSTR szVersion, LPCTSTR szMin)
{
	CString sVer(szVersion);
	CString sMin(szMin);

	CString sValVer;
	CString sValMin;

	int i = 0;
	int j = 0;

	while(1)
	{
		i = sVer.Find('.');
		j = sMin.Find('.');

		if (i == -1 || j == -1)
		{
			sValVer = sVer;
			sValMin = sMin;
		}
		else
		{
			sValVer = sVer.Left(i);
			sValMin = sMin.Left(j);
		}

		char *szDummy;
		int iDiff = strtoul(sValVer, &szDummy, 10) - strtoul(sValMin, &szDummy, 10);
		
		if (iDiff < 0)
		{
			return IDS_REINSTALL;
		}

		if (iDiff > 0)
		{
			return IDS_OK;
		}

		if (i == -1 || j == -1)
		{
			break;
		}

		sVer = sVer.Right(sVer.GetLength() - i - 1);
		sMin = sMin.Right(sMin.GetLength() - j - 1);
	}
	
	return IDS_OK;
}


/* GetLanguage
 * ----------------------------------------------------------------------------
 */
UINT CInstall::GetLanguage()
{
    LCID lcid = GetSystemDefaultLCID();

	return PRIMARYLANGID(LANGIDFROMLCID(lcid));
}


/* GetLanguageSuffix
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::GetLanguageSuffix(CString &sSuffix)
{
	sSuffix.Empty();

	CString sLang;
    LCID lcid = GetSystemDefaultLCID();
	sLang.Format("%d", PRIMARYLANGID(LANGIDFROMLCID(lcid)));

	GetKeyFromIni(SET_SCT_LANGUAGE, sLang, sSuffix);

    return ! sSuffix.IsEmpty();
}


/* TranslateKey
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::TranslateKey(CString &sKey)
{
    OSVERSIONINFO VI;
	
    VI.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&VI);

	if (VI.dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		switch (VI.dwMajorVersion)
		{
			case 4 :
			{
				// Windows NT 4.0
				sKey += '_';
				sKey += SET_SUFFIX_NT;
				return TRUE;

			} /* case 4 */

			case 5 :
			{
				switch(VI.dwMinorVersion)
				{
					case 0 :
					{
						// Windows 2000
						sKey += '_';
						sKey += SET_SUFFIX_2K;
						return TRUE;
					}
					case 1 :
					{
						// Windows XP
						sKey += '_';
						sKey += SET_SUFFIX_XP;
						return TRUE;
					}
					case 2 :
					{
						// Windows Server 2003
						sKey += '_';
						sKey += SET_SUFFIX_2K3;
						return TRUE;
					}
				}

				break;
			
			} /* case 5 */

			case 6 :
			{
				switch(VI.dwMinorVersion)
				{
					case 0 :
					case 1 :
					{
						// Windows Vista
						sKey += '_';
						sKey += SET_SUFFIX_VI;
						return TRUE;
					}
				}

				break;

			} /* case 6 */

		} /* switch (VI.dwMajorVersion) */
	}
	else
	{
		// Windows 9x
		sKey += '_';
		sKey += SET_SUFFIX_9X;
		return TRUE;
	}

	return FALSE;
}


/* GetEmbeddedKey
 * ----------------------------------------------------------------------------
 */
LPCTSTR CInstall::GetEmbeddedKey()
{
	static char szKey[MAX_PATH];
	CString sValue;

	GetKeyFromIni(SET_SCT_GENERAL, SET_KEY_CUST, sValue);

	sprintf(szKey, SET_HVAL_4CE_COMPONENTS, (LPCTSTR)sValue);

	return szKey;
}


/* I N I   F I L E   H A N D L I N G
 * ----------------------------------------------------------------------------
 */

/* SetIniFile
 * ----------------------------------------------------------------------------
 */
void CInstall::SetIniFile(LPCTSTR szIniCommon, LPCTSTR szIniSpecial, LPCTSTR szIniLanguage)
{
	m_sIniCommon	= szIniCommon;
	m_sIniSpecial	= szIniSpecial;
	m_sIniLanguage	= szIniLanguage;
}


/* GetExeDir
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::GetExeDir(CString &sDir)
{
	sDir.Empty();

	char szDir[MAX_PATH];

	if (GetModuleFileName(NULL, szDir, MAX_PATH) == 0)
	{
		Trc("GetExeDir", "ERROR", "GetModuleFileName - %d.", GetLastError());
		return FALSE;
	}
	
	sDir = szDir;
	sDir = sDir.Left(sDir.ReverseFind('\\') + 1);

	return TRUE;
}


/* GetIniFileComm
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::GetIniFileCommon(CString &sIniCommon)
{
	if (! GetExeDir(sIniCommon))
	{
		return FALSE;
	}

	sIniCommon += m_sIniCommon;

	return TRUE;
}


/* GetIniFileInst
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::GetIniFileSpecial(CString &sIniSpecial)
{
	if (! GetExeDir(sIniSpecial))
	{
		return FALSE;
	}

	sIniSpecial += m_sIniSpecial;

	return TRUE;
}


/* GetIniFileLanguage
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::GetIniFileLanguage(CString &sIniLanguage)
{
	if (! GetExeDir(sIniLanguage))
	{
		return FALSE;
	}

	sIniLanguage += m_sIniLanguage;

	return TRUE;
}


/* GetKeyFromIni
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::GetKeyFromIni(LPCTSTR szSection, LPCTSTR szKey, CString &sValue)
{
    sValue.Empty();

	CString sIni;
	
	if (stricmp(szSection, SET_SCT_LANGUAGE) == 0)
	{
		if (! GetIniFileLanguage(sIni))
		{
			return FALSE;
		}
	}
	else if (stricmp(szSection, SET_SCT_GENERAL) == 0 || stricmp(szSection, SET_SCT_CONVERSION) == 0)
	{
		if (! GetIniFileSpecial(sIni))
		{
			return FALSE;
		}
	}
	else
	{
		if (! GetIniFileCommon(sIni))
		{
			return FALSE;
		}
	}

    char szValue[MAX_PATH];

	GetPrivateProfileString(szSection, szKey, "", szValue, MAX_PATH, sIni);

	sValue = szValue;

	return TRUE;
}


/* CheckValueInIni
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::CheckValueInIni(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szValue)
{
	CString sValues;
	CString sTemp;
	
	if (! GetKeyFromIni(szSection, szKey, sValues))
	{
		return FALSE;
	}

	while (! sValues.IsEmpty())
	{
		int iPos = sValues.Find(",");
	
		if (iPos == -1) {
			sTemp = sValues;
		} else 	{
			sTemp = sValues.Left(iPos);
		}

		if (sTemp.Compare(szValue) == 0)
		{
			return TRUE;
		}

		if (iPos == -1) {
			sValues.Empty();
		} else {
			sTemp	= sValues.Mid(iPos + 1);
			sValues	= sTemp;
		}
	}

	return FALSE;
}


/* R E G I S T R Y   A C E S S
 * ----------------------------------------------------------------------------
 */

/* CreateRegKey
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::CreateRegKey(HKEY hKey, LPCSTR szSub, LPCSTR szVal, DWORD dwType, LPBYTE pValue, DWORD dwSize)
{
    HKEY  hKeyReg;
	DWORD dwDisp;

    if (ERROR_SUCCESS != RegCreateKeyEx(hKey, szSub, 0, "", 0, KEY_SET_VALUE, NULL, &hKeyReg, &dwDisp))
    {
		Trc("CreateRegKey", "ERROR", "RegCreateKeyEx - %d.", GetLastError());
        return FALSE;
    }

    BOOL bRet = RegSetValueEx(hKeyReg, szVal, 0, dwType, pValue, dwSize) == ERROR_SUCCESS;
    RegCloseKey(hKeyReg);

    return bRet;
}


/* DeleteRegistryKey
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::DeleteRegKey(HKEY hKey, LPCSTR szKey, LPCSTR szSub)
{
	HKEY hKeyReg;

	if (ERROR_SUCCESS != RegOpenKeyEx(hKey, szKey, 0, KEY_SET_VALUE, &hKeyReg))
	{
		Trc("DeleteRegKey", "ERROR", "RegOpenKeyEx - %d.", GetLastError());
		return FALSE;
	}

	BOOL bRet = RegDeleteKey(hKeyReg, szSub);
	RegCloseKey(hKeyReg);

	return bRet;
}


/* DeleteRegistryKey
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::DeleteRegValue(HKEY hKey, LPCSTR szSub, LPCSTR szVal)
{
	HKEY hKeyReg;

	if (ERROR_SUCCESS != RegOpenKeyEx(hKey, szSub, 0, KEY_SET_VALUE, &hKeyReg))
	{
		Trc("DeleteRegValue", "ERROR", "RegOpenKeyEx - %d.", GetLastError());
		return FALSE;
	}

	BOOL bRet = RegDeleteValue(hKeyReg, szVal);
	RegCloseKey(hKeyReg);

	return bRet;
}


/* QueryRegValue
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::QueryRegValue(HKEY hKey, LPCSTR szSub, LPCSTR szVal, LPBYTE pValue, DWORD *pdwSize)
{
    HKEY  hKeyReg;
	DWORD dwType;

    if (ERROR_SUCCESS != RegOpenKeyEx(hKey, szSub, 0, KEY_QUERY_VALUE, &hKeyReg))
    {
		Trc("QueryRegValue", "ERROR", "RegOpenKeyEx - %d.", GetLastError());
        return FALSE;
    }

	BOOL bRet = RegQueryValueEx(hKeyReg, szVal, 0, &dwType, pValue, pdwSize) == ERROR_SUCCESS;
    RegCloseKey(hKeyReg);

    return bRet;
}


/* SetRegistryValue
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::SetRegValue(HKEY hKey, LPCSTR szSub, LPCSTR szVal, DWORD dwType, LPBYTE pValue, DWORD dwSize)
{
    HKEY hKeyReg;

    if (ERROR_SUCCESS != RegOpenKeyEx(hKey, szSub, 0, KEY_SET_VALUE, &hKeyReg))
    {
		Trc("SetRegValue", "ERROR", "RegOpenKeyEx - %d.", GetLastError());
        return FALSE;
    }

    BOOL bRet = RegSetValueEx(hKeyReg, szVal, 0, dwType, pValue, dwSize) == ERROR_SUCCESS;
    RegCloseKey(hKeyReg);

    return bRet;
}


/* TestRegKey
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::TestRegKey(HKEY hKey, LPCSTR szSub)
{
    HKEY hKeyReg;

    if (ERROR_SUCCESS != RegOpenKeyEx(hKey, szSub, 0, KEY_SET_VALUE, &hKeyReg))
    {
		Trc("TestRegKey", "ERROR", "RegOpenKeyEx - %d.", GetLastError());
        return FALSE;
    }

    RegCloseKey(hKeyReg);

    return TRUE;
}


/* CreateRunOnce
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::CreateRunOnce (CString &sValue)
{
	/* Create auto start key
	 */
	sValue = m_sModule;
	
	sValue += _T(" -ini \"");
	sValue += m_sIniSpecial;
	sValue += _T("\"");

	if (m_bInstallAll == TRUE)
	{
		sValue += _T(" -all");
	}
	else
	{
		sValue += _T(" -auto");
	}

	if (m_bTrace == TRUE)
	{
		sValue += _T(" -trc");
	}

	if (m_bDebugLog == TRUE)
	{
		sValue += _T(" -log");
	}

	CreateRegKey(HKEY_LOCAL_MACHINE, SET_HKEY_RUNONCE, SET_HVAL_RUNONCE, REG_SZ, (LPBYTE)(LPCTSTR)sValue, sValue.GetLength()+1);
	
	return TRUE;
}


/* T R A C I N G
 * ----------------------------------------------------------------------------
 */

/* SetTrace
 * ----------------------------------------------------------------------------
 */
void CInstall::SetTrace(BOOL bTrace)
{
	m_bTrace = bTrace;

	if (m_bTrace == TRUE && m_hTrace == NULL)
	{
		InitTrace();
	}

	if (m_bTrace == FALSE && m_hTrace != NULL)
	{
		FinalizeTrace();
	}
}


/* IsTrace
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::IsTrace()
{
	return m_bTrace;
}


/* InitTrace
 * ----------------------------------------------------------------------------
 */
void CInstall::InitTrace()
{
	if (m_bTrace == FALSE)
	{
		return;
	}

	FinalizeTrace();

	m_hTrace = CreateFile(SET_LOG_TRACE, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_hTrace == INVALID_HANDLE_VALUE)
	{
		m_hTrace = NULL;
	}

	SetFilePointer(m_hTrace, 0, NULL, FILE_END);

	Trc("InitTrace", "", "");
	Trc("InitTrace", "", "");
	Trc("InitTrace", "", "-------------------------------------------------");
	Trc("InitTrace", "", "----    T r a c e   I n i t i a l i z e d    ----");
	Trc("InitTrace", "", "-------------------------------------------------");
	Trc("InitTrace", "", "");
}


/* FinalizeTrace
 * ----------------------------------------------------------------------------
 */
void CInstall::FinalizeTrace()
{
	if (m_hTrace != NULL)
	{
		CloseHandle(m_hTrace);
		m_hTrace = NULL;
	}
}


/* Trc
 * ----------------------------------------------------------------------------
 */
void CInstall::Trc(LPCTSTR szFun, LPCTSTR szAction, LPCTSTR szFormat, ...)
{
	if (m_bTrace == FALSE || m_hTrace == NULL)
	{
		return;
	}

	CTime timeWrite = CTime::GetCurrentTime();
	CString sFormat	= timeWrite.Format("[%d.%m.%y %H:%M:%S]");
	sFormat += " [%-20s] [%-10s] *  %s";

	DWORD dw;
	CString sMsg;

	va_list args;
	va_start(args, szFormat);
	sMsg.FormatV(szFormat, args);
	va_end(args);

	CString sLine;
	sLine.Format(sFormat, szFun, szAction, sMsg);
	sLine += _T("\r\n");

	WriteFile(m_hTrace, sLine, sLine.GetLength(), &dw, NULL);
}


/* S Y S T E M   C H E C K I N G 
 * ----------------------------------------------------------------------------
 */

/* IsOSValid
 * ----------------------------------------------------------------------------
 */
UINT CInstall::IsOSValid(CString &sOsCur, CString &sOsMin, BOOL *pSP)
{
    OSVERSIONINFO VI;

    VI.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&VI);

	Trc("OS Info", "", "%d.%d.%d (%s)", VI.dwMajorVersion, VI.dwMinorVersion, LOWORD(VI.dwBuildNumber), VI.szCSDVersion);

	UINT uOK  = SET_OS_VALID_NOK;
	*pSP = FALSE;

	if (VI.dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		UINT	uWin	= (UINT)-1;
		CString sSP		= "Not Supported";
		CString sSoll;
		CString sIst;

		switch (VI.dwMajorVersion)
		{
			case 3 :
			{
				// Windows NT 3.51
				uWin = IDS_OS_WINNT3;
				
				*pSP = FALSE;
				uOK	 = SET_OS_VALID_NOK;
				
				break;

			} /* case 3 */

			case 4 :
			{
				// Windows NT 4.0
				uWin = IDS_OS_WINNT;
				
				*pSP = FALSE;
				uOK	 = SET_OS_VALID_NOK;
				
				break;
			
			}  /* case 4 */

			case 5 :
			{
				switch(VI.dwMinorVersion)
				{
					case 0 :
					{
						// Windows 2000
						uWin = IDS_OS_WIN2K;

						GetKeyFromIni(SET_SCT_SERVPACK, SET_SUFFIX_2K, sSP);

						if (GetLanguage() == LANG_POLISH)
						{
							sSoll = sSP;
							sSoll.MakeLower();

							sIst = VI.szCSDVersion;
							sIst.MakeLower();

							*pSP = sIst.Find(sSoll) == -1 ? TRUE : FALSE;
						}
						else
						{
							*pSP = stricmp(VI.szCSDVersion, sSP) < 0;
						}

						uOK	 = SET_OS_VALID_OK;

						break;
					}

					case 1 :
					{
						// Windows XP
						uWin = IDS_OS_WINXP;

						GetKeyFromIni(SET_SCT_SERVPACK, SET_SUFFIX_XP, sSP);


						if (GetLanguage() == LANG_POLISH)
						{
							sSoll = sSP;
							sSoll.MakeLower();

							sIst = VI.szCSDVersion;
							sIst.MakeLower();

							*pSP = sIst.Find(sSoll) == -1 ? TRUE : FALSE;
						}
						else
						{
							*pSP = stricmp(VI.szCSDVersion, sSP) < 0;
						}

						uOK	 = SET_OS_VALID_OK;

						break;
					}

					case 2:
					{
						// Windows 2003 Server
						uWin = IDS_OS_WIN2K3;

						GetKeyFromIni(SET_SCT_SERVPACK, SET_SUFFIX_2K3, sSP);

						if (GetLanguage() == LANG_POLISH)
						{
							sSoll = sSP;
							sSoll.MakeLower();

							sIst = VI.szCSDVersion;
							sIst.MakeLower();

							*pSP = sIst.Find(sSoll) == -1 ? TRUE : FALSE;
						}
						else
						{
							*pSP = stricmp(VI.szCSDVersion, sSP) < 0;
						}
						
						uOK	 = SET_OS_VALID_OK;

						break;
					}

					default:
					{
						// Unknown
						break;
					}
				
				} /* switch(VI.dwMinorVersion) */

				break;

			} /* case 5 */

			case 6 :
			{
				switch(VI.dwMinorVersion)
				{
					case 0:
					case 1:
					{
						// Windows Vista
						uWin = IDS_OS_WINVI;

						GetKeyFromIni(SET_SCT_SERVPACK, SET_SUFFIX_VI, sSP);

						*pSP = stricmp(VI.szCSDVersion, sSP) < 0;

						uOK  = SET_OS_VALID_OK;

						break;
					}

					default:
					{
						// Unknown
						break;
					}

				} /* switch(VI.dwMinorVersion) */

			} /* case 6 */

			default:
			{
				// Unknown
				break;
			}

		} /* switch (VI.dwMajorVersion) */

		if (uWin == (UINT)-1)
		{
			CString sWindows;
			sWindows.LoadString(IDS_OS_UNKNOWN);

			sOsMin.LoadString(IDS_OS_WIN2K);
			sOsCur.Format("%s %ld.%ld %s", sWindows, VI.dwMajorVersion, VI.dwMinorVersion, VI.szCSDVersion);
		}
		else
		{
			CString sWindows;
			sWindows.LoadString(uWin);

			sOsMin = sSP;
			
			if (VI.szCSDVersion[0] != 0)
			{
				sOsCur.Format("%s - %s", sWindows, VI.szCSDVersion);
			}
			else
			{
				sOsCur = sWindows;
			}

		}

	}
	else
	{
		UINT uWin = (UINT)-1;

		switch(VI.dwMinorVersion)
		{
			case 0 :
			{
				/* Windows 95b
				 */
				uWin = VI.szCSDVersion[1] == 'B' || VI.szCSDVersion[1] == 'C' ? IDS_OS_WIN95B : IDS_OS_WIN95A;
				uOK  = SET_OS_VALID_NOK;
				break;
			}

			case 10 :
			{
				/* Windows 98
				 */
				uWin = VI.szCSDVersion[1] == 'A' ? IDS_OS_WIN98SE  : IDS_OS_WIN98;
				// uOK  = VI.szCSDVersion[1] == 'A' ? SET_OS_VALID_OK : SET_OS_VALID_NOK;	// < V2.10.2
				uOK  = SET_OS_VALID_NOK;
				break;
			}

			case 90 :
			{
				/* Windows ME
				 */
				uWin = IDS_OS_WINME;
				// uOK  = SET_OS_VALID_WARN;	// < V2.10.2
				uOK  = SET_OS_VALID_NOK;

				break;
			}

			default:
			{
				/* Unkown
				 */
				uWin = IDS_OS_UNKNOWN;
				uOK	 = SET_OS_VALID_NOK;
				break;
			}
		}    

		sOsCur.LoadString(uWin);
		// sOsMin.LoadString(IDS_OS_WIN98SE);	// < V2.10.2
		sOsMin.LoadString(IDS_OS_WIN2K);
	}

    return uOK;
}


/* CheckSP
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::CheckSP(CString &sOsCur, CString &sOsMin, BOOL *pValidOS)
{
	BOOL bNeedSP = FALSE;

	*pValidOS = IsOSValid(sOsCur, sOsMin, &bNeedSP) != SET_OS_VALID_NOK;

    return ! bNeedSP;
}


/* CheckDEP
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::CheckDEP(void)
{
	BOOL bOK = TRUE;

	if (IsWindowsNT4() || IsWindows2K())
	{
		// DEP is not supported within NT4 and W2K
		return bOK;
	}

	if (TestRegKey(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control") == FALSE)
	{
		return bOK;
	}

	CHAR szOpt[1024];
	DWORD dwSize = sizeof(szOpt);

	if (QueryRegValue(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control", "SystemStartOptions", (LPBYTE)szOpt, &dwSize) == FALSE)
	{
		// Key not present, should be OK.
		return bOK;
	}

	#define INST_NO_EXEC	1
	#define INST_EXEC		2

	#define INST_NOTHING	0
	#define INST_OPTIN		1
	#define INST_OPTOUT		2
	#define INST_ALWAYSON	3
	#define INST_ALWAYSOFF	4

	UINT uKey = INST_NOTHING;
	UINT uOpt = INST_NOTHING;

	CString sOpt(szOpt);
	sOpt.MakeUpper();

	if (sOpt.Find("NOEXECUTE=") >= 0)
	{
		uKey = INST_NO_EXEC;
	}
	else if (sOpt.Find("EXECUTE=") >= 0)
	{
		uKey = INST_EXEC;
	}
	else
	{
		// Key not present, should be OK.
		return bOK;
	}

	if (sOpt.Find("=OPTIN") >= 0)
	{
		uOpt = INST_OPTIN;
	}
	else if (sOpt.Find("=OPTOUT") >= 0)
	{
		uOpt = INST_OPTOUT;
	}
	else if (sOpt.Find("=ALWAYSON") >= 0)
	{
		uOpt = INST_ALWAYSON;
	}
	else if (sOpt.Find("=ALWAYSOFF") >= 0)
	{
		uOpt = INST_ALWAYSOFF;
	}

	switch (uKey)
	{
		case INST_NO_EXEC:
		{
			switch (uOpt)
			{
				case INST_NOTHING:	bOK = FALSE;	break;
				case INST_OPTIN:	bOK = TRUE;		break;
				case INST_OPTOUT:	bOK = FALSE;	break;
				case INST_ALWAYSON:	bOK = FALSE;	break;
				case INST_ALWAYSOFF:bOK = TRUE;		break;
				default:			bOK = TRUE;		break;
			}
			break;
		}

		case INST_EXEC:
		{
			switch (uOpt)
			{
				case INST_NOTHING:	bOK = TRUE;		break;
				case INST_OPTIN:	bOK = TRUE;		break;
				case INST_OPTOUT:	bOK = TRUE;		break;
				case INST_ALWAYSON:	bOK = TRUE;		break;
				case INST_ALWAYSOFF:bOK = FALSE;	break;
				default:			bOK = TRUE;		break;
			}
			break;
		}

		default:
		{
			// Should not happen.
			bOK = FALSE;
			break;
		}

	} /* switch (uKey) */

	return bOK;
}

/* CheckIE
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::CheckIE(CString &sVersion)
{
    sVersion.Empty();

    char szIExp[MAX_PATH];
    DWORD dwSize = MAX_PATH;

    // Find explorer file
    if (! QueryRegValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\IEXPLORE.EXE", "", (LPBYTE)szIExp, &dwSize))
    {
		Trc("CheckIE", "ERROR", "QueryRegValue - %d.", GetLastError());
        return FALSE;
    }

    CString sMinVer;
	if (! GetKeyFromIni(SET_SCT_MINVER, SET_PRODKEY_IE, sMinVer))
	{
		Trc("CheckIE", "ERROR", "GetKeyFromIni - Min. IE Version not found.");
		return FALSE;
	}

	if (GetFileVersion(szIExp, sVersion))
    {
        return CheckVersionNumber(sVersion, sMinVer) == IDS_OK;
    }

	Trc("CheckIE", "ERROR", "GetFileVersion - %d.", GetLastError());
    return FALSE;
}


/* CheckMSI
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::CheckMSI(CString &sVersion)
{
    sVersion = "Not Installed";

    char szSysVer[MAX_PATH];

    GetSystemDirectory(szSysVer, sizeof(szSysVer));
    lstrcat(szSysVer, "\\msi.dll");

    CString sMinVer;
	if (! GetKeyFromIni(SET_SCT_MINVER, SET_PRODKEY_MSI, sMinVer))
	{
		Trc("CheckMSI", "ERROR", "GetKeyFromIni - Min. Installer Version not found.");
		return FALSE;
	}

    if (GetFileVersion(szSysVer, sVersion))
    {
		return CheckVersionNumber(sVersion, sMinVer) == IDS_OK;
    }

	Trc("CheckMSI", "ERROR", "GetFileVersion - %d.", GetLastError());
    return FALSE;
}


/* CheckJDK
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::CheckJDK(CString &sVersion)
{
    sVersion = "Not Installed";

	if (! TestRegKey(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\SDK for Java 4.0"))
	{
		if (! TestRegKey(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\SDKJava"))
		{
			Trc("CheckJDK", "ERROR", "JDK not found.");
			return FALSE;
		}
		else
		{
			sVersion = "5.0.2924.0";
		}
	}
	else
	{
		sVersion = "5.0.3229.0";
	}

    CString sMinVer;
	if (! GetKeyFromIni(SET_SCT_MINVER, SET_PRODKEY_JDK, sMinVer))
	{
		Trc("CheckJDK", "ERROR", "GetKeyFromIni - Min. JDK Version not found.");
		return FALSE;
	}

	return CheckVersionNumber(sVersion, sMinVer) == IDS_OK;
}


/* CheckJVM
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::CheckJVM(CString &sVersion)
{
    sVersion = "Not Installed";

    char szJVMxp[MAX_PATH];
    char szDummy[MAX_PATH];
    DWORD dwSize = MAX_PATH;

    // Find Java VM version number
    if (! QueryRegValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Active Setup\\Installed Components\\{08B0E5C0-4FCB-11CF-AAA5-00401C608500}", "Version", (LPBYTE)szJVMxp, &dwSize))
    {
		Trc("CheckJVM", "ERROR", "QueryRegValue - %d.", GetLastError());
        return FALSE;
    }

    CString sMinVer;
	if (! GetKeyFromIni(SET_SCT_MINVER, SET_PRODKEY_JVM, sMinVer))
	{
		Trc("CheckJVM", "ERROR", "GetKeyFromIni - Min. JVM Version not found.");
		return FALSE;
	}

	sVersion = szJVMxp;
	sVersion.Replace(",", ".");
	
	BOOL bSunVM   = FALSE;
	BOOL bOtherVM = FALSE;

	if (bSunVM == FALSE && CheckVersionNumber(sVersion, "5.0.5000.0") == IDS_OK)
	{
		bSunVM = TRUE;
	}

	if (bSunVM == FALSE)
	{
		dwSize = MAX_PATH;
		if (QueryRegValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Active Setup\\Installed Components\\{08B0E5C0-4FCB-11CF-AAA5-00401C608500}", "KeyFileName", (LPBYTE)szDummy, &dwSize))
		{
			CString sDummy(szDummy);

			sDummy.MakeLower();

			if (sDummy.Find("jre") != -1 || sDummy.Find("regutils.dll") != -1)
			{
				bSunVM = TRUE;
			}

			if (bSunVM == FALSE && sDummy.Find("msjava.dll") == -1)
			{
				bOtherVM = TRUE;
			}
		}
	}

	if (bOtherVM == TRUE)
	{
		sVersion = "Other VM";
	}

	if (bSunVM == TRUE)
	{
		sVersion = "Sun JVM";
	}

	return CheckVersionNumber(sVersion, sMinVer) == IDS_OK;
}


/* CheckIIS
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::CheckIIS(CString &sVersion)
{
    sVersion.Empty();

    char szIISxp[MAX_PATH];
    DWORD dwSize = MAX_PATH;

    // Find explorer file
    if (! QueryRegValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\INETMGR.EXE", "", (LPBYTE)szIISxp, &dwSize))
    {
		Trc("CheckIIS", "ERROR", "QueryRegValue - %d.", GetLastError());
        return FALSE;
    }

    CString sMinVer;
	if (! GetKeyFromIni(SET_SCT_MINVER, SET_PRODKEY_IIS, sMinVer))
	{
		Trc("CheckIIS", "ERROR", "GetKeyFromIni - Min. IE Version not found.");
		return FALSE;
	}

	CString sIISxp;

	if (strnicmp(szIISxp, "%windir%", strlen("%windir%")) != 0)
	{
		sIISxp = szIISxp;
	}
	else
	{
		char szEnv[100];
		DWORD dwLen = GetEnvironmentVariable("windir", szEnv, 100);

		if (dwLen == 0)
		{
			Trc("CheckIIS", "ERROR", "GetEnvironmentVariable failed - %d.", GetLastError());
			return FALSE;
		}

		sIISxp  = szEnv;
		sIISxp += szIISxp + strlen("%windir%");
	}
	
	if (GetFileVersion(sIISxp, sVersion))
    {
        return CheckVersionNumber(sVersion, sMinVer) == IDS_OK;
    }

	Trc("CheckIIS", "ERROR", "GetFileVersion - %d.", GetLastError());
    return FALSE;
}


/* CheckLM
 * ----------------------------------------------------------------------------
 */
UINT CInstall::CheckLM(CString &sVersion)
{
	return Check4C(SET_PRODKEY_LM, sVersion);
}


/* Check4C
 * ----------------------------------------------------------------------------
 */
UINT CInstall::Check4C(LPCSTR szKey, CString &sVersion)
{
    sVersion = "Not Installed";

	HINSTANCE hMSI = LoadLibrary (TEXT("MSI.DLL"));
	if (hMSI == NULL)
	{
		Trc("Check4C", "ERROR", "LoadLibrary - %d.", GetLastError());
		return IDS_INSTALL;
	}
			
	PMsiGetProductInfoA pMsiGetProductInfoA = (PMsiGetProductInfoA)::GetProcAddress (hMSI, _T("MsiGetProductInfoA"));

	CString sCode;
	GetKeyFromIni(SET_SCT_PCODE, szKey, sCode);
	
	if (pMsiGetProductInfoA(sCode, INSTALLPROPERTY_INSTALLEDPRODUCTNAME, NULL, NULL) == ERROR_UNKNOWN_PRODUCT)
	{
		// Trc("Check4C", "ERROR", "MsiGetProductInfo - %d.", GetLastError());
		FreeLibrary(hMSI);
		return IDS_INSTALL;
	}

	FreeLibrary(hMSI);

	char szPath[MAX_PATH];
	DWORD dwPath = MAX_PATH;

	if (stricmp(szKey, SET_PRODKEY_LM) == 0)
	{
		if (! QueryRegValue(HKEY_LOCAL_MACHINE, SET_HKEY_LM_INSTPATH, SET_HVAL_LM_INSTPATH, (BYTE *)szPath, &dwPath))
		{
			Trc("Check4C - LM", "ERROR", "QueryRegValue - %d.", GetLastError());
			return IDS_INSTALL;
		}
	}
	else
	{
		if (! QueryRegValue(HKEY_LOCAL_MACHINE, SET_HKEY_INSTPATH, SET_HVAL_INSTPATH, (BYTE *)szPath, &dwPath))
		{
			Trc("Check4C", "ERROR", "QueryRegValue - %d.", GetLastError());
			return IDS_INSTALL;
		}
	}

	CString sSubDir;
	GetKeyFromIni(SET_SCT_SUBDIR, szKey, sSubDir);

	CString sExeToCheck(szPath);
	sExeToCheck += sSubDir;

	GetFileVersion(sExeToCheck, sVersion);

    CString sMinVer;
	if (! GetKeyFromIni(SET_SCT_MINVER, szKey, sMinVer))
	{
		Trc("Check4C", "ERROR", "GetKeyFromIni - Min. 4C Version not found.");
		return IDS_INSTALL;
	}

	if (sMinVer.CompareNoCase(SET_DONT_CARE) == 0)
	{
		sVersion = SET_DONT_CARE;
		return IDS_OK;
	}

	return CheckVersionNumber(sVersion, sMinVer);
}


/* I N S T A L L A T I O N
 * ----------------------------------------------------------------------------
 */

/* InstallSP
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::InstallSP(CPropBase *pProp)
{
	CString sKey(SET_PRODKEY_SP);
	TranslateKey(sKey);

	CString sExe;
	GetKeyFromIni(SET_SCT_MS_SETUP, sKey, sExe);

	if (sExe.IsEmpty())
	{
		CString sProduct;
		GetKeyFromIni(SET_SCT_DESCRIPTION, sKey, sProduct);

		CString sMsg;
		sMsg.Format(IDS_MSG_NOT_ON_CD, sProduct);

		pProp->MessageBox(sMsg, m_sTitle, MB_OK | MB_ICONHAND);
		return FALSE;
	}

	CString sSuffix;
	if (! GetLanguageSuffix(sSuffix))
	{
		CString sProduct;
		GetKeyFromIni(SET_SCT_DESCRIPTION, sKey, sProduct);

		CString sMsg;
		sMsg.Format(IDS_MSG_LANGUAGE, sProduct);

		pProp->MessageBox(sMsg, m_sTitle, MB_OK | MB_ICONHAND);
		return FALSE;
	}
	
	CString sPath;
	sPath.Format(sExe, sSuffix);

	Trc("InstallSP", "", "Key = %s  Path = %s", sKey, sPath);
	
	return InstallMSProduct(pProp, sKey, sPath, TRUE);
}


/* InstallMSI
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::InstallMSI(CPropBase *pProp)
{
	CString sKey(SET_PRODKEY_MSI);
	sKey += '_';

	if (IsWindowsNTBased())
	{
		sKey += SET_SUFFIX_NT;
	}
	else
	{
		sKey += SET_SUFFIX_9X;
	}

	CString sPath;
	GetKeyFromIni(SET_SCT_MS_SETUP, sKey, sPath);

	if (m_bInstallAll == TRUE)
	{
		Sleep(2500);
	}

	Trc("InstallMSI", "", "Key = %s  Path = %s", sKey, sPath);
	
	return InstallMSProduct(pProp, sKey, sPath, FALSE);			
}


/* InstallIE
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::InstallIE(CPropBase *pProp)
{
	CString sSuffix;
	if (! GetLanguageSuffix(sSuffix))
	{
		CString sProduct;
		GetKeyFromIni(SET_SCT_DESCRIPTION, SET_PRODKEY_IE, sProduct);

		CString sMsg;
		sMsg.Format(IDS_MSG_LANGUAGE, sProduct);

		pProp->MessageBox(sMsg, m_sTitle, MB_OK | MB_ICONHAND);
		return FALSE;
	}

	CString sDummy;
	GetKeyFromIni(SET_SCT_MS_SETUP, SET_PRODKEY_IE, sDummy);
	
	CString sPath;
	sPath.Format(sDummy, sSuffix);

	Trc("InstallIE", "", "Key = %s  Path = %s", SET_PRODKEY_IE, sPath);
	
	return InstallMSProduct(pProp, SET_PRODKEY_IE, sPath, TRUE);
}


/* InstallJDK
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::InstallJDK(CPropBase *pProp)
{
	CString sPath;
	GetKeyFromIni(SET_SCT_MS_SETUP, SET_PRODKEY_JDK, sPath);
	
	Trc("InstallJDK", "", "Key = %s  Path = %s", SET_PRODKEY_IE, sPath);
	
	return InstallMSProduct(pProp, SET_PRODKEY_JDK, sPath, FALSE);
}


/* InstallJVM
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::InstallJVM(CPropBase *pProp)
{
	CString sPath;
	GetKeyFromIni(SET_SCT_MS_SETUP, SET_PRODKEY_JVM, sPath);
	
	Trc("InstallJVM", "", "Key = %s  Path = %s", SET_PRODKEY_JVM, sPath);
	
	return InstallMSProduct(pProp, SET_PRODKEY_JVM, sPath, TRUE);
}


/* InstallIIS
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::InstallIIS(CPropBase *pProp)
{
	CString sPath;
	GetKeyFromIni(SET_SCT_MS_SETUP, SET_PRODKEY_IIS, sPath);
	
	Trc("InstallIIS", "", "Key = %s  Path = %s", SET_PRODKEY_IIS, sPath);
	
	return Execute(pProp, SET_PRODKEY_IIS, sPath);
}


/* Install4C
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::Install4C(CPropBase *pProp, LPCTSTR szKey)
{	
	if (stricmp(szKey, SET_PRODKEY_CTRL) == 0)
	{
		BOOL bOK = CheckDEP();

		if (bOK == FALSE)
		{
			CString sMsg;
			CString sTitle;

			if (GetLanguage() == LANG_GERMAN)
			{
				sMsg  =	"Die aktuellen Einstellungen des Windows Features 'Data Execution Prevention (DEP)' sind \r\n";
				sMsg +=	"nicht kompatibel mit dem ATCMControl Control Dienst. Der ATCMControl Control Dienst wird nach\r\n";
				sMsg +=	"einem Start von Windows nicht automatisch gestartet.\r\n\r\n";
				sMsg +=	"Um dieses Problem zu lösen, ändern Sie bitte die DEP Einstellung auf 'OptIn':\r\n";
				sMsg +=	"<Start/Systemsteuerung/System/Erweitert/Systemleistung-Einstellung/Datenausführungsverhinderung/\r\n";
				sMsg +=	"'Datenausführungsverhinderung nur für erforderliche Windows-Programme und -Dienste aktivieren.'>\r\n\r\n";
				sMsg +=	"Bitte booten Sie die den PC nach einer Änderung neu. Eine genauere Beschreibung über die\r\n";
				sMsg +=	"Einstellungen des DEP Features kann im File ReadMe.htm auf der ATCMControl Installations-CD.\r\n";
				sMsg +=	"gefunden werden.\r\n\r\n";
				sMsg +=	"Möchten Sie mit der ATCMControl Installation fortfahren?\r\n";

				sTitle = "Ungültige DEP Einstellungen";
			}
			else
			{
				sMsg  =	"The current settings of the 'Data Execution Prevention' (DEP) Windows Feature is not\r\n";
				sMsg +=	"compatible with the ATCMControl Control service. The ATCMControl Control service will not start\r\n";
				sMsg +=	"automatically after a Windows system boot.\r\n\r\n";
				sMsg +=	"In order to solve this issue change the DEP settings to 'OptIn' by accessing\r\n";
				sMsg +=	"<Start / Control Panel / System / Advanced / Performance - Settings / Data Execution Prevention /\r\n";
				sMsg +=	"'Turn on DEP for essential Windows programs and services only'>\r\n\r\n";
				sMsg +=	"Please remember to reboot the system after changing this setting. A more detailed explanation\r\n";
				sMsg +=	"of the DEP settings can be found in the file ReadMe.htm on the ATCMControl Installation CD\r\n\r\n";
				sMsg +=	"Do you want to continue the ATCMControl installation?\r\n";

				sTitle = "Invalid DEP Settings";
			}

			if (pProp->MessageBox(sMsg, sTitle, MB_YESNO | MB_ICONHAND) != IDYES)
			{
				return TRUE;
			}
		}
	}

	if (m_sInstallPath.IsEmpty())
	{
		m_sInstallPath = "c:\\";
	}

	if (m_sInstallPath.GetAt(m_sInstallPath.GetLength() - 1) != '\\')
	{
		m_sInstallPath += '\\';
	}

	CreateRegKey(HKEY_LOCAL_MACHINE, SET_HKEY_INSTPATH, SET_HVAL_INSTPATH, REG_SZ, (LPBYTE)(LPCTSTR)m_sInstallPath, m_sInstallPath.GetLength()+1);

	Trc("Install4C", "", "Key = %s  Path = %s", szKey, m_sInstallPath);

	return Install4CComponent(pProp, szKey);
}


/* UnInstall4C
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::UnInstall4C(CPropBase *pProp, LPCTSTR szKey)
{
	Trc("UnInstall4C", "", "Key = %s", szKey);

	return UnInstall4CComponent(pProp, szKey);
}


/* InstallProduct
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::InstallMSProduct(CPropBase *pMain, LPCTSTR szKey, LPCTSTR szPath, BOOL bReboot)
{
    PROCESS_INFORMATION PI;
    STARTUPINFO SI;

	memset(&SI, 0x00, sizeof(SI));
    SI.cb = sizeof(SI);

	CString sPath;
	GetExeDir(sPath);
	
	sPath += szPath;

	Trc("InstallMSProduct", "", "Key = %s  Path = %s", szKey, sPath);

	if (! CreateProcess(sPath, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &SI, &PI))
	{
		Trc("InstallMSProduct", "", "Key = %s  Path = %s", szKey, szPath);

		// Can't find file, in case it is an absolute path, use it as it was given
		if (! CreateProcess(szPath, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &SI, &PI))
		{
			UINT uRes = GetLastError();
			
			Trc("InstallMSProduct", "ERROR", "Key = %s - %d", szKey, uRes);

			HANDLE lpMsgBuf;
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, uRes, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
			
			CString sProduct;
			GetKeyFromIni(SET_SCT_DESCRIPTION, szKey, sProduct);
			
			CString sMessage;
			sMessage.Format(IDS_MSG_MS_FAILED, sProduct, lpMsgBuf, uRes);
			
			LocalFree(lpMsgBuf);
			
			pMain->MessageBox(sMessage, m_sTitle, MB_OK | MB_ICONHAND);
			
			return FALSE;
		}
	}

	/* Create auto start key
	 */
	CString sVal;
	CreateRunOnce(sVal);

	Trc("InstallMSProduct", "", "Key = %s  Registry = %s", szKey, sVal);

	/* Wait for installation
	 */
	pMain->GetParent()->ShowWindow(SW_HIDE);

    while (WAIT_OBJECT_0 != MsgWaitForMultipleObjects(1, &PI.hProcess, FALSE, INFINITE, QS_ALLINPUT))
    {
		MSG msg;
        while (PeekMessage(&msg, AfxGetMainWnd()->GetSafeHwnd(), 0, 0, PM_REMOVE)) 
        {
            DispatchMessage(&msg);
        }
    }

	/* Check exit code of installer
	 */
	BOOL  bRet	 = TRUE;
	DWORD dwExit = 4711;

	GetExitCodeProcess(PI.hProcess, &dwExit);

	Trc("InstallMSProduct", "", "Key = %s  Finished, RetVal = %d", szKey, dwExit);

	if (dwExit == ERROR_SUCCESS && bReboot == TRUE)
	{
		/* Installation was canceled
		 */
		Trc("InstallMSProduct", "Cancel", "Key = %s  %d", szKey, dwExit);
		// DeleteRegValue(HKEY_LOCAL_MACHINE, SET_HKEY_RUNONCE, SET_HVAL_RUNONCE);
		bRet = FALSE;
	}
	else if (dwExit == ERROR_SUCCESS && bReboot == FALSE)
	{
		/* Installation successfull
		 */
		DeleteRegValue(HKEY_LOCAL_MACHINE, SET_HKEY_RUNONCE, SET_HVAL_RUNONCE);
		bRet = TRUE;
	}
	else if (dwExit == ERROR_SUCCESS_REBOOT_REQUIRED)
	{
		/* Installation successfull, reboot required
		 */
		Trc("InstallMSProduct", "Reboot", "Key = %s  %d", szKey, dwExit);
		bRet = FALSE;
	}
	else
	{
		/* Unexpected error
		 */
		Trc("InstallMSProduct", "ERROR", "Key = %s  %d", szKey, dwExit);
		DeleteRegValue(HKEY_LOCAL_MACHINE, SET_HKEY_RUNONCE, SET_HVAL_RUNONCE);
		bRet = FALSE;
	}

	pMain->GetParent()->ShowWindow(SW_NORMAL);
    CloseHandle(PI.hProcess);

    return bRet;
}


/* Execute
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::Execute(CPropBase *pMain, LPCTSTR szKey, LPCTSTR szPath)
{
    PROCESS_INFORMATION PI;
    STARTUPINFO SI;

	memset(&SI, 0x00, sizeof(SI));
    SI.cb = sizeof(SI);

	CString sPath;
	
	char szIExp[MAX_PATH];
	DWORD dwSize = MAX_PATH;

	// Find explorer file
	if (! QueryRegValue(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\IEXPLORE.EXE", "", (LPBYTE)szIExp, &dwSize))
	{
		Trc("Execute", "ERROR", "QueryRegValue - %d.", GetLastError());
		return FALSE;
	}

	sPath  = szIExp;
	sPath += ' ';

	CString sExe;
	GetExeDir(sExe);

	sPath += sExe;
	
	sPath += szPath;

	Trc("Execute", "", "Key = %s  Path = %s", szKey, sPath);

	char *szTemp = new char [sPath.GetLength() + 1];
	strcpy (szTemp, sPath);

	if (! CreateProcess(NULL, szTemp, NULL, NULL, FALSE, 0, NULL, NULL, &SI, &PI))
	{
		UINT uRes = GetLastError();
		
		Trc("Execute", "ERROR", "Key = %s - %d", szKey, uRes);

		HANDLE lpMsgBuf;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, uRes, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
		
		CString sProduct;
		GetKeyFromIni(SET_SCT_DESCRIPTION, szKey, sProduct);
		
		CString sMessage;
		sMessage.Format(IDS_MSG_MS_FAILED, sProduct, lpMsgBuf, uRes);
		
		LocalFree(lpMsgBuf);
		
		pMain->MessageBox(sMessage, m_sTitle, MB_OK | MB_ICONHAND);
		
		delete szTemp;
		szTemp = NULL;

		return FALSE;
	}

	delete szTemp;
	szTemp = NULL;

	/* Wait for installation
	 */
	pMain->GetParent()->ShowWindow(SW_HIDE);

    while (WAIT_OBJECT_0 != MsgWaitForMultipleObjects(1, &PI.hProcess, FALSE, INFINITE, QS_ALLINPUT))
    {
		MSG msg;
        while (PeekMessage(&msg, AfxGetMainWnd()->GetSafeHwnd(), 0, 0, PM_REMOVE)) 
        {
            DispatchMessage(&msg);
        }
    }

	/* Check exit code of installer
	 */
	BOOL  bRet	 = TRUE;
	DWORD dwExit = 4711;

	GetExitCodeProcess(PI.hProcess, &dwExit);

	Trc("Execute", "", "Key = %s  Finished, RetVal = %d", szKey, dwExit);

	pMain->GetParent()->ShowWindow(SW_NORMAL);
    CloseHandle(PI.hProcess);

    return bRet;
}


/* Install4CComponent
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::Install4CComponent(CPropBase *pMain, LPCTSTR szKey)
{
	HINSTANCE hMSI = LoadLibrary (TEXT("MSI.DLL"));
	if (hMSI == NULL)
	{
		CString sMsg;
		sMsg.LoadString(IDS_MSG_NO_MSI);

		pMain->MessageBox(sMsg, m_sTitle, MB_OK | MB_ICONHAND);
		return FALSE;
	}
			
	CString sPath;
	GetKeyFromIni(SET_SCT_INSTALLPATH, szKey, sPath);

	INSTALLUILEVEL dwUILevel = INSTALLUILEVEL_BASIC;
	if (stricmp(szKey, SET_PRODKEY_KERNEL) == 0)
	{
		dwUILevel = INSTALLUILEVEL_BASIC; // INSTALLUILEVEL_FULL;
	}
	else if (stricmp(szKey, SET_PRODKEY_LM) == 0)
	{
		dwUILevel = INSTALLUILEVEL_BASIC;
	}
	else if (stricmp(szKey, SET_PRODKEY_CTRL) == 0)
	{
		dwUILevel = INSTALLUILEVEL_BASIC;
	}
	else
	{
		CString sCurVer;

		if (Check4C(SET_PRODKEY_KERNEL, sCurVer) != IDS_OK)
		{
			Trc("Install4CComponent", "ERROR", "%s: Kernel not installed", szKey);
			return FALSE;
		}
	}

	Trc("Install4CComponent", "", "Key = %s  Path = %s", szKey, sPath);

	/* Create auto start key
	 */
	CString sVal(m_sModule);
	CreateRunOnce(sVal);

	Trc("Install4CComponent", "", "Key = %s  Registry = %s", szKey, sVal);

	/* Determine features to be installed
	 */
	CString sFeature;
	
	if (stricmp(szKey, SET_PRODKEY_CTRL) == 0)
	{
		sFeature = SET_FEAT_CTRL;

		if (m_dwToDo & SET_FLAG_CONS)
		{
			sFeature += _T(",");
			sFeature += SET_FEAT_CONS;
		}

		if (m_dwToDo & SET_FLAG_RTCE)
		{
			sFeature += _T(",");
			sFeature += SET_FEAT_RTCE;
		}
	}

	#define  ERROR_SUCCESS_REBOOT_INITIATED 1641

	// Install product
	UINT uRes = InstallMsiPackage(sPath, AfxGetMainWnd()->GetSafeHwnd(), hMSI, dwUILevel, sFeature.IsEmpty() ? NULL : (LPCTSTR)sFeature);
	
	Trc("Install4CComponent", "", "Key = %s  Finished, RetVal = %d", szKey, uRes);

	if (uRes != ERROR_SUCCESS					&& uRes != ERROR_INSTALL_USEREXIT && 
		uRes != ERROR_SUCCESS_REBOOT_INITIATED  && uRes != ERROR_SUCCESS_REBOOT_REQUIRED)
	{
		Trc("Install4CComponent", "ERROR", "Key = %s  %d", szKey, uRes);

		HANDLE lpMsgBuf;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, uRes, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);

		CString sProduct;
		GetKeyFromIni(SET_SCT_DESCRIPTION, szKey, sProduct);

		CString sMessage;
		sMessage.Format(IDS_MSG_MSI_FAILED, sProduct, lpMsgBuf, uRes);

		LocalFree(lpMsgBuf);

		pMain->MessageBox(sMessage, m_sTitle, MB_OK | MB_ICONHAND);
	}

	if (uRes != ERROR_SUCCESS_REBOOT_INITIATED && uRes != ERROR_SUCCESS_REBOOT_REQUIRED)
	{
		DeleteRegValue(HKEY_LOCAL_MACHINE, SET_HKEY_RUNONCE, SET_HVAL_RUNONCE);
	}

	if (uRes == ERROR_SUCCESS_REBOOT_INITIATED || uRes == ERROR_SUCCESS_REBOOT_REQUIRED)
	{
		Trc("Install4CComponent", "Reboot", "Key = %s  %d", szKey, uRes);
	}

	if (uRes == ERROR_INSTALL_USEREXIT)
	{
		Trc("Install4CComponent", "Cancel", "Key = %s  %d", szKey, uRes);
	}
				
	FreeLibrary (hMSI) ;
	
	return uRes == ERROR_SUCCESS;
}


/* UnInstall4CComponent
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::UnInstall4CComponent(CPropBase *pMain, LPCTSTR szKey)
{	 
	HINSTANCE hMSI = LoadLibrary (TEXT("MSI.DLL"));
	if (hMSI == NULL)
	{
		CString sMsg;
		sMsg.LoadString(IDS_MSG_NO_MSI);

		pMain->MessageBox(sMsg, m_sTitle, MB_OK | MB_ICONHAND);
		return FALSE;
	}
	
	CString sCode;
	GetKeyFromIni(SET_SCT_PCODE, szKey, sCode);

	// UnInstall product
	UINT uRes = UnInstallMsiPackage(sCode, AfxGetMainWnd()->GetSafeHwnd(), hMSI, INSTALLUILEVEL_BASIC);
	
	if (uRes != ERROR_SUCCESS && uRes != ERROR_INSTALL_USEREXIT)
	{
		HANDLE lpMsgBuf;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, uRes, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);

		CString sProduct;
		GetKeyFromIni(SET_SCT_DESCRIPTION, szKey, sProduct);

		CString sMessage;
		sMessage.Format(IDS_MSG_MSI_FAILED, sProduct, lpMsgBuf, uRes);

		LocalFree(lpMsgBuf);

		pMain->MessageBox(sMessage, m_sTitle, MB_OK | MB_ICONHAND);
	}
	
	FreeLibrary(hMSI);

	return uRes == ERROR_SUCCESS;
}


/* UnInstallMsiPackage
 * ----------------------------------------------------------------------------
 */
UINT CInstall::UnInstallMsiPackage(LPCTSTR szCode, HWND hWnd, HINSTANCE hMSI, INSTALLUILEVEL dwUILevel)
{	
	PMsiEnableLogA		  pMsiEnableLogA		= (PMsiEnableLogA)		  ::GetProcAddress (hMSI, _T("MsiEnableLogA"));
	PMsiSetInternalUI	  pMsiSetInternalUI		= (PMsiSetInternalUI)	  ::GetProcAddress (hMSI, _T("MsiSetInternalUI"));
	PMsiConfigureProductA pMsiConfigureProductA = (PMsiConfigureProductA) ::GetProcAddress (hMSI, _T("MsiConfigureProductA"));

    if (m_bDebugLog)
    {
		// Enable Log
        DWORD dwLogMode =	INSTALLLOGMODE_ERROR		| INSTALLLOGMODE_WARNING | 
							INSTALLLOGMODE_OUTOFMEMORY	| INSTALLLOGMODE_USER |
							INSTALLLOGMODE_INFO			|	INSTALLLOGMODE_DIAGNOSTIC |
							INSTALLLOGMODE_COMMONDATA	| INSTALLLOGMODE_RESERVED |
							INSTALLLOGMODE_ACTIONSTART	| INSTALLLOGMODE_ACTIONDATA |
							INSTALLLOGMODE_PROPERTYDUMP;

		pMsiEnableLogA(dwLogMode, SET_LOG_UNINSTALL, TRUE);
    }
    else
    {
		pMsiEnableLogA(0, NULL, TRUE);
    }
	
	// Set internal UI Level
	pMsiSetInternalUI(dwUILevel, &hWnd);
	
	Trc("UnInstallMsiPackage", "", szCode);

	// Uninstall
	return pMsiConfigureProductA(szCode, INSTALLLEVEL_DEFAULT, INSTALLSTATE_ABSENT);
}


/* InstallMsiPackage
 * ----------------------------------------------------------------------------
 */
UINT CInstall::InstallMsiPackage(LPCTSTR szPath, HWND hWnd, HINSTANCE hMSI, INSTALLUILEVEL dwUILevel, LPCTSTR szFeatures)
{

	PMsiEnableLogA		  pMsiEnableLogA		= (PMsiEnableLogA)		  ::GetProcAddress (hMSI, _T("MsiEnableLogA"));
	PMsiSetInternalUI	  pMsiSetInternalUI		= (PMsiSetInternalUI)	  ::GetProcAddress (hMSI, _T("MsiSetInternalUI"));
	PMsiInstallProductA	  pMsiInstallProductA	= (PMsiInstallProductA)   ::GetProcAddress (hMSI, _T("MsiInstallProductA"));

	CString sPath;
	
	GetExeDir(sPath);
	sPath += szPath;
		
	CString sDummy = sPath.Left(sPath.ReverseFind(_T('\\')));
	CString sCmdLine;

	/* Determine setup language
	 */
	sCmdLine.Format("TRANSFORMS=%d.mst", m_dwLanguage);
	
	CString sMST;
	sMST.Format("%s\\%d.mst", (LPCTSTR)sDummy, m_dwLanguage);

	CFileStatus status;
	if (! CFile::GetStatus(sMST, status))
	{
		sCmdLine.Empty();
	}

	/* Determine features to be installed
	 */
	if (! sCmdLine.IsEmpty())
	{
		sCmdLine += _T(" ");
	}

	if (szFeatures == NULL)
	{
		sCmdLine += _T("ADDLOCAL=ALL");
	}
	else
	{
		sCmdLine += _T("ADDLOCAL=");
		sCmdLine += szFeatures;
	}
	
    if (m_bDebugLog)
    {
		// Enable Log
        DWORD dwLogMode =	INSTALLLOGMODE_ERROR		| INSTALLLOGMODE_WARNING | 
							INSTALLLOGMODE_OUTOFMEMORY	| INSTALLLOGMODE_USER |
							INSTALLLOGMODE_INFO			|	INSTALLLOGMODE_DIAGNOSTIC |
							INSTALLLOGMODE_COMMONDATA	| INSTALLLOGMODE_RESERVED |
							INSTALLLOGMODE_ACTIONSTART	| INSTALLLOGMODE_ACTIONDATA |
							INSTALLLOGMODE_PROPERTYDUMP;

		pMsiEnableLogA(dwLogMode, SET_LOG_INSTALL, TRUE);
    }
    else
    {
		pMsiEnableLogA(0, NULL, TRUE);
    }
	
	// Set internal UI Level
	pMsiSetInternalUI(dwUILevel, &hWnd);
		
	Trc("InstallMsiPackage", "", "%s  %s", sPath, sCmdLine);

	// Install
	return pMsiInstallProductA(sPath, sCmdLine);
}


/* I N S T A L L A T I O N   S T A T E
 * ----------------------------------------------------------------------------
 */

/* ActivateList
 * ----------------------------------------------------------------------------
 */
void CInstall::ActivateList(CPropBase *pProp)
{
	/* Update Listbox
	 */
	UpdateListbox(TRUE);
	EnableControls(pProp);
	SelectNextLBItem(TRUE, pProp);
}


/* GetDefaultToDo
 * ----------------------------------------------------------------------------
 */
DWORD CInstall::GetDefaultToDo()
{
	DWORD dwToDo = SET_FLAG_KERNEL;

	dwToDo |= m_sType == SET_TYPE_PC		 ? SET_FLAG_CTRL | SET_FLAG_PCAO | SET_FLAG_ECAO | SET_FLAG_FCAO | SET_FLAG_BCAO | SET_FLAG_XCAO | SET_FLAG_GAAO | SET_FLAG_DEMO | SET_FLAG_PCDB | SET_FLAG_PCCL | SET_FLAG_CONS : 0;
	dwToDo |= m_sType == SET_TYPE_EMBEDDED	 ? SET_FLAG_DEMO | SET_FLAG_AO1  | SET_FLAG_AO2  | SET_FLAG_AO3  | SET_FLAG_AO4                  : 0;

	return dwToDo;
}


/* SetTodo
 * ----------------------------------------------------------------------------
 */
void CInstall::SetTodo(DWORD dwToDo)
{
	m_dwToDo = dwToDo;
}


/* IsOKMS
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::IsOKMS()
{
	BOOL	bInst = TRUE;
	CState *pState;

	for (WORD w = 0; w <= SET_CTRL_LAST_MS; w++)
	{
		if (m_mAddOn.Lookup(w, (CObject*&)pState))
		{
			bInst &= (pState->m_uState == IDS_OK);
		}
	}

	return bInst;
}


/* IsOKKern
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::IsOKKern()
{
	BOOL	bInst = TRUE;
	CState	*pState;

	for (WORD w = SET_CTRL_LAST_MS; w <= SET_CTRL_LAST_4C; w++)
	{
		if (w == SET_CTRL_KEY_CTRL)
		{
			continue;
		}
		
		if (m_mAddOn.Lookup(w, (CObject*&)pState))
		{
			bInst &= (pState->m_uState == IDS_OK);
		}
	}

	return bInst;
}


/* IsNotInstKern
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::IsNotInstKern()
{
	BOOL	bNotInst = FALSE;
	CState	*pState;

	if (m_mAddOn.Lookup(SET_CTRL_KEY_KRNL, (CObject*&)pState))
	{
		bNotInst = (pState->m_uState == IDS_INSTALL);
	}

	return bNotInst;
}


/* IsNotInstAddOn
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::IsNotInstAddOn()
{
	BOOL	bNotInst = TRUE;
	CState	*pState;
	WORD	wKey;

	for (POSITION pos = m_mAddOn.GetStartPosition(); pos != NULL; )
	{
		m_mAddOn.GetNextAssoc(pos, wKey, (CObject*&)pState);

		if (wKey > SET_CTRL_KEY_KRNL) 
		{
			bNotInst &= (pState->m_uState == IDS_INSTALL);
		}
	}

	return bNotInst;
}

	
/* IsInstAnything
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::IsInstAnything()
{
	BOOL	bSomething = FALSE;
	CState	*pState;
	WORD	wKey;

	for (POSITION pos = m_mAddOn.GetStartPosition(); pos != NULL; )
	{
		m_mAddOn.GetNextAssoc(pos, wKey, (CObject*&)pState);

		if (wKey > SET_CTRL_LAST_MS) 
		{
			bSomething |= (pState->m_uState != IDS_INSTALL);
		}
	}

	return bSomething;
}


/* IsInstSomething
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::IsInstSomething()
{
	BOOL	bSomething = FALSE;
	CState	*pState;
	WORD	wKey;

	for (POSITION pos = m_mAddOn.GetStartPosition(); pos != NULL; )
	{
		m_mAddOn.GetNextAssoc(pos, wKey, (CObject*&)pState);

		if (wKey > SET_CTRL_LAST_MS && wKey != SET_CTRL_KEY_LM) 
		{
			bSomething |= (pState->m_uState != IDS_INSTALL);
		}
	}

	return bSomething;
}


/* IsInst4C
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::IsInst4C(LPCTSTR szKey)
{
	CString sVersion;

	UINT uState = Check4C(szKey, sVersion);

	return uState == IDS_OK || uState == IDS_REINSTALL;
}


/* DeleteWordMap
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::DeleteWordMap(CMapWordToOb *pMap)
{
	POSITION pos;
	WORD	 wKey;
	CState	*pState;

	for(pos = pMap->GetStartPosition(); pos != NULL;)
	{
		pMap->GetNextAssoc(pos, wKey, (CObject*&)pState);
		delete pState;
	}
	pMap->RemoveAll();

	return TRUE;
}


/* DeleteStringMap
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::DeleteStringMap(CMapStringToOb *pMap)
{
	POSITION pos;
	CString	 sKey;
	CState	*pState;

	for(pos = pMap->GetStartPosition(); pos != NULL;)
	{
		pMap->GetNextAssoc(pos, sKey, (CObject*&)pState);
		delete pState;
	}
	pMap->RemoveAll();

	return TRUE;
}


/* DeleteStringMap
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::ToBeInstalled(WORD wKey, LPCTSTR szKey)
{
	switch (wKey)
	{
		case SET_CTRL_KEY_IIS:
		{
			if (CheckValueInIni(SET_SCT_GENERAL, SET_INI_SYSTEM, szKey))
			{
				// Only if Console is to be installed
				return (m_dwToDo & SET_FLAG_CONS)  != 0;
			}
			break;
		}

		case SET_CTRL_KEY_SP:
		case SET_CTRL_KEY_IE:
		case SET_CTRL_KEY_MSI:
		case SET_CTRL_KEY_JDK:
		case SET_CTRL_KEY_JVM:
		{
			return CheckValueInIni(SET_SCT_GENERAL, SET_INI_SYSTEM, szKey);
		}

		case SET_CTRL_KEY_LM:
		{
			return CheckValueInIni(SET_SCT_GENERAL, SET_INI_4CLM, szKey) || IsInst4C(szKey);
		}

		case SET_CTRL_KEY_CTRL:
		{
			return (CheckValueInIni(SET_SCT_GENERAL, SET_INI_4CCONTROL, szKey) && (m_dwToDo & SET_FLAG_CTRL)  != 0) || IsInst4C(szKey);
		}

		case SET_CTRL_KEY_KRNL: 
		{
			return (CheckValueInIni(SET_SCT_GENERAL, SET_INI_4CKERNEL, szKey) && (m_dwToDo & SET_FLAG_KERNEL)  != 0) || IsInst4C(szKey) ;
		}

		default:
		{
			if (m_sType == SET_TYPE_PC)
			{
				CString sKey(szKey);

				if (sKey == SET_PRODKEY_PCAO)
				{ 
					return ((m_dwToDo & SET_FLAG_PCAO) != 0) && CheckValueInIni(SET_SCT_GENERAL, SET_INI_4CADDON, szKey);
				}
				
				if (sKey == SET_PRODKEY_ECAO)		
				{ 
					return ((m_dwToDo & SET_FLAG_ECAO) != 0) && CheckValueInIni(SET_SCT_GENERAL, SET_INI_4CADDON, szKey);
				}
				
				if (sKey == SET_PRODKEY_FCAO)		
				{ 
					return ((m_dwToDo & SET_FLAG_FCAO) != 0) && CheckValueInIni(SET_SCT_GENERAL, SET_INI_4CADDON, szKey);
				}
				
				if (sKey == SET_PRODKEY_BCAO)		
				{ 
					return ((m_dwToDo & SET_FLAG_BCAO) != 0) && CheckValueInIni(SET_SCT_GENERAL, SET_INI_4CADDON, szKey);
				}
				
				if (sKey == SET_PRODKEY_XCAO)		
				{ 
					return ((m_dwToDo & SET_FLAG_XCAO) != 0) && CheckValueInIni(SET_SCT_GENERAL, SET_INI_4CADDON, szKey);
				}
				
				if (sKey == SET_PRODKEY_GAAO)
				{ 
					return ((m_dwToDo & SET_FLAG_GAAO) != 0) && CheckValueInIni(SET_SCT_GENERAL, SET_INI_4CADDON, szKey);
				}
				
				if (sKey == SET_PRODKEY_DEMO)		
				{ 
					return ((m_dwToDo & SET_FLAG_DEMO) != 0) && CheckValueInIni(SET_SCT_GENERAL, SET_INI_4CADDON, szKey);
				}
				
				if (sKey == SET_PRODKEY_PCDB)		
				{ 
					return ((m_dwToDo & SET_FLAG_PCDB) != 0) && CheckValueInIni(SET_SCT_GENERAL, SET_INI_4CADDON, szKey);
				}
				
				if (sKey == SET_PRODKEY_PCCL)		
				{ 
					return ((m_dwToDo & SET_FLAG_PCCL) != 0) && CheckValueInIni(SET_SCT_GENERAL, SET_INI_4CADDON, szKey); 
				}

				return ((m_dwToDo & SET_FLAG_KERNEL) != 0) && CheckValueInIni(SET_SCT_GENERAL, SET_INI_4CADDON, szKey);
			}

			if (m_sType == SET_TYPE_EMBEDDED)
			{
				CString sKey(szKey);
				if (sKey == SET_PRODKEY_DEMO)		
				{ 
					return ((m_dwToDo & SET_FLAG_DEMO) != 0) && CheckValueInIni(SET_SCT_GENERAL, SET_INI_4CADDON, szKey); 
				}

				CString sVal;

				GetKeyFromIni(SET_SCT_CONVERSION, SET_PRODKEY_AO1, sVal);
				if (sVal == szKey)
				{
					return ((m_dwToDo & SET_FLAG_AO1) != 0) && CheckValueInIni(SET_SCT_GENERAL, SET_INI_4CADDON, szKey);
				}

				GetKeyFromIni(SET_SCT_CONVERSION, SET_PRODKEY_AO2, sVal);
				if (sVal == szKey)
				{
					return ((m_dwToDo & SET_FLAG_AO2) != 0) && CheckValueInIni(SET_SCT_GENERAL, SET_INI_4CADDON, szKey);
				}

				GetKeyFromIni(SET_SCT_CONVERSION, SET_PRODKEY_AO3, sVal);
				if (sVal == szKey)
				{
					return ((m_dwToDo & SET_FLAG_AO3) != 0) && CheckValueInIni(SET_SCT_GENERAL, SET_INI_4CADDON, szKey);
				}

				GetKeyFromIni(SET_SCT_CONVERSION, SET_PRODKEY_AO4, sVal);
				if (sVal == szKey)
				{
					return ((m_dwToDo & SET_FLAG_AO4) != 0) && CheckValueInIni(SET_SCT_GENERAL, SET_INI_4CADDON, szKey);
				}
				
				return ((m_dwToDo & SET_FLAG_KERNEL) != 0) && CheckValueInIni(SET_SCT_GENERAL, SET_INI_4CADDON, szKey);
			
			} // if (m_sType == SET_TYPE_EMBEDDED)

		} // default:

	} // switch(wKey)

	return FALSE;
}



/* L I S T   C O N T R O L
 * ----------------------------------------------------------------------------
 */

/* SetListCtrl
 * ----------------------------------------------------------------------------
 */
void CInstall::SetListCtrl(CListCtrl *pListCtrl)
{
	m_pListCtrl = pListCtrl;
	ASSERT(m_pListCtrl);

	if (m_pListCtrl != NULL)
	{
		m_pListCtrl->SetImageList(&m_ImageList, LVSIL_SMALL);
		CreateLBColumns();
	}
}


/* GetListCtrl
 * ----------------------------------------------------------------------------
 */
CListCtrl *CInstall::GetListCtrl()
{
	ASSERT(m_pListCtrl);

	return m_pListCtrl;
}


/* UpdateListbox
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::UpdateListbox(BOOL bRefill)
{
	if (m_pListCtrl == NULL)
	{
		// Listbox is still not visible --> rebuild always!
		bRefill = TRUE;
	}

    if (bRefill)
	{
		DestroyLBContents();
	}

	CString sDummy;
	CString sVersion;

	INT  iIndex = 0;

	BOOL bValidOS;
	BOOL bInst;

	/* ------------------------------------------------------------------------ */

	if (ToBeInstalled(SET_CTRL_KEY_IIS, SET_PRODKEY_IIS))
	{
		bInst = ! CheckIIS(sVersion);

		if (bRefill)
		{
			GetKeyFromIni(SET_SCT_MINVER, SET_PRODKEY_IIS, sDummy);
			AddLBItem(iIndex, IDS_LB_IIS, sDummy, SET_CTRL_KEY_IIS, SET_PRODKEY_IIS, SET_ICO_IIS);
		}
		UpdateLBItem(iIndex, SET_CTRL_KEY_IIS, sVersion, bInst ? IDS_INSTALL : IDS_OK);		

		iIndex++;
	}
	
	/* ------------------------------------------------------------------------ */

	if (ToBeInstalled(SET_CTRL_KEY_SP, SET_PRODKEY_SP))
	{
		bInst = ! CheckSP(sVersion, sDummy, &bValidOS);

		if (bRefill)
		{
			AddLBItem(iIndex, IDS_OSVERSION, sDummy, SET_CTRL_KEY_SP, SET_PRODKEY_SP, SET_ICO_OS);
		}	
		UpdateLBItem(iIndex, SET_CTRL_KEY_SP, sVersion, bInst ? IDS_INSTALL : IDS_OK);

		iIndex++;
	}

	/* ------------------------------------------------------------------------ */

	if (ToBeInstalled(SET_CTRL_KEY_IE, SET_PRODKEY_IE))
	{
		bInst = ! CheckIE(sVersion);

		if (bRefill)
		{
			GetKeyFromIni(SET_SCT_MINVER, SET_PRODKEY_IE, sDummy);
			AddLBItem(iIndex, IDS_LB_MSIE, sDummy, SET_CTRL_KEY_IE, SET_PRODKEY_IE, SET_ICO_IE);
		}
		UpdateLBItem(iIndex, SET_CTRL_KEY_IE, sVersion, bInst ? IDS_INSTALL : IDS_OK);		

		iIndex++;
	}

	/* ------------------------------------------------------------------------ */

	if (ToBeInstalled(SET_CTRL_KEY_MSI, SET_PRODKEY_MSI))
	{
		bInst = ! CheckMSI(sVersion);

		if (bRefill)
		{
			GetKeyFromIni(SET_SCT_MINVER, SET_PRODKEY_MSI, sDummy);
			AddLBItem(iIndex, IDS_LB_MSI, sDummy, SET_CTRL_KEY_MSI, SET_PRODKEY_MSI, SET_ICO_MSI);
		}
		UpdateLBItem(iIndex, SET_CTRL_KEY_MSI, sVersion, bInst ? IDS_INSTALL : IDS_OK);		

		iIndex++;
	}
	
	/* ------------------------------------------------------------------------ */

	if (ToBeInstalled(SET_CTRL_KEY_JDK, SET_PRODKEY_JDK))
	{
		bInst = ! CheckJDK(sVersion);

		if (bRefill)
		{
			GetKeyFromIni(SET_SCT_MINVER, SET_PRODKEY_JDK, sDummy);
			AddLBItem(iIndex, IDS_LB_JDK, sDummy, SET_CTRL_KEY_JDK, SET_PRODKEY_JDK, SET_ICO_J);
		}
		UpdateLBItem(iIndex, SET_CTRL_KEY_JDK, sVersion, bInst ? IDS_INSTALL : IDS_OK);		

		iIndex++;
	}
	
	/* ------------------------------------------------------------------------ */

	if (ToBeInstalled(SET_CTRL_KEY_JVM, SET_PRODKEY_JVM))
	{
		bInst = ! CheckJVM(sVersion);

		if (bRefill)
		{
			GetKeyFromIni(SET_SCT_MINVER, SET_PRODKEY_JVM, sDummy);
			AddLBItem(iIndex, IDS_LB_JVM, sDummy, SET_CTRL_KEY_JVM, SET_PRODKEY_JVM, SET_ICO_J);
		}
		UpdateLBItem(iIndex, SET_CTRL_KEY_JVM, sVersion, bInst ? IDS_INSTALL : IDS_OK);		

		iIndex++;
	}
	
	/* ------------------------------------------------------------------------ */

	if (ToBeInstalled(SET_CTRL_KEY_LM, SET_PRODKEY_LM))
	{
		UINT uState = CheckLM(sVersion);

		if (bRefill)
		{
			GetKeyFromIni(SET_SCT_MINVER, SET_PRODKEY_LM, sDummy);
			AddLBItem(iIndex, IDS_LB_LM, sDummy, SET_CTRL_KEY_LM, SET_PRODKEY_LM, SET_ICO_LM);
		}
		UpdateLBItem(iIndex, SET_CTRL_KEY_LM, sVersion, uState);

		iIndex++;
	}

	/* ------------------------------------------------------------------------ */

	if (ToBeInstalled(SET_CTRL_KEY_CTRL, SET_PRODKEY_CTRL))
	{
		if (bRefill)
		{
			CString sMinVer;

			GetKeyFromIni(SET_SCT_DESCRIPTION,	SET_PRODKEY_CTRL, sDummy);
			GetKeyFromIni(SET_SCT_MINVER,		SET_PRODKEY_CTRL, sMinVer);
			AddLBItem(iIndex, sDummy, sMinVer, SET_CTRL_KEY_CTRL, SET_PRODKEY_CTRL, SET_ICO_C2);
		}

		UINT uState = Check4C(SET_PRODKEY_CTRL, sVersion);
		UpdateLBItem(iIndex, SET_CTRL_KEY_CTRL, sVersion, uState);

		iIndex++;
	}

	/* ------------------------------------------------------------------------ */

	WORD	w4CItem = 0;
	WORD	wKey	= (WORD)(SET_CTRL_KEY_KRNL + w4CItem);

	if (ToBeInstalled(SET_CTRL_KEY_KRNL, SET_PRODKEY_KERNEL))
	{
		if (bRefill)
		{
			CString sMinVer;

			GetKeyFromIni(SET_SCT_DESCRIPTION,	SET_PRODKEY_KERNEL, sDummy);
			GetKeyFromIni(SET_SCT_MINVER,		SET_PRODKEY_KERNEL, sMinVer);
			AddLBItem(iIndex, sDummy, sMinVer, SET_CTRL_KEY_KRNL, SET_PRODKEY_KERNEL, SET_ICO_4CKERN);
		}

		UINT uState = Check4C(SET_PRODKEY_KERNEL, sVersion);
		UpdateLBItem(iIndex, SET_CTRL_KEY_KRNL, sVersion, uState);		
		
		iIndex++;
	}

	w4CItem++;

	/* ------------------------------------------------------------------------ */
	
	CString sKey;

	CString sAddOn;
	GetKeyFromIni(SET_SCT_GENERAL, SET_INI_4CADDON, sAddOn);

	while (! sAddOn.IsEmpty())
	{
		wKey = (WORD)(SET_CTRL_KEY_KRNL + w4CItem);

		int iPos = sAddOn.Find(",");
		
		if (iPos == -1) {
			sKey = sAddOn;
		} else 	{
			sKey = sAddOn.Left(iPos);
		}

		if (ToBeInstalled(wKey, sKey))
		{
			if (bRefill)
			{
				CString sIcon;
				CString sMinVer;

				GetKeyFromIni(SET_SCT_ICON,			sKey, sIcon);
				GetKeyFromIni(SET_SCT_MINVER,		sKey, sMinVer);
				GetKeyFromIni(SET_SCT_DESCRIPTION,	sKey, sDummy);			
				char *szDummy;
				AddLBItem(iIndex, sDummy, sMinVer, wKey, sKey, strtoul(sIcon, &szDummy, 10));
			}
			UINT uState = Check4C(sKey, sVersion);
			UpdateLBItem(iIndex, wKey, sVersion, uState);
		
			iIndex++;
		}
		
		if (iPos == -1) {
			sAddOn.Empty();
		} else {
			sKey	= sAddOn.Mid(iPos + 1);
			sAddOn	= sKey;
		}

		w4CItem++;
	}

	/* ------------------------------------------------------------------------ */
	
	// Handle all allready installed 4C components which are not part of this
	// package.

	if (m_pListCtrl != NULL)
	{
		for (int i = m_pListCtrl->GetItemCount() - 1; i >= m_iLastRegular; i--)
		{
			// Delete old entries first
			DeleteLBItem(i);
		}
	}

	m_iLastRegular = iIndex;

	/* ------------------------------------------------------------------------ */
	
	GetKeyFromIni(SET_SCT_COMPONENTS, SET_INI_4CALL, sAddOn);

	while (! sAddOn.IsEmpty())
	{
		wKey = (WORD)(SET_CTRL_KEY_KRNL + w4CItem);

		int iPos = sAddOn.Find(",");
		
		if (iPos == -1) {
			sKey = sAddOn;
		} else 	{
			sKey = sAddOn.Left(iPos);
		}

		if (! ToBeInstalled(wKey, sKey))
		{
			UINT uState	  = Check4C(sKey, sVersion);

			if (uState == IDS_OK || uState == IDS_REINSTALL)
			{
				CString sIcon;
				CString sMinVer;

				GetKeyFromIni(SET_SCT_ICON,			sKey, sIcon);
				GetKeyFromIni(SET_SCT_MINVER,		sKey, sMinVer);
				GetKeyFromIni(SET_SCT_DESCRIPTION,	sKey, sDummy);			
				char *szDummy;
				AddLBItem(iIndex, sDummy, sMinVer, wKey, sKey, strtoul(sIcon, &szDummy, 10));
				
				UpdateLBItem(iIndex, wKey, sVersion, uState);

				iIndex++;
				w4CItem++;
			}
		}
		
		if (iPos == -1) {
			sAddOn.Empty();
		} else {
			sKey	= sAddOn.Mid(iPos + 1);
			sAddOn	= sKey;
		}
	}

	m_wMaxKey = (WORD)(SET_CTRL_KEY_KRNL + w4CItem + 1);

	return TRUE;
}


/* DestroyLBContents
 * ----------------------------------------------------------------------------
 */
void CInstall::DestroyLBContents()
{
	if (m_pListCtrl != NULL && IsWindow(m_pListCtrl->GetSafeHwnd()))
	{
		m_pListCtrl->DeleteAllItems();
	}

	DeleteWordMap(&m_mAddOn);

	m_iLastRegular = 0x7fffffff;
}


/* AddLBItem
 * ----------------------------------------------------------------------------
 */
void CInstall::AddLBItem(INT iIndex, UINT uItem, LPCTSTR szVersion, WORD wKey, LPCTSTR szKey, UINT uImage)
{
	CString sItem;
	sItem.LoadString(uItem);

	AddLBItem(iIndex, sItem, szVersion, wKey, szKey, uImage);
}


/* AddLBItem
 * ----------------------------------------------------------------------------
 */
void CInstall::AddLBItem(INT iIndex, LPCTSTR szItem, LPCTSTR szVersion, WORD wKey, LPCTSTR szKey, UINT uImage)
{
	Trc("AddLBItem",    "", "Key = %2d  Version = %-20s  Item = %-30s", wKey, szVersion, szItem);

	if (m_pListCtrl != NULL)
	{
		m_pListCtrl->InsertItem (iIndex, szItem, uImage);
		m_pListCtrl->SetItemText(iIndex, 2, szVersion);
		m_pListCtrl->SetItemData(iIndex, wKey);
	}

	CState *pState = new CState(szKey, IDS_OK);
	m_mAddOn.SetAt(wKey, pState);
}


/* DeleteLBItem
 * ----------------------------------------------------------------------------
 */
void CInstall::DeleteLBItem(INT iIndex)
{
	if (m_pListCtrl == NULL)
	{
		ASSERT(m_pListCtrl);
		return;
	}

	CState *pState;
	WORD wKey = (WORD)m_pListCtrl->GetItemData(iIndex);

	if (m_mAddOn.Lookup(wKey, (CObject*&)pState))
	{
		m_mAddOn.RemoveKey(wKey);
		delete pState;
	}

	if (m_pListCtrl != NULL)
	{
		m_pListCtrl->DeleteItem(iIndex);
	}
}


/* UpdateLBItem
 * ----------------------------------------------------------------------------
 */
void CInstall::UpdateLBItem(INT iIndex, WORD wKey, LPCTSTR szVersion, UINT uState)
{
	CString sAction;
	sAction.LoadString(uState);

	Trc("UpdateLBItem", "", "Key = %2d  Version = %-20s", wKey, szVersion);

	if (m_pListCtrl != NULL)
	{
		m_pListCtrl->SetItemText(iIndex, 1, szVersion);
		m_pListCtrl->SetItemText(iIndex, 3, sAction);
	}

	CState *pState;
	if (m_mAddOn.Lookup(wKey, (CObject*&)pState))
	{
		pState->m_uState = uState;
	}
}


/* SelectLBItem
 * ----------------------------------------------------------------------------
 */
void CInstall::SelectLBItem(WORD wKey)
{
	for (int i = 0; i < m_pListCtrl->GetItemCount(); i++)
	{
		if (wKey == (WORD)m_pListCtrl->GetItemData(i))
		{
			m_pListCtrl->SetItem(i, 0, LVIF_STATE, NULL, 0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED, 0);
			m_pListCtrl->EnsureVisible(i, FALSE);

			break;
		}
	}
}


/* SelectNextLBItem
 * ----------------------------------------------------------------------------
 */
void CInstall::SelectNextLBItem(BOOL bNext, CPropBase *pProp)
{
	if (bNext)
	{
		for (int i = 0; i < m_pListCtrl->GetItemCount(); i++)
		{
			WORD wKey = (WORD)m_pListCtrl->GetItemData(i);

			CState *pState;
			if (m_mAddOn.Lookup(wKey, (CObject*&)pState))
			{
				if (pState->m_uState == IDS_INSTALL)
				{
					m_pListCtrl->SetItem(i, 0, LVIF_STATE, NULL, 0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED, 0);
					m_pListCtrl->EnsureVisible(i, FALSE);

					pProp->GetDlgItem(IDC_INSTALL)->SetFocus();

					break;
				}
				else
				{
					m_pListCtrl->SetItem(i, 0, LVIF_STATE, NULL, 0, 0, 0, 0);
				}
			}
		}

		if (i == m_pListCtrl->GetItemCount())
		{
			pProp->GetParent()->GetDlgItem(IDCANCEL)->SetFocus();
		}
	}
	else
	{
		for (int i = m_pListCtrl->GetItemCount() - 1; i >= 0; i--)
		{
			WORD wKey = (WORD)m_pListCtrl->GetItemData(i);
			if (wKey < SET_CTRL_KEY_KRNL)
			{
				m_pListCtrl->SetItem(i, 0, LVIF_STATE, NULL, 0, 0, 0, 0);
				continue;
			}

			CState *pState;
			if (m_mAddOn.Lookup(wKey, (CObject*&)pState))
			{
				if (pState->m_uState != IDS_INSTALL)
				{
					m_pListCtrl->SetItem(i, 0, LVIF_STATE, NULL, 0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED, 0);
					m_pListCtrl->EnsureVisible(i, FALSE);

					pProp->GetDlgItem(IDC_UNINSTALL)->SetFocus();

					break;
				}
				else
				{
					m_pListCtrl->SetItem(i, 0, LVIF_STATE, NULL, 0, 0, 0, 0);
				}
			}
		}

		if (i < 0)
		{
			pProp->GetParent()->GetDlgItem(IDCANCEL)->SetFocus();
		}
	}
}


/* CreateImageList
 * ----------------------------------------------------------------------------
 */
void CInstall::CreateImageList()
{
	HICON h;

	m_ImageList.Create(16, 16, ILC_MASK, 10, 10);
	
	h = AfxGetApp()->LoadIcon(IDI_INST_OS);
	m_ImageList.Add(h);
	h = AfxGetApp()->LoadIcon(IDI_INST_IE);
	m_ImageList.Add(h);
	h = AfxGetApp()->LoadIcon(IDI_INST_MSI);
	m_ImageList.Add(h);
	h = AfxGetApp()->LoadIcon(IDI_INST_IIS);
	m_ImageList.Add(h);
	h = AfxGetApp()->LoadIcon(IDI_INST_J);
	m_ImageList.Add(h);

	h = AfxGetApp()->LoadIcon(IDI_INST_4CLM);
	m_ImageList.Add(h);

	h = AfxGetApp()->LoadIcon(IDI_INST_4CKERN);
	m_ImageList.Add(h);
	
	h = AfxGetApp()->LoadIcon(IDI_INST_4CADDON);
	m_ImageList.Add(h);
	
	h = AfxGetApp()->LoadIcon(IDI_INST_C1);
	m_ImageList.Add(h);
	h = AfxGetApp()->LoadIcon(IDI_INST_C2);
	m_ImageList.Add(h);
	h = AfxGetApp()->LoadIcon(IDI_INST_C3);
	m_ImageList.Add(h);
	h = AfxGetApp()->LoadIcon(IDI_INST_C4);
	m_ImageList.Add(h);
	h = AfxGetApp()->LoadIcon(IDI_INST_C5);
	m_ImageList.Add(h);
}


/* CreateLBColumns
 * ----------------------------------------------------------------------------
 */
void CInstall::CreateLBColumns()
{
	if (m_pListCtrl == NULL)
	{
		return;
	}

    /* Create columns for list control
	 */
	CString strHelp;
    strHelp.LoadString(IDS_LBC_ACTION);
    m_pListCtrl->InsertColumn(0, strHelp, LVCFMT_LEFT, 90); 

    strHelp.LoadString(IDS_LBC_EXPECTED_VERSION);
    m_pListCtrl->InsertColumn(0, strHelp, LVCFMT_LEFT, 115); 

    strHelp.LoadString(IDS_LBC_VERSION);
    m_pListCtrl->InsertColumn(0, strHelp, LVCFMT_LEFT, 175); 

    strHelp.LoadString(IDS_LBC_PRODUCT);
    m_pListCtrl->InsertColumn(0, strHelp, LVCFMT_LEFT, 197); 
}


/* DisableAllControls
 * ----------------------------------------------------------------------------
 */
void CInstall::DisableAllControls(CPropBase *pProp)
{
	m_bDisable = TRUE;

	pProp->GetDlgItem(IDC_INSTALL)->EnableWindow(FALSE);
	pProp->GetDlgItem(IDC_UNINSTALL)->EnableWindow(FALSE);

	pProp->GetDlgItem(IDC_INSTALL_ALL)->EnableWindow(FALSE);
	pProp->GetDlgItem(IDC_UNINSTALL_ALL)->EnableWindow(FALSE);

	pProp->GetParent()->GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
	pProp->GetPropSheet()->SetWizardButtons(0);
	pProp->EnableClose(FALSE);

	m_pListCtrl->EnableWindow(FALSE);
}


/* EnableAllControls
 * ----------------------------------------------------------------------------
 */
void CInstall::EnableAllControls(CPropBase *pProp)
{
	m_bDisable = FALSE;

	SetWizardButtons(pProp);
	pProp->EnableClose(TRUE);

	m_pListCtrl->EnableWindow(TRUE);

	EnableControls(pProp);
}


/* EnableControls
 * ----------------------------------------------------------------------------
 */
void CInstall::EnableControls(CPropBase *pProp)
{
	WORD	wSelKey	  = (WORD)-1;
	int		iSelected = m_pListCtrl->GetNextItem(-1, LVNI_SELECTED);

	CState *pState;
	
	if(iSelected != -1)
	{
		wSelKey = (WORD)m_pListCtrl->GetItemData(iSelected);
	}

	/* Install button
	 */
	pProp->GetDlgItem(IDC_INSTALL)->EnableWindow(FALSE);

	if (m_bUnInstallOnly == FALSE && iSelected != -1 && m_mAddOn.Lookup(wSelKey, (CObject*&)pState))
	{
		if (wSelKey <= SET_CTRL_LAST_MS)
		{
			// MS
			pProp->GetDlgItem(IDC_INSTALL)->EnableWindow(pState->m_uState == IDS_INSTALL);
		}
		else if (wSelKey <= SET_CTRL_LAST_4C)
		{
			// Kernel and LM and Control
			pProp->GetDlgItem(IDC_INSTALL)->EnableWindow(pState->m_uState == IDS_INSTALL && IsOKMS());
		} 
		else 
		{
			// AddOn
			pProp->GetDlgItem(IDC_INSTALL)->EnableWindow(pState->m_uState == IDS_INSTALL && IsOKMS() && IsOKKern());
		}
	}

	/* Uninstall button
	 */
	pProp->GetDlgItem(IDC_UNINSTALL)->EnableWindow(FALSE);

	if (iSelected != -1 && m_mAddOn.Lookup(wSelKey, (CObject*&)pState))
	{
		if (wSelKey <= SET_CTRL_LAST_MS)
		{
			// MS
			pProp->GetDlgItem(IDC_UNINSTALL)->EnableWindow(FALSE);
		}
		else if (wSelKey > SET_CTRL_LAST_4C)
		{
			// AddOn
			pProp->GetDlgItem(IDC_UNINSTALL)->EnableWindow(pState->m_uState != IDS_INSTALL);
		}
		else if (wSelKey == SET_CTRL_KEY_CTRL)
		{
			// Control
			pProp->GetDlgItem(IDC_UNINSTALL)->EnableWindow(pState->m_uState != IDS_INSTALL);
		}
		else if (wSelKey == SET_CTRL_KEY_LM)
		{
			// LM
			pProp->GetDlgItem(IDC_UNINSTALL)->EnableWindow(pState->m_uState != IDS_INSTALL);
		}
		else
		{
			// Kernel
			pProp->GetDlgItem(IDC_UNINSTALL)->EnableWindow(pState->m_uState != IDS_INSTALL && IsNotInstAddOn());
		}
	}

	/* Un-/Install all buttons
	 */
	pProp->GetDlgItem(IDC_UNINSTALL_ALL)->EnableWindow(GetUnInstallAllState(pProp));

	pProp->GetDlgItem(IDC_INSTALL_ALL)->EnableWindow(m_bUnInstallOnly == FALSE && GetInstallAllState(pProp));
}


/* GetInstallAllState
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::GetInstallAllState(CPropBase *pProp)
{
	POSITION pos;
	BOOL bInst	= FALSE;
	BOOL bOK	= TRUE;

	for(pos = m_mAddOn.GetStartPosition(); pos != NULL; )
	{
		WORD wKey;
		CState *pState;
		m_mAddOn.GetNextAssoc(pos, wKey, (CObject*&)pState);

		bInst |= (pState->m_uState == IDS_INSTALL);
		bOK   &= (pState->m_uState != IDS_REINSTALL);
	}

	return bInst && bOK;
}


/* GetUnInstallAllState
 * ----------------------------------------------------------------------------
 */
BOOL CInstall::GetUnInstallAllState(CPropBase *pProp)
{
	return IsInstAnything();
}


/* SetWizardButtons
 * ----------------------------------------------------------------------------
 */
void CInstall::SetWizardButtons(CPropBase *pProp)
{
	if (GetInstallAllState(pProp) == FALSE)
	{
		pProp->GetPropSheet()->SetWizardButtons(PSWIZB_FINISH);
		pProp->GetParent()->GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
	}
	else
	{
		pProp->GetPropSheet()->SetWizardButtons(m_bUnInstallOnly ? 0 : PSWIZB_BACK);
		pProp->GetParent()->GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
	}
}


/* OnItemChangedListCtrl
 * ----------------------------------------------------------------------------
 */
void CInstall::OnItemChangedListCtrl(CPropBase *pProp, NM_LISTVIEW* pNMListView)
{
    if (!m_bDisable && pNMListView->iItem >= 0 && (pNMListView->uNewState & LVIS_SELECTED)) 
	{
		EnableControls(pProp);
    }
}


/* OnInstall
 * ----------------------------------------------------------------------------
 */
void CInstall::OnInstall(CPropBase *pProp) 
{
	int iSelected = m_pListCtrl->GetNextItem(-1, LVNI_SELECTED);
	if (iSelected == -1)
	{
		return;
	}

	OnBeginInstall();
	{
		DisableAllControls(pProp);

		m_bInstallAll = FALSE;

		WORD wKey = (WORD)m_pListCtrl->GetItemData(iSelected);

		Trc("OnInstall", "", "Key = %2d", wKey);

		switch (wKey)
		{
			case SET_CTRL_KEY_SP :
			{
				InstallSP(pProp);
				break;
			}

			case SET_CTRL_KEY_IE :
			{
				InstallIE(pProp);
				break;
			}

			case SET_CTRL_KEY_MSI : 
			{
				InstallMSI(pProp);
				break;
			}

			case SET_CTRL_KEY_JDK : 
			{
				InstallJDK(pProp);
				break;
			}

			case SET_CTRL_KEY_JVM : 
			{
				InstallJVM(pProp);
				break;
			}

			case SET_CTRL_KEY_IIS : 
			{
				InstallIIS(pProp);
				break;
			}

			case SET_CTRL_KEY_CTRL :
			{
				Install4C(pProp, SET_PRODKEY_CTRL);
				break;
			}

			case SET_CTRL_KEY_LM :
			{
				Install4CComponent(pProp, SET_PRODKEY_LM);
				break;
			}

			default:
			{
				if (wKey < SET_CTRL_KEY_KRNL)
				{
					break;
				}

				CState *pState;
				if (! m_mAddOn.Lookup(wKey, (CObject*&)pState))
				{
					break;
				}

				Install4C(pProp, pState->m_sKey);

				break;
			}
		}	

		UpdateListbox(FALSE);
		EnableAllControls(pProp);
		SelectNextLBItem(TRUE, pProp);

	} OnFinishInstall();
}


/* OnInstallAll
 * ----------------------------------------------------------------------------
 */
void CInstall::OnInstallAll(CPropBase *pProp)
{
	BOOL bRet = TRUE;

	DisableAllControls(pProp);

	if (m_bInstallAll == TRUE)
	{
		Sleep(1000);
	}

	OnBeginInstall();
	{
		m_bInstallAll = TRUE;

		for (WORD wKey = 0; wKey <= m_wMaxKey; wKey++)
		{
			CState *pState;
			if (! m_mAddOn.Lookup(wKey, (CObject *&)pState))
			{
				continue;
			}

			if (pState->m_uState != IDS_INSTALL)
			{
				continue;
			}

			Trc("OnInstallAll", "", "Key = %2d", wKey);

			SelectLBItem(wKey);

			switch(wKey)
			{
				case SET_CTRL_KEY_SP:
				{
					bRet = InstallSP(pProp);
					break;
				}
				case SET_CTRL_KEY_IE:
				{
					bRet = InstallIE(pProp);
					break;
				}
				case SET_CTRL_KEY_MSI:
				{
					bRet = InstallMSI(pProp);
					break;
				}
				case SET_CTRL_KEY_JDK : 
				{
					InstallJDK(pProp);
					break;
				}
				case SET_CTRL_KEY_JVM : 
				{
					InstallJVM(pProp);
					break;
				}
				case SET_CTRL_KEY_IIS : 
				{
					InstallIIS(pProp);
					break;
				}
				case SET_CTRL_KEY_LM:
				{
					bRet = Install4CComponent(pProp, pState->m_sKey);
					break;
				}
				case SET_CTRL_KEY_CTRL :
				case SET_CTRL_KEY_KRNL:
				{
					bRet = Install4C(pProp, pState->m_sKey);
					break;
				}
				default:
				{
					bRet = Install4C(pProp, pState->m_sKey);
					break;
				}
			}

			UpdateListbox(FALSE);

			Sleep(250);

			if (wKey == SET_CTRL_KEY_IIS && pState->m_uState != IDS_OK)
			{
				// Check if IIS is really installed, otherwise stop installation
				bRet = FALSE;
			}

			if (! bRet)
			{
				break;
			}
		}

		m_bInstallAll = FALSE;

		EnableAllControls(pProp);

		pProp->GetParent()->GetDlgItem(IDCANCEL)->SetFocus();

	} OnFinishInstall();

	if (m_bAutoClose == TRUE && GetInstallAllState(pProp) == FALSE)
	{
		pProp->EndDialog(IDCANCEL);
	}
}


/* OnUninstall
 * ----------------------------------------------------------------------------
 */
void CInstall::OnUninstall(CPropBase *pProp)
{
	int iSelected = m_pListCtrl->GetNextItem(-1, LVNI_SELECTED);
	if (iSelected == -1)
	{
		return;
	}

	WORD wKey = (WORD)m_pListCtrl->GetItemData(iSelected);
	if (wKey < SET_CTRL_KEY_KRNL && wKey != SET_CTRL_KEY_LM && wKey != SET_CTRL_KEY_CTRL)
	{
		return;
	}

	CState *pState;
	if (! m_mAddOn.Lookup(wKey, (CObject*&)pState))
	{
		return;
	}

	OnBeginUnInstall();
	{
		Trc("OnUninstall", "", "Key = %2d", wKey);

		m_bUnInstallAll = FALSE;

		DisableAllControls(pProp);

		if (pState->m_uState != IDS_INSTALL)
		{
			UnInstall4C(pProp, pState->m_sKey);
		}
		
		if (   (pState->m_sKey == SET_PRODKEY_KERNEL && ! ToBeInstalled(SET_CTRL_KEY_KRNL, SET_PRODKEY_KERNEL))
			|| (pState->m_sKey == SET_PRODKEY_CTRL && ! ToBeInstalled(SET_CTRL_KEY_CTRL, SET_PRODKEY_CTRL))
			|| (pState->m_sKey == SET_PRODKEY_LM && ! ToBeInstalled(SET_CTRL_KEY_LM, SET_PRODKEY_LM)))
		{
			UpdateListbox(TRUE);
		}
		else
		{
			UpdateListbox(FALSE);
		}
		EnableAllControls(pProp);
		SelectNextLBItem(FALSE, pProp);

	} OnFinishUnInstall();
}


/* OnUninstallAll
 * ----------------------------------------------------------------------------
 */
void CInstall::OnUninstallAll(CPropBase *pProp)
{
	BOOL	 bRet = TRUE;

	if (m_bUnInstallAll != TRUE)
	{
		CString sMsg;
		sMsg.LoadString(IDS_MSG_UNI_ALL);

		if (pProp->MessageBox(sMsg, m_sTitle, MB_YESNO | MB_ICONQUESTION) != IDYES)
		{
			return;
		}
	}
	
	OnBeginUnInstall();
	{
		DisableAllControls(pProp);

		m_bUnInstallAll = TRUE;

		for (WORD wKey = m_wMaxKey; wKey > SET_CTRL_LAST_MS; wKey--)
		{
			/* Uninstall 4C AddOns
			 */
			CState *pState;

			if (m_mAddOn.Lookup(wKey, (CObject*&)pState))
			{
				if (pState->m_uState != IDS_OK && pState->m_uState != IDS_REINSTALL)
				{
					continue;
				}
				
				Trc("OnUninstallAll", "", "Key = %2d", wKey);

				SelectLBItem(wKey);

				bRet = UnInstall4C(pProp, pState->m_sKey);

				if (   (pState->m_sKey == SET_PRODKEY_KERNEL && ! ToBeInstalled(SET_CTRL_KEY_KRNL, SET_PRODKEY_KERNEL))
					|| (pState->m_sKey == SET_PRODKEY_CTRL && ! ToBeInstalled(SET_CTRL_KEY_CTRL, SET_PRODKEY_CTRL))
					|| (pState->m_sKey == SET_PRODKEY_LM && ! ToBeInstalled(SET_CTRL_KEY_LM, SET_PRODKEY_LM)))
				{
					UpdateListbox(TRUE);
				}
				else
				{
					UpdateListbox(FALSE);
				}

				if (! bRet)
				{
					break;
				}

				Sleep(100);			
			}
		}

		EnableAllControls(pProp);

		m_bUnInstallAll = FALSE;

		pProp->GetParent()->GetDlgItem(IDCANCEL)->SetFocus();

	} OnFinishUnInstall();

	if (m_bAutoClose == TRUE && GetUnInstallAllState(pProp) == FALSE)
	{
		pProp->EndDialog(IDCANCEL);
	}
}


/* OnBeginInstall
 * ----------------------------------------------------------------------------
 */
void CInstall::OnBeginInstall()
{
	CString sRegKey;
	sRegKey = m_sType == SET_TYPE_PC ? SET_HVAL_4C_COMPONENTS : GetEmbeddedKey();

	CreateRegKey(HKEY_LOCAL_MACHINE, SET_HKEY_SETUP, SET_HVAL_INSTPATH, REG_SZ, (LPBYTE)(LPCTSTR)m_sInstallPath, m_sInstallPath.GetLength() + 1);
	CreateRegKey(HKEY_LOCAL_MACHINE, SET_HKEY_SETUP, sRegKey, REG_DWORD, (LPBYTE)&m_dwToDo, sizeof(m_dwToDo));
	CreateRegKey(HKEY_LOCAL_MACHINE, SET_HKEY_SETUP, SET_HVAL_LANGUAGE, REG_DWORD, (LPBYTE)&m_dwLanguage, sizeof(m_dwLanguage));
}


/* OnFinishInstall
 * ----------------------------------------------------------------------------
 */
void CInstall::OnFinishInstall()
{

}


/* OnBeginUnInstall
 * ----------------------------------------------------------------------------
 */
void CInstall::OnBeginUnInstall()
{
	CString sRegKey;
	sRegKey = m_sType == SET_TYPE_PC ? SET_HVAL_4C_COMPONENTS : GetEmbeddedKey();

	CreateRegKey(HKEY_LOCAL_MACHINE, SET_HKEY_SETUP, SET_HVAL_INSTPATH, REG_SZ, (LPBYTE)(LPCTSTR)m_sInstallPath, m_sInstallPath.GetLength() + 1);
	CreateRegKey(HKEY_LOCAL_MACHINE, SET_HKEY_SETUP, sRegKey, REG_DWORD, (LPBYTE)&m_dwToDo, sizeof(m_dwToDo));
	CreateRegKey(HKEY_LOCAL_MACHINE, SET_HKEY_SETUP, SET_HVAL_LANGUAGE, REG_DWORD, (LPBYTE)&m_dwLanguage, sizeof(m_dwLanguage));
}


/* OnFinishUnInstall
 * ----------------------------------------------------------------------------
 */
void CInstall::OnFinishUnInstall()
{
	if (! IsInstSomething())
	{
		CString sRegKey;
		sRegKey = m_sType == SET_TYPE_PC ? SET_HVAL_4C_COMPONENTS : GetEmbeddedKey();

		DeleteRegValue(HKEY_LOCAL_MACHINE, SET_HKEY_SETUP, SET_HVAL_INSTPATH);
		DeleteRegValue(HKEY_LOCAL_MACHINE, SET_HKEY_SETUP, sRegKey);
		DeleteRegValue(HKEY_LOCAL_MACHINE, SET_HKEY_SETUP, SET_HVAL_4C_COMPONENTS);
		DeleteRegValue(HKEY_LOCAL_MACHINE, SET_HKEY_SETUP, SET_HVAL_LANGUAGE);
	}
}

/* ---------------------------------------------------------------------------- */
