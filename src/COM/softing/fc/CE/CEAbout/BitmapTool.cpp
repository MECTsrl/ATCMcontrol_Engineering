
#include "stdafx.h"
#include "BitmapTool.h"

//----  Local Defines:   -----------------------------------------------------*
// Colors
#define rgbWhite RGB(255,255,255)
// Raster op codes
#define DSa     0x008800C6L
#define DSx     0x00660046L


//----  Static Initializations:   --------------------------------------------*
#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////////
//------------- MEMBER FUNCTIONS OF CLASS CTransBitmapBtn
///////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------*
/**
 * Function name			  CTransBitmapBtn::LoadBitmap
 * Description		load the bitmap from resource	
 * @param			LPCTSTR lpszBitmapResource
 * @return			BOOL if bitmap found
  
 * @exception			
 * @see			
*/
BOOL CTransBitmapBtn::LoadBitmap(LPCTSTR lpszBitmapResource) {
   // delete old bitmaps (if present)
   m_bitmap.DeleteObject();

   if(!m_bitmap.LoadBitmap(lpszBitmapResource)) {
      if(HIWORD(lpszBitmapResource) != 0) {
			TRACE1("ERROR: Cannot find bitmap resource named '%s'.\n",
				lpszBitmapResource);
      } else {
			TRACE1("ERROR: Cannot find bitmap resource with IDB 0x%04X.\n",
				LOWORD((DWORD)lpszBitmapResource));
      }
      return(false);   // need this one image
   }
   return(true);
}


//------------------------------------------------------------------*
/**
 * Function name			CTransBitmapBtn::SizeToContent
 * Description		resize the button to the size of the bitmap	
 * @return			void 
 * @exception			
 * @see			
*/
void CTransBitmapBtn::SizeToContent() {
    ASSERT(m_bitmap.m_hObject != NULL);

    VERIFY(SetWindowPos(NULL, -1, -1,
                        m_bitmap.GetWidth(), m_bitmap.GetHeight(),
                        SWP_NOMOVE|SWP_NOZORDER|SWP_NOREDRAW|SWP_NOACTIVATE));
}


///////////////////////////////////////////////////////////////////////////////
//------------- MEMBER FUNCTIONS OF CLASS CTransBmp
///////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------*
/**
 * Function name			CTransBmp::CTransBmp
 * Description	    Constuctor		
 * @return			
 * @exception			
 * @see			
*/
CTransBmp::CTransBmp(): 
    m_hbmMask(NULL),
    m_crTranspMask(0),
    m_iWidth(0),
    m_iHeight(0)
{
}

//------------------------------------------------------------------*
/**
 * Function name			CTransBmp::~CTransBmp
 * Description	    Destructor		
 * @return			
 * @exception			
 * @see			
*/
CTransBmp::~CTransBmp() {
    if (m_hbmMask) {
        ::DeleteObject(m_hbmMask);
    }
}
  
//------------------------------------------------------------------*
/**
 * Function name			CTransBmp::GetMetrics
 * Description		update cache with bitmap width/height	
 * @return			void 
 * @exception			
 * @see			
*/
void CTransBmp::GetMetrics() {
    // Get the width and height into cache
    ASSERT(m_hObject);
    BITMAP      bm;
    GetObject(sizeof(bm), &bm);
    m_iWidth  = bm.bmWidth;
    m_iHeight = bm.bmHeight;
}

//------------------------------------------------------------------*
/**
 * Function name			CTransBmp::GetWidth
 * Description		returns the width of bitmap	
 * @return			int
 * @exception			
 * @see			
*/
int CTransBmp::GetWidth() {
    if((m_iWidth == 0) || (m_iHeight == 0)){
        GetMetrics();
    }
    return m_iWidth;
}

//------------------------------------------------------------------*
/**
 * Function name			CTransBmp::GetHeight
 * Description		returns the height of bitmap	
 * @return			int 
 * @exception			
 * @see			
*/
int CTransBmp::GetHeight() {
    if((m_iWidth == 0) || (m_iHeight == 0)){
        GetMetrics();
    }
    return m_iHeight;
}

//------------------------------------------------------------------*
/**
 * Function name			CTransBmp::CreateMask
 * Description		create the mask for the transparent color
 *                  the upper left pixel determines the transparent color	
 * @param			HDC hDC
 * @return			void 
 * @exception			
 * @see			
*/
void CTransBmp::CreateMask(HDC hDC) {
    ASSERT(m_hObject);
    ASSERT(hDC);

    // Nuke any existing mask.
    if (m_hbmMask) {
        ::DeleteObject(m_hbmMask);
    }

    // Create memory DCs to work with.
    CDC *pCDC = CDC::FromHandle(hDC);
    ASSERT(pCDC);
    CDC cdcMask;
    CDC cdcImage;
    cdcMask.CreateCompatibleDC(pCDC);
    cdcImage.CreateCompatibleDC(pCDC);

    // Create a monochrome bitmap for the mask.
    m_hbmMask = ::CreateBitmap(GetWidth(), GetHeight(), 1, 1, NULL);

    // Select the mono bitmap into its DC.
    HBITMAP hbmOldMask = (HBITMAP) cdcMask.SelectObject(m_hbmMask);

    // Select the image bitmap into its DC.
    HBITMAP hbmOldImage = (HBITMAP) cdcImage.SelectObject(m_hObject);

    // Set the transparency color to be the top-left pixel.
    m_crTranspMask = cdcImage.GetPixel(0, 0);
    cdcImage.SetBkColor(m_crTranspMask);

    // Make the mask.
    cdcMask.BitBlt(0, 0, GetWidth(), GetHeight(),
        &cdcImage, 0, 0,
        SRCCOPY);

    cdcMask.SelectObject(hbmOldMask);
    cdcImage.SelectObject(hbmOldImage);
}

