/* $Header: /4CReleased/V2.20.00/TargetBase/PROJECTWIZARDBASE/ProjectWizardPropsPage.cpp 1     28.02.07 19:06 Ln $
 * ------------------------------------------------------------------------------
 *
 * =FILENAME			$Workfile: ProjectWizardPropsPage.cpp $
 *						 $Logfile: /4CReleased/V2.20.00/TargetBase/PROJECTWIZARDBASE/ProjectWizardPropsPage.cpp $
 *
 * =PROJECT 			ATCMControl V2.x
 *
 * =SWKE				Targetbase
 *
 * =COMPONENT			ProjectWizardBase
 *
 * =CURRENT 	 $Date: 28.02.07 19:06 $
 *			 $Revision: 1 $
 *
 * ------------------------------------------------------------------------------
 *
 
 

 
 *	All Rights Reserved.
 *
 * ------------------------------------------------------------------------------
 */

/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"
#include "ProjectWizardPropsPage.h"
#include "ProjectWizardData.h"
#include "ProjectWizardRes.h"

#define WIZ_URL_FILEHEADER  _T("file://")


BEGIN_MESSAGE_MAP(CProjectWizardPropertiesPage, CPropertyPage)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowseUrl)
END_MESSAGE_MAP()


CProjectWizardPropertiesPage::CProjectWizardPropertiesPage (CPropertySheet *pParent, CProjectWizardData *pData, UINT uiCaption)
: CProjectWizardPage (CProjectWizardPropertiesPage::IDD, pParent, pData, uiCaption)
{
}


//------------------------------------------------------------------*
/**
 * DDX
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CProjectWizardPropertiesPage::DoDataExchange (CDataExchange *pDX)
{
    CPropertyPage::DoDataExchange (pDX);

    DDX_Text(pDX, IDC_ALIAS, m_strAlias);
    DDX_Text(pDX, IDC_USER_TEXT1, m_strUserText1);
    DDX_Text(pDX, IDC_USER_TEXT2, m_strUserText2);
    DDX_Text(pDX, IDC_USER_TEXT3, m_strUserText3);
    DDX_Text(pDX, IDC_URL, m_strURL);

    if (pDX->m_bSaveAndValidate)
        m_pData->SetProperties (m_strAlias, m_strUserText1, m_strUserText2, m_strUserText3, m_strURL);
}


//------------------------------------------------------------------*
/**
 * Finish button handler
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
BOOL CProjectWizardPropertiesPage::OnWizardFinish () 
{
    UpdateData (TRUE);

	return (CProjectWizardPage::OnWizardFinish ());
}


//------------------------------------------------------------------*
/**
 * Called when the page becomes active. Enables back and finish buttons
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
BOOL CProjectWizardPropertiesPage::OnSetActive ()
{
    m_pParent->SetWizardButtons (PSWIZB_BACK | PSWIZB_FINISH);   

	return (CPropertyPage::OnSetActive ());
}


//------------------------------------------------------------------*
/**
 * Browse button handler. Shows a file browse dialog
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CProjectWizardPropertiesPage::OnBrowseUrl() 
{
    CString strFilter;
    CString strTitle;
    CString strURL;

    strFilter.LoadString (IDS_URL_FILTER);
    strTitle.LoadString (IDS_URL_FILE);

    CFileDialog dialog (TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, strFilter, this);

    dialog.m_ofn.lpstrTitle = strTitle;
    dialog.m_ofn.lpstrInitialDir = m_pData->GetPath ();

    if (dialog.DoModal () != IDOK)
        return;

    strURL = dialog.GetPathName ();
    strURL = WIZ_URL_FILEHEADER + strURL;

    SetDlgItemText (IDC_URL, strURL);
}

/* ---------------------------------------------------------------------------- */
