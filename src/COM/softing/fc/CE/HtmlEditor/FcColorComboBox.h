#if !defined(AFX_FCCOLORCOMBOBOX_H__D3F87DA3_CFB4_11D2_9FFC_00A024363DFC__INCLUDED_)
#define AFX_FCCOLORCOMBOBOX_H__D3F87DA3_CFB4_11D2_9FFC_00A024363DFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FcColorComboBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFcColorComboBox window

class CFcColorComboBox : public CComboBox
{
// Construction
public:
	CFcColorComboBox();

// Attributes
public:

// Operations
public:
	void Init();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFcColorComboBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFcColorComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFcColorComboBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FCCOLORCOMBOBOX_H__D3F87DA3_CFB4_11D2_9FFC_00A024363DFC__INCLUDED_)
