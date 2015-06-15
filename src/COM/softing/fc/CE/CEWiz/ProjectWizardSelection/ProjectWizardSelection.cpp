
#include "stdafx.h"
#include "resource.h"
#include "NewProjectDlg.h"
#include "ProjectWizardSelection.h"


DLLFUNC HRESULT ProjectWizardSelection(BSTR &bstrFileName)
{
	AFX_MANAGE_STATE (AfxGetStaticModuleState ());
    int iCode;
    CNewProjectDlg dialog (AfxGetMainWnd ());

    bstrFileName = NULL;

    iCode = dialog.DoModal ();
    if (iCode == -1)
        return (E_UNEXPECTED);


    // User cancelled selection dialog.
    //---------------------------------

    if (iCode != IDOK)
        return (S_FALSE);


    // Abort if project wizard has been cancelled
    // or failed to create the project.
    //-------------------------------------------

    if (dialog.m_hrDialogState != S_OK)
        return (dialog.m_hrDialogState);


    // Return project file.
    //---------------------

    bstrFileName = dialog.m_strFileName.AllocSysString ();

    return (S_OK);
}

