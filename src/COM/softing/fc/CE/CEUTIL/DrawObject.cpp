
//----  Local Defines:   -------------------------------------------*

//----  Includes:   -------------------------------------------*
#include "stdafx.h"
#include "DrawObject.h"

//----  Static Initializations:   ----------------------------------*


/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------*
/**
 * draw object constructor.
 *
 *  by default the alignment is top left and all draw flags are set
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
CDrawObject::CDrawObject() :
  m_eHorzAlignment(eHA_left),
  m_eVertAlignment(eVA_top),
  m_uiDrawFlags(-1)
{
}

//------------------------------------------------------------------*
/**
 * draw object destructor.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
CDrawObject::~CDrawObject()
{
}

//------------------------------------------------------------------*
/**
 * get horizontal alignment.
 *
 * @param           -
 * @return          horizontal alignment
 * @exception       -
 * @see             SetHorizontalAlignment()
*/
EHorizontalAlignment CDrawObject::GetHorizontalAlignment()
{
    return m_eHorzAlignment;
}

//------------------------------------------------------------------*
/**
 * set horizontal alignment.
 *
 * @param           eHorzAlignment: horizontal alignment
 * @return          -
 * @exception       -
 * @see             GetHorizontalAlignment()
*/
void CDrawObject::SetHorizontalAlignment(EHorizontalAlignment eHorzAlignment)
{
    m_eHorzAlignment = eHorzAlignment;
}


//------------------------------------------------------------------*
/**
 * get vertical alignment
 *
 * @param           -
 * @return          vertical alignment
 * @exception       -
 * @see             SetVerticalAlignment()
*/
EVerticalAlignment CDrawObject::GetVerticalAlignment()
{
    return m_eVertAlignment;
}

//------------------------------------------------------------------*
/**
 * set vertical alignment
 *
 * @param           eVertAlignment: vertical alignment
 * @return          -
 * @exception       -
 * @see             SetVerticalAlignment()
*/
void CDrawObject::SetVerticalAlignment(EVerticalAlignment eVertAlignment)
{
    m_eVertAlignment = eVertAlignment;
}

//------------------------------------------------------------------*
/**
 * get draw flags.
 *
 * @param           -
 * @return          draw flags as unsigned integer
 * @exception       -
 * @see             SetDrawFlags()
*/
UINT CDrawObject::GetDrawFlags()
{
    return m_uiDrawFlags;
}

//------------------------------------------------------------------*
/**
 * set draw flags.
 *
 * @param           uiDrawFlags: draw flags as unsigned integer
 * @return          -
 * @exception       -
 * @see             GetDrawFlags()
*/
void CDrawObject::SetDrawFlags(UINT uiDrawFlags)
{
    m_uiDrawFlags = uiDrawFlags;
}



/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------*
/**
 * draw rect constructor.
 *
 *  by default the line width is 0.3 mm and the line color is black.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             CDrawRect::CDrawRect(1)
*/
CDrawRect::CDrawRect() :
  m_tRect(0,0,0,0),
  m_iLineWidth(3),
  m_uiLineColor(0)
{
}

//------------------------------------------------------------------*
/**
 * draw rect constructor with input rect.
 *
 *  by default the line width is 0.3 mm and the line color is black.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
CDrawRect::CDrawRect(const CRect& crtRect) :
  m_tRect(crtRect),
  m_iLineWidth(1),
  m_uiLineColor(0)
{
}

//------------------------------------------------------------------*
/**
 * draw rect destructor.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
CDrawRect::~CDrawRect()
{
}

//------------------------------------------------------------------*
/**
 * get rect.
 *
 * @param           -
 * @return          rect object.
 * @exception       -
 * @see             SetRect()
*/
CRect CDrawRect::GetRect()
{
    return m_tRect;
}

//------------------------------------------------------------------*
/**
 * set rect.
 *
 * @param           crtRect: input rect
 * @return          -
 * @exception       -
 * @see             GetRect()
*/
void CDrawRect::SetRect(const CRect& crtRect)
{
    m_tRect = crtRect;
}

//------------------------------------------------------------------*
/**
 * get line width.
 *
 * @param           -
 * @return          line width in logical units
 * @exception       -
 * @see             SetLineWidth()
*/
int CDrawRect::GetLineWidth()
{
    return m_iLineWidth;
}

//------------------------------------------------------------------*
/**
 * set line width.
 *
 * @param           iLineWidth: line width in logical units
 * @return          -
 * @exception       -
 * @see             GetLineWidth()
*/
void CDrawRect::SetLineWidth(int iLineWidth)
{
    m_iLineWidth = iLineWidth;
}

//------------------------------------------------------------------*
/**
 * get line color.
 *
 * @param           -
 * @return          line color
 * @exception       -
 * @see             SetLineColor()
*/
UINT CDrawRect::GetLineColor()
{
    return m_uiLineColor;
}

//------------------------------------------------------------------*
/**
 * set line color.
 *
 * @param           uiLineColor: line color
 * @return          -
 * @exception       -
 * @see             GetLineColor()
*/
void CDrawRect::SetLineColor(UINT uiLineColor)
{
    m_uiLineColor = uiLineColor;
}


//------------------------------------------------------------------*
/**
 * get position.
 *
 *  get current position.
 *  this is the fixpoint within the draw rect and is dependent
 *  on the alignment (e. g. top left)
 *
 * @param           -
 * @return          current position.
 * @exception       -
 * @see             SetPosition()
*/
CPoint CDrawRect::GetPosition()
{
    return GetRectFixpoint(m_tRect, m_eHorzAlignment, m_eVertAlignment);
}



//------------------------------------------------------------------*
/**
 * set position.
 *
 *  use this in combination with the alignment to set the rect to
 *  the desired position.
 *  example: put draw rect in bottom right corner of the drawing rect
 *           (this point is called tDrawRectBotRight)
 *  1. set horizontal alignment to HA_right
 *  2. set vertical alignment to VA_bottom
 *  3. call SetPosition(): SetPosition(tDrawRectBotRight);
 *  that's it.
 *
 * @param           crtPoint: new position.
 * @return          -
 * @exception       -
 * @see             GetPosition(), Move()
*/
void CDrawRect::SetPosition(const CPoint& crtPoint)
{
    CPoint  tMove(crtPoint);
    CPoint  tPoint;
    
    tPoint = GetPosition();
    tMove.Offset(-tPoint);
    Move(tMove);
}

