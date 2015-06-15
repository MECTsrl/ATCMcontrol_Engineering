// 
// Stingray Software Extension Classes
// Copyright (C) 1997 Steve Schauer
// Copyright (C) 1997 Stingray Software Inc.
// All Rights Reserved.
// 
// This source code is only intended as a supplement to the
// Stingray Extension Class product.
// See the SEC help files for detailed information
// regarding using SEC classes.
// 
//----------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////
//
// OEPageset.cpp
//
//
// Description: Page format dialog box
//
//

#include "stdafx.h"
#include "secedit.h"
#include "oepageset.h"
#include <malloc.h>

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SECPageSetupDlg dialog

IMPLEMENT_DYNAMIC(OEPageSetupDlg, CDialog)

//@doc OEPageSetupDlg
//@mfunc Constructor for the Objective Edit Page Setup Dialog.
//@parm CWnd* pParent Pointer to the parent window - default is NULL.

OEPageSetupDlg::OEPageSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(OEPageSetupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(OEPageSetupDlg)
	m_iFooterTime = -1;
	m_iHeaderTime = -1;
	//}}AFX_DATA_INIT
}

//@doc OEPageSetupDlg
//@mfunc Internal DoDataExchange handler for OEPageSetupDlg
//@rdesc void 
//@parm CDataExchange* pDX
void OEPageSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(OEPageSetupDlg)
	DDX_Text(pDX, IDC_FOOTER, m_strFooter);
	DDX_Text(pDX, IDC_HEADER, m_strHeader);
	DDX_Radio(pDX, IDC_FOOTER_FILE, m_iFooterTime);
	DDX_Radio(pDX, IDC_HEADER_FILE, m_iHeaderTime);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(OEPageSetupDlg, CDialog)
	//{{AFX_MSG_MAP(OEPageSetupDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// OEPageSetup member functions


static void Replace(CString& strModify, LPCTSTR pszWhat, LPCTSTR pszWith)
	// replace pszWhat with pwzWith in string strModify
{
	int i;
	if ((i = strModify.Find(pszWhat)) >= 0)
	{
		strModify = strModify.Left(i) + pszWith +
			strModify.Mid(i+_tcslen(pszWhat));
	}
}

//@doc OEPageSetupDlg
//@mfunc Internal OEPageSetupDlg member function.
//@rdesc void 
//@parm CString& strFormat
//@parm   LPCTSTR pszFileName
//@parm  UINT nPage
void OEPageSetupDlg::FormatFilePage(CString& strFormat,	 LPCTSTR pszFileName, UINT nPage)
{
	TCHAR sz[32];
	wsprintf(sz, _T("%d"), nPage);
	::Replace(strFormat, _T("&p"), sz);
	if (pszFileName == NULL)
	{
		pszFileName = sz;
		sz[0] = 0;
	}
	::Replace(strFormat, _T("&f"), pszFileName);
}

//@doc OEPageSetupDlg
//@mfunc Internal OEPageSetupDlg member function.
//@rdesc void 
//@parm CString& strHeader
//@parm  CTime& time
//@parm  LPCTSTR pszFileName
//@parm  UINT nPage
void OEPageSetupDlg::FormatHeader(CString& strHeader, CTime& time, LPCTSTR pszFileName, UINT nPage)
{
	CString strFormat = m_strHeader;
	FormatFilePage(strFormat, pszFileName, nPage);
#ifdef _UNICODE
	int nLen = strFormat.GetLength();
	char *pszFormat = (char*)_alloca(nLen+1);
	wcstombs(pszFormat, strFormat, nLen+1);
	strHeader = time.Format(pszFormat);
#else
	strHeader = time.Format(strFormat);
#endif
}

//@doc OEPageSetupDlg
//@mfunc Internal OEPageSetupDlg member function.
//@rdesc void 
//@parm CString& strFooter
//@parm  CTime& time
//@parm LPCTSTR pszFileName
//@parm  UINT nPage
void OEPageSetupDlg::FormatFooter(CString& strFooter, CTime& time,LPCTSTR pszFileName, UINT nPage)
{
	CString strFormat = m_strFooter;
	FormatFilePage(strFormat, pszFileName, nPage);
#ifdef _UNICODE
	int nLen = strFormat.GetLength();
	char *pszFormat = (char*)_alloca(nLen+1);
	wcstombs(pszFormat, strFormat, nLen+1);
	strFooter = time.Format(pszFormat);
#else
	strFooter = time.Format(strFormat);
#endif
}


static TCHAR BASED_CODE szPageSetup[] = _T("PageSetup");
static TCHAR BASED_CODE szHeader[] = _T("Header");
static TCHAR BASED_CODE szFooter[] = _T("Footer");
static TCHAR BASED_CODE szHeaderTime[] = _T("HeaderTime");
static TCHAR BASED_CODE szFooterTime[] = _T("FooterTime");

//@doc OEPageSetupDlg
//@mfunc Internal OEPageSetupDlg member function.
//@rdesc void 
void OEPageSetupDlg::Initialize()
{
	m_strHeader = AfxGetApp()->GetProfileString(szPageSetup, szHeader,
		_T("&f %c"));
	m_strFooter = AfxGetApp()->GetProfileString(szPageSetup, szFooter,
		_T("Page: &p"));
	m_iHeaderTime = AfxGetApp()->GetProfileInt(szPageSetup, szHeaderTime, 0);
	m_iFooterTime = AfxGetApp()->GetProfileInt(szPageSetup, szFooterTime, 0);
	m_strHeaderOld = m_strHeader;
	m_strFooterOld = m_strFooter;
	m_iHeaderTimeOld = m_iHeaderTime;
	m_iFooterTimeOld = m_iFooterTime;
}

//@doc OEPageSetupDlg
//@mfunc Internal OEPageSetupDlg member function.
//@rdesc void 
void OEPageSetupDlg::Terminate()
{
	AfxGetApp()->WriteProfileString(szPageSetup, szHeader, m_strHeader);
	AfxGetApp()->WriteProfileString(szPageSetup, szFooter, m_strFooter);
	AfxGetApp()->WriteProfileInt(szPageSetup, szHeaderTime, m_iHeaderTime);
	AfxGetApp()->WriteProfileInt(szPageSetup, szFooterTime, m_iFooterTime);
}

/////////////////////////////////////////////////////////////////////////////
// OEPageSetupDlg message handlers
