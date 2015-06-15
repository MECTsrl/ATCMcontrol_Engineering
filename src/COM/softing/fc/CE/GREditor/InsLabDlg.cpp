

#include "stdafx.h"
#include "GrEditor.h"
#include "InsLabDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInsLabDlg dialog

CInsLabDlg::CInsLabDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInsLabDlg::IDD, pParent)
{
   int  i;

	//{{AFX_DATA_INIT(CInsLabDlg)
	m_strName = _T("");
	m_iLabTyp = 0;
	m_bLine = FALSE;
	//}}AFX_DATA_INIT
   for( i=0; i<(sizeof(m_Prev)/sizeof(m_Prev[0])); i++)
   {
      m_Prev[i].LoadBitmap( IDB_GR_LAB_PREV01+i );
   }
   m_pNameList = NULL;
}


void CInsLabDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInsLabDlg)
	DDX_CBString(pDX, IDC_GR_LAB_NAME, m_strName);
	DDX_Radio(pDX, IDC_GR_LAB_SRC, m_iLabTyp);
	DDX_Check(pDX, IDC_GR_LAB_LINE, m_bLine);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInsLabDlg, CDialog)
	//{{AFX_MSG_MAP(CInsLabDlg)
	ON_BN_CLICKED(IDC_GR_LAB_SRC, OnLabChanged)
	ON_BN_CLICKED(IDC_GR_LAB_DEST, OnLabChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInsLabDlg message handlers

BOOL CInsLabDlg::OnInitDialog() 
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
   // display the bitmap
   OnLabChanged();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInsLabDlg::OnLabChanged() 
{
   int  i;

	i = ( ((CButton*)GetDlgItem(IDC_GR_LAB_SRC))->GetCheck() == 1) ? 0 : 1;
   ((CStatic*)GetDlgItem(IDC_GR_LAB_PREV))->SetBitmap( m_Prev[i] );
   if( i == 0 )
   {
      ((CButton*)GetDlgItem(IDC_GR_LAB_LINE))->EnableWindow(FALSE);
   } else {
      ((CButton*)GetDlgItem(IDC_GR_LAB_LINE))->EnableWindow(TRUE);
   }
}

void CInsLabDlg::OnOK() 
{
	// TODO: Add extra validation here

	CDialog::OnOK();
}

