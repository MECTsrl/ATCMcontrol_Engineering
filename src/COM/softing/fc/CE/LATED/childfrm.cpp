//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "LATED.h"
#include "childfrm.h"
#include "latedview.h"
#include "latedlist.h"
//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame() : m_wndSplitter()
{
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// Modify the Window class or styles here by modifying
	// the CREATESTRUCT cs

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// Add your specialized code here and/or call the base class
    CRect rect;

#ifdef LATED_SPLITTER
    if(!m_wndSplitter.CreateStatic (this,1,2))
    {
        TRACE(_T("Failed to CreateStaticSplitter\n"));
        return FALSE;
    }

    GetClientRect(&rect);
    CSize SizeWindow = rect.Size();
    
    if (!m_wndSplitter.CreateView(0, 0, pContext->m_pNewViewClass, 
        CSize((SizeWindow.cx/2),SizeWindow.cy), pContext))
    {
        TRACE0("Failed to create first pane\n");
        return FALSE;
    }   

    if (!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CLATEDList), 
        CSize((SizeWindow.cx/2),SizeWindow.cy), pContext))
    {
        TRACE0("Failed to create second pane\n");
        return FALSE;
    }
    return TRUE;
#else 
    return CMDIChildWnd::OnCreateClient(lpcs, pContext);
#endif
}