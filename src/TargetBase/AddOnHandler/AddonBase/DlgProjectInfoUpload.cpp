/* ----  Includes:	 ---------------------------------------------------------- */

#include "stdafx.h"
#include "addonbaseresource.h"
#include "DlgProjectInfoUpload.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgProjectInfoUpload dialog


CDlgProjectInfoUpload::CDlgProjectInfoUpload(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgProjectInfoUpload::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgProjectInfoUpload)
	m_strCreated = _T("");
	m_strModified = _T("");
	m_strName = _T("");
	m_strOwner = _T("");
	m_strText1 = _T("");
	m_strText2 = _T("");
	m_strText3 = _T("");
	m_strVersion = _T("");
	//}}AFX_DATA_INIT
}


void CDlgProjectInfoUpload::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgProjectInfoUpload)
	DDX_Text(pDX, IDC_EDIT_PI_CREATED, m_strCreated);
	DDX_Text(pDX, IDC_EDIT_PI_MOD, m_strModified);
	DDX_Text(pDX, IDC_EDIT_PI_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_PI_OWNER, m_strOwner);
	DDX_Text(pDX, IDC_EDIT_PI_TEXT1, m_strText1);
	DDX_Text(pDX, IDC_EDIT_PI_TEXT2, m_strText2);
	DDX_Text(pDX, IDC_EDIT_PI_TEXT3, m_strText3);
	DDX_Text(pDX, IDC_EDIT_PI_VERSION, m_strVersion);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgProjectInfoUpload, CDialog)
	//{{AFX_MSG_MAP(CDlgProjectInfoUpload)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/* ---------------------------------------------------------------------------- */
