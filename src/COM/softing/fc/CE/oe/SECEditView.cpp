// SECEditView.cpp : implementation file
//

#include "stdafx.h"
#include "SECEditView.h"
/////////////////////////////////////////////////////////////////////////////
// SECEditView

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(SECEditView, CView)

//@doc SECEditView
//@mfunc SECEditView Constructor.
SECEditView::SECEditView()
{
	m_bPreviewSelection = TRUE;
}

//@doc SECEditView
//@mfunc SECEditView Destructor.
SECEditView::~SECEditView()
{
}


BEGIN_MESSAGE_MAP(SECEditView, SECEditCore<CView>)
	//{{AFX_MSG_MAP(SECEditView)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	SECEDITCORE_MESSAGE_LIST()
	#if _MFC_VER >= 0x0421
	ON_WM_MOUSEWHEEL()
	#endif
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SECEditView diagnostics


/////////////////////////////////////////////////////////////////////////////
// SECEditView message handlers
//@doc SECEditView
//@mfunc Internal implementation of OnInitialUpdate.
//@rdesc void 
void SECEditView::OnInitialUpdate() 
{
    // CView::OnInitialUpdate calls OnUpdate which
    // uses attributes being initialized in InitView.
    // Reverse calling order should be better.
#ifdef ORIGINAL_VERSION
	CView::OnInitialUpdate();
	InitView();
#else
	InitView();
	CView::OnInitialUpdate();
#endif

	// If we are embedded in a CSplitterWnd, then we should
	// synchronize up with the other view
   	CWnd* pWnd = GetParent();
	if(pWnd->IsKindOf(RUNTIME_CLASS(CSplitterWnd)) && (pWnd->GetStyle() & SPLS_DYNAMIC_SPLIT))
	{
		m_pSplitterParent = (CSplitterWnd*)pWnd;
		VERIFY(m_pSplitterParent->IsChildPane(this, m_nSplitRow, m_nSplitCol));

		ASSERT(m_nSplitRow < 2);
		ASSERT(m_nSplitCol < 2);

		if (m_nSplitRow > 0 || m_nSplitCol > 0)
		{
			// copy settings from other pane
			SECEditView* pView;

			pView = (SECEditView*)m_pSplitterParent->GetPane(0, 0);

			ASSERT(pView != NULL);
			if (pView->IsKindOf(RUNTIME_CLASS(SECEditView)))
			{
				if (m_nSplitRow > 0)
				{
					pView = (SECEditView*)m_pSplitterParent->GetPane(0, m_nSplitCol);

					ASSERT(pView != NULL);
					if (pView->IsKindOf(RUNTIME_CLASS(SECEditView)))
					{
						// We are on the bottom of a horizontal splitter,
						// so only get the horizontal alignment
						m_iLeftEdge = pView->m_iLeftEdge;
						MySetScrollPos(SB_HORZ,m_iLeftEdge,m_bUpdate);
					}
				}

				if (m_nSplitCol > 0)
				{
					pView = (SECEditView*) m_pSplitterParent->GetPane(m_nSplitRow, 0);

					ASSERT(pView != NULL);
					if (pView->IsKindOf(RUNTIME_CLASS(SECEditView)))
					{
						// We are on the right of a vertical splitter, so
						// we should only synch with the vertical alignment
						m_pTopLine = pView->m_pTopLine;
						m_pCurLine = pView->m_pCurLine;
						m_iFirstLineNo = pView->m_iFirstLineNo;
						MySetScrollPos(SB_VERT,m_iFirstLineNo-1,TRUE);
					}
				}
			}
		}
	}
}


//@doc SECEditView
//@mfunc Decide whether or not to enable the Print Selection
//              option
//@rdesc BOOL Enable print selection?
//@parm CPrintInfo* pInfo Pointer to a CPrintInfo structure passed to us by MFC.

BOOL SECEditView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// If you want print preview to preview only the selection
	// then this is the place to do it.
	// Set m_bPreviewSelection to false to always preview
	// the whole thing
	if (m_bPreviewSelection)
		{
		if (m_BlockStart.pLine && m_BlockEnd.pLine)
			{
			// enable the selection button
			pInfo->m_pPD->m_pd.Flags &= ~PD_NOSELECTION;

			// check the selection button
			pInfo->m_pPD->m_pd.Flags |= PD_SELECTION;
			}
		}

	BOOL rVal = DoPreparePrinting(pInfo);

	m_bPrintSelection = pInfo->m_pPD->PrintSelection();

	return rVal;
}


