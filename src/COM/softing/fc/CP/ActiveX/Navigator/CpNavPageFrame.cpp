// FcNavPageFrame.cpp : implementation file
//

#include "stdafx.h"
#include "CpNavigator.h"
#include "CpNavTreeCtrl.h"
#include "CpObjectList.h"
#include "CpNavPageFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCpNavPageFrame
static CString m_ClassName;
CCpNavPageFrame::CCpNavPageFrame()
{
	if (m_ClassName.IsEmpty())
	{
		m_ClassName = AfxRegisterWndClass(0);
	}
}

CCpNavPageFrame::~CCpNavPageFrame()
{
}

//*****************************************************************************
BOOL CCpNavPageFrame::Create(
	CRect& rect,
	CWnd* pParentWnd, 
	UINT nID)
//*****************************************************************************
{
	return CWnd::CreateEx(
		WS_EX_CLIENTEDGE, 
		m_ClassName, 
		NULL,
		WS_CHILD | WS_BORDER, 
		0, 0, rect.Width(), rect.Height(), 
		pParentWnd->m_hWnd, 
		(HMENU)nID);
}

BEGIN_MESSAGE_MAP(CCpNavPageFrame, CWnd)
	//{{AFX_MSG_MAP(CCpNavPageFrame)
    ON_MESSAGE(WM_FCNAV_NEWURL, OnNewURL)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCpNavPageFrame message handlers
/////////////////////////////////////////////////////////////////////////////
// Called by the tree control if a new hyperlink has been selected
//*****************************************************************************	
LRESULT CCpNavPageFrame::OnNewURL(
	WPARAM wParam, 
	LPARAM lParam)
//*****************************************************************************	
{
	GetParent()->SendMessage(WM_FCNAV_NEWURL, wParam, lParam);
	return 0;
}
