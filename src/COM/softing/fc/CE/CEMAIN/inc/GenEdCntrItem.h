
#ifndef __GENEDCNTRITEM_H_
#define __GENEDCNTRITEM_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

// CntrItem.h : interface of the CGenEdCntrItem class
//

class CGenEdDoc;
class CGenEdView;

#include "PrgEditorIF.h"
#include "fccallback.h"
#include "CEStateMan.h"
#include "StdEdEdipos.h"
#include "StdEditorIF.h"
#include "CSC_Online.h"


#if _MFC_VER == 0x0421

class COleDocObjectItem : public COleClientItem
{
	friend class COleFrameHook;
	DECLARE_DYNAMIC(COleDocObjectItem)

// Constructors
public:
	COleDocObjectItem(COleDocument* pContainerDoc = NULL);

//Overridables
public:
	virtual void Serialize(CArchive& ar);
	LPOLEDOCUMENTVIEW GetActiveView() const;
	virtual void Release(OLECLOSE dwCloseOption = OLECLOSE_NOSAVE);
	virtual void OnInsertMenus(CMenu* pMenuShared,
		LPOLEMENUGROUPWIDTHS lpMenuWidths);
	virtual void OnRemoveMenus(CMenu *pMenuShared);
	virtual CMenu* GetHelpMenu(UINT& nPosition);

// Operations
public:
	static BOOL OnPreparePrinting(CView* pCaller, CPrintInfo* pInfo,
		BOOL bPrintAll = TRUE);
	static void OnPrint(CView* pCaller, CPrintInfo* pInfo,
		BOOL bPrintAll = TRUE);
	BOOL GetPageCount(LPLONG pnFirstPage, LPLONG pcPages);
	HRESULT ExecCommand(DWORD nCmdID,
		DWORD nCmdExecOpt = OLECMDEXECOPT_DONTPROMPTUSER,
		const GUID* pguidCmdGroup = NULL);

// Implementation
public:
	virtual ~COleDocObjectItem();
	CMenu* m_pHelpPopupMenu;
protected:
	void ActivateAndShow();
	LPOLEDOCUMENTVIEW m_pActiveView;
	LPPRINT m_pIPrint;
	BOOL SupportsIPrint();
	BOOL m_bInHelpMenu;

	BEGIN_INTERFACE_PART(OleDocumentSite, IOleDocumentSite)
		INIT_INTERFACE_PART(COleDocObjectItem, OleDocumentSite)
		STDMETHOD(ActivateMe)(LPOLEDOCUMENTVIEW pViewToActivate);
	END_INTERFACE_PART(OleDocumentSite)

	DECLARE_INTERFACE_MAP()
};

#endif // _MFC_VER == 0x0421


class CGenEdCntrItem : public COleDocObjectItem
{
	DECLARE_SERIAL(CGenEdCntrItem)

// Constructors
public:
	CGenEdCntrItem(CGenEdDoc* pContainer = NULL);
		// Note: pContainer is allowed to be NULL to enable IMPLEMENT_SERIALIZE.
		//  IMPLEMENT_SERIALIZE requires the class have a constructor with
		//  zero arguments.  Normally, OLE items are constructed with a
		//  non-NULL document pointer.

// Attributes
public:
	CGenEdDoc* GetDocument()
		{ return (CGenEdDoc*)COleDocObjectItem::GetDocument(); }
	CGenEdView* GetActiveView()
		{ return (CGenEdView*)COleClientItem::GetActiveView(); }

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGenEdCntrItem)
	public:
	virtual void OnChange(OLE_NOTIFICATION wNotification, DWORD dwParam);
	virtual void OnActivate();
	protected:
	virtual void OnGetItemPosition(CRect& rPosition);
	virtual void OnDeactivateUI(BOOL bUndoable);
	virtual BOOL OnChangeItemPosition(const CRect& rectPos);
    virtual void OnDeactivate ();
	//}}AFX_VIRTUAL

	virtual void Release (OLECLOSE dwCloseOption=OLECLOSE_NOSAVE);

    // for inserting KAD accelerators:
    virtual BOOL OnGetWindowContext(CFrameWnd** ppMainFrame,
		CFrameWnd** ppDocFrame, LPOLEINPLACEFRAMEINFO lpFrameInfo);

