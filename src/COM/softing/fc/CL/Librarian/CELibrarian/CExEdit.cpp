/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CExEdit.cpp 1     28.02.07 18:59 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CExEdit.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CExEdit.cpp $
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
#include "CExEdit.h"
//----  Static Initializations:   ----------------------------------*


// CExEdit.cpp: Implementierungsdatei
//

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExEdit

CExEdit::CExEdit()
{
	m_phbr = new CBrush(0x00ffffff);
}

CExEdit::~CExEdit()
{
	if ( m_phbr != 0)
	{
		delete m_phbr;
		m_phbr = 0;
	}
}


BEGIN_MESSAGE_MAP(CExEdit, CEdit)
	//{{AFX_MSG_MAP(CExEdit)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_CHAR()
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CExEdit 

HBRUSH CExEdit::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// TODO: Attribute des Gerätekontexts hier ändern
	
	// TODO: Pinsel ungleich NULL zurückgeben, falls die Behandlungsroutine des übergeordneten nicht aufgerufen werden soll
	//pDC->SetBkMode(TRANSPARENT);
	//CBrush* m_phbr = new CBrush(0x00ffffff);
	pDC->SetBkColor(0x00ffffff);
	return (HBRUSH) m_phbr->GetSafeHandle();
}

void CExEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Code für die Behandlungsroutine für Nachrichten hier einfügen und/oder Standard aufrufen
	
	//CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CExEdit::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Code für die Behandlungsroutine für Nachrichten hier einfügen
	
}

BOOL CExEdit::DestroyWindow() 
{
	// TODO: Speziellen Code hier einfügen und/oder Basisklasse aufrufen
	if ( m_phbr != 0)
	{
		delete m_phbr;
		m_phbr = 0;
	}
	return CEdit::DestroyWindow();
}


/*
 *----------------------------------------------------------------------------*
 *  $History: CExEdit.cpp $
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
