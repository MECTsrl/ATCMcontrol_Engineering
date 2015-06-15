
#ifndef __PANEL_H_
#define __PANEL_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


// Panel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPanelDoc document
// Update defines for associated vies (lHint param of OnUpdate)
#define PANELDOC_UPD_DEF 0L

class CPanelDoc : public CDocument
{
    CString m_StrURL;       //URL of activated document
protected:
	CPanelDoc();            // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPanelDoc)

// Attributes
public:

// Operations
public:
    void SetURL(const CString & StrURL);
    const CString & GetURL(void) const;
    
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPanelDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPanelDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual void SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU = TRUE);

	// Generated message map functions
protected:
	//{{AFX_MSG(CPanelDoc)
	afx_msg void OnUpdateFileNew(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileOpen(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMruList(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.


#endif // __PANEL_H_

