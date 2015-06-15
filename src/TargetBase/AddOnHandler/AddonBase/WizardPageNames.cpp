/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"
#include "WizardPageNames.h"
#include "ProjectWizardResource.h"
//#include "ProjectWizardData.h"
#include "BaseAddonParameter.h"
#include "BaseProjectData.h"
#include "CommPropertySheet.h"
#include "BaseCommPropertyPage.h"
#include "utilif.h"


//----  Local Defines:   -------------------------------------------*
#define BITMAP_WIDTH 20

//----  Static Initializations:   ----------------------------------*

BEGIN_MESSAGE_MAP(CWizardPageNames, CPropertyPage)
	//{{AFX_MSG_MAP(CWizardPageNames)
	ON_BN_CLICKED(IDC_4CWIZ_COMMPROP, On4cwizCommprop)
	ON_CBN_SELCHANGE(IDC_4CWIZ_FIRMWARE_VERSION, OnSelchange4cwizFirmwareVersion)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_4CWIZ_LCONFS, OnResourceChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CWizardPageNames::CWizardPageNames(CPropertySheet *pParent, CProjectWizardData *pData)
: CProjectWizardPage(CWizardPageNames::IDD, pParent, pData)
{
	//{{AFX_DATA_INIT(CWizardPageNames)
    m_iActualResource = 0;
	m_sResource = _T("");
	m_sProgram = _T("");
    m_iLanguage = 0;
	m_sTask = _T("");
	m_sTarget = _T("");
	m_strOPCServer = _T("");
	//}}AFX_DATA_INIT
}

void CWizardPageNames::DoDataExchange(CDataExchange* pDX)
// exchange data between the window elements and the corresponding variables
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizardPageNames)
	DDX_Control(pDX, IDC_4CWIZ_COMM_CHANNEL, m_tCommChannelCombo);
	DDX_Control(pDX, IDC_4CWIZ_FIRMWARE_VERSION, m_tVersionCombo);
	DDX_Text(pDX, IDC_4CWIZ_RESOURCE, m_sResource);
	DDX_Text(pDX, IDC_4CWIZ_PROGRAM, m_sProgram);
	DDX_CBIndex(pDX, IDC_4CWIZ_LANGUAGE, m_iLanguage);
	DDX_Text(pDX, IDC_4CWIZ_TASK, m_sTask);
	DDX_Text(pDX, IDC_4CWIZ_TARGET, m_sTarget);
	DDX_Text(pDX, IDC_4CWIZ_ADDRESS, m_strOPCServer);
	DDX_Control(pDX, IDC_4CWIZ_LCONFS, m_resources);
	//}}AFX_DATA_MAP
}

void CWizardPageNames::OnResourceChanged(NMHDR* pNMHDR, LRESULT* pResult) 
// this function saves the data of the actual resource and displays the
// data of the new chosen resource
{
	NM_LISTVIEW *pListView;
    
    pListView = (NM_LISTVIEW*) pNMHDR;
	if(pListView->uNewState == (LVIS_SELECTED|LVIS_FOCUSED)) 
    {
        if(m_iActualResource != pListView->iItem) 
        {
            WriteResourceData();    // save old data
            m_iActualResource = pListView->iItem;
            FillCommChannelCombo();
            ReadResourceData();     // load new data
        }
	}
	*pResult = 0;

}

