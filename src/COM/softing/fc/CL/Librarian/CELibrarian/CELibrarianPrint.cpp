/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CELibrarianPrint.cpp 1     28.02.07 18:59 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: CELibrarianPrint.cpp $
 *                      $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian/CELibrarianPrint.cpp $
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

#include "CMainFrm.h"
#include "CELibrarianDoc.h"
#include "CELibrarianView.h"
#include "CEHeaderView.h"
#include "toolbox.h"
#include "utilif.h"
#include "htmlhelp.h"
#include "CAboutGeneral.h"
//----  Static Initializations:   ----------------------------------*




#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// Print user data

class CPrintUserData
{
    protected:

    int     m_iPageNum;
    CString m_strHeaderName;
    CString m_strLibraryName;

    public:

    CPrintUserData()
    {
        m_iPageNum = 0;
        m_strHeaderName = _T("");
        m_strLibraryName = _T("");
    }

	virtual ~CPrintUserData()
    {
    }

    int GetPageNum() 
    {
        return m_iPageNum;
    }

    int IncrementPageNum() 
    {
        return ++m_iPageNum;
    }

    void ResetPageNum() 
    {
        m_iPageNum = 0;
    }

    LPCTSTR GetHeaderName() 
    {
        return m_strHeaderName;
    }

    void SetHeaderName(LPCTSTR pszHeaderName) 
    {
        m_strHeaderName = pszHeaderName;
    }

    LPCTSTR GetLibraryName() 
    {
        return m_strLibraryName;
    }

    void SetLibraryName(LPCTSTR pszLibraryName) 
    {
        m_strLibraryName = pszLibraryName;
    }
};

CFont * SelectPrintFont(CDC * pDCPrint, CFont * pFont)
{
	int iPixel = pDCPrint->GetDeviceCaps(LOGPIXELSY);
	pFont->CreateFont(-10 * iPixel / 72, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		            CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH, _T("Courier New"));
    return pDCPrint->SelectObject(pFont);
}

void SetMargins(CDC * pDCPrint, CPrintInfo * pPrintInfo)
{
	CRect * pRect = &pPrintInfo->m_rectDraw;
    int iOldMapMode = pDCPrint->SetMapMode(MM_LOMETRIC);
    CRect MapRect(0, 0, 100, -100);
    pDCPrint->LPtoDP(&MapRect);
    pDCPrint->SetMapMode(iOldMapMode);
    pDCPrint->DPtoLP(&MapRect);
    pRect->left += MapRect.right - MapRect.left;
    pRect->right -= MapRect.right - MapRect.left;
    pRect->top += MapRect.bottom - MapRect.top;
    pRect->bottom -= MapRect.bottom - MapRect.top;
}

