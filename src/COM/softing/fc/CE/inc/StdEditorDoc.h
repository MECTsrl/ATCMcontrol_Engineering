// StdEditorDoc.h : interface of the CStdEditorDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_STDEDITORDOC_H__CC73FC7F_F147_11D1_882F_006097E1BE6E__INCLUDED_)
#define AFX_STDEDITORDOC_H__CC73FC7F_F147_11D1_882F_006097E1BE6E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "StdEdAttr.h"
#include "CEMsg.h"


class CStdEditorDoc : public COleServerDoc
{
protected: // create from serialization only
	CStdEditorDoc ();
	DECLARE_DYNAMIC (CStdEditorDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStdEditorDoc)
	//}}AFX_VIRTUAL

	virtual void Serialize(CArchive& ar);

// Implementation
public:
	static const int GotoPositionHint;
	static const int GetPositionHint;
	static const int SetColorHint;
	static const int SetFontHint;
	static const int GotoLocationHint;

	virtual ~CStdEditorDoc ();

#ifdef _DEBUG
	virtual void AssertValid () const;
	virtual void Dump (CDumpContext& dc) const;
#endif
	virtual void DeleteContents ();

	CString GetDocumentFile ();
	CSize GetClientExtent ();

	CString GetPrinterName ();
	long GetPageWidth ();
	long GetPageHeight ();

	virtual void PositionChanged (const CStdEditorPosition &position);
	virtual HRESULT GotoURL (const CString &strURL);
	void PrintSystemMessage (WORD wFacility, HRESULT hr);
    void PrintMessage (WORD wFacility, HRESULT hrMessage, CString &strMessage);

	bool GetMessageText (HRESULT hr, CString &strMessage);
    bool CheckReadonly ();
    void InvalidateReadonlyCache ();

    // SIS, 25.09.01: get selected text for find in files dialog
    virtual CString GetSelectedText();

