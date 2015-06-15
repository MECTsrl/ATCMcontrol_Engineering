

/*doc*=======================================================================*/
/*inc*=======================================================================*/

#include "stdafx.h"

#include "GrEditorUtil.h"

/*const*=====================================================================*/

#define COL_PRN_WHITE      RGB(255,255,255)
#define COL_PRN_LIGHT      RGB(230,230,230)
#define COL_PRN_DARK       RGB(180,180,180)
#define COL_PRN_BLACK      RGB(  0,  0,  0)

/*macro*=====================================================================*/
/*glovar*====================================================================*/

// show connect ID's and position
BOOL   m_bDebug2=FALSE;

/*type*======================================================================*/

class CGDIResources
{
public:
   CGDIResources () { GDIHelper::CreateResources(); }
   ~CGDIResources() { GDIHelper::DeleteResources(); }
};

/*locvar*====================================================================*/

CGDIResources TheResources;

HCURSOR GDIHelper::hCursor[HC_LAST] = 
{
   ::LoadCursor(::GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_GR_INSCOL )),  // HC_INSCOL
   ::LoadCursor(::GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_GR_INSCOL1)),  // HC_INSCOL1 
   ::LoadCursor(::GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_GR_DELCOL )),  // HC_DELCOL  
   ::LoadCursor(::GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_GR_DELCOL1)),  // HC_DELCOL1 
   ::LoadCursor(::GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_GR_INSROW )),  // HC_INSROW  
   ::LoadCursor(::GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_GR_INSROW1)),  // HC_INSROW1 
   ::LoadCursor(::GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_GR_DELROW )),  // HC_DELROW  
   ::LoadCursor(::GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_GR_DELROW1)),  // HC_DELROW1 
   ::LoadCursor(::GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_GR_PENCUR )),  // HC_PENCUR  
   ::LoadCursor(::GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_GR_PENCUR1)),  // HC_PENCUR1  
   ::LoadCursor(                  NULL , MAKEINTRESOURCE(IDC_SIZEALL   )),  // HC_SIZEALL 
   ::LoadCursor(                  NULL , MAKEINTRESOURCE(IDC_ARROW     )),  // HC_ARROW   
   ::LoadCursor(                  NULL , MAKEINTRESOURCE(IDC_SIZEWE    )),  // HC_SIZEWE  
   ::LoadCursor(                  NULL , MAKEINTRESOURCE(IDC_SIZENS    )),  // HC_SIZENS  
   ::LoadCursor(                  NULL , MAKEINTRESOURCE(IDC_SIZENWSE  )),  // HC_SIZENWSE
   ::LoadCursor(                  NULL , MAKEINTRESOURCE(IDC_SIZENESW  )),  // HC_SIZENESW
   ::LoadCursor(                  NULL , MAKEINTRESOURCE(IDC_CROSS     ))   // HC_CROSS   
};

