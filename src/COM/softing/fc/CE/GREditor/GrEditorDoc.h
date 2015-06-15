#if !defined(AFX_GREDITORDOC_H__59119A21_6422_11D2_9A3D_0000C0D26FBC__INCLUDED_)
#define AFX_GREDITORDOC_H__59119A21_6422_11D2_9A3D_0000C0D26FBC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*doc*=======================================================================*/
/*inc*=======================================================================*/

#include "GrEditorIF.h"
#include "PrgEditorDoc.h"
#include "Symbols.h"

#include "ElemPtrList.h"
#include "GrEditorAction.h"

/*const*=====================================================================*/
/*macro*=====================================================================*/
/*type*======================================================================*/
/*glovar*====================================================================*/
/*proto*=====================================================================*/

class CGrEditorView;
class CGrEditorSrvrItem;

class CGrEditorDoc : public CXMLDocument<CPrgEditorDoc>
{
protected: // create from serialization only
   CGrEditorDoc();
   DECLARE_DYNCREATE(CGrEditorDoc)

// Attributes
public:
   // Hints for UpdateAllViews/OnUpdate

    static const int UpdateWindowHint;
    static const int UpdateDrawHint;
    static const int UpdateDrawElemHint;
    static const int UpdateSelectionHint;
    static const int UpdateMarkedElemHint;
    static const int UpdateOleItemsHint;
    static const int UpdateRubberHint;
    static const int UpdateFixPointsHint;
    static const int UpdateLightPinHint;
    static const int UpdateModifiedHint;
    static const int UpdateColorHint;
    static const int EditNameStopHint;
    static const int UpdateStatusHint;
    static const int SetPageSizeHint;
    static const int UpdateSTActionsHint;
    static const int SyncSTActionsHint;
    static const int VarSelStopHint;
    static const int StartLDTracingHint;
    static const int StopLDTracingHint;

    CGrEditorSrvrItem* GetEmbeddedItem()
    { 
       return (CGrEditorSrvrItem*)CPrgEditorDoc::GetEmbeddedItem(); 
    }

protected: 
   CString    m_Header;         // save header in stand alone mode
   CElemList  m_Elem;           // 
   CString    m_strFont;        // name of font
   int        m_iFontHeight;    // size of font in points
   CSize      m_PageSize;       // size of page (logical)
   CSize      m_DocSize;        // size of doc  (logical)
   CSize      m_PageGrid;       // size of page in grid 
   CSize      m_DocGrid;        // size of doc in grid 
   int        m_iMargLeft;      // margins in inch * 100
   int        m_iMargTop; 
   int        m_iMargRight;
   int        m_iMargBottom;
   CRect      m_Margin0;        // margin rect before rounding (logical)
   CRect      m_Margin;         //             after rounding  (logical)
   
   GRID_TYPE  m_eGrid;          // none point cross line
   int        m_iGrid;          // distance of grid 
   EDI_MODE   m_eEditMode;      // move select line resize ..
   LINE_TYPE  m_eLineMode;      // ortho rubber
   
   EDIT_LANG  m_eCurrLang;      // current type of language !!! MUST BE MOVED TO CElemPtrList !!!
   CString    m_sCurrLang;      // current type of language
   bool       m_bEnableSFC;     // flag for SFC option
   bool       m_bRecon;         // flag: reconnect at move/resize
   bool       m_bNoDelContents; // no call of DelContents at LoadDocument !!
   bool       m_bStandAloneDoc; // no call of DelContents at LoadDocument !!
   bool       m_bPrintSTHeader; // print ST-header?
   
   static bool m_bFindNoWords;  // work around .. for display messages
   static bool m_bFindNoRegEx;  
   
   CSECEditor m_HeaderEditor;
   CArray<CSECEditor*,CSECEditor*> m_Editors;
   
   // Operations
public:
   bool IsModeNormal ();
   bool IsModeMonitor ();
   bool IsStandAloneDoc () { return m_bStandAloneDoc; }
   
