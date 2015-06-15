/////////////////////////////////////////////////////////////////////////////
// MvcBufferedWrapper.h : Declaration of MvcBufferedWrapper_T
//
// Stingray Software Extension Classes
// Copyright (C) 1997 Stingray Software Inc,
// All rights reserved.
//
// This source code is only intended as a supplement to
// the Objective Toolkit Class Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding
// the Objective Toolkit product.
//
// Author:       Dean Hallman
// Description:  Declaration of MvcBufferedWrapper_T interface.
//
// $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/inc/STINGRAY/Foundation/MVC/MFC/MvcBufferedWrapper.h 1     28.02.07 18:58 Ln $
// $NoKeywords: $

// @doc MvcBufferedWrapper_T
//
// MvcBufferedWrapper_T class

#pragma once

#ifndef __MFC_MVCBUFFEREDWRAPPER_H__
#define __MFC_MVCBUFFEREDWRAPPER_H__

// Header Includes
#include <Foundation\MVC\MvcWrapper.h>

//
// MVC Extension DLL
// Initialize declaration context
//
#ifdef _SFLDLL
	#undef AFXAPP_DATA
	#define AFXAPP_DATA AFXAPI_DATA
	#undef AFX_DATA
	#define AFX_DATA    SFL_DATAEXT
#endif //_SFLDLL

namespace stingray {
namespace foundation {

//@doc MvcBufferedWrapper_T
//@mdata BOOL | MvcBufferedWrapper_T | m_bEnableBuffering | Turns screen buffering on or off
//@xref <c MvcController>

//@doc MvcBufferedWrapper_T
//@mdata CDC* | MvcBufferedWrapper_T | m_pBufferDC | Memory DC for offscreen buffer
//@xref <c MvcController>

//@doc MvcBufferedWrapper_T
//@mdata CBitmap* | MvcBufferedWrapper_T | m_pBufferBmp | Bitmap that is selected into offscreen DC
//@xref <c MvcController>

//@doc MvcBufferedWrapper_T
//@mdata CPalette* | MvcBufferedWrapper_T | m_pBufferPal | Palette used for offscreen buffer
//@xref <c MvcController>

//@doc MvcBufferedWrapper_T
//@mdata HBITMAP | MvcBufferedWrapper_T | m_hPrevBufferBmp | For restoring buffer DC
//@xref <c MvcController>

//@doc MvcBufferedWrapper_T
//@mdata HPALETTE | MvcBufferedWrapper_T | m_hPrevBufferPal | For restoring buffer DC
//@xref <c MvcController>

//@doc MvcBufferedWrapper_T
//@mdata int | MvcBufferedWrapper_T | m_nBufPadding | Extra width and height added to buffer to reduce reallocation.
//@xref <c MvcController>

//@doc MvcBufferedWrapper_T
//@mdata CRect | MvcBufferedWrapper_T | m_rcUpdate | Update rectangle.
//@xref <c MvcController>


/////////////////////////////////////////////////////////////////////////////
//
// MvcBufferedWrapper_T
//
// @class MvcBufferedWrapper_T |
// The MvcBufferedWrapper_T class wraps a viewport and provides off-screen,
// flicker free rendering.
//
// @tcarg class | base_t | The class of the viewport to derive from.
//

template<class base_t>
class MvcBufferedWrapper_T : public base_t
{
// Constructors & Destructor
public:
	// @cmember
	/* Constructor.*/
	MvcBufferedWrapper_T();
	// @cmember
	/* Destructor.*/
	virtual ~MvcBufferedWrapper_T();

// Attributes
protected:
	// @cmember
	/* Turn screen buffering on or off*/
	BOOL m_bEnableBuffering;
	// @cmember
	/* Memory DC for offscreen buffer*/
	CDC* m_pBufferDC;
	// @cmember
	/* Bitmap that is selected into offscreen DC*/
	CBitmap* m_pBufferBmp;
	// @cmember
	/* Palette used for offscreen buffer*/
	CPalette* m_pBufferPal;
	// @cmember
	/* For restoring buffer DC*/
	HBITMAP m_hPrevBufferBmp;
	// @cmember
	/* For restoring buffer DC*/
	HPALETTE m_hPrevBufferPal;
	// @cmember
	/* Extra width and height added to buffer to reduce reallocation.*/
	int m_nBufPadding;
	// @cmember
	/* Update rectangle.*/
	CRect m_rcUpdate;

// Operations
public:
	// @cmember
	/* Enable or disable use of the offscreen buffer.*/
	void EnableBuffering(const BOOL bEnableBuffering);

