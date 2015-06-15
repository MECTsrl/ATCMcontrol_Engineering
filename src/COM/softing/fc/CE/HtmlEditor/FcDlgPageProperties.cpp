

#include "stdafx.h"
#include "fchtmled.h"
#include "FcHtmlEdDoc.h"
#include "FcHtmlEdView.h"
#include "site.h"
#include "FcHtmlColor.h"

#include <TriEdCID.h>

#include "FcDlgPageProperties.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFcDlgPageProperties dialog


CFcDlgPageProperties::CFcDlgPageProperties(CFcHtmlEdView* pParent /*=NULL*/)
	: CDialog(CFcDlgPageProperties::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFcDlgPageProperties)
	m_BgColor = _T("");
	m_TextColor = _T("");
	m_BgImage = _T("");
	//}}AFX_DATA_INIT
	ASSERT_VALID(pParent);
	m_pParent = pParent;

	m_pHTMLDocument2 = NULL;
	IUnknown* lpUnk = m_pParent->GetTriEditObject();
	ASSERT(lpUnk != NULL);
	if ( lpUnk != NULL )
	{
		// request the "document" object from the object
		HRESULT hr = lpUnk->QueryInterface(IID_IHTMLDocument2, (void **)&m_pHTMLDocument2);
		ASSERT(SUCCEEDED(hr));
		ASSERT(m_pHTMLDocument2 != NULL);
		if (SUCCEEDED(hr))
		{
			
		}
	}

}
CFcDlgPageProperties::~CFcDlgPageProperties()
{
	if (m_pHTMLDocument2 != NULL)
	{
		m_pHTMLDocument2->Release();	
		m_pHTMLDocument2 = NULL;
	}
}


void CFcDlgPageProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFcDlgPageProperties)
	DDX_Control(pDX, IDC_CB_COLOR_BG, m_CbCtrlColorBg);
	DDX_Control(pDX, IDC_CB_COLOR_TEXT, m_CbCtrlColorText);
	DDX_CBString(pDX, IDC_CB_COLOR_BG, m_BgColor);
	DDX_CBString(pDX, IDC_CB_COLOR_TEXT, m_TextColor);
	DDX_Text(pDX, IDC_EDIT_IMAGE_BG, m_BgImage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFcDlgPageProperties, CDialog)
	//{{AFX_MSG_MAP(CFcDlgPageProperties)
	ON_BN_CLICKED(IDC_BTN_COLOR_BG, OnBtnColorBg)
	ON_BN_CLICKED(IDC_BTN_COLOR_TEXT, OnBtnColorText)
	ON_BN_CLICKED(IDC_BTN_IMAGE_BG, OnBtnImageBg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFcDlgPageProperties message handlers


BOOL CFcDlgPageProperties::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_CbCtrlColorBg.Init();
	m_CbCtrlColorText.Init();

	m_BgColor = GetBodyAttribute(CString("bgColor"));
	m_TextColor = GetBodyAttribute(CString("text"));
	m_BgImage = GetBodyAttribute(CString("background"));

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFcDlgPageProperties::OnBtnColorBg() 
{
	SelectCustomColor(m_BgColor);
}

void CFcDlgPageProperties::OnBtnColorText() 
{
	SelectCustomColor(m_TextColor);
}

void CFcDlgPageProperties::OnBtnImageBg() 
{
	CString filter;
	filter.LoadString(ID_IMAGE_FILTER);

	CFileDialog dlg (TRUE, "*.gif", NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter);

	if (dlg.DoModal () == IDOK)
	{
		m_BgImage = dlg.GetPathName(); 
		if (m_BgImage.GetLength() > 0)
		{
			m_BgImage = "file://" + m_BgImage;
			UpdateData(FALSE);
		}
	}
}

void CFcDlgPageProperties::OnOK() 
{
	UpdateData(TRUE);

	SetBodyAttribute(CString("bgColor"), GetColorEntry(m_BgColor));
	SetBodyAttribute(CString("text"), GetColorEntry(m_TextColor));
	SetBodyAttribute(CString("background"), m_BgImage);

	CDialog::OnOK();
}

CString CFcDlgPageProperties::GetColorEntry(const CString& str) 
{
	if (str.GetLength() > 0)
	{
		CString col = FcGetHtmlColorNameFromString(str);
		if (col.GetLength() > 0)
		{
			return col;
		}
		else
		{
			col = FcGetHtmlColorValueFromString(str);
			if (FcCheckHtmlColor(col))
				return col;
		}
	}
	return CString("");
}



CString CFcDlgPageProperties::GetBodyAttribute(const CString& name)
{
	ASSERT(m_pHTMLDocument2 != NULL);	
	IHTMLElement* pBody = NULL;
	m_pHTMLDocument2->get_body(&pBody);
	ASSERT(pBody != NULL);
	if (pBody != NULL)
	{
        CString retval;
		VARIANT va;
		VariantInit(&va);
		BSTR strAttributeName = name.AllocSysString();
		pBody->getAttribute(strAttributeName, 0, &va);
        SysFreeString(strAttributeName);
        if (va.vt == VT_BSTR)
            retval = va.bstrVal;
		VariantClear(&va);
		return retval;
	}
	return CString("");
}

void CFcDlgPageProperties::SetBodyAttribute(const CString& name, const CString& value)
{
	ASSERT(m_pHTMLDocument2 != NULL);	
	IHTMLElement* pBody = NULL;
	m_pHTMLDocument2->get_body(&pBody);
	ASSERT(pBody != NULL);
	if (pBody != NULL)
	{
		BSTR strAttributeName = name.AllocSysString();
		if (value.GetLength() > 0)
		{
			VARIANT va;
			VariantInit(&va);
			va.vt = VT_BSTR;
			va.bstrVal = value.AllocSysString();
			pBody->setAttribute(strAttributeName, va, FALSE);
            VariantClear(&va);
		}
		else
		{
			pBody->removeAttribute(strAttributeName, FALSE, NULL);
		}
        SysFreeString(strAttributeName);
	}
}

void CFcDlgPageProperties::SelectCustomColor(CString& ColEntry) 
{
	CColorDialog* pColorDlg = new CColorDialog;
	if (pColorDlg->DoModal() == IDOK)
	{
		ColEntry = FcColorRef2HtmlColor(pColorDlg->GetColor()); 
		int ColorIndex = FcGetHtmlColorIndex(ColEntry);
		if (ColorIndex >= 0)
			ColEntry = FcGetHtmlColorNameAndValue(ColorIndex);
		UpdateData(FALSE);
	}
	delete pColorDlg;		
}
