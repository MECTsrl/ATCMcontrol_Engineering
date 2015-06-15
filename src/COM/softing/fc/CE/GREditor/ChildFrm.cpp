

#include "stdafx.h"
#include "GrEditor.h"

#include "ChildFrm.h"
#include "GrEditorDoc.h"
#include "STEditorView.h"

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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

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
	// TODO: Add your specialized code here and/or call the base class
#define SPLITTER
#ifdef  SPLITTER
    if(!m_wndSplitter.CreateStatic (this,2,1))
    {
        TRACE(_T("Failed to CreateStaticSplitter\n"));
        return FALSE;
    }
    
    if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CSTEditorView), CSize(35, 130), pContext))
    {
        TRACE0("Failed to create first pane\n");
        return FALSE;
    }

    if (!m_wndSplitter.CreateView(1, 0, pContext->m_pNewViewClass, CSize(200, 150), pContext))
    {
        TRACE0("Failed to create second pane\n");
        return FALSE;
    }   

    return TRUE;
#else
	return CMDIChildWnd::OnCreateClient(lpcs, pContext);
#endif
}
