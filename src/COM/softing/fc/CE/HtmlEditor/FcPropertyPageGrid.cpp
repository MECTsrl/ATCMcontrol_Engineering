

#include "stdafx.h"
#include "fchtmled.h"
#include "FcHtmlEdDoc.h"
#include "FcHtmlEdView.h"
#include "FcPropertyPageGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFcPropertyPageGrid property page

IMPLEMENT_DYNCREATE(CFcPropertyPageGrid, CPropertyPage)

CFcPropertyPageGrid::CFcPropertyPageGrid(CFcHtmlEdView* pParent /*=NULL*/) : CPropertyPage(CFcPropertyPageGrid::IDD)
{
	//{{AFX_DATA_INIT(CFcPropertyPageGrid)
	m_Horz = 0;
	m_Vert = 0;
	m_bGridOn = FALSE;
	//}}AFX_DATA_INIT

	ASSERT_VALID(pParent);
	m_pParent = pParent;
}

CFcPropertyPageGrid::~CFcPropertyPageGrid()
{
}

void CFcPropertyPageGrid::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFcPropertyPageGrid)
	DDX_Text(pDX, IDC_GRID_HORZ, m_Horz);
	DDV_MinMaxUInt(pDX, m_Horz, 1, 100);
	DDX_Text(pDX, IDC_GRID_VERT, m_Vert);
	DDV_MinMaxUInt(pDX, m_Vert, 1, 100);
	DDX_Check(pDX, IDC_GRID_ON, m_bGridOn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFcPropertyPageGrid, CPropertyPage)
	//{{AFX_MSG_MAP(CFcPropertyPageGrid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFcPropertyPageGrid message handlers

BOOL CFcPropertyPageGrid::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// get grid settings from option object
	CFcHtmlEdOptions* pOptions =  ((CFcHtmlEdApp*)AfxGetApp())->GetOptions();
	m_Horz = pOptions->GetGridHorizontal();
	m_Vert = pOptions->GetGridVertical();
	m_bGridOn = pOptions->IsGridActive();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFcPropertyPageGrid::OnOK() 
{
	UpdateData(TRUE);

	m_pParent->SetGrid(m_Horz, m_Vert, m_bGridOn);

	CPropertyPage::OnOK();
}

