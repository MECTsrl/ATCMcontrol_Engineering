/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"
#include "BaseTargetData.h"
#include "TargetSettingsSheet.h"
#include "basedefs.h"


/////////////////////////////////////////////////////////////////////////////
// CTargetSettingsSheet

IMPLEMENT_DYNAMIC(CTargetSettingsSheet, CPropertySheet)


//------------------------------------------------------------------*
/**
 * constructor
 *
 * @see             
*/
CTargetSettingsSheet::CTargetSettingsSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage) :
  CPropertySheet(nIDCaption, pParentWnd, iSelectPage),
  m_pBaseTargetData(NULL)
{
}


//------------------------------------------------------------------*
/**
 * constructor
 *
 * @see             
*/
CTargetSettingsSheet::CTargetSettingsSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage) :
  CPropertySheet(pszCaption, pParentWnd, iSelectPage),
  m_pBaseTargetData(NULL)
{
}

//------------------------------------------------------------------*
/**
 * destructor
 *
 * @see             
*/
CTargetSettingsSheet::~CTargetSettingsSheet()
{
    
}




BEGIN_MESSAGE_MAP(CTargetSettingsSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CTargetSettingsSheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTargetSettingsSheet message handlers



//------------------------------------------------------------------*
/**
 * Init initialize the dialog box
 *
 * @param           strFileName: the project file
 * @param           pICEProjInfo: pointer to the project manager
 * @return          
 * @exception       -
 * @see             
*/
HRESULT CTargetSettingsSheet::Init(const CString& strFileName, ICEProjInfo* pICEProjInfo, CBaseTargetData* pBaseTargetData)
{
    ////////////////////////////////////////////////////
    // set members
    m_pICEProjInfo = pICEProjInfo;
    m_strFileName = strFileName;
    m_pBaseTargetData = pBaseTargetData;

    ////////////////////////////////////////////////////
    // add debug page
    AddPage(&m_debugPage);

    // and fill it
    BOOL bNCC;
    m_debugPage.m_iDebugOpt = ReadDebugState(bNCC);
    if (m_debugPage.m_iDebugOpt == -1)
    {
        m_debugPage.m_iDebugOpt = 1;
        m_bChanged = TRUE;
    }

    m_debugPage.m_bNCCDisabled = !bNCC;

    CBaseAddonParameter*    pAddonPar = m_pBaseTargetData->GetAddonParameter();
    ASSERT(pAddonPar);
    if(pAddonPar)
    {
        m_debugPage.m_bSupportNCC = pAddonPar->m_bSupportNCC;
    }

    m_debugPage.Init(this);
    ////////////////////////////////////////////////////


    ////////////////////////////////////////////////////
    // add connect page
    AddPage(&m_connectPage);
    // and fill it
    m_connectPage.m_strConnectTimeout = ReadConnectTimeout();
    if (m_connectPage.m_strConnectTimeout.IsEmpty())
    {
        m_connectPage.m_strConnectTimeout = _T("100");
        m_bChanged = TRUE;
    }
    m_connectPage.Init(this);
    /////////////////////////////////////////////////////

    return S_OK;
}



//------------------------------------------------------------------*
/**
 * ReadDebugState get the breakpoint options from the 4cp file
 *
 * @param           bNCC: will be set to true if ncc is enabled, 
 *                        false if disabled in 4cp file.
 * @return          0,1,2 : the breakpoint option set
 *                  -1 : no option set
 * @exception       -
 * @see             
*/
int CTargetSettingsSheet::ReadDebugState(BOOL& bNCC)
{
    HRESULT hr;

    int iDS = -1;
    int iNCC = -1;

    CBaseAddonParameter*    pAddonPar = m_pBaseTargetData->GetAddonParameter();

    CString strSection;
    strSection.Format("%s%s", TARGET_SET_SECTION, pAddonPar->m_strFileExtension);

    CComBSTR sSection(strSection);
    CComBSTR sKey = TARGET_SET_OPTION;
    SAFEARRAY* parrKeys;

    hr = m_pICEProjInfo->CPGetAllValuesForKeyFromSection(sSection, sKey,
                                                         &parrKeys);

    if (hr==S_OK)
    {
        BSTR HUGEP *pbstr;
        BSTR bstr;
        unsigned long i;

        // Get a pointer to the elements of the array.
        hr = ::SafeArrayAccessData(parrKeys, (void HUGEP**)&pbstr);
        ASSERT(hr == S_OK);
        if (hr==S_OK)
        {
            for (i = 0; i < ((parrKeys)->rgsabound[0]).cElements; i++)
            {
                CString str;
                bstr = pbstr[i];
                str = bstr;
                if (str.CompareNoCase(TARGET_SET_BP_NONE)==0)
                {
                    iDS = 0;
                    m_strOrigDebug = TARGET_SET_BP_NONE;
                }
                else if (str.CompareNoCase(TARGET_SET_BP_SIMPLE)==0)
                {
                    iDS = 1;
                    m_strOrigDebug = TARGET_SET_BP_SIMPLE;
                }
                else if (str.CompareNoCase(TARGET_SET_BP_ADVANCED)==0)
                {
                    iDS = 2;
                    m_strOrigDebug = TARGET_SET_BP_ADVANCED;
                }
                else if (str.CompareNoCase(TARGET_SET_NCC_0)==0)
                {
                    iNCC = 0;
                }
                else if (str.CompareNoCase(TARGET_SET_NCC_1)==0)
                {
                    iNCC = 1;
                }
            }
        }
        ::SafeArrayUnaccessData(parrKeys);		
        ::SafeArrayDestroy(parrKeys);
    }

    if (iNCC==-1)
    {
        iNCC = 1;
        if (iDS==2)
        {
            iNCC = 0;
        }
    }

    bNCC = (iNCC==1);

    return iDS;
}


//------------------------------------------------------------------*
/**
 * ReadConnectTimeout reads the connect timeout from the 4cp file
 *
 * @return          the connect timeout
 *                   empty string if not set
 * @exception       -
 * @see             
*/
CString CTargetSettingsSheet::ReadConnectTimeout()
{
    HRESULT hr;

    CBaseAddonParameter*    pAddonPar = m_pBaseTargetData->GetAddonParameter();

    CString strValue;
    CString strSection;
    strSection.Format("%s%s", TARGET_SET_TARGET_SECTION, pAddonPar->m_strKADType);

    CComBSTR sSection = strSection;
    CComBSTR sKey = TARGET_SET_CONN_KEY;
    CComBSTR sValue;

    hr = m_pICEProjInfo->CPGetValueForKeyFromSection(sSection, sKey, &sValue);
    
    if (hr==S_OK)
    {
        strValue = sValue;
    }

    return strValue; 
}



//------------------------------------------------------------------*
/**
 * StoreChanges store the changes of the dialog box back into the 4cp file
 *
 * @return          
 * @exception       -
 * @see             
*/
BOOL CTargetSettingsSheet::StoreChanges()
{
    HRESULT hr;

    if (!m_bChanged)
    {
        return TRUE;
    }

    CBaseAddonParameter*    pAddonPar = m_pBaseTargetData->GetAddonParameter();

    // debug page
    int iDebugState = m_debugPage.m_iDebugOpt;
    BOOL bNCC = !m_debugPage.m_bNCCDisabled;
    CString strSection;
    strSection.Format("%s%s", TARGET_SET_SECTION, pAddonPar->m_strFileExtension);

    CComBSTR sSection(strSection);
    CComBSTR sKey(TARGET_SET_OPTION);

    // delete old line
    if (!m_strOrigDebug.IsEmpty())
    {
        CComBSTR sDelKey = m_strOrigDebug;
        hr = m_pICEProjInfo->CPDeleteKeyWithValueFromSection(sSection, sKey, sDelKey);
        ASSERT(hr==S_OK);
    }

    CComBSTR sNCCKey0 = TARGET_SET_NCC_0;
    CComBSTR sNCCKey1 = TARGET_SET_NCC_1;
    hr = m_pICEProjInfo->CPDeleteKeyWithValueFromSection(sSection, sKey, sNCCKey0);
    hr = m_pICEProjInfo->CPDeleteKeyWithValueFromSection(sSection, sKey, sNCCKey1);

    // insert new line 
    CComBSTR sValue;
    switch(iDebugState)
    {
    case 0:
        sValue = TARGET_SET_BP_NONE;
        break;
    case 1:
        sValue = TARGET_SET_BP_SIMPLE;
        break;
    case 2:
        sValue = TARGET_SET_BP_ADVANCED;
        break;
    }
    hr = m_pICEProjInfo->CPInsertToSection(sSection, sKey, sValue);
    if (hr!=S_OK)
    {
        return FALSE;
    }

    m_strOrigDebug = sValue;

    CComBSTR sValueNCC;
    if (bNCC)
    {
        sValueNCC = TARGET_SET_NCC_1;
    }
    else
    {
        sValueNCC = TARGET_SET_NCC_0;
    }
    hr = m_pICEProjInfo->CPInsertToSection(sSection, sKey, sValueNCC);


    // connect page
    strSection.Format("%s%s", TARGET_SET_TARGET_SECTION, pAddonPar->m_strKADType);

    sSection = strSection;
    sKey = TARGET_SET_CONN_KEY;

    // delete old line
    hr = m_pICEProjInfo->CPDeleteFromSection(sSection, sKey);
       
    // insert new line 
    sValue = m_connectPage.m_strConnectTimeout;
    
    hr = m_pICEProjInfo->CPInsertToSection(sSection, sKey, sValue);
    if (hr!=S_OK)
    {
        return FALSE;
    }

    m_bChanged = FALSE;
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * SetChanged mark the data of the dialog box changed
 *
 * @param           changed: the state of the data
 * @return          
 * @exception       -
 * @see             
*/
void CTargetSettingsSheet::SetChanged(BOOL changed)
{
    m_bChanged = changed;
}





/////////////////////////////////////////////////////////////////////////////
// CDebugSettingsPage property page

IMPLEMENT_DYNCREATE(CDebugSettingsPage, CPropertyPage)


//------------------------------------------------------------------*
/**
 * constructor
 *
 * @see             
*/
CDebugSettingsPage::CDebugSettingsPage() : CPropertyPage(CDebugSettingsPage::IDD)
{
	//{{AFX_DATA_INIT(CDebugSettingsPage)
	m_iDebugOpt = -1;
	m_bNCCDisabled = FALSE;
	//}}AFX_DATA_INIT
    m_pParent = NULL;
    m_bSupportNCC = FALSE;
}


//------------------------------------------------------------------*
/**
 * destructor
 *
 * @see             
*/
CDebugSettingsPage::~CDebugSettingsPage()
{
}



//------------------------------------------------------------------*
/**
 * Init the property page
 *
 * @param           pParent: pointer to the property sheet class, 
 *                           the property sheet has to write back the changes
 * @return          
 * @exception       -
 * @see             
*/
void CDebugSettingsPage::Init(CTargetSettingsSheet* pParent)
{
    m_pParent = pParent;
    m_bOldNCCStatus = m_bNCCDisabled;
}


//------------------------------------------------------------------*
/**
 * DoDataExchange
 *
 * @see             
*/
void CDebugSettingsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDebugSettingsPage)
	DDX_Radio(pDX, IDC_SETTINGS_BP_NONE, m_iDebugOpt);
	DDX_Check(pDX, IDC_CHECK_NCC_DISABLED, m_bNCCDisabled);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDebugSettingsPage, CPropertyPage)
	//{{AFX_MSG_MAP(CDebugSettingsPage)
	ON_BN_CLICKED(IDC_SETTINGS_BP_ADVANCED, OnMarkDirty)
	ON_BN_CLICKED(IDC_SETTINGS_BP_NONE, OnMarkDirty)
	ON_BN_CLICKED(IDC_SETTINGS_BP_SIMPLE, OnMarkDirty)
	ON_BN_CLICKED(IDC_CHECK_NCC_DISABLED, OnMarkDirty)
    ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDebugSettingsPage message handlers



