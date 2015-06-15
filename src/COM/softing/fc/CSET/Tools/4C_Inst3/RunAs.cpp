

#include "stdafx.h"

/* If no stdafx.h is included!
 */
#include <windows.h>
#include <tchar.h>
#include <ntsecapi.h>
#include <lm.h>

#include "atlbase.h"
#include "atlconv.h"

#include "RunAs.h"
#include "InstCheck.h"

#include "resource.h"

#define GUIDSTR_MAX 38

static DWORD	SetRunAsPassword(LPTSTR lpszAppId, LPTSTR lpszUser, LPTSTR lpszPass);
static DWORD	SetAccountRights (LPTSTR User, LPTSTR lpszPriv);
static DWORD	GetPrincipalSID (LPTSTR lpszUser, PSID *Sid);
static BOOL		LookupUserFromRid(LPTSTR lpszComputer, DWORD Rid, LPTSTR lpszUser, PDWORD cchUser);
static BOOL		LookupGroupFromRid(LPTSTR lpszComputer, DWORD Rid, LPTSTR lpszGroup, PDWORD cchGroup);


#define D_USER		_T("4ControlOPC_USR")
#define D_PWD		_T("CCCC9473snug")
#define D_COMMENT	_T("ATCMControl Administrativ System Account")



typedef NET_API_STATUS (NET_API_FUNCTION *PFNNetUserAdd)(LPCWSTR, DWORD, LPBYTE, LPDWORD);
typedef NET_API_STATUS (NET_API_FUNCTION *PFNNetLocalGroupAddMembers)(LPCWSTR, LPCWSTR, DWORD, LPBYTE, DWORD);
typedef NET_API_STATUS (NET_API_FUNCTION* PFNNetApiBufferFree1)(LPVOID);
typedef NET_API_STATUS (NET_API_FUNCTION* PFNNetUserModalsGet)(LPCWSTR, DWORD, LPBYTE*);
typedef NET_API_STATUS (NET_API_FUNCTION* PFNNetUserSetInfo)(LPCWSTR, LPCWSTR, DWORD, LPBYTE, LPDWORD);


typedef NTSTATUS (NTAPI* PFNLsaAddAccountRights)(LSA_HANDLE, PSID, PLSA_UNICODE_STRING, ULONG);
typedef NTSTATUS (NTAPI* PFNLsaClose)(LSA_HANDLE);
typedef NTSTATUS (NTAPI* PFNLsaOpenPolicy)(PLSA_UNICODE_STRING, PLSA_OBJECT_ATTRIBUTES, ACCESS_MASK, PLSA_HANDLE);
typedef NTSTATUS (NTAPI* PFNLsaStorePrivateData)(LSA_HANDLE, PLSA_UNICODE_STRING, PLSA_UNICODE_STRING);



static HMODULE                      g_RA_hDll = NULL;
static PFNNetUserAdd                g_RA_procNetUserAdd;
static PFNNetLocalGroupAddMembers   g_RA_procNetLocalGroupAddMembers;
static PFNNetApiBufferFree1         g_RA_procNetApiBufferFree1;
static PFNNetUserModalsGet          g_RA_procNetUserModalsGet;
static PFNNetUserSetInfo            g_RA_procNetUserSetInfo;
static PFNLsaAddAccountRights       g_RA_procLsaAddAccountRights;
static PFNLsaClose                  g_RA_procLsaClose;
static PFNLsaOpenPolicy             g_RA_procLsaOpenPolicy;
static PFNLsaStorePrivateData       g_RA_procLsaStorePrivateData;




