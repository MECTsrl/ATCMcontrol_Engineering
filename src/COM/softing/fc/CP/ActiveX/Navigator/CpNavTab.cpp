// FcNavTab.cpp : implementation file
//

#include "stdafx.h"
#include "CpNavigator.h"
#include "CpNavTab.h"
#include "CpObjectList.h"
#include "CpNavTreeCtrl.h"
#include "CpNavPageFrame.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCpNavTab

CCpNavTab::CCpNavTab(BOOL b3DFrame /*=TRUE*/)
{
	if (b3DFrame)
		m_p3DFrame = new CCpNavPageFrame;
	else
		m_p3DFrame = NULL;
}

CCpNavTab::~CCpNavTab()
{
	if (m_p3DFrame != NULL)
		delete m_p3DFrame;
}


BEGIN_MESSAGE_MAP(CCpNavTab, CTabCtrl)
	//{{AFX_MSG_MAP(CCpNavTab)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelChange)
    ON_MESSAGE(WM_FCNAV_NEWURL, OnNewURL)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//*****************************************************************************	
void CCpNavTab::MoveWindow( 
	LPCRECT lpRect, 
	BOOL bRepaint/*=TRUE*/)
//*****************************************************************************	
{
	ASSERT_VALID(this);
	if (IsWindow(m_hWnd))
	{
		CTabCtrl::MoveWindow(lpRect, bRepaint);
		AdjustChildren(bRepaint);
	}
}

//*****************************************************************************	
void CCpNavTab::MoveWindow( 
	int x, int y, 
	int nWidth, int nHeight, 
	BOOL bRepaint/*=TRUE*/)
//*****************************************************************************	
{
	ASSERT_VALID(this);
	if (IsWindow(m_hWnd))
	{
		CTabCtrl::MoveWindow(x, y, nWidth, nHeight, bRepaint);
		AdjustChildren(bRepaint);
	}
}

//*****************************************************************************	
BOOL CCpNavTab::SetWindowPos( 
	const CWnd* pWndInsertAfter, 
	int x, int y, 
	int cx, int cy, 
	UINT nFlags )
//*****************************************************************************	
{
	ASSERT_VALID(this);
	if (IsWindow(m_hWnd))
	{
		BOOL rv = CTabCtrl::SetWindowPos(pWndInsertAfter, x, y, cx, cy, nFlags);
		AdjustChildren((nFlags & SWP_NOREDRAW) != 0);	
		return rv;
	}
	return FALSE;
}


//*****************************************************************************	
BOOL CCpNavTab::AddItem(
	CCpNavTreeCtrl *pTreeCtrl,
	unsigned StringId)
//*****************************************************************************	
{
	CString s;
	s.LoadString(StringId);
	return AddItem(pTreeCtrl, s);
}

//*****************************************************************************	
BOOL CCpNavTab::AddItem(
	CCpNavTreeCtrl *pTreeCtrl,
	const CString& TabText)
//*****************************************************************************	
{
	ASSERT_VALID(this);
	ASSERT(IsWindow(m_hWnd));
	ASSERT_VALID(pTreeCtrl);

	if (IsWindow(m_hWnd))
	{
		// insert item in tab control
		char Text[64];
		strncpy(Text, TabText, sizeof(Text));
		TC_ITEM ts; 
		memset(&ts, 0, sizeof(ts));
		ts.mask = TCIF_TEXT;
		ts.cchTextMax = strlen(Text);;
		ts.pszText = Text;
		VERIFY(CTabCtrl::InsertItem(GetItemCount(), &ts) >= 0);

		// calculate child area
		CRect Rect;		
		GetClientRect(Rect);
		AdjustRect(FALSE, Rect);
		
		// create 3D frame
		CWnd *pCtrlParent = this;
		if (m_p3DFrame != NULL)
		{
			if (!IsWindow(m_p3DFrame->m_hWnd))
			{
				VERIFY(m_p3DFrame->Create(Rect, this, 2));
				m_p3DFrame->EnableWindow(TRUE);
				m_p3DFrame->ShowWindow(SW_SHOW);
			}
			m_p3DFrame->GetClientRect(Rect);
			pCtrlParent = m_p3DFrame;
		}

		VERIFY(pTreeCtrl->Create(Rect, pCtrlParent, 1));
		if (GetItemCount() == 1)
		{
			pTreeCtrl->EnableWindow(TRUE);
			pTreeCtrl->ShowWindow(SW_SHOW);
		}
		else
		{
			pTreeCtrl->ShowWindow(SW_HIDE);
			pTreeCtrl->EnableWindow(FALSE);
		}
		
		m_PageArray.Add(pTreeCtrl);

		if (GetItemCount() == 1)
			SetFont(pTreeCtrl->GetFont());
	}
	return 0;
}

//*****************************************************************************	
CCpNavTreeCtrl* CCpNavTab::GetItem(
	int nIndex)