COLORREF GDIHelper::ulColor[2][CO_LAST] = 
{
   {  // printer
      COL_PRN_BLACK,  // CO_NORMAL    
      COL_PRN_BLACK,  // CO_SELECT    
      COL_PRN_BLACK,  // CO_ERROR     
      COL_PRN_LIGHT,  // CO_COMMENT   
      COL_PRN_DARK ,  // CO_RUBBER    
      COL_PRN_BLACK,  // CO_EDITLINE  
      COL_PRN_LIGHT,  // CO_GRID      
      COL_PRN_WHITE,  // CO_BACKGD    
      COL_PRN_DARK ,  // CO_ELEMBK    
      COL_PRN_LIGHT,  // CO_FBLKBK    
      COL_PRN_LIGHT,  // CO_LADDBK    
      COL_PRN_DARK ,  // CO_EDGE      
      COL_PRN_DARK ,  // CO_LIGHTPIN
      COL_PRN_DARK ,  // CO_BRKREC    
      COL_PRN_DARK ,  // CO_BRKDIS    
      COL_PRN_DARK ,  // CO_BRKACT    
      COL_PRN_LIGHT,  // CO_BRKNOC    
      COL_UNUSED   ,  // CO_UNUSED    
      COL_PRN_WHITE,  // CO_BACKGD_SFC
      COL_PRN_DARK ,  // CO_ELEMBK_SFC
      COL_PRN_DARK ,  // CO_LADDBK_SFC
      COL_PRN_LIGHT,  // CO_FBLKBK_SFC
      COL_PRN_DARK ,  // CO_ACTIVE_SFC
      COL_PRN_BLACK,  // CO_FORCED_SFC
      COL_PRN_BLACK   // CO_BLOCKD_SFC
   },
   {  // screen
      RGB(  0,  0,  0),  // CO_NORMAL    
      RGB(  0,128,255),  // CO_SELECT    
      RGB(255,  0,  0),  // CO_ERROR     
      RGB(200,200,200),  // CO_COMMENT   
      RGB(  0,255,255),  // CO_RUBBER    
      RGB(150, 50,200),  // CO_EDITLINE  
      RGB(127,127,127),  // CO_GRID      
      RGB(255,255,200),  // CO_BACKGD    
      RGB(230,230,175),  // CO_ELEMBK    
      RGB(255,255,230),  // CO_FBLKBK    
      RGB(100,200,100),  // CO_LADDBK    
      RGB( 32, 32,196),  // CO_EDGE      
      RGB(255,  0,  0),  // CO_LIGHTPIN
      RGB(255,255,  0),  // CO_BRKREC    
      RGB(255,255,255),  // CO_BRKDIS    
      RGB(255,  0,  0),  // CO_BRKACT    
      RGB(255,160,160),  // CO_BRKNOC    
      COL_UNUSED      ,  // CO_UNUSED    
      RGB(230,242,255),  // CO_BACKGD_SFC
      RGB(195,215,255),  // CO_ELEMBK_SFC
      RGB(100,200,100),  // CO_LADDBK_SFC
      RGB(245,250,255),  // CO_FBLKBK_SFC
      RGB(200,200,200),  // CO_ACTIVE_SFC
      RGB(  0,128,  0),  // CO_FORCED_SFC
      RGB(255,  0,  0)   // CO_BLOCKD_SFC
   }
};

CBrush       *GDIHelper::pBrush [2][BT_LAST];   // brushs [0][] - printer
                                                //        [1][] - screen
CPen         *GDIHelper::pPen   [2][PE_LAST];   // pens   [0][] - printer
                                                //        [1][] - screen
LPBITMAPINFO GDIHelper::lpBmInfo[2][4*BM_LAST]; // bitmaps[0][] - printer
                                                //        [1][] - screen
//  [][2n]   - uncommented              CO_ELEMBK     CO_NORMAL
//  [][2n+1] - commented                CO_ELEMBK     CO_COMMENT
//  [][2n+2] - uncommented sfc manual   CO_BACKGD_SFC CO_NORMAL
//  [][2n+3] - commented   sfc manual   CO_BACKGD_SFC CO_COMMENT

CPalette     *GDIHelper::pColPal[2];            // GR color palette
bool     GDIHelper::bPalRealized[2];            // palette is realized

long CGRTool::lUtilLastID[ID_LAST];

/*proto*=====================================================================*/
/*func*======================================================================*/

