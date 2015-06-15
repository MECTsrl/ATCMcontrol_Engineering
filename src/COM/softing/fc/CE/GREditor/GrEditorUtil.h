#if !defined(AFX_CEFBDUTIL_H__INCLUDED_)
#define AFX_CEFBDUTIL_H__INCLUDED_

/*doc*=======================================================================*/
/*inc*=======================================================================*/

#include "GrResource.h"

/*const*=====================================================================*/

#define BM_LAST      (IDB_GR_ICONVARFB - IDB_GR_ICONCMT + 1)

/*macro*=====================================================================*/
/*type*======================================================================*/
/*glovar*====================================================================*/

extern BOOL m_bDebug2;

/*proto*=====================================================================*/

class CGDIResources;

class GDIHelper
{
   friend CGDIResources;   // helper for init

public:
   // the user interface :

   static CPalette    *RealizePalette  (CDC *pDC, bool bScreen);
   static void         UnrealizePalette(CDC *pDC, CPalette *pOldPal);

   static HCURSOR      GetCursor(              HCURSOR_TYPE CursorType);
   static CBrush      *GetBrush (bool bScreen, BRUSH_TYPE   BrushType );
   static CPen        *GetPen   (bool bScreen, PEN_TYPE     PenType   );
   static COLORREF     GetColor (bool bScreen, COLOR_TYPE   ColorType );
   static void         SetColor (bool bScreen, COLOR_TYPE   ColorType, COLORREF ulCol);

   static LPBITMAPINFO GetBmInfo(bool bScreen, UINT nIDBitmap, bool bCommented, bool bSFCmanual);

private:
   // private data not for using outside this class ! 

   static HCURSOR       hCursor[HC_LAST];
   static COLORREF      ulColor [2][CO_LAST];   // colors [0][]    - printer
                                                //        [1][]    - screen
   static CBrush       *pBrush  [2][BT_LAST];   // brushs [0][]    - printer
                                                //        [1][]    - screen
   static CPen         *pPen    [2][PE_LAST];   // pens   [0][]    - printer
                                                //        [1][]    - screen
   static LPBITMAPINFO  lpBmInfo[2][4*BM_LAST]; // bitmaps[0][]    - printer
                                                //        [1][]    - screen
   //  [][2n]   - uncommented              CO_ELEMBK     CO_NORMAL
   //  [][2n+1] - commented                CO_ELEMBK     CO_COMMENT
   //  [][2n+2] - uncommented sfc manual   CO_BACKGD_SFC CO_NORMAL
   //  [][2n+3] - commented   sfc manual   CO_BACKGD_SFC CO_COMMENT

   static CPalette     *pColPal[2];             // GR color palette
   static bool          bPalRealized[2];        // palette is realized

   static void         CreateResources(void);
   static void         DeleteResources(void);

   static void         CreatePalette(void);
   static void         DeletePalette(void);

   static LPBITMAPINFO CreateBmInfo(bool bScreen, UINT nIDBitmap, 
                                    bool bCommented, bool bSFCmanual);
   static void         DeleteBmInfo(LPBITMAPINFO & lpBmI);
};

class CGRTool
{
public:
   // detection of direction (orientation) between two points
   enum DIR_TYPE
   {
      DT_NS=0,         // direction north -> south
      DT_EW,           //           east  -> west
      DT_SN,           //           south -> north
      DT_WE,           //           west  -> east
      DT_NO,           // no direction (points are identical, ..)

      DT_LAST
   };

   static DIR_TYPE GetDir(CPoint ps, CPoint pe);

   // enumeration of elements, names, ..
   enum ID_TYPE
   {
      ID_PIN=0,        // ident for pins
      ID_VAR,          // ident for variable,constants in/out
      ID_LAB,          // ident for labels/jumps
      ID_CON,          // ident for connectors
      ID_STEP,         // ident for steps
      ID_TRAN,         // ident for transitions
      ID_ACT,          // ident for actions/actionblocks
      ID_INST,         // ident for FB instances
      ID_CONTACT,      // ident for contacts
      ID_COIL,         // ident for coils

      ID_LAST
   };

   static void     ResetIDs ();
   static void     SetID    (ID_TYPE eID, long lID);
   static long     GetNextID(ID_TYPE eID);
private:
   // private data not for using outside this class ! 
   static long lUtilLastID[ID_LAST];
};

#endif // !defined(AFX_CEFBDUTIL_H__INCLUDED_)
