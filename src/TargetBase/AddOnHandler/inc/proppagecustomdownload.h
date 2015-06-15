#ifndef _PROPPAGECUSTOMDOWNLOAD_H_
#define _PROPPAGECUSTOMDOWNLOAD_H_

#include "BasePropertyPage.h"
#include "AddonBaseResource.h"

/////////////////////////////////////////////////////////////////////////////
// CDownloadPage dialog

class CPropPageCustomDownload : public CBasePropertyPage
{

// Construction
public:
	CPropPageCustomDownload();
	~CPropPageCustomDownload();

// Dialog Data
	//{{AFX_DATA(CPropPageCustomDownload)
	enum { IDD = IDD_PROPPAGE_CUSTDOWNLOAD };
	CString	m_strDir;
	//}}AFX_DATA


    virtual BOOL Validate();

    CString GetDir();
    void SetDir(const CString& str);


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropPageCustomDownload)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropPageCustomDownload)
	afx_msg void OnCdBtnBrowse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif
