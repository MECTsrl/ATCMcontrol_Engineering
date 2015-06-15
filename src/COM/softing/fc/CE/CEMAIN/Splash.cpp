
// CG: This file was added by the Splash Screen component.
// Splash.cpp : implementation file
//

#include "stdafx.h"  
#include "resource.h"  

#include "Splash.h"  

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//   Splash Screen class

BOOL CSplashWnd::c_bShowSplashWnd;
CSplashWnd* CSplashWnd::c_pSplashWnd;
CSplashWnd::CSplashWnd()
{
    m_hBitmap = NULL;
    m_pPalette = NULL;
    m_uiTimerId = 0;
}

CSplashWnd::~CSplashWnd()
{
    if(m_uiTimerId) {
        KillTimer(m_uiTimerId);
        m_uiTimerId = 0;
    }

    if (m_pPalette)
    {
        delete m_pPalette;
    }

    if (m_hBitmap)
    {
        DeleteObject(m_hBitmap);
    }

    // Clear the static window pointer.
	ASSERT(c_pSplashWnd == this);
	c_pSplashWnd = NULL;
}

BEGIN_MESSAGE_MAP(CSplashWnd, CWnd)
	//{{AFX_MSG_MAP(CSplashWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CSplashWnd::EnableSplashScreen(BOOL bEnable /*= TRUE*/)
{
	c_bShowSplashWnd = bEnable;
}

void CSplashWnd::ShowSplashScreen(LPCTSTR lpszBitmapName, CWnd* pParentWnd /*= NULL*/)
{
	if (!c_bShowSplashWnd || c_pSplashWnd != NULL)
		return;

	// Allocate a new splash screen, and create the window.
	c_pSplashWnd = new CSplashWnd;
	if (!c_pSplashWnd->Create(lpszBitmapName, pParentWnd))
		delete c_pSplashWnd;
	else
		c_pSplashWnd->UpdateWindow();
}

BOOL CSplashWnd::PreTranslateAppMessage(MSG* pMsg)
{
	if (c_pSplashWnd == NULL)
		return FALSE;

	// If we get a keyboard or mouse message, hide the splash screen.
	if (pMsg->message == WM_KEYDOWN ||
	    pMsg->message == WM_SYSKEYDOWN ||
	    pMsg->message == WM_LBUTTONDOWN ||
	    pMsg->message == WM_RBUTTONDOWN ||
	    pMsg->message == WM_MBUTTONDOWN ||
	    pMsg->message == WM_NCLBUTTONDOWN ||
	    pMsg->message == WM_NCRBUTTONDOWN ||
	    pMsg->message == WM_NCMBUTTONDOWN)
	{
		c_pSplashWnd->HideSplashScreen();
		return TRUE;	// message handled here
	}

	return FALSE;	// message not handled
}

BOOL CSplashWnd::Create(LPCTSTR lpszBitmapName, CWnd* pParentWnd /*= NULL*/)
{
    if(!LoadBitmap(lpszBitmapName)) {
		return FALSE;
    }

	BITMAP bm;
	GetObject(m_hBitmap, sizeof(bm), &bm);

	return CreateEx(0,
		AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW)),
		NULL, WS_POPUP | WS_VISIBLE, 0, 0, bm.bmWidth, bm.bmHeight, pParentWnd->GetSafeHwnd(), NULL);
}

void CSplashWnd::HideSplashScreen()
{
    // Delete palette
    if(m_uiTimerId) {
        KillTimer(m_uiTimerId);
        m_uiTimerId = 0;
    }

    if (m_pPalette)
    {
        delete(m_pPalette);
        m_pPalette = NULL;
    }

    if (m_hBitmap)
    {
        DeleteObject(m_hBitmap);
        m_hBitmap = NULL;
    }

	// Destroy the window, and update the mainframe.
	DestroyWindow();
	
    CWnd * pWnd = AfxGetMainWnd();
    if(pWnd) {
        pWnd->UpdateWindow();
    }

}

void CSplashWnd::PostNcDestroy()
{
	// Free the C++ class.
	delete this;
}

int CSplashWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Center the window.
	CenterWindow();

	// Set a timer to destroy the splash screen.
    //TODO read time from registry
	m_uiTimerId = SetTimer(1, 4500, NULL);
    ASSERT(m_uiTimerId);

	return 0;
}

void CSplashWnd::OnPaint()
{
	CPaintDC dc(this);
    CPalette * pOldPalette;
    HBITMAP hOldBitmap;

	CDC dcImage;
	if (!dcImage.CreateCompatibleDC(&dc))
		return;

	BITMAP bm;
	GetObject(m_hBitmap, sizeof(bm), &bm);

    if (m_pPalette)
    {
       pOldPalette = dc.SelectPalette(m_pPalette, FALSE);
       dc.RealizePalette();
    }

	// Paint the image.
	hOldBitmap = (HBITMAP)::SelectObject(dcImage.m_hDC, m_hBitmap);
	dc.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &dcImage, 0, 0, SRCCOPY);
    ::SelectObject(dcImage.m_hDC, hOldBitmap);

    if (m_pPalette)
    {
       dc.SelectPalette(pOldPalette, FALSE);
    }

}

void CSplashWnd::OnTimer(UINT nIDEvent)
{
	// Destroy the splash screen window.
	HideSplashScreen();
}

BOOL CSplashWnd::CreateDIBPalette (LPBITMAPINFO lpbmi, short int * lpiNumColors) 
{ 
  LPBITMAPINFOHEADER  lpbi;
  LPLOGPALETTE     lpPal;
  int              i;
  BOOL             bRet = FALSE;

  lpbi = (LPBITMAPINFOHEADER)lpbmi;
  if (lpbi->biBitCount <= 8)
  {
    *lpiNumColors = (short int)(1 << lpbi->biBitCount);
  }
  else
  {
    *lpiNumColors = 0;  // No palette needed for 24 BPP DIB
  }

  if (*lpiNumColors)
  {
    lpPal = (LPLOGPALETTE)new BYTE[sizeof (LOGPALETTE) + sizeof (PALETTEENTRY) * (*lpiNumColors)];
    if(lpPal == NULL) {
      ASSERT("CESPLASH> No memory");
      return NULL;
    }

    lpPal->palVersion    = 0x300;
    lpPal->palNumEntries = *lpiNumColors;

    for (i = 0;  i < *lpiNumColors;  i++)
    {
      lpPal->palPalEntry[i].peRed   = lpbmi->bmiColors[i].rgbRed;
      lpPal->palPalEntry[i].peGreen = lpbmi->bmiColors[i].rgbGreen;
      lpPal->palPalEntry[i].peBlue  = lpbmi->bmiColors[i].rgbBlue;
      lpPal->palPalEntry[i].peFlags = 0;
    }

    m_pPalette = new(CPalette);
    ASSERT(m_pPalette);
    if (m_pPalette)
    {
        m_pPalette->CreatePalette(lpPal);
        bRet = TRUE;
    }
    delete lpPal;
  }
  return bRet;
}

BOOL CSplashWnd::LoadBitmap(LPCTSTR lpszBitmapName)
{
    if(m_hBitmap) {
        DeleteObject(m_hBitmap);
        m_hBitmap = NULL;
    }

    m_hBitmap = (HBITMAP)LoadImage(0, lpszBitmapName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE /*| LR_CREATEDIBSECTION*/);
    if(m_hBitmap) {
    }
    return(m_hBitmap != NULL);
}

