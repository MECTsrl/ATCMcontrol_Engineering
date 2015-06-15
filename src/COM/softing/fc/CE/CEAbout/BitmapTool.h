
///////////////////////////////////////////////////////////////////////////////
//  class CTransBmp
class CTransBmp : public CBitmap
///////////////////////////////////////////////////////////////////////////////
//  class CTransBitmapButton
//      display transparent bitmap as ownerdrawn button in dialog
//      the transparent color is determined by the upper left pixel!
///////////////////////////////////////////////////////////////////////////////
{
public:
// =Construction
                            CTransBmp();
                            ~CTransBmp();

// =Operations
    void                    Draw(HDC hDC, int x, int y);
    void                    Draw(CDC* pDC, int x, int y);
                                // draw bitmap on DC / CDC
    void                    DrawTrans(HDC hDC, int x, int y);
    void                    DrawTrans(CDC* pDC, int x, int y);
                                // draw bitmap transparently on DC / CDC
                                // mask color is determined by upper left pixel
    int                     GetWidth();
                                // width of bitmap in pixels
    int                     GetHeight();
                                // height of bitmap in pixels

//=Helpers
private:
    void                    GetMetrics();
    void                    CreateMask(HDC hDC);

private:
    int                     m_iWidth;
    int                     m_iHeight;
                                // cache of bitmap height and width
    HBITMAP                 m_hbmMask;
                                // Handle to mask bitmap
    COLORREF                m_crTranspMask;
                                // color of transparent pixels
};



/////////////////////////////////////////////////////////////////////////////
// CTransBitmapBtn
class CTransBitmapBtn: public CWindowImpl<CTransBitmapBtn>
{

public:
   CTransBitmapBtn() {
   }

BEGIN_MSG_MAP(CTransBitmapBtn)
   MESSAGE_HANDLER(WM_PAINT, OnPaint)
END_MSG_MAP()

	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
   {
      m_bitmap.DrawTrans(GetDC(), 0, 0);
      bHandled = false;
      return(0);
   }

	BOOL  LoadBitmap(LPCTSTR lpszBitmapResource);
                                // load bitmap resource
	BOOL  LoadBitmap(UINT nIDBitmapResource);
                                // load bitmap resource
	void  SizeToContent();
                                // size button to size of bitmap

// =Attributes
protected:
    CTransBmp               m_bitmap;
};


//------------------------------------------------------------------*
/**
 * Function name			 CTransBmp::Draw
 * Description		draw bitmap on given DC
	
 * @param			CDC* pDC: device context to draw on
 * @param			int x: x/y device coordinates to draw bitmap to
 * @param			int y
 * @return			inline void 
 * @exception			
 * @see			
*/
inline void  CTransBmp::Draw(CDC* pDC, int x, int y)
{
    ASSERT(pDC != NULL);
    Draw(pDC->GetSafeHdc(), x, y);
}

//------------------------------------------------------------------*
/**
 * Function name			CTransBmp::DrawTrans
 * Description		draw the tranparent bitmap on given DC	
 * @param			CDC *pDC: device context to draw on
 * @param			int x: x/y device coordinates to draw bitmap to
 * @param			int y
 * @return			inline void 
 * @exception			
 * @see			
*/
inline void CTransBmp::DrawTrans(CDC *pDC, int x, int y)
{
    ASSERT(pDC != NULL);
    DrawTrans(pDC->GetSafeHdc(), x, y);
}


//------------------------------------------------------------------*
/**
 * Function name			CTransBitmapBtn::LoadBitmap
 * Description		Load the bitmap from int-resource	
 * @param			UINT nIDBitmapResource
 * @return			inline BOOL 
 * @exception			
 * @see			
*/
inline BOOL CTransBitmapBtn::LoadBitmap(UINT nIDBitmapResource)
{
    return LoadBitmap(MAKEINTRESOURCE(nIDBitmapResource));
}