// Implementation
public:
	~CGenEdCntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual void Serialize(CArchive& ar);

	bool CreateServer (const GUID& rtGUID);
	bool IsStandardEditor ();
	bool IsProgramEditor ();

    void SetDelayedPositioning (bool bDelay);

    void KeepOffServer ();

	CString GetPositionText ();

	ICEEdit *GetICEEdit ();
    ICEPrint*   GetICEPrint();

	HRESULT CheckICEEdit ();
	HRESULT CheckICEPrgEdit ();
	HRESULT CheckICEMonitor ();
	HRESULT CheckICEDebug ();

	HRESULT LoadDocument (const CString &strFileName);
	HRESULT SaveDocument ();
	HRESULT SaveDocumentAs (const CString &strFileName);
	HRESULT SetInstance (const CString &strInstanceName, const CString &strMachineName, const CString &strMachineAddress);
	HRESULT GetModified ();
	HRESULT SetModified (bool bModified);
	HRESULT ForceReadOnly (bool bOn);
	HRESULT GetPosition (EDP_POS_QUALIFIER& posQual, long& lX, long& lY, long& lCX, long& lCY);
	HRESULT GotoPosition (EDP_POS_QUALIFIER posQual, long lX, long lY, long lCX, long lCY);
	HRESULT SetClientExtent (CSize size);
	HRESULT GotoLocation (const CString &strLocation);
	HRESULT FindFirst (const CString &strStringToSearch, UINT uiOption, CString &strLocation);
	HRESULT FindNext (CString &strLocation);
	HRESULT SetPrintInfo (const CString &strPrinterName, long lWidth, long lHeight);
	HRESULT GetPageCount (long &lCount);
	HRESULT GetPages (CStringArray &pages);
    HRESULT SetBaseFile (const CString &strBaseFile);

	HRESULT SetSymbolFile (const CString &strSymbolFile);
	HRESULT NotifyConnect (const CString &strMachineAddress);
	HRESULT NotifyDisconnect (const CString &strMachineAddress);
	HRESULT NotifyDebugState (CSC_SESSION_STATE state);
	HRESULT InsertFBCall  (const CString &strFB);
    HRESULT InsertVariable(const CString &strVar);

	HRESULT ShowBreakpoint (EDP_POS_QUALIFIER posQual, long lX, long lY, long lCX, long lCY, short sState);
	HRESULT HideBreakpoint (EDP_POS_QUALIFIER posQual, long lX, long lY, long lCX, long lCY);
	HRESULT ShowStoppoint  (EDP_POS_QUALIFIER posQual, long lX, long lY, long lCX, long lCY);
	HRESULT HideStoppoint ();
    HRESULT GetSelectedText(CString &strText);

	CString GetDocumentFile ();
	void SetDocumentFile (const CString &strDocumentFile);

	virtual BOOL IsBlank () const;

	virtual BOOL OnShowControlBars (CFrameWnd *pFrameWnd, BOOL bShow);

	HRESULT StoreWatchpoints (CStringArray &watchpoints);
	HRESULT RestoreWatchpoints (const CStringArray &watchpoints);

    HRESULT Resubscribe ();

protected:
	virtual void OnPositionChanged (EDP_POS_QUALIFIER posQual, long lX, long lY, long lCX, long lCY);

	CString m_strDocumentFile;

    bool m_bKeepOffServer;
    CComPtr<IClassFactory> m_pClassFactory;

	CComPtr<ICEEdit>                m_pICEEdit;
	CComPtr<ICEPrgEdit>             m_pICEPrgEdit;
	CComPtr<ICEMonitor>             m_pICEMonitor;
	CComPtr<ICEDebug>               m_pICEDebug;
	CComPtr<IConnectionPoint>       m_pIConnectionPoint;

	bool                m_bStandardEditor;
	bool                m_bProgramEditor;

    EDP_POS_QUALIFIER   m_PosQual;
	long                m_lX;
	long                m_lY;
	long                m_lCX;
	long                m_lCY;

	DWORD               m_dwEditNotifyCookie;
	DWORD               m_dwMessageCookie;

	bool                m_bDelayPositioning;
	bool                m_bDelayedPosition;
    EDP_POS_QUALIFIER   m_DelayedPosQual;
	long                m_lDelayedX;
	long                m_lDelayedY;
	long                m_lDelayedCX;
	long                m_lDelayedCY;

	bool                m_bDelayedLocation;
	CString             m_strDelayedLocation;

    FC_CALLBACK (CEStateChangeCB, CGenEdCntrItem) m_StateChangeCB;

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CGenEdCntrItem)
	afx_msg SCODE GetGenEdDoc (LPDISPATCH FAR* pDocumentDispatch);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

    BEGIN_INTERFACE_PART (CEEditNotifyObj, ICEEditNotify)
	    STDMETHOD (OnPositionChanged)(EDP_POS_QUALIFIER posQual, long lX, long lY, long lCX, long lCY);
    END_INTERFACE_PART(CEEditNotifyObj)

private:
	HRESULT GetServerConnectionPoint (const IID &iidSink);
	HRESULT AdviseServer ();
	HRESULT UnadviseServer ();
    void RegisterMessageSource ();
    void UnregisterMessageSource ();
    void OnStateChange (CEStateNotify stateNotify, unsigned int uiStage);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // __GENEDCNTRITEM_H_


