#ifndef __VIEW_CORE_INL__
#define __VIEW_CORE_INL__
// 
// Stingray Software Extension Classes
// Copyright (C) 1997 Steve Schauer
// Copyright (C) 1997 Stingray Software Inc.
// All Rights Reserved.
// 
// This source code is only intended as a supplement to the
// Stingray Extension Class product.
// See the SEC help files for detailed information
// regarding using SEC classes.
// 
//----------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////
//
// View_Core.cpp
//
//
// Description: SECEditCore construction/initialization
//
//


#include "SECEdit.h"


//@doc SECEditCore
//@mfunc This virtual function *must* be overridden in your derived
//              class to return a pointer to a valid SECEdit object.
//              Typically the SECEdit object will be a member variable
//              of your CDocument derived class.
// <nl> For Example:     return &(GetDocument()->m_Edit);
//@rdesc SECEdit* A  pointer to the SECEdit object for your application.

template <class BC>
SECEdit* SECEditCore<BC>::GetEdit()
   {
   // This function must be overridden.  See the sample MDIApp for
   // a real example.
   ASSERT(0);
	return NULL;
	}


//@doc SECEditCore
//@mfunc Internal function that initialize default SECEditCore settings
//@rdesc int 
template <class BC>
int SECEditCore<BC>::InitView()
   {
	SECEdit* pEdit = GetEdit();
	ASSERT(pEdit != NULL);

	m_pTopLine = m_pCurLine = pEdit->ListGetFirst(pEdit->m_pLine, NULL);
   m_iFirstLineNo = 1;
   m_iLineNo = 1;
   m_iColNo = 0;
	pEdit->m_iBookmarkCount = CountBookmarks();
	m_iLeftEdge = 0;
	m_iMaxChars = 80;
	m_iBlockType = OE_STREAM;
	m_bSelecting = FALSE;
	m_BlockStart.pLine = NULL;
	m_BlockStart.iOffset = 0;
	m_BlockEnd = m_BlockStart;
	m_bTrack = FALSE;
	m_iTimer = 0;
	m_bEatIt = FALSE;
	GetClientRect(m_rect); 
	m_bHaveCaret = FALSE;
	m_badrect.SetRectEmpty();  // for MakeCursorVisible()
   m_bUpdate = TRUE;
	m_bCopyLine = FALSE;
	m_bCutLine = FALSE;
	m_finddlg = NULL;
	m_bFinding = FALSE;
	m_replacedlg = NULL;
	m_bReplacing = FALSE;
	m_hPrinterFont = NULL;
	LOGFONT lf;
	CClientDC dc(this);
	memset(&lf,0,sizeof(lf));
   _tcscpy(lf.lfFaceName,pEdit->m_fontname);
   lf.lfHeight = MulDiv(-(pEdit->m_fontsize), dc.GetDeviceCaps(LOGPIXELSY), 72);
	lf.lfWeight = pEdit->m_fontweight;
	lf.lfItalic = (char)pEdit->m_fontitalic;
   lf.lfCharSet = (char)pEdit->m_fontcharset;
   lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
   lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
   lf.lfQuality = DEFAULT_QUALITY;
   lf.lfPitchAndFamily = FIXED_PITCH | FF_MODERN;
	SetFont(lf);

	MySetScrollRange(SB_VERT, 0, pEdit->m_iLinesInFile-1, m_bUpdate);
   MySetScrollRange(SB_HORZ, 0, OE_MAXLINELENGTH, m_bUpdate);

	// Load the accelerator Table
	HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE(IDR_SEC_DEVSTUDIO_ACCEL), RT_ACCELERATOR);
	m_hAccelTable = ::LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_SEC_DEVSTUDIO_ACCEL));

	// This will make the scroll bar do the right thing when it is first 
	// created in a splitter window
	if (m_iFirstLineNo)
		MySetScrollPos(SB_VERT, m_iFirstLineNo-1,TRUE);

   return 0;
   }

//@doc SECEditCore
//@mfunc Changes the font for the SECEditCore to the font passed in through the font argument.
//@rdesc void 
//@parm LOGFONT& font A LOGFONT structure for the font of the view.

template <class BC>
void SECEditCore<BC>::SetFont(LOGFONT& font)
{
	CClientDC dc(this);
	TEXTMETRIC metrics;

	// fixed pitch only
	ASSERT(font.lfPitchAndFamily & FIXED_PITCH);
	font.lfPitchAndFamily |= FIXED_PITCH;

	m_font.DeleteObject();
	m_font.CreateFontIndirect(&font);
	CFont* oldfont = dc.SelectObject(&m_font);
	dc.GetTextMetrics(&metrics);
#ifdef ORIGINAL_VERSION
	m_iMarkSpace = m_iLineHeight = metrics.tmHeight+metrics.tmExternalLeading;
#else
	m_iLineHeight = metrics.tmHeight+metrics.tmExternalLeading;
	m_iMarkSpace = m_iLineHeight * 13 / 10;
#endif
	m_iCharWidth = metrics.tmAveCharWidth;
	dc.SelectObject(oldfont);
}


//@doc SECEditCore
//@mfunc Sets the SECEditCore’s current font to the specified font. 
//              If bRedraw is TRUE, the window will also be redrawn.
//@rdesc void 
//@parm  CFont* pFont Pointer to a CFont which is the new font to use for the SECEditCore
//@parm  BOOL bRedraw Redraw the window?
template <class BC>
void SECEditCore<BC>::SetFont( CFont* pFont, BOOL bRedraw )
{
	CClientDC dc(this);
	TEXTMETRIC metrics;

	LOGFONT lf;
	pFont->GetLogFont(&lf);

	// fixed pitch only
	ASSERT(lf.lfPitchAndFamily & FIXED_PITCH);
	lf.lfPitchAndFamily |= FIXED_PITCH;

	m_font.DeleteObject();
	m_font.CreateFontIndirect(&lf);
	CFont* oldfont = dc.SelectObject(&m_font);
	dc.GetTextMetrics(&metrics);
#ifdef ORIGINAL_VERSION
	m_iMarkSpace = m_iLineHeight = metrics.tmHeight+metrics.tmExternalLeading;
#else
	m_iLineHeight = metrics.tmHeight+metrics.tmExternalLeading;
	m_iMarkSpace = m_iLineHeight * 13 / 10;
#endif
	m_iCharWidth = metrics.tmAveCharWidth;
	dc.SelectObject(oldfont);
	if (bRedraw)
	{
		InvalidateRect(NULL,FALSE);
		UpdateWindow();
	}
}


//@doc SECEditCore
//@mfunc Gets the current font for this view.
//@rdesc CFont* A pointer to a CFont that contains the current display font used by the SECEditCore.
template <class BC>
CFont* SECEditCore<BC>::GetFont( )
	{
	return &m_font;
	}
  

#endif //__VIEW_CORE_INL__