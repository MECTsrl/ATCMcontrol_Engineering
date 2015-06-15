#if !defined(AFX_CEWATCHEDIT_H__A2F76106_FBAA_11D2_A016_00A024363DFC__INCLUDED_)
#define AFX_CEWATCHEDIT_H__A2F76106_FBAA_11D2_A016_00A024363DFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CeWatchEdit.h : header file
//
class CCeWatchElement;
class CCeWatchEditContainer;
/////////////////////////////////////////////////////////////////////////////
// CCeWatchEdit window

#define CEWATCH_EDITTYPE_NAME   0
#define CEWATCH_EDITTYPE_VALUE  1
#define CEWATCH_EDITTYPE_FORMAT 2

class CCeWatchEdit : public CEdit
{
public:
	CCeWatchEdit(CCeWatchEditContainer* pContainer);
	virtual ~CCeWatchEdit();

private:
	CCeWatchEditContainer* m_pContainer;

    HACCEL m_hAccelerators;

	//{{AFX_VIRTUAL(CCeWatchEdit)
    public:
    virtual BOOL PreTranslateMessage (MSG *pMsg);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CCeWatchEdit)
	afx_msg void OnKillFocus();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnCopy ();
    afx_msg void OnCut ();
    afx_msg void OnPaste ();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CCeWatchEditCombo window

class CCeWatchEditCombo : public CComboBox
{
// Construction
public:
	CCeWatchEditCombo(CCeWatchEditContainer* pContainer);
	bool Load(const CString& strDataType);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCeWatchEditCombo)
    virtual BOOL PreTranslateMessage (MSG *pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCeWatchEditCombo();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCeWatchEditCombo)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillfocus();
	afx_msg void OnSelendok();
    afx_msg void OnCopy ();
    afx_msg void OnCut ();
    afx_msg void OnPaste ();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	CCeWatchEditContainer* m_pContainer;
    HACCEL m_hAccelerators;
};


class CCeWatchEditContainer
{
// Construction
public:
	CCeWatchEditContainer(int m_EditItem, unsigned nEditType, CCeDisplayElement*  pDisplayElement);
	BOOL Create(const RECT& rect, CCeWatchList *pListCtrl, UINT nId);

// Attributes
public:
	inline int GetEditItem() const { return m_iEditItem; }
	inline unsigned GetEditType() const { return m_nEditType; }
	inline void IgnoreFocus(bool bIgnore) { m_bIgnoreFocus = bIgnore; }
	bool IsVisible();
	void GetText(CString& str);
	CString GetFormatFile();
	bool ExistFormatFile();

// Operations
public:
// Implementation
public:
	virtual ~CCeWatchEditContainer();

private:
	BOOL CreateEditWnd(const RECT& rect, CCeWatchList *pListCtrl, UINT nId, const CString& strText);
	BOOL CreateComboBox(const RECT& rect, CCeWatchList *pListCtrl, UINT nId, const CString& strText);

public:
	int        m_iEditItem;
	unsigned   m_nEditType;
	bool       m_bIgnoreFocus;
	CCeDisplayElement*  m_pDisplayElement;

	CCeWatchEdit*       m_pEditWnd;
	CCeWatchEditCombo*  m_pEditCbWnd;
};



/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CEWATCHEDIT_H__A2F76106_FBAA_11D2_A016_00A024363DFC__INCLUDED_)
