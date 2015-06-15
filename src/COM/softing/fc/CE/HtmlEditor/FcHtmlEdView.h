
// FcHtmlEdView.h : interface of the CFcHtmlEdView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FCHTMLEDVIEW_H__A04CED84_B51D_11D2_9FD9_00A024363DFC__INCLUDED_)
#define AFX_FCHTMLEDVIEW_H__A04CED84_B51D_11D2_9FD9_00A024363DFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct	IHTMLDocument2;
struct  IHTMLTxtRange;

class CFcFontBar;
class CFcHtmlEdView : public CView, public IOleInPlaceFrame, public IAdviseSink, public IOleCommandTarget
{
protected: // create from serialization only
	CFcHtmlEdView();
	DECLARE_DYNCREATE(CFcHtmlEdView)

// Attributes
public:
	CFcHtmlEdDoc* GetDocument();

// Operations
public:
	BOOL Save();
	BOOL Load();
	HRESULT ExecTriEditCommand(ULONG ucmdID, VARIANT* pVarIn, VARIANT* pVarOut, BOOL bPromptUser);
	DWORD QueryTriEditCommand(ULONG ucmdID);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFcHtmlEdView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFcHtmlEdView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CFcHtmlEdView)
	afx_msg void OnCancelEditSrvr();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnFontNameChange();
	afx_msg void OnFontSizeChange();
	afx_msg void OnHtmlTagChange();
	afx_msg void OnFilePrint();
	afx_msg void OnEditClear();
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	afx_msg void OnEditFind();
	afx_msg void OnEditSelectAll();
	afx_msg void OnViewOptions();
	afx_msg void OnInsertPicture();
	afx_msg void OnInsertAnchor();
	afx_msg void OnInsertHtml();
	afx_msg void OnFormatAlignLeft();
	afx_msg void OnFormatAlignCenter();
	afx_msg void OnFormatAlignRight();
	afx_msg void OnFormatFont();
	afx_msg void OnFormatStyle();
	afx_msg void OnFormatBold();
	afx_msg void OnFormatItalic();
	afx_msg void OnFormatUnderline();
	afx_msg void OnFormatBullets();
	afx_msg void OnFormatNumber();
	afx_msg void OnFormatPage();
	afx_msg void OnFormatIncreaseIndent();
	afx_msg void OnFormatDecreaseIndent();
	afx_msg void OnFormatTextColor();
	afx_msg void OnFormatAbsolute();
	afx_msg void OnFormatLock();
	afx_msg void OnFormatGrid();
	afx_msg void OnZorderBackward();
	afx_msg void OnZorderBehindtext();
	afx_msg void OnZorderForward();
	afx_msg void OnZorderFrontoftext();
	afx_msg void OnZorderToBack();
	afx_msg void OnZorderToFront();
	afx_msg void OnTableInsertTable();
	afx_msg void OnTableInsertCell();
	afx_msg void OnTableInsertCol();
	afx_msg void OnTableInsertRow();
	afx_msg void OnTableMergeCells();
	afx_msg void OnTableSplitCell();
	afx_msg void OnTableDeleteCells();
	afx_msg void OnTableDeleteCols();
	afx_msg void OnTableDeleteRows();
	afx_msg void OnUpdateFilePrint(CCmdUI* pCmdUI);	
	afx_msg void OnUpdateEditClear(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditFind(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditSelectAll(CCmdUI* pCmdUI);
	afx_msg void OnUpdateInsertPicture(CCmdUI* pCmdUI);
	afx_msg void OnUpdateInsertAnchor(CCmdUI* pCmdUI);
	afx_msg void OnUpdateInsertHtml(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTableInsertTable(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTableInsertCell (CCmdUI* pCmdUI);
	afx_msg void OnUpdateTableInsertCol  (CCmdUI* pCmdUI);
	afx_msg void OnUpdateTableInsertRow  (CCmdUI* pCmdUI);
	afx_msg void OnUpdateTableMergeCells (CCmdUI* pCmdUI);
	afx_msg void OnUpdateTableSplitCell  (CCmdUI* pCmdUI);
	afx_msg void OnUpdateTableDeleteCells(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTableDeleteCols (CCmdUI* pCmdUI);
	afx_msg void OnUpdateTableDeleteRows (CCmdUI* pCmdUI);
	afx_msg void OnUpdateFormatStyle(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFormatFont(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFormatDecreaseindent(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFormatIncreaseindent(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFormatBold(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFormatAlignCenter(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFormatAlignLeft(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFormatAlignRight(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFormatBullets(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFormatItalic(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFormatNumber(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFormatUnderline(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFormatColor(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFormatAbsolute(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFormatLock(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFormatGrid(CCmdUI* pCmdUI);
	afx_msg void OnUpdateZorderBackward(CCmdUI* pCmdUI);
	afx_msg void OnUpdateZorderBehindtext(CCmdUI* pCmdUI);
	afx_msg void OnUpdateZorderForward(CCmdUI* pCmdUI);
	afx_msg void OnUpdateZorderFrontoftext(CCmdUI* pCmdUI);
	afx_msg void OnUpdateZorderToBack(CCmdUI* pCmdUI);
	afx_msg void OnUpdateZorderToFront(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private :
        ULONG           m_cRef;
		HWND			m_hWndObj;			// The object's window
		unsigned        m_ChangeCount;
        BOOL            m_bLoaded;          // is set if document is loaded

        IOleInPlaceActiveObject *m_pIOleIPActiveObject;

public:
        class CSite*	m_pSite;            //Site holding object        
		LPUNKNOWN GetTriEditObject();

		BOOL SetGrid(unsigned Horz, unsigned Vert, BOOL bActive);

		//Shared IUnknown implementation
        STDMETHODIMP         QueryInterface(REFIID, void **);
        STDMETHODIMP_(ULONG) AddRef(void);
        STDMETHODIMP_(ULONG) Release(void);

		//IOleInPlaceFrame implementation
        STDMETHODIMP         GetWindow(HWND *);
        STDMETHODIMP         ContextSensitiveHelp(BOOL);
        STDMETHODIMP         GetBorder(LPRECT);
        STDMETHODIMP         RequestBorderSpace(LPCBORDERWIDTHS);
        STDMETHODIMP         SetBorderSpace(LPCBORDERWIDTHS);
        STDMETHODIMP         SetActiveObject(LPOLEINPLACEACTIVEOBJECT
                                 , LPCOLESTR);
        STDMETHODIMP         InsertMenus(HMENU, LPOLEMENUGROUPWIDTHS);
        STDMETHODIMP         SetMenu(HMENU, HOLEMENU, HWND);
        STDMETHODIMP         RemoveMenus(HMENU);
        STDMETHODIMP         SetStatusText(LPCOLESTR);
        STDMETHODIMP         EnableModeless(BOOL);
        STDMETHODIMP         TranslateAccelerator(LPMSG, WORD);

		//IAdviseSink implementation
        STDMETHODIMP_(void)  OnDataChange(LPFORMATETC, LPSTGMEDIUM);
        STDMETHODIMP_(void)  OnViewChange(DWORD, LONG);
        STDMETHODIMP_(void)  OnRename(LPMONIKER);
        STDMETHODIMP_(void)  OnSave();
        STDMETHODIMP_(void)  OnClose();
		
		//IOleCommandTarget
        STDMETHODIMP QueryStatus(const GUID *pguidCmdGroup, ULONG cCmds
            , OLECMD prgCmds[], OLECMDTEXT *pCmdText);
        
        STDMETHODIMP Exec(const GUID *pguidCmdGroup, DWORD nCmdID
            , DWORD nCmdexecopt, VARIANTARG *pvaIn, VARIANTARG *pvaOut);
private:
	BOOL CreateDocObject( TCHAR* pchPath );
	void InitHtmlTagNames();
	void UpdateFontBar();
	void UpdateUIItem(CCmdUI* pCmdUI, DWORD nCmdID);
	BOOL InsertHtml(LPCSTR HtmlString);
	CFcFontBar* GetFontBar();
};

#ifndef _DEBUG  // debug version in FcHtmlEdView.cpp
inline CFcHtmlEdDoc* CFcHtmlEdView::GetDocument()
   { return (CFcHtmlEdDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FCHTMLEDVIEW_H__A04CED84_B51D_11D2_9FD9_00A024363DFC__INCLUDED_)
