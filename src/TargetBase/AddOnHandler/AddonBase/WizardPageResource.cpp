/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"
#include "WizardPageResource.h"
#include "ProjectWizardData.h"
#include "utilif.h"
#include "BaseProjectData.h"
#include "BaseAddonParameter.h"
#include "basedefs.h"

//----  Local Defines:   -------------------------------------------*
#define NUMBER_RESOURCES_FORMAT_STRING _T("%d Resource(s)")

//----  Static Initializations:   ----------------------------------*




BEGIN_MESSAGE_MAP(CWizardPageResource, CPropertyPage)
END_MESSAGE_MAP()


CWizardPageResource::CWizardPageResource(CPropertySheet *pParent, CProjectWizardData *pData)
: CProjectWizardPage(CWizardPageResource::IDD, pParent, pData)
{
}


void CWizardPageResource::Init()
// initializes the wizard page
{
    int iResourceNumber;
    CComboBox *pCombo;
    CString strNumberResources;
    int iMaxResources = ((CBaseProjectData*)m_pData)->GetMaxResourceCount();

    pCombo = (CComboBox *)GetDlgItem(IDC_4CWIZ_NUMBER_RESOURCES);

    for(iResourceNumber = 0; iResourceNumber < iMaxResources; ++iResourceNumber)
    {
        strNumberResources.Format(NUMBER_RESOURCES_FORMAT_STRING, iResourceNumber + 1);
        pCombo->AddString(strNumberResources);
    };

    PopulateTables(1);

    pCombo->SetCurSel(0);
}


void CWizardPageResource::PopulateTables(int iCount)
{
    int i;
    CString strFormatResource;
    CString strFormatTarget;
    CString strFormatTask;
    CString strFormatProgram;

    CBaseProjectData*   pData = (CBaseProjectData*) m_pData;
    ASSERT(pData);
    if(!pData)
    {
        return;
    }
    CBaseAddonParameter*    pAddonPar = pData->m_pAddonPar;
    ASSERT(pAddonPar);
    if(!pAddonPar)
    {
        return;
    }

    CProjectWizardResource *pResource;
    CString str;
    CProjectWizardTask *pTask;
    CString strFirmwareVersion;
    CString strCommChannel;
    int     iFirmwareVersionIndex;

    pAddonPar->GetMaxFirmwareVersion(iFirmwareVersionIndex);

    CString strLocalMachine(_T("localhost"));
//    UTIL_GetLocalMachineName(strLocalMachine.GetBuffer(100), 100);
//    strLocalMachine.ReleaseBuffer();

    pData->SetResourceCount(iCount);

    //if(iCount < 10)
    //{
        strFormatResource.LoadString(IDS_STD_RESOURCE);
        strFormatTarget.Format("%s%%d", ((CBaseProjectData*)m_pData)->m_pAddonPar->m_strFilePrefix); //GetTargetType());
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

    for(i = 0; i < iCount; ++i)
    {

        pResource = pData->GetResource(i);
        ASSERT(pResource != NULL);
        if(pResource == NULL)
        {
            continue;
        }

        str.Format(strFormatResource, i + 1);
        pResource->SetResourceName(str);

        str.Format(strFormatTarget, i + 1);
        pResource->SetTarget(str);

        strCommChannel = pData->GetDefaultCommChannel();

        pResource->SetOPCServer(_T("localhost"));
        pResource->SetCommChannel(strCommChannel);
        pResource->SetFirmwareVersionIndex(iFirmwareVersionIndex);

        CFirmwareData*  pFirmwareData = pAddonPar->GetFirmwareData(iFirmwareVersionIndex);
        if(pFirmwareData)
        {
            CCommChannelData    tCommChannelData;
            pFirmwareData->GetCommChannelDataFromId(strCommChannel, tCommChannelData);
            pResource->SetConnectPar(tCommChannelData.m_strStdConnPar, strCommChannel);
        }

        CString strConnectPar = pResource->GetConnectPar();
        CString strControlString;

        strControlString = pAddonPar->BuildControlString(strLocalMachine, strCommChannel, strConnectPar, iFirmwareVersionIndex);
        pResource->SetControlString(strControlString);


        pResource->SetTaskCount(1);

        pTask = pResource->GetTask(0);
        ASSERT(pTask != NULL);
        if(pTask == NULL)
            continue;

        str.Format(strFormatTask, i + 1);
        pTask->SetName(str);

        str.Format(strFormatProgram, i + 1);
        pTask->SetProgram(str);
    };
}


BOOL CWizardPageResource::OnSetActive() 
// displays the actual data in the wizard page when the page gets active
{
    CComboBox *pCombo;

    pCombo = (CComboBox *) GetDlgItem(IDC_4CWIZ_NUMBER_RESOURCES);	
    pCombo->SetCurSel(m_pData->GetResourceCount() - 1);
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
//    if(pCancelWnd != NULL) {
//		m_pParent->PostMessage(WM_COMMAND,MAKEWPARAM(IDCANCEL,BN_CLICKED),
//			(LPARAM) pCancelWnd->GetSafeHwnd());
//    }
//	return -1;
//}

void CWizardPageResource::DoDataExchange(CDataExchange* pDX)
// exchange data between the window elements and the corresponding variables
{
	CComboBox *pCombo;

	pCombo = (CComboBox *) GetDlgItem(IDC_4CWIZ_NUMBER_RESOURCES);	
	if(pDX->m_bSaveAndValidate) 
    {
		PopulateTables(pCombo->GetCurSel() + 1);
	}
	CPropertyPage::DoDataExchange(pDX);
}

