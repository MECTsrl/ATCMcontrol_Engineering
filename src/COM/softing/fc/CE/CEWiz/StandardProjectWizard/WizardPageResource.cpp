#include "stdafx.h"
#include "WizardPageResource.h"
#include "ProjectWizardData.h"
#include "utilif.h"


//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



BEGIN_MESSAGE_MAP(CWizardPageResource, CPropertyPage)
END_MESSAGE_MAP()


CWizardPageResource::CWizardPageResource (CPropertySheet *pParent, CProjectWizardData *pData)
: CProjectWizardPage (CWizardPageResource::IDD, pParent, pData)
{
}


void CWizardPageResource::Init()
// initializes the wizard page
{
    int iResourceNumber;
    CComboBox *pCombo;
    CString strNumberResources;

    pCombo = (CComboBox *)GetDlgItem (IDC_4CWIZ_NUMBER_RESOURCES);

    for (iResourceNumber = 0; iResourceNumber < NUMBER_RESOURCES_MAX; ++iResourceNumber)
    {
        strNumberResources.Format (NUMBER_RESOURCES_FORMAT_STRING, iResourceNumber + 1);
        pCombo->AddString (strNumberResources);
    };

    PopulateTables (1);

    pCombo->SetCurSel (0);
}


void CWizardPageResource::PopulateTables (int iCount)
{
    int i;
    CString strFormatResource;
    CString strFormatTarget;
    CString strFormatTask;
    CString strFormatProgram;

    // Set resource count. Setting the count to zero
    // first removes all existing resources.
    //----------------------------------------------

    m_pData->SetResourceCount (0);
    m_pData->SetResourceCount (iCount);

    //if(iCount < 10)
    //{
        strFormatResource.LoadString(IDS_STD_RESOURCE);
        strFormatTarget.LoadString(IDS_STD_TARGET);
        strFormatTask.LoadString(IDS_STD_TASK);
        strFormatProgram.LoadString(IDS_STD_PROGRAM);
    //}
    //else
    //{
    //OH: this was used to count like this: 01 02 03 ...
    //    strFormatResource.LoadString(IDS_STD_RESOURCE2);
    //    strFormatTarget.LoadString(IDS_STD_TARGET2);
    //    strFormatTask.LoadString(IDS_STD_TASK2);
    //    strFormatProgram.LoadString(IDS_STD_PROGRAM2);
    //}

    for (i = 0; i < iCount; ++i)
    {
        CProjectWizardResource *pResource;
        CString str;
        CProjectWizardTask *pTask;

        pResource = m_pData->GetResource (i);
        ASSERT (pResource != NULL);
        if (pResource == NULL)
            continue;

        str.Format (strFormatResource, i + 1);
        pResource->SetResource (str);

        str.Format (strFormatTarget, i + 1);
        pResource->SetTarget (str);

        UTIL_GetLocalMachineName (str.GetBuffer (100), 100);
        str.ReleaseBuffer ();
        pResource->SetAddress (str);

        pResource->SetTaskCount (1);

        pTask = pResource->GetTask (0);
        ASSERT (pTask != NULL);
        if (pTask == NULL)
            continue;

        str.Format (strFormatTask, i + 1);
        pTask->SetName (str);

        str.Format (strFormatProgram, i + 1);
        pTask->SetProgram (str);
    };
}


BOOL CWizardPageResource::OnSetActive() 
// displays the actual data in the wizard page when the page gets active
{
    CComboBox *pCombo;

    pCombo = (CComboBox *) GetDlgItem(IDC_4CWIZ_NUMBER_RESOURCES);	
    pCombo->SetCurSel (m_pData->GetResourceCount () - 1);
    //OH: was m_pParent->SetWizardButtons(PSWIZB_BACK|PSWIZB_NEXT);   
    m_pParent->SetWizardButtons(PSWIZB_NEXT);   

    return CProjectWizardPage::OnSetActive();
}

//LRESULT CWizardPageResource::OnWizardBack() 
//// sends a cancel message when the back button of the first dialog is pressed
//{
//	CWnd *pCancelWnd;
//
//	pCancelWnd = m_pParent->GetDlgItem(IDCANCEL);
//    if (pCancelWnd != NULL) {
//		m_pParent->PostMessage(WM_COMMAND,MAKEWPARAM(IDCANCEL,BN_CLICKED),
//			(LPARAM) pCancelWnd->GetSafeHwnd());
//    }
//	return -1;
//}

void CWizardPageResource::DoDataExchange(CDataExchange* pDX)
// exchange data between the window elements and the corresponding variables
{
	CComboBox *pCombo;
	TSubSystems subSystems;

	pCombo = (CComboBox *) GetDlgItem(IDC_4CWIZ_NUMBER_RESOURCES);	
	if(pDX->m_bSaveAndValidate) {
		DDX_Check(pDX,IDC_4CWIZ_CHKPROFI,m_bProfibus);
		DDX_Check(pDX,IDC_4CWIZ_CHKSCOM,m_bSerialCommunication);

		PopulateTables (pCombo->GetCurSel () + 1);

		subSystems = SUBSYSTEM_ANY;
		if(m_bProfibus) {
			subSystems |= SUBSYSTEM_PROFIBUS;
		}
        if(m_bSerialCommunication) {
            subSystems |= SUBSYSTEM_SERIAL_COMMUNICATION;
        }
		m_pData->SetSubSystems(subSystems);
	} else {
		subSystems = m_pData->GetSubSystems();
		m_bProfibus = NORMBOOL(subSystems&SUBSYSTEM_PROFIBUS);
		m_bSerialCommunication = NORMBOOL(subSystems&SUBSYSTEM_SERIAL_COMMUNICATION);
		DDX_Check(pDX,IDC_4CWIZ_CHKPROFI,m_bProfibus);
		DDX_Check(pDX,IDC_4CWIZ_CHKSCOM,m_bSerialCommunication);
	}
	CPropertyPage::DoDataExchange(pDX);
}