   void  GetSTViewText();
   void  SetHeaderViewText();
   CView* GetSTViewRef(CPoint context);
   
   bool CheckInPlaceActive(){return IsInPlaceActive()==TRUE;}
   
   CSECEditor* GetEditor(UINT id);
   UINT        CreateEditor(const CString& FileMappingKey);
   void        DeleteEditor(UINT id);
   void        DeleteAllEditors();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGrEditorDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void Serialize(CArchive& ar);
	virtual void DeleteContents();
	protected:
	virtual COleServerItem* OnGetEmbeddedItem();
	virtual BOOL SaveModified();
	virtual COleIPFrameWnd* CreateInPlaceFrame(CWnd* pParentWnd);
	virtual void DestroyInPlaceFrame(COleIPFrameWnd* pFrameWnd);
	//}}AFX_VIRTUAL

// Implementation
public:
   CRect        m_RegionError;      // region with error (overlapping nets)
   CAction       m_Action;
   float         m_fZoom;
   
   virtual ~CGrEditorDoc();
   
   LPCTSTR       GetFontName () { return (LPCTSTR)m_strFont; }
   LINE_TYPE     GetLineMode () { return m_eLineMode;        }
   CElemList *   GetElemList () { return &m_Elem;            }
   GRID_TYPE     GetGridType () { return m_eGrid;            }
   int           GetGridSize ();
   EDI_MODE      GetEditMode () { return m_eEditMode;        }
   void          SetEditMode (EDI_MODE eNewMode);
   bool          GetReconMode() { return m_bRecon;           }
   bool          GetSfcMode  () { return m_bEnableSFC;       }
   void          SetSfcMode  (bool eNewMode);
   EDIT_LANG     GetCurrLang () { return m_eCurrLang;        }
   void          SetCurrLang (EDIT_LANG eCurrLang);
   
   bool         ConvertPar( LPTSTR pszLine, long *lValues, int iCount);
   void         SetColor(bool bScreen, COLOR_TYPE ColorType);
   HRESULT      ComputePageSize();
   void         InitPageSize(void);
   void         InitPageSize(CDC* pDC);
   const CSize& GetPageSize () const { return m_PageSize;   }
   const CSize& GetDocSize  () const { return m_DocSize;    }
   const CSize& GetPageGrid () const { return m_PageGrid;   }
   const CSize& GetDocGrid  () const { return m_DocGrid;    }
   const CRect& GetMargin   () const { return m_Margin;     }
   float        SetZoom     (float fZoom);
   void         SetMarginRect(BOOL bReduce);
   void         PreparePrintDC(CDC* pDC, int iPageNr);
   void         ToggleFixPoint(CPoint P);
   
   CString GetSmartTextGr(const CStdEditorPosition &position, CStdEditorPosition &startPosition, CStdEditorPosition &endPosition, bool bOnlyAbbrText=false, int iGrid=1);
   CString GetSmartText  (const CStdEditorPosition &position, CStdEditorPosition &startPosition, CStdEditorPosition &endPosition);

   // d-2155 22.03.05 SIS >>
   virtual CString GetTrimmedText (const CStdEditorPosition &startPosition, const CStdEditorPosition &endPosition, CStdEditorPosition &trimmedStart, CStdEditorPosition &trimmedEnd);
   // d-2155 22.03.05 SIS <<
   
   HRESULT DecodeLocation (const CString &strLocation, CString &strType, CStdEditorPosition &position, CPoint &endPos);
   
   void SystemMessage (HRESULT hr);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

    // SIS, 25.09.01: get selected text for find in files dialog
    virtual CString GetSelectedText();

protected:
   virtual CDocObjectServer* GetDocObjectServer(LPOLEDOCUMENTSITE pDocSite);
   
   virtual bool LoadDocument (const CString &strFileName);
   virtual bool SaveDocument (const CString &strFileName);
   virtual void SetModified  (bool bModified);
   virtual void OnSerializeDocument (CArchive &ar);
   virtual void SerializeXML(CXMLArchive& ar);
   virtual void OnUpdateReadOnly ();
   virtual void OnUpdateModified ();
   virtual void OnIdle ();
   
