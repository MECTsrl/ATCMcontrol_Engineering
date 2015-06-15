#ifndef __DRAWOBJECT_H_
#define __DRAWOBJECT_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*
class CDrawObject;
class CDrawRect;
class CTextRect;
class CBitmapRect;
class CDrawGrid;

//----  Defines:    ------------------------------------------------*
typedef CArray<CDrawRect*, CDrawRect*> TDrawRectArray;

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

// draw flag definitions
#define DF_FRAME        0x1       // draw frame?
#define DF_RECT         0x2       // draw rect (fill rect)?
#define DF_TEXT         0x4       // draw texts?
#define DF_GRID         0x8       // draw grid?
#define DF_GRID_RECTS   0x10      // draw grid rects (contents of grid cells)?
#define DF_BITMAP       0x20      // draw bitmap?

typedef enum EDrawType
{
    eDT_rect,
    eDT_text,
    eDT_bitmap,
    eDT_grid,
};

// bitmap draw mode enumerator
typedef enum EBMDrawMode
{
    eBM_normal,     // bitmap is drawn in its original size
    eBM_fit,        // bitmap is scaled to fit in rect (isotropic scaling)
    eBM_fill        // bitmap is streched to fill whole rect
};

typedef enum EHorizontalAlignment
{
    eHA_none    = 0,
    eHA_left    = 1,
    eHA_center  = 2,
    eHA_right   = 3,
};

typedef enum EVerticalAlignment
{
    eVA_none    = 0,
    eVA_top     = -5,
    eVA_mid     = -7,
    eVA_bottom  = -11,
};

typedef enum EDrawAlignment
{
    eDA_left_top        = eHA_left * eVA_top,       // -5
    eDA_left_mid        = eHA_left * eVA_mid,       // -7
    eDA_left_bottom     = eHA_left * eVA_bottom,    // -11
    eDA_center_top      = eHA_center * eVA_top,     // -10,
    eDA_center_mid      = eHA_center * eVA_mid,     // -14,
    eDA_center_bottom   = eHA_center * eVA_bottom,  // -22,
    eDA_right_top       = eHA_right * eVA_top,      // -15,
    eDA_right_mid       = eHA_right * eVA_mid,      // -21,
    eDA_right_bottom    = eHA_right * eVA_bottom,   // -33
};

//------------------------------------------------------------------*
/**
 *  class CDrawObject
 *
 *  - virtual base class of drawing objects
 *  - each object is characterized by its type, position, size, alignment
 *  - each object can be positioned, moved and sized
 *  - each object has drawing flags for hiding some aspects
 *
 *  caution:
 *    for correct drawing the mapping mode has to be set to MM_LOMETRIC
 *    and y axis must be oriented top down.
 *
*/
class CDrawObject
{
public:
    CDrawObject();
    virtual ~CDrawObject();

    // type
    virtual EDrawType GetDrawType() = 0;

    // alignment
    EHorizontalAlignment GetHorizontalAlignment();
    void SetHorizontalAlignment(EHorizontalAlignment eHorzAlignment);
    EVerticalAlignment GetVerticalAlignment();
    void SetVerticalAlignment(EVerticalAlignment eVertAlignment);

    // position and size
    // position is not a member of the base class
    virtual CPoint GetPosition() = 0;
    virtual void SetPosition(const CPoint& crtPoint) = 0;
    virtual CSize GetSize() = 0;
    virtual void SetSize(const CSize& crtSize) = 0;
    virtual void Move(const CPoint& crtPoint) = 0;

    // drawing
    UINT GetDrawFlags();
    void SetDrawFlags(UINT uiDrawFlags);
    virtual void Draw(CDC* pDC) = 0;

protected:
    EHorizontalAlignment    m_eHorzAlignment;
    EVerticalAlignment      m_eVertAlignment;
    UINT                    m_uiDrawFlags;

};



//------------------------------------------------------------------*
/**
 *  class CDrawRect
 *
 *  - class for drawing a rectangle
 *  - derived from CDrawObject
 *  - attributes: line width and color
 *
 *  @see    CDrawObject
*/
class CDrawRect : public CDrawObject
{
public:
    CDrawRect();
    CDrawRect(const CRect& crtRect);
    virtual ~CDrawRect();

    CRect GetRect();
    virtual void SetRect(const CRect& crtRect);

    int GetLineWidth();
    void SetLineWidth(int iLineWidth);

    UINT GetLineColor();
    void SetLineColor(UINT uiLineColor);

    void Align(const CRect& crtRect);
    void Fit(const CRect& crtRect);

    virtual CPoint GetPosition();
    virtual void SetPosition(const CPoint& crtPoint);

    virtual CSize GetSize();
    virtual void SetSize(const CSize& crtSize);

    virtual void Move(const CPoint& crtPoint);

    virtual EDrawType GetDrawType() { return eDT_rect;};
    virtual void Draw(CDC* pDC);

protected:
//    void DrawFrame(CDC* pDC);
//    void DrawRect(CDC* pDC);

protected:
    CRect   m_tRect;        // draw rect in logical units
    int     m_iLineWidth;   // line width in logical units
    UINT    m_uiLineColor;  // line color
    //UINT    m_uiRectColor;  // rect color
};





//------------------------------------------------------------------*
/**
 *  class CTextRect
 *
 *  - class for drawing a text within a rectangle
 *  - derived from CDrawRect
 *  - attributes: text, font, multiline flag, text alignment, text offset
 *
 *  @see    CDrawRect
*/
class CTextRect : public CDrawRect
{
public:
    CTextRect();
    CTextRect(const CRect& crtRect, const CString& crstrText = _T(""));
    virtual ~CTextRect();

    CString GetText();
    void SetText(const CString& crstrText);