void PrintStdHeader(CDC * pDCPrint, CPrintInfo * pPrintInfo)
{
    CPrintUserData * pUserData = (CPrintUserData *)(pPrintInfo->m_lpUserData);
    TEXTMETRIC tm;
	CRect rect = pPrintInfo->m_rectDraw;
	CFont Font;

    CFont * pOldFont = SelectPrintFont(pDCPrint, &Font);
    pDCPrint->GetTextMetrics(&tm);

	//
    // print ATCMControl bitmap
	//
	
    CDC dcMem;
    CBitmap Bitmap;
    BITMAP bm;
    Bitmap.LoadBitmap(IDB_4CONTROL);
    Bitmap.GetBitmap(&bm);
    CSize Size(bm.bmWidth, bm.bmHeight);
    Size.cx *= 4;
    Size.cy *= 4;
    pDCPrint->DPtoLP(&Size);
    dcMem.CreateCompatibleDC(pDCPrint);
    dcMem.SelectObject(&Bitmap);
    pDCPrint->StretchBlt(rect.left, rect.top, Size.cx, Size.cy, &dcMem, 
                         0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

	//
    // print library name right aligned
	//
    CString strHelp = pUserData->GetLibraryName();
    CSize TextSize = pDCPrint->GetTextExtent(strHelp);

	pDCPrint->TextOut(rect.right - TextSize.cx, 
                      rect.top + max (0, (Size.cy - TextSize.cy) / 2), strHelp);

	rect.top += max(Size.cy, TextSize.cy) + tm.tmHeight / 4;
	pDCPrint->MoveTo(rect.left, rect.top);
	pDCPrint->LineTo(rect.right, rect.top);
	rect.top += tm.tmHeight / 2;
    pPrintInfo->m_rectDraw.top = rect.top;

    pDCPrint->SelectObject(pOldFont);
}

void PrintStdFooter(CDC * pDCPrint, CPrintInfo * pPrintInfo)
{
    TEXTMETRIC tm;
	CRect rect = pPrintInfo->m_rectDraw;
	CFont Font;

    CFont * pOldFont = SelectPrintFont(pDCPrint, &Font);
    pDCPrint->GetTextMetrics(&tm);

	//
    // get printing date and time
	//
    SYSTEMTIME SysTime;
    FILETIME FileTime;
    GetSystemTime(&SysTime);
    SystemTimeToFileTime(&SysTime, &FileTime);
    COleDateTime OleTime(FileTime);
    CPrintUserData * pUserData = (CPrintUserData *)(pPrintInfo->m_lpUserData);

    CString strHelp;
    CSize Size;

	rect.bottom -= tm.tmHeight;

    // print date and time left aligned
    strHelp = OleTime.Format();
	pDCPrint->TextOut(rect.left, rect.bottom, strHelp);

	//
    // print itemname center aligned
	//
    strHelp = pUserData->GetHeaderName();
    Size = pDCPrint->GetTextExtent(strHelp);
	pDCPrint->TextOut((rect.left + rect.right - Size.cx) / 2, rect.bottom, strHelp);

	//
    // print page number right aligned
	//
    strHelp.Format(IDS_FOOTER_PAGENUM, pUserData->GetPageNum());
    Size = pDCPrint->GetTextExtent(strHelp);
	pDCPrint->TextOut(rect.right - Size.cx, rect.bottom, strHelp);

	rect.bottom -= tm.tmHeight;
	pDCPrint->MoveTo(rect.left, rect.bottom);
	pDCPrint->LineTo(rect.right, rect.bottom);
	rect.bottom -= tm.tmHeight;
    pPrintInfo->m_rectDraw.bottom = rect.bottom;

    pDCPrint->SelectObject(pOldFont);
}

/////////////////////////////////////////////////////////////////////////////
// Printing Dialog

static BOOL g_bUserAbort;

class CPrjPrintDialog : public CDialog
{
public:
	//{{AFX_DATA(CPrjPrintDialog)
	enum { IDD = AFX_IDD_PRINTDLG };
	//}}AFX_DATA

	
	CPrjPrintDialog::CPrjPrintDialog(CWnd* pParent)
		{
			Create(CPrjPrintDialog::IDD, pParent);      // modeless !
			g_bUserAbort = FALSE;

			//SU _afxWinState->m_bUserAbort = FALSE;
		}
	virtual ~CPrjPrintDialog() { }

	virtual BOOL OnInitDialog();
	virtual void OnCancel();

protected:

};

BOOL CALLBACK PrjPrintAbortProc(HDC, int)
{
	//SU _AFX_WIN_STATE* pWinState = _afxWinState;

	MSG msg;
	while (!g_bUserAbort &&
		::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE))
	{
		if (!AfxGetThread()->PumpMessage())
			return FALSE;   // terminate if WM_QUIT received
	}
	return !g_bUserAbort;
	//SU return !pWinState->m_bUserAbort;
}

BOOL CPrjPrintDialog::OnInitDialog()
{
	SetWindowText(AfxGetAppName());
	CenterWindow();
	return CDialog::OnInitDialog();
}

void CPrjPrintDialog::OnCancel()
{
	g_bUserAbort = TRUE;
	//SU _afxWinState->m_bUserAbort = TRUE;  // flag that user aborted print
	CDialog::OnCancel();
}


/////////////////////////////////////////////////////////////////////////////
// CELibrarianApp-Befehle