//@doc SECEditView
//@mfunc Format the header and footer, determine the # of pages
//              that will be printed, and create the printer font.  Override if you want to customize
//this behavior.
//@rdesc void 
//@parm CDC* pDC Printer DC
//@parm  CPrintInfo* pInfo Pointer to an MFC CPrinterInfo

void SECEditView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);

	// get the latest page setup values from the registry
	dlgPageSetup.Initialize();

	LPCTSTR pszFileName = GetDocument()->GetPathName();
	BOOL bForceSysTime = _tcschr(pszFileName, '.') == NULL;
	CTime timeSys = CTime::GetCurrentTime();
	CFileStatus status;
	CFile::GetStatus(pszFileName, status);

	if (dlgPageSetup.m_iHeaderTime != 0 || bForceSysTime)
		m_timeHeader = timeSys;
	else
		m_timeHeader = status.m_mtime;

	if (dlgPageSetup.m_iFooterTime != 0 || bForceSysTime)
		m_timeFooter = timeSys;
	else
		m_timeFooter = status.m_mtime;

	if (m_hPrinterFont == NULL)
		{
		// get current screen font object metrics
		CFont* pFont = &m_font;
		LOGFONT lf;
		LOGFONT lfSys;
		if (pFont == NULL)
			return;
		VERIFY(pFont->GetObject(sizeof(LOGFONT), &lf));
		VERIFY(::GetObject(::GetStockObject(SYSTEM_FONT), sizeof(LOGFONT),
			&lfSys));
		if (lstrcmpi((LPCTSTR)lf.lfFaceName, (LPCTSTR)lfSys.lfFaceName) == 0)
			return;

		// map to printer font metrics
		HDC hDCFrom = ::GetDC(NULL);
		lf.lfHeight = ::MulDiv(lf.lfHeight, pDC->GetDeviceCaps(LOGPIXELSY),
			::GetDeviceCaps(hDCFrom, LOGPIXELSY));
		lf.lfWidth = ::MulDiv(lf.lfWidth, pDC->GetDeviceCaps(LOGPIXELSX),
			::GetDeviceCaps(hDCFrom, LOGPIXELSX));
		::ReleaseDC(NULL, hDCFrom);

		// create it, if it fails we just use the printer's default.
		m_hMirrorFont = ::CreateFontIndirect(&lf);
		m_hPrinterFont = m_hMirrorFont;
		}

	// select the font so we can calc the metrics
	CFont* pOldFont = NULL;
	if (m_hPrinterFont != NULL)
		pOldFont = pDC->SelectObject(CFont::FromHandle(m_hPrinterFont));
   /* Get the height of one line and the height of a page */

	// get some constants
	int iWidth = pDC->GetDeviceCaps(HORZRES);
	int iHeight = pDC->GetDeviceCaps(VERTRES);

	// calculate our print rect
	CRect rect;
	rect.top = rect.left = 0;
	rect.bottom = iHeight;
	rect.right = iWidth;
	TEXTMETRIC metrics;
	pDC->GetTextMetrics(&metrics);
	m_iPrintLineHeight = metrics.tmHeight+metrics.tmExternalLeading;
	int iPageHeight = pDC->GetDeviceCaps(VERTRES);
	int iCharWidth = metrics.tmAveCharWidth;
	int maxchars = 	rect.Width() / iCharWidth;
	maxchars--;
	// go thru global OEPageSetupDlg to format the header and footer
	CString strHeader;
	dlgPageSetup.FormatHeader(strHeader, m_timeHeader, pszFileName, 1);
	CString strFooter;
	dlgPageSetup.FormatFooter(strFooter, m_timeFooter, pszFileName, 1);

	// exlude space for the header
	if (!strHeader.IsEmpty())
		iPageHeight -=	m_iPrintLineHeight + m_iPrintLineHeight / 4 + 
		               m_iPrintLineHeight / 4;

	// exlude space for the footer
	if (!strFooter.IsEmpty())
		iPageHeight -= m_iPrintLineHeight + m_iPrintLineHeight/4 
		               + m_iPrintLineHeight/4;

	m_iLinesPerPage = iPageHeight / m_iPrintLineHeight;
	if (pOldFont != NULL)
		{
		//////////////////////////////////////////////////////
		// Pagination.  Note we have to do it here to be able to
		// give an accurate page count to the print preview code.
		// it's not just a count of lines because we wrap lines
		// that won't fit inside the rect.
		//
		PLINEDESC pFirst,pSecond;
		int iLineCount = 0;
		SECEdit* pEdit = GetEdit();
		ASSERT(pEdit != NULL);
		if (m_bPrintSelection)
			{
			pFirst = m_BlockStart.pLine;
			pSecond = m_BlockEnd.pLine->pNext;
			}
		else
			{
			pFirst = pEdit->ListGetFirst(m_pTopLine,NULL);
			pSecond = NULL;
			}
		iLineCount = CountLines();
		m_PageList.RemoveAll();
		// set the array's initial size to allow for a bunch of split lines
		m_PageList.SetSize(iLineCount/m_iLinesPerPage+8);
		int iPage = 0;
		m_PageList.SetAt(iPage,pEdit->GetLineNo(pFirst));
		iLineCount = 0;
		int iTotalLines = 0;
		while (pFirst != pSecond)
			{
			int iAddLines = 0;
			if ((pFirst->iTxtSize - pEdit->m_iLineEndLength) > maxchars)
				{
				int iSize = pFirst->iTxtSize;
				// first (maybe) adjust for OE_CR/OE_LF
				char cFirst,cSecond;
				cFirst = pFirst->pText[iSize-pEdit->m_iLineEndLength];
				cSecond = pEdit->m_szLineEnd[0];
				if (cFirst == cSecond)
					iSize -= pEdit->m_iLineEndLength;

				iAddLines = iSize / maxchars;
				if (iSize % maxchars)
					iAddLines++;

				iTotalLines += iAddLines;

				// skip this whole line if it's the last line of a
				// page, so we don't have to handle a line being
				// split across pages
				if ((iLineCount+iAddLines) <= m_iLinesPerPage)
					{
					iLineCount += iAddLines;
					iAddLines = 0;
					}
				}
			else
				{
				iLineCount++;
				iTotalLines++;
				}

			if ((iLineCount+iAddLines) >= m_iLinesPerPage)
				{
				if (pFirst->pNext)
					{
					iPage++;
					int iLineNo = pEdit->GetLineNo(pFirst);
					if (!iAddLines)
						iLineNo++;
					m_PageList.SetAtGrow(iPage,iLineNo);
					iLineCount = 0;
					}
				}

			if (!iAddLines)
				pFirst = pFirst->pNext;
			}

		pInfo->SetMaxPage(iPage+1);

		pDC->SelectObject(pOldFont);
		}


	ASSERT_VALID(this);
}