//------------------------------------------------------------------*
/**
 * OnApply: apply/ok button pressed: store changes
 *
 * @return          
 * @exception       -
 * @see             
*/
BOOL CDebugSettingsPage::OnApply() 
{
	ASSERT (m_pParent);
    BOOL res = m_pParent->StoreChanges();
    res = res & CPropertyPage::OnApply();
	return res;
}



//------------------------------------------------------------------*
/**
 * OnMarkDirty: something has been changed in the dialog, so data has been changed
 *
 * @return          
 * @exception       -
 * @see             
*/
void CDebugSettingsPage::OnMarkDirty() 
{
    SetModified(TRUE);
    ASSERT(m_pParent);
    m_pParent->SetChanged(TRUE);

    int iOldStatus = m_iDebugOpt;

    UpdateData(TRUE);

    if ((m_iDebugOpt==2) && (iOldStatus<2))
    {
        m_bOldNCCStatus = m_bNCCDisabled;
        m_bNCCDisabled = TRUE;
        UpdateData(FALSE);
        if(m_bSupportNCC && !m_pParent->m_pBaseTargetData->IsNCCBreakpointSupport())
        {
            CWnd* pCheckBox = NULL;
            pCheckBox = GetDlgItem(IDC_CHECK_NCC_DISABLED);
            if (pCheckBox)
            {
                pCheckBox->EnableWindow(FALSE);
            }
        }
    }

    if ((m_iDebugOpt<2) && (iOldStatus==2))
    {
        m_bNCCDisabled = m_bOldNCCStatus;
        UpdateData(FALSE);
        if(m_bSupportNCC && !m_pParent->m_pBaseTargetData->IsNCCBreakpointSupport())
        {
            CWnd* pCheckBox = NULL;
            pCheckBox = GetDlgItem(IDC_CHECK_NCC_DISABLED);
            if (pCheckBox)
            {
                pCheckBox->EnableWindow(TRUE);
            }
        }
    }
}


