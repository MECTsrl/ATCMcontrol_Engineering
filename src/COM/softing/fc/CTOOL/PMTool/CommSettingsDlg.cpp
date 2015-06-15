// CommSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "CommSettingsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCommSettingsDlg dialog


CCommSettingsDlg::CCommSettingsDlg(CMainPropertySheet* pMainSheet, CWnd* pParent /*=NULL*/)
	: CDialog(CCommSettingsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCommSettingsDlg)
	//}}AFX_DATA_INIT

    m_pMainSheet    = pMainSheet;
    m_pFWArr        = NULL;
    m_bInit         = TRUE;
    m_bConnEdited   = FALSE;
}

CCommSettingsDlg::~CCommSettingsDlg()
{
    if(m_pFWArr)
    {
        delete m_pFWArr;
        m_pFWArr = NULL;
    }
}

void CCommSettingsDlg::Initialize(  const CStringArray& crastrTargetTypes,
                                    const CString& crstrTargetType, 
                                    const CString& crstrFirmwareVersion,
                                    const CString& crstrCommChannel)
{
    m_astrTargetTypes.Copy(crastrTargetTypes);
    m_strTargetType         = crstrTargetType;
    m_strFirmwareVersion    = crstrFirmwareVersion;
    m_strCommChannel        = crstrCommChannel;
}

void CCommSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCommSettingsDlg)
	DDX_Control(pDX, IDC_TARGET_TYPE_COMBO, m_tTargetTypeCombo);
	DDX_Control(pDX, IDC_TARGET_FIRMWARE_COMBO, m_tFirmwareCombo);
	DDX_Control(pDX, IDC_COMM_CHANNEL_COMBO, m_tCommChannelCombo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCommSettingsDlg, CDialog)
	//{{AFX_MSG_MAP(CCommSettingsDlg)
	ON_CBN_SELCHANGE(IDC_TARGET_TYPE_COMBO, OnSelchangeTargetTypeCombo)
	ON_CBN_SELCHANGE(IDC_TARGET_FIRMWARE_COMBO, OnSelchangeTargetFirmwareCombo)
	ON_CBN_SELCHANGE(IDC_COMM_CHANNEL_COMBO, OnSelchangeCommChannelCombo)
	ON_CBN_EDITCHANGE(IDC_CONN_PAR_COMBO, OnEditchangeConnParCombo)
	ON_EN_CHANGE(IDC_CONNPAR_EDIT, OnChangeConnparEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommSettingsDlg message handlers


BOOL CCommSettingsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

    if(!m_strTargetType.IsEmpty())
    {
        m_pFWArr = new TFirmwareArr;
        if(!m_pMainSheet->LoadKADFile(m_strTargetType, *m_pFWArr))
        {
            delete m_pFWArr;
            m_pFWArr = NULL;
        }
    }

    FillTargetTypeCombo();
    m_bInit = FALSE;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCommSettingsDlg::FillTargetTypeCombo()
{
    m_tTargetTypeCombo.ResetContent();

    int iNumTargets = m_astrTargetTypes.GetSize();
    int iSel = CB_ERR;
    for(int iTarget = 0; iTarget < iNumTargets; ++iTarget)
    {
        m_tTargetTypeCombo.AddString(m_astrTargetTypes[iTarget]);
        if(m_strTargetType.CompareNoCase(m_astrTargetTypes[iTarget]) == 0)
        {
            iSel = iTarget;
        }
    }
    if(iSel == CB_ERR)
    {
		// if given target has been uninstalled
		// show it also
		m_tTargetTypeCombo.InsertString(0, m_strTargetType);
        iSel = 0;
    }
    if(m_tTargetTypeCombo.GetCount())
    {
        m_tTargetTypeCombo.SetCurSel(iSel);
        FillFirmwareCombo();
    }
}

void CCommSettingsDlg::OnSelchangeTargetTypeCombo() 
{
    UpdateData();

    CString strTargetType;
    m_tTargetTypeCombo.GetWindowText(strTargetType);

    TFirmwareArr*   pFWArr = new TFirmwareArr;

    if(m_pMainSheet->LoadKADFile(strTargetType, *pFWArr))
    {
        m_strTargetType = strTargetType;
        if(m_pFWArr)
        {
            delete m_pFWArr;
        }
        m_pFWArr = pFWArr;
    }
    else
    {
        delete pFWArr;
        
        // delete from target type array
        int iFound = -1;
        int iNumTT = m_astrTargetTypes.GetSize();
        for(int iTT = 0; iTT < iNumTT; ++iTT)
        {
            if(strTargetType.CompareNoCase(m_astrTargetTypes[iTT]) == 0)
            {
                iFound = iTT;
                break;
            }
        }
        if(iFound != -1)
        {
            m_astrTargetTypes.RemoveAt(iFound);
        }
    }
    FillTargetTypeCombo();
}

void CCommSettingsDlg::FillFirmwareCombo()
{
    m_tFirmwareCombo.ResetContent();

    if(!m_pFWArr)
    {
        return;
    }

    int             iNumFW = m_pFWArr->GetSize();
    CString         strVersion;
    int             iSel = CB_ERR;
    for(int iFW = 0; iFW < iNumFW; ++iFW)
    {
        strVersion.Format(_T("%u"), (*m_pFWArr)[iFW].m_uiVersion);
        m_tFirmwareCombo.AddString(strVersion);
        if(m_strFirmwareVersion.CompareNoCase(strVersion) == 0)
        {
            //m_strConnectGuid = (*m_pFWArr)[iFW].m_strCustomConnectGuid;
            iSel = iFW;
        }
    }
    if(iSel == CB_ERR)
    {
        iSel = m_tFirmwareCombo.GetCount() - 1;     // choose latest version
    }
    if(m_tFirmwareCombo.GetCount())
    {
        m_tFirmwareCombo.SetCurSel(iSel);
        FillCommChannelCombo();
    }
}

void CCommSettingsDlg::FillCommChannelCombo()
{
    m_tCommChannelCombo.ResetContent();
    
    if(!m_pFWArr)
    {
        return;
    }

    CString strFirmwareVersion;
    m_tFirmwareCombo.GetWindowText(strFirmwareVersion);

    CFirmwareData       tFWData;
    CCommChannelData    tCCData;
    int                 iSel = CB_ERR;
    if(GetFirmwareData(strFirmwareVersion, tFWData))
    {
        int iNumCC = tFWData.m_tCommChannelArr.GetSize();
        for(int iCC = 0; iCC < iNumCC; ++iCC)
        {
            tCCData = tFWData.m_tCommChannelArr[iCC];
            m_tCommChannelCombo.AddString(tCCData.m_strId);
            if(tCCData.m_strId.CompareNoCase(m_strCommChannel) == 0)
            {
                iSel = iCC;
            }
        }
    }
    if(iSel == CB_ERR)
    {
        iSel = 0;
    }
    if(m_tCommChannelCombo.GetCount())
    {
        m_tCommChannelCombo.SetCurSel(iSel);
    }
}


BOOL CCommSettingsDlg::GetFirmwareData(const CString& crstrFirmwareVersion, CFirmwareData& rtFWData)
{
    if(!m_pFWArr)
    {
        return FALSE;
    }

    BOOL    bReturn = FALSE;
    UINT    uiFirmwareVersion = 0;
    if(!crstrFirmwareVersion.IsEmpty())
    {
        uiFirmwareVersion = (UINT)atoi(crstrFirmwareVersion);
    }
    int iNumFW = m_pFWArr->GetSize();
    for(int iFW = 0; iFW < iNumFW; ++iFW)
    {
        if((*m_pFWArr)[iFW].m_uiVersion == uiFirmwareVersion)
        {
            rtFWData = (*m_pFWArr)[iFW];
            bReturn = TRUE;
            break;
        }
    }
    return bReturn;
}

void CCommSettingsDlg::OnSelchangeTargetFirmwareCombo() 
{
    UpdateData();

    m_tFirmwareCombo.GetWindowText(m_strFirmwareVersion);

    FillCommChannelCombo();
}

void CCommSettingsDlg::OnSelchangeCommChannelCombo() 
{
    UpdateData();

    m_tCommChannelCombo.GetWindowText(m_strCommChannel);
}


void CCommSettingsDlg::OnEditchangeConnParCombo() 
{
}


void CCommSettingsDlg::GetData
( 
    CString& rstrTargetType,
    CString& rstrFirmwareVersion,
    CString& rstrCommChannel
)
{
    rstrTargetType      = m_strTargetType;
    rstrFirmwareVersion = m_strFirmwareVersion;
    rstrCommChannel     = m_strCommChannel;
}


void CCommSettingsDlg::OnChangeConnparEdit() 
{
    m_bConnEdited = TRUE;
}

void CCommSettingsDlg::OnOK() 
{
    m_tTargetTypeCombo.GetWindowText(m_strTargetType);
    m_tFirmwareCombo.GetWindowText(m_strFirmwareVersion);
    m_tCommChannelCombo.GetWindowText(m_strCommChannel);
	
	CDialog::OnOK();
}
