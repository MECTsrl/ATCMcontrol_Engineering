
#if !defined(AFX_FCFONTBAR_H__27DD0D46_D89D_11D2_A005_00A024363DFC__INCLUDED_)
#define AFX_FCFONTBAR_H__27DD0D46_D89D_11D2_A005_00A024363DFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FcFontBar.h : header file
//

#include "FcColorComboBox.h"
/////////////////////////////////////////////////////////////////////////////
// CFcFontBar window

class CFcFontBar : public CDialogBar
{
// Construction
public:
	CFcFontBar();   // standard constructor

	CFcColorComboBox	m_CbCtrlColorText;

// Attributes
public:
	CString GetSelectedFontName() const;
	int GetSelectedFontSize() const;
	CString GetSelectedHtmlTag() const;
	void SetSelectedHtmlTag(const CString& SelectString, BOOL bEnable);
	void SetSelectedFontName(const CString& SelectString, BOOL bEnable);
	void SetSelectedFontSize(int Size, BOOL bEnable); 

// Operations
public:
	BOOL Init();
	BOOL AddHtmlTag(const CString& tag, BOOL bClear);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFcFontBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFcFontBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFcFontBar)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FCFONTBAR_H__27DD0D46_D89D_11D2_A005_00A024363DFC__INCLUDED_)