//------------------------------------------------------------------*
/**
 * align.
 *
 *  aligns to the input rect according to the current alignment settings.
 *
 * @param           crtRect: rect to align to
 * @return          -
 * @exception       -
 * @see             -
*/
void CDrawRect::Align(const CRect& crtRect)
{
    EDrawAlignment eAlign = (EDrawAlignment) (m_eHorzAlignment * m_eVertAlignment);
    CPoint  tPointSource = GetPosition();   // start point for move vector
    // end point for move vector
    CPoint  tPointDest = GetRectFixpoint(crtRect, m_eHorzAlignment, m_eVertAlignment);
    CPoint  tMove = tPointDest - tPointSource;  // move vector as point
    Move(tMove);
}

//------------------------------------------------------------------*
/**
 * fit.
 *
 *  calculates new size for CDrawRect to fit optimal in input rect.
 *  position and aspect ratio are preserved.
 *
 * @param           
 * @return          
 * @exception       -
 * @see             
*/
void CDrawRect::Fit(const CRect& crtRect)
{
    CSize tFitSize = GetFitSize(m_tRect, crtRect);
    SetSize(tFitSize);
}


//------------------------------------------------------------------*
/**
 * get size.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             SetSize()
*/
CSize CDrawRect::GetSize()
{
    return m_tRect.Size();
}

//------------------------------------------------------------------*
/**
 * set size.
 *
 *  the position stays unchanged.
 *
 * @param           crtSize: new size
 * @return          -
 * @exception       -
 * @see             GetSize(), Move()
*/
void CDrawRect::SetSize(const CSize& crtSize)
{
    CPoint  tOldPosition;
    CPoint  tNewPosition;

    tOldPosition = GetPosition();

    m_tRect.top = m_tRect.left = 0;
    m_tRect.bottom = crtSize.cy;
    m_tRect.right = crtSize.cx;
    
    tNewPosition = GetPosition();

    CPoint  tMove(tOldPosition);
    tMove.Offset(-tNewPosition);
    Move(tMove);
}


//------------------------------------------------------------------*
/**
 * move.
 *
 * @param           crtPoint: vector for moving as point
 * @return          -
 * @exception       -
 * @see             -
*/
void CDrawRect::Move(const CPoint& crtPoint)
{
    m_tRect += crtPoint;
}

//------------------------------------------------------------------*
/**
 * draw.
 *
 * @param           pDC: device context.
 * @return          -
 * @exception       -
 * @see             -
*/
void CDrawRect::Draw(CDC* pDC)
{
    if(m_uiDrawFlags & DF_FRAME)
    {
        // set pen and brush
        CPen    tPen;
        CBrush  tBrush;
        CPen*   pOldPen;
        CBrush* pOldBrush;

        LOGBRUSH    tLogBrush;
        tLogBrush.lbStyle = BS_NULL;

        tPen.CreatePen(PS_SOLID, m_iLineWidth, m_uiLineColor);
        tBrush.CreateBrushIndirect(&tLogBrush);

        pOldPen = pDC->SelectObject(&tPen);
        pOldBrush = pDC->SelectObject(&tBrush);

//        pDC->Rectangle(&m_tRect);

        pDC->MoveTo(m_tRect.left, m_tRect.top);
        pDC->LineTo(m_tRect.right, m_tRect.top);
        pDC->LineTo(m_tRect.right, m_tRect.bottom);
        pDC->LineTo(m_tRect.left, m_tRect.bottom);
        pDC->LineTo(m_tRect.left, m_tRect.top);

        pDC->SelectObject(pOldPen);
    }
}



/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------*
/**
 * text rect constructor.
 *
 *  by default no font is selected, multi line is true, text alignment 
 *  is left mid, text offset is 0
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
CTextRect::CTextRect() :
  m_pFont(NULL),
  m_bMultiline(TRUE),
  m_eHorzTextAlignment(eHA_left),
  m_eVertTextAlignment(eVA_mid),
  m_iTextOffset(0)
{
}

//------------------------------------------------------------------*
/**
 * text rect constructor with rect and text parameter.
 *
 *  by default no font is selected, multi line is true, text alignment 
 *  is left mid, text offset is 0
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
CTextRect::CTextRect(const CRect& crtRect, const CString& crstrText /*= _T("")*/) :
  CDrawRect(crtRect),
  m_strText(crstrText),
  m_pFont(NULL),
  m_bMultiline(TRUE),
  m_eHorzTextAlignment(eHA_left),
  m_eVertTextAlignment(eVA_mid),
  m_iTextOffset(0)
{
}


//------------------------------------------------------------------*
/**
 * text rect destructor.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
CTextRect::~CTextRect()
{
}

//------------------------------------------------------------------*
/**
 * get text.
 *
 * @param           -
 * @return          text string
 * @exception       -
 * @see             SetText()
*/
CString CTextRect::GetText()
{
    return m_strText;
}

//------------------------------------------------------------------*
/**
 * set text.
 *
 * @param           crstrText: input text string
 * @return          -
 * @exception       -
 * @see             GetText()
*/
void CTextRect::SetText(const CString& crstrText)
{
    m_strText = crstrText;
}

//------------------------------------------------------------------*
/**
 * get font.
 *
 * @param           -
 * @return          font pointer
 * @exception       -
 * @see             SetFont()
*/
CFont* CTextRect::GetFont()
{
    return m_pFont;
}

//------------------------------------------------------------------*
/**
 * set font.
 *
 * @param           pFont: font pointer
 * @return          -
 * @exception       -
 * @see             GetFont()
*/
void CTextRect::SetFont(CFont* pFont)
{
    m_pFont = pFont;
}

//------------------------------------------------------------------*
/**
 * get multiline flag.
 *
 * @param           -
 * @return          is multiline text rect?
 * @exception       -
 * @see             SetMultilineFlag()
*/
BOOL CTextRect::GetMultilineFlag()
{
    return m_bMultiline;
}

//------------------------------------------------------------------*
/**
 * set multiline flag.
 *
 * @param           bMultiline: multi line flag.
 * @return          -
 * @exception       -
 * @see             GetMultilineFlag()
*/
void CTextRect::SetMultilineFlag(BOOL bMultiline)
{
    m_bMultiline = bMultiline;
}

//------------------------------------------------------------------*
/**
 * get horizontal text alignment.
 *
 * @param           -
 * @return          horizontal text alignment.
 * @exception       -
 * @see             SetHorizontalTextAlignment()
*/
EHorizontalAlignment CTextRect::GetHorizontalTextAlignment()
{
    return m_eHorzTextAlignment;
}

