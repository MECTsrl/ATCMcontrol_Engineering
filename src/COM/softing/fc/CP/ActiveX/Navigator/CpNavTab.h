// FcNavTab.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCpNavTab window
class CCpNavTreeCtrl;
class CCpNavPageFrame;

class CCpNavTab : public CTabCtrl
{
// Construction
public:
	CCpNavTab(BOOL b3DFrame=TRUE);

// Attributes
public:
	virtual int SetCurSel(int nItem);
  
// Operations
public:
	virtual BOOL AddItem(CCpNavTreeCtrl *pTreeCtrl, unsigned StringId);
	virtual BOOL AddItem(CCpNavTreeCtrl *pTreeCtrl, const CString& TabText);
	virtual CCpNavTreeCtrl* GetItem(int Index);
	virtual BOOL DeleteItem(int Index);		
	virtual BOOL DeleteAllItems();
	virtual void MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint = TRUE );
	virtual void MoveWindow(LPCRECT lpRect, BOOL bRepaint = TRUE );
	virtual BOOL SetWindowPos(const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, UINT nFlags );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCpNavTab)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCpNavTab();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCpNavTab)
	afx_msg void OnSelChange(NMHDR* pNMHDR, LRESULT* pResult);
	virtual LRESULT OnNewURL(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private :	
	void AdjustChildren(BOOL bRepaint=TRUE);
	void UpdateSelection(int Index);

private :
	CArray<CCpNavTreeCtrl *, CCpNavTreeCtrl *> m_PageArray;
	CCpNavPageFrame* m_p3DFrame;

// object checking
public:
#ifdef _DEBUG
    virtual void AssertValid() const;    // Override
#endif

};

/////////////////////////////////////////////////////////////////////////////