void CELibrarianApp::OnPrintLibrary() 
{
	CString pszLibraryName, pszHeaderName;
	CPrintInfo printInfo;
    CELibrarianApp * pApp = this;

	//
	// get document
	//
	CELibrarianDoc* pDoc = (CELibrarianDoc*) GetDocument();
	pszLibraryName = pDoc->GetTitle();


	// 
	// print dialog
    // Page selection disabled
	//
    printInfo.m_pPD->m_pd.Flags |= PD_NOPAGENUMS;

	if (IDOK != pApp->DoPrintDialog(printInfo.m_pPD))
    {
		return;       // do not print
    }

    // print all pages
    printInfo.m_pPD->m_pd.nToPage = 0xffff;

	if (NULL == printInfo.m_pPD->m_pd.hDC)
    {
		return;
    }

	// gather file to print to if print-to-file selected
	CString strOutput;
	if (printInfo.m_pPD->m_pd.Flags & PD_PRINTTOFILE)
	{
		// construct CFileDialog for browsing
		CString strDef(MAKEINTRESOURCE(AFX_IDS_PRINTDEFAULTEXT));
		CString strPrintDef(MAKEINTRESOURCE(AFX_IDS_PRINTDEFAULT));
		CString strFilter(MAKEINTRESOURCE(AFX_IDS_PRINTFILTER));
		CString strCaption(MAKEINTRESOURCE(AFX_IDS_PRINTCAPTION));
		CFileDialog dlg(FALSE, strDef, strPrintDef,
			OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, strFilter);
		dlg.m_ofn.lpstrTitle = strCaption;

		if (IDOK != dlg.DoModal())
        {
			return;
        }

		// set output device to resulting path name
		strOutput = dlg.GetPathName();
	}

	//
	// set up document info and start the document printing process
	//
	DOCINFO docInfo;
	memset(&docInfo, 0, sizeof(DOCINFO));
	docInfo.cbSize = sizeof(DOCINFO);
	docInfo.lpszDocName = pszLibraryName;
	CString strPortName;
	int nFormatID;
	if (strOutput.IsEmpty())
	{
		docInfo.lpszOutput = NULL;
		strPortName = printInfo.m_pPD->GetPortName();
		nFormatID = AFX_IDS_PRINTONPORT;
	}
	else
	{
		docInfo.lpszOutput = strOutput;
		nFormatID = AFX_IDS_PRINTTOFILE;
	}
	
	//
	// setup the printing DC
	//
	CDC dcPrint;
	dcPrint.Attach(printInfo.m_pPD->m_pd.hDC);  // attach printer dc
	dcPrint.m_bPrinting = TRUE;


	//
	// setup status dialog
	//
	dcPrint.SetAbortProc(PrjPrintAbortProc);

	// disable main window while printing & init printing status dialog
	AfxGetMainWnd()->EnableWindow(FALSE);
	CPrjPrintDialog dlgPrintStatus(m_pMainWnd);

	CString strTemp;

    dlgPrintStatus.SetDlgItemText(AFX_IDC_PRINT_DOCNAME, pszLibraryName);
	dlgPrintStatus.SetDlgItemText(AFX_IDC_PRINT_PRINTERNAME,
		printInfo.m_pPD->GetDeviceName());
	AfxFormatString1(strTemp, nFormatID, strPortName);
	dlgPrintStatus.SetDlgItemText(AFX_IDC_PRINT_PORTNAME, strTemp);

    dlgPrintStatus.ShowWindow(SW_SHOW);
    dlgPrintStatus.UpdateWindow();

	//
	// start printing
	//
	if (dcPrint.StartDoc(&docInfo) == SP_ERROR)
	{
		// enable main window before proceeding
		AfxGetMainWnd()->EnableWindow(TRUE);

		// cleanup and show error message
    	dlgPrintStatus.DestroyWindow();
		dcPrint.Detach();   // will be cleaned up by CPrintInfo destructor
		AfxMessageBox(AFX_IDP_FAILED_TO_START_PRINT);
		return;
	}


    BOOL bError = FALSE;
    CPrintUserData UserData;
    CString strBuffer;
 
    printInfo.m_lpUserData = (LPVOID)&UserData;
    UserData.SetLibraryName(pszLibraryName);

	//
    // print list of POU's
	//
  	//bError = PrintPOUList(&dcPrint, & printInfo);
	bError = FALSE;

	//
	// print all headers pf POU's
	//
	/*CELibrary* pCeLibrary = pDoc->GetCELibrary();
	int nn = pCeLibrary->GetPOUCount();
	CString pouName;
	for ( int ii=0 ; ii<nn && !bError; ii++)
	{	
		pouName = pCeLibrary->GetPOUName(ii);
		if (pouName != ".")
		{	
			UserData.SetHeaderName(pouName);
			bError = PrintPOUHeader( pouName, &dcPrint, &printInfo, &dlgPrintStatus);
		}
	}*/

	if (!bError)
		dcPrint.EndDoc();
	else
		dcPrint.AbortDoc();

	AfxGetMainWnd()->EnableWindow();    // enable main window
	dlgPrintStatus.DestroyWindow();
	dcPrint.Detach();   // will be cleaned up by CPrintInfo destructor
    return;
	
}