/*func*------------------------------------------------------------------------
  allocate and create used GDI resources of the GR 
      pens and brushs:  for screen and printer
      bitmaps        :  uncommented and commented
  in :-
  out:-
-----------------------------------------------------------------------------*/
void GDIHelper::CreateResources(void)
{
   int i, j;

   CreatePalette();

   for( i=0; i<=1; i++ )
   {
      for( j=0; j<BT_LAST; j++ ) pBrush[i][j] = new CBrush;
      for( j=0; j<PE_LAST; j++ ) pPen  [i][j] = new CPen;
      for( j=0; j<BM_LAST; j++ )
      {
         lpBmInfo[i][4*j  ] = CreateBmInfo((i==0 ? false : true), j, false, false);
         lpBmInfo[i][4*j+1] = CreateBmInfo((i==0 ? false : true), j, true , false);
         lpBmInfo[i][4*j+2] = CreateBmInfo((i==0 ? false : true), j, false, true );
         lpBmInfo[i][4*j+3] = CreateBmInfo((i==0 ? false : true), j, true , true );
      }
   }
   // printer
   pBrush[0][BT_RUBBER    ]->CreateSolidBrush(PALETTEINDEX(CO_RUBBER    ));
   pBrush[0][BT_BACKGD    ]->CreateSolidBrush(PALETTEINDEX(CO_BACKGD    ));
   pBrush[0][BT_SELECT    ]->CreateSolidBrush(PALETTEINDEX(CO_SELECT    ));
   pBrush[0][BT_ELEMBK    ]->CreateSolidBrush(PALETTEINDEX(CO_ELEMBK    ));
   pBrush[0][BT_FBLKBK    ]->CreateSolidBrush(PALETTEINDEX(CO_FBLKBK    ));
   pBrush[0][BT_LADDBK    ]->CreateSolidBrush(PALETTEINDEX(CO_LADDBK    ));
   pBrush[0][BT_LIGHTPIN  ]->CreateSolidBrush(PALETTEINDEX(CO_LIGHTPIN  ));
   pBrush[0][BT_BRKREC    ]->CreateSolidBrush(PALETTEINDEX(CO_BRKREC    ));
   pBrush[0][BT_BRKDIS    ]->CreateSolidBrush(PALETTEINDEX(CO_BRKDIS    ));
   pBrush[0][BT_BRKACT    ]->CreateSolidBrush(PALETTEINDEX(CO_BRKACT    ));
   pBrush[0][BT_BRKNOC    ]->CreateSolidBrush(PALETTEINDEX(CO_BRKNOC    ));
   pBrush[0][BT_BACKGD_SFC]->CreateSolidBrush(PALETTEINDEX(CO_BACKGD_SFC));
   pBrush[0][BT_ELEMBK_SFC]->CreateSolidBrush(PALETTEINDEX(CO_ELEMBK_SFC));
   pBrush[0][BT_LADDBK_SFC]->CreateSolidBrush(PALETTEINDEX(CO_LADDBK_SFC));
   pBrush[0][BT_FBLKBK_SFC]->CreateSolidBrush(PALETTEINDEX(CO_FBLKBK_SFC));
   pBrush[0][BT_ACTIVE_SFC]->CreateSolidBrush(PALETTEINDEX(CO_ACTIVE_SFC));
   // screen
   pBrush[1][BT_RUBBER    ]->CreateSolidBrush(PALETTEINDEX(CO_RUBBER    ));
   pBrush[1][BT_BACKGD    ]->CreateSolidBrush(PALETTEINDEX(CO_BACKGD    ));
   pBrush[1][BT_SELECT    ]->CreateSolidBrush(PALETTEINDEX(CO_SELECT    ));
   pBrush[1][BT_ELEMBK    ]->CreateSolidBrush(PALETTEINDEX(CO_ELEMBK    ));
   pBrush[1][BT_FBLKBK    ]->CreateSolidBrush(PALETTEINDEX(CO_FBLKBK    ));
   pBrush[1][BT_LADDBK    ]->CreateSolidBrush(PALETTEINDEX(CO_LADDBK    ));
   pBrush[1][BT_LIGHTPIN  ]->CreateSolidBrush(PALETTEINDEX(CO_LIGHTPIN  ));
   pBrush[1][BT_BRKREC    ]->CreateSolidBrush(PALETTEINDEX(CO_BRKREC    ));
   pBrush[1][BT_BRKDIS    ]->CreateSolidBrush(PALETTEINDEX(CO_BRKDIS    ));
   pBrush[1][BT_BRKACT    ]->CreateSolidBrush(PALETTEINDEX(CO_BRKACT    ));
   pBrush[1][BT_BRKNOC    ]->CreateSolidBrush(PALETTEINDEX(CO_BRKNOC    ));
   pBrush[1][BT_BACKGD_SFC]->CreateSolidBrush(PALETTEINDEX(CO_BACKGD_SFC));
   pBrush[1][BT_ELEMBK_SFC]->CreateSolidBrush(PALETTEINDEX(CO_ELEMBK_SFC));
   pBrush[1][BT_LADDBK_SFC]->CreateSolidBrush(PALETTEINDEX(CO_LADDBK_SFC));
   pBrush[1][BT_FBLKBK_SFC]->CreateSolidBrush(PALETTEINDEX(CO_FBLKBK_SFC));
   pBrush[1][BT_ACTIVE_SFC]->CreateSolidBrush(PALETTEINDEX(CO_ACTIVE_SFC));

   // printer
   pPen[0][PE_NORMAL  ]->CreatePen(PS_SOLID, 1, PALETTEINDEX(CO_NORMAL    ));
   pPen[0][PE_SELECT  ]->CreatePen(PS_DOT,   1, PALETTEINDEX(CO_SELECT    ));
   pPen[0][PE_ERROR   ]->CreatePen(PS_SOLID, 1, PALETTEINDEX(CO_ERROR     ));
   pPen[0][PE_COMMENT ]->CreatePen(PS_DOT,   0, PALETTEINDEX(CO_COMMENT   ));
   pPen[0][PE_LABEL   ]->CreatePen(PS_DOT,   0, PALETTEINDEX(CO_NORMAL    ));
   pPen[0][PE_GRID    ]->CreatePen(PS_SOLID, 1, PALETTEINDEX(CO_GRID      ));
   pPen[0][PE_PAGE    ]->CreatePen(PS_SOLID, 1, PALETTEINDEX(CO_EDGE      ));
   pPen[0][PE_SHEET   ]->CreatePen(PS_DOT,   1, PALETTEINDEX(CO_EDGE      ));
   pPen[0][PE_RUBBER  ]->CreatePen(PS_SOLID, 1, PALETTEINDEX(CO_RUBBER    ));
   pPen[0][PE_EDITLINE]->CreatePen(PS_SOLID, 1, PALETTEINDEX(CO_EDITLINE  ));
   pPen[0][PE_FORCED  ]->CreatePen(PS_SOLID, 1, PALETTEINDEX(CO_FORCED_SFC));
   pPen[0][PE_BLOCKED ]->CreatePen(PS_SOLID, 1, PALETTEINDEX(CO_BLOCKD_SFC));
   // screen
   pPen[1][PE_NORMAL  ]->CreatePen(PS_SOLID, 1, PALETTEINDEX(CO_NORMAL    ));
   pPen[1][PE_SELECT  ]->CreatePen(PS_DOT,   1, PALETTEINDEX(CO_SELECT    ));
   pPen[1][PE_ERROR   ]->CreatePen(PS_SOLID, 1, PALETTEINDEX(CO_ERROR     ));
   pPen[1][PE_COMMENT ]->CreatePen(PS_SOLID, 1, PALETTEINDEX(CO_COMMENT   ));
   pPen[1][PE_LABEL   ]->CreatePen(PS_DOT,   1, PALETTEINDEX(CO_NORMAL    ));
   pPen[1][PE_GRID    ]->CreatePen(PS_SOLID, 1, PALETTEINDEX(CO_GRID      ));
   pPen[1][PE_PAGE    ]->CreatePen(PS_SOLID, 1, PALETTEINDEX(CO_EDGE      ));
   pPen[1][PE_SHEET   ]->CreatePen(PS_DOT,   1, PALETTEINDEX(CO_EDGE      ));
   pPen[1][PE_RUBBER  ]->CreatePen(PS_SOLID, 1, PALETTEINDEX(CO_RUBBER    ));
   pPen[1][PE_EDITLINE]->CreatePen(PS_SOLID, 1, PALETTEINDEX(CO_EDITLINE  ));
   pPen[1][PE_FORCED  ]->CreatePen(PS_SOLID, 1, PALETTEINDEX(CO_FORCED_SFC));
   pPen[1][PE_BLOCKED ]->CreatePen(PS_SOLID, 1, PALETTEINDEX(CO_BLOCKD_SFC));
}

