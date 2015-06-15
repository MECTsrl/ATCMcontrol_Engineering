/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/FBStextDlg.cpp 1     28.02.07 18:59 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: FBStextDlg.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/FBStextDlg.cpp $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                4CL
 *
 * =COMPONENT           CELibrarian
 *
 * =CURRENT      $Date: 28.02.07 18:59 $
 *               $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *





 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  6/25/2001  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/



//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "CELibrarian.h"
#include "FBStextDlg.h"
//----  Static Initializations:   ----------------------------------*


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld CFBStextDlg 


CFBStextDlg::CFBStextDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFBStextDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFBStextDlg)
	m_stEdit = _T("");
	//}}AFX_DATA_INIT
	m_init = 0;
	m_pEditBkBrush = new CBrush(RGB(255,255,255));
}


void CFBStextDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFBStextDlg)
	DDX_Control(pDX, IDC_CL_FBSTVIEW_EDIT, m_dlgEdit);
	DDX_Text(pDX, IDC_CL_FBSTVIEW_EDIT, m_stEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFBStextDlg, CDialog)
	//{{AFX_MSG_MAP(CFBStextDlg)
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CFBStextDlg 

void CFBStextDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// Edit Control der Dialoggröße anpassen
	if (m_init)
	{
		m_dlgEdit.SetWindowPos(NULL, 0, 0, cx-4, cy-4, SWP_NOMOVE);
		m_dlgEdit.SetSel(-1,0);
	}
	m_init = 1;

}

void CFBStextDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	m_dlgEdit.SetSel(-1,0);
	
}

HBRUSH CFBStextDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	if (pWnd == &m_dlgEdit && pWnd != NULL)
	{
		pDC->SetTextColor(RGB(0,0,0));
		pDC->SetBkColor(RGB(255,255,255));
		return (HBRUSH) (m_pEditBkBrush->GetSafeHandle());
	}
	else
		return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CFBStextDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	delete m_pEditBkBrush;	
}


/*
 *----------------------------------------------------------------------------*
 *  $History: FBStextDlg.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 18:59
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:31
 * Created in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 * 
 * *****************  Version 2  *****************
 * User: Oh           Date: 6/25/01    Time: 3:32p
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 *==
 *----------------------------------------------------------------------------*
*/