    // SIS, 13.11.01: prepare edit (enable editor to change tab)
    virtual BOOL PrepareEdit();

protected:
	virtual BOOL OnOpenDocument (LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument (LPCTSTR lpszPathName);
    virtual void OnCloseDocument ();

	virtual bool LoadDocument (const CString &strFileName);
	virtual bool SaveDocument (const CString &strFileName);
	virtual bool GetModified ();
	virtual void SetModified (bool bModified);
	virtual void GetPosition (CStdEditorPosition &position);
	virtual void GotoPosition (CStdEditorPosition position);
	virtual void ForceReadOnly (bool bOn);
	virtual void SetLogicalColor (CStdEditorColor color);
	virtual void SetFont (CFont *font);
	virtual HRESULT SetClientExtent (CSize size);
	virtual HRESULT GotoLocation (const CString &strLocation);
	virtual HRESULT FindFirst (const CString &strStringToSearch, UINT uiOption, CString &strLocation);
	virtual HRESULT FindNext (CString &strLocation);
	virtual HRESULT SetPrintInfo (const CString &strPrinterName, long lWidth, long lHeight);
	virtual HRESULT GetPageCount (long &lCount);
	virtual HRESULT GetPages (CStringArray &pages);
    virtual HRESULT SetBaseFile (const CString &strBaseFile);
    virtual HRESULT Lock (bool bLock);

	virtual void OnSerializeDocument (CArchive &ar) = 0;
	virtual void OnUpdateReadOnly ();
	virtual void OnUpdateModified ();

	HRESULT GetGenEdItem (CComPtr<IDispatch> &pGenEdItem);
	HRESULT GetGenEdDoc (CComPtr<IDispatch> &pGenEdDoc);
	HRESULT GetGenEdCont (CComPtr<IDispatch> &pGenEdCont);

	virtual void OnDocWindowActivate (BOOL bActivate);
	virtual void OnIdle ();
    virtual bool QueryDelayClose ();

    // SIS: CEDOCU
    virtual HRESULT GetPrintOptions(CString& rstrOptions);
    virtual HRESULT SetPrintOptions(const CString& crstrOptions);
    virtual HRESULT ShowPrintOptionsDialog(CString& rstrOptions);

    bool ParsePrintOptions(const CString& crstrOptions, const CString& crstrAttribute, CString& rstrValue);

	CString m_strDocumentFile;
	bool    m_bForcedReadOnly;
	CSize   m_ClientExtent;

	CString m_strPrinterName;
	long    m_lPageWidth;
	long    m_lPageHeight;

    unsigned int m_uiLockCount;
    bool m_bDelayedClose;

    // The base file is the file the loaded file is based upon.
    // In many cases m_strDocumentFile and m_strBaseFile will
    // be identical. However if an editor content was loaded
    // from a temporary file that has been created by the ex-
    // tractor m_strBaseFile denotes the file that has been
    // passed to the extractor and m_strDocumentFile denotes
    // the file that has been created by the extractor. An
    // example is the header part of the 4gr editor.
    CString m_strBaseFile;
    bool    m_bReadonlyCached;
    bool    m_bReadonlyCache;

// Generated message map functions
protected:
	//{{AFX_MSG(CStdEditorDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CStdEditorDoc)
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

    BEGIN_INTERFACE_PART(CEEditObj, ICEEdit)
	    STDMETHOD(LoadDocument)(/*[in]*/ BSTR szDocName);
	    STDMETHOD(SaveDocument)(/*[in]*/ BSTR szDocName);
		STDMETHOD(GetModified)();
		STDMETHOD(SetModified)(/*[in]*/ BOOL bModified);
		STDMETHOD(GetPosition)(/*[out]*/EDP_POS_QUALIFIER* posQual, /*[out]*/ long* lX, /*[out]*/ long* lY, /*[out]*/ long* lCX, /*[out]*/ long* lCY);
		STDMETHOD(GotoPosition)(EDP_POS_QUALIFIER posQual, /*[in]*/ long lX, /*[in]*/ long lY, /*[in]*/ long lCX, /*[in]*/ long lCY);
		STDMETHOD(ForceReadOnly)(/*[in]*/ BOOL bOn);
		STDMETHOD(SetLogicalColor)(/*[in]*/ unsigned int iKind, /*[in]*/ OLE_COLOR color);
		STDMETHOD(SetFont)(/*[in]*/ BSTR font, int iSize, int iFlags);
		STDMETHOD(SetClientExtent)(/*[in]*/ long lWidth, /*[in]*/ long lHeight);
		STDMETHOD(GotoLocation)(/*[in]*/ BSTR szLocation);
		STDMETHOD(FindFirst)(/*[in, string]*/ BSTR sStringToSearch, /*[in]*/ UINT uiOption, /*[out, string]*/ BSTR *psLocation);
		STDMETHOD(FindNext)(/*[out, string]*/ BSTR *psLocation);
		STDMETHOD(SetPrintInfo)(/*[in, string]*/ BSTR sPrinterName, /*[in]*/ long lWidth, /*[in]*/ long lHeight);
		STDMETHOD(GetPageCount)(/*[out]*/ long *lCount);
		STDMETHOD(GetPages)(/*[out]*/ SAFEARRAY/*(BSTR)*/ **pPages);
        STDMETHOD(SetBaseFile)(/*[in, string]*/ BSTR szBaseFile);
        STDMETHOD(Lock)(/*[in]*/ BOOL bLock);
        STDMETHOD(GetSelectedText)(/*[out, string]*/ BSTR* psText);
    END_INTERFACE_PART(CEEditObj)

    BEGIN_INTERFACE_PART(CEPrintObj, ICEPrint)
        STDMETHOD(PRLoadDocument)(/*[in]*/ BSTR sFilePathName);
        STDMETHOD(PRSetPrintInfo)(/*[in, string]*/ BSTR sPrinterName, /*[in]*/ long lWidth, /*[in]*/ long lHeight);
        STDMETHOD(PRGetModuleName)(/*[out,string]*/ BSTR* psModuleName);
        STDMETHOD(PRGetPrintOptions)(/*[out, string]*/ BSTR* psOptions);
        STDMETHOD(PRSetPrintOptions)(/*[in, string]*/ BSTR sOptions);
        STDMETHOD(PRShowPrintOptionsDialog)(/*[out, string]*/ BSTR* psOptions);
        STDMETHOD(PRGetPageCount)(/*[out]*/ long *lCount);
        STDMETHOD(PRGetPages)(/*[out]*/ SAFEARRAY/*(BSTR)*/ **ppsaPages);
        STDMETHOD(PRGetXMLString)(/*[out, string]*/ BSTR* psXMLString);
    END_INTERFACE_PART(CEPrintObj)

    DECLARE_INTERFACE_MAP()

	BEGIN_CONNECTION_PART(CStdEditorDoc, CEEditNotifyConnPt)
		CONNECTION_IID (IID_ICEEditNotify)
		virtual void OnAdvise (BOOL bAdvise);
	END_CONNECTION_PART(CEEditNotifyConnPt)

	BEGIN_CONNECTION_PART(CStdEditorDoc, CEMessageEventConnPt)
		CONNECTION_IID (IID__ICEMessageEvent)
		virtual void OnAdvise (BOOL bAdvise);
	END_CONNECTION_PART(CEMessageEventConnPt)

	DECLARE_CONNECTION_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDEDITORDOC_H__CC73FC7F_F147_11D1_882F_006097E1BE6E__INCLUDED_)


