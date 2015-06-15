// PropCfgUserData.cpp : implementation file
//

#include "stdafx.h"
#include "ConfigGSD.h"
#include "PropCfgUserData.h"
#include "SheetConfigGSD.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PropCfgUserData property page

IMPLEMENT_DYNCREATE(PropCfgUserData, CPropertyPage)

//---------------------------- CONSTRUCTOR -----------------------------------*
PropCfgUserData::PropCfgUserData() : CPropertyPage(PropCfgUserData::IDD)
{
	//{{AFX_DATA_INIT(PropCfgUserData)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_ctrlCfgData.SetLinePropperties(8, 21);
	m_ctrlUserPrm.SetLinePropperties(8, 21);

	m_bUserChanged = FALSE;
	m_CfgBytes.RemoveAll();
	m_UserPrms.RemoveAll();
}

//---------------------------- DESTRUCTOR ------------------------------------*
PropCfgUserData::~PropCfgUserData()
{
}

//----------------------------- FUNCTION -------------------------------------*
void PropCfgUserData::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PropCfgUserData)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PropCfgUserData, CPropertyPage)
	//{{AFX_MSG_MAP(PropCfgUserData)
	ON_BN_CLICKED(IDC_RESTORE_CFG_DATA, OnRestoreCfgData)
	ON_BN_CLICKED(IDC_RESTORE_USER_DATA, OnRestoreUserData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PropCfgUserData message handlers

//----------------------------- FUNCTION -------------------------------------*
BOOL PropCfgUserData::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString szLineNmb;
	for (int i = 0; i < 21*8; i+=8)	{
		CString Nmb;
		Nmb.Format(_T("%d.\r\n"), i);
		szLineNmb += Nmb;
	}
	VERIFY(m_ctrlUserPrm.SubclassDlgItem(IDC_EDIT_USER_PRMS, this));
	VERIFY(m_ctrlCfgData.SubclassDlgItem(IDC_EDIT_CONFIGBYTES, this));

	GetDlgItem(IDC_EDIT_CONFIG_LINES)->SetWindowText(szLineNmb);
	GetDlgItem(IDC_EDIT_USER_LINES)->SetWindowText(szLineNmb);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//----------------------------- FUNCTION -------------------------------------*
BOOL PropCfgUserData::SetUserPrmData(VARIANT& vtUserPrm)
{
	HRESULT hr;
	m_UserPrms.RemoveAll();
	m_DefaultUserPrm.RemoveAll();

    ASSERT(vtUserPrm.vt == (VT_UI1 | VT_ARRAY));
    if (vtUserPrm.vt == (VT_UI1 | VT_ARRAY))
    {
        // now show values
        SAFEARRAY   *psaBytes = vtUserPrm.parray;
        long    lBound = 0, uBound = 0;
        hr = ::SafeArrayGetLBound(psaBytes, 1, &lBound);
        _ASSERTE(SUCCEEDED(hr));
        hr = ::SafeArrayGetUBound(psaBytes, 1, &uBound);
        _ASSERTE(SUCCEEDED(hr));
        // loop thru all objects in this level:
        for (long i = lBound; i <= uBound; i++)
        {
            BYTE    ucVal;
            hr = ::SafeArrayGetElement(psaBytes, &i, &ucVal);
            _ASSERTE(SUCCEEDED(hr));
            if (FAILED(hr)) {
                continue;
            }
			m_UserPrms.Add(ucVal);
			m_DefaultUserPrm.Add(ucVal);
            // next byte
        }
		return TRUE;
    }
	return FALSE;
}

//----------------------------- FUNCTION -------------------------------------*
BOOL PropCfgUserData::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class
	if (!m_ctrlCfgData.HasChanged())	{
		m_CfgBytes.RemoveAll();
		((CSheetConfigGSD*)GetParent())->GetCfgByteArray(&m_CfgBytes);
	}

    m_DefaultCfgBytes.RemoveAll();
	((CSheetConfigGSD*)GetParent())->GetCfgByteArray(&m_DefaultCfgBytes);
	m_ctrlUserPrm.SetHexContent(m_UserPrms.GetData(), m_UserPrms.GetSize());
	m_ctrlCfgData.SetHexContent(m_CfgBytes.GetData(), m_CfgBytes.GetSize());
	
	return CPropertyPage::OnSetActive();
}