	// @cmember
	/* Draws an area of the viewport to the offscreen buffer.*/
	virtual void DrawOffscreen();

	// @cmember
	/* Transfers the entire offscreen buffer to a screen DC.*/
	virtual void Refresh(CDC* pDC);
	// @cmember
	/* Transfers a rectangular area of the offscreen buffer to a screen DC.*/
	virtual void Refresh(CDC* pDC, const CRect &rcUpdate);

	// @cmember
	/* Set the palette to use for drawing to the offscreen buffer.*/
	void SetPalette(CPalette *pPal);

	// @cmember
	/* Set padding added to offscreen buffer to reduce reallocation when*/
	// resizing occurs.
	void SetBufferPadding(const int nBufPadding);

// Overrides
public:
	// @cmember
	/* Draws the entire viewport to a device context.*/
	virtual void Draw(CDC* pDC);

	// @cmember
	/* Initializes the viewport.*/
	virtual void OnInitialUpdate();

	//@cmember
	/* Invalidate the specified rectangle within the viewport.*/
	virtual void InvalidateRect(const CRect& rcLog, BOOL bErase = TRUE, BOOL bRepairNow = FALSE);

	//@cmember
	/* Returns the current invalid region as a rectangle.*/
	virtual BOOL GetUpdateRect(LPRECT lpRect, BOOL bErase = FALSE);

