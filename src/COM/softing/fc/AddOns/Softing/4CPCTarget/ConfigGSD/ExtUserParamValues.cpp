// ExtUserParamValues.cpp : implementation file
//

#include "stdafx.h"
#include "ExtParam.h"
#include "ExtUserParamValues.h"
#include "ConfGSD_Utils.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ExtUserParamValues dialog

//--------------------------- CONSTRUCTOR ------------------------------------*
ExtUserParamValues::ExtUserParamValues(ExtParam* pParam, CWnd* pParent /*=NULL*/)
	: CDialog(ExtUserParamValues::IDD, pParent)
{
	//{{AFX_DATA_INIT(ExtUserParamValues)
	m_dwParamMax = 0;
	m_dwParamMin = 0;
	m_dwOffset = 0;
	//}}AFX_DATA_INIT
	m_pParam = pParam;
}


//----------------------------- FUNCTION -------------------------------------*
void ExtUserParamValues::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
    if (m_pParam->IsVariableEnumParam()) {
	    DDX_Control(pDX, IDC_EXT_PRM_VALUES1, m_cboExtPrmValues);
        ::ShowWindow(::GetDlgItem(m_hWnd, IDC_EXT_PRM_VALUES), SW_HIDE);
    }
    else {
	    DDX_Control(pDX, IDC_EXT_PRM_VALUES, m_cboExtPrmValues);
        ::ShowWindow(::GetDlgItem(m_hWnd, IDC_EXT_PRM_VALUES1), SW_HIDE);
    }
	//{{AFX_DATA_MAP(ExtUserParamValues)
	DDX_Text(pDX, IDC_EXT_PRM_MAX, (LONG&)m_dwParamMax);
	DDX_Text(pDX, IDC_EXT_PRM_MIN, (LONG&)m_dwParamMin);
	DDX_Text(pDX, IDC_EXT_PRM_OFFSET, m_dwOffset);
	//}}AFX_DATA_MAP
    if (pDX->m_bSaveAndValidate) {
		if (!IsValidParamValue())	{
			m_cboExtPrmValues.SetFocus();
			pDX->PrepareEditCtrl(IDC_EXT_PRM_VALUES);
			pDX->Fail();
		}
	}
}


BEGIN_MESSAGE_MAP(ExtUserParamValues, CDialog)
	//{{AFX_MSG_MAP(ExtUserParamValues)
	ON_CBN_SELCHANGE(IDC_EXT_PRM_VALUES, OnSelchangeExtPrmValues)
	ON_CBN_EDITCHANGE(IDC_EXT_PRM_VALUES, OnEditchangeExtPrmValues)
	ON_CBN_SELCHANGE(IDC_EXT_PRM_VALUES1, OnSelchangeExtPrmValues)
	ON_CBN_EDITCHANGE(IDC_EXT_PRM_VALUES1, OnEditchangeExtPrmValues)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ExtUserParamValues message handlers

