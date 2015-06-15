#include "stdafx.h"
#include "StandardProjectWizard.h"
#include "ProjectWizardComObj.h"
#include "ProjectWizardDialog.h"
#include "ProjectWizardData.h"


//------------------------------------------------------------------*
/**
 * Executes project wizard
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
HRESULT CProjectWizardComObj::ExecuteProjectWizard (BSTR sPath, BSTR sProjectName, BSTR *psFileName)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    HRESULT hr;
    CProjectWizardData data;
    CProjectWizardDialog dlg (&data);

    if (psFileName == NULL)
        return (E_POINTER);

    *psFileName = NULL;
    hr = data.Init (CString (sPath), CString (sProjectName));
    if (FAILED (hr))
        return (hr);

    if (dlg.DoModal () != ID_WIZFINISH)
        return (S_FALSE);

    hr = data.CreateProjectFiles ();
    if (FAILED (hr))
        return (hr);

    return (S_OK);
}


//------------------------------------------------------------------*
/**
 * Gets name and description of wizard
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
HRESULT CProjectWizardComObj::GetDescription (LONG iLanguageId, BSTR *psName, BSTR *psDescription)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
    CString strName;
    CString strDescription;

    if (psName == NULL || psDescription == NULL)
        return (E_POINTER);

    if (*psName)
    {
        ::SysFreeString(*psName);
    }

    strName.LoadString (IDS_WIZARD_NAME);
    *psName = strName.AllocSysString ();

    strDescription.LoadString (IDS_WIZARD_DESCRIPTION);
    *psDescription = strDescription.AllocSysString ();

	return (S_OK);
}