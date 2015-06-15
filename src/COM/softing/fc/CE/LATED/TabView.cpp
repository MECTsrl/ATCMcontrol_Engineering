#include "stdafx.h"
#include "ChildDialog.h"
#include "TabView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabView implementation

IMPLEMENT_DYNCREATE (CTabView, CCtrlView)

BEGIN_MESSAGE_MAP (CTabView, CCtrlView)
	ON_WM_SIZE ()
	ON_WM_DESTROY ()
	ON_WM_SETFOCUS ()
	ON_WM_KILLFOCUS ()
	ON_NOTIFY_REFLECT (TCN_SELCHANGING, OnSelChanging)
	ON_NOTIFY_REFLECT (TCN_SELCHANGE, OnSelChange)
END_MESSAGE_MAP()

BOOL CTabView::PreCreateWindow (CREATESTRUCT& cs) 
{
	//
	// Make sure the common controls library is initialized.
	//
	::InitCommonControls ();
	return CCtrlView::PreCreateWindow (cs);
}

/////////////////////////////////////////////////////////////////////////////
// Public interface

int CTabView::AddPage (LPCTSTR pszTitle, int nPageID, LPCTSTR pszTemplateName)
{
	//
	// Verify that the dialog template is compatible with CTabView
	// (debug builds only). If your app asserts here, make sure the dialog
	// resource you're adding to the view is a borderless child window and
	// is not marked visible.
	//
#ifdef _DEBUG
	if (pszTemplateName != NULL) {
		BOOL bResult = CheckDialogTemplate (pszTemplateName);
		ASSERT (bResult);
	}
#endif // _DEBUG
	//
	// Add a page to the tab control.
	//
	TC_ITEM item;
	item.mask = TCIF_TEXT;
	item.pszText = (LPTSTR) pszTitle;
	int nIndex = GetTabCtrl ().GetItemCount ();
	
	if (GetTabCtrl ().InsertItem (nIndex, &item) == -1)
		return -1;

	//
	// Add controls to the page.
	//
	if (pszTemplateName == NULL) {
		//
		// Not much to do if pszTemplateName is NULL.
		//
		int nArrayIndex = m_pPages.Add (NULL);
		ASSERT (nIndex == nArrayIndex);

		nArrayIndex = m_nPageIDs.Add (nPageID);
		ASSERT (nIndex == nArrayIndex);

		OnInitPage (nIndex, nPageID);
		m_hFocusWnd.Add (NULL);
	}
	else {
		//
		// Create a modeless dialog box.
		//
		CChildDialog* pDialog = new CChildDialog;

		if (pDialog == NULL) {
			GetTabCtrl ().DeleteItem (nIndex);
			return -1;
		}

		if (!pDialog->Create (pszTemplateName, this)) {
			GetTabCtrl ().DeleteItem (nIndex);
			pDialog->DestroyWindow ();
			delete pDialog;
			return -1;
		}

		//
		// Record the address of the dialog object and the page ID.
		//
		int nArrayIndex = m_pPages.Add (pDialog);
		ASSERT (nIndex == nArrayIndex);

		nArrayIndex = m_nPageIDs.Add (nPageID);
		ASSERT (nIndex == nArrayIndex);

		//
		// Size and position the dialog box within the view.
		//
		pDialog->SetParent (this); // Just to be sure

		CRect rect;
		GetClientRect (&rect);

		if (rect.Width () > 0 && rect.Height () > 0)
			ResizeDialog (nIndex, rect.Width (), rect.Height ());

		//
		// Initialize the page.
		//
		if (OnInitPage (nIndex, nPageID)) {
			//
			// Make the first control in the dialog is the one that
			// receives the input focus when the page is displayed.
			//
			HWND hwndFocus = pDialog->GetTopWindow ()->m_hWnd;
			nArrayIndex = m_hFocusWnd.Add (hwndFocus);
			ASSERT (nIndex == nArrayIndex);
		}
		else {
			//
			// Make the control that currently has the input focus is the one
			// that receives the input focus when the page is displayed.
			//
			m_hFocusWnd.Add (::GetFocus ());
		}

		//
		// If this is the first page added to the view, make it visible.
		//
		if (nIndex == 0)
			pDialog->ShowWindow (SW_SHOW);
	}
	return nIndex;
}

