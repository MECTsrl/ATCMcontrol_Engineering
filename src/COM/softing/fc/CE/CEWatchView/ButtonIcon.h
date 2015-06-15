#if !defined(AFX_BUTTONICON_H__E1AA1518_F365_11D2_A014_00A024363DFC__INCLUDED_)
#define AFX_BUTTONICON_H__E1AA1518_F365_11D2_A014_00A024363DFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////
// CButtonIcon window

class CButtonIcon : public CButton
{
// Construction
public:
	CButtonIcon();

// Attributes
public:
	void SetIcons(int IdIconEnable, int IdIconDisable);

// Operations
public:
	virtual BOOL EnableWindow(BOOL bEnable=TRUE);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CButtonIcon)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CButtonIcon();

	// Generated message map functions
protected:
	//{{AFX_MSG(CButtonIcon)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	HICON m_hIconEnable;
	HICON m_hIconDisable;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BUTTONICON_H__E1AA1518_F365_11D2_A014_00A024363DFC__INCLUDED_)
