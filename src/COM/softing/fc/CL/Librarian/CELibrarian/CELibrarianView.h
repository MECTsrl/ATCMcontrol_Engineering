/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CELibrarianView.h 1     28.02.07 18:59 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CELibrarianView.h $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CELibrarianView.h $
 *
 * =PROJECT             CAK1020 ATCMControlV2.0
 *
 * =SWKE                4CL
 *
 * =COMPONENT           CELibrarian
 *
 * =CURRENT      $Date: 28.02.07 18:59 $
 *               $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *





 *==
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    @DESCRIPTION@
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  6/25/2001  SU      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/

#ifndef __CELIBRARIANVIEW_H_
#define __CELIBRARIANVIEW_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CELibrarianCntrItem;
class CFBListCtrl;
class CELibrarianDoc;
class CMainFrame;

class CELibrarianView : public CView
{
protected: // Nur aus Serialisierung erzeugen
	CELibrarianView();
	DECLARE_DYNCREATE(CELibrarianView)

// Attribute
public:
	CELibrarianDoc* GetDocument();
	void LockDrop(BOOL lock);
	HRESULT ShowPouHeader(const CString& strPou);
	HRESULT SelectPou(const CString& strPou);

	// m_pSelection enthält die Auswahl des aktuellen CELibrarianCntrItem-Objekts.
	// Für viele Anwendungen ist eine derartige Member-Variable nicht angemessen, um
	//  z.B. eine Mehrfachauswahl oder eine Auswahl von Objekten zu repräsentieren,
	//  die keine CELibrarianCntrItem-Objekte sind. Dieser Auswahlmechanismus
	//  dient nur dazu, Ihnen bei den ersten Schritten zu helfen.

	/* server eingebaut
	// ZU ERLEDIGEN: Ersetzen Sie diesen Auswahlmechanismus durch einen für Ihre Anwendung geeigneten.
	CELibrarianCntrItem* m_pSelection;
	*/

// Operationen
public:

// Überladungen
	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(CELibrarianView)
	public:
	virtual void OnDraw(CDC* pDC);  // überladen zum Zeichnen dieser Ansicht
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	protected:
	virtual void OnInitialUpdate(); // das erste mal nach der Konstruktion aufgerufen
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementierung
public:
	virtual ~CELibrarianView();
	CMainFrame* m_pMainFrame;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generierte Message-Map-Funktionen
protected:
	CFBListCtrl* m_pFBListCtrl;
	CImageList m_ClImageList;
	UINT m_uiClipFormatId;
	COleDropTarget m_dropTarget;
	BOOL m_lockDrop;
	
	//{{AFX_MSG(CELibrarianView)
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCancelEditCntr();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnNextPane();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // Testversion in CELibrarianView.cpp
inline CELibrarianDoc* CELibrarianView::GetDocument()
   { return (CELibrarianDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.



#endif // __CELIBRARIANVIEW_H_


/*
 *----------------------------------------------------------------------------*
 *  $History: CELibrarianView.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 18:59
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:31
 * Created in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 * 
 * *****************  Version 3  *****************
 * User: Oh           Date: 7/10/01    Time: 11:58a
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 * 
 * *****************  Version 2  *****************
 * User: Oh           Date: 6/25/01    Time: 3:32p
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 *==
 *----------------------------------------------------------------------------*
*/
