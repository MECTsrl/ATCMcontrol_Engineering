/////////////////////////////////////////////////////////////////////////////
// @doc MvcBufferedViewport
// @module MvcBufferedVp.h | A viewport that supports offscreen buffering
// of window output to improve performance and reduce screen flicker.
// 
// Objective Diagram<tm>
// <nl>Copyright <cp> 1997 Stingray Software, Inc. All rights reserved.
// 
// This source code is only intended as a supplement to the Objective 
// Diagram User's Guide and related electronic documentation provided with 
// the library. See these sources for detailed information regarding the 
// Objective Diagram product.
// 
// Author: Jeff Boenig
// <nl>Created: 1/98
// 
/////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __MFC_MVCBUFFEREDVP_H__
#define __MFC_MVCBUFFEREDVP_H__

#include <Foundation\MVC\MFC\MvcViewport.h>

// MVC Extension DLL
// Initialize declaration context
#ifdef _SFLDLL
	#undef AFXAPP_DATA
	#define AFXAPP_DATA AFXAPI_DATA
	#undef AFX_DATA
	#define AFX_DATA SFL_DATAEXT
#endif // _SFLDLL

namespace stingray {
namespace foundation {

/////////////////////////////////////////////////////////////////////////////
// MvcBufferedViewport

//@doc MvcBufferedViewport
//@mdata BOOL | MvcBufferedViewport | m_bEnableBuffering | Turn screen buffering on or off
//@xref <c MvcBufferedViewport>

//@doc MvcBufferedViewport
//@mdata CDC* | MvcBufferedViewport | m_pBufferDC | Memory DC for offscreen buffer
//@xref <c MvcBufferedViewport>

//@doc MvcBufferedViewport
//@mdata CBitmap* | MvcBufferedViewport | m_pBufferBmp | Bitmap that is selected into offscreen DC
//@xref <c MvcBufferedViewport>

//@doc MvcBufferedViewport
//@mdata CPalette* | MvcBufferedViewport | m_pBufferPal | Palette used for offscreen buffer
//@xref <c MvcBufferedViewport>


//@doc MvcBufferedViewport
//@mdata HBITMAP | MvcBufferedViewport | m_hPrevBufferBmp | For restoring buffer DC
//@xref <c MvcBufferedViewport>

//@doc MvcBufferedViewport
//@mdata HPALETTE | MvcBufferedViewport | m_hPrevBufferPal | For restoring buffer DC
//@xref <c MvcBufferedViewport>

//@doc MvcBufferedViewport
//@mdata BOOL | MvcBufferedViewport | m_bForceDraw | Flag to force a redraw when the next Refresh occurs
//@xref <c MvcBufferedViewport>

//@doc MvcBufferedViewport
//@mdata int | MvcBufferedViewport | m_nBufPadding | Extra width and height added to buffer to reduce reallocation
//@xref <c MvcBufferedViewport>


// @class A viewport that supports offscreen buffering of window output to
// improve performance and reduce screen flicker.
// @base public | MvcViewport
class MvcBufferedViewport : public MvcViewport
{
// Constructors & Destructor
public:
	// @cmember
	/* Constructor.*/
	MvcBufferedViewport();
	// @cmember
	/* Destructor.*/
	virtual ~MvcBufferedViewport();

// Operations
public:
	// @cmember
	/* Enable or disable use of the offscreen buffer.*/
	void EnableBuffering(const BOOL bEnableBuffering);

	// @cmember
	/* Get pointer to offscreen buffer device context.*/
	CDC* GetBufferDC();

	// @cmember
	/* Set the palette to use for drawing.*/
	void SetPalette(CPalette *pPal);

	// @cmember
	/* Draws the entire diagram to a device context.*/
	virtual void Draw(CDC* pDC);
	// @cmember
	/* Draw a rectangular area of the viewport to a device context.*/
	virtual void DrawRect(CDC* pDC, const CRect &rcUpdate);

	// @cmember
	/* Draw the entire diagram to the offscreen buffer.*/
	void DrawOffscreen();
	// @cmember
	/* Draw a rectangular area of the viewport to the offscreen buffer.*/
	void DrawOffscreen(const CRect &rcUpdate);

	// @cmember
	/* Transfers the entire offscreen buffer to a screen DC.*/
	void Refresh(CDC* pDC);
	// @cmember
	/* Transfers a rectangular area of the offscreen buffer to a screen DC.*/
	void Refresh(CDC* pDC, const CRect &rcUpdate);

	// @cmember
	/* Set padding added to offscreen buffer to reduce reallocation when*/
	// resizing occurs.
	void SetBufferPadding(const int nBufPadding);

// Overrides
public:
	// @cmember
	/* Initializes the viewport.*/
	virtual void OnInitialUpdate();

	// @cmember
	/* Set the width and height of the viewport in device units. Reallocates*/
	// the offscreen buffer, if necessary.
	virtual CSize SetSize(int cx, int cy);
	CSize SetSize(CSize size) { return SetSize(size.cx,size.cy); }
	//@cmember
	/* Set the origin in logical units (i.e. scroll position)*/
	virtual CPoint SetLogOrigin(int x, int y);
	//@cmember
	/* Set the size of the visual part in logical units*/
	virtual CSize SetLogExtents(int cx, int cy);
	//@cmember
	/* Set the size of the visual part in logical units*/
	CSize SetLogExtents(const CSize& sizeLog) { return SetLogExtents(sizeLog.cx,sizeLog.cy); }
	//@cmember
	/* Moves the logical rectangle by the specified deltas*/
	virtual CRect Pan(int xOff,int yOff);

// Implementation
protected:
	// @cmember
	/* Creates memory DC and bitmap for offscreen buffer.*/
	BOOL CreateOffscreenBuffer();
	// @cmember
	/* Compares the current buffer size to the viewport extent.*/
	BOOL NeedToResizeBuffer();
	// @cmember
	/* Deletes the bitmap and memory DC.*/
	void DestroyOffscreenBuffer();
	// @cmember
	/* Creates the DC to be used for the offscreen buffer.*/
	virtual BOOL CreateOffscreenDC();

// Attributes
protected:
	//@access Protected members
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
	/* Flag to force a redraw when the next Refresh occurs*/
	BOOL m_bForceDraw;
	// @cmember
	/* Extra width and height added to buffer to reduce reallocation.*/
	int m_nBufPadding;
};

/////////////////////////////////////////////////////////////////////////////
// MvcBufferedViewport inline methods

// @mfunc Get a pointer to the offscreen buffer device context.
// @rdesc Pointer to memory DC
inline CDC* MvcBufferedViewport::GetBufferDC()
{
	return m_pBufferDC;
}

// @mfunc Enable or disable use of the offscreen buffer.
// @rdesc void
inline void MvcBufferedViewport::EnableBuffering( const BOOL bEnableBuffering )
{
	m_bEnableBuffering = bEnableBuffering;
}

// @mfunc Set padding added to offscreen buffer to reduce reallocation when
// resizing occurs.
// @rdesc void
// @parm Padding width and height in device units.
inline void MvcBufferedViewport::SetBufferPadding( const int nBufPadding )
{
	m_nBufPadding = nBufPadding;
}

};  // namespace stingray::foundation
};  // namespace stingray

/////////////////////////////////////////////////////////////////////////////

// MVC Extension DLL
// Reset declaration context
#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif  // #ifndef __MFC_MVCBUFFEREDVP_H__