//------------------------------------------------------------------*
/**
 * set horizontal text alignment.
 *
 * @param           eHorzTextAlignment: horizontal text alignment
 * @return          -
 * @exception       -
 * @see             GetHorizontalTextAlignment()
*/
void CTextRect::SetHorizontalTextAlignment(EHorizontalAlignment eHorzTextAlignment)
{
    m_eHorzTextAlignment = eHorzTextAlignment;
}


//------------------------------------------------------------------*
/**
 * get vertical text alignment.
 *
 * @param           -
 * @return          vertical text alignment.
 * @exception       -
 * @see             SetVerticalTextAlignment()
*/
EVerticalAlignment CTextRect::GetVerticalTextAlignment()
{
    return m_eVertTextAlignment;
}

//------------------------------------------------------------------*
/**
 * set vertical text alignment.
 *
 * @param           eVertTextAlignment: vertical text alignment
 * @return          -
 * @exception       -
 * @see             GetVerticalTextAlignment()
*/
void CTextRect::SetVerticalTextAlignment(EVerticalAlignment eVertTextAlignment)
{
    m_eVertTextAlignment = eVertTextAlignment;
}


//------------------------------------------------------------------*
/**
 * get text offset.
 *
 * @param           -
 * @return          text offset in logical units
 * @exception       -
 * @see             SetTextOffset()
*/
int CTextRect::GetTextOffset()
{
    return m_iTextOffset;
}

//------------------------------------------------------------------*
/**
 * set text offset.
 *
 *  text offset is not used with HA_center and VA_mid
 *
 * @param           iTextOffset: text offset from rect border in logical units
 * @return          -
 * @exception       -
 * @see             GetTextOffset()
*/
void CTextRect::SetTextOffset(int iTextOffset)
{
    m_iTextOffset = iTextOffset;
}


//------------------------------------------------------------------*
/**
 * get text position.
 *
 *  get current text position.
 *  this is the fixpoint within the text rect and is dependent
 *  on the alignment (e. g. top left)
 *
 * @param           -
 * @return          current text position.
 * @exception       -
 * @see             GetPosition()
*/
CPoint CTextRect::GetTextPosition()
{
    EDrawAlignment eAlign = (EDrawAlignment) (m_eHorzTextAlignment * m_eVertTextAlignment);
    CPoint  tPointReturn = GetRectFixpoint(m_tRect, m_eHorzTextAlignment, m_eVertTextAlignment);
    switch(eAlign)
    {
    case eDA_left_top:
        tPointReturn.Offset(m_iTextOffset, m_iTextOffset);
        break;
    case eDA_left_mid:
        tPointReturn.Offset(m_iTextOffset, 0);
        break;
    case eDA_left_bottom:
        tPointReturn.Offset(m_iTextOffset, -m_iTextOffset);
        break;
    case eDA_center_top:
        tPointReturn.Offset(0, m_iTextOffset);
        break;
    case eDA_center_mid:
        break;
    case eDA_center_bottom:
        tPointReturn.Offset(0, -m_iTextOffset);
        break;
    case eDA_right_top:
        tPointReturn.Offset(-m_iTextOffset, m_iTextOffset);
        break;
    case eDA_right_mid:
        tPointReturn.Offset(-m_iTextOffset, 0);
        break;
    case eDA_right_bottom:
        tPointReturn.Offset(-m_iTextOffset, -m_iTextOffset);
        break;
    }
    return tPointReturn;
}

//------------------------------------------------------------------*
/**
 * draw.
 *
 * @param           pDC: device context.
 * @return          -
 * @exception       -
 * @see             -
*/
void CTextRect::Draw(CDC* pDC)
{
    // draw rect
    CDrawRect::Draw(pDC);

    // text drawing enabled?
    if(m_uiDrawFlags & DF_TEXT)
    {
        // text not empty?
        if(!m_strText.IsEmpty())
        {
            // draw text
            CFont*          pOldFont;
            CStringArray    astrLines;

            ASSERT(m_pFont);
            if(!m_pFont)
            {
                return;
            }

    		int nSavedDC = pDC->SaveDC();

		    CRgn rgn;
		    rgn.CreateRectRgnIndirect(&m_tRect);
		    pDC->SelectClipRgn(&rgn);

            pOldFont = pDC->SelectObject(m_pFont);

            int iNumLines = SplitLines(m_strText, astrLines);

            TEXTMETRIC  tm;
            pDC->GetTextMetrics(&tm);

            EDrawAlignment eAlign = (EDrawAlignment) (m_eHorzTextAlignment * m_eVertTextAlignment);
            CPoint  tStartPoint = GetTextPosition();
            UINT    uiOldAlign;
            UINT    uiNewAlign;
            int     iLineOffset;                        // offset for first line
            int     iLineHeight = tm.tmHeight;          // height of one line
            int     iLineSpacing = iLineHeight / 4;     // line spacing is 1/4 of line height

            switch(eAlign)
            {
            case eDA_left_top:
                {
                    uiNewAlign = TA_LEFT | TA_TOP;
                }
                break;
            case eDA_left_mid:
                {
                    iLineOffset = (iNumLines * iLineHeight + (iNumLines-1)*iLineSpacing)/ 2;
                    tStartPoint.Offset(0, -iLineOffset);
                    uiNewAlign = TA_LEFT | TA_TOP;
                }
                break;
            case eDA_left_bottom:
                {
                    iLineOffset = iNumLines * iLineHeight + (iNumLines-1)*iLineSpacing;
                    tStartPoint.Offset(0, -iLineOffset);
                    uiNewAlign = TA_LEFT | TA_TOP;
                }
                break;
            case eDA_center_top:
                {
                    uiNewAlign = TA_CENTER | TA_TOP;
                }
                break;
            case eDA_center_mid:
                {
                    iLineOffset = (iNumLines * iLineHeight + (iNumLines-1)*iLineSpacing)/ 2;
                    tStartPoint.Offset(0, -iLineOffset);
                    uiNewAlign = TA_CENTER | TA_TOP;
                }
                break;
            case eDA_center_bottom:
                {
                    iLineOffset = iNumLines * iLineHeight + (iNumLines-1)*iLineSpacing;
                    tStartPoint.Offset(0, -iLineOffset);
                    uiNewAlign = TA_CENTER | TA_TOP;
                }
                break;
            case eDA_right_top:
                {
                    uiNewAlign = TA_RIGHT | TA_TOP;
                }
                break;
            case eDA_right_mid:
                {
                    iLineOffset = (iNumLines * iLineHeight + (iNumLines-1)*iLineSpacing)/2;
                    tStartPoint.Offset(0, -iLineOffset);
                    uiNewAlign = TA_RIGHT | TA_TOP;
                }
                break;
            case eDA_right_bottom:
                {
                    iLineOffset = iNumLines * iLineHeight + (iNumLines-1)*iLineSpacing;
                    tStartPoint.Offset(0, -iLineOffset);
                    uiNewAlign = TA_RIGHT | TA_TOP;
                }
                break;
            }
            uiOldAlign = pDC->SetTextAlign(uiNewAlign);
            for(int iLine = 0; iLine < iNumLines; ++iLine)
            {
                pDC->TextOut(tStartPoint.x, tStartPoint.y, astrLines[iLine]);
                tStartPoint.Offset(0, iLineHeight + iLineSpacing);
            }
            pDC->SetTextAlign(uiOldAlign);
    		pDC->RestoreDC(nSavedDC);
	    	rgn.DeleteObject();
        }
    }
}


