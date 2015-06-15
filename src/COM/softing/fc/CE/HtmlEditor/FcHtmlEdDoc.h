// FcHtmlEdDoc.h : interface of the CFcHtmlEdDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FCHTMLEDDOC_H__A04CED82_B51D_11D2_9FD9_00A024363DFC__INCLUDED_)
#define AFX_FCHTMLEDDOC_H__A04CED82_B51D_11D2_9FD9_00A024363DFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FcHtmlEdIF.h"
#include "StdEditorDoc.h"

class CFcHtmlEdSrvrItem;

class CFcHtmlEdDoc : public CStdEditorDoc
{
protected: // create from serialization only
	CFcHtmlEdDoc();
	DECLARE_DYNCREATE(CFcHtmlEdDoc)

// Attributes
public:
	CFcHtmlEdSrvrItem* GetEmbeddedItem()
		{ return (CFcHtmlEdSrvrItem*)COleServerDoc::GetEmbeddedItem(); }

	inline const CString& GetEdDocPathName() { return m_EdDocFileName; };
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFcHtmlEdDoc)
	protected:
	virtual COleServerItem* OnGetEmbeddedItem();
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual void OnSerializeDocument(CArchive &ar);
	virtual ~CFcHtmlEdDoc();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual CDocObjectServer* GetDocObjectServer(LPOLEDOCUMENTSITE pDocSite);

// Generated message map functions
protected:
	//{{AFX_MSG(CFcHtmlEdDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CFcHtmlEdDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
private :
	CString m_EdDocFileName;
	void SetEdDocFile(CArchive& ar);
	void ResetEdDocFile();
    void CopyFileAndFilterAbsPath(CArchive& ar);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FCHTMLEDDOC_H__A04CED82_B51D_11D2_9FD9_00A024363DFC__INCLUDED_)