BOOL CDebugSettingsPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

    if(m_bSupportNCC)
    {
        if (m_iDebugOpt==2 && !m_pParent->m_pBaseTargetData->IsNCCBreakpointSupport())
        {
            m_bOldNCCStatus = m_bNCCDisabled;
            m_bNCCDisabled = TRUE;
            UpdateData(FALSE);
            CWnd* pCheckBox = NULL;
            pCheckBox = GetDlgItem(IDC_CHECK_NCC_DISABLED);
            if (pCheckBox)
            {
                pCheckBox->EnableWindow(FALSE);
            }
        }

        if (m_iDebugOpt<2 && !m_pParent->m_pBaseTargetData->IsNCCBreakpointSupport())
        {
            m_bNCCDisabled = m_bOldNCCStatus;
            UpdateData(FALSE);
            CWnd* pCheckBox = NULL;
            pCheckBox = GetDlgItem(IDC_CHECK_NCC_DISABLED);
            if (pCheckBox)
            {
                pCheckBox->EnableWindow(TRUE);
            }
        }
    }
    else
    {
        CWnd* pCheckBox = GetDlgItem(IDC_CHECK_NCC_DISABLED);
        if(pCheckBox)
        {
            pCheckBox->ShowWindow(SW_HIDE);
        }
    }

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}




