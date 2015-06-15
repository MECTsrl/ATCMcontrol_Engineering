// SECEditCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "SECEditCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// SECEditCtrl

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(SECEditCtrl, CWnd)

//@doc SECEditCtrl
//@mfunc SECEditCtrl Constructor. Takes no parameters, and initializes m_pSECEdit to NULL.
SECEditCtrl::SECEditCtrl()
{
	m_pSECEdit = NULL;
}

//@doc SECEditCtrl
//@mfunc SECEditCtrl Destructor. Deletes m_pSECEdit if necessary.
SECEditCtrl::~SECEditCtrl()
{
	if(m_pSECEdit)
		delete m_pSECEdit;
}

BEGIN_MESSAGE_MAP(SECEditCtrl, SECEditCore<CWnd>)
	// SECEditCtrl Message Handlers
	//{{AFX_MSG_MAP(SECEditCtrl)
	ON_WM_GETDLGCODE()
	ON_WM_MOUSEACTIVATE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	SECEDITCORE_MESSAGE_LIST()
END_MESSAGE_MAP()


//@doc SECEditCtrl
//@mfunc This function should be called in the OnInitDialog handler
//for the dialog in which you want to place the SECEditCtrl.
//Note that this function destroys the window and replaces it, rather than
//just subclassing it. If you are replacing an Edit Control, you may use
//class wizard to map EN_SETFOCUS and EN_KILLFOCUS, and these notifications
//will be sent.
//@rdesc BOOL 
//@parm The ID of the control to replace
//@parm The parent window of SECEditCtrl
//@parm Extended style of the window (defaults to WS_EX_CLIENTEDGE)
//@comm dwExStyle defaults to WS_EX_CLIENTEDGE.
BOOL SECEditCtrl::Initialize(UINT nID, CWnd * pParentWnd, DWORD dwExStyle)
{
	CRect rect;
	CWnd* pWnd = pParentWnd->GetDlgItem(nID);
	pWnd->GetWindowRect(&rect);
	pParentWnd->ScreenToClient(&rect);

	// If creating dynamically, model your create call after this one
	BOOL bCreated = Create(NULL, _T("SECEdit"), WS_VISIBLE | WS_CHILD | WS_TABSTOP, 
						   rect, pParentWnd, nID);

	ASSERT(bCreated);
	if(!bCreated)
		return FALSE;

	// Set the Z-Order so this new control keeps the same tab order as the
	// control it is replacing
	SetWindowPos(pWnd, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOREDRAW | SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);

	// Destroy the control being replaced
	pWnd->DestroyWindow();

	// Add 3D border to control
	ModifyStyleEx(NULL, dwExStyle, SWP_FRAMECHANGED);

	return bCreated;
}


//@doc SECEditCtrl
//@mfunc Returns the SECEdit* that this edit window is using internally.
//SECEditCtrl maintains this member internally (unlike SECEditView) and
//you don't need to override this function, but you may if you wish to return
//a different SECEdit*.
//@rdesc SECEdit*
SECEdit* SECEditCtrl::GetEdit()
{
	if(m_pSECEdit == NULL)
	{
		// Has not yet been created so create one
		// for this SECEditCtrl via a virtual function
		// that may be overridden
		CreateNewEdit();
	}

	return m_pSECEdit;
}

//@doc SECEditCtrl
//@mfunc Creates the SECEdit member that is maintained by SECEditCtrl.
//Override this function and do nothing if you wish to maintain your
//own SECEdit pointer.
//@rdesc void 
void SECEditCtrl::CreateNewEdit()
{
	// This function creates a new SECEdit and assigns
	// it to m_pSECEdit. Override this function and set
	// m_pSECEdit to your provided SECEdit if desired, 
	// or also override GetEdit to do your own custom processing
	if(m_pSECEdit)
		return;

	m_pSECEdit = new SECEdit;
}


/////////////////////////////////////////////////////////////////////////////
// SECEditCtrl message handlers

UINT SECEditCtrl::OnGetDlgCode() 
{
	// Must return this so we get messages
	return DLGC_WANTMESSAGE;
}

int SECEditCtrl::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 
{
	int nMouseActivate = SECEditCore<CWnd>::OnMouseActivate(pDesktopWnd, nHitTest, message);

	if(nMouseActivate == MA_ACTIVATE || nMouseActivate == MA_ACTIVATEANDEAT && GetFocus() != this)
		SetFocus();

	return nMouseActivate;
}

//@doc SECEditCtrl
//@mfunc todo
//@rdesc void 
//@parm CWnd* pOldWnd
void SECEditCtrl::OnSetFocus(CWnd* pOldWnd) 
{
	SECEditCore<CWnd>::OnSetFocus(pOldWnd);

	// Send EN_SETFOCUS to the parent so developers can
	// Use class wizard to map this message
	CWnd* pParent = GetParent();
	ASSERT_VALID(pParent);

	WPARAM wParam = MAKEWPARAM(GetDlgCtrlID(), EN_SETFOCUS);
	LPARAM lParam = (LPARAM)GetSafeHwnd();

	pParent->SendMessage(WM_COMMAND, wParam, lParam);	
}

//@doc SECEditCtrl
//@mfunc todo
//@rdesc void 
//@parm CWnd* pNewWnd
void SECEditCtrl::OnKillFocus(CWnd* pNewWnd) 
{
	SECEditCore<CWnd>::OnKillFocus(pNewWnd);

	// Send EN_KILLFOCUS to parent when we lose focus
	CWnd* pParent = GetParent();
	ASSERT_VALID(pParent);

	WPARAM wParam = MAKEWPARAM(GetDlgCtrlID(), EN_KILLFOCUS);
	LPARAM lParam = (LPARAM)GetSafeHwnd();

	pParent->SendMessage(WM_COMMAND, wParam, lParam);
}

int SECEditCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (SECEditCore<CWnd>::OnCreate(lpCreateStruct) == -1)
		return -1;

	RegisterDropTarget();

	InitView();

	// These lines tweak the Vertical Scroll Bar 
	// and makes it appear properly
	MySetScrollRange(SB_VERT, 0, 32000, TRUE); 
	MySetScrollRange(SB_VERT, 0, 0,TRUE); 

	return 0;
}

//@doc SECEditCtrl
//@mfunc Resgisters the custom drop target for SECEditCtrl. Override this
//function and do nothing if you don't want SECEditCtrl to be a drop target.
//@rdesc void 
void SECEditCtrl::RegisterDropTarget()
{
	// Override and do nothing to not
	// Have this be a drop target
	m_dt.Register(this);

}

#ifndef ORIGINAL_VERSION
void SECEditCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	SECEditCore<CWnd>::OnChar(nChar, nRepCnt, nFlags);

	nFlags; //UNUSED
	nRepCnt;//UNUSED
	switch (nChar)
   {
		case VK_ESCAPE:
         CWnd* wnd = GetParent();
         if (wnd)
         {
            wnd->PostMessage(WM_USER, (WPARAM)nChar, (LPARAM)this);
         }
         break;

	}
}
#endif
