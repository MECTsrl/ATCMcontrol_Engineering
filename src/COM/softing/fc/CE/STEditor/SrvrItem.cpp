
// SrvrItem.cpp : implementation of the CSTEditorSrvrItem class
//

#include "stdafx.h"
#include "fc_todebug\fc_assert.h"
#include "STEditor.h"
#include "STEditorDoc.h"
#include "SrvrItem.h"
#include "STEditorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSTEditorSrvrItem implementation

IMPLEMENT_DYNAMIC(CSTEditorSrvrItem, CDocObjectServerItem)

CSTEditorSrvrItem::CSTEditorSrvrItem(CSTEditorDoc* pContainerDoc)
	: CDocObjectServerItem(pContainerDoc, TRUE)
{
}

CSTEditorSrvrItem::~CSTEditorSrvrItem()
{
	// TODO: add cleanup code here
}

void CSTEditorSrvrItem::Serialize(CArchive& ar)
{
	// CSTEditorSrvrItem::Serialize will be called by the framework if
	//  the item is copied to the clipboard.  This can happen automatically
	//  through the OLE callback OnGetClipboardData.  A good default for
	//  the embedded item is simply to delegate to the document's Serialize
	//  function.  If you support links, then you will want to serialize
	//  just a portion of the document.

	if (!IsLinkedItem())
	{
		CSTEditorDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		pDoc->Serialize(ar);
	}
}

BOOL CSTEditorSrvrItem::OnGetExtent(DVASPECT dwDrawAspect, CSize& rSize)
{
	CSTEditorDoc *pDoc;

	// Most applications, like this one, only handle drawing the content
	//  aspect of the item.  If you wish to support other aspects, such
	//  as DVASPECT_THUMBNAIL (by overriding OnDrawEx), then this
	//  implementation of OnGetExtent should be modified to handle the
	//  additional aspect(s).

	if (dwDrawAspect != DVASPECT_CONTENT)
		return CDocObjectServerItem::OnGetExtent(dwDrawAspect, rSize);

	// CSTEditorSrvrItem::OnGetExtent is called to get the extent in
	//  HIMETRIC units of the entire item.

	pDoc = GetDocument ();
	assert (pDoc != NULL);
	if (pDoc == NULL)
	{
		return (FALSE);
	};
	rSize = pDoc->GetClientExtent ();   // Currently extent is not HIMETRIC!!!

	return TRUE;
}

BOOL CSTEditorSrvrItem::OnDraw (CDC *pDC, CSize &rSize)
{
	// Remove this if you use rSize
	UNREFERENCED_PARAMETER(rSize);

	CSTEditorDoc *pDoc;
	POSITION pPosition;
	CSTEditorView *pView;

	pDoc = GetDocument ();
	ASSERT_VALID (pDoc);

	pDC->SetMapMode (MM_TEXT);
	pDC->SetWindowOrg (0, 0);

	pPosition = pDoc->GetFirstViewPosition ();
	pView = dynamic_cast<CSTEditorView *> (pDoc->GetNextView (pPosition));
	assert (pView != NULL);
	if (pView != NULL)
	{
		pView->DrawInto (pDC, pDoc->GetClientExtent ());
	};

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CSTEditorSrvrItem diagnostics

#ifdef _DEBUG
void CSTEditorSrvrItem::AssertValid() const
{
	CDocObjectServerItem::AssertValid();
}

void CSTEditorSrvrItem::Dump(CDumpContext& dc) const
{
	CDocObjectServerItem::Dump(dc);
}
#endif

/////////////////////////////////////////////////////////////////////////////