	// @cmember
	/* Set the width and height of the viewport in device units. Reallocates*/
	// the offscreen buffer, if necessary.
	virtual CSize SetSize(int cx, int cy);
	CSize SetSize(CSize size) { return SetSize(size.cx,size.cy); }

// Implementation
protected:
	// @cmember
	/* Creates memory DC and bitmap for offscreen buffer.*/
	BOOL CreateOffscreenBuffer();
	// @cmember
	/* Compares the current buffer size to the viewport dimensions.*/
	BOOL NeedToResizeBuffer();
	// @cmember
	/* Deletes the bitmap and memory DC.*/
	void DestroyOffscreenBuffer();
	// @cmember
	/* Clears the offscreen buffer before rendering.*/
	virtual void OnBufferedEraseBkgnd(CDC* pDC, CRect rcFill);
};

/////////////////////////////////////////////////////////////////////////////
// MvcBufferedWrapper_T methods

// @doc MvcBufferedWrapper_T
// @mfunc Constructor.
template<class base_t>
MvcBufferedWrapper_T<base_t>::MvcBufferedWrapper_T()
{
	m_bEnableBuffering = TRUE;
	m_pBufferDC = NULL;
	m_pBufferBmp = NULL;
	m_pBufferPal = NULL;
	m_hPrevBufferBmp = NULL;
	m_hPrevBufferPal = NULL;
	m_nBufPadding = 100;
	m_rcUpdate = CRect(0,0,0,0);
}

// @doc MvcBufferedWrapper_T
// @mfunc Destructor.
template<class base_t>
MvcBufferedWrapper_T<base_t>::~MvcBufferedWrapper_T()
{
	DestroyOffscreenBuffer();
	SetPalette(NULL);
}

// @doc MvcBufferedWrapper_T
// @mfunc Set the palette to use for drawing to the offscreen buffer.
// @rdesc void
// @parm Pointer to palette to use in offscreen buffer.
template<class base_t>
void MvcBufferedWrapper_T<base_t>::SetPalette(CPalette *pPal)
{
	if (m_pBufferPal != NULL)
	{
		if (m_pBufferDC != NULL && m_hPrevBufferPal != NULL)
		{
			m_pBufferDC->SelectPalette(CPalette::FromHandle(m_hPrevBufferPal), FALSE);
			m_hPrevBufferPal = NULL;
		}

		delete m_pBufferPal;
		m_pBufferPal = NULL;
	}

	// Setup new palette
	if (pPal != NULL)
	{
		ASSERT_VALID(pPal);

		USHORT nEntries = static_cast<USHORT>(pPal->GetEntryCount());
		LPLOGPALETTE pLogPal = reinterpret_cast<LPLOGPALETTE>(new BYTE[ sizeof( LOGPALETTE ) + ( ( nEntries - 1 ) * sizeof( PALETTEENTRY ) ) ]); 
		
		ASSERT(pLogPal != NULL);
		if (pLogPal != NULL)
		{
			pLogPal->palVersion = 0x300;
			pLogPal->palNumEntries = nEntries;
			pPal->GetPaletteEntries(0, nEntries, &pLogPal->palPalEntry[0]);
			
			m_pBufferPal = new CPalette();
			
			ASSERT(m_pBufferPal != NULL);
			if (m_pBufferPal != NULL)
			{
				m_pBufferPal->CreatePalette(pLogPal);
			}

			delete[] pLogPal;
		}
	}

}

// @doc MvcBufferedWrapper_T
// @mfunc Creates memory DC and bitmap for offscreen buffer.
// @rdesc TRUE if offscreen buffer successfully created, otherwise FALSE.
template<class base_t>
BOOL MvcBufferedWrapper_T<base_t>::CreateOffscreenBuffer()
{
	if (!m_bEnableBuffering)
	{
		return FALSE;
	}

	if (m_pBufferDC != NULL || m_pBufferBmp != NULL)
	{
		return FALSE;
	}

	CWnd* pWnd = GetWnd();

	if (pWnd == NULL || !(::IsWindow(pWnd->m_hWnd)))
	{
		return FALSE;
	}

	CRect rcMargins;
	base_t::GetMargins(rcMargins);
	CSize szViewport = GetSize() + rcMargins.Size();

	if (szViewport.cx <= 0 || szViewport.cy <= 0)
	{
		return FALSE;
	}

	CDC* pWndDC = pWnd->GetWindowDC();
	OnPrepareDC(pWndDC);

	m_pBufferDC = new CDC();
	ASSERT(m_pBufferDC != NULL);

	if (m_pBufferDC->CreateCompatibleDC(pWndDC))
	{
		this->OnPrepareDC(m_pBufferDC);

		CSize szBuffer(szViewport);
		szBuffer.cx += m_nBufPadding;
		szBuffer.cy += m_nBufPadding;

		if( m_pBufferPal != NULL )
		{
			CPalette *pal = m_pBufferDC->SelectPalette(m_pBufferPal, FALSE);
			if (pal != NULL)
			{
				m_hPrevBufferPal = static_cast<HPALETTE>(*pal);
				m_pBufferDC->RealizePalette();
			}
		}

		m_pBufferBmp = new CBitmap();
		ASSERT(m_pBufferBmp != NULL);

		BOOL bBmpCreated = m_pBufferBmp->CreateCompatibleBitmap(pWndDC,
		                                                        szBuffer.cx,
		                                                        szBuffer.cy);

		if (bBmpCreated)
		{
			// Bitmap creation successful
			m_pBufferBmp->SetBitmapDimension(szBuffer.cx, szBuffer.cy);
			CBitmap *bmp = m_pBufferDC->SelectObject(m_pBufferBmp);
			if (bmp != NULL)
			{
				m_hPrevBufferBmp = static_cast<HBITMAP>(*bmp);
			}
		}
		else
		{
			// Bitmap creation failed. Destroy the bitmap and DC.

			delete m_pBufferBmp;
			m_pBufferBmp = NULL;

			if (m_pBufferPal != NULL)
			{
				CPalette* pal = m_pBufferDC->SelectPalette(m_pBufferPal, FALSE);
				if (pal != NULL)
				{
					m_hPrevBufferPal = static_cast<HPALETTE>(*pal);
				}
			}

			delete m_pBufferDC;
			m_pBufferDC = NULL;
		}
	}
	else
	{
		delete m_pBufferDC;
		m_pBufferDC = NULL;
	}

	OnCleanupDC(pWndDC);
	pWnd->ReleaseDC(pWndDC);

	return (m_pBufferDC != NULL);
}

// @doc MvcBufferedWrapper_T
// @mfunc Compares the current buffer size to the viewport dimensions.
// @rdesc TRUE if offscreen buffer should be reallocated.
template<class base_t>
BOOL MvcBufferedWrapper_T<base_t>::NeedToResizeBuffer()
{
	BOOL bNeedToResize = FALSE;

	if (m_pBufferBmp != NULL)
	{
		CSize bmpSize = m_pBufferBmp->GetBitmapDimension();
		CRect rcMargins;
		base_t::GetMargins(rcMargins);
		CSize szMargins(rcMargins.left + rcMargins.right, rcMargins.top + rcMargins.bottom);
		CSize szViewport = GetSize() + szMargins;
		bNeedToResize = (bmpSize.cx < szViewport.cx) || (bmpSize.cy < szViewport.cy);
	}

	return bNeedToResize;
}

// @doc MvcBufferedWrapper_T
// @mfunc Deletes the bitmap and memory DC.
// @rdesc void
template<class base_t>
void MvcBufferedWrapper_T<base_t>::DestroyOffscreenBuffer()
{

	// Restore previous GDI objects
	if (m_hPrevBufferPal != NULL && m_pBufferDC != NULL)
	{
		VERIFY(m_pBufferDC->SelectObject(CPalette::FromHandle(m_hPrevBufferPal)));
		m_hPrevBufferPal = NULL;
	}

	if (m_hPrevBufferBmp != NULL && m_pBufferDC != NULL)
	{
		VERIFY(m_pBufferDC->SelectObject(CBitmap::FromHandle(m_hPrevBufferBmp)));
		m_hPrevBufferBmp = NULL;
	}

	// Palette lifetime is controlled by SetPalette and destructor; don't delete the palette.

	delete m_pBufferBmp;
	m_pBufferBmp = NULL;
	
	delete m_pBufferDC;
	m_pBufferDC = NULL;
}

// @doc MvcBufferedWrapper_T
// @mfunc Invalidate the specified rectangle within the viewport.
// @rdesc void
// @parm const CRect& | rcLog | Specifies the rectangle to invalidate, in logical coordinates
// @parmopt BOOL | bErase | TRUE | Should the visual part be completely erased prior to redraw
// @parmopt BOOL | bRepairNow | FALSE | Repair the invalid area immediately
template<class base_t>
void MvcBufferedWrapper_T<base_t>::InvalidateRect(const CRect& rcLog, BOOL bErase, BOOL bRepairNow)
{
	DC dc(this);
	// A viewport with an upward positive Y requires normalization
	CRect rcLogNormal(rcLog);
	rcLogNormal.NormalizeRect();
	m_rcUpdate = rcLogNormal;

	// Windows always omits the right and bottom unit in a rectangle.
	// So, to compensate, we add one to the right and bottom.  Otherwise,
	// we get screen garbage on the right and bottom borders.
	m_rcUpdate.right++;
	m_rcUpdate.bottom++;

	dc.LPtoDP(&m_rcUpdate);
	m_rcUpdate.NormalizeRect();
	GetWnd()->InvalidateRect(m_rcUpdate, bErase);

	DrawOffscreen();

	if (bRepairNow)
	{
		GetWnd()->UpdateWindow();
	}
}

// @doc MvcBufferedWrapper_T
// @mfunc Retreives the coordinates of the smallest rectangle that encloses the update region.
// The bErase parameter specifies whether GetUpdateRect should erase the background of the
// update region.
// @parm LPRECT | lpRect | Rectangle of update region.
// @parm BOOL | bErase | Specifies whether the background of the update region is to be erased.
// @rdesc Specifies the status of the update region.  The value is nonzero if the region is not empty;
// otherwise, 0.
template<class base_t>
BOOL MvcBufferedWrapper_T<base_t>::GetUpdateRect(LPRECT lpRect, BOOL bErase)
{
	BOOL bStatus = GetWnd()->GetUpdateRect(lpRect, bErase);

	if (bStatus && !m_rcUpdate.IsRectEmpty())
	{
		if (m_rcUpdate.left < lpRect->left)
		{
			lpRect->left = m_rcUpdate.left;
		}

		if (m_rcUpdate.right > lpRect->right)
		{
			lpRect->right = m_rcUpdate.right;
		}

		if (m_rcUpdate.top < lpRect->top)
		{
			lpRect->top = m_rcUpdate.top;
		}

		if (m_rcUpdate.bottom > lpRect->bottom)
		{
			lpRect->bottom = m_rcUpdate.bottom;
		}
	}

	return bStatus;
}

// @doc MvcBufferedWrapper_T
// @mfunc Draws an area of the viewport to the offscreen buffer. If buffering is disabled,
// this function does nothing.
// @syntax void MvcBufferedWrapper_T::DrawOffscreen()
// @rdesc void
template<class base_t>
void MvcBufferedWrapper_T<base_t>::DrawOffscreen()
{
	if (m_bEnableBuffering && m_pBufferDC != NULL)
	{
		int nSaveDC = m_pBufferDC->SaveDC();
		OnPrepareDC(m_pBufferDC);

		CRect rcUpdate;

		if (GetWnd()->GetUpdateRect(&rcUpdate, FALSE))
		{
			DPtoLP(&rcUpdate);
		}
		else
		{
			rcUpdate = GetLogBounds();
		}

		// Ensure a normalized rect for viewport's with 
		// an upward positive Y-Axis.
		rcUpdate.NormalizeRect();

		OnBufferedEraseBkgnd(m_pBufferDC, rcUpdate);
		base_t::Draw(m_pBufferDC);

		if (nSaveDC != 0)
		{
			m_pBufferDC->RestoreDC(nSaveDC);
		}

		m_rcUpdate = CRect(0,0,0,0);
	}
}

// @doc MvcBufferedWrapper_T
// @mfunc Draws the entire diagram to a device context.
// @rdesc void
// @parm The device context to draw in.
template<class base_t>
void MvcBufferedWrapper_T<base_t>::Draw(CDC* pDC)
{
	if (m_bEnableBuffering && !pDC->IsPrinting())
	{
		//
		// Perform Refresh() only. Rendering to the offscreen
		// buffer has already been performed in one of the
		// invalidate functions.
		//
		CRect rcUpdate;

		if (!(GetWnd()->GetUpdateRect(&rcUpdate, FALSE)))
		{
			CRect rcMargins;
			base_t::GetMargins(rcMargins);
			rcUpdate = GetBounds();
			rcUpdate.InflateRect(&rcMargins);
		}
		
		// Ensure a normalized rect for viewport's with 
		// an upward positive Y-Axis.
		rcUpdate.NormalizeRect();
		Refresh(pDC, rcUpdate);
	}
	else
	{
		base_t::Draw(pDC);
	}
}

//@doc MvcBufferedWrapper_T
//@mfunc Transfers the offscreen buffer to a screen DC.
//@syntax void MvcBufferedWrapper_T::Refresh(CDC* pDC)
//@syntax void MvcBufferedWrapper_T::Refresh(CDC* pDC, const CRect &rcUpdate)
//@rdesc void
//@parm CDC* | pDC | Device context to refresh.
//@parm CRect& | rcUpdate | Rectangular area to refresh in container units.
template<class base_t>
inline void MvcBufferedWrapper_T<base_t>::Refresh(CDC* pDC)
{
	CRect rcMargins;
	base_t::GetMargins(rcMargins);
	CRect rcBounds = GetBounds();
	rcBounds.InflateRect(&rcMargins);
	Refresh(pDC, rcBounds);
}

template<class base_t>
void MvcBufferedWrapper_T<base_t>::Refresh(CDC* pDC, const CRect &rcUpdate)
{
	CRect rcMargins;
	base_t::GetMargins(rcMargins);
	CRect rcBounds = GetBounds();
	rcBounds.InflateRect(&rcMargins);
	CRect rcClippedUpdate(rcUpdate);
	rcClippedUpdate.IntersectRect(&rcClippedUpdate, &rcBounds);

	base_t::Erase(pDC);

	if (!m_bEnableBuffering || m_pBufferDC == NULL)
	{
		// Buffering is disabled. Revert to normal draw.
		base_t::Draw(pDC);
	}
	else
	{
		//
		// Make sure that source and destination DC's are set
		// to MM_TEXT. The offscreen buffer is already in
		// device units.
		//
		int prevMapMode = pDC->SetMapMode(MM_TEXT);

		pDC->SetViewportOrg(0,0);
		pDC->SetWindowOrg(0,0);

		m_pBufferDC->SetMapMode(MM_TEXT);
		m_pBufferDC->SetViewportOrg(0,0);
		m_pBufferDC->SetWindowOrg(0,0);

		CPalette *pPrevPal = NULL;
		if (m_pBufferPal != NULL)
		{
			pPrevPal = pDC->SelectPalette(m_pBufferPal, FALSE);
			ASSERT(pPrevPal != NULL);
			if (pPrevPal != NULL)
			{
				pDC->RealizePalette();
			}
		}
 
		pDC->BitBlt(rcClippedUpdate.left, rcClippedUpdate.top,
					rcClippedUpdate.Width(), rcClippedUpdate.Height(),
					m_pBufferDC,
					rcClippedUpdate.left, rcClippedUpdate.top,
					SRCCOPY);

		if (pPrevPal != NULL)
		{
			pDC->SelectPalette(pPrevPal, FALSE);
		}

		pDC->SetMapMode(prevMapMode);
	}
}

// @doc MvcBufferedWrapper_T
// @mfunc Called when the viewport is first instantiated. This is used
// to initialize the viewport.
// @rdesc void
template<class base_t>
inline void MvcBufferedWrapper_T<base_t>::OnInitialUpdate()
{
	base_t::OnInitialUpdate();
	CreateOffscreenBuffer();
	DrawOffscreen();
}

// @doc MvcBufferedWrapper_T
// @mfunc Set the width and height of the viewport in device units. Reallocates
// the offscreen buffer, if necessary.
// @rdesc CSize The new size
// @parm int | cx | The horizontal size
// @parm  int | cy | The vertical size
template<class base_t>
CSize MvcBufferedWrapper_T<base_t>::SetSize(int cx, int cy)
{
	CSize rc = base_t::SetSize(cx, cy);

	if (m_bEnableBuffering && GetWnd() != NULL)
	{
		if (m_pBufferDC == NULL || NeedToResizeBuffer())
		{
			DestroyOffscreenBuffer();
			CreateOffscreenBuffer();
		}
		DrawOffscreen();
	}

	return rc;
}

// @doc MvcBufferedWrapper_T
// @mfunc Enable or disable use of the offscreen buffer.
// @rdesc void
// @parm Boolean flag that indicates if buffering should be enabled or disabled.
template<class base_t>
inline void MvcBufferedWrapper_T<base_t>::EnableBuffering(const BOOL bEnableBuffering)
{
	m_bEnableBuffering = bEnableBuffering;
}

// @doc MvcBufferedWrapper_T
// @mfunc Set padding added to offscreen buffer to reduce reallocation when
// resizing occurs.
// @rdesc void
// @parm Padding width and height in device units.
template<class base_t>
inline void MvcBufferedWrapper_T<base_t>::SetBufferPadding(const int nBufPadding)
{
	m_nBufPadding = nBufPadding;
}

// @doc MvcBufferedWrapper_T
// @mfunc Clears the offscreen buffer before rendering.
// @rdesc void
// @parm Pointer to device context to erase.
// @parm Rectangular area to erase.
template<class base_t>
inline void MvcBufferedWrapper_T<base_t>::OnBufferedEraseBkgnd(CDC* pDC, CRect rcFill)
{
	CBrush br(::GetSysColor(COLOR_WINDOW));
	pDC->FillRect(&rcFill, &br);
}

///////////////////////////////////////////////////////////////////////////

};  // namespace stingray::foundation
};  // namespace stingray

//
// MVC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif  // #ifndef __MFC_MVCBUFFEREDWRAPPER_H__
