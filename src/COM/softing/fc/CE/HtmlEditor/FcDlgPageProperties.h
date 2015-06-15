
#if !defined(AFX_FCDLGPAGEPROPERTIES_H__DA459CC6_D13D_11D2_9FFC_00A024363DFC__INCLUDED_)
#define AFX_FCDLGPAGEPROPERTIES_H__DA459CC6_D13D_11D2_9FFC_00A024363DFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FcDlgPageProperties.h : header file
//

#include "FcColorComboBox.h"
/////////////////////////////////////////////////////////////////////////////
// CFcDlgPageProperties dialog

class CFcDlgPageProperties : public CDialog
{
// Construction
public:
	CFcDlgPageProperties(CFcHtmlEdView* pParent = NULL);   // standard constructor
	virtual ~CFcDlgPageProperties();   


// Dialog Data
	//{{AFX_DATA(CFcDlgPageProperties)
	enum { IDD = IDD_PAGE };
	CFcColorComboBox	m_CbCtrlColorBg;
	CFcColorComboBox	m_CbCtrlColorText;

	CString	m_BgColor;
	CString	m_TextColor;
	CString	m_BgImage;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFcDlgPageProperties)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFcDlgPageProperties)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnColorBg();
	afx_msg void OnBtnColorText();
	afx_msg void OnBtnImageBg();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:	
	CFcHtmlEdView* m_pParent;
	IHTMLDocument2* m_pHTMLDocument2;

private:	
	CString GetColorEntry(const CString& str);
	CString GetBodyAttribute(const CString& name);
	void SetBodyAttribute(const CString& name, const CString& value);
	void SelectCustomColor(CString& ColEntry);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FCDLGPAGEPROPERTIES_H__DA459CC6_D13D_11D2_9FFC_00A024363DFC__INCLUDED_)