FCINST_API LONG FCINST_DECL SetOPCServerRunAs(HWND hWnd, LPSTR lpszAppId, LPSTR lpszSrvName)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    USES_CONVERSION;



    g_RA_hDll = LoadLibraryA("netapi32.dll");
    if (NULL == g_RA_hDll)
    {
        ::MessageBoxA(hWnd, "Failed to create ATCMControl OPC user.", 
                      "ATCMControl Setup", MB_OK|MB_SETFOREGROUND|MB_TOPMOST);

        return -1;
    }

    g_RA_procNetUserAdd              = (PFNNetUserAdd)GetProcAddress(g_RA_hDll, "NetUserAdd");
    g_RA_procNetLocalGroupAddMembers = (PFNNetLocalGroupAddMembers)GetProcAddress(g_RA_hDll, "NetLocalGroupAddMembers");
    g_RA_procNetApiBufferFree1       = (PFNNetApiBufferFree1)GetProcAddress(g_RA_hDll, "NetApiBufferFree");
    g_RA_procNetUserModalsGet        = (PFNNetUserModalsGet)GetProcAddress(g_RA_hDll, "NetUserModalsGet");
    g_RA_procNetUserSetInfo          = (PFNNetUserSetInfo)GetProcAddress(g_RA_hDll, "NetUserSetInfo");


    if (NULL == g_RA_procNetUserAdd ||
        NULL == g_RA_procNetLocalGroupAddMembers ||
        NULL == g_RA_procNetApiBufferFree1 ||
        NULL == g_RA_procNetUserModalsGet ||
        NULL == g_RA_procNetUserSetInfo)
    {
        FreeLibrary(g_RA_hDll);
     
        ::MessageBoxA(hWnd, "Failed to create ATCMControl OPC user.", 
                      "ATCMControl Setup", MB_OK|MB_SETFOREGROUND|MB_TOPMOST);

        return -1;
    }

    HMODULE hModAdvapi32 = NULL;
    hModAdvapi32 = GetModuleHandleA("advapi32.dll");
    if (NULL == hModAdvapi32)
    {
        FreeLibrary(g_RA_hDll);

        ::MessageBoxA(hWnd, "Failed to create ATCMControl OPC user.", 
                      "ATCMControl Setup", MB_OK|MB_SETFOREGROUND|MB_TOPMOST);

        return -1;

    }

    g_RA_procLsaAddAccountRights = (PFNLsaAddAccountRights)GetProcAddress(hModAdvapi32, "LsaAddAccountRights");
    g_RA_procLsaClose            = (PFNLsaClose)GetProcAddress(hModAdvapi32, "LsaClose");
    g_RA_procLsaOpenPolicy       = (PFNLsaOpenPolicy)GetProcAddress(hModAdvapi32, "LsaOpenPolicy");
    g_RA_procLsaStorePrivateData = (PFNLsaStorePrivateData)GetProcAddress(hModAdvapi32, "LsaStorePrivateData");

    if (NULL == g_RA_procLsaAddAccountRights ||
        NULL == g_RA_procLsaClose ||
        NULL == g_RA_procLsaOpenPolicy ||
        NULL == g_RA_procLsaStorePrivateData)
    {
        FreeLibrary(g_RA_hDll);
     
        ::MessageBoxA(hWnd, "Failed to create ATCMControl OPC user.", 
                      "ATCMControl Setup", MB_OK|MB_SETFOREGROUND|MB_TOPMOST);

        return -1;
    }
    

    TCHAR lpszAdminGroup[UNLEN+1];
	DWORD cchAdminGroup = UNLEN;
	DWORD dwResult;

	if(! GetAdminGroup(lpszAdminGroup, &cchAdminGroup))
	{    
        FreeLibrary(g_RA_hDll);

        CString strCaption;
        strCaption.LoadString(IDS_MSG_BOX_TITLE);
        CString strText;
        strText.FormatMessage(IDS_ERR_SRV_ACCOUNT);
        ::MessageBox(hWnd, strText, strCaption, MB_OK|MB_SETFOREGROUND|MB_TOPMOST);
		return -1;
	}


	if ((dwResult = CreateUser(D_USER, D_PWD, D_COMMENT)) != 0)
	{
        FreeLibrary(g_RA_hDll);

        CString strCaption;
        strCaption.LoadString(IDS_MSG_BOX_TITLE);
        CString strText;
        strText.FormatMessage(IDS_ERR_SRV_ACCOUNT);
        ::MessageBox(hWnd, strText, strCaption, MB_OK|MB_SETFOREGROUND|MB_TOPMOST);
		return -1;
	}

	if ((dwResult = HideUser(D_USER)) != 0)
	{
        FreeLibrary(g_RA_hDll);

        CString strCaption;
        strCaption.LoadString(IDS_MSG_BOX_TITLE);
        CString strText;
        strText.FormatMessage(IDS_ERR_SRV_ACCOUNT);
        ::MessageBox(hWnd, strText, strCaption, MB_OK|MB_SETFOREGROUND|MB_TOPMOST);
		return -1;
	}

	if ((dwResult = AddUserToGroup(lpszAdminGroup, D_USER)) != 0)
	{
        FreeLibrary(g_RA_hDll);

        CString strCaption;
        strCaption.LoadString(IDS_MSG_BOX_TITLE);
        CString strText;
        strText.FormatMessage(IDS_ERR_SRV_ACCOUNT);
        ::MessageBox(hWnd, strText, strCaption, MB_OK|MB_SETFOREGROUND|MB_TOPMOST);
		return -1;
	}

    TCHAR szHost[256];
    GetLocalMachineName(szHost, 256);

    TCHAR szHostUser[1000];
    _stprintf(szHostUser, _T("%s\\%s"), szHost, D_USER);

	if ((dwResult = SetRunAsOption(A2T(lpszAppId), szHostUser, D_PWD)) != 0)
	{
        FreeLibrary(g_RA_hDll);

        CString strCaption;
        strCaption.LoadString(IDS_MSG_BOX_TITLE);
        CString strText;
        strText.FormatMessage(IDS_ERR_SRV_ACCOUNT);
        ::MessageBox(hWnd, strText, strCaption, MB_OK|MB_SETFOREGROUND|MB_TOPMOST);
		return -1;
	}

    FreeLibrary(g_RA_hDll);

    return 0;
}




