
/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"
#include "AddonBaseResource.h"

#include "PropPageTargetGeneral.h"
#include "BaseAddonParameter.h"
#include "TargetPropertySheet.h"

#include "utilif.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropPageTargetGeneral dialog


CPropPageTargetGeneral::CPropPageTargetGeneral(CWnd* pParent /*=NULL*/)
	: CBasePropertyPage(CPropPageTargetGeneral::IDD)
{
	//{{AFX_DATA_INIT(CPropPageTargetGeneral)
	m_strLastModDate = _T("");
	m_strCreateUser = _T("");
	m_strLastModUser = _T("");
	m_strName = _T("");
	m_strOPCServer = _T("");
	m_strCreateDate = _T("");
	//}}AFX_DATA_INIT
    m_bNameChanged = FALSE;
    m_uiFirmwareIndex = 0;
}

void CPropPageTargetGeneral::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropPageTargetGeneral)
	DDX_Control(pDX, IDC_PROPGEN_FIRMWARE, m_tFirmwareCombo);
	DDX_Control(pDX, IDC_PROPGEN_COMM_CHANNEL_CB, m_tCommChannelCombo);
	DDX_Control(pDX, IDC_PROPGEN_RESOURCES, m_tResourceCombo);
	DDX_Text(pDX, IDC_PROPGEN_LASTMODDATE, m_strLastModDate);
	DDX_Text(pDX, IDC_PROPGEN_CREATEUSER, m_strCreateUser);
	DDX_Text(pDX, IDC_PROPGEN_LASTMODUSER, m_strLastModUser);
	DDX_Text(pDX, IDC_PROPGEN_NAME, m_strName);
	DDX_Text(pDX, IDC_PROPGEN_OPCSERVER, m_strOPCServer);
	DDX_Text(pDX, IDC_PROPGEN_CREATEDATE, m_strCreateDate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropPageTargetGeneral, CPropertyPage)
	//{{AFX_MSG_MAP(CPropPageTargetGeneral)
	ON_CBN_SELCHANGE(IDC_PROPGEN_COMM_CHANNEL_CB, OnSelchangePropgenCommChannelCb)
	ON_CBN_SELCHANGE(IDC_PROPGEN_FIRMWARE, OnSelchangePropgenFirmware)
	ON_EN_CHANGE(IDC_PROPGEN_NAME, OnChangePropgenName)
	ON_CBN_SELCHANGE(IDC_PROPGEN_RESOURCES, OnSelchangePropgenResources)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//------------------------------------------------------------------*
/**
 * initialization.
 *
 * @param           pProjInfo: project info interface pointer
 * @param           pAddonPar: addon parameter for static target data
 * @param           crastrResources: resource names for resource combo
 * @param           bNewDlg: new target dialog?
 * @return          -
 * @exception       -
 * @see             -
*/
void CPropPageTargetGeneral::Init(ICEProjInfo* pProjInfo, CBaseAddonParameter* pAddonPar, const CStringArray& crastrResources, BOOL bNewDlg)
{
    ASSERT(pProjInfo);
    m_pProjInfo = pProjInfo;
    ASSERT(pAddonPar);
    m_pAddonPar = pAddonPar;

    m_astrResources.RemoveAll();
    m_astrResources.Append(crastrResources);

    m_bNewDlg = bNewDlg;
}

/////////////////////////////////////////////////////////////////////////////
// CPropPageTargetGeneral message handlers

BOOL CPropPageTargetGeneral::OnInitDialog() 
{
	CDialog::OnInitDialog();

    FillCombos();

    CheckCommChannels();

    // if properties dialog, disable name edit
    if(!m_bNewDlg)
    {
        CEdit*   pEdit = (CEdit*)GetDlgItem(IDC_PROPGEN_NAME);
        pEdit->EnableWindow(FALSE);
    }

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}




//------------------------------------------------------------------*
/**
 * fill combos.
 *
 *  fill resource combo
 *  fill firmware combo
 *  fill communication channel combo
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             FillResourceCombo(), FillFirmwareCombo(), FillCommChannelCombo()
*/
void CPropPageTargetGeneral::FillCombos()
{
    FillResourceCombo();

    FillFirmwareCombo();

    FillCommChannelCombo();
}