   virtual void OnShowBreakpoint (CBreakpoint *pBP);
   virtual void OnHideBreakpoint (CBreakpoint *pBP);
   virtual void OnShowStoppoint ();
   virtual void OnHideStoppoint ();
   
   bool m_bSpecialWPDisplay;
   long m_lWPDisplayX;
   long m_lWPDisplayY;
   long m_lWPDisplayOffsetX;
   long m_lWPDisplayOffsetY;
   
   virtual HRESULT FindFirst (const CString &strStringToSearch, UINT uiOption, CString &strLocation);
   virtual HRESULT FindNext (CString &strLocation);
   
   virtual HRESULT SetPrintInfo (const CString &strPrinterName, long lWidth, long lHeight);
   virtual HRESULT GetPageCount (long &lCount);
   virtual HRESULT GetPages (CStringArray &pages);

   virtual HRESULT ShowPrintOptionsDialog(CString& rstrOptions);
   virtual HRESULT GetPrintOptions(CString& rstrOptions);
   virtual HRESULT SetPrintOptions(const CString& crstrOptions);
   
   //print support for CSTEditorView instances
   long    GetPrintLinesPerPage_ST(CDC *pDCPrint);
   HRESULT GetPageCount_ST (CSECEditor &Edit, CDC &dcPrint, long &lCount);
   HRESULT GetPages_ST     (CSECEditor& Edit, CDC &dcPrint, CStringArray &pages);
   HRESULT AttachPrinterDC (CDC *pDC);
   
   virtual void SetInstance (const CString &strInstanceName, const CString &strMachineName, const CString &strMachineAddress);
   
   virtual HRESULT StartLDTracing (void);
   virtual HRESULT StopLDTracing (void);
   
   // Generated message map functions
protected:
   //{{AFX_MSG(CGrEditorDoc)
   afx_msg void OnViewGrid();
   afx_msg void OnOptLinerubb();
   afx_msg void OnUpdateOptLinerubb(CCmdUI* pCmdUI);
   afx_msg void OnOptColnormal();
   afx_msg void OnOptColselect();
   afx_msg void OnOptColerror();
   afx_msg void OnOptColcomment();
   afx_msg void OnOptColrubber();
   afx_msg void OnOptColeditline();
   afx_msg void OnOptColgrid();
   afx_msg void OnOptColsheet();
   afx_msg void OnOptColelembk();
   afx_msg void OnOptColladdbk();
   afx_msg void OnOptColfblkbk();
   afx_msg void OnOptColedge();
   afx_msg void OnOptCollightpin();
   afx_msg void OnOptColbrkrec();
   afx_msg void OnOptColbrkdis();
   afx_msg void OnOptColbrkact();
   afx_msg void OnOptColbrkdec();
   afx_msg void OnOptColbackgd_sfc();
   afx_msg void OnOptColelembk_sfc();
   afx_msg void OnOptColladdbk_sfc();
   afx_msg void OnOptColfblkbk_sfc();
   afx_msg void OnOptColactive_sfc();
   afx_msg void OnOptColforced_sfc();
   afx_msg void OnOptColblockd_sfc();
   afx_msg void OnOptFont();
   afx_msg void OnOptRecon();
   afx_msg void OnUpdateOptRecon(CCmdUI* pCmdUI);
   afx_msg void OnUpdateZoom(CCmdUI* pCmdUI);
   afx_msg void OnAppStatistic();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
      DECLARE_OLECMD_MAP()
      
      // Generated OLE dispatch map functions
      //{{AFX_DISPATCH(CGrEditorDoc)
      // NOTE - the ClassWizard will add and remove member functions here.
      //    DO NOT EDIT what you see in these blocks of generated code !
      //}}AFX_DISPATCH
      DECLARE_DISPATCH_MAP()
      DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


#endif // !defined(AFX_GREDITORDOC_H__59119A21_6422_11D2_9A3D_0000C0D26FBC__INCLUDED_)
