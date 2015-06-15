// 18.12.2000, SIS: Dokument-Klasse, die vom MFC-Applikationswizard angelegt wurde
//                  Erhaelt keinen Header, da sie spaeter nicht mehr gebraucht wird
// CEMAINDoc.h : interface of the CCEMAINDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CEMAINDOC_H__C0B15B53_E0A3_4313_AD0F_DC0DFAAF7CE8__INCLUDED_)
#define AFX_CEMAINDOC_H__C0B15B53_E0A3_4313_AD0F_DC0DFAAF7CE8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include "fccallback.h"
#include "CEStateMan.h"


class CCEMAINDoc : public CDocument
{
protected: // create from serialization only
	CCEMAINDoc();
	DECLARE_DYNCREATE(CCEMAINDoc)

// Attributes
public:
    CString         m_StrPath;
    CString         m_StrProjectTitle;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCEMAINDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void OnCloseDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCEMAINDoc();

    FC_CALLBACK (CEStateChangeCB, CCEMAINDoc) m_StateChangeCB;


#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
    void LoadProjectListViewSettings(CSettings* pSettings);
    void SaveProjectListViewSettings(CSettings* pSettings);

protected:
//    void RemoveAllTargets(bool bKeepAssignments = false);
    void RemoveAllTargets();
    virtual BOOL SaveModified ();
    bool SaveDocuments ();

    HRESULT GetBreakpointManager(CComPtr<IBreakPoint>& pIBreakPoint);
    void OnStateChange (CEStateNotify stateNotify, unsigned int uiStage);

    BOOL CheckProjectOpened(const CString& crstrProjectFilePath);
    BOOL WriteLockFile(const CString crstrProjectFilePath);
    BOOL ReadLockFile(const CString crstrProjectFilePath, CString& rstrUser);
    BOOL RemoveLockFile(const CString& crstrProjectFilePath, BOOL bForce = FALSE);
    CString GetLockFilePathName(const CString& crstrProjectFilePath);
    CString GetCurrentUser();

protected:
    CStdioFile      m_tLockFile;        // lock file
    BOOL            m_bLockFileOpen;    // lock file open?
    BOOL            m_bOpen;            // project document opened?

// Generated message map functions
protected:
	//{{AFX_MSG(CCEMAINDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
    afx_msg void OnUpdateFileClose(CCmdUI *pCmdUI);
    afx_msg void OnUpdateDocumentOpen(CCmdUI *pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

// global helper functions
void PathFromFile(CString& StrPath,LPCTSTR lpszFileName);
BOOL ProjectFromFile(CString& StrProject, LPCTSTR lpszFileName);
BOOL ConvertProject(CString& strProject);

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CEMAINDOC_H__C0B15B53_E0A3_4313_AD0F_DC0DFAAF7CE8__INCLUDED_)
