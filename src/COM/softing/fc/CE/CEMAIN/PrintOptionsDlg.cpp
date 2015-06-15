
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*

#include "stdafx.h"
#include "resource.h"
#include "PrintOptionsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrintOptionsDlg dialog


CPrintOptionsDlg::CPrintOptionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPrintOptionsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPrintOptionsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPrintOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrintOptionsDlg)
	DDX_Control(pDX, IDC_PO_MODULE_COMBO, m_tModuleCombo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPrintOptionsDlg, CDialog)
	//{{AFX_MSG_MAP(CPrintOptionsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrintOptionsDlg message handlers

BOOL CPrintOptionsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

    FillCombo();

    m_tModuleCombo.SetCurSel(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//------------------------------------------------------------------*
/**
 * set print modules.
 *
 * @param           rastrPrintModules: string array of module names
 * @return          -
 * @exception       -
 * @see             -
*/
void CPrintOptionsDlg::SetPrintModules(CStringArray& rastrPrintModules)
{
    m_astrModules.Copy(rastrPrintModules);
}

//------------------------------------------------------------------*
/**
 * on ok.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CPrintOptionsDlg::OnOK() 
{
	CDialog::OnOK();
    m_tModuleCombo.GetWindowText(m_strModule);
}


//------------------------------------------------------------------*
/**
 * get print module.
 *
 *  return currently selected module.
 *
 * @param           -
 * @return          module name of currently selected module.
 * @exception       -
 * @see             -
*/
CString CPrintOptionsDlg::GetPrintModule()
{
    return m_strModule;
}

//------------------------------------------------------------------*
/**
 * fill combo.
 *
 *  fills combo box with names from m_astrModules
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CPrintOptionsDlg::FillCombo()
{
    int iNumModules = m_astrModules.GetSize();
    for(int iModule = 0; iModule < iNumModules; ++iModule)
    {
        m_tModuleCombo.AddString(m_astrModules[iModule]);
    }
}