//@doc SECEditView
//@mfunc Print the contents of the current view
//
//@rdesc void 
//@parm CDC* pDC Printer DC 
//@parm  CPrintInfo* pInfo MFC CPrintInfo structure pointer.

void SECEditView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{

	USES_CONVERSION_T;

	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);

	// get string to show as "filename" in header/footer
	LPCTSTR pszFileName = GetDocument()->GetPathName();
	if (pszFileName[0] == 0)
		pszFileName = GetDocument()->GetTitle();

	// go thru global OEPageSetupDlg to format the header and footer
	CString strHeader;
	dlgPageSetup.FormatHeader(strHeader, m_timeHeader, pszFileName,
		pInfo->m_nCurPage);
	CString strFooter;
	dlgPageSetup.FormatFooter(strFooter, m_timeFooter, pszFileName,
		pInfo->m_nCurPage);

	char TextLine[OE_MAXLINELENGTH+1];
	int iLength;

	int iLine = m_PageList.GetAt(pInfo->m_nCurPage-1);
	int iLastPage = pInfo->GetMaxPage();
	int iLastLine;
	if (iLastPage == (int)pInfo->m_nCurPage)
		iLastLine = iLine + m_iLinesPerPage;
	else
		iLastLine = m_PageList.GetAt(pInfo->m_nCurPage) - 1;
	PLINEDESC pLine = pEdit->GoToLineNo(iLine);

	CRect rect = pInfo->m_rectDraw;
	CFont* pOldFont = NULL;
	if (m_hPrinterFont != NULL)
		pOldFont = pDC->SelectObject(CFont::FromHandle(m_hPrinterFont));
	pDC->SetBkMode(TRANSPARENT);

	// draw and exclude space for header
	if (!strHeader.IsEmpty())
		{
		pDC->TextOut(rect.left, rect.top, strHeader);
		rect.top += m_iPrintLineHeight + m_iPrintLineHeight / 4;
		pDC->MoveTo(rect.left, rect.top);
		pDC->LineTo(rect.right, rect.top);
		rect.top += m_iPrintLineHeight / 4;
		}

	// allow space for footer
	pInfo->m_rectDraw = rect;
	if (!strFooter.IsEmpty())
		rect.bottom -= m_iPrintLineHeight + m_iPrintLineHeight/4 
		               + m_iPrintLineHeight/4;

	// draw body text
	int y = rect.top;
	int iCharWidth;
	TEXTMETRIC metrics;
	pDC->GetTextMetrics(&metrics);
	iCharWidth = metrics.tmAveCharWidth;

	int maxchars = rect.right / iCharWidth;

	while (pLine && (iLine <= iLastLine))
		{
      iLength = PaintExpandTabs(pLine->pText,TextLine,pLine->iTxtSize,
			                pEdit->m_iTabSize,pEdit->m_iLineEndLength);
      if (iLength)
         {
         // (maybe) adjust for OE_CR/OE_LF
			char cFirst,cSecond;
			cFirst = TextLine[iLength-pEdit->m_iLineEndLength];
			cSecond = pEdit->m_szLineEnd[0];
         if (cFirst == cSecond)
            iLength -= pEdit->m_iLineEndLength;
         }
      
		if (!iLength)
			y += m_iPrintLineHeight;
		else
			{
			int iIndex = 0;
			while (iLength > 0)
				{
				int iOutLength = min(iLength,maxchars);
				pDC->TextOut(rect.left, y, A2T(&TextLine[iIndex]), iOutLength);
				iIndex += iOutLength;
				iLength -= iOutLength;
				y += m_iPrintLineHeight;
				}
			}
		if (m_bPrintSelection)
			if (pLine == m_BlockEnd.pLine)
				{
				pInfo->m_bContinuePrinting = FALSE;
				break;
				}
		pLine = pLine->pNext;
		iLine++;

		// Make sure that there is some selection on this line
		// If this is the last line. If not, don't print the line
		if(pLine == m_BlockEnd.pLine && m_BlockEnd.iOffset == 0)
			{
				pInfo->m_bContinuePrinting = FALSE;
				break;
			}

		}

	// draw footer
	if (!strFooter.IsEmpty())
		{
		rect.bottom += m_iPrintLineHeight/2;
		pDC->TextOut(rect.left, rect.bottom, strFooter);
		rect.bottom -= m_iPrintLineHeight / 4;
		pDC->MoveTo(rect.left, rect.bottom);
		pDC->LineTo(rect.right, rect.bottom);
		rect.bottom -= m_iPrintLineHeight / 4;
		}

	if (pOldFont != NULL)
		pDC->SelectObject(pOldFont);
}