//*****************************************************************************	
{
	ASSERT_VALID(this);
	ASSERT(IsWindow(m_hWnd));
	ASSERT(nIndex >= 0 && nIndex < GetItemCount());
	ASSERT_VALID(m_PageArray.GetAt(nIndex));	
	return m_PageArray.GetAt(nIndex);
}

//*****************************************************************************	
BOOL CCpNavTab::DeleteItem(
	int nIndex)
//*****************************************************************************	
{
	ASSERT_VALID(this);
	ASSERT(IsWindow(m_hWnd));
	ASSERT(nIndex >= 0 && nIndex < GetItemCount());
	ASSERT_VALID(m_PageArray.GetAt(nIndex));	

	CCpNavTreeCtrl *pPage = m_PageArray.GetAt(nIndex);
	ASSERT_VALID(pPage);
	pPage->DestroyWindow();
	m_PageArray.RemoveAt(nIndex);		
	BOOL rv = CTabCtrl::DeleteItem(nIndex);
	if (GetItemCount() > 0)
	{
		CTabCtrl::SetCurSel(0);
		UpdateSelection(CTabCtrl::GetCurSel());
	}

	return rv;
}

//*****************************************************************************	
BOOL CCpNavTab::DeleteAllItems()
//*****************************************************************************	
{
	ASSERT_VALID(this);
	ASSERT(IsWindow(m_hWnd));
	for (int ii = GetItemCount()-1; ii >= 0; ii--)
	{
		CCpNavTreeCtrl *pPage = m_PageArray.GetAt(ii);
		if (pPage != NULL)
		{
			ASSERT_VALID(pPage);
			pPage->DestroyWindow();
		}
	}
	m_PageArray.RemoveAll();
	return CTabCtrl::DeleteAllItems();
}

//*****************************************************************************	
void CCpNavTab::UpdateSelection(int nIndex) 
//*****************************************************************************	
{
	ASSERT_VALID(this);
	ASSERT(IsWindow(m_hWnd));
	ASSERT(nIndex >= 0 && nIndex < GetItemCount());
	for (int ii = 0; ii < GetItemCount(); ii++)
	{
		CCpNavTreeCtrl *pPage = m_PageArray.GetAt(ii);			
		ASSERT_VALID(pPage);
		if (ii != nIndex)
		{
			if (pPage->IsWindowVisible())
			{
				pPage->ShowWindow(SW_HIDE);
				pPage->EnableWindow(FALSE);
			}
		}
		else
		{
			if (!pPage->IsWindowVisible())
			{
				pPage->EnableWindow(TRUE);
				pPage->ShowWindow(SW_SHOW);
			}
		}
	}
}

//*****************************************************************************	
void CCpNavTab::AdjustChildren( 
	BOOL bRepaint/*=TRUE*/)
//*****************************************************************************	
{
	ASSERT_VALID(this);
	if (IsWindow(m_hWnd))
	{
		CRect Rect;
		GetClientRect(Rect);
		AdjustRect(FALSE, Rect);
		if (m_p3DFrame != NULL)
		{
			ASSERT_VALID(m_p3DFrame);
			ASSERT(IsWindow(m_p3DFrame->m_hWnd));
			m_p3DFrame->MoveWindow(Rect, bRepaint);
			m_p3DFrame->GetClientRect(Rect);
		}
		for (int ii = 0; ii  < GetItemCount( ); ii++)
		{
			CCpNavTreeCtrl *pPage = m_PageArray.GetAt(ii);			
			if (pPage != NULL)
			{
				ASSERT_VALID(pPage);
				pPage->MoveWindow(Rect, bRepaint);
			}
		}
	}
}

//*****************************************************************************	
int CCpNavTab::SetCurSel(int nIndex) 
//*****************************************************************************	
{ 
	ASSERT_VALID(this);
	if (IsWindow(m_hWnd))
	{
		ASSERT(nIndex >= 0 && nIndex < GetItemCount( ));
		if (nIndex >= 0 && nIndex < GetItemCount( ))
		{
			if (GetCurSel() != nIndex)
			{
				int rv = CTabCtrl::SetCurSel(nIndex);	
				UpdateSelection(nIndex); 	
				return rv;
			}
		}
	}
	return -1;
}

//*****************************************************************************	
#ifdef _DEBUG
void CCpNavTab::AssertValid() const
//*****************************************************************************	
{
    // call inherited AssertValid first
    CTabCtrl::AssertValid();

	if (IsWindow(m_hWnd))
	{
		ASSERT(GetItemCount() == m_PageArray.GetSize());
	}
} 
#endif

/////////////////////////////////////////////////////////////////////////////
// CCpNavTab message handlers

void CCpNavTab::OnSelChange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateSelection(GetCurSel()); 	
	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////
// Called by the tree control if a new hyperlink has been selected
//*****************************************************************************	
LRESULT CCpNavTab::OnNewURL(
	WPARAM wParam, 
	LPARAM lParam)
//*****************************************************************************	
{
	GetParent()->SendMessage(WM_FCNAV_NEWURL, wParam, lParam);
	return 0;
}