//------------------------------------------------------------------*
/**
 * Function name			CTransBmp::Draw
 * Description		draw bitmap on given DC	
 * @param			HDC hDC: device context to draw on
 * @param			int x: x/y device coordinates to draw bitmap to
 * @param			int y
 * @return			void 
 * @exception			
 * @see			
*/
void CTransBmp::Draw(HDC hDC, int x, int y) {
    ASSERT(hDC);
    ASSERT(m_hObject);
    // Create a memory DC.
    CDC *pCDC = CDC::FromHandle(hDC);
    ASSERT(pCDC);
    CDC cdcMem;
    cdcMem.CreateCompatibleDC(pCDC);
    // Select the bitmap into the mem DC.
    HBITMAP hbmold = 
        (HBITMAP) cdcMem.SelectObject((HBITMAP)(m_hObject));
    // Blt the bits.
    pCDC->BitBlt(x, y, GetWidth(), GetHeight(),
        &cdcMem, 0, 0,
        SRCCOPY);
    cdcMem.SelectObject(hbmold);
}

//------------------------------------------------------------------*
/**
 * Function name			CTransBmp::DrawTrans
 * Description		draw bitmap transparently on given DC	
 * @param			HDC hDC: device context to draw on
 * @param			int x: x/y device coordinates to draw bitmap to
 * @param			int y
 * @return			void 
 * @exception			
 * @see			
*/
void CTransBmp::DrawTrans(HDC hDC, int x, int y) {
    ASSERT(hDC);
    if(!m_hbmMask) {
        CreateMask(hDC);
    }
    ASSERT(m_hObject);
    ASSERT(m_hbmMask);

    int dx = GetWidth();
    int dy = GetHeight();

    // Create a memory DC to which to draw.
    HDC hdcOffScr = ::CreateCompatibleDC(hDC);
    // Create a bitmap for the off-screen DC that is really
    // color-compatible with the destination DC.
    HBITMAP hbmOffScr = ::CreateBitmap(dx, dy, 
                             (BYTE)GetDeviceCaps(hDC, PLANES),
                             (BYTE)GetDeviceCaps(hDC, BITSPIXEL),
                             NULL);
    // Select the buffer bitmap into the off-screen DC.
    HBITMAP hbmOldOffScr = (HBITMAP)::SelectObject(hdcOffScr, hbmOffScr);

    // Copy the image of the destination rectangle to the
    // off-screen buffer DC, so we can play with it.
    ::BitBlt(hdcOffScr, 0, 0, dx, dy,
             hDC,       x, y,
             SRCCOPY);

    // Create a memory DC for the source image.
    HDC hdcImage = ::CreateCompatibleDC(hDC); 
    HBITMAP hbmOldImage = (HBITMAP)::SelectObject(hdcImage, m_hObject);

    // Create a memory DC for the mask.
    HDC hdcMask = ::CreateCompatibleDC(hDC);
    HBITMAP hbmOldMask = (HBITMAP)::SelectObject(hdcMask, m_hbmMask);

    // XOR the image with the destination.
    ::SetBkColor(hdcOffScr, rgbWhite);
    ::BitBlt(hdcOffScr, 0, 0, dx, dy,
             hdcImage,  0, 0,
             DSx);
    // AND the destination with the mask.
    ::BitBlt(hdcOffScr, 0, 0, dx, dy,
             hdcMask,   0, 0,
             DSa);
    // XOR the destination with the image again.
    ::BitBlt(hdcOffScr, 0, 0, dx, dy,
             hdcImage,  0, 0,
             DSx);

    // Copy the resultant image back to the screen DC.
    ::BitBlt(hDC,       x, y, dx, dy,
             hdcOffScr, 0, 0,
             SRCCOPY);

    // Tidy up.
    ::SelectObject(hdcOffScr, hbmOldOffScr);
    ::SelectObject(hdcImage, hbmOldImage);
    ::SelectObject(hdcMask, hbmOldMask);
    ::DeleteObject(hbmOffScr);
    ::DeleteDC(hdcOffScr);
    ::DeleteDC(hdcImage);
    ::DeleteDC(hdcMask);
}
  