//----------------------------- FUNCTION -------------------------------------*
BOOL PropCfgUserData::GetUserPrmData(CByteArray * pUserPrms)
{
	pUserPrms->RemoveAll();
	pUserPrms->SetSize(m_UserPrms.GetSize());
	for (int i = 0; i < m_UserPrms.GetSize(); i++)	{
		pUserPrms->SetAt(i, m_UserPrms.GetAt(i));
	}
	return TRUE;
}

//----------------------------- FUNCTION -------------------------------------*
BOOL PropCfgUserData::CreateCfgByteArray(CByteArray * pCfgBytes)
{
	pCfgBytes->RemoveAll();
	pCfgBytes->SetSize(m_CfgBytes.GetSize());
	for (int iCount = 0; iCount <= m_CfgBytes.GetUpperBound(); iCount++)	{
		pCfgBytes->SetAt(iCount, m_CfgBytes.GetAt(iCount));
	}
	return TRUE;
}

//----------------------------- FUNCTION -------------------------------------*
BOOL PropCfgUserData::IsChangedByUser()
{
	return m_bUserChanged;
}

//----------------------------- FUNCTION -------------------------------------*
BOOL PropCfgUserData::SetUserChanged(BOOL bNew)
{
	BOOL	bOld = m_bUserChanged;
	m_bUserChanged = bNew;
	return bOld;
}

//----------------------------- FUNCTION -------------------------------------*
BOOL PropCfgUserData::OnKillActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_ctrlCfgData.HasChanged())	{
		int iMaxLen = m_ctrlCfgData.GetWindowTextLength();
		BYTE*   pData = new BYTE[iMaxLen];
		ASSERT(pData != NULL);
		int cnt = m_ctrlCfgData.GetHexContent(pData, iMaxLen);
		m_CfgBytes.RemoveAll();
		m_CfgBytes.SetSize(cnt);
		for (int i = 0; i < cnt; i++)
			m_CfgBytes.SetAt(i, pData[i]);
		delete [] pData;
	}

	if (m_ctrlUserPrm.HasChanged())	{
		int iMaxLen = m_ctrlUserPrm.GetWindowTextLength();
		BYTE*   pData = new BYTE[iMaxLen];
		ASSERT(pData != NULL);
		int cnt = m_ctrlUserPrm.GetHexContent(pData, iMaxLen);
		m_UserPrms.RemoveAll();
		m_UserPrms.SetSize(cnt);
		for (int i = 0; i < cnt; i++)
			m_UserPrms.SetAt(i, pData[i]);
		delete [] pData;
	}

	return CPropertyPage::OnKillActive();
}
//----------------------------- FUNCTION -------------------------------------*
void PropCfgUserData::OnRestoreCfgData() 
{
	// TODO: Add your control notification handler code here
	if (((CSheetConfigGSD*)GetParent())->StationIsModular())	{
		m_CfgBytes.RemoveAll();
		((CSheetConfigGSD*)GetParent())->GetCfgByteArray(&m_CfgBytes);
	}
	else	{
		m_CfgBytes.RemoveAll();
		m_CfgBytes.SetSize(m_DefaultCfgBytes.GetSize());
		for (int i = 0; i < m_DefaultCfgBytes.GetSize(); i++)	{
			m_CfgBytes.SetAt(i, m_DefaultCfgBytes.GetAt(i));
		}
	}
	m_ctrlCfgData.SetHexContent(m_CfgBytes.GetData(), m_CfgBytes.GetSize());
	m_ctrlCfgData.ResetChangedFlag();
}

//----------------------------- FUNCTION -------------------------------------*
void PropCfgUserData::OnRestoreUserData() 
{
	// TODO: Add your control notification handler code here
	m_UserPrms.RemoveAll();
	m_UserPrms.SetSize(m_DefaultUserPrm.GetSize());
	for (int i = 0; i < m_DefaultUserPrm.GetSize(); i++)	{
		m_UserPrms.SetAt(i, m_DefaultUserPrm.GetAt(i));
	}
	m_ctrlUserPrm.SetHexContent(m_UserPrms.GetData(), m_UserPrms.GetSize());
	m_ctrlUserPrm.ResetChangedFlag();
}
