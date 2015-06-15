/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CEHeaderView.cpp 1     28.02.07 18:59 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CEHeaderView.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CEHeaderView.cpp $
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
#include "CEHeaderView.h"
#include "CELibrarianDoc.h"
//#include "CELibraryException.h" //OH: was 4cdll

//----  Static Initializations:   ----------------------------------*



// CEHeaderView.cpp: Implementierungsdatei
//


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEHeaderView

IMPLEMENT_DYNCREATE(CEHeaderView, CEditView)

CEHeaderView::CEHeaderView()
{
	m_pEditFont = new CFont();
}

CEHeaderView::~CEHeaderView()
{
	if (m_pEditFont != NULL)
		delete m_pEditFont;
}

BEGIN_MESSAGE_MAP(CEHeaderView, CEditView)
	//{{AFX_MSG_MAP(CEHeaderView)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	//}}AFX_MSG_MAP
	// Standard-Druckbefehle
	ON_COMMAND(ID_FILE_PRINT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CEditView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Zeichnung CEHeaderView 

void CEHeaderView::OnDraw(CDC* pDC)
{
	TCHAR* pText;
	CELibrarianDoc* pDoc = (CELibrarianDoc*) GetDocument();
	CELibrary* pCELib= pDoc->GetCELibrary();
	CString fbText;
	CString fbName = pDoc->GetActualItem();
	
	if ( fbName != "" )
	{
		/*try
		{*/
			unsigned long ulFileByteSize;
			int rv=pCELib->GetPOUHeaderByteSize(fbName, &ulFileByteSize);
			if ( rv != CELibrary::Err_Ok)
			{
				CString error;
				// no header information available
				error.LoadString(IDS_CE_ERR_NOHEADER);
				GetEditCtrl().SetWindowText(error);
				return;
			}
			pText = (TCHAR*) malloc(ulFileByteSize+1);
			rv=pCELib->GetPOUHeaderAsBuffer(fbName, pText, ulFileByteSize);
			if ( rv != CELibrary::Err_Ok)
			{
				pDoc->ErrorMessage(rv);
				return;
			}
			pText[ulFileByteSize] = '\0';
			pCELib->GetPOUFileAsString(fbName, pText);
			fbText=pText;
			free(pText);
			GetEditCtrl().SetWindowText(fbText);
		/*}
		catch ( CELibraryException ex )
		{
			ex.ReportError();
		}*/
	}
	else
	{	
		GetEditCtrl().SetWindowText(fbText);
	}
}

/////////////////////////////////////////////////////////////////////////////
// Diagnose CEHeaderView

#ifdef _DEBUG
void CEHeaderView::AssertValid() const
{
	CEditView::AssertValid();
}

void CEHeaderView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen für Nachrichten CEHeaderView 

void CEHeaderView::OnInitialUpdate() 
{
	CEditView::OnInitialUpdate();
}

void CEHeaderView::OnSize(UINT nType, int cx, int cy) 
{
	CEditView::OnSize(nType, cx, cy);

}

int CEHeaderView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEditView::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CEHeaderView::OnActivateView(BOOL bActivate, CEditView* pActivateView, CEditView* pDeactiveView) 
{
	CEditView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

BOOL CEHeaderView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	return TRUE;
}

void CEHeaderView::OnEditCopy() 
{
	GetEditCtrl().Copy();	
}


void CEHeaderView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	int start=0,end=0;
	GetEditCtrl().GetSel(start,end);	
	pCmdUI->Enable((start!=end));
}

BOOL CEHeaderView::DestroyWindow() 
{
	return CEditView::DestroyWindow();
}

BOOL CEHeaderView::OnPreparePrinting(CPrintInfo* pInfo) 
{
	return DoPreparePrinting(pInfo);
}

void CEHeaderView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	CEditView::OnBeginPrinting(pDC, pInfo);
	CFont * m_pFont = CFont::FromHandle((HFONT)GetStockObject(ANSI_FIXED_FONT));
    CFont * m_pOldFont = pDC->SelectObject(m_pFont);
}

void CEHeaderView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	CEditView::OnEndPrinting(pDC, pInfo);
}

void CEHeaderView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{	
	CEditView::OnPrint(pDC, pInfo);
}

void CEHeaderView::OnPrintHeader(const CString& fbName, CDC* pDC, CPrintInfo* pInfo) 
{
	TCHAR* pText;
	CELibrarianDoc* pDoc = (CELibrarianDoc*) GetDocument();
	CELibrary* pCELib= pDoc->GetCELibrary();
	CString fbText;
	
	if ( fbName != "" )
	{
		/*try
		{*/
			unsigned long ulFileByteSize;
			int rv=pCELib->GetPOUHeaderByteSize(fbName, &ulFileByteSize);
			if ( rv != CELibrary::Err_Ok)
			{
				CString error;
				// no header information available
				error.LoadString(IDS_CE_ERR_NOHEADER);
				GetEditCtrl().SetWindowText(error);
				return;
			}
			pText = (TCHAR*) malloc(ulFileByteSize+1);
			rv=pCELib->GetPOUHeaderAsBuffer(fbName, pText, ulFileByteSize);
			if ( rv != CELibrary::Err_Ok)
			{
				pDoc->ErrorMessage(rv);
				return;
			}
			pText[ulFileByteSize] = '\0';
			pCELib->GetPOUFileAsString(fbName, pText);
			fbText=pText;
			free(pText);
			GetEditCtrl().SetWindowText(fbText);
		/*}
		catch ( CELibraryException ex )
		{
			ex.ReportError();
		}*/
	}
	else
	{	
		GetEditCtrl().SetWindowText(fbText);
	}
}

void CEHeaderView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	CEditView::OnUpdate(pSender, lHint, pHint);
	CPaintDC dc(this);
	OnDraw(&dc);
}


/*
 *----------------------------------------------------------------------------*
 *  $History: CEHeaderView.cpp $
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
