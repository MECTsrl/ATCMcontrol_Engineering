/* $Header: /4CReleased/V2.20.00/Target/TEMPLATES/AddOnHandler/AProjectData.cpp 1     28.02.07 18:54 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: AProjectData.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/Target/TEMPLATES/AddOnHandler/AProjectData.cpp $
 *
 * =PROJECT 			4CONTROL V2.x
 *
 * =SWKE				Target
 *
 * =COMPONENT			AddOnHandler
 *
 * =CURRENT 	 $Date: 28.02.07 18:54 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 */

/* ----  Includes:	 ---------------------------------------------------------- */

#include "StdAfx.h"
#include "AProjectData.h"
#include "AAddOnParameter.h"
#include "GHFile.h"
#include "utilif.h"

/* ----  Target Specific Includes:    ----------------------------------------- */

/* ----  Local Defines:   ----------------------------------------------------- */

#define DEFAULT_POU_PROT_REG_KEY  _T("SOFTWARE\\Softing\\4ControlV2\\POUPROT\\ATHW119")

/* ----  Global Variables:   -------------------------------------------------- */

/* ----  Local Functions:   --------------------------------------------------- */

/* ----  Implementations:   --------------------------------------------------- */


/* ---------------------------------------------------------------------------- */
/**
 * CAProjectData
 *
 */
CAProjectData::CAProjectData()
{

}

/* ---------------------------------------------------------------------------- */
/**
 * ~CAProjectData
 *
 */
CAProjectData::~CAProjectData()
{

}

/* ---------------------------------------------------------------------------- */
/**
 * CreateAddonParameter
 *
 */
CBaseAddonParameter* CAProjectData::CreateAddonParameter()
{
    return (CBaseAddonParameter*) new CAAddOnParameter;
}

/* ---------------------------------------------------------------------------- */
/**
 * GetMaxResourceCount
 *
 */
int CAProjectData::GetMaxResourceCount()
{
    return 5;
}

/* ---------------------------------------------------------------------------- */
/**
 * GetCommChannelDescription
 *
 */
CString CAProjectData::GetCommChannelDescription(const CString& crstrCommChannel)
{
    return CBaseProjectData::GetCommChannelDescription(crstrCommChannel);
}

/* ---------------------------------------------------------------------------- */
/**
 * GetDefaultCommChannel
 *
 */
CString CAProjectData::GetDefaultCommChannel()
{
    return CBaseProjectData::GetDefaultCommChannel();
}

/* ---------------------------------------------------------------------------- */
/**
 * CreateProjectFiles
 *
 */
HRESULT CAProjectData::CreateProjectFiles()
{
    HRESULT hr;

    hr = CBaseProjectData::CreateProjectFiles();
    if(hr != S_OK)
    {
        return hr;
    }

    hr = CreateGHFile();
    return hr;
}



/* ---------------------------------------------------------------------------- */
/**
 * CreateGHFile
 *
 */
HRESULT CAProjectData::CreateGHFile()
{
    CGHFile tGHFile;
    CString strPassword = GetDefaultPassword();
    CString strFileName;
    strFileName.Format("%s\\Project.4gh",m_strPath);

    tGHFile.SetFileName(strFileName);
    tGHFile.SetPassword(strPassword);
    tGHFile.SetProjectGuid(m_strProjectGuid);

    if(!tGHFile.Write())
    {
        return E_FAIL;
    }

    return S_OK;
}


/* ---------------------------------------------------------------------------- */
/**
 * GetDefaultPassword
 *
 */
CString CAProjectData::GetDefaultPassword()
{
    CString strPassword(_T("123456"));
    CString strEncrypted;
    long    lResult;
    HKEY    hKey;

    lResult = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, DEFAULT_POU_PROT_REG_KEY, 0, KEY_READ, &hKey);

    if (lResult != ERROR_SUCCESS)
    {
        return strPassword;
    }

    DWORD   dwBufSize = _MAX_PATH;
    DWORD   dwType;
    lResult = ::RegQueryValueEx(hKey, _T("DefPD"), NULL, &dwType, (unsigned char*)strEncrypted.GetBuffer(_MAX_PATH), &dwBufSize);
    strEncrypted.ReleaseBuffer();

    if (lResult != ERROR_SUCCESS) 
    {
        //close the key
        ::RegCloseKey(hKey);
        return strPassword;
    }

    //close the key
    ::RegCloseKey(hKey);

    // now decrypt it
    CString strDecrypted;
    long    lSize = strEncrypted.GetLength();

    HRESULT hr = UTIL_DecryptString(strEncrypted, strDecrypted.GetBuffer(lSize), NULL);
    strDecrypted.ReleaseBuffer();

    ASSERT(hr == S_OK);
    if(hr != S_OK)
    {
        return strPassword;
    }

    strPassword = strDecrypted;

    return strPassword;
}


/* ============================================================================
 * T A R G E T   S P E C I F I C
 * ============================================================================
 *
 */


/* ---------------------------------------------------------------------------- */