//------------------------------------------------------------------*
/**
 * split lines.
 *
 *  if multiline split lines at (carriage return +) line feed
 *
 * @param           crstrText: text to split
 * @param           rastrLines: string array of splitted lines
 * @return          number of lines
 * @exception       -
 * @see             -
*/
int CTextRect::SplitLines(const CString& crstrText, CStringArray& rastrLines)
{
    rastrLines.RemoveAll();
    if(!m_bMultiline)
    {
        rastrLines.Add(crstrText);
        return 1;
    }

    int iStart = 0;
    int iEnd;
    int iNumChar;
    CString strTmp;

    iEnd = crstrText.Find(_T('\n'));
    if(iEnd == -1)
    {
        rastrLines.Add(crstrText);
        return 1;
    }

    while(iEnd != -1)
    {
        if(iEnd > 0 && crstrText[iEnd-1] == _T('\r'))
        {
            iNumChar = iEnd - iStart - 1;
        }
        else
        {
            iNumChar = iEnd - iStart;
        }
        strTmp = crstrText.Mid(iStart, iEnd - iStart);
        rastrLines.Add(strTmp);
        iStart = iEnd + 1;
        iEnd = crstrText.Find(_T('\n'), iStart);
    }
    strTmp = crstrText.Mid(iStart);
    if(!strTmp.IsEmpty())
    {
        rastrLines.Add(strTmp);
    }

    return rastrLines.GetSize();
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------*
/**
 * bitmap rect constructor.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
CBitmapRect::CBitmapRect() :
  m_hImage(NULL),
  m_eDrawMode(eBM_normal)
{
}

//------------------------------------------------------------------*
/**
 * bitmap rect constructor with rect as parameter.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
CBitmapRect::CBitmapRect(const CRect& crtRect) :
  CDrawRect(crtRect),
  m_hImage(NULL),
  m_eDrawMode(eBM_normal)
{
}

//------------------------------------------------------------------*
/**
 * bitmap rect destructor.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
CBitmapRect::~CBitmapRect()
{
    if(m_hImage)
    {
        ::DeleteObject(m_hImage);
    }
}

//------------------------------------------------------------------*
/**
 * get bitmap path name.
 *
 * @param           -
 * @return          bitmap file name incl path.
 * @exception       -
 * @see             SetBitmapPathName()
*/
CString CBitmapRect::GetBitmapPathName()
{
    return m_strBitmapPathName;
}

//------------------------------------------------------------------*
/**
 * set bitmap path name.
 *
 * @param           crstrBitmapPathName: bitmap file name incl path.
 * @return          -
 * @exception       -
 * @see             GetBitmapPathName()
*/
void CBitmapRect::SetBitmapPathName(const CString& crstrBitmapPathName)
{
    m_strBitmapPathName = crstrBitmapPathName;
}


//------------------------------------------------------------------*
/**
 * get draw mode.
 *
 * @param           -
 * @return          draw mode.
 * @exception       -
 * @see             SetDrawMode()
*/
EBMDrawMode CBitmapRect::GetDrawMode()
{
    return m_eDrawMode;
}

//------------------------------------------------------------------*
/**
 * set draw mode.
 *
 * @param           eDrawMode: new draw mode
 * @return          -
 * @exception       -
 * @see             GetDrawMode()
*/
void CBitmapRect::SetDrawMode(EBMDrawMode eDrawMode)
{
    m_eDrawMode = eDrawMode;
}


//------------------------------------------------------------------*
/**
 * draw.
 *
 * @param           pDC: device context.
 * @return          -
 * @exception       -
 * @see             -
*/
void CBitmapRect::Draw(CDC* pDC)
{

    if(LoadImage())
    {
        CDC dcMem;
        dcMem.CreateCompatibleDC(pDC);

        CBitmap tBitmap;
        tBitmap.Attach(m_hImage);

        BITMAP bmpInfo;
        tBitmap.GetBitmap(&bmpInfo);

        CBitmap* pOldBitmap = dcMem.SelectObject(&tBitmap);

        int iX;
        int iY;

        switch(m_eDrawMode)
        {
        case eBM_normal:
            {
                iX = m_tRect.left + (m_tRect.Width() - bmpInfo.bmWidth) / 2;
                iY = m_tRect.top + (m_tRect.Height() - bmpInfo.bmHeight) / 2;
                pDC->BitBlt(iX, iY, bmpInfo.bmWidth, bmpInfo.bmHeight, &dcMem, 0, 0, SRCCOPY);
            }
            break;
        case eBM_fit:
            {
                CSize   tBmpSize(bmpInfo.bmWidth, bmpInfo.bmHeight);
                CSize   tRectSize = m_tRect.Size();

                double  dfX = (double) tRectSize.cx / tBmpSize.cx;
                double  dfY = (double) tRectSize.cy / tBmpSize.cy;
                double  dfFakt = min(dfX, dfY);

                tBmpSize.cx = (LONG)(double) tBmpSize.cx * dfFakt;
                tBmpSize.cy = (LONG)(double) tBmpSize.cy * dfFakt;

                iX = m_tRect.left + (m_tRect.Width() - tBmpSize.cx) / 2;
                iY = m_tRect.top + (m_tRect.Height() - tBmpSize.cy) / 2;

                pDC->StretchBlt(iX, iY, tBmpSize.cx, tBmpSize.cy, &dcMem, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, SRCCOPY);
            }
            break;
        case eBM_fill:
            {
                CSize   tBmpSize(bmpInfo.bmWidth, bmpInfo.bmHeight);
                CSize   tRectSize = m_tRect.Size();

                pDC->StretchBlt(m_tRect.left, m_tRect.top, tRectSize.cx, tRectSize.cy, &dcMem, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, SRCCOPY);
            }
            break;
        }

        dcMem.SelectObject(pOldBitmap);
    }

    CDrawRect::Draw(pDC);
}


//------------------------------------------------------------------*
/**
 * load image.
 *
 * @param           -
 * @return          successful?
 * @exception       -
 * @see             -
*/
BOOL CBitmapRect::LoadImage()
{
    if(m_hImage)
    {
        return TRUE;
    }

    if(m_strBitmapPathName.IsEmpty())
    {
        return FALSE;
    }

    m_hImage = (HBITMAP)::LoadImage(0, m_strBitmapPathName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

    if(!m_hImage)
    {
        return FALSE;
    }
    return TRUE;
}


/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------*
/**
 * draw grid constructor.
 *
 *  by default number of rows and columns are 0, line width for grid is 0.3 mm
 *  grid line color is black
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
CDrawGrid::CDrawGrid() :
  m_iNumberOfRows(0),
  m_iNumberOfColumns(0),
  m_iGridLineWidth(0),
  m_uiGridLineColor(0),
  m_piColumnWidth(NULL),
  m_piRowHeight(NULL),
  m_pFont(NULL),
  m_iTextOffset(0),
  m_eHorzTextAlignment(eHA_none),
  m_eVertTextAlignment(eVA_none)
{
}

//------------------------------------------------------------------*
/**
 * draw grid constructor with rect as parameter.
 *
 *  by default number of rows and columns are 0, line width for grid is 0.3 mm
 *  grid line color is black
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
CDrawGrid::CDrawGrid(const CRect& crtRect) :
  CDrawRect(crtRect),
  m_iNumberOfRows(0),
  m_iNumberOfColumns(0),
  m_iGridLineWidth(0),
  m_uiGridLineColor(0),
  m_piColumnWidth(NULL),
  m_piRowHeight(NULL),
  m_pFont(NULL),
  m_iTextOffset(0),
  m_eHorzTextAlignment(eHA_none),
  m_eVertTextAlignment(eVA_none)
{
}

//------------------------------------------------------------------*
/**
 * draw grid destructor.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
CDrawGrid::~CDrawGrid()
{
    FreeMemory();
}

//------------------------------------------------------------------*
/**
 * set rect.
 *
 *  overwritten method of CDrawRect.
 *  after setting the new rect the grid cell rects have to be calculated
 *  anew using CalculateLayout().
 *
 * @param           crtRect: input rect
 * @return          -
 * @exception       -
 * @see             CDrawRect::SetRect()
*/
void CDrawGrid::SetRect(const CRect& crtRect)
{
    CDrawRect::SetRect(crtRect);
    CalculateLayout();
}

//------------------------------------------------------------------*
/**
 * move.
 *
 *  overwritten method of CDrawRect.
 *  after setting the new rect the grid cell rects have to be calculated
 *  anew using CalculateLayout().
 *
 * @param           crtPoint: move vector as point
 * @return          -
 * @exception       -
 * @see             CDrawRect::Move()
*/
void CDrawGrid::Move(const CPoint& crtPoint)
{
    CDrawRect::Move(crtPoint);
    CalculateLayout();
}

//------------------------------------------------------------------*
/**
 * get font.
 *
 * @param           -
 * @return          font pointer
 * @exception       -
 * @see             SetFont()
*/
CFont* CDrawGrid::GetFont()
{
    return m_pFont;
}

//------------------------------------------------------------------*
/**
 * set font.
 *
 * @param           pFont: font pointer
 * @return          -
 * @exception       -
 * @see             GetFont()
*/
void CDrawGrid::SetFont(CFont* pFont)
{
    m_pFont = pFont;
}


//------------------------------------------------------------------*
/**
 * get text offset.
 *
 *  (text offset is used if not set in text rect)
 *
 * @param           -
 * @return          text offset.
 * @exception       -
 * @see             SetTextOffset()
*/
int CDrawGrid::GetTextOffset()
{
    return m_iTextOffset;
}

//------------------------------------------------------------------*
/**
 * set text offset.
 *
 *  text offset is not used with HA_center and VA_mid
 *
 * @param           iTextOffset: text offset from rect border in logical units
 * @return          -
 * @exception       -
 * @see             GetTextOffset()
*/
void CDrawGrid::SetTextOffset(int iTextOffset)
{
    m_iTextOffset = iTextOffset;
}

//------------------------------------------------------------------*
/**
 * get horizontal text alignment.
 *
 * @param           -
 * @return          horizontal text alignment.
 * @exception       -
 * @see             SetHorizontalTextAlignment()
*/
EHorizontalAlignment CDrawGrid::GetHorizontalTextAlignment()
{
    return m_eHorzTextAlignment;
}

//------------------------------------------------------------------*
/**
 * set horizontal text alignment.
 *
 * @param           eHorzTextAlignment: horizontal text alignment
 * @return          -
 * @exception       -
 * @see             GetHorizontalTextAlignment()
*/
void CDrawGrid::SetHorizontalTextAlignment(EHorizontalAlignment eHorzTextAlignment)
{
    m_eHorzTextAlignment = eHorzTextAlignment;
}


//------------------------------------------------------------------*
/**
 * get vertical text alignment.
 *
 * @param           -
 * @return          vertical text alignment.
 * @exception       -
 * @see             SetVerticalTextAlignment()
*/
EVerticalAlignment CDrawGrid::GetVerticalTextAlignment()
{
    return m_eVertTextAlignment;
}

//------------------------------------------------------------------*
/**
 * set vertical text alignment.
 *
 * @param           eVertTextAlignment: vertical text alignment
 * @return          -
 * @exception       -
 * @see             GetVerticalTextAlignment()
*/
void CDrawGrid::SetVerticalTextAlignment(EVerticalAlignment eVertTextAlignment)
{
    m_eVertTextAlignment = eVertTextAlignment;
}



//------------------------------------------------------------------*
/**
 * get grid line width.
 *
 * @param           -
 * @return          line width in units of 0.1 mm for drawing grid.
 * @exception       -
 * @see             SetGridLineWidth()
*/
int CDrawGrid::GetGridLineWidth()
{
    return m_iGridLineWidth;
}

//------------------------------------------------------------------*
/**
 * set grid line width.
 *
 * @param           iGridLineWidth: line width in units of 0.1 mm for drawing grid.
 * @return          -
 * @exception       -
 * @see             GetGridLineWidth()
*/
void CDrawGrid::SetGridLineWidth(int iGridLineWidth)
{
    m_iGridLineWidth = iGridLineWidth;
}

//------------------------------------------------------------------*
/**
 * get grid line color.
 *
 * @param           -
 * @return          grid line color.
 * @exception       -
 * @see             SetGridLineColor()
*/
UINT CDrawGrid::GetGridLineColor()
{
    return m_uiGridLineColor;
}

//------------------------------------------------------------------*
/**
 * set grid line color.
 *
 * @param           uiGridLineColor: grid line color.
 * @return          -
 * @exception       -
 * @see             GetGridLineColor()
*/
void CDrawGrid::SetGridLineColor(UINT uiGridLineColor)
{
    m_uiGridLineColor = uiGridLineColor;
}


//------------------------------------------------------------------*
/**
 * get grid size.
 *
 * @param           riNumColumns: number of columns
 * @param           riNumRows: number of rows
 * @return          -
 * @exception       -
 * @see             SetGridSize()
*/
void CDrawGrid::GetGridSize(int& riNumColumns, int& riNumRows)
{
    riNumColumns = m_iNumberOfColumns;
    riNumRows = m_iNumberOfRows;
}


//------------------------------------------------------------------*
/**
 * set grid size.
 *
 *  call this method only once at the beginning.
 *  old data is destroyed and new is allocated.
 *
 * @param           iNumColumns: number of columns
 * @param           iNumRows: number of rows
 * @return          -
 * @exception       -
 * @see             GetGridSize()
*/
void CDrawGrid::SetGridSize(int iNumColumns, int iNumRows)
{
    FreeMemory();

    m_iNumberOfColumns = iNumColumns;
    m_iNumberOfRows = iNumRows;

    AllocateMemory();
}

//------------------------------------------------------------------*
/**
 * free memory.
 *
 *  all draw rect objects are destroyed.
 *  all layout data is destroyed.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CDrawGrid::FreeMemory()
{
    int iNumDrawRects = m_iNumberOfColumns * m_iNumberOfRows;
    CDrawRect*  pDrawRect;
    for(int iRect = 0; iRect < iNumDrawRects; ++iRect)
    {
        pDrawRect = m_apDrawRects[iRect];
        if(pDrawRect)
        {
            delete pDrawRect;
        }
    }
    m_apDrawRects.RemoveAll();

    if(m_piColumnWidth)
    {
        delete [] m_piColumnWidth;
        m_piColumnWidth = NULL;
    }
    if(m_piRowHeight)
    {
        delete [] m_piRowHeight;
        m_piRowHeight = NULL;
    }
}


//------------------------------------------------------------------*
/**
 * allocate memory.
 *
 *  throws memory exception, if memory could not be allocated.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CDrawGrid::AllocateMemory()
{
    int iNumDrawRects = m_iNumberOfColumns * m_iNumberOfRows;
    ASSERT(iNumDrawRects > 0);
    if(iNumDrawRects > 0)
    {
        m_apDrawRects.SetSize(iNumDrawRects);
        for(int iRect = 0; iRect < iNumDrawRects; ++iRect)
        {
            m_apDrawRects[iRect] = NULL;
        }
    }
    ASSERT(m_piColumnWidth == NULL);
    ASSERT(m_piRowHeight == NULL);
    m_piColumnWidth = new int[m_iNumberOfColumns];
    m_piRowHeight = new int[m_iNumberOfRows];
    if(!m_piColumnWidth || !m_piRowHeight)
    {
        ::AfxThrowMemoryException();
    }
    Init();
}


//------------------------------------------------------------------*
/**
 * init.
 *
 *  init allocated members.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CDrawGrid::Init()
{
    int iNumDrawRects = m_apDrawRects.GetSize();
    for(int iRect = 0; iRect < iNumDrawRects; ++iRect)
    {
        m_apDrawRects[iRect] = NULL;
    }
    int iCol, iRow;
    for(iCol = 0; iCol < m_iNumberOfColumns; ++iCol)
    {
        m_piColumnWidth[iCol] = -1;
    }
    for(iRow = 0; iRow < m_iNumberOfRows; ++iRow)
    {
        m_piRowHeight[iRow] = -1;
    }
}


//------------------------------------------------------------------*
/**
 * get column width.
 *
 * @param           iColumn: index of column
 * @return          column width in units of 0.1 mm
 * @exception       -
 * @see             SetColumnWidth()
*/
int CDrawGrid::GetColumnWidth(int iColumn)
{
    if(iColumn >= m_iNumberOfColumns)
    {
        return -1;
    }
    ASSERT(m_piColumnWidth);
    return m_piColumnWidth[iColumn];
}


//------------------------------------------------------------------*
/**
 * set column width.
 *
 * @param           iColumn: index of column
 * @param           iWidth: column width in units of 0.1 mm
 * @return          successful?
 * @exception       -
 * @see             GetColumnWidth()
*/
BOOL CDrawGrid::SetColumnWidth(int iColumn, int iWidth)
{
    if(iColumn >= m_iNumberOfColumns)
    {
        return FALSE;
    }
    ASSERT(m_piColumnWidth);
    m_piColumnWidth[iColumn] = iWidth;
    return TRUE;
}

//------------------------------------------------------------------*
/**
 * get row height.
 *
 * @param           iRow: index of row
 * @return          row height in units of 0.1 mm
 * @exception       -
 * @see             SetRowHeight()
*/
int CDrawGrid::GetRowHeight(int iRow)
{
    if(iRow >= m_iNumberOfRows)
    {
        return -1;
    }
    ASSERT(m_piRowHeight);
    return m_piRowHeight[iRow];
}


//------------------------------------------------------------------*
/**
 * set row height.
 *
 * @param           iRow: index of row
 * @param           iHeight: row height in units of 0.1 mm
 * @return          successful?
 * @exception       -
 * @see             GetRowHeight()
*/
BOOL CDrawGrid::SetRowHeight(int iRow, int iHeight)
{
    if(iRow >= m_iNumberOfRows)
    {
        return FALSE;
    }
    ASSERT(m_piRowHeight);
    m_piRowHeight[iRow] = iHeight;
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * get draw rect.
 *
 * @param           iColumn: index of column
 * @param           iRow: index of row
 * @param           pDrawRect: pointer to draw rect object
 * @return          successful?
 * @exception       -
 * @see             SetDrawRect()
*/
BOOL CDrawGrid::GetDrawRect(int iColumn, int iRow, CDrawRect* pDrawRect)
{
    pDrawRect = NULL;

    if(iColumn > m_iNumberOfColumns || iColumn < 0)
    {
        return FALSE;
    }

    if(iRow > m_iNumberOfRows || iRow < 0)
    {
        return FALSE;
    }

    int iIndex = GetIndex(iColumn, iRow);
    int iSize = m_apDrawRects.GetSize();

    ASSERT(iIndex < iSize);

    pDrawRect = m_apDrawRects[iIndex];
    return TRUE;
}


//------------------------------------------------------------------*
/**
 * get draw rect.
 *
 * @param           iColumn: index of column
 * @param           iRow: index of row
 * @param           pDrawRect: pointer to draw rect object
 * @return          successful?
 * @exception       -
 * @see             SetDrawRect()
*/
BOOL CDrawGrid::SetDrawRect(int iColumn, int iRow, CDrawRect* pDrawRect)
{
    if(iColumn > m_iNumberOfColumns || iColumn < 0)
    {
        return FALSE;
    }

    if(iRow > m_iNumberOfRows || iRow < 0)
    {
        return FALSE;
    }

    int iIndex = GetIndex(iColumn, iRow);
    int iSize = m_apDrawRects.GetSize();

    ASSERT(iIndex < iSize);

    m_apDrawRects[iIndex] = pDrawRect;

    // dont allow child draw rect to draw its own frame:
    UINT    uiOldFlags = pDrawRect->GetDrawFlags();
    pDrawRect->SetDrawFlags(uiOldFlags & ~DF_FRAME);

    return TRUE;
}

//------------------------------------------------------------------*
/**
 * get index.
 *
 *  helper function to get index of object from column and row indices.
 *
 * @param           iColumn: index of column
 * @param           iRow: index of row
 * @return          index of draw rect object
 * @exception       -
 * @see             -
*/
int CDrawGrid::GetIndex(int iColumn, int iRow)
{
    return iRow * m_iNumberOfColumns + iColumn;
}


//------------------------------------------------------------------*
/**
 * reset.
 *
 *  calls FreeMemory() and resets grid size.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             FreeMemory()
*/
void CDrawGrid::Reset()
{
    FreeMemory();
    m_iNumberOfColumns = 0;
    m_iNumberOfRows = 0;
}


//------------------------------------------------------------------*
/**
 * draw.
 *
 * @param           pDC: device context.
 * @return          -
 * @exception       -
 * @see             -
*/
void CDrawGrid::Draw(CDC* pDC)
{
    DrawRects(pDC);
    DrawLines(pDC);

    CDrawRect::Draw(pDC);
}


//------------------------------------------------------------------*
/**
 * draw lines.
 *
 *  draw grid lines.
 *
 * @param           pDC: device context.
 * @return          -
 * @exception       -
 * @see             Draw()
*/
void CDrawGrid::DrawLines(CDC* pDC)
{
    if(m_uiDrawFlags & DF_GRID)
    {
        // get grid pen
        CPen    tPen;
        tPen.CreatePen(PS_SOLID, m_iGridLineWidth, m_uiGridLineColor);

        CPen*   pOldPen = pDC->SelectObject(&tPen);
        CPoint  tStartPoint;
        CPoint  tEndPoint;

        tStartPoint = m_tRect.TopLeft();
        tEndPoint.x   = m_tRect.left;
        tEndPoint.y   = m_tRect.bottom;
        int     iColumnWidth = m_tRect.Width() / m_iNumberOfColumns;
        int     iRowHeight = m_tRect.Height() / m_iNumberOfRows;

        for(int iCol = 0; iCol < m_iNumberOfColumns - 1; ++iCol)
        {
            if(m_piColumnWidth[iCol] != -1)
            {
                iColumnWidth = m_piColumnWidth[iCol];
            }
            tStartPoint.x += iColumnWidth;
            tEndPoint.x += iColumnWidth;
            pDC->MoveTo(tStartPoint);
            pDC->LineTo(tEndPoint);
        }

        tStartPoint = m_tRect.TopLeft();
        tEndPoint.x   = m_tRect.right;
        tEndPoint.y   = m_tRect.top;

        for(int iRow = 0; iRow < m_iNumberOfRows - 1; ++iRow)
        {
            if(m_piRowHeight[iRow] != -1)
            {
                iRowHeight = m_piRowHeight[iRow];
            }
            tStartPoint.y += iRowHeight;
            tEndPoint.y += iRowHeight;
            pDC->MoveTo(tStartPoint);
            pDC->LineTo(tEndPoint);
        }

        pDC->SelectObject(pOldPen);
    }
}


//------------------------------------------------------------------*
/**
 * draw rects.
 *
 *  draw rect objects.
 *
 * @param           pDC: device context.
 * @return          -
 * @exception       -
 * @see             Draw()
*/
void CDrawGrid::DrawRects(CDC* pDC)
{
    if(m_uiDrawFlags & DF_GRID_RECTS)
    {
        int         iNumDrawRects = m_apDrawRects.GetSize();
        CDrawRect*  pDrawRect;
    
        for(int iRect = 0; iRect < iNumDrawRects; ++iRect)
        {
            pDrawRect = m_apDrawRects[iRect];
            if(pDrawRect)
            {
                if(pDrawRect->GetDrawType() == eDT_text)
                {
                    CTextRect*  pTextRect = (CTextRect*) pDrawRect;
                    if(m_pFont)
                    {
                        pTextRect->SetFont(m_pFont);
                    }
                    if(m_iTextOffset != 0)
                    {
                        pTextRect->SetTextOffset(m_iTextOffset);
                    }
                    if(m_eHorzTextAlignment != eHA_none)
                    {
                        pTextRect->SetHorizontalTextAlignment(m_eHorzTextAlignment);
                    }
                    if(m_eVertTextAlignment != eVA_none)
                    {
                        pTextRect->SetVerticalTextAlignment(m_eVertTextAlignment);
                    }
                }   
                if(pDrawRect->GetDrawType() == eDT_grid)
                {
                    CDrawGrid*  pDrawGrid = (CDrawGrid*) pDrawRect;
                    if(m_pFont)
                    {
                        pDrawGrid->SetFont(m_pFont);
                    }
                    if(m_iTextOffset != 0)
                    {
                        pDrawGrid->SetTextOffset(m_iTextOffset);
                    }
                }   
                pDrawRect->Draw(pDC);
            }
        }
    }
}


//------------------------------------------------------------------*
/**
 * calculate layout.
 *
 *  calculate rects for draw rect objects according to the grid settings.
 *
 * @param           -
 * @return          -
 * @exception       -
 * @see             -
*/
void CDrawGrid::CalculateLayout()
{
    if(m_iNumberOfColumns == 0 || m_iNumberOfRows == 0)
    {
        return;
    }

    CPoint  tTopLeft;
    CPoint  tBottomRight;
    int     iCol;
    int     iRow;
    int     iColumnWidth = m_tRect.Width() / m_iNumberOfColumns;
    int     iRowHeight = m_tRect.Height() / m_iNumberOfRows;

    tTopLeft = m_tRect.TopLeft();
    tBottomRight = tTopLeft;

    for(iRow = 0; iRow < m_iNumberOfRows; ++iRow)
    {
        if(m_piRowHeight[iRow] != -1)
        {
            iRowHeight = m_piRowHeight[iRow];
        }

        // if last row or row is too hight to fit into rect -> take rect bottom
        if((iRow == m_iNumberOfRows - 1) || (tBottomRight.y >= m_tRect.bottom))
        {
            tBottomRight.y = m_tRect.bottom;
        }
        else
        {
            tBottomRight.y += iRowHeight;
        }
        for(iCol = 0; iCol < m_iNumberOfColumns; ++iCol)
        {
            if(m_piColumnWidth[iCol] != -1)
            {
                iColumnWidth = m_piColumnWidth[iCol];
            }
            if((iCol == m_iNumberOfColumns - 1) || (tBottomRight.x >= m_tRect.right))
            {
                tBottomRight.x = m_tRect.right;
            }
            else
            {
                tBottomRight.x += iColumnWidth;
            }
            SetGridCellRect(iCol, iRow, CRect(tTopLeft, tBottomRight));
            tTopLeft.x += iColumnWidth;
        }
        tTopLeft.y += iRowHeight;
        tTopLeft.x = m_tRect.left;
        tBottomRight.x = m_tRect.left;
        iColumnWidth = m_tRect.Width() / m_iNumberOfColumns;
    }
}


//------------------------------------------------------------------*
/**
 * set grid cell rect.
 *
 * @param           iCol: index of column
 * @param           iRow: index of row
 * @param           crtRect: input rect
 * @return          -
 * @exception       -
 * @see             -
*/
void CDrawGrid::SetGridCellRect(int iCol, int iRow, const CRect& crtRect)
{
    int iIndex = GetIndex(iCol, iRow);
    int iSize = m_apDrawRects.GetSize();
    ASSERT(iIndex < iSize);
    if(iIndex >= iSize)
    {
        return;
    }

    CDrawRect*  pDrawRect = m_apDrawRects[iIndex];
    if(pDrawRect)
    {
        pDrawRect->SetRect(crtRect);
    }
}

//------------------------------------------------------------------*
/**
 * get rect fixpoint.
 *
 * @param           crtRect: rect object.
 * @param           eHorzAlignment: horizontal alignment
 * @param           eVertAlignment: vertical alignment
 * @return          fixpoint in rect.
 * @exception       -
 * @see             -
*/
CPoint GetRectFixpoint(const CRect& crtRect, EHorizontalAlignment eHorzAlignment, EVerticalAlignment eVertAlignment)
{
    EDrawAlignment eAlign = (EDrawAlignment) (eHorzAlignment * eVertAlignment);
    CPoint  tPointReturn;
    switch(eAlign)
    {
    case eDA_left_top:
        tPointReturn = crtRect.TopLeft();
        break;
    case eDA_left_mid:
        {
            long lMid = (crtRect.bottom + crtRect.top) / 2;
            tPointReturn.x = crtRect.left;
            tPointReturn.y = lMid;
        }
        break;
    case eDA_left_bottom:
        tPointReturn.x = crtRect.left;
        tPointReturn.y = crtRect.bottom;
        break;
    case eDA_center_top:
        {
            long lCenter = (crtRect.left + crtRect.right) / 2;
            tPointReturn.x = lCenter;
            tPointReturn.y = crtRect.top;
        }
        break;
    case eDA_center_mid:
        {
            long lCenter = (crtRect.left + crtRect.right) / 2;
            long lMid = (crtRect.bottom + crtRect.top) / 2;
            tPointReturn.x = lCenter;
            tPointReturn.y = lMid;
        }
        break;
    case eDA_center_bottom:
        {
            long lCenter = (crtRect.left + crtRect.right) / 2;
            tPointReturn.x = lCenter;
            tPointReturn.y = crtRect.bottom;
        }
        break;
    case eDA_right_top:
        tPointReturn.x = crtRect.right;
        tPointReturn.y = crtRect.top;
        break;
    case eDA_right_mid:
        {
            long lMid = (crtRect.bottom + crtRect.top) / 2;
            tPointReturn.x = crtRect.right;
            tPointReturn.y = lMid;
        }
        break;
    case eDA_right_bottom:
        tPointReturn = crtRect.BottomRight();
        break;
    default:    // default is top left.
        tPointReturn = crtRect.TopLeft();
        break;
    }
    return tPointReturn;
}


//------------------------------------------------------------------*
/**
 * get fit size.
 *
 * @param           crtRectToFit: rect to be fitted.
 * @param           crtRectTemplate: rect into that is to be fitted.
 * @return          fitted size
 * @exception       -
 * @see             -
*/
CSize GetFitSize(const CRect& crtRectToFit, const CRect& crtRectTemplate)
{
    CSize   tRectToFitSize = crtRectToFit.Size();
    CSize   tRectTemplateSize = crtRectTemplate.Size();

    double  dfX = (double) tRectTemplateSize.cx / tRectToFitSize.cx;
    double  dfY = (double) tRectTemplateSize.cy / tRectToFitSize.cy;
    double  dfFakt = min(dfX, dfY);

    tRectToFitSize.cx = (LONG)(double) tRectToFitSize.cx * dfFakt;
    tRectToFitSize.cy = (LONG)(double) tRectToFitSize.cy * dfFakt;

    return tRectToFitSize;
}

