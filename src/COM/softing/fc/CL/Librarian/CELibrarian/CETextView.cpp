/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CETextView.cpp 1     28.02.07 18:59 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CETextView.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CETextView.cpp $
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
#include "CELibrarian.h"
#include "CETextView.h"
#include "CELibrarianDoc.h"
#include "CExEdit.h"
//----  Static Initializations:   ----------------------------------*

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CETextView

IMPLEMENT_DYNCREATE(CETextView, CView)

CETextView::CETextView()
{
	m_pEditCtrl = new CExEdit();
	m_pEditFont = new CFont();
}

CETextView::~CETextView()
{
	if ( m_pEditCtrl != NULL)
		delete m_pEditCtrl;
	if (m_pEditFont != NULL)
		delete m_pEditFont;
}

CEdit* CETextView::GetEditCtrl() const
{
	return (CEdit*) m_pEditCtrl;
}

BEGIN_MESSAGE_MAP(CETextView, CView)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_MOUSEWHEEL()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Zeichnung CETextView 

void CETextView::OnDraw(CDC* pDC)
{
	TCHAR* pText;
	CELibrarianDoc* pDoc = (CELibrarianDoc*) GetDocument();
	CELibrary* pCELib= pDoc->GetCELibrary();
	CString fbText;
	CString fbName = pDoc->GetActualItem();
	
	if ( fbName != "" )
	{
			unsigned long ulFileByteSize;
			int rv=pCELib->GetPOUPublicHeaderByteSize(fbName, &ulFileByteSize);
			if ( rv != CELibrary::Err_Ok)
			{
				CString error;
				// no header information available
				error.LoadString(IDS_CE_ERR_NOHEADER);
				GetEditCtrl()->SetWindowText(error);
				return;
			}
			pText = (TCHAR*) malloc(ulFileByteSize+1);
			rv=pCELib->GetPOUPublicHeaderAsBuffer(fbName, pText, ulFileByteSize);
			if ( rv != CELibrary::Err_Ok)
			{
				pDoc->ErrorMessage(rv);
				return;
			}
			pText[ulFileByteSize] = _T('\0');
			fbText=pText;
			free(pText);
			GetEditCtrl()->SetWindowText(fbText);
	}
	else
	{	
		GetEditCtrl()->SetWindowText(fbText);
	}
}

/////////////////////////////////////////////////////////////////////////////
// Diagnose CETextView

#ifdef _DEBUG
void CETextView::AssertValid() const
{
	CView::AssertValid();
}

void CETextView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif 

void CETextView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	static bool init = true; 
	if (init) {
		GetEditCtrl()->ShowWindow(SW_SHOW);
		init = false;
	}
}

void CETextView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);

	GetEditCtrl()->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOMOVE);	
}

int CETextView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	//
	// create edit control
	//
	CRect rect;
	GetClientRect(rect);
	DWORD style = WS_CHILD | WS_VISIBLE | ES_MULTILINE | WS_HSCROLL | WS_VSCROLL;
	GetEditCtrl()->Create(style,rect,this,1);
	GetEditCtrl()->SetReadOnly( TRUE );

	//
	// set font of edit control
	//
	LOGFONT logfont;
	memset(&logfont, 0, sizeof(logfont));
	logfont.lfHeight = 12;
	logfont.lfWidth = 0;
	logfont.lfEscapement = 0;
	logfont.lfOrientation = 0;  
	logfont.lfWeight=0;   
	logfont.lfItalic=FALSE;
	logfont.lfUnderline=FALSE;
	logfont.lfStrikeOut=0;
	logfont.lfCharSet=ANSI_CHARSET;
	logfont.lfOutPrecision=OUT_DEFAULT_PRECIS;
	logfont.lfClipPrecision=CLIP_DEFAULT_PRECIS;
	logfont.lfQuality=DEFAULT_QUALITY;
    logfont.lfPitchAndFamily = FIXED_PITCH | FF_MODERN;
	lstrcpy(logfont.lfFaceName, _T("Courier"));
	BOOL test=m_pEditFont->CreateFontIndirect(&logfont);
	GetEditCtrl()->SetFont(m_pEditFont);

	return 0;
}

void CETextView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

BOOL CETextView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	return TRUE;
}

BOOL CETextView::DestroyWindow() 
{
	return CView::DestroyWindow();
}


BOOL CETextView::OnPreparePrinting(CPrintInfo* pInfo) 
{
	return DoPreparePrinting(pInfo);
}

void CETextView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	CView::OnBeginPrinting(pDC, pInfo);
	CFont * m_pFont = CFont::FromHandle((HFONT)GetStockObject(ANSI_FIXED_FONT));
    CFont * m_pOldFont = pDC->SelectObject(m_pFont);
}

void CETextView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	CView::OnEndPrinting(pDC, pInfo);
}

void CETextView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	OnDraw(pDC);
}


void CETextView::OnSetFocus(CWnd* pOldWnd) 
{
	CView::OnSetFocus(pOldWnd);
	GetEditCtrl()->SetFocus();		
}


/*
 *----------------------------------------------------------------------------*
 *  $History: CETextView.cpp $
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
 * User: Oh           Date: 10/12/01   Time: 10:01a
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 * 
 * *****************  Version 2  *****************
 * User: Oh           Date: 6/25/01    Time: 3:32p
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 *==
 *----------------------------------------------------------------------------*
*/