/*func*------------------------------------------------------------------------
  free and deallocate used GDI resources of the GR (pens and brushs) for screen
  and printer
  in :-
  out:-
-----------------------------------------------------------------------------*/
void GDIHelper::DeleteResources(void)
{
   int i, j;

   for( i=0; i<=1; i++ )
   {
      for( j=0; j<BT_LAST; j++ ) 
      {
         pBrush[i][j]->DeleteObject();
         if( pBrush[i][j] ) 
         {
            delete pBrush[i][j];
            pBrush[i][j] = NULL;
         }
      }
      for( j=0; j<PE_LAST; j++ ) 
      {
         pPen  [i][j]->DeleteObject();
         if( pPen  [i][j] )
         {
            delete pPen  [i][j];
            pPen  [i][j] = NULL;
         }
      }
      for( j=0; j<BM_LAST; j++ ) 
      {
         DeleteBmInfo(lpBmInfo[i][4*j  ]);
         DeleteBmInfo(lpBmInfo[i][4*j+1]);
         DeleteBmInfo(lpBmInfo[i][4*j+2]);
         DeleteBmInfo(lpBmInfo[i][4*j+3]);
      }
   }

   DeletePalette();
}

/*func*------------------------------------------------------------------------
  create the color palette with the in GR used colors
  (bitmaps and elements for screen and printer)
  in :-
  out:-
-----------------------------------------------------------------------------*/
void GDIHelper::CreatePalette(void)
{
   LPBITMAPINFO   lpbi;
   int            i, j, k, iColBm, iCol;
   HANDLE         hPal;
   LPLOGPALETTE   lpPal;
   HRSRC          hRsrc;
   HGLOBAL        hGlobal;
   
   if( ((hRsrc = FindResource(::GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_GR_ICONVARFB), RT_BITMAP)) != NULL) &&
      ((hGlobal = LoadResource(::GetModuleHandle(NULL), hRsrc)) != NULL) )
   {
      if( (lpbi = (LPBITMAPINFO)LockResource(hGlobal)) == NULL )
      {
         UnlockResource(hGlobal);
         ASSERT(FALSE);
         return;
      }
      
      if (lpbi->bmiHeader.biBitCount <= 8)
      {
         iColBm = (1 << lpbi->bmiHeader.biBitCount);
      } else {
         iColBm = 0;  // No palette needed for 24 BPP DIB
      }
      
      if (lpbi->bmiHeader.biClrUsed > 0)
      {
         iColBm = lpbi->bmiHeader.biClrUsed;  // Use biClrUsed
      }

      iCol = iColBm + CO_LAST;
      if (iCol)
      {
         hPal = GlobalAlloc (GHND, sizeof (LOGPALETTE) +
            sizeof (PALETTEENTRY) * (iCol));
         if( hPal != NULL )
         {
            lpPal = (LPLOGPALETTE) GlobalLock (hPal);
            if (lpPal != NULL )
            {
               for( k=0; k<=1; k++ )
               {
                  // load colors of the GR elements for printer [0] and screen [1]
                  for( i=0; i<CO_LAST; i++ )
                  {
                     lpPal->palPalEntry[i].peRed   = GetRValue(ulColor[k][i]);
                     lpPal->palPalEntry[i].peGreen = GetGValue(ulColor[k][i]);
                     lpPal->palPalEntry[i].peBlue  = GetBValue(ulColor[k][i]);
                     lpPal->palPalEntry[i].peFlags = 0;
                  }
                  // load colors of the bitmaps for printer [0] and screen [1]
                  for( j=0; i<iCol; i++, j++ )
                  {
                     lpPal->palPalEntry[i].peRed   = lpbi->bmiColors[j].rgbRed;
                     lpPal->palPalEntry[i].peGreen = lpbi->bmiColors[j].rgbGreen;
                     lpPal->palPalEntry[i].peBlue  = lpbi->bmiColors[j].rgbBlue;
                     lpPal->palPalEntry[i].peFlags = 0;
                  }
                  lpPal->palVersion    = 0x300;
                  lpPal->palNumEntries = (short)i;
                  
                  pColPal[k] = new CPalette;
                  bPalRealized[k] = false;
                  ASSERT(pColPal[k]);
                  if( pColPal[k] ) pColPal[k]->CreatePalette(lpPal);
               }
               
               GlobalUnlock (hPal);
            }
            GlobalFree (hPal);
         }
      }
      UnlockResource (hGlobal);
      FreeResource (hGlobal);
   }
}

