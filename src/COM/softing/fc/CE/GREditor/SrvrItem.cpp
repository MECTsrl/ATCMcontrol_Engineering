

/*doc*=======================================================================*/
/*inc*=======================================================================*/

#include "stdafx.h"
#include "GrEditor.h"
#include "GrEditorDoc.h"
#include "GrEditorView.h"
#include "STEditorView.h"
#include "Elem.h"
#include "SrvrItem.h"

/*const*=====================================================================*/
/*macro*=====================================================================*/
/*glovar*====================================================================*/
/*type*======================================================================*/
/*locvar*====================================================================*/

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*proto*=====================================================================*/
/*func*======================================================================*/

/////////////////////////////////////////////////////////////////////////////
// CGrEditorSrvrItem implementation

IMPLEMENT_DYNAMIC(CGrEditorSrvrItem, CDocObjectServerItem)

CGrEditorSrvrItem::CGrEditorSrvrItem(CGrEditorDoc* pContainerDoc)
	: CDocObjectServerItem(pContainerDoc, TRUE)
{
	// TODO: add one-time construction code here
	//  (eg, adding additional clipboard formats to the item's data source)
}

CGrEditorSrvrItem::~CGrEditorSrvrItem()
{
	// TODO: add cleanup code here
}

void CGrEditorSrvrItem::Serialize(CArchive& ar)
{
	// CGrEditorSrvrItem::Serialize will be called by the framework if
	//  the item is copied to the clipboard.  This can happen automatically
	//  through the OLE callback OnGetClipboardData.  A good default for
	//  the embedded item is simply to delegate to the document's Serialize
	//  function.  If you support links, then you will want to serialize
	//  just a portion of the document.

	if (!IsLinkedItem())
	{
		CGrEditorDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		pDoc->Serialize(ar);
	}
}

BOOL CGrEditorSrvrItem::OnGetExtent(DVASPECT dwDrawAspect, CSize& rSize)
{
    // Most applications, like this one, only handle drawing the content
    //  aspect of the item.  If you wish to support other aspects, such
    //  as DVASPECT_THUMBNAIL (by overriding OnDrawEx), then this
    //  implementation of OnGetExtent should be modified to handle the
    //  additional aspect(s).
    
    if (dwDrawAspect != DVASPECT_CONTENT)
        return CDocObjectServerItem::OnGetExtent(dwDrawAspect, rSize);
    
    // CGrEditorSrvrItem::OnGetExtent is called to get the extent in
    //  HIMETRIC units of the entire item.  The default implementation
    //  here simply returns a hard-coded number of units.
    
    CGrEditorDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    
    rSize = pDoc->GetDocSize();
    CClientDC dc(NULL);
    
    // use a mapping mode based on logical units
    //  (we can't use MM_LOENGLISH because MM_LOENGLISH uses physical inches)
    
    CSize docSize, sizeNum, sizeDenom;
    int   xExt, yExt;
    
    docSize     = pDoc->GetDocGrid();
    
    // pDoc->GetZoomFactor(&sizeNum, &sizeDenom);
    xExt = (int)( 
        (float)docSize.cx *
        (float)dc.GetDeviceCaps(LOGPIXELSX) / 100.0 );
    yExt = (int)( 
        (float)docSize.cy *
        (float)dc.GetDeviceCaps(LOGPIXELSY) / 100.0 );
    
    dc.SetMapMode(MM_ANISOTROPIC);
    dc.SetViewportExt((int)xExt, (int)yExt);
    dc.SetWindowExt(docSize);
    dc.LPtoHIMETRIC(&rSize);
    
    return TRUE;
}

BOOL CGrEditorSrvrItem::OnDraw(CDC* pDC, CSize& rSize)
{
    // Remove this if you use rSize
    UNREFERENCED_PARAMETER(rSize);
    
    POSITION       pPosition;
    CSize          Size;
    CGrEditorView *pGrView;
    CSTEditorView *pStView;
    CGrEditorDoc  *pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    pPosition = pDoc->GetFirstViewPosition ();
    while(pPosition)
    {
        pGrView = dynamic_cast<CGrEditorView *> (pDoc->GetNextView (pPosition));
        //       assert (pView != NULL);
        if (pGrView != NULL)
        {
            pGrView->CheckMode();  // leave EM_PROPEDIT - mode if set
            
            pGrView->m_DrawTarget = CGrEditorView::InactiveView;
            pGrView->OnPrepareDC (pDC);
            //            CSize Size = pDoc->GetClientExtent ();
            CRect R;
            pGrView->GetClientRect(&R);
            Size = R.Size();

            pGrView->DrawInto (pDC, Size);
            //           pGrView->OnDraw(pDC);
            pGrView->m_DrawTarget = CGrEditorView::ActiveView;
        }else
        {
            pStView = dynamic_cast<CSTEditorView *> (pDoc->GetNextView (pPosition));
            if(pStView && pStView->GetContext()==CPoint(0,0)) // Header
            {
               pStView->m_DrawTarget = CSTEditorView::InactiveView;
               pStView->OnPrepareDC (pDC);
               CRect R;
               pStView->GetClientRect(&R);
               Size = R.Size();
               
               pStView->DrawInto(pDC, rSize);

               pStView->m_DrawTarget = CSTEditorView::ActiveView;
            }
        }
    }
    return TRUE;
}

void CGrEditorSrvrItem::OnShow()
{
    CDocObjectServerItem::OnShow();
    
    POSITION       pPosition;
    CView          *pView;
    CGrEditorDoc  *pDoc = GetDocument();
    
    pPosition = pDoc->GetFirstViewPosition ();
    while(pPosition)
    {
        pView = pDoc->GetNextView (pPosition);
        if(pView)
        {
            if ( dynamic_cast<CGrEditorView *>(pView) != NULL)
            {
                pView->Invalidate();
                ((CGrEditorView*)pView)->ScrollToPosition ( ((CGrEditorView*)pView)->m_LastScrollPos );
            }else
            {
                pView->Invalidate();
            }
        }
    }
}

void CGrEditorSrvrItem::OnHide()
{
	CDocObjectServerItem::OnHide();
}

/////////////////////////////////////////////////////////////////////////////
// CGrEditorSrvrItem diagnostics

#ifdef _DEBUG
void CGrEditorSrvrItem::AssertValid() const
{
	CDocObjectServerItem::AssertValid();
}

void CGrEditorSrvrItem::Dump(CDumpContext& dc) const
{
	CDocObjectServerItem::Dump(dc);
}
#endif

/////////////////////////////////////////////////////////////////////////////
