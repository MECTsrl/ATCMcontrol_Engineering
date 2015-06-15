
#ifndef __HEXEDIT_HPP_
#define __HEXEDIT_HPP_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*
#define COL_NBR 8
//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

/////////////////////////////////////////////////////////////////////////////
// CHexEdit window

class CHexEdit : public CEdit
{
// =Construction
public:
	CHexEdit();

// =Operations
public:
    void                    SetHexContent(BYTE const* pBuffer, int iBufLen, int iMaxLen = 0);
    int                     GetHexContent(BYTE* pBuffer, int iMaxBufLen);

// =Overridables
protected:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHexEdit)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// =Implementation
public:
	virtual ~CHexEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHexEdit)
	afx_msg void OnUpdate();
	afx_msg void OnVscroll();
	afx_msg void OnChange();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG

// =Attributes
public:
	void ResetChangedFlag();
	BOOL HasChanged();
	void SetLinePropperties(DWORD dwColPerRow, DWORD dwVisibleLine);

private:
	BOOL	CreateHexString(CString &sText, BYTE* pBuffer, int iMaxBufLen);
			// make a Hexstring from bytes
private:
	BOOL	m_bHasChanged;		// User changed data
	DWORD	m_dwVisibleLines;	// defines the count for visible Lines (Linenumbers)
	DWORD	m_dwColPerRow;		// defines number of Cols per Row
    int		m_iMaxLen;
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

/////////////////////////////////////////////////////////////////////////////
#endif // __HEXEDIT_HPP_

