

#include "stdafx.h"
#include "GrEditor.h"
#include "InsTranDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInsTranDlg dialog

CInsTranDlg::CInsTranDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInsTranDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInsTranDlg)
	m_strName = _T("");
	m_bSetDefaultSize = FALSE;
	//}}AFX_DATA_INIT
   m_pNameList = NULL;
}

void CInsTranDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInsTranDlg)
	DDX_CBString(pDX, IDC_GR_TRAN_NAME, m_strName);
	DDX_Check(pDX, IDC_GR_DEFAULT_SIZE, m_bSetDefaultSize);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CInsTranDlg, CDialog)
	//{{AFX_MSG_MAP(CInsTranDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInsTranDlg message handlers

BOOL CInsTranDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	POSITION pos;
   int      iLoc;

   // fill the comboboxes ..
	if (m_pNameList != NULL)
	{
	   for (pos = m_pNameList->GetHeadPosition(); pos != NULL; )
		{
			NameBox().AddString(m_pNameList->GetNext(pos));
		}
      // and set the selection to the DDX variable value			
      iLoc = NameBox().FindStringExact(0, m_strName);
      if( iLoc == CB_ERR ) iLoc = 0;
		NameBox().SetCurSel( iLoc );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CInsTranDlg::OnOK() 
{
	// TODO: Add extra validation here

	CDialog::OnOK();
}