/*func*------------------------------------------------------------------------
  delete the locked memory of the logical color palette
  in :-
  out:-
-----------------------------------------------------------------------------*/
void GDIHelper::DeletePalette(void)
{
   int i;
   
   for( i=0; i<=1; i++ )
   {
      if( pColPal[i] && (bPalRealized[i] == false) )
      {
         pColPal[i]->DeleteObject();
         delete pColPal[i];
         pColPal[i] = NULL;
      }
   }
}

/*func*------------------------------------------------------------------------
  realize the GR color palette for screen or printer
  in :dc          - device context
      bScreen     - false: realize palette for printer
                    true : realize palette for screen
  out:pointer to previous palette
-----------------------------------------------------------------------------*/
CPalette *GDIHelper::RealizePalette(CDC *pDC, bool bScreen)
{
   CPalette *pOldPal=NULL;
   int i = bScreen ? 1 : 0;

   ASSERT(pDC);
   ASSERT(pColPal[i]);

   if( pDC && pColPal[i] )
   {
      pOldPal = pDC->SelectPalette(pColPal[i], FALSE);
      pDC->RealizePalette();
      bPalRealized[i] = true;
   }

   return pOldPal;
}

/*func*------------------------------------------------------------------------
  select the previous color palette
  in :dc          - device context
      pOldPal     - old color palette
  out:-
-----------------------------------------------------------------------------*/
void GDIHelper::UnrealizePalette(CDC *pDC, CPalette *pOldPal)
{
   ASSERT(pDC);
   ASSERT(pOldPal);
   if( pDC && pOldPal)
   {
      pDC->SelectPalette(pOldPal, FALSE);
      if( pOldPal != pColPal[0] ) bPalRealized[0] = false;
      if( pOldPal != pColPal[1] ) bPalRealized[1] = false;
   }
}