/*------------------------------------------------------------------*/
/**
 * SetRunAsOption
 *
 */
DWORD SetRunAsOption (LPTSTR lpszAppId, LPTSTR lpszUser, LPTSTR lpszPass)
{
MB("SetRunAsOption", MB_OK);

	DWORD dwReturn;
	HKEY  hKey;
	TCHAR lpszKey[256];
			
	_stprintf (lpszKey, _T("APPID\\%s"), lpszAppId);
	
	dwReturn = RegOpenKeyEx (HKEY_CLASSES_ROOT, lpszKey, 0, KEY_ALL_ACCESS, &hKey);
	if (dwReturn != ERROR_SUCCESS)
	{
		return dwReturn;
	}
	
	dwReturn = RegSetValueEx (hKey, _T("RunAs"), 0, REG_SZ, (LPBYTE)lpszUser, _tcslen(lpszUser) * sizeof (TCHAR));
	if (dwReturn != ERROR_SUCCESS)
	{
		return dwReturn;
	}
	
	RegCloseKey (hKey);
	
	return SetRunAsPassword (lpszAppId, lpszUser, lpszPass);
}


/*------------------------------------------------------------------*/
/**
 * SetRunAsPassword
 *
 */
static DWORD SetRunAsPassword(LPTSTR lpszAppId, LPTSTR lpszUser, LPTSTR lpszPass)
{
MB("SetRunAsPassword", MB_OK);

    USES_CONVERSION;

    LSA_OBJECT_ATTRIBUTES objectAttributes;
    HANDLE                policyHandle = NULL;
    LSA_UNICODE_STRING    lsaKeyString;
    LSA_UNICODE_STRING    lsaPwdString;
    WCHAR                 lpszKey [4 + GUIDSTR_MAX + 1];
    DWORD                 dwReturn;

    wcscpy (lpszKey, L"SCM:");
    wcscat (lpszKey, T2W(lpszAppId));

    lsaKeyString.Length			= (USHORT) ((wcslen (lpszKey) + 1) * sizeof (WCHAR));
    lsaKeyString.MaximumLength	= (GUIDSTR_MAX + 5) * sizeof (WCHAR);
    lsaKeyString.Buffer			= lpszKey;

    lsaPwdString.Length			= (USHORT) ((wcslen (T2CW(lpszPass)) + 1) * sizeof (WCHAR));
    lsaPwdString.Buffer			= T2W(lpszPass);
    lsaPwdString.MaximumLength = lsaPwdString.Length;

    // Open the local security policy

    memset (&objectAttributes, 0x00, sizeof (LSA_OBJECT_ATTRIBUTES));
    objectAttributes.Length = sizeof (LSA_OBJECT_ATTRIBUTES);

    dwReturn = g_RA_procLsaOpenPolicy (NULL, &objectAttributes, POLICY_CREATE_SECRET, &policyHandle);
    if (dwReturn != ERROR_SUCCESS)
	{
        return dwReturn;
	}

    // Store the user's password

    dwReturn = g_RA_procLsaStorePrivateData (policyHandle, &lsaKeyString, &lsaPwdString);
    if (dwReturn != ERROR_SUCCESS)
    {
        g_RA_procLsaClose (policyHandle);
        return dwReturn;
    }

    g_RA_procLsaClose (policyHandle);

	return SetAccountRights (lpszUser, _T("SeBatchLogonRight"));
}


