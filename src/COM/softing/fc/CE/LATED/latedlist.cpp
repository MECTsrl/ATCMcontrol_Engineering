//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "stdedattr.h"
#include "lateddoc.h"
#include "latedlist.h"

//----  Local Defines:   -------------------------------------------*

//----  Static Initializations:   ----------------------------------*


/////////////////////////////////////////////////////////////////////////////
// CLATEDList

IMPLEMENT_DYNCREATE(CLATEDList, CListView)

BEGIN_MESSAGE_MAP(CLATEDList, CListView)
	//{{AFX_MSG_MAP(CLATEDList)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLATEDView construction/destruction

CLATEDList::CLATEDList(): CListView()
{
	//{{AFX_DATA_INIT(CLATEDList)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// add construction code here

}

CLATEDList::~CLATEDList()
{
}

BOOL CLATEDList::PreCreateWindow(CREATESTRUCT& cs)
{
	// Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return CListView::PreCreateWindow(cs);
}

void CLATEDList::OnInitialUpdate()
{
	CListView::OnInitialUpdate();
}

/////////////////////////////////////////////////////////////////////////////
// CLATEDList diagnostics

#ifdef _DEBUG
void CLATEDList::AssertValid() const
{
	CView::AssertValid();
}

void CLATEDList::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLATEDDoc* CLATEDList::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLATEDDoc)));
	return (CLATEDDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLATEDView message handlers

int CLATEDList::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{

	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}

void CLATEDList::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// Add your specialized code here and/or call the base class

    if (lHint == CStdEditorDoc::GetPositionHint) {
        return;
    }

    if (lHint == CStdEditorDoc::GotoPositionHint) {
        return;
    }

    CListView::OnUpdate(pSender,lHint,pHint);
}

void CLATEDList::OnDraw(CDC* pDC)
{}

void CLATEDList::DrawInto (CDC *pDC, CSize drawAreaSize)
{
    CRect drawArea;
    
    drawArea.left = 0;
    drawArea.top = 0;
    drawArea.right = drawAreaSize.cx;
    drawArea.bottom = drawAreaSize.cy;

    OnDraw(pDC);
}