/*func*------------------------------------------------------------------------
  set color for screen or printer, handle the GDI resources
  in :bScreen     - false: set color of printer element
                    true : set color of screen element
      ColorType   - desired element (COLOR_TYPE)
      ulCol       - color value
  out:-
-----------------------------------------------------------------------------*/
void GDIHelper::SetColor(bool bScreen, COLOR_TYPE ColorType, COLORREF ulCol)
{
   bool bChanged;
   
   ASSERT((ColorType >= 0) || (ColorType < CO_LAST));
   
   if (bScreen) 
   {
      bChanged = ulColor[1][ColorType] != ulCol;
      ulColor[1][ColorType] = ulCol;
   } else {
      bChanged = ulColor[0][ColorType] != ulCol;
      ulColor[0][ColorType] = ulCol;
   }

   if (bChanged)
   {
      DeleteResources();
      CreateResources();
   }
}

/*func*------------------------------------------------------------------------
  get color for screen or printer
  in :bScreen     - false: get color of printer element
                    true : get color of screen element
      ColorType   - desired color (COLOR_TYPE)
  out:color
-----------------------------------------------------------------------------*/
COLORREF GDIHelper::GetColor(bool bScreen, COLOR_TYPE ColorType)
{
   ASSERT((ColorType >= 0) || (ColorType < CO_LAST));
   
   return ( bScreen ) ? ulColor[1][ColorType] : ulColor[0][ColorType];
}

/*func*------------------------------------------------------------------------
  get cursor
  in :CursorType   - desired cursor (HCURSOR_TYPE)
  out:cursor handle
-----------------------------------------------------------------------------*/
HCURSOR GDIHelper::GetCursor(HCURSOR_TYPE CursorType)
{
   ASSERT((CursorType >= 0) || (CursorType < HC_LAST));

   return hCursor[CursorType];
}

/*func*------------------------------------------------------------------------
  get brush for screen or printer
  in :bScreen     - false: get brush of printer element
                    true : get brush of screen element
      BrushType   - desired brush (BRUSH_TYPE)
  out:pointer to brush
-----------------------------------------------------------------------------*/
CBrush *GDIHelper::GetBrush(bool bScreen, BRUSH_TYPE BrushType)
{
   ASSERT((BrushType >= 0) || (BrushType < BT_LAST));
   
   return ( bScreen ) ? pBrush[1][BrushType] : pBrush[0][BrushType];
}

