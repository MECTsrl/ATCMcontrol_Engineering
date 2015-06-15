/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CELibrarianView.cpp 1     28.02.07 18:59 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CELibrarianView.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CELibrarianView.cpp $
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



//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include <afxadv.h>
#include "CELibrarian.h"

#include "CELibrarianDoc.h"
#include "CELibrarianView.h"
#include "FBListCtrl.h"
#include "CMainFrm.h"
#include "DragItem.h" 
#include "cedb.h"
//----  Static Initializations:   ----------------------------------*



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

 
// drag and drop
//#define CELIB_CLIP_FORMAT _T("CELIB_CLIP_FORMAT")
#define BITMAP_WIDTH 20

/////////////////////////////////////////////////////////////////////////////
// CELibrarianView

IMPLEMENT_DYNCREATE(CELibrarianView, CView)

BEGIN_MESSAGE_MAP(CELibrarianView, CView)
	//{{AFX_MSG_MAP(CELibrarianView)
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_COMMAND(ID_CANCEL_EDIT_CNTR, OnCancelEditCntr)
	ON_WM_CREATE()
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_NEXT_PANE, OnNextPane)
	//}}AFX_MSG_MAP
	// Standard-Druckbefehle
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CELibrarianView Konstruktion/Destruktion

CELibrarianView::CELibrarianView()
{
	/* server eingebaut
	m_pSelection = NULL;
	*/
	
	// new List Control
	m_pFBListCtrl = new CFBListCtrl(this);
	
	// clipboard format id
	m_uiClipFormatId = RegisterClipboardFormat(CELIB_CLIP_FORMAT);

	m_lockDrop = FALSE;
	m_pMainFrame = NULL;

}

CELibrarianView::~CELibrarianView()
{
	if ( m_pFBListCtrl != NULL )
		delete m_pFBListCtrl;
}

BOOL CELibrarianView::PreCreateWindow(CREATESTRUCT& cs)
{
	// ZU ERLEDIGEN: Ändern Sie hier die Fensterklasse oder das Erscheinungsbild, indem Sie
	//  CREATESTRUCT cs modifizieren.

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CELibrarianView Zeichnen

void CELibrarianView::OnDraw(CDC* pDC)
{
	CELibrarianDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// ZU ERLEDIGEN: Hier Code zum Zeichnen der ursprünglichen Daten hinzufügen
	// ZU ERLEDIGEN: Auch alle OLE-Elemente im Dokument zeichnen

	// Auswahl an beliebiger Position zeichnen. Dieser Code sollte entfernt
	//  werden, sobald Ihr tatsächlicher Zeichencode implementiert ist. Diese Position
	//  entspricht exakt dem von CELibrarianCntrItem zurückgegebenen Rechteck,
	//  um den Effekt der direkten Bearbeitung zu gewährleisten.

	// ZU ERLEDIGEN: Entfernen Sie diesen Code, sobald Ihre richtige Zeichenroutine vollständig ist.
	/* server eingebaut
	if (m_pSelection == NULL)
	{
		POSITION pos = pDoc->GetStartPosition();
		m_pSelection = (CELibrarianCntrItem*)pDoc->GetNextClientItem(pos);
	}
	if (m_pSelection != NULL)
		m_pSelection->Draw(pDC, CRect(10, 10, 210, 210));
	*/
}

void CELibrarianView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	
	static bool init = true; 
	if (init) {
		m_pFBListCtrl->ShowWindow(SW_SHOW);
		init = false;
		m_pFBListCtrl->InitColumns();
		// 
		// image list
		//
		//m_ClImageList.Create(IDB_NAVMAGELIST, BITMAP_WIDTH, 0, RGB(255, 0, 255));
        //OH: Get the Images from the KAD
        m_pFBListCtrl->m_pKADInfo->CopyImageList(m_ClImageList);
		m_ClImageList.SetBkColor(GetSysColor(COLOR_WINDOW));
		m_pFBListCtrl->SetImageList(&m_ClImageList, LVSIL_SMALL);

		RECT rect;
		GetClientRect(&rect);
		//
		// set size of third row
		//
		int cx01 = m_pFBListCtrl->GetColumnWidth(0) + m_pFBListCtrl->GetColumnWidth(1);
		BOOL rv=m_pFBListCtrl->SetColumnWidth(2,rect.right-cx01);
	}

	m_pFBListCtrl->Refresh();
}

