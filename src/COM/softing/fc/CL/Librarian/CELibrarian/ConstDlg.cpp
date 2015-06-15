/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/ConstDlg.cpp 1     28.02.07 18:59 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: ConstDlg.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/ConstDlg.cpp $
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
#include "celibrarian.h"
#include "ConstDlg.h"
//#include "CELibrary.h" //OH: was 4cdll

//----  Static Initializations:   ----------------------------------*



// ConstDlg.cpp: Implementierungsdatei
//


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld ConstDlg 
void AFXAPI DDX_List(CDataExchange* pDX, int nIDC, CListCtrlEx& m_listCtrl, CELibrarianDoc* pDocument);

ConstDlg::ConstDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ConstDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(ConstDlg)
		// HINWEIS: Der Klassen-Assistent fügt hier Elementinitialisierung ein
	//}}AFX_DATA_INIT
	m_init=0;
}


void ConstDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ConstDlg)
	DDX_Control(pDX, IDC_CL_LIST1, m_listCtrl);
	DDX_List(pDX, IDC_CL_LIST1, m_listCtrl, m_pDocument);
	//}}AFX_DATA_MAP
}

 
BEGIN_MESSAGE_MAP(ConstDlg, CDialog)
	//{{AFX_MSG_MAP(ConstDlg)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten ConstDlg 

BOOL ConstDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX-Eigenschaftenseiten sollten FALSE zurückgeben
}

void AFXAPI DDX_List(CDataExchange* pDX, int nIDC, CListCtrlEx& m_listCtrl, CELibrarianDoc* pDocument)
{
	if (pDocument == 0)
		return;
	if ( pDX->m_bSaveAndValidate )
	{
	}
	else
	{
		BOOL brv=m_listCtrl.InsertColumn(0,"Constant",LVCFMT_LEFT,200,0);
		brv=m_listCtrl.InsertColumn(1,"Type",LVCFMT_LEFT,150,1);
		brv=m_listCtrl.InsertColumn(2,"Value",LVCFMT_LEFT,222,2);
		/*TCHAR pszConstName[MAX_PATH], pszConstType[MAX_PATH], pszConstValue[MAX_PATH];
		CELibrary* pCELibrary=pDocument->GetCELibrary();
		int nn = pCELibrary->GetConstCount();
		for ( int ii=0; ii<nn; ii++ )
		{
			int rv = pCELibrary->GetConst(ii,pszConstName, pszConstType, pszConstValue);
			BOOL brv=m_listCtrl.AddItem(ii,0,(LPCTSTR) pszConstName);
			brv=m_listCtrl.AddItem(ii,1,(LPCTSTR) pszConstType);
			brv=m_listCtrl.AddItem(ii,2,(LPCTSTR) pszConstValue);
		}*/
	}
}

void ConstDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if (m_init)
	{
		m_listCtrl.SetWindowPos(NULL, 0, 0, cx-24, cy-24, SWP_NOMOVE);
		//
		// set size of third row
		//
		int cx01 = m_listCtrl.GetColumnWidth(0) + m_listCtrl.GetColumnWidth(1);
		BOOL rv=m_listCtrl.SetColumnWidth(2,cx-24-cx01);
	}
	m_init = 1;	

}


/*
 *----------------------------------------------------------------------------*
 *  $History: ConstDlg.cpp $
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