/*------------------------------------------------------------------*/
/**
 * SetAccountRights
 *
 */
static DWORD SetAccountRights (LPTSTR User, LPTSTR lpszPriv)
{
MB("SetAccountRights", MB_OK);

    USES_CONVERSION;

    LSA_HANDLE            policyHandle;
    LSA_OBJECT_ATTRIBUTES objectAttributes;
    PSID                  principalSID;
    LSA_UNICODE_STRING    lsaPrivilegeString;
    DWORD                 dwReturn;

    memset (&objectAttributes, 0, sizeof(LSA_OBJECT_ATTRIBUTES));
    dwReturn = g_RA_procLsaOpenPolicy (NULL, &objectAttributes, POLICY_CREATE_ACCOUNT | POLICY_LOOKUP_NAMES, &policyHandle);
	if (dwReturn != ERROR_SUCCESS)
    {
        return dwReturn;
    }

    GetPrincipalSID (User, &principalSID);

    lsaPrivilegeString.Length			= (USHORT) (wcslen (T2CW(lpszPriv)) * sizeof (WCHAR));
    lsaPrivilegeString.MaximumLength	= (USHORT) (lsaPrivilegeString.Length + sizeof (WCHAR));
    lsaPrivilegeString.Buffer			= T2W(lpszPriv);

    dwReturn = g_RA_procLsaAddAccountRights (policyHandle, principalSID, &lsaPrivilegeString, 1);

    free (principalSID);
    g_RA_procLsaClose (policyHandle);

    return dwReturn;
}


/*------------------------------------------------------------------*/
/**
 * GetPrincipalSID
 *
 */
static DWORD GetPrincipalSID (LPTSTR lpszUser, PSID *Sid)
{
MB("GetPrincipalSID", MB_OK);

    DWORD        sidSize;
    TCHAR        refDomain [256];
    DWORD        refDomainSize;
    DWORD        dwReturn;
    SID_NAME_USE snu;

    sidSize = 0;
    refDomainSize = 255;

    LookupAccountName (NULL, lpszUser, *Sid, &sidSize, refDomain, &refDomainSize, &snu);

    dwReturn = GetLastError();
    if (dwReturn != ERROR_INSUFFICIENT_BUFFER)
	{
        return dwReturn;
	}

    *Sid = (PSID) malloc (sidSize);
    refDomainSize = 255;

    if (! LookupAccountName (NULL, lpszUser, *Sid, &sidSize, refDomain, &refDomainSize, &snu))
    {
        return GetLastError();
    }

    return ERROR_SUCCESS;
}