    CFont* GetFont();
    void SetFont(CFont* pFont);

    int GetTextOffset();
    void SetTextOffset(int iTextOffset);

    BOOL GetMultilineFlag();
    void SetMultilineFlag(BOOL bMultiline);

    EHorizontalAlignment GetHorizontalTextAlignment();
    void SetHorizontalTextAlignment(EHorizontalAlignment eHorzTextAlignment);

    EVerticalAlignment GetVerticalTextAlignment();
    void SetVerticalTextAlignment(EVerticalAlignment eVertTextAlignment);

    CPoint GetTextPosition();

    virtual EDrawType GetDrawType() { return eDT_text; };
    virtual void Draw(CDC* pDC);

protected:
    int SplitLines(const CString& crstrText, CStringArray& rastrLines);

protected:
    CFont*                  m_pFont;
    BOOL                    m_bMultiline;
    CString                 m_strText;
    EHorizontalAlignment    m_eHorzTextAlignment;
    EVerticalAlignment      m_eVertTextAlignment;
    int                     m_iTextOffset;          // distance from rect frame
//    UINT    m_uiTextColor;
};


//------------------------------------------------------------------*
/**
 *  class CBitmapRect
 *
 *  - class for drawing a bitmap within a rectangle
 *  - derived from CDrawRect
 *  - attributes: bitmap file path name
 *
 *  @see    CDrawRect
*/
class CBitmapRect : public CDrawRect
{
public:
    CBitmapRect();
    CBitmapRect(const CRect& crtRect);
    virtual ~CBitmapRect();

    CString GetBitmapPathName();
    void SetBitmapPathName(const CString& crstrBitmapPathName);

    EBMDrawMode GetDrawMode();
    void SetDrawMode(EBMDrawMode eDrawMode);

    virtual EDrawType GetDrawType() { return eDT_bitmap;};
    virtual void Draw(CDC* pDC);

protected:
    BOOL LoadImage();

protected:
    CString     m_strBitmapPathName;
    HBITMAP     m_hImage;
    EBMDrawMode m_eDrawMode;
};



//------------------------------------------------------------------*
/**
 *  class CDrawGrid
 *
 *  - class for drawing a rectangular grid containing draw rect objects
 *    or objects derived from draw rect
 *
 *  Usage:
 *  1. set number of columns and number of rows
 *  2. specify column widths and row heights
 *  3. create draw rect objects on the heap, initialize them (without rect data)
 *     and set them into the grid cells using SetDrawRect()
 *  4. call SetRect()
 *  
 *  In step 5 the rect positions of all rect objects in the grid are calculated
 *  using the CalculateLayout method.
 *
 *  Whenever column width or heights are changed, CalculateLayout has to be called.
 *
 *  number of columns and rows can only be set once at the beginning. Later calls 
 *  to SetSize() deletes all draw rect objects.
 *
 *  @see    CDrawRect
*/
class CDrawGrid : public CDrawRect
{
public:
    CDrawGrid();
    CDrawGrid(const CRect& crtRect);
    virtual ~CDrawGrid();

    virtual void SetRect(const CRect& crtRect);
    virtual void Move(const CPoint& crtPoint);

    CFont* GetFont();
    void SetFont(CFont* pFont);

    int GetTextOffset();
    void SetTextOffset(int iTextOffset);

    int GetGridLineWidth();
    void SetGridLineWidth(int iGridLineWidth);

    UINT GetGridLineColor();
    void SetGridLineColor(UINT uiGridLineColor);

    void GetGridSize(int& riNumColumns, int& riNumRows);
    void SetGridSize(int iNumColumns, int iNumRows);

    int GetColumnWidth(int iColumn);
    BOOL SetColumnWidth(int iColumn, int iWidth);

    int GetRowHeight(int iRow);
    BOOL SetRowHeight(int iRow, int iHeight);

    BOOL GetDrawRect(int iColumn, int iRow, CDrawRect* pDrawRect);
    BOOL SetDrawRect(int iColumn, int iRow, CDrawRect* pDrawRect);

    EHorizontalAlignment GetHorizontalTextAlignment();
    void SetHorizontalTextAlignment(EHorizontalAlignment eHorzTextAlignment);

    EVerticalAlignment GetVerticalTextAlignment();
    void SetVerticalTextAlignment(EVerticalAlignment eVertTextAlignment);

    void CalculateLayout();

    void Reset();

    virtual EDrawType GetDrawType() { return eDT_grid;};
    virtual void Draw(CDC* pDC);

protected:
    void FreeMemory();
    void AllocateMemory();
    void Init();
    int GetIndex(int iColumn, int iRow);
    void CalculateFont();
    void DrawLines(CDC* pDC);
    void DrawRects(CDC* pDC);
    void SetGridCellRect(int iCol, int iRow, const CRect& crtRect);

protected:
    int                     m_iGridLineWidth;
    UINT                    m_uiGridLineColor;
    int                     m_iNumberOfRows;
    int                     m_iNumberOfColumns;
    TDrawRectArray          m_apDrawRects;          // array of CDrawRect*
    int*                    m_piColumnWidth;
    int*                    m_piRowHeight;
    CFont*                  m_pFont;                // overrides font set in text rect
    int                     m_iTextOffset;          // overrides text offset in text rect
    EHorizontalAlignment    m_eHorzTextAlignment;   // overrides alignment in text rect
    EVerticalAlignment      m_eVertTextAlignment;   // overrides alignment in text rect
};


CPoint GetRectFixpoint(const CRect& crtRect, EHorizontalAlignment eHorzAlignment, EVerticalAlignment eVertAlignment);
CSize GetFitSize(const CRect& crtRectToFit, const CRect& crtRectTemplate);


#endif // __DRAWOBJECT_H_

