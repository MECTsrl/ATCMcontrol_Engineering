#if !defined(AFX_TABVIEW_H__04D55FB9_752E_4FC1_AFB9_C9E3CFBEFB9B__INCLUDED_)
#define AFX_TABVIEW_H__04D55FB9_752E_4FC1_AFB9_C9E3CFBEFB9B__INCLUDED_

#include <afxtempl.h>
#include "ChildDialog.h"

#ifdef _DEBUG
#pragma pack (push, 1)

typedef struct
{
	WORD dlgVer;
	WORD signature;
	DWORD helpID;
	DWORD exStyle;
	DWORD style;
	WORD cDlgItems;
	short x;
	short y;
	short cx;
	short cy;
} DLGTEMPLATEEX;

#pragma pack (pop)
#endif // _DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTabView declaration

class CTabView : public CCtrlView
{
	DECLARE_DYNCREATE (CTabView)

public:
	CTabView() :
		CCtrlView (_T ("SysTabControl32"), AFX_WS_DEFAULT_VIEW) {}

	CTabCtrl& GetTabCtrl () const { return *(CTabCtrl*) this; }

	int AddPage (LPCTSTR pszTitle, int nPageID,
		LPCTSTR pszTemplateName);
	int AddPage (LPCTSTR pszTitle, int nPageID, int nTemplateID);
	BOOL RemovePage (int nIndex);
	int GetPageCount ();
	BOOL GetPageTitle (int nIndex, CString& strTitle);
	BOOL SetPageTitle (int nIndex, LPCTSTR pszTitle);
	int GetPageID (int nIndex);
	int SetPageID (int nIndex, int nPageID);
	BOOL ActivatePage (int nIndex);
	int GetActivePage ();
	CWnd* GetPage (int nIndex);
	int GetPageIndex (int nPageID);

protected:
	CTypedPtrArray<CObArray, CChildDialog*> m_pPages;
	CArray<HWND, HWND> m_hFocusWnd;
	CArray<int, int> m_nPageIDs;

	virtual BOOL PreCreateWindow (CREATESTRUCT& cs);
	virtual BOOL OnInitPage (int nIndex, int nPageID);
	virtual void OnActivatePage (int nIndex, int nPageID);
	virtual void OnDeactivatePage (int nIndex, int nPageID);
	virtual void OnDestroyPage (int nIndex, int nPageID);

#ifdef _DEBUG
	BOOL CheckDialogTemplate (LPCTSTR pszTemplateName);
#endif // _DEBUG
	void ResizeDialog (int nIndex, int cx, int cy);

	afx_msg void OnSize (UINT nType, int cx, int cy);
	afx_msg void OnDestroy ();
	afx_msg void OnSetFocus (CWnd* pOldWnd);
	afx_msg void OnKillFocus (CWnd* pNewWnd);
	afx_msg void OnSelChanging (NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelChange (NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_TABVIEW_H__04D55FB9_752E_4FC1_AFB9_C9E3CFBEFB9B__INCLUDED_)