//@doc SECEditView
//@mfunc Delete the printer font and clean up after printing.
//@rdesc void 
//@parm CDC* /*pDC*/
//@parm  CPrintInfo* /*pInfo*/

void SECEditView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	if (m_hMirrorFont != NULL && m_hPrinterFont == m_hMirrorFont)
		{
		::DeleteObject((HGDIOBJ*)&m_hMirrorFont);
		m_hPrinterFont = NULL;
		}
}

//@doc SECEditView
//@mfunc Internal handler.
//@rdesc void 
//@parm BOOL bActivate
//@parm  CView* pActivateView
//@parm  CView* pDeactiveView
void SECEditView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{

	static BOOL bFirstTime = TRUE;
	if (bFirstTime)
		{
		MakeCursorVisible();
		bFirstTime = FALSE;
		}
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

//@doc SECEditView
//@mfunc Internal handler. If a line is deleted from another view, we need to
//              make sure our m_pCurLine and m_pTopLine are still
//              valid.
//
//@rdesc void 
//@parm CView* pSender
//@parm  LPARAM lHint
//@parm  CObject* pHint
void SECEditView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) //ADMacroDone
{
	pSender; //UNUSED
	SECEditHint* hint;

	if ((lHint == IDS_OE_DELETELINE) && pHint->IsKindOf(RUNTIME_CLASS(SECEditHint)))
		{
		hint = (SECEditHint*)pHint;
		CheckLines(hint->m_pLine);
		}

	// If we are embedded in a dynamic splitter,
	// make sure we are properly synched with our other views
	if((lHint == OE_SYNCH_SPLITTERS) &&  m_pSplitterParent)
	{
		BOOL bNeedUpdate = FALSE;

		ASSERT_VALID(m_pSplitterParent);
		ASSERT(m_pSplitterParent->IsKindOf(RUNTIME_CLASS(CSplitterWnd)));

		if(pSender == NULL)
			pSender = (CView*)m_pSplitterParent->GetActivePane();

		// Need to synch up the appropriate views to the
		// active view
		if(pSender->IsKindOf(RUNTIME_CLASS(SECEditView)) && (pSender != this))
		{
			SECEditView* pView = (SECEditView*)pSender;
			int nRow = pView->m_nSplitRow;
			int nCol = pView->m_nSplitCol;

			if(nCol == m_nSplitCol)
			{
				// Align the left edges
				if(m_iLeftEdge != pView->m_iLeftEdge)
				{
					m_iLeftEdge = pView->m_iLeftEdge;
					bNeedUpdate = TRUE;
					MySetScrollPos(SB_HORZ,m_iLeftEdge,m_bUpdate);
				}
			}

			if(nRow == m_nSplitRow)
			{
				// Align the tops
				if(m_iFirstLineNo != pView->m_iFirstLineNo)
				{
					m_pTopLine = pView->m_pTopLine;
					m_pCurLine = pView->m_pCurLine;
					m_iFirstLineNo = pView->m_iFirstLineNo;
					MySetScrollPos(SB_VERT,m_iFirstLineNo-1,TRUE);
					bNeedUpdate = TRUE;
				}
			}
		}
		if(bNeedUpdate)
			InvalidateRect(NULL,FALSE);

		return;
	}

	if (m_bUpdate)
		InvalidateRect(NULL,FALSE);
}