/*------------------------------------------------------------------*/
/**
 * CreateUser
 *
 */
DWORD CreateUser(LPTSTR lpszUser, LPTSTR lpszPass, LPTSTR lpszComment)
{
MB("CreateUser", MB_OK);

    USES_CONVERSION;

    USER_INFO_1               user_info;
    NET_API_STATUS            dwReturn;

    user_info.usri1_name			= T2W(lpszUser);
    user_info.usri1_password		= T2W(lpszPass);
    user_info.usri1_priv			= USER_PRIV_USER;
    user_info.usri1_home_dir		= L"";
    user_info.usri1_comment			= T2W(lpszComment);
    user_info.usri1_flags			= UF_SCRIPT | UF_DONT_EXPIRE_PASSWD | UF_PASSWD_CANT_CHANGE ;
    user_info.usri1_script_path		= L"";

    USER_INFO_1008 user_info_change;
    user_info_change.usri1008_flags = UF_SCRIPT | UF_DONT_EXPIRE_PASSWD | UF_PASSWD_CANT_CHANGE ;

    dwReturn = g_RA_procNetUserAdd(NULL, 1, (LPBYTE)&user_info, NULL);

    if (dwReturn  == NERR_UserExists)
	{
   		dwReturn = 0;

        dwReturn = g_RA_procNetUserSetInfo(NULL, user_info.usri1_name, 1008, (LPBYTE)&user_info_change, NULL);      
        dwReturn = 0;
    }

	return dwReturn;
}


/*------------------------------------------------------------------*/
/**
 * HideUser
 *
 */
DWORD HideUser(LPTSTR lpszUser)
{
	DWORD dwReturn;
	HKEY  hKey;

    // VISTAFIX 14.02.08 SIS >>
    // Use RegCreateKeyEx instead of RegOpenKeyEx since key may not exist.
    DWORD   dwDisposition;
	dwReturn = RegCreateKeyEx(HKEY_LOCAL_MACHINE,
                              _T("Software\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon\\SpecialAccounts\\UserList"), 
                              0, 
                              NULL, 
                              REG_OPTION_NON_VOLATILE,
                              KEY_ALL_ACCESS, 
                              NULL, 
                              &hKey, 
                              &dwDisposition);
    // VISTAFIX 14.02.08 SIS <<

	if (dwReturn != ERROR_SUCCESS)
	{
		return 0;
	}
	
	DWORD dwVal = 0;

	dwReturn = RegSetValueEx (hKey, lpszUser, 0, REG_DWORD, (LPBYTE)&dwVal, sizeof(dwVal));
	if (dwReturn != ERROR_SUCCESS)
	{
		return 0;
	}
	
	RegCloseKey (hKey);

	return 0;
}

/*------------------------------------------------------------------*/
/**
 * AddUserToGroup
 *
 */
DWORD AddUserToGroup(LPTSTR lpszGroup, LPTSTR lpszUser)
{
MB("AddUserToGroup", MB_OK);

    USES_CONVERSION;

    LOCALGROUP_MEMBERS_INFO_3 localgroup_members;
    NET_API_STATUS            dwReturn = 0;

	localgroup_members.lgrmi3_domainandname = T2W(lpszUser);

	dwReturn = g_RA_procNetLocalGroupAddMembers(NULL, T2W(lpszGroup), 3, (LPBYTE)&localgroup_members, 1);

	if (dwReturn == ERROR_MEMBER_IN_ALIAS)
	{
		dwReturn = 0;
	}

	return dwReturn;
}

/*------------------------------------------------------------------*/
/**
 * GetAdminGroup
 *
 */
BOOL GetAdminGroup(LPTSTR lpszGroup, PDWORD cchGroup)
{
	return LookupGroupFromRid(NULL, DOMAIN_ALIAS_RID_ADMINS, lpszGroup, cchGroup);
}