int CTabView::AddPage (LPCTSTR pszTitle, int nPageID, int nTemplateID)
{
	return AddPage (pszTitle, nPageID, MAKEINTRESOURCE (nTemplateID));
}

BOOL CTabView::RemovePage (int nIndex)
{
	if (nIndex >= GetTabCtrl ().GetItemCount ())
		return FALSE;

	//
	// Notify derived classes that the page is being destroyed.
	//
	OnDestroyPage (nIndex, m_nPageIDs[nIndex]);

	//
	// Switch pages if the page being deleted is the current page and it's
	// not the only remaining page.
	//
	int nCount = GetTabCtrl ().GetItemCount ();
	if (nCount > 1 && nIndex == GetTabCtrl ().GetCurSel ()) {
		int nPage = nIndex + 1;
		if (nPage >= nCount)
			nPage = nCount - 2;
		ActivatePage (nPage);
	}

	//
	// Remove the page from the tab control.
	//
	GetTabCtrl ().DeleteItem (nIndex);

	//
	// Destroy the dialog (if any) that represents the page.
	//
	CChildDialog* pDialog = m_pPages[nIndex];
	if (pDialog != NULL) {
		pDialog->DestroyWindow ();	
		delete pDialog;
	}

	//
	// Clean up, repaint, and return.
	//
	m_pPages.RemoveAt (nIndex);
	m_hFocusWnd.RemoveAt (nIndex);
	m_nPageIDs.RemoveAt (nIndex);
	Invalidate ();
	return TRUE;
}

int CTabView::GetPageCount ()
{
	return GetTabCtrl ().GetItemCount ();
}

BOOL CTabView::GetPageTitle (int nIndex, CString &strTitle)
{
	if (nIndex >= GetTabCtrl ().GetItemCount ())
		return FALSE;

	TCHAR szTitle[1024];

	TC_ITEM item;
	item.mask = TCIF_TEXT;
	item.pszText = szTitle;
	item.cchTextMax = sizeof szTitle / sizeof (TCHAR);

	if (!GetTabCtrl ().GetItem (nIndex, &item))
		return FALSE;

	strTitle = item.pszText;
	return TRUE;
}

BOOL CTabView::SetPageTitle (int nIndex, LPCTSTR pszTitle)
{
	if (nIndex >= GetTabCtrl ().GetItemCount ())
		return FALSE;

	TC_ITEM item;
	item.mask = TCIF_TEXT;
	item.pszText = (LPTSTR) pszTitle;
	
	BOOL bResult = GetTabCtrl ().SetItem (nIndex, &item);
	if (bResult)
		Invalidate ();
	return bResult;
}

int CTabView::GetPageID (int nIndex)
{
	if (nIndex >= GetTabCtrl ().GetItemCount ())
		return -1;

	return m_nPageIDs[nIndex];
}

int CTabView::SetPageID (int nIndex, int nPageID)
{
	if (nIndex >= GetTabCtrl ().GetItemCount ())
		return -1;

	int nOldPageID = m_nPageIDs[nIndex];
	m_nPageIDs[nIndex] = nPageID;
	return nOldPageID;
}

BOOL CTabView::ActivatePage (int nIndex)
{
	if (nIndex >= GetTabCtrl ().GetItemCount ())
		return FALSE;

	//
	// Do nothing if the specified page is already active.
	//
	if (nIndex == GetTabCtrl ().GetCurSel ())
		return TRUE;

	//
	// Deactivate the current page.
	//
	int nOldIndex = GetTabCtrl ().GetCurSel ();

	if (nIndex != -1) {
		CChildDialog* pDialog = m_pPages[nOldIndex];
		if (pDialog != NULL) {
			m_hFocusWnd[nOldIndex] = ::GetFocus ();
			pDialog->ShowWindow (SW_HIDE);
		}
	}

	//
	// Activate the new one.
	//
	GetTabCtrl ().SetCurSel (nIndex);
	CChildDialog* pDialog = m_pPages[nIndex];

	if (pDialog != NULL) {
		::SetFocus (m_hFocusWnd[nIndex]);
		CRect rect;
		GetClientRect (&rect);
		ResizeDialog (nIndex, rect.Width (), rect.Height ());
		pDialog->ShowWindow (SW_SHOW);
	}
	return TRUE;
}