BOOL CELibrarianApp::PrintPOUHeader(LPCTSTR pouName, CDC *pDCPrint, 
                      CPrintInfo * pPrintInfo, CDialog* pDlgPrintStatus)
{
	BOOL bError = FALSE;
	CFont Font;
	CFont * pOldFont;

	//
	// get active document and library
	//
	CELibrarianDoc* pDoc = (CELibrarianDoc*) GetDocument();
	//CELibrary* pCeLibrary = pDoc->GetCELibrary();

	//
	// create invisible view to print with
	//
	CEHeaderView* pView = new CEHeaderView();
	CRect rectDefault(0,0,0,0);
	BOOL bb=pView->Create(NULL,NULL,AFX_WS_DEFAULT_VIEW,rectDefault,
		m_pMainWnd,AFX_IDW_PANE_FIRST+1,NULL);
	pDoc->AddView(pView);

 
	pPrintInfo->m_bContinuePrinting = TRUE;
	//TO_DEBUGPRINT1(_T("CE> Printing %s\n"), pszProjectFile);
	pView->OnBeginPrinting(pDCPrint, pPrintInfo);

	// Guarantee values are in the valid range
	UINT nEndPage = pPrintInfo->GetToPage();
	UINT nStartPage = pPrintInfo->GetFromPage();

	if (nEndPage < pPrintInfo->GetMinPage())
		nEndPage = pPrintInfo->GetMinPage();
	if (nEndPage > pPrintInfo->GetMaxPage())
		nEndPage = pPrintInfo->GetMaxPage();

	if (nStartPage < pPrintInfo->GetMinPage())
		nStartPage = pPrintInfo->GetMinPage();
	if (nStartPage > pPrintInfo->GetMaxPage())
		nStartPage = pPrintInfo->GetMaxPage();

	int nStep = (nEndPage >= nStartPage) ? 1 : -1;
	nEndPage = (nEndPage == 0xffff) ? 0xffff : nEndPage + nStep;

	CString strTemp;
	VERIFY(strTemp.LoadString(AFX_IDS_PRINTPAGENUM));


	//
	// create font
	//
	pOldFont = SelectPrintFont(pDCPrint, &Font);

	for (pPrintInfo->m_nCurPage = nStartPage;
		pPrintInfo->m_nCurPage != nEndPage; pPrintInfo->m_nCurPage += nStep)
	{
		pView->OnPrepareDC(pDCPrint, pPrintInfo);

		// check for end of print
		if (!pPrintInfo->m_bContinuePrinting)
			break;

		TCHAR szBuf[80];
		((CPrintUserData *)(pPrintInfo->m_lpUserData))->IncrementPageNum();
		wsprintf(szBuf, strTemp, 
		  ((CPrintUserData *)(pPrintInfo->m_lpUserData))->GetPageNum());
		pDlgPrintStatus->SetDlgItemText(AFX_IDC_PRINT_PAGENUM, szBuf);

		// set up drawing rect to entire page (in logical coordinates)
		pPrintInfo->m_rectDraw.SetRect(0, 0,
			pDCPrint->GetDeviceCaps(HORZRES),
			pDCPrint->GetDeviceCaps(VERTRES));
		pDCPrint->DPtoLP(&pPrintInfo->m_rectDraw);

		// attempt to start the current page
		if (pDCPrint->StartPage() < 0)
		{
			bError = TRUE;
			break;
		}

		//
		// must call OnPrepareDC on newer versions of Windows because
		// StartPage now resets the device attributes.
		pView->OnPrepareDC(pDCPrint, pPrintInfo);

		ASSERT(pPrintInfo->m_bContinuePrinting);

		//
		// page successfully started, so now render the page
		//
		SetMargins(pDCPrint, pPrintInfo);
		PrintStdHeader(pDCPrint, pPrintInfo);
		PrintStdFooter(pDCPrint, pPrintInfo);

		pOldFont = pDCPrint->SelectObject(&Font);
		pView->OnPrintHeader(pouName, pDCPrint, pPrintInfo);
		pView->OnPrint(pDCPrint, pPrintInfo);
		pDCPrint->SelectObject(pOldFont);

		if (pDCPrint->EndPage() < 0  || !PrjPrintAbortProc(pDCPrint->m_hDC, 0))
		{
			bError = TRUE;
			break;
		}
	}

	pView->OnEndPrinting(pDCPrint, pPrintInfo);
	pDoc->RemoveView(pView);
	pView->DestroyWindow();
	//delete pView;

    return bError;
}


/*
 *----------------------------------------------------------------------------*
 *  $History: CELibrarianPrint.cpp $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 18:59
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CL/Librarian/CELibrarian
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:31
 * Created in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 * 
 * *****************  Version 2  *****************
 * User: Oh           Date: 6/25/01    Time: 3:32p
 * Updated in $/4Control/COM/softing/fc/CL/Librarian/CELibrarian
 *==
 *----------------------------------------------------------------------------*
*/