//------------------------------------------------------------------*
/**
 * Writes the resource specific data
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CWizardPageNames::ReadResourceData()
{
    CProjectWizardResource *pResource;
    CProjectWizardTask *pTask;
    CString             strCommChannel;
    CString             strFirmwareVersion;
    UINT                uiAnzFirmware;
    UINT                uiFirmwareVersionIndex;
    CBaseProjectData*       pData = (CBaseProjectData*)m_pData;
    CBaseAddonParameter*    pAddonPar = pData->m_pAddonPar;
    CString             strCommChannelDesc;

    pResource = pData->GetResource(m_iActualResource);

    m_sResource = pResource->GetResourceName();
    m_sTarget = pResource->GetTarget();

    m_strOPCServer = pResource->GetOPCServer();
    strCommChannel = pResource->GetCommChannel();

    uiFirmwareVersionIndex = pResource->GetFirmwareVersionIndex();

    uiAnzFirmware = pAddonPar->GetFirmwareCount();

    ASSERT(uiFirmwareVersionIndex < uiAnzFirmware);
    if(uiFirmwareVersionIndex < uiAnzFirmware)
    {
        CFirmwareData* pFirmwareData = pAddonPar->GetFirmwareData(uiFirmwareVersionIndex);
        CCommChannelData tCommChannelData;
        if(pFirmwareData->GetCommChannelDataFromId(strCommChannel, tCommChannelData))
        {
            strCommChannelDesc = pAddonPar->GetCommChannelDescription(strCommChannel);
        }
    }

    int iSel = m_tCommChannelCombo.FindStringExact(0, strCommChannelDesc);
    if(iSel >= 0)
    {
        m_tCommChannelCombo.SetCurSel(iSel);
    }

    m_tVersionCombo.SetCurSel(uiFirmwareVersionIndex);
   
    pTask = pResource->GetTask(0);

    m_sProgram = pTask->GetProgram();
    m_sTask = pTask->GetName();
    m_iLanguage = pTask->GetLanguage();

    UpdateData(FALSE);
}


//------------------------------------------------------------------*
/**
 * Reads the resource specific data
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CWizardPageNames::WriteResourceData()
{
    CProjectWizardResource *pResource;
    CProjectWizardTask *pTask;
    CString strControlAddress;
    CString strFirmwareVersion;
    CBaseProjectData*       pData = (CBaseProjectData*)m_pData;
    CBaseAddonParameter*    pAddonPar = pData->m_pAddonPar;

    UpdateData(TRUE);

    UpdateControlData();

    pResource = m_pData->GetResource(m_iActualResource);

    pResource->SetResourceName(m_sResource);
    pResource->SetTarget(m_sTarget);
    pResource->SetOPCServer(m_strOPCServer);

    SetControlString();

    pTask = pResource->GetTask(0);

    pTask->SetProgram(m_sProgram);
    pTask->SetName(m_sTask);
    pTask->SetLanguage((TIecLanguage)m_iLanguage); 
}

//------------------------------------------------------------------*
/**
 * set control string.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CWizardPageNames::SetControlString()
{
    // build control string and save it in pResource
    CProjectWizardResource* pResource;
    CString                 strCommChannel;
    UINT                    uiFirmwareVersionIndex;
    CString                 strControlString;
    CBaseProjectData*       pData = (CBaseProjectData*)m_pData;
    CBaseAddonParameter*    pAddonPar = pData->m_pAddonPar;
    CString                 strConnectPar;

    pResource = m_pData->GetResource(m_iActualResource);
    strCommChannel = pResource->GetCommChannel();
    strConnectPar = pResource->GetConnectPar();
    uiFirmwareVersionIndex = pResource->GetFirmwareVersionIndex();

    strControlString = pAddonPar->BuildControlString(m_strOPCServer, strCommChannel, strConnectPar, uiFirmwareVersionIndex);
    pResource->SetControlString(strControlString);
}


void CWizardPageNames::Init()
// initializes the wizard page / its resource list control
{
    CRect rRect;
    CDC *pDC;
    LV_COLUMN column;

    m_resources.GetClientRect(rRect);
    pDC = m_resources.GetDC();
    pDC->LPtoDP(rRect); 
    m_resources.ReleaseDC(pDC);
    column.mask = LVCF_FMT|LVCF_WIDTH;
    column.fmt = LVCFMT_LEFT;
    column.cx = rRect.Width();
    m_resources.InsertColumn(0,&column);
    m_imageList.Create(IDB_WIZARD_IMAGE_LIST,BITMAP_WIDTH,0,RGB(255,0,255));
	m_imageList.SetBkColor(GetSysColor(COLOR_WINDOW));
	m_resources.SetImageList(&m_imageList,LVSIL_SMALL);
}

BOOL CWizardPageNames::OnSetActive() 
// updates the page after it became active
{
    BOOL bResult;
    CString sTmp;
    int iResource;
    int iResourceCount;

    bResult = CProjectWizardPage::OnSetActive();

    m_resources.DeleteAllItems();
    iResourceCount = m_pData->GetResourceCount();
    if(m_iActualResource >= iResourceCount)
        m_iActualResource = 0;
    for(iResource = 0; iResource < iResourceCount; ++iResource)
    {
        CString str;

        str.Format(IDS_4CWIZ_NTH_RESOURCE, iResource + 1);
		m_resources.InsertItem(LVIF_TEXT | LVIF_STATE | LVIF_IMAGE, iResource, str, 0, LVIS_FOCUSED | LVIS_SELECTED, 0, 0L);
	};
	if(m_resources.GetItemCount() > 0)
        m_resources.SetItemState(m_iActualResource, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);

    ReadResourceData();
    m_pParent->SetWizardButtons(PSWIZB_BACK|PSWIZB_NEXT);   
    return bResult;
}


LRESULT CWizardPageNames::OnWizardNext()
// stores the data entered by the user before leaving the actual wizard page
// performs a series of checks on the data
{
    WriteResourceData();

    if(!m_pData->CheckResources())
        return(-1);

    return(CPropertyPage::OnWizardNext());
}


LRESULT CWizardPageNames::OnWizardBack()
// stores the data entered by the user before leaving the actual wizard page
{
    WriteResourceData();

	return(CPropertyPage::OnWizardBack());
}

BOOL CWizardPageNames::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

    FillFirmwareVersionCombo();

    FillCommChannelCombo();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWizardPageNames::On4cwizCommprop() 
{
    UpdateData(TRUE);

    UpdateControlData();

    ShowCommunicationProperties();
}

void CWizardPageNames::ShowCommunicationProperties()
{
    // show communication property sheet
    CBaseProjectData*       pData = (CBaseProjectData*) m_pData;
    CBaseAddonParameter*    pAddonPar = pData->m_pAddonPar;

    ASSERT(pData && pAddonPar);
    if(!pData || !pAddonPar)
    {
        return;
    }

    // get current and all communication channels for firmware version
    CStringArray            astrCommChannels;
    CProjectWizardResource* pResource;
    UINT                    uiFirmwareVersionIndex;
    CString                 strCommChannel;

    pResource = pData->GetResource(m_iActualResource);
    uiFirmwareVersionIndex = pResource->GetFirmwareVersionIndex();
    strCommChannel = pResource->GetCommChannel();

    CFirmwareData*  pFirmwareData = pAddonPar->GetFirmwareData(uiFirmwareVersionIndex);
    ASSERT(pFirmwareData);
    if(pFirmwareData)
    {
        pFirmwareData->GetCommChannels(astrCommChannels);
    }

    // create property sheet and add page for each comm channel
    CCommPropertySheet  tCommPropSheet;

    int iNumCommChannels = astrCommChannels.GetSize();
    for(int iCommChannel = 0; iCommChannel < iNumCommChannels; ++iCommChannel)
    {
        tCommPropSheet.AddPageForChannel(astrCommChannels[iCommChannel]);
    }
    tCommPropSheet.SetActiveCommChannel(strCommChannel);

    int                     iNummCommChannels = astrCommChannels.GetSize();
    CBaseCommPropertyPage*  pCommPage;
    CString                 strConnectPar;
    for(iCommChannel = 0; iCommChannel < iNumCommChannels; ++iCommChannel)
    {
        pCommPage = tCommPropSheet.GetPageForChannel(astrCommChannels[iCommChannel]);
        if(pCommPage)
        {
            strConnectPar = pResource->GetConnectPar(pCommPage->GetCommChannel());
            if(strConnectPar.IsEmpty() && pFirmwareData)
            {
                CCommChannelData    tCommChannelData;
                if(pFirmwareData->GetCommChannelDataFromId(pCommPage->GetCommChannel(), tCommChannelData))
                {
                    strConnectPar = tCommChannelData.m_strStdConnPar;
                }
            }
            pCommPage->SetConnectParameter(strConnectPar);
        }
    }

    if(tCommPropSheet.DoModal() == IDOK)
    {
        for(iCommChannel = 0; iCommChannel < iNumCommChannels; ++iCommChannel)
        {
            pCommPage = tCommPropSheet.GetPageForChannel(astrCommChannels[iCommChannel]);
            if(pCommPage)
            {
                pResource->SetConnectPar(pCommPage->GetConnectParameter(), pCommPage->GetCommChannel());
            }
        }
    }
}


//------------------------------------------------------------------*
/**
 * fill firmware version combo box.
 *
 * @param           -
 * @return          successful?
 * @exception       -
 * @see             OnInitDialog()
*/
BOOL CWizardPageNames::FillFirmwareVersionCombo()
{
    if(!m_pData)
    {
        return FALSE;
    }

    CBaseProjectData*       pData = (CBaseProjectData*)m_pData;
    CBaseAddonParameter*    pAddonPar = pData->m_pAddonPar;
    CString                 strVersion;
    CFirmwareData*          pFirmwareData;
    int                     iFirmwareCount = pAddonPar->GetFirmwareCount();
    UINT                    uiMaxFirmwareVersion = 0;
    UINT                    uiMaxFirmwareVersionIdx = 0;

    ASSERT(iFirmwareCount > 0);
    
    // run over firmware array and add firmware version description
    for(int iFirmware = 0; iFirmware < iFirmwareCount; ++iFirmware)
    {
        pFirmwareData = pAddonPar->GetFirmwareData(iFirmware);
        m_tVersionCombo.AddString(pFirmwareData->m_strVersionDescription);
        if(pFirmwareData->m_uiVersion > uiMaxFirmwareVersion)
        {
            uiMaxFirmwareVersion = pFirmwareData->m_uiVersion;
            uiMaxFirmwareVersionIdx = iFirmware;
        }
    }

    if(iFirmwareCount > 0)
    {
        m_tVersionCombo.SetCurSel(uiMaxFirmwareVersionIdx);
    }
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * fill comm channel combo.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CWizardPageNames::FillCommChannelCombo()
{

    // get comm channel flags
    CBaseProjectData*       pData = (CBaseProjectData*) m_pData;
    CBaseAddonParameter*    pAddonPar = pData->m_pAddonPar;
    UINT                    uiAnzFirmware;
    UINT                    uiFirmwareVersionIndex;
    int                     iCurrentItem(0);
    CString                 strTmp;
    CProjectWizardResource* pResource;
    CString                 strCommChannel;     // communication channel saved in pResource
    CString                 strCommChannelDesc; // communication channel description used in combo box
    int                     iSel(0);

    pResource = pData->GetResource(m_iActualResource);
    strCommChannel = pResource->GetCommChannel();

    ASSERT(pData && pAddonPar);
    if(!pData || !pAddonPar)
    {
        return;
    }

    m_tCommChannelCombo.ResetContent();

    uiAnzFirmware = pAddonPar->GetFirmwareCount();

    uiFirmwareVersionIndex = pResource->GetFirmwareVersionIndex();

    ASSERT(uiFirmwareVersionIndex < uiAnzFirmware);
    if(uiFirmwareVersionIndex < uiAnzFirmware)
    {
        CFirmwareData* pFirmwareData = pAddonPar->GetFirmwareData(uiFirmwareVersionIndex);

        CCommChannelData    tCommChannelData;
        // run over comm channel list of current firmware data
        int iNumCommChannels = pFirmwareData->m_tCommChannelArr.GetSize();
        for(int iCommChannel = 0; iCommChannel < iNumCommChannels; ++iCommChannel)
        {
            tCommChannelData = pFirmwareData->m_tCommChannelArr[iCommChannel];
            strTmp = pData->GetCommChannelDescription(tCommChannelData.m_strId);
            m_tCommChannelCombo.AddString(strTmp);
        }
        iSel = m_tCommChannelCombo.FindStringExact(0, strCommChannel);
        if(iSel > 0)
        {
            m_tCommChannelCombo.SetCurSel(iSel);
        }
        else if(m_tCommChannelCombo.GetCount() > 0)
        {
            m_tCommChannelCombo.SetCurSel(0);
        }
    }
}
	


//------------------------------------------------------------------*
/**
 * update data from dialog controls.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CWizardPageNames::UpdateControlData()
{
    CProjectWizardResource* pResource;
    CString         strCommChannelDesc;
    CString         strCommChannel;
    int             iFirmwareIndex = m_tVersionCombo.GetCurSel();
    CBaseProjectData*       pData = (CBaseProjectData*)m_pData;
    ASSERT(pData);
    CBaseAddonParameter*    pAddonPar = pData->m_pAddonPar;
    ASSERT(pAddonPar);

    m_tCommChannelCombo.GetWindowText(strCommChannelDesc);
    strCommChannel = pAddonPar->GetCommChannelFromDescription(strCommChannelDesc);

    pResource = m_pData->GetResource(m_iActualResource);
    pResource->SetCommChannel(strCommChannel);
    pResource->SetFirmwareVersionIndex(iFirmwareIndex);
}

void CWizardPageNames::OnSelchange4cwizFirmwareVersion() 
{
    UpdateControlData();
    FillCommChannelCombo();
}

/* ---------------------------------------------------------------------------- */
