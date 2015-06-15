/* $Header: /4CReleased/V2.20.00/Target/TEMPLATES/AddOnHandler/AAddOnObject.cpp 1     28.02.07 18:54 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: AAddOnObject.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/Target/TEMPLATES/AddOnHandler/AAddOnObject.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Target
 *
 * =COMPONENT			AddOnHandler
 *
 * =CURRENT 	 $Date: 28.02.07 18:54 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

/* ----  Includes:	 ---------------------------------------------------------- */

#include "StdAfx.h"

#include "AddOnHandler.h"
#include "AAddOnObject.h"
#include "ProjectWizardDialog.h"
#include "ATargetData.h"
#include "AProjectData.h"

#include "PouProtectionDlg.h"
#include "GHFile.h"
#include "EnterPasswordDlg.h"

/* ----  Target Specific Includes:    ----------------------------------------- */

/* ----  Local Defines:   ----------------------------------------------------- */

#define DISP_POUPROTECTION          _T("PouProtection")

/* ----  Global Variables:   -------------------------------------------------- */

/* ----  Local Functions:   --------------------------------------------------- */

/* ----  Implementations:   --------------------------------------------------- */


/* ---------------------------------------------------------------------------- */
/**
 * CAAddOnObject
 *
 */
CAAddOnObject::CAAddOnObject()
  : CBaseTargetObject(_T("[*PR]\\[*PR]_Addon.kad"))
{

}

/* ---------------------------------------------------------------------------- */
/**
 * ~CAAddOnObject
 *
 */
CAAddOnObject::~CAAddOnObject()
{
}

/* ---------------------------------------------------------------------------- */
/**
 * InterfaceSupportsErrorInfo
 *
 */
STDMETHODIMP CAAddOnObject::InterfaceSupportsErrorInfo(REFIID riid)
{
	if (CAAddOnObject::InterfaceSupportsErrorInfo(riid) == S_OK)
		return S_OK;

	return S_FALSE;
}

/* ---------------------------------------------------------------------------- */
/**
 * ShowProjectWizardDialog
 *
 */
HRESULT CAAddOnObject::ShowProjectWizardDialog(CBaseProjectData* pProjectData)
{
    HRESULT hr;

    CProjectWizardDialog        tDlg(pProjectData);

    if (tDlg.DoModal () != ID_WIZFINISH)
    {
        return (S_FALSE);
    }

    hr = pProjectData->CreateProjectFiles ();
    if (FAILED (hr))
    {
        delete pProjectData;
    }
    return (hr);
}

/* ---------------------------------------------------------------------------- */
/**
 * CreateTargetData
 *
 */
CBaseTargetData* CAAddOnObject::CreateTargetData(ICEProjInfo* pICEProjInfo)
{
    CBaseTargetData*    ptTargetData = new CATargetData( this, 
                                                            pICEProjInfo,
                                                            m_strKADFileName);

    ptTargetData->Init();
    
    if(!ptTargetData)
    {
        ::AfxThrowMemoryException();
    }
    return ptTargetData;

}

/* ---------------------------------------------------------------------------- */
/**
 * CreateProjectData
 *
 */
CBaseProjectData* CAAddOnObject::CreateProjectData()
{
    CBaseProjectData* pProjectData = (CBaseProjectData*) new CAProjectData;
    if(!pProjectData)
    {
        ::AfxThrowMemoryException();
    }
    return pProjectData;
}

/* ---------------------------------------------------------------------------- */
/**
 * ActionDispatcher
 *
 */
HRESULT CAAddOnObject::ActionDispatcher(CBaseTargetData* ptBaseTargetData,
                                            const CString& crstrMethodName,
                                            const CString& crstrSourceFile,
                                            const CString& crstrSourceId,
                                            const CString& crstrAddInfo,
                                            BOOL& bHandled)
{
	HRESULT	hr = S_FALSE;
	bHandled   = FALSE;

    CATargetData* pData = dynamic_cast<CATargetData*>(ptBaseTargetData);
    if(!pData)
    {
        return E_FAIL;
    }

    ICEProjInfo*    pICEProjectInfo = ptBaseTargetData->GetProjInfo();
    if(!pICEProjectInfo)
    {
        return E_FAIL;
    }

    // these methods do not need the target file to be loaded
    if (crstrMethodName.CompareNoCase(DISP_POUPROTECTION)==0)
    {
        hr = OnPouProtection(pData, crstrSourceFile, crstrSourceId, pICEProjectInfo, crstrAddInfo);
	    bHandled = TRUE;
    }

    return hr;
}


/* ---------------------------------------------------------------------------- */
/**
 * OnPouProtection
 *
 */
HRESULT CAAddOnObject::OnPouProtection(CATargetData* pTargetData, 
									   const CString& crstrFileName, 
									   const CString& crstrIdPath, 
									   ICEProjInfo* pICEProjInfo, 
									   const CString& crstrAddInfo)
{
    HRESULT hr = S_OK;
    ASSERT(pICEProjInfo);

    // first load file
    CComBSTR    sProjectPath;
    hr = pICEProjInfo->getProjectPath(&sProjectPath);
    if (hr != S_OK)
    {
        CString strMessage;
        strMessage.LoadString(IDS_INTERNAL_ERROR);
        strMessage += _T(": OnPouProtection1");
        ::AfxMessageBox(strMessage, MB_OK|MB_ICONERROR);
        return hr;
    }

    // open 4gh file
    CGHFile     tGHFile;
    CString     strGHFileName;

    strGHFileName.Format("%s\\Project.4gh", (CString)sProjectPath);

    tGHFile.SetFileName(strGHFileName);

    if(!tGHFile.Read())
    {
        ::AfxMessageBox(IDS_4GHFILE_READ_ERR);
        return E_FAIL;
    }

    // password dialog
    CString strGHPassword = tGHFile.GetPassword();
    CEnterPasswordDlg   tEnterPWDlg(strGHPassword);
    if(tEnterPWDlg.DoModal() != IDOK)
    {
        tGHFile.SetPassword(tEnterPWDlg.m_strPassword);
        if(tEnterPWDlg.m_bPasswdChanged)
        {
            tGHFile.Write();
        }
        return E_FAIL;
    }

    tGHFile.SetPassword(tEnterPWDlg.m_strPassword);
    if(tEnterPWDlg.m_bPasswdChanged)
    {
        tGHFile.Write();
    }

    // check project guid
    CComBSTR    sSection(_T("ATTRIBUTES"));
    CComBSTR    sKey(_T("GUID"));
    CComBSTR    sValue;
    pICEProjInfo->CPGetValueForKeyFromSection(sSection, sKey, &sValue);

    CString     strProjectGuid(sValue);
    // remove ""
    strProjectGuid = strProjectGuid.Mid(1);
    strProjectGuid = strProjectGuid.Left(strProjectGuid.GetLength()-1);

    if(strProjectGuid.Compare(tGHFile.GetProjectGuid()) != 0)
    {
        ::AfxMessageBox(IDS_4GHFILE_PROJCONFILCT);
        return E_FAIL;
    }

    // user is authorized to get pou protection dialog
    CPouProtectionDlg   tDlg(&tGHFile, pICEProjInfo);
    if(tDlg.DoModal() != IDOK)
    {
        return S_FALSE;
    }

    return hr;
}



/* ============================================================================
 * T A R G E T   S P E C I F I C
 * ============================================================================
 *
 */



/* ---------------------------------------------------------------------------- */