//----------------------------- FUNCTION -------------------------------------*
BOOL ExtUserParamValues::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
	if (m_pParam == NULL)
		return TRUE;

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EXT_PRM_DESCRIPTION);
	if (pEdit)
		pEdit->SetWindowText(m_pParam->GetDescription());

    ASSERT(!m_pParam->IsConstantParam());

    m_dwOffset = m_pParam->GetByteOffset();
	m_dwParamMin = m_pParam->GetLowerValue();
	m_dwParamMax = m_pParam->GetUpperValue();
	if (!m_pParam->IsVariableEnumParam()) {
		CString szVal;
		szVal.Format(_T("%d"), m_pParam->GetUsedValue());
		m_cboExtPrmValues.SetWindowText(szVal);
    }
    else {
        int iSelIndex = -1;
		CObList* pEnumList = m_pParam->GetNamedEnumList();
		
		POSITION pos = pEnumList->GetHeadPosition();
		while (pos)	{
			NamedEnumValues* pEnumVal = (NamedEnumValues*)pEnumList->GetNext(pos);
			
			int index = m_cboExtPrmValues.AddString(pEnumVal->szName);
			if (index != CB_ERR && index != CB_ERRSPACE)	{
				m_cboExtPrmValues.SetItemData(index, pEnumVal->Value);
                if (pEnumVal->Value == m_pParam->GetUsedValue())
                    iSelIndex = index;
			}
		}
		m_cboExtPrmValues.SetCurSel(iSelIndex);
		OnSelchangeExtPrmValues();
	}

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//----------------------------- FUNCTION -------------------------------------*
void ExtUserParamValues::OnSelchangeExtPrmValues() 
{
	// TODO: Add your control notification handler code here
	int iSel = m_cboExtPrmValues.GetCurSel();
	
	DWORD Value = m_cboExtPrmValues.GetItemData(iSel);
	CString szBits;
	if (Value != CB_ERR) {
        if (m_pParam->IsBitArea()) {
            Value <<= m_pParam->GetBitAreaStart();
        }
		szBits = ::ConvertDWORD2BitString(Value);
    }

	CStatic* pBits = (CStatic*)GetDlgItem(IDC_PARAM_BITS);
	if (pBits)
		pBits->SetWindowText(szBits);
}

//----------------------------- FUNCTION -------------------------------------*
void ExtUserParamValues::OnOK() 
{
	// TODO: Add extra validation here
	if (!m_pParam->IsVariableEnumParam())	{
		CString szNmb;
		m_cboExtPrmValues.GetWindowText(szNmb);
		LPTSTR pszNmb = szNmb.GetBuffer(20);
		DWORD dwNmb = _ttol(pszNmb);
		szNmb.ReleaseBuffer();
		m_pParam->SetUsedValue(dwNmb);
	}
	else	{
		int iSel = m_cboExtPrmValues.GetCurSel();
		DWORD Value = m_cboExtPrmValues.GetItemData(iSel);
		if (Value != CB_ERR)	{
			m_pParam->SetUsedValue(Value);
		}
	}
	
	CDialog::OnOK();
}

//----------------------------- FUNCTION -------------------------------------*
void ExtUserParamValues::OnEditchangeExtPrmValues() 
{
	// TODO: Add your control notification handler code here
	if (!m_pParam->IsVariableEnumParam())	{
		if (!IsValidParamValue(FALSE))	{
			m_cboExtPrmValues.SetFocus();
		}
	}
}

//----------------------------- FUNCTION -------------------------------------*
BOOL ExtUserParamValues::IsValidParamValue(BOOL bFinalCheck /*=TRUE*/)
{
	if (m_pParam->IsVariableEnumParam())
	{
		return TRUE;
	}

	CString szNmb;
	m_cboExtPrmValues.GetWindowText(szNmb);
	szNmb.TrimLeft();
	if (szNmb.IsEmpty())
		return FALSE;

	LPTSTR pszNmb = szNmb.GetBuffer(20);
	DWORD dwNmb = _ttol(pszNmb);
	szNmb.ReleaseBuffer();
	CString szTest;
	szTest.Format(_T("%d"), dwNmb);
	if ( (szTest != szNmb) && (bFinalCheck || szNmb != _T("-")) )
	{
		MessageBox(_T("Only numbers are valid!!"),NULL, MB_ICONEXCLAMATION );
		return FALSE;
	}
	BOOL bOutOfRange = FALSE;
	if ((LONG)m_pParam->GetLowerValue() > (LONG)dwNmb)	{
		bOutOfRange = (bFinalCheck || (LONG)dwNmb<=0);
	}
	else if ( (LONG)m_pParam->GetUpperValue() < (LONG)dwNmb)
	{
		bOutOfRange = TRUE;
	}
	else	{
		bOutOfRange = FALSE;
	}

	if (bOutOfRange)	{
		MessageBox(_T("The value is out of range!"),NULL, MB_ICONEXCLAMATION );
		return FALSE;
	}
	return TRUE;
}