//------------------------------------------------------------------*
/**
 * fill resource combo.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CPropPageTargetGeneral::FillResourceCombo()
{
    CString                 strNoAssign;

    strNoAssign.LoadString(IDS_NO_ASSIGN);
    m_tResourceCombo.AddString(strNoAssign);

    int iNumRes = m_astrResources.GetSize();
    for(int iRes = 0; iRes < iNumRes; ++iRes)
    {
        m_tResourceCombo.AddString(m_astrResources[iRes]);
    }
    int iSel = m_tResourceCombo.FindStringExact(0, m_strResource);

    if(iSel >= 0)
    {
        m_tResourceCombo.SetCurSel(iSel);
    }
    else
    {
        m_tResourceCombo.SetCurSel(0);
    }
}

//------------------------------------------------------------------*
/**
 * fill firmware combo.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CPropPageTargetGeneral::FillFirmwareCombo()
{
    CFirmwareData*          pFirmwareData;
    UINT    uiAnzFirmware = m_pAddonPar->GetFirmwareCount();
    for (UINT uiFirmware = 0; uiFirmware < uiAnzFirmware; uiFirmware++)
    {
        pFirmwareData = m_pAddonPar->GetFirmwareData(uiFirmware);
        ASSERT(pFirmwareData);
        if(pFirmwareData)
        {
            m_tFirmwareCombo.AddString(pFirmwareData->m_strVersionDescription);
        }
    }
    m_tFirmwareCombo.SetCurSel(m_uiFirmwareIndex);
}


//------------------------------------------------------------------*
/**
 * fill communiation channel combo.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CPropPageTargetGeneral::FillCommChannelCombo()
{
    CFirmwareData*          pFirmwareData;
    CString                 strCommChannelDesc;

    m_tCommChannelCombo.ResetContent();

    int             iCount(0);
    pFirmwareData = m_pAddonPar->GetFirmwareData(m_uiFirmwareIndex);

    if(pFirmwareData)
    {
        int iNumCommChannels = pFirmwareData->m_tCommChannelArr.GetSize();
        CCommChannelData    tCommChannelData;
        CString             strCommChannelDesc;
        for(int iCommChannel = 0; iCommChannel < iNumCommChannels; ++iCommChannel)
        {
            tCommChannelData = pFirmwareData->m_tCommChannelArr[iCommChannel];
            strCommChannelDesc = m_pAddonPar->GetCommChannelDescription(tCommChannelData.m_strId);
            m_tCommChannelCombo.AddString(strCommChannelDesc);
        }
    }

    // now select current comm channel
    strCommChannelDesc = m_pAddonPar->GetCommChannelDescription(m_strCommChannel);
    int iSel = m_tCommChannelCombo.FindStringExact(0, strCommChannelDesc);
    if(iSel >= 0)
    {
        m_tCommChannelCombo.SetCurSel(iSel);
    }
}

//------------------------------------------------------------------*
/**
 * check dialog data.
 *
 * @param           -
 * @return          dialog data correct?
 * @exception       -
 * @see             -
*/
BOOL CPropPageTargetGeneral::CheckDialogData()
{
    m_tResourceCombo.GetWindowText(m_strResource);

    // if name changed, check it
    if(m_bNameChanged)
    {
        // check name: it should be a valid IEC identifier
        HRESULT                     hr;
        enum E_IEC_IDENTIFIER_ERROR error;
        CComBSTR                    sName(m_strName);

        hr = m_pProjInfo->CheckIECIdentifier(sName, &error);
        if (hr!=S_OK)
        {
            CPropertySheet* pPropSheet = dynamic_cast<CPropertySheet*>(GetParent());
            if(pPropSheet)
            {
                pPropSheet->SetActivePage(this);
            }

            // goto name edit
            CWnd*   pTmp = GetDlgItem(IDC_PROPGEN_NAME);
            if(pTmp)
            {
                GotoDlgCtrl(pTmp);
            }

            if (error==IDENTIFIER_SYNTAX_ERROR)
            {
                CString errorMsg;
                errorMsg.FormatMessage(IDS_ERROR_IDENTIFIER_SYNTAX_ERROR,
                                       m_strName);
                ::AfxMessageBox(errorMsg, MB_OK|MB_ICONERROR);
            }
            else if (error==IDENTIFIER_IS_KEYWORD)
            {
                CString errorMsg;
                errorMsg.FormatMessage(IDS_ERROR_IDENTIFIER_IS_KEYWORD,
                                       m_strName);
                ::AfxMessageBox(errorMsg, MB_OK|MB_ICONERROR);
            }
            else if (error==IDENTIFIER_TOO_LONG)
            {
                CString errorMsg;
                errorMsg.FormatMessage(IDS_ERROR_IDENTIFIER_TOO_LONG,
                                       m_strName);
                ::AfxMessageBox(errorMsg, MB_OK|MB_ICONERROR);
            }
            else
            {
                CString errorMsg;
                errorMsg.FormatMessage(IDS_ERROR_IDENTIFIER,
                                       m_strName);
                ::AfxMessageBox(errorMsg, MB_OK|MB_ICONERROR);
            }

            return FALSE;
        }
    }
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * on selection change of communication channel.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CPropPageTargetGeneral::OnSelchangePropgenCommChannelCb() 
{
    CString strCommChannelDesc;
    m_tCommChannelCombo.GetWindowText(strCommChannelDesc);
    m_strCommChannel = m_pAddonPar->GetCommChannelFromDescription(strCommChannelDesc);
}

//------------------------------------------------------------------*
/**
 * on selection change of firmware.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CPropPageTargetGeneral::OnSelchangePropgenFirmware() 
{
    m_uiFirmwareIndex = m_tFirmwareCombo.GetCurSel();

    CheckCommChannels();
}

//------------------------------------------------------------------*
/**
 * on selection change of resource.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CPropPageTargetGeneral::OnSelchangePropgenResources() 
{
	m_tResourceCombo.GetWindowText(m_strResource);
}

//------------------------------------------------------------------*
/**
 * check comm channels.
 *
 *  checks, if available communication channels have changed
 *  calls CheckCommChannels() of property sheet and fills
 *  communication channel combo.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CPropPageTargetGeneral::CheckCommChannels()
{
    // get current communication channel
    CString strCommChannelDesc;

    // enable / disable communication pages
    CTargetPropertySheet*   pPropSheet = dynamic_cast<CTargetPropertySheet*>(GetParent());
    if(pPropSheet)
    {
        pPropSheet->CheckCommChannels(m_tFirmwareCombo.GetCurSel());
    }

    m_tCommChannelCombo.GetWindowText(strCommChannelDesc);
    // fill comm channel combo
    FillCommChannelCombo();

    int iSel = m_tCommChannelCombo.FindStringExact(0, strCommChannelDesc);
    if(iSel >= 0)
    {
        m_tCommChannelCombo.SetCurSel(iSel);
    }
}


//------------------------------------------------------------------*
/**
 * validate.
 *
 *  check name is correct and communication channel is set.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
BOOL CPropPageTargetGeneral::Validate()
{
    CString strCommChannelDesc;
    m_tCommChannelCombo.GetWindowText(strCommChannelDesc);
    if(strCommChannelDesc.IsEmpty())
    {
        if(m_tCommChannelCombo.GetCount())
        {
            ::AfxMessageBox(IDS_NO_COMM_CHANNEL);
            GotoDlgCtrl(&m_tCommChannelCombo);
        }
        return FALSE;
    }
    // check names
    if(!CheckDialogData())
    {
        return FALSE;
    }

    return TRUE;
}

BOOL CPropPageTargetGeneral::OnSetActive() 
{
	return CPropertyPage::OnSetActive();
}

void CPropPageTargetGeneral::OnChangePropgenName() 
{
    m_bNameChanged = TRUE;	
}


void CPropPageTargetGeneral::OnOK() 
{
	CPropertyPage::OnOK();
}

/* ---------------------------------------------------------------------------- */
