#ifndef __LATEDDOC_H_
#define __LATEDDOC_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//----  Aggregate Includes:   --------------------------------------*
#include "latedif.h"
#include "StdEditorIF.h"
#include "StdEditorDoc.h"
#include "CEBackparse.h"
#include "CEProjMan.h"
#include "lcconfig.h"
#include "resource.h"

//----  Forward Class Definitions:   -------------------------------*
class CLATEDSrvrItem;

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

class CLATEDDoc : public CXMLDocument<CStdEditorDoc>
{
protected: // create from serialization only
	CLATEDDoc();
	DECLARE_DYNCREATE(CLATEDDoc)

// Attributes
public:
	CLATEDSrvrItem* GetEmbeddedItem()
	{ return (CLATEDSrvrItem*)COleServerDoc::GetEmbeddedItem(); }

    bool CheckInPlaceActive()
    {return IsInPlaceActive()==TRUE;}

// Operations
public:
    void DeleteContents();
    void OnSerializeDocument (CArchive &ar);
    void OnUpdateReadOnly();
    HRESULT doBackparse(BSTR sFileName,ICEProjInfo* pICEProjInfo, BSTR* psXmlText);
    bool LoadDocument (const CString &strFileName);
    bool SaveDocument (const CString &strFileName);
    void SerializeXML(CXMLArchive& ar);
    bool QueryDelayClose();
    void OnCloseDocument();
    
    CLCConfig* GetConfig()
    { return m_pConfig; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLATEDDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	protected:
	virtual COleServerItem* OnGetEmbeddedItem();
#ifdef LATED_SPLITTER
	virtual COleIPFrameWnd* CreateInPlaceFrame(CWnd* pParentWnd);
	virtual void DestroyInPlaceFrame(COleIPFrameWnd* pFrameWnd);
#endif
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLATEDDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual CDocObjectServer* GetDocObjectServer(LPOLEDOCUMENTSITE pDocSite);
// Generated message map functions
protected:
	//{{AFX_MSG(CLATEDDoc)
	afx_msg void OnGenerate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CLATEDDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

    BEGIN_INTERFACE_PART(BackParse,ICEBackparse)
        STDMETHOD(doBackparse)(/*[in, string]*/BSTR sFileName, 
                               /*[in]*/ ICEProjInfo* pICEProjInfo,
                               /*[out, string]*/BSTR* psXmlText);
    END_INTERFACE_PART(BackParse)

	DECLARE_INTERFACE_MAP()
private: //operations
    void DeleteConfig();
    void RegisterXMLTypes(CXMLArchive &ar);
    HRESULT HandleParseError(HRESULT hrRes, CXMLArchive& ar, CString &);
    void PrintMessage(HRESULT hr, CString &);
    HRESULT GetProjectManager(ICEProjectManager**);

//members
    /** Latcomm configuration*/
    CLCConfig* m_pConfig;
    bool       m_bDelayClose;
    bool       m_bNoDelContents;
};

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CDialogGen dialog

class CDialogGen : public CDialog , public /*implements*/ CLCMessageListener
{
// Construction
public:
	CDialogGen(CLCConfig* pConfig,CWnd* pParent = NULL);   // standard constructor
    ~CDialogGen();
// Dialog Data
	//{{AFX_DATA(CDialogGen)
	enum { IDD = IDD_GEN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogGen)
	protected:
    virtual void OnMessage(HRESULT hr, LPCTSTR pszMessage);
	virtual void OnOK();
	virtual void OnCancel();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogGen)
	virtual BOOL OnInitDialog();
	afx_msg void OnGenerate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private: //helpers
    void EnableHScroll();
private: //members
    CLCConfig* m_pConfig;
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __LATEDDOC_H_