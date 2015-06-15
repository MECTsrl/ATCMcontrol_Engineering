#if !defined(AFX_CEWATCHSHEETINSVAR_H__B66CD9F1_3533_11D3_A040_00A024363DFC__INCLUDED_)
#define AFX_CEWATCHSHEETINSVAR_H__B66CD9F1_3533_11D3_A040_00A024363DFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CeWatchSheetInsVar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCeWatchSheetInsVar

class CCeWatchSheetInsVar : public CPropertySheet
{
	DECLARE_DYNAMIC(CCeWatchSheetInsVar)

// Construction
public:
	CCeWatchSheetInsVar(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CCeWatchSheetInsVar(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:
	void SetModified(bool bModified);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCeWatchSheetInsVar)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCeWatchSheetInsVar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCeWatchSheetInsVar)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CEWATCHSHEETINSVAR_H__B66CD9F1_3533_11D3_A040_00A024363DFC__INCLUDED_)
