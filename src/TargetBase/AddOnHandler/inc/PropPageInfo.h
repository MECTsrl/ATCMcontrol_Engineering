#ifndef _PROPPAGEINFO_H_
#define _PROPPAGEINFO_H_

#include "AddonBaseResource.h"
#include "BasePropertyPage.h"


//------------------------------------------------------------------*
/**
 *  class CPropPageInfo
 *
 *  - property page for standard parameters (Text1, Text2, Text3, Alias, URL)
 *  - used in target properties dialog (CTargetPropertySheet)
 *
 *  @see    CTargetPropertySheet
 */
class CPropPageInfo : public CBasePropertyPage
{
// Construction
public:
	CPropPageInfo(CWnd* pParent = NULL);   // standard constructor


// Dialog Data
	//{{AFX_DATA(CPropPageInfo)
	enum { IDD = IDD_PROPPAGE_INFO };
	CString	m_strAlias;
	CString	m_strURL;
	CString	m_strText1;
	CString	m_strText2;
	CString	m_strText3;
	//}}AFX_DATA

    void Init(const CString& crstrProjectPath);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropPageInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropPageInfo)
	virtual BOOL OnInitDialog();
	afx_msg void OnPropgenBrowse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
    CString     m_strProjectPath;   // project path
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif