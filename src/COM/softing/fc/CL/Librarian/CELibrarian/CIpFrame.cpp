/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CIpFrame.cpp 1     28.02.07 18:59 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CIpFrame.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CIpFrame.cpp $
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
#include "CIpFrame.h"
//----  Static Initializations:   ----------------------------------*

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInPlaceFrame

IMPLEMENT_DYNCREATE(CInPlaceFrame, COleDocIPFrameWnd)

BEGIN_MESSAGE_MAP(CInPlaceFrame, COleDocIPFrameWnd)
	//{{AFX_MSG_MAP(CInPlaceFrame)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CInPlaceFrame::CInPlaceFrame()
{
}

CInPlaceFrame::~CInPlaceFrame()
{
}

int CInPlaceFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COleDocIPFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// CResizeBar implementiert direkte Größenänderungen.
	if (!m_wndResizeBar.Create(this))
	{
		TRACE0("Failed to create resize bar\n");
		return -1;      // Fehler beim Erzeugen
	}

	// Allgemein ist es von Vorteil, ein Drop-Ziel zu registrieren, das
	//  keinerlei Auswirkung auf Ihr Rahmenfenster hat. Dies verhindert bei Drops ("Abwürfen")
	// ein "Durchfallen" zu einem Container, der Drag and Drop unterstützt.
	m_dropTarget.Register(this);

	return 0;
}

// OnCreateControlBars wird automatisch aufgerufen, um Steuerleisten im Fenster der
//  Container-Anwendung zu erstellen. pWndFrame ist das hierarchisch höchste Rahmenfenster des
//  Containers und ist immer ungleich NULL. pWndDoc ist das Rahmenfenster auf Dokumentebene
//  und ist gleich NULL, wenn der Container eine SDI-Anwendung ist. Eine Server
//  Anwendung kann MFC-Steuerelementleisten in jedes Fenster platzieren.
BOOL CInPlaceFrame::OnCreateControlBars(CFrameWnd* pWndFrame, CFrameWnd* pWndDoc)
{
	// Entfernen Sie dies, wenn Sie pWndDoc verwenden
	UNREFERENCED_PARAMETER(pWndDoc);

#if _MFC_VER == 0x0421
#else
	// Set owner to this window, so messages are delivered to correct app
	m_wndReBar.SetOwner (this);

	// Create rebar on client's frame window
	if (!m_wndReBar.Create (pWndDoc, RBS_AUTOSIZE | RBS_BANDBORDERS))
	{
		TRACE0 ("Failed to create rebar\n");
		return (FALSE);
	};
#endif

	return TRUE;
}

BOOL CInPlaceFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// ZU ERLEDIGEN: Ändern Sie hier die Fensterklasse oder das Erscheinungsbild, indem Sie
	//  CREATESTRUCT cs modifizieren.

	return COleDocIPFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CInPlaceFrame Diagnose

#ifdef _DEBUG
void CInPlaceFrame::AssertValid() const
{
	COleDocIPFrameWnd::AssertValid();
}

void CInPlaceFrame::Dump(CDumpContext& dc) const
{
	COleDocIPFrameWnd::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CInPlaceFrame-Befehle


/*
 *----------------------------------------------------------------------------*
 *  $History: CIpFrame.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 18:59
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:31
 * Created in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 * 
 * *****************  Version 4  *****************
 * User: Oh           Date: 10/12/01   Time: 10:01a
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 * 
 * *****************  Version 3  *****************
 * User: Oh           Date: 7/13/01    Time: 10:29a
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 * 
 * *****************  Version 2  *****************
 * User: Oh           Date: 6/25/01    Time: 3:32p
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 *==
 *----------------------------------------------------------------------------*
*/
