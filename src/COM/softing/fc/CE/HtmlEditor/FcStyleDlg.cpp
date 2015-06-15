

#include "stdafx.h"
#include "fchtmled.h"
#include "FcHtmlEdDoc.h"
#include "FcHtmlEdView.h"

#include <TriEdCID.h>

#include "FcStyleDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFcDlgStyle dialog


CFcDlgStyle::CFcDlgStyle(CFcHtmlEdView* pParent /*=NULL*/)
	: CDialog(CFcDlgStyle::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFcDlgStyle)
	m_Align = -1;
	m_Style = _T("");
	//}}AFX_DATA_INIT

	ASSERT_VALID(pParent);
	m_pParent = pParent;
}


void CFcDlgStyle::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFcDlgStyle)
	DDX_Control(pDX, IDC_STYLE_LIST, m_StyleList);
	DDX_CBIndex(pDX, IDC_CB_ALIGN, m_Align);
	DDX_LBString(pDX, IDC_STYLE_LIST, m_Style);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFcDlgStyle, CDialog)
	//{{AFX_MSG_MAP(CFcDlgStyle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFcDlgStyle message handlers

BOOL CFcDlgStyle::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// fill Listbox with styles
	VARIANTARG va;
	VariantInit(&va);
	va.vt = VT_ARRAY;
	va.parray = NULL;
	HRESULT hr = m_pParent->ExecTriEditCommand(IDM_TRIED_GETBLOCKFMTS, NULL, &va, FALSE);
	if (SUCCEEDED(hr))
	{
		LONG iLBound, iUBound;
		SAFEARRAY* psa = V_ARRAY(&va);
		SafeArrayGetLBound(psa, 1, &iLBound);
		SafeArrayGetUBound(psa, 1, &iUBound);

		BSTR  bstr = NULL;
		for (LONG ii = iLBound; ii <= iUBound; ii++)
		{
			SafeArrayGetElement(psa, &ii, &bstr);
			if(bstr != NULL)
			{
				m_StyleList.AddString(CString(bstr));
				SysFreeString(bstr);
			}
		}
		//SafeArrayDestroy(psa);
	}
	VariantClear(&va);

	// select the currently used style
	if (SUCCEEDED(hr))
	{
		VariantInit(&va);
		va.vt = VT_BSTR;
		va.bstrVal = NULL;

		hr = m_pParent->ExecTriEditCommand(IDM_TRIED_BLOCKFMT, NULL, &va, FALSE);
		if (SUCCEEDED(hr) && va.bstrVal != NULL)
		{
			m_Style = CString(va.bstrVal);
			SysFreeString(va.bstrVal);
		}
	}

	// select currently used alignment
	if (m_pParent->QueryTriEditCommand(IDM_TRIED_JUSTIFYLEFT) & OLECMDF_LATCHED)
		m_Align = 1;
	else if (m_pParent->QueryTriEditCommand(IDM_TRIED_JUSTIFYCENTER) & OLECMDF_LATCHED)
		m_Align = 2;
	else if (m_pParent->QueryTriEditCommand(IDM_TRIED_JUSTIFYRIGHT) & OLECMDF_LATCHED)
		m_Align = 3;
	else
		m_Align = 0;

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CFcDlgStyle::OnOK() 
{
	UpdateData(TRUE);

	switch (m_Align)
	{
	case 1:
		m_pParent->ExecTriEditCommand(IDM_TRIED_JUSTIFYLEFT, NULL, NULL, FALSE);
		break;
	case 2:
		m_pParent->ExecTriEditCommand(IDM_TRIED_JUSTIFYCENTER, NULL, NULL, FALSE);
		break;
	case 3:
		m_pParent->ExecTriEditCommand(IDM_TRIED_JUSTIFYRIGHT, NULL, NULL, FALSE);
		break;
	}

	if (m_Style.GetLength() > 0)
	{
		VARIANT va;
		VariantInit(&va);
		va.vt = VT_BSTR;
		va.bstrVal = m_Style.AllocSysString();
		m_pParent->ExecTriEditCommand(IDM_TRIED_BLOCKFMT, &va, NULL, FALSE);
        VariantClear(&va);
	}

	CDialog::OnOK();
}