BOOL CDebugSettingsPage::OnHelpInfo(HELPINFO* pHelpInfo) 
{
    ASSERT(m_pParent!=NULL);
    if (m_pParent==NULL)
    {
        return 0;
    }
    
    ASSERT(m_pParent->m_pICEProjInfo!=NULL);
    if (m_pParent->m_pICEProjInfo==NULL)
    {
        return 0;
    }

    (m_pParent->m_pICEProjInfo)->ShowContextHelp(IDD_DEBUG_PROPPAGE);
    return 0;
}





/////////////////////////////////////////////////////////////////////////////
// CConnectSettingsPage property page

IMPLEMENT_DYNCREATE(CConnectSettingsPage, CPropertyPage)


//------------------------------------------------------------------*
/**
 * constructor
 *
 * @see             
*/
CConnectSettingsPage::CConnectSettingsPage() : CPropertyPage(CConnectSettingsPage::IDD)
{
	//{{AFX_DATA_INIT(CConnectSettingsPage)
	m_strConnectTimeout = _T("");
	//}}AFX_DATA_INIT
    m_pParent = NULL;
}


//------------------------------------------------------------------*
/**
 * destructor
 *
 * @see             
*/
CConnectSettingsPage::~CConnectSettingsPage()
{
}


//------------------------------------------------------------------*
/**
 * Init the property page
 *
 * @param           pParent: pointer to the property sheet class, 
 *                           the property sheet has to write back the changes
 * @return          
 * @exception       -
 * @see             
*/
void CConnectSettingsPage::Init(CTargetSettingsSheet* pParent)
{
    m_pParent = pParent;
}


//------------------------------------------------------------------*
/**
 * DoDataExchange
 *
 * @see             
*/
void CConnectSettingsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConnectSettingsPage)
	DDX_Text(pDX, IDC_CONNECT_TIMEOUT, m_strConnectTimeout);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConnectSettingsPage, CPropertyPage)
	//{{AFX_MSG_MAP(CConnectSettingsPage)
	ON_EN_CHANGE(IDC_CONNECT_TIMEOUT, OnChangeConnectTimeout)
    ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConnectSettingsPage message handlers


//------------------------------------------------------------------*
/**
 * OnApply: apply/ok button pressed: store changes
 *
 * @return          
 * @exception       -
 * @see             
*/
BOOL CConnectSettingsPage::OnApply() 
{
	ASSERT (m_pParent);
    BOOL res = m_pParent->StoreChanges();
    res = res & CPropertyPage::OnApply();
	return res;
}

//------------------------------------------------------------------*
/**
 * OnChangeConnectTimeout: something has been changed in the dialog, 
 * so data has been changed
 *
 * @return          
 * @exception       -
 * @see             
*/
void CConnectSettingsPage::OnChangeConnectTimeout() 
{
    SetModified(TRUE);
    ASSERT(m_pParent);
    m_pParent->SetChanged(TRUE);	
}





BOOL CConnectSettingsPage::OnHelpInfo(HELPINFO* pHelpInfo) 
{
    ASSERT(m_pParent!=NULL);
    if (m_pParent==NULL)
    {
        return 0;
    }
    
    ASSERT(m_pParent->m_pICEProjInfo!=NULL);
    if (m_pParent->m_pICEProjInfo==NULL)
    {
        return 0;
    }

    (m_pParent->m_pICEProjInfo)->ShowContextHelp(IDD_CONNECT_PROPPAGE);
    return 0;
}

/* ---------------------------------------------------------------------------- */
