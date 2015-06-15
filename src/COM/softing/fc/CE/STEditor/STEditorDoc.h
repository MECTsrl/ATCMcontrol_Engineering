// STEditorDoc.h : interface of the CSTEditorDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_STEDITORDOC_H__9F791443_09AC_11D2_B9A5_006008749B3D__INCLUDED_)
#define AFX_STEDITORDOC_H__9F791443_09AC_11D2_B9A5_006008749B3D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "STEditorIF.h"
#include "PrgEditorDoc.h"
#include "Symbols.h"


class CSTEditorView;
class CSTEditorSrvrItem;


class CSTEditorDoc : public CPrgEditorDoc
{
protected: // create from serialization only
	CSTEditorDoc();
	DECLARE_DYNCREATE(CSTEditorDoc)

// Attributes
public:
	static const int IdleHint;
    static const int SettingsChangedHint;       // AUTOFMT 14.08.06 SIS

	CSTEditorSrvrItem* GetEmbeddedItem()
		{ return (CSTEditorSrvrItem*)CPrgEditorDoc::GetEmbeddedItem(); }

    CSECEditor* GetEditor (UINT id);
    // implement abstract method from CPrgEditorDoc
    bool CheckInPlaceActive(){return IsInPlaceActive()==TRUE;}

    
    // SIS, 25.09.01: get selected text for find in files dialog
    virtual CString GetSelectedText();

    // SIS: 21.11.01: workaround for problems with tool tips
    virtual void RecreateToolTip();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSTEditorDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	protected:
	virtual COleServerItem* OnGetEmbeddedItem();
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSTEditorDoc();

	CStdEditorPosition ToOffsetPosition (const CStdEditorPosition &position);
	CStdEditorPosition ToDisplayPosition (const CStdEditorPosition &position);
	
   CString GetSmartText  (const CStdEditorPosition &position, 
                                CStdEditorPosition &startPosition, 
                                CStdEditorPosition &endPosition);

   CString GetTrimmedText(const CStdEditorPosition &startPosition, 
                          const CStdEditorPosition &endPosition, 
                          CStdEditorPosition &trimmedStart, 
                          CStdEditorPosition &trimmedEnd);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

    enum SmartState
    {
        SmartError,
        SmartIdentifier,
        SmartComponent,
        SmartIndexing
    };

protected:
	virtual CDocObjectServer* GetDocObjectServer(LPOLEDOCUMENTSITE pDocSite);
	CSTEditorView *GetEditView ();

	virtual bool LoadDocument (const CString &strFileName);
	virtual bool SaveDocument (const CString &strFileName);
	virtual void SetModified (bool bModified);
	virtual void OnSerializeDocument (CArchive &ar);
	virtual void OnUpdateReadOnly ();
	virtual void OnUpdateModified ();
	virtual void OnIdle ();

	virtual void OnShowBreakpoint (CBreakpoint *pBP);
	virtual void OnHideBreakpoint (CBreakpoint *pBP);
	virtual void OnShowStoppoint ();
	virtual void OnHideStoppoint ();

    virtual void OnCreateStep (CPrgEdStep *pStep);
    virtual void OnDestroyStep (CPrgEdStep *pStep);

    virtual void OnCreateAction (CPrgEdAction *pAction);
    virtual void OnDestroyAction (CPrgEdAction *pAction);

    virtual void OnCreateTransition (CPrgEdTransition *pTransition);
    virtual void OnDestroyTransition (CPrgEdTransition *pTransition);

    virtual void OnInstancePathModified ();
    
    CSECEditor m_Edit;

    long GetPrintLinesPerPage (CDC * pDCPrint);

    HRESULT AttachPrinterDC (CDC * pDC);
	virtual HRESULT GetPageCount (long &lCount);
    virtual HRESULT GetPages (CStringArray &pages);
    virtual HRESULT ShowPrintOptionsDialog(CString& rstrOptions);

// Generated message map functions
protected:
	//{{AFX_MSG(CSTEditorDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CSTEditorDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

private:
	void InstallSyntaxColoring ();
	void UpdateReadOnly ();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STEDITORDOC_H__9F791443_09AC_11D2_B9A5_006008749B3D__INCLUDED_)