//@doc SECEditView
//@mfunc Finds the active Document so we can decide whether or
//              not to draw the selection
//@rdesc CDocument* Pointer to CDocument if found, NULL if not.
CDocument* SECEditView::GetActiveDoc()
{
	CMDIChildWnd* pChild = ((CMDIFrameWnd*)AfxGetMainWnd())->MDIGetActive();
	if (!pChild)
	   return ((CFrameWnd*)AfxGetMainWnd())->GetActiveDocument();
	else
		return pChild->GetActiveDocument();
}

//@doc SECEditView
//@mfunc Internal function used to implement OLE drag and drop.
//@rdesc void 
void SECEditView::OnDragLeave() 
{
	SECEditCore<CView>::OnDragLeave();
	CView::OnDragLeave();
}

void SECEditView::UpdateViews(CWnd* pSender, LPARAM lHint, CObject* pHint)
{
	// This is declared in SECEditCore.h because many OE functions
	// Update the view. The default implementation does nothing
	// and is called for SECEditCtrl derivatives. This is called
	// for SECEditView derivatives
	CDocument* pDoc = GetDocument();
	if (pDoc)
		pDoc->UpdateAllViews((CView*)pSender, lHint, pHint);
}


int SECEditView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (SECEditCore<CView>::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Register Drop Target here.
	m_dt.Register(this);
	
	return 0;
}

void SECEditView::MySetScrollPos(int fnBar, int iPos, BOOL bRedraw)
{
	// Call the base class
	SECEditCore<CView>::MySetScrollPos(fnBar, iPos, bRedraw);
	if(m_pSplitterParent)
	{
		if((this == (CView*)m_pSplitterParent->GetActivePane()) && !m_bScrollMessage)
			UpdateViews(this, OE_SYNCH_SPLITTERS);
	}
}
