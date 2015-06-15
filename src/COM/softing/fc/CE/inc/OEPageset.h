// 
// Stingray Software Extension Classes
// Copyright (C) 1997 Steve Schauer
// Copyright (C) 1997 Stingray Software Inc.
// All Rights Reserved.
// 
// This source code is only intended as a supplement to the
// Stingray Extension Class product.
// See the SEC help files for detailed information
// regarding using SEC classes.
// 
//----------------------------------------------------------------------------


// oepageset.h : page setup dialog interface

#ifndef _OEPAGESET_H_
#define _OEPAGESET_H_

// OE Extension DLL
// Initialize declaration context
#ifdef _OEDLL
	#undef AFX_DATA
	#define AFX_DATA OE_DATAEXT
#endif // _OEDLL

/////////////////////////////////////////////////////////////////////////////
// OEPageSetupDlg dialog
//@doc OEPageSetupDlg
//@class OEPageSetupDlg derives from CDialog.  This class is a CDialog derivative that implements a
// page setup dialog for Objective Edit that lets the user customize color syntax settings.

class OEPageSetupDlg : public CDialog
{
	DECLARE_DYNAMIC(OEPageSetupDlg)
// Construction
public:
	//@cmember OEPageSetupDlg constructor.

	OEPageSetupDlg(CWnd* pParent = NULL);    // standard constructor

	//@cmember Initialize the OEPageSetupDlg dialog.
	void Initialize();
	
	//@cmember Kill the OEPageSetupDlg.
	void Terminate();

// Dialog Data
	//{{AFX_DATA(OEPageSetupDlg)
	enum { IDD = IDD_PAGE_SETUP };
	
	//@cmember String for the footer.
	CString m_strFooter;
	
	//@cmember String fot the header.
	CString m_strHeader;
	
	//@cmember Footer time format.
	int     m_iFooterTime;
	
	//@cmember Header time format.
	int     m_iHeaderTime;
	//}}AFX_DATA
	
	//@cmember Old footer string.
	CString m_strFooterOld;
	
	//@cmember Old header string.
	CString m_strHeaderOld;
	
	//@cmember Old footer time.
	int     m_iFooterTimeOld;
	
	//@cmember Old header time.
	int     m_iHeaderTimeOld;


// Operations
	
	//@cmember Formats the header based on header string, time, filename and page number.
	void FormatHeader(CString& strHeader, CTime& time,
		LPCTSTR pszFileName, UINT nPage);
	
	//@cmember Formats the footer based on footer string, time, filename and page number. 
	void FormatFooter(CString& strFooter, CTime& time,
		LPCTSTR pszFileName, UINT nPage);

// Implementation
protected:
	
	//@cmember DoDataExchange for OEPageSetup.
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	
	//@cmember Formats the page.
	static void FormatFilePage(
		CString& strFormat, LPCTSTR pszFileName, UINT nPage);

	// Generated message map functions
	//{{AFX_MSG(OEPageSetupDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


// OE Extension DLL
// Reset declaration context
#undef AFX_DATA
#define AFX_DATA

#endif // _OEPAGESET_H_

/////////////////////////////////////////////////////////////////////////////
