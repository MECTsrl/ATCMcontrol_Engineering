

#include "stdafx.h"
#include "GrEditor.h"
#include "InsStepDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInsStepDlg dialog

CInsStepDlg::CInsStepDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInsStepDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInsStepDlg)
	m_strName  = _T("");
	m_bInitial = FALSE;
	m_bCopyDefaultSize = FALSE;
	//}}AFX_DATA_INIT
   m_pNameList = NULL;
}


void CInsStepDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInsStepDlg)
	DDX_CBString(pDX, IDC_GR_STEP_NAME, m_strName);
	DDX_Check(pDX, IDC_GR_STEP_INITIAL, m_bInitial);
	DDX_Check(pDX, IDC_GR_DEFAULT_SIZE, m_bCopyDefaultSize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInsStepDlg, CDialog)
	//{{AFX_MSG_MAP(CInsStepDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInsStepDlg message handlers

BOOL CInsStepDlg::OnInitDialog() 
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


void CInsStepDlg::OnOK() 
{
	// TODO: Add extra validation here

	CDialog::OnOK();
}