/*func*------------------------------------------------------------------------
  get pen for screen or printer
  in :bScreen  - false: get pen of printer element
                 true : get pen of screen element
      PenType  - desired pen (PEN_TYPE)
  out:pointer to pen
-----------------------------------------------------------------------------*/
CPen *GDIHelper::GetPen(bool bScreen, PEN_TYPE PenType)
{
   ASSERT((PenType >= 0) || (PenType < PE_LAST));
   
   return ( bScreen ) ? pPen[1][PenType] : pPen[0][PenType];
}

/*func*------------------------------------------------------------------------
  create the given bitmap from the GR resources
  (for uncommented and commented case)
  in :bScreen     - false: get bitmap of printer element
                    true : get bitmap of screen element
      nIDBitmap   - desired bitmap
      bCommented  - false: bitmap is uncommented
                    true : Bitmap is commented
  out:pointer to the bitmap info
-----------------------------------------------------------------------------*/
LPBITMAPINFO GDIHelper::CreateBmInfo(bool bScreen, UINT nIDBitmap, 
                                     bool bCommented, bool bSFCmanual)
{
   HRSRC    hRsrc;    
   HGLOBAL  hGlobal;    
   LPBITMAPINFOHEADER  lpbih, lpbiht;
   COLORMAP cm[2];
   int      i, iNumColors;
   RGBQUAD *pRGBQuad;

   hRsrc   = FindResource(::GetModuleHandle(NULL),
                          MAKEINTRESOURCE(nIDBitmap + IDB_GR_ICONCMT),
                          RT_BITMAP);
   ASSERT(hRsrc);
   hGlobal = LoadResource(::GetModuleHandle(NULL), hRsrc);
   ASSERT(hGlobal);
   if( (hRsrc== NULL) || (hGlobal == NULL) ) 
   {
      return NULL;
   }

   lpbih = (LPBITMAPINFOHEADER)LockResource(hGlobal);
   if( lpbih == NULL )
   {
      FreeResource(hGlobal);
      return NULL;
   }

   if( lpbih->biBitCount <= 8 ) iNumColors = (1 << lpbih->biBitCount);
   else                         iNumColors = 0;  // No palette needed for 24 BPP DIB
   if( lpbih->biClrUsed > 0 )   iNumColors = lpbih->biClrUsed;  // Use biClrUsed

                                 // 16 colors fix!
   if( iNumColors != 16 )
   {
      UnlockResource(hGlobal);
      FreeResource(hGlobal);
      return NULL;
   }

   // alloc memory for header, color map and color values
   i = lpbih->biSize +
       iNumColors * sizeof(RGBQUAD) +
       (lpbih->biWidth * lpbih->biHeight * lpbih->biBitCount) / 8;
   lpbiht = (LPBITMAPINFOHEADER)::malloc(i);
   if( lpbiht == NULL )
   {
      UnlockResource(hGlobal);
      FreeResource(hGlobal);
      return NULL;
   }

   memcpy(lpbiht, lpbih, i);
   pRGBQuad = (RGBQUAD*) ((LPBYTE)lpbiht + (UINT)lpbih->biSize);

   cm[0].from = COL_UNUSED; 
   cm[0].to   = GetColor(bScreen, (bSFCmanual ? CO_ELEMBK_SFC : CO_ELEMBK));
   cm[1].from = GetColor(bScreen, CO_NORMAL);
   cm[1].to   = GetColor(bScreen, (bCommented ? CO_COMMENT : CO_NORMAL));

   for( i=0; i<iNumColors; i++ )
   {
      if( RGB( pRGBQuad[i].rgbRed, pRGBQuad[i].rgbGreen, pRGBQuad[i].rgbBlue) == cm[0].from )
      {
         pRGBQuad[i].rgbRed   = GetRValue(cm[0].to);
         pRGBQuad[i].rgbGreen = GetGValue(cm[0].to); 
         pRGBQuad[i].rgbBlue  = GetBValue(cm[0].to);
      } 
      else if( bCommented &&
               RGB( pRGBQuad[i].rgbRed, pRGBQuad[i].rgbGreen, pRGBQuad[i].rgbBlue) == cm[1].from )
      {
         pRGBQuad[i].rgbRed   = GetRValue(cm[1].to);
         pRGBQuad[i].rgbGreen = GetGValue(cm[1].to); 
         pRGBQuad[i].rgbBlue  = GetBValue(cm[1].to);
      }
   }

   UnlockResource(hGlobal);
   FreeResource(hGlobal);

   return (LPBITMAPINFO)lpbiht;
}

