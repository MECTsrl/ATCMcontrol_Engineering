//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "LATED.h"
#include "lateddoc.h"
#include "srvritem.h"
#include "latedview.h"
#include "latedlist.h"
//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLATEDSrvrItem implementation

IMPLEMENT_DYNAMIC(CLATEDSrvrItem, CDocObjectServerItem)

CLATEDSrvrItem::CLATEDSrvrItem(CLATEDDoc* pContainerDoc)
	: CDocObjectServerItem(pContainerDoc, TRUE)
{
	// add one-time construction code here
	// (eg, adding additional clipboard formats to the item's data source)
}

CLATEDSrvrItem::~CLATEDSrvrItem()
{
	// add cleanup code here
}

void CLATEDSrvrItem::Serialize(CArchive& ar)
{
	// CLATEDSrvrItem::Serialize will be called by the framework if
	//  the item is copied to the clipboard.  This can happen automatically
	//  through the OLE callback OnGetClipboardData.  A good default for
	//  the embedded item is simply to delegate to the document's Serialize
	//  function.  If you support links, then you will want to serialize
	//  just a portion of the document.

	if (!IsLinkedItem())
	{
		CLATEDDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		pDoc->Serialize(ar);
	}
}

BOOL CLATEDSrvrItem::OnGetExtent(DVASPECT dwDrawAspect, CSize& rSize)
{
	// Most applications, like this one, only handle drawing the content
	//  aspect of the item.  If you wish to support other aspects, such
	//  as DVASPECT_THUMBNAIL (by overriding OnDrawEx), then this
	//  implementation of OnGetExtent should be modified to handle the
	//  additional aspect(s).

	if (dwDrawAspect != DVASPECT_CONTENT)
		return CDocObjectServerItem::OnGetExtent(dwDrawAspect, rSize);

	// CLATEDSrvrItem::OnGetExtent is called to get the extent in
	//  HIMETRIC units of the entire item.  The default implementation
	//  here simply returns a hard-coded number of units.

	CLATEDDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// replace this arbitrary size

	rSize = CSize(3000, 3000);   // 3000 x 3000 HIMETRIC units

	return TRUE;
}

BOOL CLATEDSrvrItem::OnDraw(CDC* pDC, CSize& rSize)
{
	// Remove this if you use rSize
	UNREFERENCED_PARAMETER(rSize);
    POSITION pos;
    CLATEDView* pView = NULL;
    CLATEDList * pListView = NULL;

	CLATEDDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// set mapping mode and extent
	//  (The extent is usually the same as the size returned from OnGetExtent)
	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowOrg(0,0);
	//pDC->SetWindowExt(3000, 3000);

    pos = pDoc->GetFirstViewPosition ();
	pView = dynamic_cast<CLATEDView *> (pDoc->GetNextView (pos));
    if(pView) {
        pView->DrawInto (pDC, pDoc->GetClientExtent ());
    }

    if(!pos) {
        return TRUE;
    }

    pListView = dynamic_cast<CLATEDList *> (pDoc->GetNextView (pos));
    if(pListView) {
        pListView->DrawInto (pDC, pDoc->GetClientExtent ());
    }
    
    // add drawing code here.  Optionally, fill in the HIMETRIC extent.
	//  All drawing takes place in the metafile device context (pDC).

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CLATEDSrvrItem diagnostics

#ifdef _DEBUG
void CLATEDSrvrItem::AssertValid() const
{
	CDocObjectServerItem::AssertValid();
}

void CLATEDSrvrItem::Dump(CDumpContext& dc) const
{
	CDocObjectServerItem::Dump(dc);
}
#endif

/////////////////////////////////////////////////////////////////////////////
