#if !defined(AFX_PROPEDIT_H__7B9EF0D3_59F1_11D2_B806_0000C0D26FBC__INCLUDED_)
#define AFX_PROPEDIT_H__7B9EF0D3_59F1_11D2_B806_0000C0D26FBC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PropEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropEdit window


class CPropEdit : public CEdit
{
// Construction
public:
	CPropEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPropEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPropEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_PROPEDIT_H__7B9EF0D3_59F1_11D2_B806_0000C0D26FBC__INCLUDED_)