/*func*------------------------------------------------------------------------
  delete the locked memory of the in the GR used bitmaps
  (for screen and printer)
  in :-
  out:-
-----------------------------------------------------------------------------*/
void GDIHelper::DeleteBmInfo(LPBITMAPINFO & lpBmI)
{
   ASSERT(lpBmI);

   if(lpBmI) 
   {
      ::free(lpBmI);
      lpBmI = NULL;
   }
}

/*func*------------------------------------------------------------------------
  get brush for screen or printer
  in :bScreen     - false: get brush of printer element
                    true : get brush of screen element
      nIDBitmap   - desired bitmap
      bCommented  - false: bitmap foreground is uncommented
                    true : bitmap foreground is commented
      bSFCmanual  - false: element background 
                    true : SFC manual background 
  out:pointer to bitmapinfo
-----------------------------------------------------------------------------*/
LPBITMAPINFO GDIHelper::GetBmInfo(bool bScreen, UINT nIDBitmap, bool bCommented, 
                                  bool bSFCmanual)
{
   LPBITMAPINFO lpBmI=NULL;

   ASSERT((nIDBitmap >= IDB_GR_ICONCMT) && (nIDBitmap <= IDB_GR_ICONVARFB));

   if( (nIDBitmap >= IDB_GR_ICONCMT) && (nIDBitmap <= IDB_GR_ICONVARFB) )
   {
      nIDBitmap -= IDB_GR_ICONCMT;
      lpBmI = lpBmInfo[bScreen ? 1 : 0][4 * nIDBitmap + 
                                        (bSFCmanual ? 2 : 0) + 
                                        (bCommented ? 1 : 0)];
   }

   return lpBmI;
}

/*func*------------------------------------------------------------------------
  get the direction between two points
  in :ps     - start point
      pe     - end point
  out:       - direction type
-----------------------------------------------------------------------------*/
//         ps:DT_NS
//            |
//            v
// ps:DT_WE ->pe<- ps:DT_EW
//            ^
//            |
//         ps:DT_SN

CGRTool::DIR_TYPE CGRTool::GetDir(CPoint ps, CPoint pe)
{
   if( ps.x==pe.x ) 
   {
      if (ps.y==pe.y) return DT_NO;
      else 
      {
         if (ps.y<pe.y) return DT_NS; 
         else           return DT_SN;
      }
   } else {
      if (ps.x<pe.x) return DT_WE; else return DT_EW;
   }
}

/*func*------------------------------------------------------------------------
  get next ID for connect point
  in :eID - type of ident
  out:next ident
-----------------------------------------------------------------------------*/
long CGRTool::GetNextID(ID_TYPE eID)
{
   lUtilLastID[eID]++;
   return lUtilLastID[eID];
}

/*func*------------------------------------------------------------------------
  set ID for connect points (for instance after loading document)
  ID will only be set, if new ID is higher!
  in :eID - type of ident
      lID - new ident
  out:-
-----------------------------------------------------------------------------*/
void CGRTool::SetID(ID_TYPE eID, long lID)
{
   if( lUtilLastID[eID] < lID)
   {
      lUtilLastID[eID] = lID;
   }
}

/*func*------------------------------------------------------------------------
  clear the ID values (before loading new document)
  in :-
  out:-
-----------------------------------------------------------------------------*/
void CGRTool::ResetIDs()
{
   ID_TYPE eID;
   
   for( eID=ID_PIN; eID < ID_LAST; eID = (ID_TYPE)((int)eID + 1) )
   {
      lUtilLastID[eID] = 0;
   }
}

