


//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*

// GenEdView.cpp : implementation of the CGenEdView class
//

#include "stdafx.h"
#include <afxodlgs.h>
#include "utilif.h"
#include "CEMain.h"

#include "GenEdDoc.h"
#include "GenEdCntrItem.h"
#include "GenEdView.h"

#include "MainFrm.h"
#include "PouVarSelWindow.h"
#include "PouVarSelCtrlWrapper.h"

#include "resource.h"

#include <assert.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGenEdView

// IFCEdit class

IMPLEMENT_DYNCREATE(CGenEdView, CView)

BEGIN_MESSAGE_MAP(CGenEdView, CView)
	//{{AFX_MSG_MAP(CGenEdView)
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_MESSAGE(ACTIVATE_EDITOR, OnActivateEditor)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGenEdView construction/destruction

CGenEdView::CGenEdView ()
{
	m_pServer = NULL;
}


CGenEdView::~CGenEdView ()
{
}


BOOL CGenEdView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}


/////////////////////////////////////////////////////////////////////////////
// CGenEdView drawing

void CGenEdView::OnDraw (CDC *pDC)
{
	if (m_pServer != NULL)
	{
		CRect rect;

		GetClientRect (&rect);
		m_pServer->Draw (pDC, rect);
	};
}


void CGenEdView::OnInitialUpdate ()
{
	CView::OnInitialUpdate ();

	m_pServer = NULL;
}


/////////////////////////////////////////////////////////////////////////////
// CGenEdView printing

BOOL CGenEdView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGenEdView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGenEdView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CGenEdView::OnDestroy()
{
	// Deactivate the item on destruction; this is important
	// when a splitter view is being used.
   CView::OnDestroy();
   COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
   if (pActiveItem != NULL && pActiveItem->GetActiveView() == this)
   {
      pActiveItem->Deactivate();
      ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
   }
}


/////////////////////////////////////////////////////////////////////////////
// OLE Client support and commands


CGenEdCntrItem *CGenEdView::GetServer ()
{
	return (m_pServer);
}


void CGenEdView::ShowServer (CGenEdCntrItem *pServer)
{
	CRect rect;

	m_pServer = pServer;

	pServer->Run ();

	GetClientRect (&rect);
	m_pServer->SetClientExtent (CSize (rect.right - rect.left, rect.bottom - rect.top));
}


void CGenEdView::ActivateServer ()
{
	if (m_pServer == NULL)
		return;

	m_pServer->Activate (OLEIVERB_INPLACEACTIVATE, this);
}


void CGenEdView::DeactivateServer ()
{
	if (m_pServer == NULL)
		return;

    if(m_pServer->IsInPlaceActive())
    {
	    m_pServer->Deactivate ();
    }
}


void CGenEdView::SetFocusToServer ()
{
	if (m_pServer == NULL)
		return;

	PostMessage (ACTIVATE_EDITOR);
}


void CGenEdView::OnActivateEditor (LPARAM, WPARAM)
{
	CWnd *pWnd;

	assert (m_pServer != NULL);
	if (m_pServer == NULL)
		return;

	pWnd = m_pServer->GetInPlaceWindow ();
	if (pWnd == NULL)
		return;

	pWnd->SetFocus ();
}


// Special handling of OnSetFocus and OnSize are required for a container
//  when an object is being edited in-place.
void CGenEdView::OnSetFocus(CWnd* pOldWnd)
{
    // inform var selection from doc change
    CMainFrame* pMainFrame = NULL;
    pMainFrame = dynamic_cast<CMainFrame* > (AfxGetMainWnd ());
    if (pMainFrame == NULL)
    {
        return;
    }

    CPouVarSelWindow* pVarSelWin = pMainFrame->GetPouVarSel(false);
    if (pVarSelWin)
    {
        CPouVarSelCtrlWrapper* pVarSelWrapper = pVarSelWin->GetWrapper();
        if (pVarSelWrapper)
        {
            CGenEdDoc* pDoc = GetDocument();
            if (pDoc)
            {
                CString strPouName = pDoc->GetDocumentFile();
                CFile file;
                file.SetFilePath(strPouName);
                strPouName = file.GetFileName();
				CCEMAINApp* pApp = (CCEMAINApp*)AfxGetApp();
				if(!pApp->IsOpeningProject())
				{
					pVarSelWrapper->SetVarSelectionHint(strPouName, _T(""), _T(""));
				}
            }
        }
    }



//    if(m_pDocument)
//    {
//	    COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
//	    if (pActiveItem != NULL &&
//		    pActiveItem->GetItemState() == COleClientItem::activeUIState)
//	    {
//		    // need to set focus to this item if it is in the same view
//		    CWnd* pWnd = pActiveItem->GetInPlaceWindow();
//		    if (pWnd != NULL)
//		    {
//                SetFocusToServer ();
//			    return;
//		    }
//	    }
//    }

	CView::OnSetFocus(pOldWnd);

   
}


void CGenEdView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
    if(m_pDocument)
    {
        CGenEdDoc*  pDoc = GetDocument();
        if(pDoc)
        {
	        COleClientItem* pActiveItem = pDoc->GetInPlaceActiveItem(this);
	        if (pActiveItem != NULL && pActiveItem == m_pServer)
		        pActiveItem->SetItemRects();
	        else if (m_pServer != NULL)
	        {
		        m_pServer->SetClientExtent (CSize (cx, cy));
	        }
        }
    }
}


void CGenEdView::OnUpdate (CView *pSender, LPARAM lHint, CObject *pHint) 
{
	CView::OnUpdate (pSender, lHint, pHint);
}

/////////////////////////////////////////////////////////////////////////////
// CGenEdView diagnostics

#ifdef _DEBUG
void CGenEdView::AssertValid() const
{
	CView::AssertValid();
}

void CGenEdView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGenEdDoc* CGenEdView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGenEdDoc)));
	return (CGenEdDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGenEdView message handlers

void CGenEdView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	COleClientItem *pItem;
	CMDIFrameWnd *pMainFrame;
	CFrameWnd *pActiveFrame;
	CFrameWnd *pFrame;

	CView::OnLButtonDown(nFlags, point);

	if (m_pServer == NULL)
		return;

	pMainFrame = dynamic_cast<CMDIFrameWnd *> (AfxGetMainWnd ());
	assert (pMainFrame != NULL);
	if (pMainFrame == NULL)
		return;

	pActiveFrame = pMainFrame->MDIGetActive ();
	if (pActiveFrame == NULL)
		return;

	pFrame = GetParentFrame ();
	assert (pFrame != NULL);
	if (pFrame == NULL)
		return;

	if (pFrame != pActiveFrame)
		return;

	pItem = GetDocument ()->GetInPlaceActiveItem (this);
	if (pItem == m_pServer)
		return;

//	ActivateServer ();
	SetFocusToServer ();
}