int CTabView::GetActivePage ()
{
	return GetTabCtrl ().GetCurSel ();
}

CWnd* CTabView::GetPage (int nIndex)
{
	if (nIndex >= GetTabCtrl ().GetItemCount ())
		return NULL;

	return (CWnd*) m_pPages[nIndex];
}

int CTabView::GetPageIndex (int nPageID)
{
	int nCount = GetTabCtrl ().GetItemCount ();
	if (nCount == 0)
		return -1;

	for (int i=0; i<nCount; i++) {
		if (m_nPageIDs[i] == nPageID)
			return i;
	}

	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// Private helper functions

#ifdef _DEBUG
BOOL CTabView::CheckDialogTemplate (LPCTSTR pszTemplateName)
{
	//
	// Verify that the dialog resource exists.
	//
	ASSERT (pszTemplateName != NULL);
	HINSTANCE hInstance = AfxFindResourceHandle (pszTemplateName, RT_DIALOG);
	HRSRC hResource = ::FindResource (hInstance, pszTemplateName, RT_DIALOG);

	if (hResource == NULL)
		return FALSE; // Resource doesn't exist

	HGLOBAL hTemplate = LoadResource (hInstance, hResource);
	ASSERT (hTemplate != NULL);

	//
	// Get the dialog's style bits.
	//
	DLGTEMPLATEEX* pTemplate = (DLGTEMPLATEEX*) LockResource (hTemplate);

	DWORD dwStyle;
	if (pTemplate->signature == 0xFFFF)
		dwStyle = pTemplate->style;
	else
		dwStyle = ((DLGTEMPLATE*) pTemplate)->style;

	UnlockResource (hTemplate);
	FreeResource (hTemplate);

	//
	// Verify that the dialog is an invisible child window.
	//
	if (dwStyle & WS_VISIBLE)
		return FALSE; // WS_VISIBLE flag is set

	if (!(dwStyle & WS_CHILD))
		return FALSE; // WS_CHILD flag isn't set

	//
	// Verify that the dialog has no border and no title bar.
	//
	if (dwStyle & (WS_BORDER | WS_THICKFRAME | DS_MODALFRAME))
		return FALSE; // One or more border flags are set

	if (dwStyle & WS_CAPTION)
		return FALSE; // WS_CAPTION flag is set

	return TRUE;
}
#endif // _DEBUG

void CTabView::ResizeDialog (int nIndex, int cx, int cy)
{
	if (nIndex != -1) {
		CChildDialog* pDialog = m_pPages[nIndex];

		if (pDialog != NULL) {
			CRect rect;
			GetTabCtrl ().GetItemRect (nIndex, &rect);

			int x, y, nWidth, nHeight;
			DWORD dwStyle = GetTabCtrl ().GetStyle ();

			if (dwStyle & TCS_VERTICAL) { // Vertical tabs
				int nTabWidth =
					rect.Width () * GetTabCtrl ().GetRowCount ();
				x = (dwStyle & TCS_RIGHT) ? 4 : nTabWidth + 4;
				y = 4;
				nWidth = cx - nTabWidth - 8;
				nHeight = cy - 8;
			}
			else { // Horizontal tabs
				int nTabHeight =
					rect.Height () * GetTabCtrl ().GetRowCount ();
				x = 4;
				y = (dwStyle & TCS_BOTTOM) ? 4 : nTabHeight + 4;
				nWidth = cx - 8;
				nHeight = cy - nTabHeight - 8;
			}
			pDialog->SetWindowPos (NULL, x, y, nWidth, nHeight, SWP_NOZORDER);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// Overridables

BOOL CTabView::OnInitPage (int nIndex, int nPageID)
{
	//
	// Override in derived class to initialize pages.
	//
	return TRUE;
}

void CTabView::OnActivatePage (int nIndex, int nPageID)
{
	//
	// Override in derived class to respond to page activations.
	//
}

void CTabView::OnDeactivatePage (int nIndex, int nPageID)
{
	//
	// Override in derived class to respond to page deactivations.
	//
}

void CTabView::OnDestroyPage (int nIndex, int nPageID)
{
	//
	// Override in derived class to free resources.
	//
}

/////////////////////////////////////////////////////////////////////////////
// Message handlers

void CTabView::OnSize (UINT nType, int cx, int cy)
{
	//
	// When the view's size changes, resize the dialog (if any) shown in the
	// view to prevent the dialog from clipping the view's inside border.
	//
	CCtrlView::OnSize (nType, cx, cy);
	ResizeDialog (GetTabCtrl ().GetCurSel (), cx, cy);
}

void CTabView::OnSelChanging (NMHDR* pNMHDR, LRESULT* pResult)
{
	//
	// Notify derived classes that the selection is changing.
	//
	int nIndex = GetTabCtrl ().GetCurSel ();
	if (nIndex == -1)
		return;

	OnDeactivatePage (nIndex, m_nPageIDs[nIndex]);

	//
	// Save the input focus and hide the old page.
	//
	CChildDialog* pDialog = m_pPages[nIndex];

	if (pDialog != NULL) {
		m_hFocusWnd[nIndex] = ::GetFocus ();
		pDialog->ShowWindow (SW_HIDE);
	}
	*pResult = 0;
}

void CTabView::OnSelChange (NMHDR* pNMHDR, LRESULT* pResult)
{
	int nIndex = GetTabCtrl ().GetCurSel ();
	if (nIndex == -1)
		return;

	//
	// Show the new page.
	//
	CChildDialog* pDialog = m_pPages[nIndex];

	if (pDialog != NULL) {
		::SetFocus (m_hFocusWnd[nIndex]);
		CRect rect;
		GetClientRect (&rect);
		ResizeDialog (nIndex, rect.Width (), rect.Height ());
		pDialog->ShowWindow (SW_SHOW);
	}

	//
	// Notify derived classes that the selection has changed.
	//
	OnActivatePage (nIndex, m_nPageIDs[nIndex]);
	*pResult = 0;
}

void CTabView::OnSetFocus (CWnd* pOldWnd) 
{
	CCtrlView::OnSetFocus (pOldWnd);
	
	//
	// Set the focus to a control on the current page.
	//
	int nIndex = GetTabCtrl ().GetCurSel ();
	if (nIndex != -1)
		::SetFocus (m_hFocusWnd[nIndex]);	
}

void CTabView::OnKillFocus (CWnd* pNewWnd) 
{
	CCtrlView::OnKillFocus (pNewWnd);
	
	//
	// Save the HWND of the control that holds the input focus.
	//
	int nIndex = GetTabCtrl ().GetCurSel ();
	if (nIndex != -1)
		m_hFocusWnd[nIndex] = ::GetFocus ();	
}

void CTabView::OnDestroy () 
{
	int nCount = m_pPages.GetSize ();

	//
	// Destroy dialogs and delete CChildDialog objects.
	//
	if (nCount > 0) {
		for (int i=nCount - 1; i>=0; i--) {
			OnDestroyPage (i, m_nPageIDs[i]);
			CChildDialog* pDialog = m_pPages[i];
			if (pDialog != NULL) {
				pDialog->DestroyWindow ();
				delete pDialog;
			}
		}
	}

	//
	// Clean up the internal arrays.
	//
	m_pPages.RemoveAll ();
	m_hFocusWnd.RemoveAll ();
	m_nPageIDs.RemoveAll ();

	CCtrlView::OnDestroy ();
}