/*------------------------------------------------------------------*/
/**
 * GetAdmin
 *
 */
BOOL GetAdmin(LPTSTR lpszAdmin, PDWORD cchAdmin)
{
	return LookupUserFromRid(NULL, DOMAIN_ALIAS_RID_ADMINS, lpszAdmin, cchAdmin);
}

/*------------------------------------------------------------------*/
/**
 * LookupGroupFromRid
 *
 */
static BOOL LookupGroupFromRid(LPTSTR lpszComputer, DWORD Rid, LPTSTR lpszGroup, PDWORD cchGroup)
{
MB("LookupGroupFromRid", MB_OK);

	SID_IDENTIFIER_AUTHORITY	sia = SECURITY_NT_AUTHORITY;
	SID_NAME_USE				snu;
	PSID						pSid;
	
	TCHAR	lpszDomain[DNLEN+1];
	DWORD	cchDomain			= DNLEN;
	BOOL	bSuccess			= FALSE;
	
	// Sid is the same regardless of machine, since the well-known
	// BUILTIN domain is referenced.
	
	if(AllocateAndInitializeSid (&sia, 2, SECURITY_BUILTIN_DOMAIN_RID, Rid, 0, 0, 0, 0, 0, 0, &pSid)) 
	{	
		bSuccess = LookupAccountSid(lpszComputer, pSid, lpszGroup, cchGroup, lpszDomain, &cchDomain, &snu);
		
		FreeSid(pSid);
	}
	
	return bSuccess;
}

/*------------------------------------------------------------------*/
/**
 * LookupUserFromRid
 *
 */
static BOOL LookupUserFromRid(LPTSTR lpszComputer, DWORD Rid, LPTSTR lpszUser, PDWORD cchUser)
{
MB("LookupUserFromRid", MB_OK);

    USES_CONVERSION;

	PUSER_MODALS_INFO_2 umi2;
	UCHAR				SubAuthorityCount;
	PSID				pSid;
	SID_NAME_USE		snu;
	
	TCHAR	lpszDomain[DNLEN+1];
	DWORD	cchDomain	= DNLEN;
	BOOL	bSuccess	= FALSE; // assume failure
	
	// get the account domain Sid on the target machine
	// note: if you were looking up multiple sids based on the same
	// account domain, only need to call this once.	
	
	if(g_RA_procNetUserModalsGet(T2CW(lpszComputer), 2, (LPBYTE *)&umi2) != NERR_Success) 
	{
		return FALSE;
	}
	
	SubAuthorityCount = *GetSidSubAuthorityCount(umi2->usrmod2_domain_id);
	
	// allocate storage for new Sid. account domain Sid + account Rid	
	pSid = (PSID)HeapAlloc(GetProcessHeap(), 0, GetSidLengthRequired((UCHAR)(SubAuthorityCount + 1)));
	
	if(pSid != NULL) 
	{
		if(InitializeSid(pSid, GetSidIdentifierAuthority(umi2->usrmod2_domain_id), (BYTE)(SubAuthorityCount+1)))
		{	
			DWORD SubAuthIndex = 0;
			
			// copy existing subauthorities from account domain Sid into
			// new Sid			
			for( ; SubAuthIndex < SubAuthorityCount ; SubAuthIndex++) 
			{
				*GetSidSubAuthority(pSid, SubAuthIndex) = *GetSidSubAuthority(umi2->usrmod2_domain_id, SubAuthIndex);
			}
			
			// append Rid to new Sid			
			*GetSidSubAuthority(pSid, SubAuthorityCount) = Rid;
			
			bSuccess = LookupAccountSid(lpszComputer, pSid, lpszUser, cchUser, lpszDomain, &cchDomain, &snu);
		}
		
		HeapFree(GetProcessHeap(), 0, pSid);
	}
	
	g_RA_procNetApiBufferFree1(umi2);
	
	return bSuccess;
}