/////////////////////////////////////////////////////////////////////////////
// CELibrarianView Drucken

BOOL CELibrarianView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Standardvorbereitung
	return DoPreparePrinting(pInfo);
}

void CELibrarianView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// ZU ERLEDIGEN: Zusätzliche Initialisierung vor dem Drucken hier einfügen
}

void CELibrarianView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// ZU ERLEDIGEN: Hier Bereinigungsarbeiten nach dem Drucken einfügen
}

void CELibrarianView::OnDestroy()
{
	// Das Element bei Destruktion deaktivieren; dies ist wichtig,
	// wenn eine Teilansicht (Splitter View) verwendet wird.
   CView::OnDestroy();
   COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
   if (pActiveItem != NULL && pActiveItem->GetActiveView() == this)
   {
      pActiveItem->Deactivate();
      ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
   }
}


// Der folgende Befehls-Handler stellt die Standardtastatur als
//  Benutzerschnittstelle zum Abbruch der direkten Bearbeitungssitzung zur Verfügung. Hier
//  verursacht der Container (nicht der Server) die Deaktivierung.
void CELibrarianView::OnCancelEditCntr()
{
	// Schließen aller direkt aktiven Elemente dieser Ansicht.
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL)
	{
		pActiveItem->Close();
	}
	ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
}

// Für einen Container müssen OnSetFocus und OnSize speziell gehandhabt werden,
//  wenn ein Objekt direkt bearbeitet wird.
void CELibrarianView::OnSetFocus(CWnd* pOldWnd)
{
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL &&
		pActiveItem->GetItemState() == COleClientItem::activeUIState)
	{
		// dieses Element muss den Fokus erhalten, wenn es sich in der gleichen Ansicht befindet
		CWnd* pWnd = pActiveItem->GetInPlaceWindow();
		if (pWnd != NULL)
		{
			pWnd->SetFocus();   // kein Aufruf der Basisklasse
			return;
		}
	}
	CView::OnSetFocus(pOldWnd);
	if (m_pFBListCtrl != NULL)
		m_pFBListCtrl->SetFocus();
}

void CELibrarianView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL)
		pActiveItem->SetItemRects();

	// set size of list control
	m_pFBListCtrl->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOMOVE);

	//
	// set size of third row
	//
	int cx01 = m_pFBListCtrl->GetColumnWidth(0) + m_pFBListCtrl->GetColumnWidth(1);
	BOOL rv=m_pFBListCtrl->SetColumnWidth(2,cx-cx01);
}

/////////////////////////////////////////////////////////////////////////////
// CELibrarianView Diagnose

#ifdef _DEBUG
void CELibrarianView::AssertValid() const
{
	CView::AssertValid();
}

void CELibrarianView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CELibrarianDoc* CELibrarianView::GetDocument() // Die endgültige (nicht zur Fehlersuche kompilierte) Version ist Inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CELibrarianDoc)));
	return (CELibrarianDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CELibrarianView Nachrichten-Handler

/////////////////////////////////////////////////////////////////////
/**
 * Zusammenfassung der Funktion in einem Satz.
 * Spezifikation der Gesamtfunktion abcd
 * Spezifikation der Gesamtfunktion abcd
 * Spezifikation der Gesamtfunktion abcd
 * evtl. mit <B>HTML</B>-kommandos dazwischen
 *
 * @param a     Erklärung zu a ....
 *              immer noch Erklärung zu a ....
 *              immer noch Erklärung zu a ....
 * @param b     Erklärung zu b ....
 *              unchanged on output
 * @param c     Erklärung zu c ....
 * @return 0-9  Erklärung zum return value; 
 *              ok-Fall; Fehlerfall....
 * @exception   COutOfMemory
 * @see         ...
 */
int CELibrarianView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// drag und drop zulassen
	m_dropTarget.Register(this);

	CRect rect;
	GetClientRect(rect);
	m_pFBListCtrl->Create(LVS_REPORT | LVS_SHOWSELALWAYS, rect, this, 1);	
	return 0;
}

/////////////////////////////////////////////////////////////////////
/**
 * return dropeffect dependent on pDataObject
 *
 * @param pDataObject  
 * @param dwKeyState
 * @param point
 * @return				DROPEFFECT_COPY if object can be dropped
 *						DROPEFFECT_NONE if object can't be dropped
 * @exception   
 * @see        
 */
DROPEFFECT CELibrarianView::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
{
	DROPEFFECT de;

	if ( m_lockDrop == TRUE )
		return DROPEFFECT_NONE;

	if(pDataObject->IsDataAvailable((USHORT)m_uiClipFormatId))
		de = DROPEFFECT_COPY;
	else
		de = DROPEFFECT_NONE;

	return de;
}

/////////////////////////////////////////////////////////////////////
/**
 * insert dropped item to library
 *
 * @param pDataObject  
 * @param dwKeyState
 * @param point
 * @return				TRUE ok
 *						FALSE not ok
 * @exception
 * @see       
 */
BOOL CELibrarianView::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point) 
{
	//check first wether the data is available - use stored clipboard format here (2)

	if(pDataObject->IsDataAvailable((USHORT) m_uiClipFormatId) && (m_lockDrop == FALSE)) {
			
		//get global handle from the data object
		HGLOBAL hGlob = pDataObject->GetGlobalData((USHORT) m_uiClipFormatId);
		if(hGlob != NULL) {
			//use a shared file for read operation
			CSharedFile GlobFile;
			GlobFile.SetHandle(hGlob, FALSE); //init shared file with global handle
				
			//init archive
			CArchive ar(&GlobFile, CArchive::load);
			CDragItem DragItem;
			//use serialize method
			DragItem.Serialize(ar);
	        //now the data is inside DragItem...
			// add it to library
            int rv;
            if (DragItem.m_lObjClass == CEDB_MultimediaObj)
            {
                rv = m_pFBListCtrl->AddAddFile(DragItem.m_astrFilePath);
            }
		    else
            {
			    rv = m_pFBListCtrl->AddFBLink(DragItem.m_strObjName, DragItem.m_strKeyword, 
								DragItem.m_astrFilePath);
            }
			if ( rv == 1 )
				GetDocument()->SetModifiedFlag(TRUE);
		}
	}
	
	return CView::OnDrop(pDataObject, dropEffect, point);
}

void CELibrarianView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Speziellen Code hier einfügen und/oder Basisklasse aufrufen
	
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

BOOL CELibrarianView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Code für die Behandlungsroutine für Nachrichten hier einfügen und/oder Standard aufrufen
	
	//return CView::OnMouseWheel(nFlags, zDelta, pt);
	return TRUE;
}

void CELibrarianView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Speziellen Code hier einfügen und/oder Basisklasse aufrufen
	if (m_pFBListCtrl)
		m_pFBListCtrl->Refresh();	
}

void CELibrarianView::LockDrop(BOOL lockDrop)
{
	m_lockDrop = lockDrop;
}


void CELibrarianView::OnNextPane() 
{
	// TODO: Add your command handler code here
	if (m_pMainFrame != 0)
		m_pMainFrame->OnNextPane();
}

HRESULT CELibrarianView::ShowPouHeader(const CString& strPou)
{
	if (m_pFBListCtrl == NULL)
		return E_FAIL;
	if ( m_pFBListCtrl->DoShowFBHeader(strPou) )
		return S_OK;
	else
		return S_FALSE;
}

HRESULT CELibrarianView::SelectPou(const CString& strPou)
{
	if (m_pFBListCtrl == NULL)
		return E_FAIL;
	if ( m_pFBListCtrl->DoSelectFB(strPou) )
		return S_OK;
	else
		return S_FALSE;
}

/*
 *----------------------------------------------------------------------------*
 *  $History: CELibrarianView.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 18:59
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:31
 * Created in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 * 
 * *****************  Version 4  *****************
 * User: Oh           Date: 7/10/01    Time: 11:58a
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 * 
 * *****************  Version 3  *****************
 * User: Oh           Date: 7/05/01    Time: 5:27p
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 * 
 * *****************  Version 2  *****************
 * User: Oh           Date: 6/25/01    Time: 3:32p
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 *==
 *----------------------------------------------------------------------------*
*/
