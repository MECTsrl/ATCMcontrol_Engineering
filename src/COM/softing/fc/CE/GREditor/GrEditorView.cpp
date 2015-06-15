

/*doc*=======================================================================*/
/*inc*=======================================================================*/
#include "stdafx.h"

#include "GrEditor.h"
#include "GrEditorDoc.h"
#include "GrEditorView.h"
#include "GrEditorUtil.h"
#include "Elem.h"
#include "ElemSfc.h"
#include "MainFrm.h"
#include "PropEdit.h"
#include "InsFbDlg.h"
#include "ZoomDlg.h"
#include "CEGRError.h"
#include "CEBPManError.h"
#include "Watchpoint.h"

#include "utilif.h"
#include "dragitem.h"
#include "cedb.h"
#include "SECEdit.h"

#include "BookMark.h"
#include "iostream"


using namespace std;

/*const*=====================================================================*/
/*macro*=====================================================================*/
/*glovar*====================================================================*/
/*type*======================================================================*/
/*locvar*====================================================================*/

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*proto*=====================================================================*/
/*func*======================================================================*/

// private clipboard format (list of Draw objects)
CLIPFORMAT CGrEditorView::m_uiClipFormatId  = (CLIPFORMAT)::RegisterClipboardFormat (CB_FORMAT_4CLIB);
CLIPFORMAT CGrEditorView::m_uiCBFormatXMLId = (CLIPFORMAT)::RegisterClipboardFormat (CB_FORMAT_4CXML);

//IMPLEMENT_SERIAL(CDragItem, CObject,CB_FORMAT_4CLIB_VERSION)
//IMPLEMENT_SERIAL(CXMLDragItem, CObject,CB_FORMAT_4CXML_VERSION)

static const UINT nMsgFindReplace = ::RegisterWindowMessage(FINDMSGSTRING);

/////////////////////////////////////////////////////////////////////////////
// CGrEditorView

IMPLEMENT_DYNCREATE(CGrEditorView, CDbgEditorView)

BEGIN_MESSAGE_MAP(CGrEditorView, CDbgEditorView)
   //{{AFX_MSG_MAP(CGrEditorView)
   ON_COMMAND(ID_GR_CANCEL_EDIT_SRVR, OnCancelEditSrvr)
	ON_WM_CREATE()
   ON_WM_MOUSEWHEEL()
   ON_WM_HSCROLL()
   ON_WM_VSCROLL()
   ON_WM_SIZE()
   ON_WM_ERASEBKGND()
   ON_WM_MOUSEMOVE()
   ON_WM_LBUTTONDOWN()
   ON_WM_LBUTTONUP()
   ON_WM_LBUTTONDBLCLK()
   ON_WM_RBUTTONDOWN()
   ON_WM_KEYDOWN()
   ON_WM_KEYUP()
   ON_WM_CHAR()
   ON_WM_SETCURSOR()
   ON_WM_CAPTURECHANGED()
   ON_WM_TIMER()
   ON_COMMAND(ID_GR_INS_FB,   OnInsFb)
   ON_COMMAND(ID_GR_INS_VARIN, OnInsVarIn)
   ON_COMMAND(ID_GR_INS_VAROUT, OnInsVarOut)
   ON_COMMAND(ID_GR_INS_VARINOUT, OnInsVarinout)
   ON_COMMAND(ID_GR_INS_LAB,  OnInsLab)
   ON_COMMAND(ID_GR_INS_JMP,  OnInsJmp)
   ON_COMMAND(ID_GR_INS_CONIN,  OnInsConIn)
   ON_COMMAND(ID_GR_INS_CONOUT,  OnInsConOut)
   ON_COMMAND(ID_GR_INS_CMT,  OnInsCmt)
   ON_COMMAND(ID_GR_INS_HLINE, OnInsHLine)
   ON_COMMAND(ID_GR_INS_VLINE, OnInsVLine)
   ON_COMMAND(ID_GR_INS_RET,  OnInsRet)
   ON_COMMAND(ID_GR_INS_STEP,       OnInsStep)
   ON_COMMAND(ID_GR_INS_TRANSITION, OnInsTransition)
   ON_COMMAND(ID_GR_INS_ACTION,     OnInsAction)
   ON_COMMAND(ID_GR_INS_EXCEPTION_ACTION,     OnInsExceptionAction)
   ON_COMMAND(ID_GR_INS_ACTIONBLOCK,OnInsActionBlock)
   ON_COMMAND(ID_GR_INS_DIVCON,     OnInsDivCon)
   ON_COMMAND(ID_GR_INS_SDIVCON,    OnInsSDivCon)
   ON_COMMAND(ID_GR_INS_FB_CALL, OnInsertFBCall)
   ON_COMMAND(ID_GR_INS_POWER,       OnInsPower)
   ON_COMMAND(ID_GR_INS_CT_NORMAL,   OnInsCtNormal)
   ON_COMMAND(ID_GR_INS_CT_CLOSED,   OnInsCtClosed)
   ON_COMMAND(ID_GR_INS_CT_POSTRANS, OnInsCtPosTrans)
   ON_COMMAND(ID_GR_INS_CT_NEGTRANS, OnInsCtNegTrans)
   ON_COMMAND(ID_GR_INS_CL_NORMAL,   OnInsClNormal)
   ON_COMMAND(ID_GR_INS_CL_NEGATIVE, OnInsClNegative)
   ON_COMMAND(ID_GR_INS_CL_SET,      OnInsClSet)
   ON_COMMAND(ID_GR_INS_CL_RESET,    OnInsClReset)
   ON_COMMAND(ID_GR_INS_CL_POSTRANS, OnInsClPosTrans)
   ON_COMMAND(ID_GR_INS_CL_NEGTRANS, OnInsClNegTrans)
   ON_COMMAND(ID_GR_EDIT_RESIZE,  OnEditResize)
   ON_COMMAND(ID_EDIT_FIND,    OnEditFind)
   ON_COMMAND(ID_EDIT_REPEAT,  OnEditRepeat)
   ON_COMMAND(ID_EDIT_REPLACE, OnEditReplace)
   ON_COMMAND(ID_GR_EDIT_COL,  OnEditCol)
   ON_COMMAND(ID_GR_EDIT_ROW,  OnEditRow)
   ON_COMMAND(ID_GR_EDIT_COMMENT, OnEditComment)
   ON_COMMAND(ID_GR_EDIT_UNCOMMENT, OnEditUncomment)
   ON_COMMAND(ID_GR_EDIT_LINE, OnEditLine)
   ON_COMMAND(ID_EDIT_COPY,  OnEditCopy)
   ON_COMMAND(ID_EDIT_CUT,   OnEditCut)
   ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
   ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
   ON_COMMAND(ID_EDIT_UNDO,  OnEditUndo)
   ON_COMMAND(ID_EDIT_REDO,  OnEditRedo)
   ON_COMMAND(ID_GR_EDIT_NAME, OnEditName)
   ON_COMMAND(ID_GR_EDIT_PROPERTY, OnEditProperty)
   ON_COMMAND(ID_GR_EDIT_PROP_FBDACTION,OnSetPropFBDAction)
   ON_COMMAND(ID_GR_EDIT_PROP_LDACTION, OnSetPropLDAction )
   ON_COMMAND(ID_GR_EDIT_PROP_STACTION, OnSetPropSTAction )
   ON_COMMAND(ID_GR_EDIT_INSTAMCE, OnEditInstance)
   ON_COMMAND(ID_GR_EDIT_AUTOSIZE, OnEditAutoSize)
   ON_COMMAND(ID_GR_SEL_ALL, OnSelAll)
   ON_COMMAND(ID_GR_EN_PROPEDIT, OnEndPropEdit)
   ON_COMMAND(ID_GR_OPT_SFC, OnOptSfc)
   ON_COMMAND(ID_GR_OPT_STDSIZE, OnOptStdSize)
   ON_COMMAND(ID_GR_OPT_CHANGEWARN, OnOptChangeWarn)
   ON_COMMAND(ID_GR_STEP_TOGGLEINIT, OnStepToggleinit)
   ON_COMMAND(ID_GR_DEBUG, OnDebug)
   ON_COMMAND(ID_GR_DEBUG2, OnDebug2)
   ON_UPDATE_COMMAND_UI(ID_GR_OPT_CHANGEWARN, OnUpdateOptChangeWarn)
   ON_UPDATE_COMMAND_UI(ID_EDIT_REPLACE, OnUpdateEditReplace)
   ON_UPDATE_COMMAND_UI(ID_EDIT_REPEAT, OnUpdateNeedFind)
   ON_UPDATE_COMMAND_UI(ID_GR_EDIT_LINE, OnUpdateEditLine)
   ON_UPDATE_COMMAND_UI(ID_GR_EDIT_COL, OnUpdateEditcol)
   ON_UPDATE_COMMAND_UI(ID_GR_EDIT_ROW, OnUpdateEditrow)
   ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
   ON_UPDATE_COMMAND_UI(ID_EDIT_CUT,   OnUpdateEditCut)
   ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
   ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR,  OnUpdateEditClear)
   ON_UPDATE_COMMAND_UI(ID_GR_EDIT_RESIZE, OnUpdateEdit)
   ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
   ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
   ON_UPDATE_COMMAND_UI(ID_GR_EDIT_INSTAMCE, OnUpdateEditInstamce)
   ON_UPDATE_COMMAND_UI(ID_GR_INS_POWER, OnUpdateEditorLD)
   ON_UPDATE_COMMAND_UI(ID_GR_INS_STEP, OnUpdateEditorSFC)
   ON_UPDATE_COMMAND_UI(ID_GR_OPT_SFC, OnUpdateOptSfc)
   ON_UPDATE_COMMAND_UI(ID_GR_OPT_STDSIZE, OnUpdateOptStdSize)
   ON_UPDATE_COMMAND_UI(ID_GR_DEBUG, OnUpdateDebug1)
   ON_UPDATE_COMMAND_UI(ID_GR_DEBUG2, OnUpdateDebug2)
   ON_UPDATE_COMMAND_UI(ID_GR_VIEW_ZOOM, OnUpdateZoom)
   ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateEdit)
   ON_UPDATE_COMMAND_UI(ID_GR_INS_EXCEPTION_ACTION, OnUpdateEdit)
   ON_COMMAND(ID_GR_VIEW_ZOOM, OnZoom)
   ON_COMMAND(ID_GR_VIEW_ZOOMIN, OnZoomin)
   ON_COMMAND(ID_GR_VIEW_ZOOMOUT, OnZoomout)
   ON_COMMAND(ID_GR_ROUTE, OnGrRoute)
   ON_UPDATE_COMMAND_UI(ID_GR_ROUTE, OnUpdateGrRoute)
   ON_COMMAND(ID_GR_UNROUTE, OnGrUnroute)
   ON_UPDATE_COMMAND_UI(ID_GR_UNROUTE, OnUpdateGrUnroute)
   ON_WM_CONTEXTMENU()
   ON_WM_RBUTTONUP()
   ON_COMMAND(ID_GR_CONTEXT_MENU, OnContextMenuKey)
   ON_UPDATE_COMMAND_UI(ID_GR_VIEW_ZOOMIN, OnUpdateZoom)
   ON_UPDATE_COMMAND_UI(ID_GR_VIEW_ZOOMOUT, OnUpdateZoom)
   ON_UPDATE_COMMAND_UI(ID_EDIT_FIND, OnUpdateNeedText)
   ON_UPDATE_COMMAND_UI(ID_GR_EDIT_UNCOMMENT, OnUpdateEdit)
   ON_UPDATE_COMMAND_UI(ID_GR_EDIT_COMMENT, OnUpdateEdit)
   ON_UPDATE_COMMAND_UI(ID_GR_EDIT_NAME, OnUpdateEdit)
   ON_UPDATE_COMMAND_UI(ID_GR_EDIT_PROP_FBDACTION,OnUpdateEdit)
   ON_UPDATE_COMMAND_UI(ID_GR_EDIT_PROP_LDACTION, OnUpdateEdit)
   ON_UPDATE_COMMAND_UI(ID_GR_EDIT_PROP_STACTION, OnUpdateEdit)
   ON_UPDATE_COMMAND_UI(ID_GR_EDIT_PROPERTY, OnUpdateEdit)
   ON_UPDATE_COMMAND_UI(ID_GR_EDIT_INSTAMCE, OnUpdateEMNormal)
   ON_UPDATE_COMMAND_UI(ID_GR_SEL_ALL, OnUpdateEMNormal)
   ON_UPDATE_COMMAND_UI(ID_GR_INS_FB_CALL, OnUpdateEdit)
   ON_UPDATE_COMMAND_UI(ID_GR_INS_FB,      OnUpdateEdit)
   ON_UPDATE_COMMAND_UI(ID_GR_INS_VARIN,   OnUpdateEdit)
   ON_UPDATE_COMMAND_UI(ID_GR_INS_VAROUT,  OnUpdateEdit)
   ON_UPDATE_COMMAND_UI(ID_GR_INS_VARINOUT, OnUpdateEditorVarInOut)
   ON_UPDATE_COMMAND_UI(ID_GR_INS_LAB, OnUpdateEdit)
   ON_UPDATE_COMMAND_UI(ID_GR_INS_JMP, OnUpdateEdit)
   ON_UPDATE_COMMAND_UI(ID_GR_INS_RET, OnUpdateEdit)
   ON_UPDATE_COMMAND_UI(ID_GR_INS_CONIN, OnUpdateEdit)
   ON_UPDATE_COMMAND_UI(ID_GR_INS_CONOUT, OnUpdateEdit)
   ON_UPDATE_COMMAND_UI(ID_GR_INS_CMT, OnUpdateEdit)
   ON_UPDATE_COMMAND_UI(ID_GR_INS_HLINE, OnUpdateEdit)
   ON_UPDATE_COMMAND_UI(ID_GR_INS_VLINE, OnUpdateEdit)
   ON_UPDATE_COMMAND_UI(ID_GR_INS_TRANSITION, OnUpdateEditorSFC)
   ON_UPDATE_COMMAND_UI(ID_GR_INS_ACTION, OnUpdateEditorSFC)
   ON_UPDATE_COMMAND_UI(ID_GR_INS_ACTIONBLOCK, OnUpdateEditorSFC)
   ON_UPDATE_COMMAND_UI(ID_GR_INS_DIVCON, OnUpdateEditorSFC)
   ON_UPDATE_COMMAND_UI(ID_GR_INS_SDIVCON, OnUpdateEditorSFC)
   ON_UPDATE_COMMAND_UI(ID_GR_INS_CT_NORMAL, OnUpdateEditorLD)
   ON_UPDATE_COMMAND_UI(ID_GR_INS_CT_CLOSED, OnUpdateEditorLD)
   ON_UPDATE_COMMAND_UI(ID_GR_INS_CT_POSTRANS, OnUpdateEditorLD)
   ON_UPDATE_COMMAND_UI(ID_GR_INS_CT_NEGTRANS, OnUpdateEditorLD)
   ON_UPDATE_COMMAND_UI(ID_GR_INS_CL_NORMAL  , OnUpdateEditorLD)
   ON_UPDATE_COMMAND_UI(ID_GR_INS_CL_NEGATIVE, OnUpdateEditorLD)
   ON_UPDATE_COMMAND_UI(ID_GR_INS_CL_SET     , OnUpdateEditorLD)
   ON_UPDATE_COMMAND_UI(ID_GR_INS_CL_RESET   , OnUpdateEditorLD)
   ON_UPDATE_COMMAND_UI(ID_GR_INS_CL_POSTRANS, OnUpdateEditorLD)
   ON_UPDATE_COMMAND_UI(ID_GR_INS_CL_NEGTRANS, OnUpdateEditorLD)
	ON_WM_SYSKEYDOWN()
	ON_WM_SYSKEYUP()
	//}}AFX_MSG_MAP
   ON_COMMAND(ID_BOOKMARKSET,OnToggleBookmark)
   ON_COMMAND(ID_BOOKMARKNEXT,OnBookmarkNext)
   ON_COMMAND(ID_BOOKMARKPREV,OnBookmarkPrev)
   ON_COMMAND(ID_BOOKMARKDELETE,OnClearBookmarks)

   ON_COMMAND_RANGE(ID_GR_EDIT_ACT_NONE, ID_GR_EDIT_ACT_P1, OnEditActQual)
   ON_MESSAGE( WM_USER, OnEditAutoSize2 )
   // Special registered message for Find and Replace
   ON_REGISTERED_MESSAGE(nMsgFindReplace, OnFindReplaceCmd)
   // Standard printing commands
   ON_COMMAND(ID_FILE_PRINT, CDbgEditorView::OnFilePrint)
   ON_COMMAND(ID_FILE_PRINT_DIRECT, CDbgEditorView::OnFilePrint)
   ON_COMMAND(ID_FILE_PRINT_PREVIEW, CDbgEditorView::OnFilePrintPreview)
    // view object help
   ON_COMMAND(ID_GR_VIEW_OBJHELP, OnViewObjHelp)
   ON_MESSAGE(IDS_GR_ON_UPDATE_POSITION, OnPositionChanged)
   ON_COMMAND(ID_GR_SHOW_VAR_SEL_POPUP, OnShowVarSelPopup)
   ON_COMMAND(ID_GR_SHOW_VAR_SELECTION, OnShowVarSelection)
   ON_COMMAND_RANGE(ID_GR_INSERT_LRU_FIRST, ID_GR_INSERT_LRU_LAST, OnInsertLruPou)
   ON_COMMAND(ID_GR_XREF_LAST, OnXRefChangeRefPositionLast)
   ON_COMMAND(ID_GR_XREF_NEXT, OnXRefChangeRefPositionNext)
   ON_COMMAND(ID_GR_XREF_QUERY, OnXRefSetActiveQuery)
END_MESSAGE_MAP()


inline int PointToInt(const CPoint& P)                // conversion of CPoint to int
{
   ASSERT(MAX_XGRID*MAX_YGRID < INT_MAX);
   return P.y*MAX_XGRID+P.x;
}

/////////////////////////////////////////////////////////////////////////////
// CGrEditorView construction/destruction

CGrEditorView::CGrEditorView()
{
   _RPT0(_CRT_WARN,"\nCGrEditorView created\t");
   // normal initialization
   m_bDebug        = FALSE;
// m_bDebug2       = TRUE;  // is global in GrEditorUtil
   m_uTimerID      = 0;

   m_pFindReplaceDlg = NULL;
   m_SearchDat.m_bFindOnly     = TRUE;
   m_SearchDat.m_strFind       = _T("");
   m_SearchDat.m_strReplace    = _T("");
   m_SearchDat.m_bCase         = false;
   m_SearchDat.m_bNext         = true;
   m_SearchDat.m_bFindOnly     = true; 
   m_SearchDat.m_bWord         = false;
   m_SearchDat.m_FindCur       = CPoint(MAX_XGRID,MAX_YGRID);
   m_SearchDat.m_iFindElem     = INT_MIN;
   m_SearchDat.m_iFindPos      = 0;
   m_SearchDat.m_iFindLen      = 0;

   m_bDrawOnScreen = TRUE;

   m_pTrackedWP    = NULL;

   m_bDropSource   = false;
   m_dragFirst     = true;
   m_dragActive    = false;

   m_bActive       = false;
   m_bTick         = false;
   m_bTickCount    = 0;
   m_iTickDelta    = 0;
   m_bLocalSpacing = true;

   SetScrollSizes(MM_TEXT, CSize(0,0));
   //instantiateServer();   
}

CGrEditorView::~CGrEditorView()
{
   if( m_bFontCreated )
   {
      DeleteFont();
   }
}

BOOL CGrEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
   // TODO: Modify the Window class or styles here by modifying
   //  the CREATESTRUCT cs

   return CDbgEditorView::PreCreateWindow(cs);
}

int CGrEditorView::OnCreate (LPCREATESTRUCT lpCreateStruct)
{
    if (CDbgEditorView::OnCreate (lpCreateStruct) == -1)
        return (-1);

    if (!m_DataTip.IsCreated ())
    {
       m_DataTip.Create (this, TTS_ALWAYSTIP | TTS_NOPREFIX, this);
    }
    
    if (m_DataTip.IsCreated ())
    {
       m_DataTip.Activate ();
    }

    return (0);
}

/////////////////////////////////////////////////////////////////////////////
// CGrEditorView drawing


/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::DrawMarks(CDC *pDC, CGrEditorDoc *pDoc, CRect drawArea)
{
   EDP_POS_QUALIFIER qualifier;
   POSITION     pPosition;
   CBreakpoint *pBP=NULL;
   CPen        *pOldPen;
   CBrush      *pOldBrush;
   CRect        bpRect, r;
   CRect        stopRect;   // store rect for delete stoppoint !
   CPoint       p;
   int          iX, iY;
   BRUSH_TYPE   BrushType;

   // draw top and bottom frame for BP ..
   pOldPen   = pDC->SelectObject(GDIHelper::GetPen  (m_bDrawOnScreen, PE_NORMAL)); 
   if (IsManualMode())
   {
      pOldBrush = pDC->SelectObject(GDIHelper::GetBrush(m_bDrawOnScreen, BT_FBLKBK_SFC));
   } else {
      pOldBrush = pDC->SelectObject(GDIHelper::GetBrush(m_bDrawOnScreen, BT_FBLKBK));
   }

   p = m_LastScrollPos;
   r.SetRect(p.x, p.y, p.x + m_iDZGrid, p.y + m_iDZGrid);
   pDC->Rectangle (&r);

   if (m_DrawTarget == ActiveView)
   {
      GetClientRect (&r);
   } else {
      r = drawArea;
   }
   p = m_LastScrollPos + r.BottomRight();
   r.SetRect(p.x - m_iDZGrid, p.y - m_iDZGrid, p.x, p.y);
   pDC->Rectangle (&r);

   // draw all BP's in list ..
   pPosition = pDoc->GetFirstBPPosition ();
   while (pPosition != NULL)
   {
      pBP = pDoc->GetNextBP (pPosition);
      ASSERT (pBP != NULL);
      if (pBP == NULL)
         continue;

      qualifier = pBP->GetPosition().GetPosQual();
      switch ( qualifier ) {
         case EDP_GRBEGIN: 
            bpRect.SetRect(0,0,m_iDZGrid,m_iDZGrid); 
            bpRect.OffsetRect( m_LastScrollPos );
            break;

         case EDP_GREND  : 
            GetClientRect( &bpRect );
            bpRect.OffsetRect( m_LastScrollPos );
            bpRect.left = bpRect.right  - m_iDZGrid;
            bpRect.top  = bpRect.bottom - m_iDZGrid;
            break;

         case EDP_GRBOX  : 
            iX = pBP->GetPosition().GetX() * m_iZGrid;
            iY = pBP->GetPosition().GetY() * m_iZGrid;
            bpRect.SetRect(iX, iY, iX + m_iDZGrid, iY + m_iDZGrid); 
            break;

         default:
            continue;
      }

      if( pBP->IsActive() ) 
      {
         BrushType = pBP->IsSubscribed() ? BT_BRKACT : BT_BRKNOC;
      } else {
         BrushType = BT_BRKDIS;
      }
      pDC->SelectObject(GDIHelper::GetBrush(m_bDrawOnScreen, BrushType));

//    bpRect.left
      bpRect.right  -= m_iHZGrid;
      bpRect.top    += m_iZGrid/4;
      bpRect.bottom -= m_iZGrid/4;
      pDC->Ellipse (&bpRect);
   }

   // draw stop point
   CStdEditorPosition stopPosition;
   int    iWidth;
   int    iHeight;
   int    iLeft;
   int    iTop;
   CPoint points[7];
   bool   bDraw=true;

   if ( !m_bIsStopped && pDoc->GetStopPosition (stopPosition))
   {
      pDC->SelectObject(GDIHelper::GetBrush(m_bDrawOnScreen, BT_BRKREC));

      qualifier = stopPosition.GetPosQual();
      switch ( qualifier ) {
         case EDP_GRBEGIN: 
            stopRect.SetRect(0,0,m_iDZGrid,m_iDZGrid);
            stopRect.OffsetRect( m_LastScrollPos );
            break;

         case EDP_GREND  : 
            GetClientRect( &stopRect );
            stopRect.OffsetRect( m_LastScrollPos );
            stopRect.left = stopRect.right  - m_iDZGrid;
            stopRect.top  = stopRect.bottom - m_iDZGrid;
            break;

         case EDP_GRBOX  : 
            iX = stopPosition.GetX() * m_iZGrid;
            iY = stopPosition.GetY() * m_iZGrid;
            stopRect.SetRect(iX, iY, iX + m_iDZGrid, iY + m_iDZGrid); 
            break;

         default: bDraw = false; break;
      }

      if (bDraw)
      {
         iHeight = stopRect.Height() - m_iDZGrid /3;
         iTop    = stopRect.top      + m_iZGrid/3;
         iWidth  = stopRect.Width()  - m_iDZGrid /3;
         iLeft   = stopRect.left     + m_iZGrid/3;

         points[0].x = iLeft;              points[0].y = iTop + iHeight / 3;
         points[1].x = iLeft + iWidth / 2; points[1].y = iTop + iHeight / 3;
         points[2].x = iLeft + iWidth / 2; points[2].y = iTop;
         points[3].x = iLeft + iWidth - 1; points[3].y = iTop + iHeight / 2;
         points[4].x = iLeft + iWidth / 2; points[4].y = iTop + iHeight - 1;
         points[5].x = iLeft + iWidth / 2; points[5].y = iTop + (iHeight - 1) - iHeight / 3;
         points[6].x = iLeft;              points[6].y = iTop + (iHeight - 1) - iHeight / 3;

         pDC->Polygon (points, 7);
      }
   }
   else 
   {
      stopRect.SetRect(0,0,0,0);
   }

   pDC->SelectObject (pOldPen); 
   pDC->SelectObject (pOldBrush);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::DrawGrid(CDC * pDC, CGrEditorDoc* pDoc)
{
   CPen  *pOldPen;
   CRect  r;
   CPoint p;
   int    iDGrid= 2 * pDoc->GetGridSize();
   int    i;

   pDC->GetClipBox(&r);
   r.InflateRect(1, 1); // avoid rounding to nothing

   // Get the invalidated rectangle of the view, or in the case
   // of printing, the clipping region of the printer dc.

   if (IsManualMode())
   {
      pDC->FillRect(&r, GDIHelper::GetBrush(m_bDrawOnScreen, BT_BACKGD_SFC));
   } else {
      pDC->FillRect(&r, GDIHelper::GetBrush(m_bDrawOnScreen, BT_BACKGD));
   }

   if( m_iZGrid >= 4 ) 
   {
      pOldPen = pDC->SelectObject(GDIHelper::GetPen(m_bDrawOnScreen, PE_GRID)); 

      switch ( pDoc->GetGridType() ) {
         case GT_NO   : break;

         case GT_POINT: 
            for( p.x = ((r.left+iDGrid)/iDGrid)*iDGrid; p.x < r.right; p.x += iDGrid)
            {
               for( p.y = ((r.top+iDGrid)/iDGrid)*iDGrid; p.y < r.bottom; p.y += iDGrid)
               {
                  pDC->SetPixel(p, PALETTEINDEX(CO_GRID));
               }
            }
            break;

         case GT_CROSS: 
            i = iDGrid / 10;
            for( p.x = ((r.left+iDGrid)/iDGrid)*iDGrid; p.x < r.right; p.x += iDGrid)
            {
               for( p.y = ((r.top+iDGrid)/iDGrid)*iDGrid; p.y < r.bottom; p.y += iDGrid)
               {
                  pDC->MoveTo(p.x-i, p.y); pDC->LineTo(p.x+i+1, p.y);
                  pDC->MoveTo(p.x, p.y-i); pDC->LineTo(p.x, p.y+i+1);
               }
            }
            break;

         case GT_LINES: 
            for( p.x = ((r.left+iDGrid)/iDGrid)*iDGrid; p.x < r.right; p.x += iDGrid)
            {
               pDC->MoveTo(p.x, r.top); pDC->LineTo(p.x, r.bottom);
            }
            for( p.y = ((r.top+iDGrid)/iDGrid)*iDGrid; p.y < r.bottom; p.y += iDGrid)
            {
               pDC->MoveTo(r.left, p.y); pDC->LineTo(r.right, p.y);
            }
            break;
      }
      pDC->SelectObject(pOldPen); 
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::DrawPage(CDC * pDC, CGrEditorDoc* pDoc)
{
   CPen  *pOldPen;
   CSize  pageGrid = pDoc->GetPageGrid();
   CSize  docGrid  = pDoc->GetDocGrid();
   CPoint p;
   CRect  r;
   int    x, x2, y, y2;

   pDC->GetClipBox(&r);
   r.InflateRect(1, 1); // avoid rounding to nothing

   // draw paper edge
   pOldPen = pDC->SelectObject(GDIHelper::GetPen(m_bDrawOnScreen, PE_PAGE));

   y  = max( 0, r.top); 
   y2 = min( docGrid.cy * m_iZGrid, r.bottom);

   for( x=0; x <= docGrid.cx; x += pageGrid.cx )
   {
      x2 = x * m_iZGrid; 
      pDC->MoveTo(x2, y); 
      pDC->LineTo(x2, y2);
   }

   x = 0; x2 = docGrid.cx * m_iZGrid; 
   for( y=0; y <= docGrid.cy; y += pageGrid.cy )
   {
      y2 = y * m_iZGrid; 
      pDC->MoveTo(x,  y2); 
      pDC->LineTo(x2, y2);
   }

   // draw max extent for drawing
   pDC->SelectObject(GDIHelper::GetPen(m_bDrawOnScreen, PE_SHEET));

   y  = 0; 
   y2 = MAX_YGRID * m_iZGrid;
   x  = 0; 
   x2 = MAX_XGRID * m_iZGrid; 

   pDC->MoveTo(x2, y);  pDC->LineTo(x2, y2);
   pDC->MoveTo(x,  y2); pDC->LineTo(x2, y2);

   pDC->SelectObject(pOldPen);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::DrawList(CDC *pDC, CGrEditorDoc* pDoc, CElemList * pElemList)
{
   CRect     rectClip, rectElem, vr, r;
   CElem    *pTE;
   POSITION  pos;
   CSTEditorView *pView;
   CRgn      rgn;
   CRgn      rgnold;
   HRGN      hrgn;

   rgnold.CreateRectRgn(0,0,0,0);
   hrgn = (HRGN) rgnold.GetSafeHandle();

   ::GetClipRgn(pDC->GetSafeHdc(), hrgn);

   BOOL bPrinting = pDC->IsPrinting();
   // Get the invalidated rectangle of the view, or in the case
   // of printing, the clipping region of the printer dc.
   // !!! there seems a bug in windows to get clip rect for A3 at printing ..
   pDC->GetClipBox(&rectClip);

   // draw elements
   for( pos = pElemList->GetHeadPosition(); pos != NULL; )
   {
      pTE = pElemList->GetNext(pos);
      if( pTE )
      {
         pTE->GetBoundingRect(rectElem, m_iZGrid);
         if( bPrinting || rectElem.IntersectRect(&rectElem, &rectClip)) 
         { 
            pTE->Draw(pDC, this, m_iZGrid);

            // draw the ST-Action views on printing
            if(pTE->ChkFlag(EF_STACTION))
            {
               if(m_STViewMap.Lookup( ((CElemAct*)pTE)->GetID(), pView) && pView &&
                  (GetFocus() != pView))
               {
                  pTE->GetBodyRect(vr, m_iZGrid);
                  vr.top += 2*m_iZGrid+1;
                  vr.left++;

                  r = vr;
                  pDC->LPtoDP(&r);
                  rgn.CreateRectRgnIndirect (&r); 
                  pDC->IntersectClipRect(&vr);     // combine the new clip region !!
                  rgn.DeleteObject();

                  pView->InternalPaintWithOffset(pDC, CRect(0,0,vr.Width(), vr.Height()), false,
                     vr.TopLeft());

                  pDC->SelectClipRgn(&rgnold);   
               }
            }
         }
      }
   }
}

/*func*------------------------------------------------------------------------
  description: draw the bookmarks (if any)
  in : CDC*, CGrEditorDoc*
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::DrawBookMarks(CDC *pDC, CGrEditorDoc* /*pDoc*/)
{
   CRect rectClip;
   CRect rectBM;

   pDC->SelectObject(GDIHelper::GetPen  (true, PE_NORMAL));
   pDC->SelectObject(GDIHelper::GetBrush(true, BT_BRKDIS));

   // Note: CDbgEditorView::OnPaint() will have already adjusted the
   // viewport origin before calling OnDraw(), to reflect the
   // currently scrolled position.
   pDC->GetClipBox(&rectClip);   // doc-coordinates
   rectClip.InflateRect(1, 1);   // avoid rounding to nothing

   // draw bookmarks
   for(int idx=0; idx<m_BookMarkTab.GetSize(); idx++)
   {
      BookMark *pB = &m_BookMarkTab[idx];

      CPoint bpos = GridToDoc(pB->m_Pos);
      CSize  bsz  = pB->GetSize(pDC, this);  // box size

      rectBM.SetRect(bpos,bpos+bsz);
      rectBM -= rectBM.CenterPoint()-rectBM.TopLeft();
      rectBM.InflateRect(1,1,1,1);

      if( rectBM.IntersectRect(&rectBM, &rectClip)) 
      {
         pB->Draw(pDC,this);
      }
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::DrawFixPoints(CDC *pDC, CGrEditorDoc *pDoc)
{
   CPoint    P;
   ASSERT_VALID(pDoc);
   int i = 0;
   while( pDoc->GetElemList()->GetNextFixPoint(i,P) )
   {
      P = GridToDoc(P);
      CRect R(P-CSize(m_iHZGrid,m_iHZGrid),P+CSize(m_iHZGrid,m_iHZGrid));
      pDC->InvertRect(R);
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::DrawSelections(CDC *pDC, CGrEditorDoc* pDoc, CElemList * pElemList, CRect drawArea)
{
   CRect     rectClip;
   CRect     rectElem;
   CRect     r;
   CElem    *pTE;
   POSITION  pos;
   bool      bWithResizePoints;

   // Get the invalidated rectangle of the view, or in the case
   // of printing, the clipping region of the printer dc.
   pDC->GetClipBox(&rectClip);
   rectClip.InflateRect(1, 1); // avoid rounding to nothing

   // draw selections of elements OVER the elements
   bWithResizePoints = (bool)(pDoc->GetEditMode() != EM_MONITOR);
   for (pos = pElemList->GetHeadPosition(); pos != NULL; )
   {
      pTE = pElemList->GetNext(pos);
      if( pTE->ChkFlag(EF_SELECT) )
      { 
         pTE->GetBoundingRect(rectElem, m_iZGrid);
         if( rectElem.IntersectRect(&rectElem, &rectClip)) 
         {
            pTE->DrawSelectFrame(pDC, this, m_iZGrid, bWithResizePoints);
         }
      }
      if( pTE->ChkFlag(EF_LIGHTPIN) ) 
      { 
         pTE->GetBoundingRect(rectElem, m_iZGrid);
         if( rectElem.IntersectRect(&rectElem, &rectClip)) 
         {
            pTE->DrawLightPins(pDC, m_iZGrid);
         }
      }
   }

   if( pDoc->GetEditMode() != EM_MONITOR )   
   {
      // draw error at region ..
      if( !pDoc->m_RegionError.IsRectEmpty() )
      {
         r = pDoc->m_RegionError;
         pDC->SelectObject(GDIHelper::GetPen(m_bDrawOnScreen, PE_ERROR));
         pDC->MoveTo(r.left *m_iZGrid, r.top   *m_iZGrid);
         pDC->LineTo(r.right*m_iZGrid, r.top   *m_iZGrid);
         pDC->LineTo(r.right*m_iZGrid, r.bottom*m_iZGrid);
         pDC->LineTo(r.left *m_iZGrid, r.bottom*m_iZGrid);
         pDC->LineTo(r.left *m_iZGrid, r.top   *m_iZGrid);
      }

   } else {
      DrawMarks     (pDC, pDoc, drawArea);
      DrawWatchpoints     (pDC, drawArea); 
      DrawActionInfos     (pDC, drawArea);
      DrawTransitionInfos (pDC, drawArea);
   } 
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnDraw (CDC *pDC)
{
   CRect  r;
   CSize  drawAreaSize;

   GetClientRect(&r);
   drawAreaSize.cx = r.Width();
   drawAreaSize.cy = r.Height();

   DrawInto ( pDC, drawAreaSize);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::DrawInto (CDC *pDC, CSize drawAreaSize)
{
   int     OldRop;
   CPen   *OldPen; 
   CBrush *OldBrush, Brush;
   CRect   drawArea;

   drawArea.left = 0;
   drawArea.top = 0;
   drawArea.right = drawAreaSize.cx;
   drawArea.bottom = drawAreaSize.cy;

// --------------------

   CDC       dc;
   CDC      *pDrawDC;
   CBitmap   bitmap;
   CBitmap  *pOldBitmap=NULL;
   CPalette *pOldPal;
   BOOL      bOptimized=FALSE;
   CPoint    p;
   CRect     r, client;
   int       iDC;
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   m_bDrawOnScreen = !pDC->IsPrinting();

   pDrawDC = pDC;
   // for screen only - get clip rect
   pDC->GetClipBox(client);

   // optimize screen drawing ..
   if( m_bDrawOnScreen )
   {
      if( dc.CreateCompatibleDC(pDC) &&
          bitmap.CreateCompatibleBitmap(pDC, client.Width(), client.Height()))
      {
         OnPrepareDC( &dc, NULL);
         dc.OffsetViewportOrg( -(client.left - m_LastScrollPos.x), -(client.top - m_LastScrollPos.y));
         pOldBitmap = dc.SelectObject(&bitmap);

         dc.IntersectClipRect(client);
         pDrawDC = &dc;
         bOptimized = TRUE;
      }
   }
   // begin drawing ..
   iDC = pDrawDC->SaveDC();
   pOldPal = GDIHelper::RealizePalette(pDrawDC, m_bDrawOnScreen);

   pDrawDC->SelectObject( &m_PropFont ); // one font for all text

   if( m_bDrawOnScreen )
   {
      DrawGrid( pDrawDC, pDoc );
      DrawPage( pDrawDC, pDoc );
   }

   DrawList( pDrawDC, pDoc, pDoc->GetElemList() );
   
   if (m_bDrawOnScreen)
   {
      DrawBookMarks ( pDrawDC, pDoc);
      DrawSelections( pDrawDC, pDoc, pDoc->GetElemList(), drawArea );

      if( pDoc->GetEditMode() == EM_SELECT )
      {
         if( Brush.CreateStockObject(NULL_BRUSH) )
         {
            OldRop   = pDrawDC->SetROP2(R2_NOT);
            OldBrush = pDrawDC->SelectObject(&Brush);
            OldPen   = pDrawDC->SelectObject(GDIHelper::GetPen(m_bDrawOnScreen, PE_NORMAL));
            // draw new rect
            pDrawDC->Rectangle( m_StartPos.x, m_StartPos.y, m_EndPos.x, m_EndPos.y);
            pDrawDC->SetROP2(OldRop);
            pDrawDC->SelectObject(OldBrush);
            pDrawDC->SelectObject(OldPen  );
         }
      }
      DrawFixPoints ( pDrawDC, pDoc);
   }
   GDIHelper::UnrealizePalette(pDrawDC, pOldPal);
   pDrawDC->RestoreDC(iDC);

   // end of screen optimizing ..
   if( m_bDrawOnScreen && bOptimized )
   {
      pDC->SetViewportOrg(0,0);
      pDC->SetWindowOrg(0,0);
      pDC->SetMapMode(MM_TEXT);

      dc.SetViewportOrg(0,0);
      dc.SetWindowOrg(0,0);
      dc.SetMapMode(MM_TEXT);

      p = m_LastScrollPos;
      pDC->BitBlt(
         client.left - p.x, client.top - p.y, client.Width(), client.Height(),
         &dc, 0, 0, SRCCOPY);
      if (pOldBitmap)
      {
         dc.SelectObject(pOldBitmap);
      }
   }
}

/////////////////////////////////////////////////////////////////////////////
// OLE Server support

// The following command handler provides the standard keyboard
//  user interface to cancel an in-place editing session.  Here,
//  the server (not the container) causes the deactivation.

void CGrEditorView::OnCancelEditSrvr()
{
	EventCancel();
}

/////////////////////////////////////////////////////////////////////////////
// CGrEditorView diagnostics

#ifdef _DEBUG
void CGrEditorView::AssertValid() const
{
   CDbgEditorView::AssertValid();
}

void CGrEditorView::Dump(CDumpContext& dc) const
{
   CDbgEditorView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGrEditorView message handlers

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
   CGrEditorDoc* pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   // The document has informed this view that some data has changed.
   // we invalidate all and every time in DOC coordinates !!!

   if (lHint == CPrgEditorDoc::IdleHint)
   {
      OnIdle ();
      return;
   };

   if (lHint == 0 || 
       lHint == CGrEditorDoc::UpdateWindowHint)
   {  
      // will also be called at OnInitialUpdate !!
      CView *p = dynamic_cast<CSTEditorView*>(pSender);
      if (p==NULL)
      {
	      UpdateSTActionView(NULL);

         Invalidate(FALSE);
      }
      return;
   }

   if (lHint == CGrEditorDoc::UpdateDrawHint )
   {  // we draw the invalidated regions ..
      return;
   }

   if (lHint == CGrEditorDoc::UpdateDrawElemHint )
   {  // we draw THIS element
      InvalidateElem((CElem *)pHint);
      return;
   }

   if (lHint == CGrEditorDoc::UpdateSelectionHint )
   {
      InvalidateSelection();
      return;
   }

   if (lHint == CGrEditorDoc::UpdateMarkedElemHint )
   {
      InvalidateMarkedElem();
      return;
   }

   if (lHint == CGrEditorDoc::UpdateOleItemsHint )
   {  // if we insert OLE elements in FBD, then we need this ..
      // perhaps the FB/FCT/PRO (pre)view with COM-interface .. ?
      return;
   }

   if (lHint == CGrEditorDoc::UpdateRubberHint )
   {
      InvalidateRubber();
      return;
   }

   if (lHint == CGrEditorDoc::UpdateFixPointsHint )
   {
      InvalidateFixPoints();
      return;
   }

   if (lHint == CGrEditorDoc::UpdateLightPinHint )
   {
      InvalidateLightPins();
      return;
   }

   if (lHint == CGrEditorDoc::UpdateModifiedHint)
   {
      UpdateModified ();
      return;
   };

   if (lHint == CGrEditorDoc::UpdateColorHint)
   {
      UpdateColor ();
      return;
   };

   if (lHint == CGrEditorDoc::EditNameStopHint)
   {
      OnEditnameStop ();
      return;
   };

   if (lHint == CGrEditorDoc::VarSelStopHint)
   {
      OnVarSelStop ();
      return;
   };

   if (lHint == CGrEditorDoc::UpdateSTActionsHint)
   {
      OnUpdateSTActions((CElemAct*) pHint);
   }
   
   if (lHint == CGrEditorDoc::SyncSTActionsHint)
   {
      SyncSTActionText( (CElemPtrList*) pHint);
   }

   if (lHint == CGrEditorDoc::UpdateStatusHint)
   {
      OnUpdateStatus ();
      return;
   };

   if (lHint == CGrEditorDoc::SetPageSizeHint)
   {
      OnSetPageSize ();
      UpdateSTActionView(NULL);  // update all
      return;
   };


   if (lHint == CStdEditorDoc::GotoPositionHint)
   {
      GotoPosition (*(CStdEditorPosition *)pHint);
      return;
   };

   if (lHint == CStdEditorDoc::GetPositionHint)
   {
      GetPosition (*(CStdEditorPosition *)pHint);
      return;
   };

   if (lHint == CPrgEditorDoc::InsertFBCallHint)
   {
      OnInsertFBCallHint (*(CString *)pHint);
      return;
   };

   if (lHint == CStdEditorDoc::GotoLocationHint)
   {
      GotoLocation (*(CString *)pHint);
   };

   if (lHint == CGrEditorDoc::StartLDTracingHint)
   {
      OnSetLadderWatchpoints ();
   };
   
   if (lHint == CGrEditorDoc::StopLDTracingHint)
   {
      OnClearLadderWatchpoints ();
   };

   CDbgEditorView::OnUpdate (pSender, lHint, pHint);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::InvalidateElem(CElem *pE)
{
   CRect  r;

   ASSERT(pE);
   ASSERT(pE->IsKindOf(RUNTIME_CLASS(CElem)));

   pE->GetBoundingRect(r, m_iZGrid); 
   if (m_bDebug2)
   {
      r.InflateRect(50,50);
   } else {
      r.InflateRect(m_iZGrid,m_iZGrid);
   }
   DocToClient(r);
   InvalidateRect(r, FALSE); 
   if (pE->ChkFlag(EF_STACTION))
   {
      UpdateSTActionView((CElemAct*)pE);
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::InvalidateSelection()
{
   CRect     r;
   CElem    *pTE;
   POSITION  pos;
   CGrEditorDoc* pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   for (pos = pDoc->GetElemList()->GetHeadPosition(); pos != NULL; )
   {
      pTE = pDoc->GetElemList()->GetNext(pos);
      if (pTE && 
          pTE->ChkFlag(EF_SELECT) )
      {
         pTE->GetBoundingRect(r, m_iZGrid); 
         DocToClient(r);
         InvalidateRect(r, FALSE);          

         if(pTE->ChkFlag(EF_STACTION))
         {
            UpdateSTActionView((CElemAct*)pTE);
         }
      }
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::InvalidateMarkedElem()
{
   CRect     r;
   CElem    *pTE;
   POSITION  pos;
   CGrEditorDoc* pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   for( pos = pDoc->GetElemList()->GetHeadPosition(); pos != NULL; )
   {
      pTE = pDoc->GetElemList()->GetNext(pos);
      ASSERT_VALID(pTE);
      if( pTE && (pTE->ChkFlag(EF_UPDATE)||pTE->ChkFlag(EF_AUTOELEM)) )
      {
         pTE->GetBoundingRect(r, m_iZGrid); 
         if( m_bDebug2 ) 
         {
            r.InflateRect(50,50);
         }
         DocToClient(r);
         InvalidateRect(r, FALSE);
         pTE->ClrFlag(EF_UPDATE);

         if(pTE->ChkFlag(EF_STACTION))
         {
            UpdateSTActionView((CElemAct*)pTE);
         }
      }
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::InvalidateRubber()
{
   CRect     r;
   CElem    *pTE;
   POSITION  pos;
   CGrEditorDoc* pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   for( pos = pDoc->GetElemList()->GetHeadPosition(); pos != NULL; )
   {
      pTE = pDoc->GetElemList()->GetNext(pos);
      ASSERT_VALID(pTE);
      if( pTE && pTE->ChkFlag(EF_RUBBLINE) )
      {
         pTE->GetBoundingRect(r, m_iZGrid); 
         if( m_bDebug2 )
         {
            r.InflateRect(50,50);
         } else {
            r.InflateRect(m_iZGrid,m_iZGrid);
         }
         DocToClient(r);
         InvalidateRect(r, FALSE);
      }
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::InvalidateLightPins()
{
   CRect     r;
   CElem    *pTE;
   CPin     *pPin;
   POSITION  pos;
   int       i;
   CGrEditorDoc* pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   if ((pDoc->GetEditMode() == EM_LINE) || (pDoc->GetEditMode() == EM_LINE_START))
   {
      for( pos = pDoc->GetElemList()->GetHeadPosition(); pos != NULL; )
      {
         pTE = pDoc->GetElemList()->GetNext(pos);
         ASSERT_VALID(pTE);
         if (pTE)
         {
            bool bLightPin=false;

            for( i=0; i<pTE->m_Con.GetSize(); i++ )
            {
               pPin = pTE->m_Con.GetAt(i);
               ASSERT_VALID(pPin);
               if (pPin)
               { 
                  if ((abs(m_CurPos.x - pPin->m_Pos.x) < LIGHTPIN_RANGE) && 
                      (abs(m_CurPos.y - pPin->m_Pos.y) < LIGHTPIN_RANGE))
                  {
                     pPin->m_Flag.Set(PF_LIGHTPIN);
                     bLightPin = true;

                     r.SetRect(pPin->m_Pos.x - 1, 
                               pPin->m_Pos.y - 1,
                               pPin->m_Pos.x + 1,
                               pPin->m_Pos.y + 1);
                     GridToClient(r);
                     InvalidateRect(r, FALSE);
                  } 
                  else if (pPin->m_Flag.Chk(PF_LIGHTPIN))
                  {
                     pPin->m_Flag.Clr(PF_LIGHTPIN);

                     r.SetRect(pPin->m_Pos.x - 1, 
                               pPin->m_Pos.y - 1,
                               pPin->m_Pos.x + 1,
                               pPin->m_Pos.y + 1);
                     GridToClient(r);
                     InvalidateRect(r, FALSE);
                  }
               }
            }
            if (bLightPin)
            {
               pTE->SetFlag(EF_LIGHTPIN);
            } else {
               pTE->ClrFlag(EF_LIGHTPIN);
            }
         }
      }
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::InvalidateFixPoints()
{
   CPoint    P;
   CGrEditorDoc* pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   int i = 0;
   while( pDoc->GetElemList()->GetNextFixPoint(i,P) )
   {
      P = GridToClient(P);
      CRect R(P-CSize(m_iHZGrid,m_iHZGrid),P+CSize(m_iHZGrid+1,m_iHZGrid+1));
      InvalidateRect(R, FALSE);
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::UpdateModified ()
{
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   if ((pDoc->GetEditMode() == EM_EDITPROP) &&
       (m_pSEC_Edit != NULL))
   {
      SECEdit *pEdit;

      pEdit = m_pSEC_Edit->GetEdit ();
      ASSERT(pEdit);
      if (pEdit != NULL)
      {
         pDoc->SetModifiedFlag (pEdit->GetModified ());
      }
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::UpdateColor ()
{
   DeleteFont();
   CreateFont();
   OnUpdate(NULL, CGrEditorDoc::UpdateWindowHint, NULL);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnEditnameStop ()
{
   EditNameStop(false);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnVarSelStop ()
{
   if (GetFocus())
      ::SendMessage(GetFocus()->m_hWnd, WM_CHAR, VK_RETURN, 0);
}

/*func*------------------------------------------------------------------------
  proxy function -> CElemEditorView::OnUpdateSTActions(CElemAct* pAct)
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnUpdateSTActions(CElemAct* pAct)
{
   if(!m_bFontCreated)
   {
      CreateFont();
   }
   UpdateSTActionView(pAct);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnUpdateStatus ()
{
   UpdateStatus();
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnSetPageSize ()
{
   SetPageSize ();
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::GotoPosition (const CStdEditorPosition &position)
{
   CPoint p, p2;
	CElem *pE;
   int    i, j;
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   switch (position.GetPosQual())
   {
   case EDP_GRSTBOX :
		i = 2 * position.GetCY();  // lineno in STtext
		if (i == -1) 
			break;
		// search STview data in gr
		p2 = CPoint(position.GetX(),position.GetY());
		if (!pDoc || !pDoc->GetElemList())
			break;
      pE = pDoc->GetElemList()->SearchElem(p2,1);
		if (pE)
		{
			j = pE->GetSize().cy;
		} else {
			j = 2;
		}
		// search edit-control for STtext
		{
			CPoint context = CPoint(position.GetX(), position.GetY());
			POSITION   pos = m_STViewMap.GetStartPosition();
			CSTEditorView *pView;
			UINT Key;
			
			while (pos)
			{
				m_STViewMap.GetNextAssoc(pos, Key, pView);
				if (context == pView->GetContext())
				{
   				i -= 2 * (pView->m_iFirstLineNo - 1);
					break;
				}
			}
		}
		if (i < 0) i = 0;
		if (i > j) i = j;
		// i: relative lineno in STtext (maybe scrolled in view ..)
      p.x = (position.GetX() + 2 +1) & 0xFFFFFFFE;
      p.y = (position.GetY() + i -3) & 0xFFFFFFFE;
      break;

   case EDP_STLINE  :
   case EDP_GRBOX   :
   case EDP_GRLDBOX :
   case EDP_GRREGION:
   case EDP_GRBEGIN :
   case EDP_GREND	  :
   case EDP_GRLINE  :
   case EDP_GRSIGNAL:
      p.x = (position.GetX() + 2 +1) & 0xFFFFFFFE;
      p.y = (position.GetY()     +1) & 0xFFFFFFFE;
      break;
   }
   m_LogCurPos = GridToDoc(p); 

   MoveViewToCursor();
   m_NoAutoScroll = true;
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::GetPosition (CStdEditorPosition &position)
{
   position = CStdEditorPosition (EDP_STLINE, m_CurPos.x, m_CurPos.y);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::GotoLocation (const CString &strLocation)
{
   CStdEditorPosition position;
   CGrEditorDoc *pDoc;
   CString    strType;
   CPoint     p1, p2;
   CElem     *pE;
   HRESULT    hr;

   pDoc = GetDocument ();
   ASSERT_VALID(pDoc);

   if( pDoc )
   {
      hr = pDoc->DecodeLocation (strLocation, strType, position, p2);
      if (FAILED (hr))
      {
         return;
      };
      p1 = CPoint( position.GetX(), position.GetY() );

      if( strType != _T("") ) // not in header ..
      {
         GotoPosition (position);
      }
      
      pDoc->GetElemList()->ClearFlags(EF_EMPHASIZE);
      pDoc->m_RegionError.SetRectEmpty();

      if( strType == _T("b") || 
			 strType == _T("B") )   // action block, ST-action
      {
         if( (pE = pDoc->GetElemList()->GetCurElem( p1 )) != NULL )
         {
            if (strType == _T("b"))
					pE->SetFlag(EF_EMPHASIZE);
            CenterViewToCursor(pE->GetOrigin()); 

            if ((pE->GetType() == ET_ACTION || pE->GetType() == ET_ACTIONBLOCK) &&
                 pE->ChkFlag(EF_STACTION))
            {
               // clear old edit mode ..
               if (pDoc->GetEditMode() != EM_NORMAL)
               {
                  EventCancel();
               }
               if (pDoc->GetEditMode() != EM_NORMAL)
               {
                  EventCancel();
               }
               if (pDoc->GetEditMode() == EM_NORMAL)
               {
                  // set new Edit mode ..
                  MovePos(pE->GetOrigin().x+1, pE->GetOrigin().y+3);
                  EventEnter();
                  if (m_pSEC_Edit)
                  {
                     SECEdit *pEdit;

                     pEdit = m_pSEC_Edit->GetEdit ();
                     ASSERT(pEdit);
                     if (pEdit != NULL)
                     {
                        int iLine   = p2.x;
                        int iColumn = p2.y;
                        int iLines;
                  
                        if (iColumn <= 0) iColumn = 1;
                        if (iLine   <= 0)   iLine = 1;
                        if (iColumn <= 0)
                        {
                           PLINEDESC pLineDesc;
                     
                           pLineDesc = pEdit->GoToLineNo (iLine);
                           iColumn = pEdit->GetTabbedOffset (pLineDesc, -iColumn) + 1;
                        };
                        // Always clear a selection when jumping to a position.
                        iLines = pEdit->m_iLinesInFile;
                        iLine = ((iLine > iLines) ? iLines : iLine);
                        m_pSEC_Edit->GotoLineCol (iLine, iColumn - 1);
                     }
                  }
               }
            }
         }
      } 
      else if( strType == _T("l") ) // line
      {
         pE = pDoc->GetElemList()->SearchLine (p1, p2, true);
         if (pE == NULL)
         {
            pE = pDoc->GetElemList()->SearchLine (p1, p2, false);
         }
         if (pE != NULL)
         {
            pE->SetFlag(EF_EMPHASIZE);
            CenterViewToCursor(pE->GetOrigin()); 
         } 
      }
      else if( strType == _T("s") ) // signal
      {
         pDoc->GetElemList()->ClearFlags(EF_UPDATE);
         if (!pDoc->GetElemList()->FlagSignal(p1, EF_EMPHASIZE))
         {  // at direct contected elements is no line !
            // so we set here the common region error
            pDoc->m_RegionError.SetRect( p1.x-1, p1.y-1, p1.x+1, p1.y+1);
            CenterViewToCursor(p1); 
         }
      }
      else if( strType == _T("r") ) // region
      {
         pDoc->m_RegionError.SetRect( p1.x, p1.y, p1.x+p2.x, p1.y+p2.y);
         CenterViewToCursor(p1); 
      } 
      else if( strType == _T("begin") )
      {
         ; // TBD
      } 
      else if( strType == _T("end") )
      {
         ; // TBD
      } 
      else if( strType == _T("f") )
      {
         m_SearchDat.m_FindCur   =  p1;
         m_SearchDat.m_iFindElem =  p2.x;
         m_SearchDat.m_iFindPos  = (p2.y >> 16) & 0xFFFF;
         m_SearchDat.m_iFindLen  = (p2.y      ) & 0xFFFF;
         m_SearchDat.m_strFind   = _T("");

         CString stLoc;

         if(position == CStdEditorPosition(EDP_STLINE, 0, 0))
         {
            CSTEditorView *pSTView = (CSTEditorView*)pDoc->GetSTViewRef(CPoint(0,0));
//            GetParentFrame()->SetActiveView(pSTView);
            stLoc.Format("%i,%i", m_SearchDat.m_iFindElem, m_SearchDat.m_iFindPos);
            pSTView->GotoLocation(stLoc);
         }else
         {
            GetParentFrame()->SetActiveView(this);
            CElem* pE = pDoc->GetElemList()->GetCurElem(p1);

            if (pE)
            {
               UnselectAll();
               pE->SetFlag(EF_SELECT);

               if (pE->ChkFlag(EF_STACTION) && (p2.x == -2))  // -2: txtIdent for ST !
               {
                  CElemAct* pA = (CElemAct*)pE;
                  CPoint Context = pE->GetOrigin();

                  CSTEditorView *pSTView;
                  POSITION pos = m_STViewMap.GetStartPosition();

                  while(pos)
                  {
                     UINT Key;
                     m_STViewMap.GetNextAssoc(pos, Key, pSTView);
               
                     if(pSTView && (Context == pSTView->GetContext()) )
                     {
                        CString str;
                        int iCol=0, iRow=1, i;

                        pE->GetText(-2, 0, 32000, str);
                        if (m_SearchDat.m_iFindPos < str.GetLength())
                        {

                           for (i=0; i<m_SearchDat.m_iFindPos; i++)
                           {
                              if (str[i] == '\n') 
                              { 
                                 iRow++; iCol = 0; 
                              } else {
                                 iCol++;
                              }
                           }
                        }
//                        GetParentFrame()->SetActiveView(pSTView);
//                        stLoc.Format("%i,%i", iCol, iRow);
//                        pSTView->GotoLocation(stLoc);
                        CWnd* pWnd = GetFocus();
                        pSTView->SetFocus();
                        pSTView->SelectSearchText(iRow, iCol, m_SearchDat.m_iFindLen);
                        pWnd->SetFocus();
                        break;
                     }
                  }
               } else {
                  GetSelectedText(m_SearchDat.m_strFind);
               }
            }
         }


         CenterViewToCursor(m_SearchDat.m_FindCur); 
      } 
      else if( strType == _T("") ) // header
      {
         CString stLoc;
         stLoc.Format("%i,%i", p2.x, p2.y);

         if(position == CStdEditorPosition(EDP_STLINE, 0, 0))
         {
            CSTEditorView *pSTView = (CSTEditorView*)pDoc->GetSTViewRef(CPoint(0,0));
//            GetParentFrame()->SetActiveView(pSTView);
            pSTView->GotoLocation(stLoc);
//            pSTView->GotoPosition (CStdEditorPosition(EDP_STLINE, p2.x, p2.y));
            GetParentFrame()->SetActiveView(pSTView);
         }
      }
      else 
      {
         ; // unknown error !
      }

      pDoc->GetElemList()->MoveFlaggedToTail(EF_RUBBLINE);
      pDoc->GetElemList()->MoveFlaggedToTail(EF_EMPHASIZE);
      pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateWindowHint);
   } 
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnUpdateEditReplace(CCmdUI* pCmdUI) 
{
   if(CheckReadonly())
   {
       pCmdUI->Enable(FALSE);
       return;
   }

   CGrEditorDoc* pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   ASSERT_VALID(this);
   pCmdUI->Enable( (pDoc->GetElemList()->GetCount() > 0) &&
                   ((pDoc->GetEditMode() == EM_NORMAL) ||
                    (pDoc->GetEditMode() == EM_LINE_START) ||
                    ((pDoc->GetEditMode() >= EM_INSCOL) && (pDoc->GetEditMode() <= EM_MONITOR))) );

   ASSERT_VALID(this);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnUpdateNeedText(CCmdUI* pCmdUI) 
{
   CGrEditorDoc* pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   ASSERT_VALID(this);
   pCmdUI->Enable( (pDoc->GetElemList()->GetCount() > 0) &&
                   ((pDoc->GetEditMode() == EM_NORMAL) ||
                    (pDoc->GetEditMode() == EM_LINE_START) ||
                    ((pDoc->GetEditMode() >= EM_INSCOL) && (pDoc->GetEditMode() <= EM_MONITOR))) );

   ASSERT_VALID(this);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnUpdateNeedFind(CCmdUI* pCmdUI) 
{
   CGrEditorDoc* pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   ASSERT_VALID(this);
   pCmdUI->Enable( (pDoc->GetElemList()->GetCount() > 0) && 
                   (!m_SearchDat.m_strFind.IsEmpty()) && 
                   ((pDoc->GetEditMode() == EM_NORMAL) ||
                    (pDoc->GetEditMode() == EM_LINE_START) ||
                    ((pDoc->GetEditMode() >= EM_INSCOL) && (pDoc->GetEditMode() <= EM_MONITOR))) );
   ASSERT_VALID(this);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
BOOL CGrEditorView::OnEraseBkgnd(CDC* /*pDC*/) 
{
   // no erase - draw all in OnDraw!
   return TRUE; // CDbgEditorView::OnEraseBkgnd(pDC);
}
/*
void CGrEditorView::instantiateServer() {
	try {			
		CoInitialize(NULL);
		this->ptr.CreateInstance(__uuidof(QtEditor));
		this->ptr->stayOnTop();
		this->ptr->show();
		this->ptr->setInitialPosition();
		//std::string s((LPCTSTR)GetDocument()->GetTitle());
		this->ptr->setWindowTitle("ATCMControl_HMI.cpp");		
	} catch(_com_error &ex) {
		//cout<< ex.ErrorMessage();
	}
}

void CGrEditorView::releaseServer() {
	this->ptr->hide();
	CoUninitialize();
}

void CGrEditorView::putInputComponentOnSimulator() {
	ptr->putInputComponent();
}

void CGrEditorView::putOutputComponentOnSimulator() {
	ptr->putOutputComponent();
}
*/

// void CGrEditorView::OnInsFb()          { /* no IF !! */ ;      }
// void CGrEditorView::OnInsVarIn()       { if (!OnModifyContent ()) return; else InsertVar(VT_IN);     }
// void CGrEditorView::OnInsVarOut()      { if (!OnModifyContent ()) return; else InsertVar(VT_OUT);    }
// void CGrEditorView::OnInsVarinout()    { if (!OnModifyContent ()) return; else InsertVar(VT_INOUT);  }
// void CGrEditorView::OnInsLab()         { if (!OnModifyContent ()) return; else InsertLabel(LT_DEST); }
// void CGrEditorView::OnInsJmp()         { if (!OnModifyContent ()) return; else InsertLabel(LT_JUMP); }
// void CGrEditorView::OnInsConIn()       { if (!OnModifyContent ()) return; else InsertCon(COT_IN);    }
// void CGrEditorView::OnInsConOut()      { if (!OnModifyContent ()) return; else InsertCon(COT_OUT);   }
// void CGrEditorView::OnInsRet()         { if (!OnModifyContent ()) return; else InsertRet();          }
// void CGrEditorView::OnInsCmt()         { if (!OnModifyContent ()) return; else InsertCmt();          }
// void CGrEditorView::OnInsHLine()       { if (!OnModifyContent ()) return; else InsertLine(true,true);}
// void CGrEditorView::OnInsVLine()       { if (!OnModifyContent ()) return; else InsertLine(false,true);}

// void CGrEditorView::OnInsStep()        { if (!OnModifyContent ()) return; else InsertStep();         }
// void CGrEditorView::OnInsTransition()  { if (!OnModifyContent ()) return; else InsertTran();         }
// void CGrEditorView::OnInsAction()      { if (!OnModifyContent ()) return; else InsertAction(FALSE);  }
// void CGrEditorView::OnInsActionBlock() { if (!OnModifyContent ()) return; else InsertAction(TRUE);   }
// void CGrEditorView::OnInsDivCon()      { if (!OnModifyContent ()) return; else InsertDiv(TRUE );     }
// void CGrEditorView::OnInsSDivCon()     { if (!OnModifyContent ()) return; else InsertDiv(FALSE);     }
// void CGrEditorView::OnInsExceptionAction() { if (!OnModifyContent ()) return; else InsertAction(FALSE, _T("ExceptionAction"));  }

// void CGrEditorView::OnInsPower()       { if (!OnModifyContent ()) return; else InsertPower(true);         }
// void CGrEditorView::OnInsCtNormal()    { if (!OnModifyContent ()) return; else InsertContact(LD_NORMAL);  }
// void CGrEditorView::OnInsCtClosed()    { if (!OnModifyContent ()) return; else InsertContact(LD_CLOSED);  }
// void CGrEditorView::OnInsCtPosTrans()  { if (!OnModifyContent ()) return; else InsertContact(LD_POSTRANS);}
// void CGrEditorView::OnInsCtNegTrans()  { if (!OnModifyContent ()) return; else InsertContact(LD_NEGTRANS);}
// void CGrEditorView::OnInsClNormal()    { if (!OnModifyContent ()) return; else InsertCoil(LD_NORMAL);     }
// void CGrEditorView::OnInsClNegative()  { if (!OnModifyContent ()) return; else InsertCoil(LD_NEGATIVE);   }
// void CGrEditorView::OnInsClSet()       { if (!OnModifyContent ()) return; else InsertCoil(LD_SET);        }
// void CGrEditorView::OnInsClReset()     { if (!OnModifyContent ()) return; else InsertCoil(LD_RESET);      }
// void CGrEditorView::OnInsClPosTrans()  { if (!OnModifyContent ()) return; else InsertCoil(LD_POSTRANS);   }
// void CGrEditorView::OnInsClNegTrans()  { if (!OnModifyContent ()) return; else InsertCoil(LD_NEGTRANS);   }

void CGrEditorView::OnInsFb()          { /* no IF !! */ ;      }
void CGrEditorView::OnInsVarIn()       { if (!OnModifyContent ()) return; else {InsertVar(VT_IN); /*putInputComponentOnSimulator();*/}    }
void CGrEditorView::OnInsVarOut()      { if (!OnModifyContent ()) return; else {InsertVar(VT_OUT);  /*putOutputComponentOnSimulator(); */ }}
void CGrEditorView::OnInsVarinout()    { if (!OnModifyContent ()) return; else {InsertVar(VT_INOUT); /*putInputComponentOnSimulator(); */}}
void CGrEditorView::OnInsLab()         { if (!OnModifyContent ()) return; else {InsertLabel(LT_DEST); /*putInputComponentOnSimulator();*/}}
void CGrEditorView::OnInsJmp()         { if (!OnModifyContent ()) return; else {InsertLabel(LT_JUMP); /*putInputComponentOnSimulator();*/ }}
void CGrEditorView::OnInsConIn()       { if (!OnModifyContent ()) return; else {InsertCon(COT_IN);   /*putInputComponentOnSimulator(); */}}
void CGrEditorView::OnInsConOut()      { if (!OnModifyContent ()) return; else {InsertCon(COT_OUT);   }}
void CGrEditorView::OnInsRet()         { if (!OnModifyContent ()) return; else {InsertRet(); /*putInputComponentOnSimulator();*/          }}
void CGrEditorView::OnInsCmt()         { if (!OnModifyContent ()) return; else {InsertCmt(); /*putInputComponentOnSimulator();*/         }}
void CGrEditorView::OnInsHLine()       { if (!OnModifyContent ()) return; else {InsertLine(true,true); /*putInputComponentOnSimulator();*/}}
void CGrEditorView::OnInsVLine()       { if (!OnModifyContent ()) return; else {InsertLine(false,true); /*putInputComponentOnSimulator();*/}}

void CGrEditorView::OnInsStep()        { if (!OnModifyContent ()) return; else {InsertStep(); /*putInputComponentOnSimulator();*/         }}
void CGrEditorView::OnInsTransition()  { if (!OnModifyContent ()) return; else {InsertTran(); /*putInputComponentOnSimulator();*/        }}
void CGrEditorView::OnInsAction()      { if (!OnModifyContent ()) return; else {InsertAction(FALSE); /*putInputComponentOnSimulator();*/  }}
void CGrEditorView::OnInsActionBlock() { if (!OnModifyContent ()) return; else {InsertAction(TRUE); /*putInputComponentOnSimulator();*/  }}
void CGrEditorView::OnInsDivCon()      { if (!OnModifyContent ()) return; else {InsertDiv(TRUE ); /*putInputComponentOnSimulator();*/    }}
void CGrEditorView::OnInsSDivCon()     { if (!OnModifyContent ()) return; else {InsertDiv(FALSE); /*putInputComponentOnSimulator(); */   }}
void CGrEditorView::OnInsExceptionAction() { if (!OnModifyContent ()) return; else {InsertAction(FALSE, _T("ExceptionAction")); /*putInputComponentOnSimulator();*/ }}

void CGrEditorView::OnInsPower()       { if (!OnModifyContent ()) return; else {InsertPower(true); /*putInputComponentOnSimulator(); */        }}
void CGrEditorView::OnInsCtNormal()    { if (!OnModifyContent ()) return; else {InsertContact(LD_NORMAL); /*putInputComponentOnSimulator();*/  }}
void CGrEditorView::OnInsCtClosed()    { if (!OnModifyContent ()) return; else {InsertContact(LD_CLOSED); /*putInputComponentOnSimulator();*/ }}
void CGrEditorView::OnInsCtPosTrans()  { if (!OnModifyContent ()) return; else {InsertContact(LD_POSTRANS); /*putInputComponentOnSimulator();*/}}
void CGrEditorView::OnInsCtNegTrans()  { if (!OnModifyContent ()) return; else {InsertContact(LD_NEGTRANS); /*putInputComponentOnSimulator();*/}}
void CGrEditorView::OnInsClNormal()    { if (!OnModifyContent ()) return; else {InsertCoil(LD_NORMAL);  /*putInputComponentOnSimulator(); */  }}
void CGrEditorView::OnInsClNegative()  { if (!OnModifyContent ()) return; else {InsertCoil(LD_NEGATIVE); /*putInputComponentOnSimulator();*/   }}
void CGrEditorView::OnInsClSet()       { if (!OnModifyContent ()) return; else {InsertCoil(LD_SET);  /*putInputComponentOnSimulator();*/      }}
void CGrEditorView::OnInsClReset()     { if (!OnModifyContent ()) return; else {InsertCoil(LD_RESET); /*putInputComponentOnSimulator(); */    }}
void CGrEditorView::OnInsClPosTrans()  { if (!OnModifyContent ()) return; else {InsertCoil(LD_POSTRANS); /*putInputComponentOnSimulator(); */  }}
void CGrEditorView::OnInsClNegTrans()  { if (!OnModifyContent ()) return; else {InsertCoil(LD_NEGTRANS); /*putInputComponentOnSimulator();*/  }}


/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnEditCol() 
{
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   if (!OnModifyContent ())
   {
      ::MessageBeep(-1);
      return;
   }
   if( pDoc->GetEditMode() == EM_NORMAL )
   {
      pDoc->GetElemList()->UpdatePinStatus();

      if( GetKeyState(VK_SHIFT) < 0) 
      {
         if( pDoc->GetElemList()->CheckSpace(CPoint(-2,0), m_CurPos, m_bLocalSpacing) ) 
         { 
            pDoc->SetEditMode( EM_DELCOL );
         } else {                                           
            pDoc->SetEditMode( EM_DELCOL1 );
         }
      } else {
         if( pDoc->GetElemList()->CheckSpace(CPoint( 2,0), m_CurPos, m_bLocalSpacing) ) 
         {
            pDoc->SetEditMode( EM_INSCOL );
         } else {                                           
            pDoc->SetEditMode( EM_INSCOL1 );
         }
      }
   } else {
      pDoc->SetEditMode( EM_NORMAL );
   }
   UpdateCursor();
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnEditRow() 
{
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   if (!OnModifyContent ())
   {
      ::MessageBeep(-1);
      return;
   }
   if( pDoc->GetEditMode() == EM_NORMAL )
   {
      pDoc->GetElemList()->UpdatePinStatus();

      if( GetKeyState(VK_SHIFT) < 0) 
      {
         if( pDoc->GetElemList()->CheckSpace(CPoint(0,-2), m_CurPos, m_bLocalSpacing) ) 
         {
            pDoc->SetEditMode( EM_DELROW );
         } else {
            pDoc->SetEditMode( EM_DELROW1 );
         }
      } else {
         if( pDoc->GetElemList()->CheckSpace(CPoint(0, 2), m_CurPos, m_bLocalSpacing) ) 
         {
            pDoc->SetEditMode( EM_INSROW );
         } else {
            pDoc->SetEditMode( EM_INSROW1 );
         }
      }
   } else {
      pDoc->SetEditMode( EM_NORMAL );
   }
   UpdateCursor();
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnEditProperty() 
{
   CElem       *pE;
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   if (!OnModifyContent ())
   {
      ::MessageBeep(-1);
      return;
   }
   if( (pE = pDoc->GetElemList()->SearchElem(m_LogCurPos, m_iZGrid)) != NULL ) 
   {
      UnselectAll();
      pE->SetFlag(EF_SELECT);
      pDoc->GetElemList()->MoveFlaggedToTail(EF_SELECT);
      pDoc->GetElemList()->MoveFlaggedToTail(EF_RUBBLINE);
      pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateSelectionHint);

      switch (pE->GetType())
      {
         case ET_CONIN      :
         case ET_CONOUT     : PropCon    ( (CElemCon*    )pE ); break;

         case ET_LABEL      :
         case ET_JUMP       : PropLab    ( (CElemLab*    )pE ); break;

         case ET_VARIN      :
         case ET_VAROUT     :
         case ET_VARINOUT   : PropVar    ( (CElemVar*    )pE ); break;

         case ET_COMMENT    : PropCmt    ( (CElemCmt*    )pE ); break;
 
         case ET_STEP       : PropStep   ( (CElemStep*   )pE ); break;

         case ET_TRANSITION : PropTran   ( (CElemTran*   )pE ); break;

         case ET_ACTION     : PropAct    ( (CElemAct*    )pE ); break;
         case ET_ACTIONBLOCK: PropActBlk ( (CElemActBlk* )pE ); break;

         case ET_SIMDIV     :
         case ET_ALTDIV     : PropDiv    ( (CElemDiv*    )pE ); break;

         case ET_CONTACT    : PropContact( (CElemContact*)pE ); break;
         case ET_COIL       : PropCoil   ( (CElemCoil*   )pE ); break;

         case ET_LINE       : PropLine   ( (CElemLine *)  pE ); break;
         case ET_POWERRAIL  : PropPower  ( (CElemPower*)  pE ); break;
         case ET_FB         : PropFblk   ( (CElemFblk*)   pE ); break;
         case ET_FUNCTION   : PropFblk   ( (CElemFblk*)   pE ); break;
         case ET_PROCEDURE  : PropFblk   ( (CElemFblk*)   pE ); break;
      }
      pDoc->GetElemList()->PurgeTempVars();
      pDoc->GetElemList()->CreateTempVars(false);  // false: don't copy selection
   }
}


/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnSetPropFBDAction()
{
   CElem       *pE;
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   bool bSTactWarn;

   if (OnModifyContent () && pDoc)
   {       
       if( (pE = pDoc->GetElemList()->SearchElem(m_LogCurPos, m_iZGrid)) != NULL ) 
       {
           UnselectAll();
           pE->SetFlag(EF_SELECT);
           pDoc->GetElemList()->MoveFlaggedToTail(EF_SELECT);
           pDoc->GetElemList()->MoveFlaggedToTail(EF_RUBBLINE);
           pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateSelectionHint);
           
           switch (pE->GetType())
           {
           case ET_ACTION     : 
           case ET_ACTIONBLOCK: 
               // warning for STaction
               bSTactWarn = FALSE;
               if (!pE->ChkFlag(EF_STACTION))
               {
                  pE->SetFlag(EF_STACTION);
                  if (pDoc->GetElemList()->IsElemOverlapped(pE))
                  {
                     bSTactWarn = TRUE;
                  }
                  pE->ClrFlag(EF_STACTION);
                  pDoc->GetElemList()->IsOneOverlapped();
               }
               if (bSTactWarn)
               {
                  if( AfxMessageBox( IDS_GR_STACTION_WARN_YN, MB_ICONSTOP | MB_YESNO ) == IDYES)
                  {                                       
                     break;
                  }
               }
               pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateSelectionHint);
               // change action type
               pDoc->m_Action.StoreState(pDoc->GetElemList());
               SetActionType((CElemAct*)pE,AT_FBD);
               m_NextActionType = EF_LAST;   // for insertion
               break;
           }
       }
   }
}


/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnSetPropLDAction() 
{
   CElem       *pE;
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   bool bSTactWarn;

   if (OnModifyContent () && pDoc)
   {       
       if( (pE = pDoc->GetElemList()->SearchElem(m_LogCurPos, m_iZGrid)) != NULL ) 
       {
           UnselectAll();
           pE->SetFlag(EF_SELECT);
           pDoc->GetElemList()->MoveFlaggedToTail(EF_SELECT);
           pDoc->GetElemList()->MoveFlaggedToTail(EF_RUBBLINE);
           pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateSelectionHint);
           
           switch (pE->GetType())
           {
           case ET_ACTION     : 
           case ET_ACTIONBLOCK: 
               // warning for STaction
               bSTactWarn = FALSE;
               if (!pE->ChkFlag(EF_STACTION))
               {
                  pE->SetFlag(EF_STACTION);
                  if (pDoc->GetElemList()->IsElemOverlapped(pE))
                  {
                     bSTactWarn = TRUE;
                  }
                  pE->ClrFlag(EF_STACTION);
                  pDoc->GetElemList()->IsOneOverlapped();
               }
               if (bSTactWarn)
               {
                  if( AfxMessageBox( IDS_GR_STACTION_WARN_YN, MB_ICONSTOP | MB_YESNO ) == IDYES)
                  {                                       
                     break;
                  }
               }
               pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateSelectionHint);
               // change action type
               pDoc->m_Action.StoreState(pDoc->GetElemList());
               SetActionType((CElemAct*)pE,AT_LD);
               m_NextActionType = EF_LDACTION;  // for insertion
               break;
           }
       }
   }
}


/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnSetPropSTAction()
{
   CElem       *pE;
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   bool bSTactWarn;

   if (OnModifyContent () && pDoc)
   {       
       if( (pE = pDoc->GetElemList()->SearchElem(m_LogCurPos, m_iZGrid)) != NULL ) 
       {
           UnselectAll();
           pE->SetFlag(EF_SELECT);
           pDoc->GetElemList()->MoveFlaggedToTail(EF_SELECT);
           pDoc->GetElemList()->MoveFlaggedToTail(EF_RUBBLINE);
           pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateSelectionHint);

           switch (pE->GetType())
           {
           case ET_ACTION     : 
           case ET_ACTIONBLOCK: 
               // warning for STaction
               bSTactWarn = FALSE;
               if (!pE->ChkFlag(EF_STACTION))
               {
                  pE->SetFlag(EF_STACTION);
                  if (pDoc->GetElemList()->IsElemOverlapped(pE))
                  {
                     bSTactWarn = TRUE;
                  }
                  pE->ClrFlag(EF_STACTION);
                  pDoc->GetElemList()->IsOneOverlapped();
               }
               if (bSTactWarn)
               {
                  if( AfxMessageBox( IDS_GR_STACTION_WARN_YN, MB_ICONSTOP | MB_YESNO ) == IDYES)
                  {                                       
                     break;
                  }
               }
               pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateSelectionHint);
               // change action type
               pDoc->m_Action.StoreState(pDoc->GetElemList());
               SetActionType((CElemAct*)pE,AT_ST);
               m_NextActionType = EF_STACTION;  // for insertion
               break;
           }
       }
   }
}


/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnEditName() 
{ 
   if (!OnModifyContent ())
   {
      ::MessageBeep(-1);
      return;
   }
   EventEnter(); 
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnEditInstance() 
{
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   CElem  *pE;
   CRect   r;
   CPoint  p;

   if (!OnModifyContent ())
   {
      ::MessageBeep(-1);
      return;
   }
   if( ((pE = pDoc->GetElemList()->SearchElem(m_LogCurPos, m_iZGrid)) != NULL) && 
        (pE->GetType() == ET_FB) ) 
   {
      UnselectAll();
      pE->SetFlag(EF_SELECT);
      pDoc->GetElemList()->MoveFlaggedToTail(EF_SELECT);
      pDoc->GetElemList()->MoveFlaggedToTail(EF_RUBBLINE);
      pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateSelectionHint);

      CPoint point = pE->GetOrigin()+CSize(pE->GetSize().cx/2,1);    // calc safe instance name point
      CText &text = pE->GetTextAt( GridToDoc(point), p, m_iZGrid);   // search using doc coords!
      pE->GetTextRect(-2, r, m_iZGrid);
      if (text.m_Flag.Chk(TF_EDITABLE))
      {
         DocToGrid(r);
         EditNameStart(text, r, pE);
         pDoc->SetEditMode(EM_EDITPROP);
      }
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnEditAutoSize2(WPARAM wParam, LPARAM /*lParam*/)
{
   if ((wParam == (UINT)VK_RETURN) &&
//     (lParam == (LONG)m_pPropEdit) &&
       (m_pPropEdit != NULL) && 
       (m_pPropText != NULL))
   {
      m_pPropText->StrRef().Empty();
      m_pPropEdit->GetWindowText( m_pPropText->StrRef() );
      OnEditAutoSize();
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnEditAutoSize() 
{
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   POSITION  pos;
   CElem    *pE;
   CPoint    dp, tp, po;
   CSize     ds, ts, so;
   CRect     r;
   bool      bAutoSized=false;

   if (!OnModifyContent () || 
         (pDoc == NULL) ||
       !((pDoc->GetEditMode() == EM_NORMAL  ) || 
         (pDoc->GetEditMode() == EM_EDITPROP)))
   {
      ::MessageBeep(-1);
      return;
   }
   
   if(pDoc->GetElemList()->DropOutSelElems())
      pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateWindowHint);

   for (pos = pDoc->GetElemList()->GetHeadPosition(); pos != NULL; )
   {
      pE = pDoc->GetElemList()->GetNext(pos);
      if (pE && 
          pE->ChkFlag(EF_SELECT))
      {
         if ((m_pPropElem == pE) && 
             (m_pPropText != NULL))
         {
            m_pPropText->StrRef().Empty();
            m_pPropEdit->GetWindowText( m_pPropText->StrRef() );
         }
                   
         po = pE->GetOrigin();  // save original position and size ..
         so = pE->GetSize();
         bool bResized = pE->SetAutoSize(dp, ds);
         ASSERT((dp.x  & 1) == 0);
         ASSERT((dp.y  & 1) == 0);
         ASSERT((ds.cx & 1) == 0);
         ASSERT((ds.cy & 1) == 0);

         if (bResized)
         {
            // check the result ..
            while (pDoc->GetElemList()->IsElemOverlapped(pE,false) && bResized)
            {
               if (dp.x  < 0) tp.x  = 2; else if (dp.x  > 0) tp.x  = -2; else tp.x  = 0;
               if (dp.y  < 0) tp.y  = 2; else if (dp.y  > 0) tp.y  = -2; else tp.y  = 0;
               if (ds.cx < 0) ts.cx = 2; else if (ds.cx > 0) ts.cx = -2; else ts.cx = 0;
               if (ds.cy < 0) ts.cy = 2; else if (ds.cy > 0) ts.cy = -2; else ts.cy = 0;
               
               if (ts.cx == 0 && ts.cy == 0)
               {
                  bResized = false;
               }

               if (pE->Resize(tp.x, tp.y, ts.cx, ts.cy) == false)
               {
                  pE->SetOrigin(po);
                  pE->SetSize(so);
                  bResized = false;
                  break;
               }
            } 
            if (bResized)
            {
               if ((m_pPropEdit != NULL) &&
                   (m_pPropElem != NULL) && 
                   (pDoc->GetEditMode() == EM_EDITPROP))
               {
                  m_pPropEdit->GetWindowRect(&r);
                  ScreenToClient(&r);
                  r += GridToClient(dp);
                  ds = GridToClient(ds);
                  r.right  += ds.cx;
                  r.bottom += ds.cy;
                  m_pPropEdit->SetWindowPos(NULL, r.left, r.top, r.Width(), r.Height(), SWP_NOZORDER);
                  m_pPropEdit->SetSel(0, 0, FALSE);
                  m_pPropEdit->SetSel(m_pPropEdit->LineLength(-1), m_pPropEdit->LineLength(-1), FALSE);
               }
               bAutoSized = true;
            }
         }
      }
   }

   // close operation like a resize
   pDoc->GetElemList()->ConnectAllElems();
   
   if(pDoc->GetElemList()->DropInSelectedElems())  // adjust possible DropIn - Overlappings
      pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateWindowHint);
   
   pDoc->GetElemList()->IsOneOverlapped();

   if (bAutoSized)
   {
      pDoc->m_Action.StoreState(pDoc->GetElemList());  // op. ok -> store
      pDoc->SetModifiedFlag(pDoc->m_Action.StateChangedQ());
   }
   pDoc->UpdateAllViews(NULL);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnEditLine() 
{
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   if (!OnModifyContent ())
   {
      ::MessageBeep(-1);
      return;
   }
   if( pDoc->GetEditMode() == EM_LINE_START ) 
   {
      pDoc->SetEditMode( EM_NORMAL     );
   } 
   else if (pDoc->GetEditMode() == EM_NORMAL) 
   {
      pDoc->SetEditMode( EM_LINE_START );
   }
   UpdateCursor();
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnUpdateEditLine(CCmdUI* pCmdUI) 
{
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);

	pCmdUI->SetCheck( (pDoc->GetEditMode() == EM_LINE      ) || 
                     (pDoc->GetEditMode() == EM_LINE_START) );
   pCmdUI->Enable  ( (pDoc->GetEditMode() == EM_LINE_START) || 
                     (pDoc->GetEditMode() == EM_NORMAL && !CheckReadonly()    ) );
}

/*func*------------------------------------------------------------------------
  processes spacing 
  in : mode: requested direction
  out:- (if checkSlots() returns true: performs operation)
-----------------------------------------------------------------------------*/
void CGrEditorView::OnSpacing(EDI_MODE mode)
{
   CWaitCursor wc;
   CPoint p;
   bool bCan = false;
   CGrEditorDoc* pDoc = GetDocument();
   if(pDoc)
   {
      m_bLocalSpacing = !( GetKeyState(VK_CONTROL) < 0); // global operation
      bCan = CheckSlots();

      switch(mode)
      {
         case EM_INSCOL :
            p = CPoint( 2, 0); 
            break;
         
         case EM_DELCOL :
            p = CPoint(-2, 0); 
            break;
         
         case EM_INSROW : 
            p = CPoint( 0, 2); 
            break;
         
         case EM_DELROW : 
            p = CPoint( 0,-2); 
            break;
      }

      if(bCan)
      {
         pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateWindowHint);
         if ( pDoc->GetElemList()->InsertSpace(p, m_CurPos, m_bLocalSpacing) )
         {
            pDoc->GetElemList()->ConnectAllElems();
            CheckSlots();
         }
      
         if( pDoc->GetElemList()->IsOneOverlapped() ) 
         {
            pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateMarkedElemHint);
            if(p.x<0 || p.y<0)
            {
               GetCursorPos(&p);
               AfxMessageBox( IDS_GR_ERR_INVALID_POS, MB_ICONSTOP | MB_OK);
               OnDeleteSTActions(false);
               pDoc->m_Action.ReloadLastState(pDoc->GetElemList());
               pDoc->SetModifiedFlag(pDoc->m_Action.StateChangedQ());
               pDoc->GetElemList()->AddAllElemsToPinTable();
               pDoc->GetElemList()->bValidPos = !pDoc->GetElemList()->IsOneOverlapped();
               pDoc->UpdateAllViews(NULL);
               SetCursorPos(p.x,p.y);
            }            
            GrReleaseCapture();
         }

         pDoc->m_Action.StoreState(pDoc->GetElemList());  // op. ok -> store
         pDoc->SetModifiedFlag(pDoc->m_Action.StateChangedQ());
         pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateWindowHint);
         CheckSlots();
      }
   }
}

/*func*------------------------------------------------------------------------
  toggles stop point if the grid cursor locates at any fixed line
  in :- 
  out:-
-----------------------------------------------------------------------------*/
void CGrEditorView::OnToggleFixPoint()
{
   CGrEditorDoc* pDoc = GetDocument();
   if(pDoc)
   {
      pDoc->ToggleFixPoint(m_CurPos);    // 
      pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateMarkedElemHint);
   }
}

/*func*------------------------------------------------------------------------
  process events mouse:lbuttondown or keyboard:space down
  in :- 
  out:-
-----------------------------------------------------------------------------*/
void CGrEditorView::EventDown()
{
   CPoint       p;
   CPin        *pPin;
   CElem       *pTE;
   CRect        rect;
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   m_bLocalSpacing = !( GetKeyState(VK_CONTROL) < 0); // global operation

 	switch ( pDoc->GetEditMode() ) 
   {
      case EM_NORMAL:  
          if(CheckReadonly())
          {
              // try to select ONE element
              pTE = pDoc->GetElemList()->SearchElem(m_LogCurPos,m_iZGrid);
              UnselectAll();

              if (pTE != NULL) 
              {
                 pTE->SetFlag(EF_SELECT);
                 pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateSelectionHint);
              }
              break;
          }

         // remove emphasizes ..
         if( !pDoc->m_RegionError.IsRectEmpty() )
         {
            pDoc->m_RegionError.SetRectEmpty();
            pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateWindowHint);
         }

         if(!pDoc->GetElemList()->bValidPos)
         {
            pDoc->GetElemList()->bValidPos = !pDoc->GetElemList()->IsOneOverlapped();
         } 
         else 
         {
            if( pDoc->GetElemList()->ClearFlags(EF_EMPHASIZE) )
            {  // and update view, if necessary ..
               pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateWindowHint);
            }
         }
         // remove find results
         if (m_SearchDat.m_FindCur != CPoint(0,0))
         {
            pTE = pDoc->GetElemList()->SearchElem(m_SearchDat.m_FindCur,1);
            if( pTE )
            {
               pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateDrawElemHint, pTE);
            }
            m_SearchDat.m_FindCur   = CPoint(0,0);
            m_SearchDat.m_iFindElem = INT_MIN;
            m_SearchDat.m_iFindPos  = 0;
            m_SearchDat.m_iFindLen  = 0;
         }

         // mark TempVar positions
         pDoc->GetElemList()->FlagTempVarPins();

         pTE = pDoc->GetElemList()->SearchElem(m_LogCurPos,m_iZGrid);
         // look for element at actual position
         GrSetCapture();
         // check start of line mode ..
         if( GetKeyState(VK_MENU) < 0 )
         {
            if (pDoc->GetElemList()->IsOneSelected())
            {
                 UnselectAll();
//               pDoc->m_Action.StoreState(pDoc->GetElemList());  
//               pDoc->SetModifiedFlag(pDoc->m_Action.StateChangedQ());
            }

            if( (pTE == NULL) || pTE->IsConnectable(m_CurPos) )
            {
               goto m_line_start;
            }
         }else
         {
            // select, deselect of drag mode ..
            if( pTE != NULL)
            {
               if (pTE->ChkFlag(EF_SELECT) &&
                   pTE->IsResizeRect( m_LogCurPos, m_iZGrid)) 
               {  // resize element
                  if(OnModifyContent())
                  {
                     pDoc->SetEditMode( EM_RESIZE );
                     pTE->GetResizePos(m_eResizeMode, m_CurPos);
                     m_BeforeOpPos = m_StartPos = m_CurPos;
                  }
               } else {
                  // ST-Action to activate ?
                  if (pTE && pTE->ChkFlag(EF_STACTION))
                  {
                     pTE->GetTextRect(-1, rect, m_iZGrid);
                     if (rect.PtInRect(m_LogCurPos))
                     {
                        CText *text = &pTE->GetTextAt(m_LogCurPos, p, m_iZGrid);
                        if (text->m_Flag.Chk(TF_EDITABLE))
                        {
                           GrReleaseCapture();
                           DocToGrid(rect);
                           if(OnModifyContent())
                           {
                               EditNameStart(*text, rect, pTE);
                               pDoc->SetEditMode(EM_EDITPROP);
                           }
                        }
                        break;
                     }
                  }
                  
                  pDoc->SetEditMode( EM_DRAG_MOVE );
                  m_BeforeOpPos = m_StartPos = m_CurPos;
               }
               pDoc->GetElemList()->MoveFlaggedToTail(EF_SELECT);
               pDoc->GetElemList()->MoveFlaggedToTail(EF_RUBBLINE);
            } 
            else 
            {
               if (  pDoc->GetElemList()->IsOneSelected() &&
                   !(GetKeyState(VK_CONTROL) < 0))
               {
                   UnselectAll();
//                  pDoc->m_Action.StoreState(pDoc->GetElemList());  
//                  pDoc->SetModifiedFlag(pDoc->m_Action.StateChangedQ());
               }

               pDoc->SetEditMode( EM_SELECT );
               m_BeforeOpPos = m_StartPos = m_EndPos = m_LogCurPos;
            }
         } 
         break;

      case EM_RESIZE_START:
         pTE = pDoc->GetElemList()->SearchElem(m_LogCurPos,m_iZGrid);
         if (pTE == NULL)
         {   // no hit ..
            pDoc->SetEditMode( EM_NORMAL );
         } 
         else 
         {
            if( !pTE->ChkFlag(EF_SELECT) )
            {
               if( !(GetKeyState(VK_CONTROL) < 0) ) 
               {
                  UnselectAll();
               } 
               pTE->SetFlag(EF_SELECT);
               pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateSelectionHint);
            }
            // now move/resize selected elements ..
            GrSetCapture();
            pDoc->SetEditMode( EM_RESIZE_DIR );
            pDoc->GetElemList()->MoveFlaggedToTail(EF_SELECT);
            pDoc->GetElemList()->MoveFlaggedToTail(EF_RUBBLINE);
            m_BeforeOpPos = m_StartPos = m_CurPos;
         }
         break;

      case EM_INSERT:
         m_bIgnoreForFBCall = false;
         // fall through ..
      case EM_SELECT:
      case EM_RESIZE:
      case EM_MOVE  :
         GrSetCapture();
//m_SignalState.Init(pDoc->GetElemList());
         break;

      case EM_LINE_START:
        m_line_start:
         if( (pTE = pDoc->GetElemList()->SearchElem(m_LogCurPos, m_iZGrid)) != NULL ) 
         {
            pPin = pTE->SearchConn(m_CurPos);
         } 
         if( (pTE == NULL) || pTE->IsConnectable(m_CurPos) )
         {
            UnselectAll();
            EditLineStart(m_CurPos); // begin a new line
            pDoc->SetEditMode( EM_LINE );
            GrSetCapture();
         }
         break;

      case EM_LINE:
         if( pDoc->GetLineMode() == LDM_ORTHO )
         {
            EditLineStop();          // end the actual line
            EditLineStart(m_EndPos); // begin a new line
         }
         break;

      case EM_INSCOL :
      case EM_DELCOL :
      case EM_INSROW : 
      case EM_DELROW : 
      case EM_INSCOL1 :
      case EM_DELCOL1 :
      case EM_INSROW1 : 
      case EM_DELROW1 : 
         OnSpacing(pDoc->GetEditMode());
         break;    

      case EM_EDITPROP :
         EditNameStop(false);
         pDoc->SetEditMode(EM_NORMAL);
         pDoc->UpdateAllViews(NULL);
         break;

      case EM_DRAG_MOVE:
          // try to select ONE element
          pTE = pDoc->GetElemList()->SearchElem(m_LogCurPos,m_iZGrid);
          UnselectAll();

          if (pTE != NULL) 
          {
             pTE->SetFlag(EF_SELECT);
             pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateSelectionHint);
          }
          break;

      case EM_MONITOR :
         {
	         CWatchpoint *pWP;
	         CPoint grabPosition;

	         pWP = HitWatchpoint( m_LogCurPos, grabPosition );
	         if (pWP && 
               (pWP->GetPosition().GetPosQual() != EDP_GRLDBOX))
	         {
               if (pDoc->RaiseWatchpoint (pWP))
               {
                  pDoc->UpdateAllViews(NULL);
               }
               
               GrSetCapture();
	            m_pTrackedWP     = pWP;
   	         m_WPGrabPosition = grabPosition;

               break;
	         }

            // try to select ONE element
            pTE = pDoc->GetElemList()->SearchElem(m_LogCurPos,m_iZGrid);
            UnselectAll();

            if (pTE != NULL) 
            {
               pTE->SetFlag(EF_SELECT);
               pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateSelectionHint);
            }

            // ST-Action to activate ?
            if (pTE && pTE->ChkFlag(EF_STACTION))
            {
               CSTEditorView *pView;

               if (m_STViewMap.Lookup(((CElemAct*)pTE)->GetID(), pView) && pView)
               {
                  pTE->GetTextRect(-2, rect, m_iZGrid);
                  if (rect.PtInRect(m_LogCurPos))
                  {
                     pView->EnableWindow(TRUE);
                     pView->ShowWindow(SW_SHOW);
                     m_pSEC_Edit = pView;
                     CFrameWnd* pFrame = GetParentFrame();
                     if (pFrame)
                     {
                        pFrame->SetActiveView(pView);
                     }
                     pView->SetFocus();
                  }
               }
            } else {
               if (m_pSEC_Edit)
               {
                  m_pSEC_Edit->GetEdit()->m_crBackground = GDIHelper::GetColor(true, CO_BACKGD);
                  m_pSEC_Edit->EnableWindow(FALSE);
                  m_pSEC_Edit->ShowWindow(SW_HIDE);
                  m_pSEC_Edit = NULL;
               }
               pDoc->UpdateAllViews(NULL);
            }
         } 
         break;
   }
// MoveViewToCursor();
   UpdateCursor(false);
}

/*func*------------------------------------------------------------------------
  process events mouse:lbuttonup or keyboard:space up
  in :- 
  out:-
-----------------------------------------------------------------------------*/
void CGrEditorView::EventUp()
{
   int          OldRop;
   CPen        *OldPen; 
   CBrush      *OldBrush, Brush;
   CRect        r;      // rect for update view
   CPoint       p;
   CElem       *pTE;
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   if (GetCapture() != this) return; 

   CClientDC dc(this);
   OnPrepareDC(&dc, NULL);

   pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateMarkedElemHint);
   pDoc->GetElemList()->ClearFlags(EF_EMPHASIZE);

   switch ( pDoc->GetEditMode() ) 
   {
      case EM_SELECT:
         if( Brush.CreateStockObject(NULL_BRUSH) )
         {
            OldRop   = dc.SetROP2(R2_NOT);
            OldBrush = dc.SelectObject(&Brush);
            OldPen   = dc.SelectObject(GDIHelper::GetPen(m_bDrawOnScreen, PE_NORMAL));
            // delete old rect
            dc.Rectangle( m_StartPos.x, m_StartPos.y, m_EndPos.x, m_EndPos.y);
            dc.SetROP2(OldRop);
            dc.SelectObject(OldBrush);
            dc.SelectObject(OldPen  );
         }

         m_StartPos = DocToGrid(m_StartPos);
         m_EndPos   = DocToGrid(m_EndPos  );
         r = CRect(m_StartPos, m_EndPos);
         pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateSelectionHint);
         pDoc->GetElemList()->ToggleFlagsByRect(EF_SELECT, r);
         pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateSelectionHint);
         // resize rect sometimes on screen ?!
         pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateWindowHint);
         pDoc->SetEditMode( EM_NORMAL );
         GrReleaseCapture();
         UpdateCursor();
         break; 

      case EM_LINE:
         EditLineStop();          // end the actual line
         GrReleaseCapture();
         pDoc->SetEditMode( EM_LINE_START );
         pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateWindowHint);
         UpdateCursor();
         break;

      case EM_DRAG_MOVE:
         pTE = pDoc->GetElemList()->SearchElem(m_LogCurPos,m_iZGrid);
         if( pTE )
         {
            pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateSelectionHint);
            if( pTE->ChkFlag(EF_SELECT) )
            {
               if( GetKeyState(VK_CONTROL) < 0) 
               {  // unselect element
                  pTE->ClrFlag(EF_SELECT);
               }
            } else {
               if( GetKeyState(VK_CONTROL) >= 0) 
               {  // unselect all elements
                  UnselectAll();
               } 
               // select element, start move -> EM_MOVE
               pTE->SetFlag(EF_SELECT);
            }
            pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateSelectionHint);
            pDoc->SetEditMode( EM_NORMAL );
            GrReleaseCapture();
//            pDoc->m_Action.StoreState(pDoc->GetElemList());  // op. ok -> store
//            pDoc->SetModifiedFlag(pDoc->m_Action.StateChangedQ());
         }
         DetectSignalChange();
         break;

      case EM_INSERT:
         if (m_bIgnoreForFBCall) 
         {
            m_bIgnoreForFBCall = false;
            break;
         }
         // else -> fall through !
      case EM_MOVE  :
      case EM_RESIZE:
         {
            CWaitCursor wc;

            InvalidateFixPoints();
            InvalidateRubber();
            pDoc->GetElemList()->InitMoveFixPoints();
            pDoc->GetElemList()->DoneMoveFixPoints();
            pDoc->GetElemList()->PurgeOpenRubberLines(EF_LAST);
            
            pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateMarkedElemHint);
            pDoc->GetElemList()->ClearFlags(EF_EMPHASIZE);

            if( pDoc->GetElemList()->IsOneOverlapped(false, EF_SELECT) ) 
            {
               pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateMarkedElemHint);

               GetCursorPos(&p);
/* <--
               if( AfxMessageBox( IDS_GR_ERR_INVALID_POS, MB_ICONSTOP | MB_RETRYCANCEL) == IDCANCEL) 
               {
                  pDoc->SetEditMode( EM_NORMAL );  // abort move

                  m_bFirstMove = true;      // EM_MOVE => EM_NORMAL 
                  OnDeleteSTActions(false);
                  pDoc->m_Action.ReloadLastState(pDoc->GetElemList());
                  pDoc->SetModifiedFlag(pDoc->m_Action.StateChangedQ());
                  pDoc->GetElemList()->AddAllElemsToPinTable();
                  pDoc->GetElemList()->bValidPos = !pDoc->GetElemList()->IsOneOverlapped();
                  pDoc->UpdateAllViews(NULL);

                  GrReleaseCapture();
               } else {
                  GrSetCapture();
               }
replaced during DCS 413 by: 
   --> */      
               ::MessageBeep(-1); // prevent user for new selection
               GrSetCapture();
/* -- */
               SetCursorPos(p.x, p.y);
            } else {
               pDoc->SetEditMode( EM_NORMAL );
               m_bFirstMove  = true;      // EM_MOVE => EM_NORMAL

               pDoc->GetElemList()->ConnectAllElems();

               if(pDoc->GetElemList()->DropInSelectedElems())  // adjust possible DropIn - Overlappings
                  pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateWindowHint);

               if(!pDoc->GetElemList()->bValidPos)
               {
                  pDoc->GetElemList()->bValidPos = !pDoc->GetElemList()->IsOneOverlapped();
               }

               if(pDoc->GetElemList()->ReRouteDone()) // route temp rubber lines
               {
                  pDoc->GetElemList()->ConnectAllElems();
                  pDoc->GetElemList()->InitMoveFixPoints();
                  pDoc->GetElemList()->DoneMoveFixPoints();

                  pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateWindowHint);
               }else
               {
                  pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateMarkedElemHint);
                  pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateSelectionHint);
               }
               GrReleaseCapture();

               pDoc->m_Action.StoreState(pDoc->GetElemList());  // op. ok -> store
               pDoc->SetModifiedFlag(pDoc->m_Action.StateChangedQ());
            }

            DetectSignalChange();

         }
         MoveViewToCursor();
         UpdateCursor(false);
         break; 

      case EM_MONITOR :
         {
         CPrgWPClientData *pClientData;

	      if( m_pTrackedWP )
	      {
            pClientData = (CPrgWPClientData *)m_pTrackedWP->GetClientData ();
	         ASSERT (pClientData != NULL);
	         if (pClientData != NULL)
	         {
		         CRect rect;
		         long lDisplayX;
		         long lDisplayY;
		         long lDisplayOffsetX;
		         long lDisplayOffsetY;

		         rect = pClientData->GetArea ();

		         lDisplayX = rect.left / m_iZGrid; // m_iCharWidth;
		         lDisplayY = rect.top  / m_iZGrid; // m_iLineHeight;

		         lDisplayOffsetX = rect.left - lDisplayX * m_iZGrid; // m_iCharWidth;
		         lDisplayOffsetY = rect.top  - lDisplayY * m_iZGrid; // m_iLineHeight;

		         pClientData->SetDisplay (lDisplayX, lDisplayY + 1, lDisplayOffsetX, lDisplayOffsetY);
            };

            GrReleaseCapture();
	         m_pTrackedWP = NULL;
	      }
      } break;
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::EventEnter()
{
   CElem         *pE;
   CRect          r;
   CPoint         p;
   CGrEditorDoc  *pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   switch ( pDoc->GetEditMode() ) {
      case EM_MONITOR: 
         if (OnToggleLadder())
         {
            break;
         }
         // OnModifyContent() ...
         // fall through !

      case EM_NORMAL:
         if (!OnModifyContent ())
         {
            ::MessageBeep(-1);
            break;
         }
         pE = pDoc->GetElemList()->SearchElem(m_LogCurPos, m_iZGrid);

         if( pE != NULL ) 
         {
            if(pE->GetType() != ET_LINE)
            {
               UnselectAll();
               pE->SetFlag(EF_SELECT);
            }
            pDoc->GetElemList()->MoveFlaggedToTail(EF_SELECT);
            pDoc->GetElemList()->MoveFlaggedToTail(EF_RUBBLINE);
            pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateSelectionHint);

            if (pE->SearchTextRect(m_LogCurPos, r, m_iZGrid))
            {
               CText *text = &pE->GetTextAt(m_LogCurPos, p, m_iZGrid);
               if (text->m_Flag.Chk(TF_EDITABLE))
               {
                  DocToGrid(r);
                  EditNameStart(*text, r, pE);
                  pDoc->SetEditMode(EM_EDITPROP);
               }
            }
            else if(pE->GetType() == ET_LINE)  // SELECT Signal extention
            {
               bool bRem = pE->ChkFlag(EF_COMMENT);
               if(!pDoc->GetElemList()->SelectSignal(pE->GetOrigin(), pE->ChkFlag(EF_SELECT),bRem))
                  pDoc->GetElemList()->SelectSignal(pE->GetOrigin()+pE->GetSize(), pE->ChkFlag(EF_SELECT),bRem);
               pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateSelectionHint);
            }
         }
         break;

      case EM_LINE_START:
         pDoc->SetEditMode( EM_NORMAL );
         pDoc->UpdateAllViews(NULL);
         UpdateCursor();
         break;

      case EM_LINE:
         if(m_EditLine)
         {
            pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateDrawElemHint, m_EditLine);
         } else {
            pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateWindowHint, NULL);
         }
         EditLineStop();

         pDoc->SetEditMode( EM_LINE_START );
         GrReleaseCapture();
         pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateMarkedElemHint);
         UpdateCursor();
         break;

      case EM_EDITPROP   :
         EditNameStop(false);
         pDoc->SetEditMode(EM_NORMAL);
         pDoc->UpdateAllViews(NULL);
         UpdateCursor();
         break;

      // handle spacing to handle DblClk on fast spacing
      case EM_INSCOL :
      case EM_DELCOL :
      case EM_INSROW : 
      case EM_DELROW : 
      case EM_INSCOL1 :
      case EM_DELCOL1 :
      case EM_INSROW1 : 
      case EM_DELROW1 : 
         OnSpacing(pDoc->GetEditMode());
         break;    

      default: break;
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::EventCancel()
{
   CGrEditorDoc *pDoc = GetDocument();
   ASSERT(pDoc);

   if (pDoc)
   {  // remove emphasizes
      if( pDoc->GetElemList()->ClearFlags(EF_EMPHASIZE) )
      {  // and update view, if necessary ..
         pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateWindowHint);
      }
   }
   CDbgEditorView::EventCancel();
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnEndPropEdit()
{
   CGrEditorDoc *pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   
   if (pDoc->GetEditMode() == EM_EDITPROP)
   {
      pDoc->SetEditMode( EM_NORMAL );
   }
   pDoc->UpdateAllViews(NULL);
   UpdateCursor();
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnMouseMove(UINT nFlags, CPoint point) 
{
   int          OldRop;
   CPen        *OldPen; 
   CBrush      *OldBrush, Brush;
   bool         bSucc;
   CPoint       p, p2, GridPos;
   CElem       *pTE;
   CRect        r;
   CString      strName;
	CFont       *pOldFont;

   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   if( (GetCapture() != NULL) && (GetCapture() != this)) return; 

   {  
       // optimizing mouse move events ..
      MSG msg;    
      while ( ::PeekMessage( &msg, NULL, WM_MOUSEMOVE, WM_MOUSEMOVE, PM_REMOVE ) )     
      { 
         point.x = LOWORD(msg.lParam);  // horizontal position of cursor 
         point.y = HIWORD(msg.lParam);  // vertical position of cursor 
         nFlags  = msg.wParam;
      }
   }

   CClientDC dc(this);
   OnPrepareDC(&dc, NULL);
   pOldFont = dc.SelectObject (&m_FixedFont);

   // if capture is activ - must should correct the coordinate
   if( point.x > 0x8000) point.x = (LONG)((SHORT)&point.x);
   if( point.y > 0x8000) point.y = (LONG)((SHORT)&point.y);
   // watch current mouse (cursor) position for keyboard handling
   GetClientRect(&r);
   p  = point;
   p2 = ClientToDoc (point);
   
   if      (p.x < 0       ) m_LogCurPos.x += (m_bTick ? max(-m_LogCurPos.x, -m_iTickDelta) : 0);
   else if (p.x > r.right ) m_LogCurPos.x += (m_bTick ? min( MAX_XGRID * m_iZGrid - r.right, m_iTickDelta) : 0);
   else                     m_LogCurPos.x  = p2.x;

   if      (p.y < 0       ) m_LogCurPos.y += (m_bTick ? max(-m_LogCurPos.y, -m_iTickDelta) : 0);
   else if (p.y > r.bottom) m_LogCurPos.y += (m_bTick ? min(MAX_YGRID * m_iZGrid - r.bottom, m_iTickDelta) : 0);
   else                     m_LogCurPos.y  = p2.y;

   GridPos = DocToGrid(m_LogCurPos);
   SetCurPos(GridPos);
   m_bTick  = false;

   switch ( pDoc->GetEditMode() ) {
      case EM_NORMAL:

         pTE = pDoc->GetElemList()->SearchElem(m_LogCurPos,m_iZGrid);
         if( pTE )
         {
            // check for resize corners (-> updatecursor)
            m_eResizeMode = pTE->GetResizeType(m_LogCurPos, m_iZGrid);
         }
         break;

      case EM_DRAG_MOVE:
         m_EndPos = m_CurPos;
         if( m_StartPos != m_EndPos && OnModifyContent())
         {
            pTE = pDoc->GetElemList()->SearchElem(GridToDoc(m_StartPos),m_iZGrid);
            if( (GetKeyState(VK_CONTROL) < 0) )
            {
               if (pTE && !pTE->ChkFlag(EF_SELECT))
               {
                  pTE->SetFlag(EF_SELECT);
                  pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateDrawElemHint, pTE);
               }

               // begin drag and drop operation
               COleDataSource* pDataSource = NULL;
               try
               {
                  CXMLDragItem DragItem;

                  DragItem.m_lObjClass = -1;          // object class id -> see cedb.h
                  pDoc->GetElemList()->GenXMLData( DragItem.m_strSource, pDoc->GetPathName() ); // source stream

                  CSharedFile GlobFile;
                  CArchive  ar(&GlobFile, CArchive::store);
                  DragItem.Serialize(ar);
                  ar.Close();

                  pDataSource = new COleDataSource;
                  pDataSource->CacheGlobalData(m_uiCBFormatXMLId, GlobFile.Detach());
                  pDataSource->SetClipboard();

		            // drag the object
                  m_bDropSource = true;
		            if( (pDataSource->DoDragDrop( DROPEFFECT_COPY | DROPEFFECT_MOVE) == DROPEFFECT_MOVE) &&
                      m_bDropSource )
		            {
                     OnEditClear();
                  }
                  m_bDropSource = false;

                  // finish operation ..
                  pDoc->SetEditMode( EM_NORMAL );
                  pDoc->UpdateAllViews(NULL);
                  GrReleaseCapture();
               }
               catch(CException *e) 
               {
                  delete pDataSource;
                  e->Delete();
               }
            }else 
            {
               if (pTE && !pTE->ChkFlag(EF_SELECT))
               {
                  if (pDoc->GetElemList()->IsOneSelected())
                  {
                     UnselectAll();
                  }

                  pTE->SetFlag(EF_SELECT);
//                  pDoc->m_Action.StoreState(pDoc->GetElemList());  
//                  pDoc->SetModifiedFlag(pDoc->m_Action.StateChangedQ());
                  pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateDrawElemHint, pTE);
               }

               pDoc->SetEditMode( EM_MOVE );
               pDoc->GetElemList()->MoveFlaggedToTail(EF_SELECT);
               pDoc->GetElemList()->MoveFlaggedToTail(EF_RUBBLINE);
               pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateSelectionHint);
               goto m_move;
            }
         }
    	   if (!m_NoAutoScroll && !CheckReadonly()) MoveViewToCursor();
         break;

      case EM_RESIZE:
      case EM_MOVE  :
      case EM_INSERT:
        m_move:
         m_EndPos = m_CurPos;
         if( m_StartPos != m_EndPos && OnModifyContent())
         {
//            bool bUpdateGroupSelection = m_bFirstMove;

            // copy selection from FBLKs only at 1st move (except in resize mode)
            pDoc->GetElemList()->CreateTempVars(m_bFirstMove && pDoc->GetEditMode() != EM_RESIZE);

            pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateSelectionHint);
            pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateMarkedElemHint);
            pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateRubberHint);
            pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateFixPointsHint);

            pDoc->GetElemList()->PurgeTempVars();  // create temp vars (if necessary/possible)
            
            p = CPoint(m_EndPos - m_StartPos);
            
            bool bDropOutPurge = false;
            
            if( m_bFirstMove )
            {
               CWaitCursor wc;
               if( pDoc->GetEditMode() == EM_MOVE )
               {
                  pDoc->GetElemList()->UpdateGroupSelect();
                  pDoc->GetElemList()->InitMoveFixPoints();
               }
               m_bFirstMove = false;
               if (pDoc->GetEditMode() != EM_INSERT)
               {
                  //---------------- drop out as default (+SHIFT key: reroute)   (see: OnKeyMove(...)!!!)
                  if(/*!*/(GetKeyState(VK_SHIFT) < 0)) // /*no*/ shift key pressed
                  {
                     pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateWindowHint);
                     pDoc->GetElemList()->ReRouteDeconnect(pDoc->GetReconMode(),
                        pDoc->GetElemList()->GetIECLanguange(m_CurPos, 1) == LANG_FBD);
                  }else
                  {
                     // no rubber line insertion (instead of: drop out, if possible)
                     if(pDoc->GetElemList()->DropOutSelElems())
                     {
                        bDropOutPurge = true;
                        pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateWindowHint);
                     }
                     pDoc->GetElemList()->SignalChangeInit(true);
                  }
               }
               pDoc->GetElemList()->UpdatePinStatus();
            }

            // perform operation itself
            if( pDoc->GetEditMode() == EM_RESIZE )
            {
               bSucc = pDoc->GetElemList()->ResizeSelElems( p, m_eResizeMode);
               if(!bSucc)
               {
                  pDoc->GetElemList()->ClearConflictingTempVars();
                  bSucc = pDoc->GetElemList()->ResizeSelElems( p, m_eResizeMode);
               }
            } else 
            {
               pDoc->GetElemList()->MoveFixPoints( CSize(p.x,p.y) );
               bSucc = pDoc->GetElemList()->MoveSelElems( p );
               if(!bSucc)
               {
                  pDoc->GetElemList()->MoveFixPoints( CSize(-p.x,-p.y) );
               }
            }
            
            if(bDropOutPurge)
            {
               pDoc->GetElemList()->DropOutPurge();
               bDropOutPurge = false;
            }

            if( bSucc )
            {
               m_StartPos = m_EndPos;
            } else {
               ;
            }

            // temp var handling(
            pDoc->GetElemList()->PurgeTempVars();  // create temp vars (if necessary/possible)
            pDoc->GetElemList()->CreateTempVars(false);
            pDoc->GetElemList()->IsOneOverlapped(false,EF_SELECT);   // must update EF_EMPHASIZE flag
            // temp var handling)

            pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateMarkedElemHint);
            pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateSelectionHint);
         }
    	   if (!m_NoAutoScroll) MoveViewToCursor();
         break;

      case EM_SELECT:
         if( Brush.CreateStockObject(NULL_BRUSH) )
         {
            OldRop   = dc.SetROP2(R2_NOT);
            OldBrush = dc.SelectObject(&Brush);
            OldPen   = dc.SelectObject(GDIHelper::GetPen(m_bDrawOnScreen, PE_NORMAL));
            // delete old rect
            dc.Rectangle( m_StartPos.x, m_StartPos.y, m_EndPos.x, m_EndPos.y);
            m_EndPos = m_LogCurPos; 
            // draw new rect
            dc.Rectangle( m_StartPos.x, m_StartPos.y, m_EndPos.x, m_EndPos.y);

            dc.SetROP2(OldRop);
            dc.SelectObject(OldBrush);
            dc.SelectObject(OldPen  );
         }
    	   if (!m_NoAutoScroll) MoveViewToCursor();
         break;

      case EM_LINE:
         if( pDoc->GetLineMode() == LDM_RUBB )
         {
            p  = m_CurPos;             // new destination
            p2 = m_CurPos - m_EndPos;  // all in one step
         } else {
            ASSERT(m_EditLine);
            p  = m_CurPos;
            // fix the destination to orthogonal coords
            if( ((m_EditLine->GetSize() == CSize(0,0)) &&
                  (abs(p.x - m_StartPos.x) > abs(p.y - m_StartPos.y))) ||
                (m_EditLine->GetSize().cx != 0) )
            {  // dir is horizontal
               p.x = (p.x + 1) & 0xFFFFFFFE;
               p.y  = m_StartPos.y;
               if( p.x > m_EndPos.x ) p2.x = 2; else p2.x = -2;
               p2.y = 0;
            } else {
               // dir is vertical
               p.x = m_StartPos.x;
               p.y = (p.y + 1) & 0xFFFFFFFE;
               p2.x = 0;
               if( p.y > m_EndPos.y ) p2.y = 2; else p2.y = -2;
            }
         } 
         // p is now our destination (finish point for this action)
         if( pDoc->GetLineMode() == LDM_RUBB )
         {
            m_EndPos = m_StartPos + p2;
            if(p2.x!=0 || p2.y!=0)
            {
               EditLineProc();
               pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateLightPinHint, NULL);
            }
         }else
         {
            ASSERT(m_EditLine);
            pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateDrawElemHint, m_EditLine);
            while ( m_EndPos != p )
            {
               m_EndPos = m_StartPos + p2;
               if( !EditLineProc() ) 
               {
                  break;
               }
               pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateLightPinHint, NULL);
               pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateDrawElemHint, m_EditLine);
            } 
            pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateMarkedElemHint);
         }
    	   if (!m_NoAutoScroll) MoveViewToCursor();
         break;

      case EM_LINE_START:
         pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateLightPinHint, NULL);
    	   if (!m_NoAutoScroll) MoveViewToCursor();
         break;

      case EM_INSCOL : 
      case EM_INSCOL1: 
      case EM_DELCOL : 
      case EM_DELCOL1: 
      case EM_INSROW : 
      case EM_INSROW1: 
      case EM_DELROW : 
      case EM_DELROW1: 
			CheckSlots(); 
    	   if (!m_NoAutoScroll) MoveViewToCursor();
			break;

      case EM_MONITOR:
         if( m_pTrackedWP )
         {
            TrackWP (point);
			   if (!m_NoAutoScroll) MoveViewToCursor();
         }
         break;
   }

 	dc.SelectObject(pOldFont);

   UpdateCursor(false);

   if( m_bDebug ) OnTimer(0);    // only for update screen ..
   
	CDbgEditorView::OnMouseMove(nFlags, point);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
BOOL CGrEditorView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
   if(m_pSEC_Edit)
   {
      return FALSE;
   }

   if (zDelta < 0)
   {
      while (-zDelta >= WHEEL_DELTA / 3)
      {
         if (nFlags & MK_CONTROL)
         {
            ScrollRel(2,0,FALSE);
         }
         else
         {
            ScrollRel(0,2,FALSE);
         };
         
         zDelta += WHEEL_DELTA / 3;
      }
   }
   else
   {
      while (zDelta >= WHEEL_DELTA / 3)
      {
         if (nFlags & MK_CONTROL)
         {
            ScrollRel(-2,0,FALSE);
         }
         else
         {
            ScrollRel(0,-2,FALSE);
         };
         
         zDelta -= WHEEL_DELTA / 3;
      }
   }
   
   if (m_pTrackedWP != NULL)
   {
      ScreenToClient (&pt);
      TrackWP (pt);
   }
   
   return (TRUE);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnLButtonDown(UINT nFlags, CPoint point) 
{
   CheckMousePos(point);

   // for POU-selection-dialog ..
   CFrameWnd* pFrame = GetParentFrame();
   if (pFrame)
   {
      pFrame->SetActiveView(this);
   }
   SetFocus();

   EventDown();
   CDbgEditorView::OnLButtonDown(nFlags, point);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnLButtonUp(UINT nFlags, CPoint point) 
{
   EventUp();
	CDbgEditorView::OnLButtonUp(nFlags, point);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
   EventEnter();
	CDbgEditorView::OnLButtonDblClk(nFlags, point);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnRButtonDown(UINT nFlags, CPoint point) 
{
   CGrEditorDoc *pDoc = GetDocument();
   ASSERT(pDoc);

   if (pDoc)
   {  // remove emphasizes
      if( pDoc->GetElemList()->ClearFlags(EF_EMPHASIZE) )
      {  // and update view, if necessary ..
         pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateWindowHint);
      }
   }

   CDbgEditorView::OnRButtonDown(nFlags, point);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnRButtonUp(UINT nFlags, CPoint point) 
{
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   switch ( pDoc->GetEditMode() ) 
   {
      case EM_NORMAL:
      case EM_MONITOR:
         // call context menu
         CDbgEditorView::OnRButtonUp(nFlags, point);
         break;
   }

   EventCancel();
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnContextMenu(CWnd* /*pWnd*/, CPoint point) 
{
   CGrEditorDoc*pDoc = GetDocument();
   CElem  *pE;
	DWORD   dwStyle;
   CMenu   menu;
   CPoint  m_ContextMenuPoint;
   CRect   r;
	bool    bNeedSeparator;
   bool    bSpecialMenu;
   bool    bDefaultMenu;
   bool    bSelecting;
   CString strConMenuText;

   ASSERT(pDoc);
   if (pDoc)
   {  // remove emphasizes
      if( pDoc->GetElemList()->ClearFlags(EF_EMPHASIZE) )
      {  // and update view, if necessary ..
         pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateWindowHint);
      }
   }

	if (!menu.CreatePopupMenu()) 
   {
      return;
   }

   // check if edit is possible
//   if (pDoc->IsModeNormal () && !OnModifyContent())
//   {
//      return;
//   }

   m_ContextMenuPoint = point;
   ScreenToClient (&m_ContextMenuPoint);
   // if capture is activ - must should correct the (possible negative) coordinate
   if( m_ContextMenuPoint.x > 0x8000) 
   {
      m_ContextMenuPoint.x = (LONG)((SHORT)&m_ContextMenuPoint.x);
   }
   if( m_ContextMenuPoint.y > 0x8000) 
   {
      m_ContextMenuPoint.y = (LONG)((SHORT)&m_ContextMenuPoint.y);
   }
   m_ContextMenuPoint = ClientToDoc (m_ContextMenuPoint);
   m_LogCurPos        = m_ContextMenuPoint;
   SetCurPos (DocToGrid(m_ContextMenuPoint));
   
   // make sure window is active
   GetParentFrame()->ActivateFrame();

   // set selection to element under cursor
   pE = pDoc->GetElemList()->SearchElem(m_LogCurPos, m_iZGrid);
   if (pE != NULL) 
   {
      if (!pE->ChkFlag(EF_SELECT))
      {
         UnselectAll();
         pE->SetFlag(EF_SELECT);
         pDoc->GetElemList()->MoveFlaggedToTail(EF_SELECT);
         pDoc->GetElemList()->MoveFlaggedToTail(EF_RUBBLINE);
         pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateSelectionHint);
      }
   }

   // build the menu DYNAMIC ..
	bNeedSeparator = false;
   bSpecialMenu   = false;
   bDefaultMenu   = false;
   bSelecting = pDoc->GetElemList()->IsOneSelected();

   if (pDoc->IsModeNormal ())
   {
      // first entry ..
      if (!pE)
      {
         strConMenuText.LoadString(IDS_GR_CONMENU_EDIT_LINE);
         menu.AppendMenu (MF_STRING, ID_GR_EDIT_LINE, strConMenuText);
         bNeedSeparator = true;
         bDefaultMenu   = true;
      } else {
         switch (pE->GetType())
         {
            case ET_FB        :
               strConMenuText.LoadString(IDS_GR_CONMENU_EDIT_INSTAMCE);
               menu.AppendMenu (MF_STRING, ID_GR_EDIT_INSTAMCE, strConMenuText);
               bNeedSeparator = true;
               break;

            case ET_FUNCTION:
               break;

            case ET_LABEL     :
            case ET_JUMP      : 
            case ET_VARIN     :
            case ET_VAROUT    :
            case ET_VARINOUT  : 
            case ET_CONIN     :
            case ET_CONOUT    : 
            case ET_COMMENT   : 
            case ET_TRANSITION: 
            case ET_ACTION    :
            case ET_CONTACT   :
            case ET_COIL      :
               strConMenuText.LoadString(IDS_GR_CONMENU_EDIT_NAME);
               menu.AppendMenu (MF_STRING, ID_GR_EDIT_NAME, strConMenuText);
               bNeedSeparator = true;
               break;

            case ET_STEP      : 
               strConMenuText.LoadString(IDS_GR_CONMENU_EDIT_NAME);
               menu.AppendMenu (MF_STRING, ID_GR_EDIT_NAME, strConMenuText);
               menu.AppendMenu (MF_SEPARATOR);
               strConMenuText.LoadString(IDS_GR_CONMENU_STEP_TOGGLEINIT);
               menu.AppendMenu (MF_STRING, ID_GR_STEP_TOGGLEINIT, strConMenuText);
               bNeedSeparator = true;
               break;

            case ET_ACTIONBLOCK :
               r.SetRect(pE->GetOrigin(), pE->GetOrigin()+CSize(Q_WIDTH, 2));
               GridToDoc(r);
               if (r.PtInRect(m_ContextMenuPoint))
               {
                  bSpecialMenu = true;
                  strConMenuText.LoadString(IDS_GR_CONMENU_EDIT_ACT_NONE);
                  menu.AppendMenu (MF_STRING, ID_GR_EDIT_ACT_NONE, strConMenuText);
                  menu.AppendMenu (MF_STRING, ID_GR_EDIT_ACT_N,    _T ("&N"));
                  menu.AppendMenu (MF_STRING, ID_GR_EDIT_ACT_R,    _T ("&R"));
                  menu.AppendMenu (MF_STRING, ID_GR_EDIT_ACT_S,    _T ("&S"));
#ifndef V120_SFC_NOTIMEQUALIFIER
                  menu.AppendMenu (MF_STRING, ID_GR_EDIT_ACT_L,    _T ("&L"));
                  menu.AppendMenu (MF_STRING, ID_GR_EDIT_ACT_D,    _T ("&D"));
#endif
                  menu.AppendMenu (MF_STRING, ID_GR_EDIT_ACT_P,    _T ("&P"));
#ifndef V120_SFC_NOTIMEQUALIFIER
                  menu.AppendMenu (MF_STRING, ID_GR_EDIT_ACT_SD,   _T ("SD"));
                  menu.AppendMenu (MF_STRING, ID_GR_EDIT_ACT_DS,   _T ("DS"));
                  menu.AppendMenu (MF_STRING, ID_GR_EDIT_ACT_SL,   _T ("SL"));
#endif
                  menu.AppendMenu (MF_STRING, ID_GR_EDIT_ACT_P0,   _T ("P&0"));
                  menu.AppendMenu (MF_STRING, ID_GR_EDIT_ACT_P1,   _T ("P&1"));
                  bSpecialMenu   = true;
                  bNeedSeparator = true;
               } else {
                  strConMenuText.LoadString(IDS_GR_CONMENU_EDIT_NAME);
                  menu.AppendMenu (MF_STRING, ID_GR_EDIT_NAME, strConMenuText);
                  bNeedSeparator = true;
               }
               break;

            default           : 
               strConMenuText.LoadString(IDS_GR_CONMENU_EDIT_LINE);
               menu.AppendMenu (MF_STRING, ID_GR_EDIT_LINE, strConMenuText);
               bNeedSeparator = true;
               bDefaultMenu   = true;
               break;
         }
      }
   }

	if (pDoc->IsModeNormal () && !bSpecialMenu)
   {
		if (bNeedSeparator)
		{
			menu.AppendMenu (MF_SEPARATOR);
		}

      dwStyle = bSelecting ? MF_STRING : MF_GRAYED | MF_STRING;

      strConMenuText.LoadString(IDS_GR_CONMENU_EDIT_CUT);
      menu.AppendMenu (dwStyle, ID_EDIT_CUT, strConMenuText);

      strConMenuText.LoadString(IDS_GR_CONMENU_EDIT_COPY);
		menu.AppendMenu (dwStyle, ID_EDIT_COPY, strConMenuText);
	
		if (OpenClipboard ())
		{
			if (IsClipboardFormatAvailable (m_uiCBFormatXMLId))
			{
				dwStyle = MF_STRING;
         } else {
				dwStyle = MF_GRAYED|MF_STRING;
			}
			CloseClipboard ();
		}
		else
		{
			dwStyle = MF_GRAYED | MF_STRING;
		};
      strConMenuText.LoadString(IDS_GR_CONMENU_EDIT_PASTE);
		menu.AppendMenu (dwStyle, ID_EDIT_PASTE, strConMenuText);

      strConMenuText.LoadString(IDS_GR_CONMENU_EDIT_CLEAR);
		menu.AppendMenu (MF_STRING, ID_EDIT_CLEAR, strConMenuText);
		
      bNeedSeparator = true;
   }

   if (pDoc->IsModeNormal () && bDefaultMenu && !bSpecialMenu)
	{
		CMenu subMenuFBD;
		CMenu subMenuSFC;
		CMenu subMenuLD;
      CMenu subMenuLRU;

		if (bNeedSeparator)
		{
			menu.AppendMenu (MF_SEPARATOR);
		}

      strConMenuText.LoadString(IDS_GR_CONMENU_INSERT_FB_CALL);
		menu.AppendMenu (MF_STRING, ID_GR_INS_FB_CALL, strConMenuText);
      
      pDoc = GetDocument ();
      if (pDoc != NULL)  
      {   
         HRESULT hr;
         hr = pDoc->GetLRUPous(m_listLruPous);
         if (hr!=S_OK || m_listLruPous.GetCount()==0)
         {
            strConMenuText.LoadString(IDS_GR_CONMENU_INSERT_LRU_FIRST);
            menu.AppendMenu (MF_STRING | MF_GRAYED, ID_GR_INSERT_LRU_FIRST, strConMenuText); 
         }
         else
         {
            if (subMenuLRU.CreatePopupMenu())
            {
               POSITION pos;
               pos = m_listLruPous.GetHeadPosition();
               int count = 0;
               while(pos)
               {
                  CString str = m_listLruPous.GetNext(pos);
                  subMenuLRU.AppendMenu(MF_STRING, ID_GR_INSERT_LRU_FIRST+count, str);
                  count++;
                  if (ID_GR_INSERT_LRU_FIRST+count > ID_GR_INSERT_LRU_LAST)
                  {
                     break;
                  }
               }
               strConMenuText.LoadString(IDS_GR_CONMENU_INSERT_LRU_FIRST);
               menu.AppendMenu (MF_POPUP, (UINT)subMenuLRU.Detach (), strConMenuText);
            }
         }
      }
      
      // no VarFeedback in LD
      dwStyle = ((pDoc->GetCurrLang() == EL_FBD) || 
                 (pDoc->GetCurrLang() == EL_ALL)) ? MF_STRING : MF_GRAYED | MF_STRING;

		if (subMenuFBD.CreatePopupMenu ())
		{
         strConMenuText.LoadString(IDS_GR_CONMENU_INS_VARIN);
			subMenuFBD.AppendMenu (MF_STRING, ID_GR_INS_VARIN   , strConMenuText);
         strConMenuText.LoadString(IDS_GR_CONMENU_INS_VAROUT);
			subMenuFBD.AppendMenu (MF_STRING, ID_GR_INS_VAROUT  , strConMenuText);
         strConMenuText.LoadString(IDS_GR_CONMENU_INS_VARINOUT);
			subMenuFBD.AppendMenu (dwStyle,   ID_GR_INS_VARINOUT, strConMenuText);
         strConMenuText.LoadString(IDS_GR_CONMENU_INS_JMP);
			subMenuFBD.AppendMenu (MF_STRING, ID_GR_INS_JMP     , strConMenuText);
         strConMenuText.LoadString(IDS_GR_CONMENU_INS_LAB);
			subMenuFBD.AppendMenu (MF_STRING, ID_GR_INS_LAB     , strConMenuText);
         strConMenuText.LoadString(IDS_GR_CONMENU_INS_RET);
			subMenuFBD.AppendMenu (MF_STRING, ID_GR_INS_RET     , strConMenuText);
         strConMenuText.LoadString(IDS_GR_CONMENU_INS_CONOUT);
			subMenuFBD.AppendMenu (MF_STRING, ID_GR_INS_CONOUT  , strConMenuText);
         strConMenuText.LoadString(IDS_GR_CONMENU_INS_CONIN);
			subMenuFBD.AppendMenu (MF_STRING, ID_GR_INS_CONIN   , strConMenuText);
         strConMenuText.LoadString(IDS_GR_CONMENU_INS_CMT);
			subMenuFBD.AppendMenu (MF_STRING, ID_GR_INS_CMT     , strConMenuText);

         strConMenuText.LoadString(IDS_GR_CONMENU_SUBMENU_FBD);
			menu.AppendMenu (MF_POPUP, (UINT)subMenuFBD.Detach (),strConMenuText);
		}

      dwStyle = pDoc->GetSfcMode() ? MF_STRING : MF_GRAYED | MF_STRING;
     
		if (subMenuSFC.CreatePopupMenu ())
		{
         strConMenuText.LoadString(IDS_GR_CONMENU_INS_STEP);
			subMenuSFC.AppendMenu (dwStyle, ID_GR_INS_STEP       , strConMenuText);
         strConMenuText.LoadString(IDS_GR_CONMENU_INS_TRANSITION);
			subMenuSFC.AppendMenu (dwStyle, ID_GR_INS_TRANSITION , strConMenuText);
         strConMenuText.LoadString(IDS_GR_CONMENU_INS_ACTION);
			subMenuSFC.AppendMenu (dwStyle, ID_GR_INS_ACTION     , strConMenuText);
         strConMenuText.LoadString(IDS_GR_CONMENU_INS_ACTIONBLOCK);
			subMenuSFC.AppendMenu (dwStyle, ID_GR_INS_ACTIONBLOCK, strConMenuText);
         strConMenuText.LoadString(IDS_GR_CONMENU_INS_DIVCON);
			subMenuSFC.AppendMenu (dwStyle, ID_GR_INS_DIVCON     , strConMenuText);
         strConMenuText.LoadString(IDS_GR_CONMENU_INS_SDIVCON);
			subMenuSFC.AppendMenu (dwStyle, ID_GR_INS_SDIVCON    , strConMenuText);
         strConMenuText.LoadString(IDS_GR_CONMENU_INS_EXCEPTION_ACTION);
         subMenuSFC.AppendMenu (dwStyle, ID_GR_INS_EXCEPTION_ACTION, strConMenuText);

         strConMenuText.LoadString(IDS_GR_CONMENU_INS_SUBMENU_SFC);
			menu.AppendMenu (MF_POPUP, (UINT)subMenuSFC.Detach (), strConMenuText);
		}

      dwStyle = ((pDoc->GetCurrLang() == EL_LD) || 
                 (pDoc->GetCurrLang() == EL_ALL)) ? MF_STRING : MF_GRAYED | MF_STRING;
     
		if (subMenuLD.CreatePopupMenu ())
		{
         strConMenuText.LoadString(IDS_GR_CONMENU_INS_POWER);
			subMenuLD.AppendMenu (dwStyle, ID_GR_INS_POWER,       strConMenuText);
         strConMenuText.LoadString(IDS_GR_CONMENU_CT_NORMAL);
			subMenuLD.AppendMenu (dwStyle, ID_GR_INS_CT_NORMAL,   strConMenuText);
         strConMenuText.LoadString(IDS_GR_CONMENU_CT_CLOSED);
			subMenuLD.AppendMenu (dwStyle, ID_GR_INS_CT_CLOSED,   strConMenuText);
         strConMenuText.LoadString(IDS_GR_CONMENU_CT_POSTRANS);
			subMenuLD.AppendMenu (dwStyle, ID_GR_INS_CT_POSTRANS, strConMenuText);
         strConMenuText.LoadString(IDS_GR_CONMENU_CT_NEGTRANS);
			subMenuLD.AppendMenu (dwStyle, ID_GR_INS_CT_NEGTRANS, strConMenuText);
         strConMenuText.LoadString(IDS_GR_CONMENU_CL_NORMAL);
			subMenuLD.AppendMenu (dwStyle, ID_GR_INS_CL_NORMAL,   strConMenuText);
         strConMenuText.LoadString(IDS_GR_CONMENU_CL_NEGATIVE);
			subMenuLD.AppendMenu (dwStyle, ID_GR_INS_CL_NEGATIVE, strConMenuText);
         strConMenuText.LoadString(IDS_GR_CONMENU_CL_SET);
			subMenuLD.AppendMenu (dwStyle, ID_GR_INS_CL_SET,      strConMenuText);
         strConMenuText.LoadString(IDS_GR_CONMENU_CL_RESET);
			subMenuLD.AppendMenu (dwStyle, ID_GR_INS_CL_RESET,    strConMenuText);
         strConMenuText.LoadString(IDS_GR_CONMENU_CL_POSTRANS);
			subMenuLD.AppendMenu (dwStyle, ID_GR_INS_CL_POSTRANS, strConMenuText);
         strConMenuText.LoadString(IDS_GR_CONMENU_CL_NEGTRANS);
			subMenuLD.AppendMenu (dwStyle, ID_GR_INS_CL_NEGTRANS, strConMenuText);

         strConMenuText.LoadString(IDS_GR_CONMENU_INS_SUBMENU_LD);
			menu.AppendMenu (MF_POPUP, (UINT)subMenuLD.Detach (), strConMenuText);
		}

		bNeedSeparator = true;
	}

   if (pDoc->IsModeNormal () && !bSpecialMenu)
   {
      if (bNeedSeparator)
      {
         menu.AppendMenu (MF_SEPARATOR);
      }
   
      dwStyle = bSelecting ? MF_STRING : MF_GRAYED | MF_STRING;

      strConMenuText.LoadString(IDS_GR_CONMENU_EDIT_COMMENT);
      menu.AppendMenu (dwStyle, ID_GR_EDIT_COMMENT,   strConMenuText);
      strConMenuText.LoadString(IDS_GR_CONMENU_EDIT_UNCOMMENT);
		menu.AppendMenu (dwStyle, ID_GR_EDIT_UNCOMMENT, strConMenuText);

      bNeedSeparator = true;
   }
   
   if (pDoc->IsModeNormal () && !bSpecialMenu)
   {
      if (pE)
      {
         bool bAction = false;
         switch (pE->GetType())
         {
            case ET_ACTION     :
            case ET_ACTIONBLOCK:
                bAction = true;
            case ET_LABEL      :
            case ET_JUMP       : 
            case ET_VARIN      :
            case ET_VAROUT     :
            case ET_VARINOUT   : 
            case ET_CONIN      :
            case ET_CONOUT     : 
            case ET_COMMENT    : 
            case ET_STEP       : 
            case ET_TRANSITION : 
            case ET_ALTDIV     :
            case ET_SIMDIV     :
            case ET_CONTACT    :
            case ET_COIL       :
            case ET_LINE       : 
            case ET_POWERRAIL  :
            case ET_FB         :
            case ET_FUNCTION   :
            case ET_PROCEDURE  :
               if (bNeedSeparator)
               {
                  menu.AppendMenu (MF_SEPARATOR);
               }
   
               if (bAction)
               {
                   strConMenuText.LoadString(IDS_GR_CONMENU_EDIT_PROP_FBDACTION);
                   menu.AppendMenu (MF_STRING, ID_GR_EDIT_PROP_FBDACTION, strConMenuText);
                   strConMenuText.LoadString(IDS_GR_CONMENU_EDIT_PROP_LDACTION);
                   menu.AppendMenu (MF_STRING, ID_GR_EDIT_PROP_LDACTION,  strConMenuText);
                   strConMenuText.LoadString(IDS_GR_CONMENU_EDIT_PROP_STACTION);
                   menu.AppendMenu (MF_STRING, ID_GR_EDIT_PROP_STACTION,  strConMenuText);
                   
                   bool bLD = false;
                   int iGroupSize = ((CElemContain*)pE)->GroupSize(); 
                   if( iGroupSize > 0 )
                   {
                       if( ((CElemContain*)pE)->GroupCheckElemLD() )
                       {
                           menu.EnableMenuItem(ID_GR_EDIT_PROP_FBDACTION, MF_BYCOMMAND | MF_GRAYED | MF_DISABLED );
                       }
//                       menu.EnableMenuItem(ID_GR_EDIT_PROP_STACTION, MF_BYCOMMAND | MF_GRAYED | MF_DISABLED );
                   }

                   if(pE->ChkFlag(EF_STACTION))
                   {
                       menu.CheckMenuItem(ID_GR_EDIT_PROP_STACTION,  MF_BYCOMMAND | MF_CHECKED  );
                       menu.CheckMenuItem(ID_GR_EDIT_PROP_LDACTION,  MF_BYCOMMAND | MF_UNCHECKED);
                       menu.CheckMenuItem(ID_GR_EDIT_PROP_FBDACTION, MF_BYCOMMAND | MF_UNCHECKED);

                   }else if(pE->ChkFlag(EF_LDACTION))
                   {
                       menu.CheckMenuItem(ID_GR_EDIT_PROP_STACTION,  MF_BYCOMMAND | MF_UNCHECKED);
                       menu.CheckMenuItem(ID_GR_EDIT_PROP_LDACTION,  MF_BYCOMMAND | MF_CHECKED  );
                       menu.CheckMenuItem(ID_GR_EDIT_PROP_FBDACTION, MF_BYCOMMAND | MF_UNCHECKED);
                   }else
                   {
                       menu.CheckMenuItem(ID_GR_EDIT_PROP_STACTION,  MF_BYCOMMAND | MF_UNCHECKED);
                       menu.CheckMenuItem(ID_GR_EDIT_PROP_LDACTION,  MF_BYCOMMAND | MF_UNCHECKED);
                       menu.CheckMenuItem(ID_GR_EDIT_PROP_FBDACTION, MF_BYCOMMAND | MF_CHECKED  );
                   }
               }
		         
               strConMenuText.LoadString(IDS_GR_CONMENU_EDIT_PROPERTY);
               menu.AppendMenu (MF_STRING, ID_GR_EDIT_PROPERTY, strConMenuText);
               bNeedSeparator = true;
               break;
         }
      }
         
      if (bNeedSeparator)
      {
         menu.AppendMenu (MF_SEPARATOR);
      }

      strConMenuText.LoadString(IDS_GR_CONMENU_SHOW_VAR_SELECTION);
      menu.AppendMenu (MF_STRING, ID_GR_SHOW_VAR_SELECTION, strConMenuText); 
      
      if (pE)
      {
         switch (pE->GetType())
         {
         case ET_VARIN      :
         case ET_VAROUT     :
         case ET_VARINOUT   : 
         case ET_FB         :
         case ET_TRANSITION :
         case ET_CONTACT    :
         case ET_COIL       :
            strConMenuText.LoadString(IDS_GR_CONMENU_SHOW_VAR_SEL_POPUP);
            menu.AppendMenu (MF_STRING, ID_GR_SHOW_VAR_SEL_POPUP, strConMenuText);

            break;
         }
      }

      strConMenuText.LoadString(IDS_GR_CONMENU_XREF_QUERY);
      menu.AppendMenu (MF_STRING, ID_GR_XREF_QUERY, strConMenuText); 
      strConMenuText.LoadString(IDS_GR_CONMENU_XREF_LAST);
      menu.AppendMenu (MF_STRING, ID_GR_XREF_LAST, strConMenuText); 
      strConMenuText.LoadString(IDS_GR_CONMENU_XREF_NEXT);
      menu.AppendMenu (MF_STRING, ID_GR_XREF_NEXT, strConMenuText); 

      bNeedSeparator = true;
   }

   if (pE && pDoc->IsModeNormal ())
   {
      switch (pE->GetType())
      {
         case ET_FB        :
         case ET_FUNCTION:
            if (bNeedSeparator)
            {
               menu.AppendMenu (MF_SEPARATOR);
            }
            strConMenuText.LoadString(IDS_GR_CONMENU_VIEW_OBJHELP);
            menu.AppendMenu (MF_STRING, ID_GR_VIEW_OBJHELP, strConMenuText);
            bNeedSeparator = true;
            break;
      }
   }
   
   if (pDoc->IsModeMonitor () && pDoc->CanMonitor ())
   {
      DWORD dwStyle1;
      DWORD dwStyle2;
      DWORD dwStyle3;
   
      if (bNeedSeparator)
      {
         menu.AppendMenu (MF_SEPARATOR);
      };
   
      dwStyle1 = FindWatchpointByPoint (m_ContextMenuPoint) != NULL ? MF_STRING : MF_GRAYED | MF_STRING;
      dwStyle2 = pDoc->GetFirstWPPosition () != NULL ? MF_STRING : MF_GRAYED | MF_STRING;
      dwStyle3 = pDoc->IsConnected () ? MF_STRING : MF_GRAYED | MF_STRING;
   
      strConMenuText.LoadString(IDS_GR_CONMENU_SET_WATCHPOINT);
      menu.AppendMenu (MF_STRING, ID_GR_SET_WATCHPOINT       , strConMenuText);
      strConMenuText.LoadString(IDS_GR_CONMENU_CLEAR_WATCHPOINT);
      menu.AppendMenu (dwStyle1 , ID_GR_CLEAR_WATCHPOINT     , strConMenuText);
      strConMenuText.LoadString(IDS_GR_CONMENU_CLEAR_ALL_WATCHPOINTS);
      menu.AppendMenu (dwStyle2 , ID_GR_CLEAR_ALL_WATCHPOINTS, strConMenuText);
      strConMenuText.LoadString(IDS_GR_CONMENU_WRITE_WATCHPOINT);
      menu.AppendMenu (dwStyle3 , ID_GR_WRITE_WATCHPOINT     , strConMenuText);

      bNeedSeparator = true;
   }

   if (pDoc->IsModeMonitor () && pDoc->CanDebug ())
   {
      CStdEditorPosition position;
      DWORD dwStyle1;
      DWORD dwStyle2;
      DWORD dwStyle3;
   
      if (bNeedSeparator)
      {
         menu.AppendMenu (MF_SEPARATOR);
      };
   
      BuildBreakpoint (position);
      dwStyle1 = pDoc->LookupBreakpoint (position) != NULL ? MF_STRING : MF_GRAYED | MF_STRING;
      dwStyle2 = pDoc->GetFirstBPPosition () != NULL ? MF_STRING : MF_GRAYED | MF_STRING;
      dwStyle3 = pDoc->IsDebugConnected () ? MF_STRING : MF_GRAYED | MF_STRING;
   
      strConMenuText.LoadString(IDS_GR_CONMENU_TOGGLE_BREAKPOINT);
      menu.AppendMenu (MF_STRING, ID_GR_TOGGLE_BREAKPOINT        , strConMenuText);
      strConMenuText.LoadString(IDS_GR_CONMENU_REMOVE_ALL_BREAKPOINTS);
      menu.AppendMenu (dwStyle2 , ID_GR_REMOVE_ALL_BREAKPOINTS   , strConMenuText);
      strConMenuText.LoadString(IDS_GR_CONMENU_TOGGLE_BREAKPOINT_ENABLED);
      menu.AppendMenu (dwStyle1 , ID_GR_TOGGLE_BREAKPOINT_ENABLED, strConMenuText);
   
      menu.AppendMenu (MF_SEPARATOR);
   
      if (
          (pDoc->GetCurrLang() == EL_ALL))
      {
         dwStyle = GetSFCMenuStyle (ID_GR_TOGGLE_MANUAL_MODE);
         strConMenuText.LoadString(IDS_GR_CONMENU_TOGGLE_MANUAL_MODE);
         menu.AppendMenu (dwStyle, ID_GR_TOGGLE_MANUAL_MODE, strConMenuText);
   
         menu.AppendMenu (MF_SEPARATOR);
      }
      
      strConMenuText.LoadString(IDS_GR_CONMENU_CONTINUE_TASK);
      menu.AppendMenu (dwStyle3 , ID_GR_CONTINUE_TASK   , strConMenuText);

      strConMenuText.LoadString(IDS_GR_CONMENU_STEP_INTO);
      menu.AppendMenu (dwStyle3 , ID_GR_STEP_INTO, strConMenuText);
      strConMenuText.LoadString(IDS_GR_CONMENU_STEP_OVER);
      menu.AppendMenu (dwStyle3 , ID_GR_STEP_OVER, strConMenuText);
      strConMenuText.LoadString(IDS_GR_CONMENU_STEP_OUT);
      menu.AppendMenu (dwStyle3 , ID_GR_STEP_OUT, strConMenuText);
   
      if (pDoc->GetCurrLang() == EL_ALL)
      {
         strConMenuText.LoadString(IDS_GR_CONMENU_PERFORM_CYCLE);
         menu.AppendMenu (GetSFCMenuStyle (ID_GR_PERFORM_CYCLE), ID_GR_PERFORM_CYCLE, strConMenuText);
   
         ExtendMenuBySFCItems (&menu);
      }   
      bNeedSeparator = true;
   }

   if (!pDoc->IsModeMonitor())
	{
		if (bNeedSeparator)
		{
			menu.AppendMenu (MF_SEPARATOR);
		};

      UINT Flags = MF_STRING;

      if(m_BookMarkTab.GetSize()==0)
         Flags |= MF_GRAYED;

      strConMenuText.LoadString(IDS_GR_CONMENU_BOOKMARKSET);
		menu.AppendMenu(MF_STRING, ID_BOOKMARKSET ,   strConMenuText);
      strConMenuText.LoadString(IDS_GR_CONMENU_BOOKMARKNEXT);
      menu.AppendMenu(Flags,     ID_BOOKMARKNEXT,   strConMenuText);
      strConMenuText.LoadString(IDS_GR_CONMENU_BOOKMARKPREV);
      menu.AppendMenu(Flags,     ID_BOOKMARKPREV,   strConMenuText);
      strConMenuText.LoadString(IDS_GR_CONMENU_BOOKMARKCLEAR);
      menu.AppendMenu(Flags,     ID_BOOKMARKDELETE, strConMenuText);

		bNeedSeparator = true;
	};

   menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, GetParentFrame());
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::ExtendMenuBySFCItems (CMenu *pMenu)
{
   CGrEditorDoc *pDoc;
   CElem *pE;
   CPoint p;
   CString strConMenuText;

   pDoc = GetDocument ();
   if (pDoc == NULL)
   {
      return;
   }

   pE = pDoc->GetElemList()->SearchElem(m_LogCurPos, m_iZGrid);
   if (pE)
   {
      p = pE->GetOrigin();
   } else {
      p = m_CurPos;
   }

   pMenu->AppendMenu (MF_SEPARATOR);

   if (pDoc->IsTransitionPosition (p.x, p.y))
   {
      strConMenuText.LoadString(IDS_GR_CONMENU_FORCE_TRANSITION_ONCE);
      pMenu->AppendMenu (GetSFCMenuStyle (ID_GR_FORCE_TRANSITION_ONCE), ID_GR_FORCE_TRANSITION_ONCE, strConMenuText);
      strConMenuText.LoadString(IDS_GR_CONMENU_FORCE_TRANSITION);
      pMenu->AppendMenu (GetSFCMenuStyle (ID_GR_FORCE_TRANSITION),      ID_GR_FORCE_TRANSITION,      strConMenuText);
      strConMenuText.LoadString(IDS_GR_CONMENU_BLOCK_TRANSITION);
      pMenu->AppendMenu (GetSFCMenuStyle (ID_GR_BLOCK_TRANSITION),      ID_GR_BLOCK_TRANSITION,      strConMenuText);
   };
   
   strConMenuText.LoadString(IDS_GR_CONMENU_FORCE_ALL_TRANSITIONS);
   pMenu->AppendMenu (GetSFCMenuStyle (ID_GR_FORCE_ALL_TRANSITIONS ), ID_GR_FORCE_ALL_TRANSITIONS , strConMenuText);
   strConMenuText.LoadString(IDS_GR_CONMENU_BLOCK_ALL_TRANSITIONS);
   pMenu->AppendMenu (GetSFCMenuStyle (ID_GR_BLOCK_ALL_TRANSITIONS ), ID_GR_BLOCK_ALL_TRANSITIONS , strConMenuText);
   strConMenuText.LoadString(IDS_GR_CONMENU_REVERT_ALL_TRANSITIONS);
   pMenu->AppendMenu (GetSFCMenuStyle (ID_GR_REVERT_ALL_TRANSITIONS), ID_GR_REVERT_ALL_TRANSITIONS, strConMenuText);

#if 0
   strConMenuText.LoadString(IDS_GR_CONMENU_GLOBALLY_FORCE_ALL_TRANSITIONS);
   pMenu->AppendMenu (GetSFCMenuStyle (ID_GR_GLOBALLY_FORCE_ALL_TRANSITIONS), ID_GR_GLOBALLY_FORCE_ALL_TRANSITIONS, strConMenuText);
#endif

   pMenu->AppendMenu (MF_SEPARATOR);

   if (pDoc->IsActionPosition (p.x, p.y))
   {
      strConMenuText.LoadString(IDS_GR_CONMENU_FORCE_ACTION_ONCE);
      pMenu->AppendMenu (GetSFCMenuStyle (ID_GR_FORCE_ACTION_ONCE), ID_GR_FORCE_ACTION_ONCE, strConMenuText);
      strConMenuText.LoadString(IDS_GR_CONMENU_FORCE_ACTION);
      pMenu->AppendMenu (GetSFCMenuStyle (ID_GR_FORCE_ACTION),      ID_GR_FORCE_ACTION,      strConMenuText);
      strConMenuText.LoadString(IDS_GR_CONMENU_BLOCK_ACTION);
      pMenu->AppendMenu (GetSFCMenuStyle (ID_GR_BLOCK_ACTION),      ID_GR_BLOCK_ACTION,      strConMenuText);
   }

   strConMenuText.LoadString(IDS_GR_CONMENU_FORCE_ALL_ACTIONS);
   pMenu->AppendMenu (GetSFCMenuStyle (ID_GR_FORCE_ALL_ACTIONS) , ID_GR_FORCE_ALL_ACTIONS , strConMenuText);
   strConMenuText.LoadString(IDS_GR_CONMENU_BLOCK_ALL_ACTIONS);
   pMenu->AppendMenu (GetSFCMenuStyle (ID_GR_BLOCK_ALL_ACTIONS) , ID_GR_BLOCK_ALL_ACTIONS , strConMenuText);
   strConMenuText.LoadString(IDS_GR_CONMENU_REVERT_ALL_ACTIONS);
   pMenu->AppendMenu (GetSFCMenuStyle (ID_GR_REVERT_ALL_ACTIONS), ID_GR_REVERT_ALL_ACTIONS, strConMenuText);

#if 0
   strConMenuText.LoadString(IDS_GR_CONMENU_GOBALLY_BLOCK_ALL_ACTIONS);
   pMenu->AppendMenu (GetSFCMenuStyle (ID_GR_GLOBALLY_BLOCK_ALL_ACTIONS), ID_GR_GLOBALLY_BLOCK_ALL_ACTIONS, strConMenuText);
#endif
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
DWORD CGrEditorView::GetSFCMenuStyle (UINT uiId)
{
    bool bEnabled;
    bool bChecked;
    DWORD dwStyle;

    GetSFCMenuProperties (uiId, bEnabled, bChecked);

    dwStyle = MF_STRING;

    if (!bEnabled)
    {
        dwStyle |= MF_GRAYED;
    }
    if (bChecked)
    {
        dwStyle |= MF_CHECKED;
    }

    return (dwStyle);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::InvertPin()
{
   CElem *pTE;
   CPin  *pPin;
   CRect  r;
   CPoint p;
   POSITION pos;
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   bool bReady = false;
   
   if( (pos = pDoc->GetElemList()->GetHeadPosition()) )
   {
      do {
         if( (pTE = (CElem*) pDoc->GetElemList()->GetNext(pos)) != NULL)
         {
            pTE->GetRect(r, m_iZGrid);       // cursor inside element area
            r.InflateRect(1,1);
            if((m_LogCurPos.x >= r.left  ) && 
               (m_LogCurPos.x <= r.right ) &&
               (m_LogCurPos.y >= r.top   ) && 
               (m_LogCurPos.y <= r.bottom) )
            {
               switch(pTE->GetType())
               {
               case ET_FB:
               case ET_FUNCTION:
               case ET_PROCEDURE:
                  {
                     pPin = pTE->SearchPin (m_LogCurPos,m_iZGrid);
                     if (pPin)
                     {  
                        // typ == BOOL .. ??
                        
                        if( !pPin->m_Flag.Chk(PF_F_EDGE) && 
                           !pPin->m_Flag.Chk(PF_R_EDGE) ) // only level controlled pins
                        {
                           if( pPin->m_Flag.Chk(PF_NEGATE) ) 
                           {
                              pPin->m_Flag.Clr(PF_NEGATE); 
                           } else {
                              pPin->m_Flag.Set(PF_NEGATE);
                           }
                           
                           pDoc->UpdateAllViews (NULL, CGrEditorDoc::UpdateDrawElemHint, pTE);
                           pDoc->m_Action.StoreState (pDoc->GetElemList());
                           pDoc->SetModifiedFlag(pDoc->m_Action.StateChangedQ());
                        }
                        bReady = true;
                     }
                  } break;
               case ET_COIL:
               case ET_CONTACT:
                  {
                     pTE->GetRect(r, m_iZGrid);
                     if((m_LogCurPos.x >= r.left  ) && 
                        (m_LogCurPos.x <= r.right ) &&
                        (m_LogCurPos.y >= r.top   ) && 
                        (m_LogCurPos.y <= r.bottom) )
                     {
                        ((CElemLdBase*)pTE)->Invert();
                        pDoc->UpdateAllViews (NULL, CGrEditorDoc::UpdateDrawElemHint, pTE);
                        pDoc->m_Action.StoreState (pDoc->GetElemList());
                        pDoc->SetModifiedFlag(pDoc->m_Action.StateChangedQ());
                        bReady = true;
                     }
                  }
                  break;
               }//switch
            }
         }//if
      } while (!bReady && (pTE != NULL) && (pos != NULL));
   }
}


/*func*------------------------------------------------------------------------
  description: changes from resize mode to move mode (if possible)
  in :
  out:
-----------------------------------------------------------------------------*/
bool CGrEditorView::TryChangeToMoveMode()
{
   int iSHLines = 0;
   int iSVLines = 0;
   int iSElems  = 0;
   
   CGrEditorDoc *pDoc = GetDocument();
   if(pDoc)
   {
      ASSERT(pDoc->GetEditMode()==EM_RESIZE);   // expect resize mode

      POSITION pos = pDoc->GetElemList()->GetHeadPosition();
      while(pos)
      {
         CElem* pE = pDoc->GetElemList()->GetNext(pos);
         if(pE->ChkFlag(EF_SELECT))
         {
            if(pE->GetType()==ET_LINE && !pE->ChkFlag(EF_RUBBLINE))
            {
               if(pE->GetSize().cx != 0)
               {
                  iSHLines++;
               }else if(pE->GetSize().cy != 0)
               {
                  iSVLines++;
               }
            }else
            {
               iSElems++;
            }
         }
      }
   
      if( (iSHLines==0 || iSVLines==0) && iSElems==0 )
      {
         pDoc->SetEditMode(EM_MOVE);
         m_eResizeMode = RS_LAST;
         UpdateCursor(false);
         return true;
      }
   }
   return false;
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnKeyMove(UINT nChar)
{
   int          OldRop;
   CPen        *OldPen; 
   CBrush      *OldBrush, Brush;
   CElem       *pTE;
   CElemLine   *pL;
   CPoint       p, p2, pRelLogOffs, pGridCurPos;
   CRect        r;
   bool         bMove(false);
   bool         bSucc;
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);

	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);

   switch ( pDoc->GetEditMode() ) {
      case EM_RESIZE_DIR:
          if(OnModifyContent())
	      switch ( nChar ) {
            case VK_NUMPAD4:
            case VK_LEFT   : m_eResizeMode = RS_W;  goto m_dir_ok;

            case VK_NUMPAD6:
            case VK_RIGHT  : m_eResizeMode = RS_E;  goto m_dir_ok;

            case VK_NUMPAD8:
            case VK_UP     : m_eResizeMode = RS_N;  goto m_dir_ok;

            case VK_NUMPAD2:
            case VK_DOWN   : m_eResizeMode = RS_S;  goto m_dir_ok;

            case VK_NUMPAD7:
            case VK_HOME   : m_eResizeMode = RS_NW; goto m_dir_ok;

            case VK_NUMPAD1:
            case VK_END    : m_eResizeMode = RS_SW; goto m_dir_ok;

            case VK_NUMPAD9:
            case VK_PRIOR  : m_eResizeMode = RS_NE; goto m_dir_ok;

            case VK_NUMPAD3:
            case VK_NEXT   : m_eResizeMode = RS_SE; goto m_dir_ok;

            default: pDoc->SetEditMode( EM_NORMAL ); break;

             m_dir_ok:
               pTE = pDoc->GetElemList()->SearchElem(m_LogCurPos,m_iZGrid);
               if (pTE == NULL)
               {
                  pDoc->SetEditMode( EM_NORMAL );
               } else {
                  pDoc->SetEditMode( EM_RESIZE );
                  pTE->GetResizePos(m_eResizeMode, m_CurPos);
                  m_BeforeOpPos = m_StartPos = m_CurPos;
                  m_LogCurPos = GridToDoc( m_CurPos );
                  pDoc->GetElemList()->MoveFlaggedToTail(EF_SELECT);
                  pDoc->GetElemList()->MoveFlaggedToTail(EF_RUBBLINE);
                  MoveViewToCursor(TRUE);
                  UpdateCursor();
               }
               break;
         }
         return;

      case EM_RESIZE: // resize
      case EM_INSERT:
      case EM_LINE  :
         bMove = true;
         switch ( nChar ) {
            case VK_LEFT : MoveRel( -2, 0); break;
            case VK_RIGHT: MoveRel(  2, 0); break;
            case VK_UP   : MoveRel( 0, -2); break;
            case VK_DOWN : MoveRel( 0,  2); break;
            default      : bMove = false;   break;
         }
         m_CurPos.x = (m_CurPos.x & 0xFFFFFFFE) + (m_BeforeOpPos.x & 0x01);
         m_CurPos.y = (m_CurPos.y & 0xFFFFFFFE) + (m_BeforeOpPos.y & 0x01);
         break;

      default:
         m_CurPos.x &= 0xFFFFFFFE;
         m_CurPos.y &= 0xFFFFFFFE;

         bMove = true;
         GetClientRect( &r );
         ClientToDoc(r);

         CSize stdSize = GetInsertSize(IDS_GR_REG_LINESIZE);
         
         switch ( nChar ) 
         {
            case VK_LEFT : 
               if( GetKeyState(VK_SHIFT) < 0 && !(GetKeyState(VK_SPACE) < 0)) 
               {
                  if(OnModifyContent() && pDoc->GetEditMode()==EM_NORMAL)
                  {
                     p = m_CurPos;
                     pL = InsertLine(true,false);
                     if(pDoc->GetElemList()->IsOneOverlapped(false,EF_SELECT))
                     {
                        pDoc->GetElemList()->ClearConflictingTempVars();
                     }
                     while ((pL->GetSize().cx > 0) && 
                            (pDoc->GetElemList()->IsOneOverlapped(false,EF_SELECT)))
                     {
                        pL->Resize(2,0,-2,0);
                        MoveRel(2,0);
                     }
                     EventDown();
                     EventUp();
                     while (!pDoc->GetElemList()->SearchLine(m_CurPos, m_CurPos + CSize(1,0), false) && 
                            (p.x > m_CurPos.x))
                     {
                        MoveRel(2,0);
                     }
                  }
               }else
               {
                  if( GetKeyState(VK_CONTROL) < 0) // ctrl key pressed
                  {         // ---------------- element left -----------------
                     ScrollRel( -2, 0);
                  } else if( GetKeyState(VK_MENU) < 0) { // alt key pressed
                     MoveRel( -stdSize.cx, 0);
                  } else {  // ---------------- col left ---------------------
                     MoveRel( -2, 0);
                  } 
               }
               break;

            case VK_RIGHT: 
               if( GetKeyState(VK_SHIFT) < 0 && !(GetKeyState(VK_SPACE) < 0)) 
               {
                  if(OnModifyContent() && pDoc->GetEditMode()==EM_NORMAL)
                  {
                     p = m_CurPos;
                     pL = InsertLine(true,true);
                     if(pDoc->GetElemList()->IsOneOverlapped(false,EF_SELECT))
                     {
                        pDoc->GetElemList()->ClearConflictingTempVars();
                     }
                     while ((pL->GetSize().cx > 0) && 
                            (pDoc->GetElemList()->IsOneOverlapped(false,EF_SELECT)))
                     {
                        pL->Resize(0,0,-2,0);
                        MoveRel(-2,0);
                     }
                     EventDown();
                     EventUp();
                     while (!pDoc->GetElemList()->SearchLine(m_CurPos, m_CurPos + CSize(-1,0), false) && 
                            (p.x < m_CurPos.x))
                     {
                        MoveRel(-2,0);
                     }
                  }
               }else
               {
                  if( GetKeyState(VK_CONTROL) < 0) 
                  {         // ---------------- element right ----------------
                     ScrollRel( 2, 0);
                  } else if( GetKeyState(VK_MENU) < 0) { // alt key pressed
                     MoveRel( stdSize.cx, 0);
                  } else {  // ---------------- col right --------------------
                     MoveRel(  2, 0); 
                  } 
               }
               break;

            case VK_UP   : 
               if( GetKeyState(VK_SHIFT) < 0 && !(GetKeyState(VK_SPACE) < 0)) 
               {
                  if(OnModifyContent() && pDoc->GetEditMode()==EM_NORMAL)
                  {
                     p = m_CurPos;
                     pL = InsertLine(false,false);
                     if(pDoc->GetElemList()->IsOneOverlapped(false,EF_SELECT))
                     {  // remove temp vars under line
                        pDoc->GetElemList()->ClearConflictingTempVars();
                     }
                     while ((pL->GetSize().cy > 0) && 
                            (pDoc->GetElemList()->IsOneOverlapped(false,EF_SELECT)))
                     {  // reduce line at overlapped element
                        pL->Resize(0,2,0,-2);
                        MoveRel(0,2);
                     }
                     EventDown();
                     EventUp();
                     while (!pDoc->GetElemList()->SearchLine(m_CurPos, m_CurPos + CSize(0,1), false) && 
                            (p.y > m_CurPos.y))
                     {
                        MoveRel(0,2);
                     }
                  }
               } else
               {
                  if( GetKeyState(VK_CONTROL) < 0) 
                  {         // ---------------- scroll line up ---------------
                     ScrollRel( 0, -2);
                  } else if( GetKeyState(VK_MENU) < 0) { // alt key pressed
                     MoveRel( 0, -stdSize.cy);
                  } else {  // ---------------- line up ----------------------
                     MoveRel( 0, -2);
                  } 
               }
               break;

            case VK_DOWN : 
               if( GetKeyState(VK_SHIFT) < 0 && !(GetKeyState(VK_SPACE) < 0)) 
               {
                  if(OnModifyContent() && pDoc->GetEditMode()==EM_NORMAL)
                  {
                     p = m_CurPos;
                     pL = InsertLine(false,true);
                     if(pDoc->GetElemList()->IsOneOverlapped(false,EF_SELECT))
                     {
                        pDoc->GetElemList()->ClearConflictingTempVars();
                     }
                     while ((pL->GetSize().cy > 0) && 
                            (pDoc->GetElemList()->IsOneOverlapped(false,EF_SELECT)))
                     {
                        pL->Resize(0,0,0,-2);
                        MoveRel(0,-2);
                     }
                     EventDown();
                     EventUp();
                     while (!pDoc->GetElemList()->SearchLine(m_CurPos, m_CurPos + CSize(0,-1), false) && 
                            (p.y < m_CurPos.y))
                     {
                        MoveRel(0,-2);
                     }
                  }
               }else
               {
                  if( GetKeyState(VK_CONTROL) < 0) 
                  {         // ---------------- scroll line down -------------
                     ScrollRel( 0, 2);
                  } else if( GetKeyState(VK_MENU) < 0) { // alt key pressed
                     MoveRel( 0, stdSize.cy);
                  } else {  // ---------------- line down --------------------
                     MoveRel( 0, 2); 
                  } 
               }
               break;

            case VK_HOME : 
               if( GetKeyState(VK_CONTROL) < 0) 
               {         // ---------------- leftmost position ------------
                  MovePos( 0, m_CurPos.y); 
               } else {  // ---------------- line end ---------------------
                  MoveRel( -r.Width() / m_iZGrid, 0); 
               } 
               break;

            case VK_END  : 
               if( GetKeyState(VK_CONTROL) < 0) 
               {         // ---------------- rightmost position -----------
                  MovePos( MAX_XGRID, m_CurPos.y); 
               } else {  // ---------------- one page right ---------------
                  MoveRel( r.Width() / m_iZGrid, 0); 
               } 
               break;

            case VK_PRIOR : 
               if( GetKeyState(VK_CONTROL) < 0) 
               {         // ---------------- topmost position -------------
                  MovePos( m_CurPos.x,  0);
               } else {  // ---------------- one page up ------------------
                  MoveRel( 0, -( r.Height() / m_iZGrid ));
               } 
               break;

            case VK_NEXT : 
               if( GetKeyState(VK_CONTROL) < 0) 
               {         // ---------------- bottom position --------------
                  MovePos( m_CurPos.x,  MAX_YGRID);
               } else {  // ---------------- one page down ----------------
                  MoveRel( 0,  ( r.Height() / m_iZGrid ));
               } 
               break;

            case VK_ADD  : 
               if( GetKeyState(VK_CONTROL) < 0) 
               {         // ---------------- micro-zoom in ----------------
                  OnChangeZoom(zoom_in_small);
               } else {  // ---------------- zoom in ----------------------
                  OnChangeZoom(zoom_in);
               }
               break;

            case VK_SUBTRACT: 
               if( GetKeyState(VK_CONTROL) < 0) 
               {         // ---------------- micro zoom out ---------------
                  OnChangeZoom(zoom_out_small);
               } else {  // ---------------- zoom out ---------------------
                  OnChangeZoom(zoom_out);
               }
               break;

            case VK_MULTIPLY:
               OnChangeZoom(zoom_to_default);
               break;

            default: 
               bMove = false;
               break;
         }
         break;
   }

   // additional actions ..
   if( bMove )
   {
      switch ( pDoc->GetEditMode() ) 
      {
         case EM_RESIZE:
         case EM_MOVE  :
         case EM_INSERT:
            m_EndPos = m_CurPos;
            if( m_StartPos != m_EndPos )
            {
               if( pDoc->GetEditMode() == EM_RESIZE )
               {
                  switch( m_eResizeMode )
                  {  // only horizontal resize 
                     case RS_W : 
                     case RS_E : 
                        if( m_EndPos.y != m_StartPos.y )
                        {
                           if(!TryChangeToMoveMode())
                           {
                              m_EndPos.y = m_StartPos.y;
                              MovePos(m_EndPos.x, m_EndPos.y); 
                           }
                        }
                        break;
                     // only vertical resize 
                     case RS_N : 
                     case RS_S : 
                        if( m_EndPos.x != m_StartPos.x )
                        {
                           if(!TryChangeToMoveMode())
                           {
                              m_EndPos.x = m_StartPos.x;
                              MovePos(m_EndPos.x, m_EndPos.y); 
                           }
                        }
                        break;

                     case RS_NW: 
                     case RS_SW: 
                     case RS_NE: 
                     case RS_SE: break;
                  }
               }

               // copy selection from FBLKs only at 1st move (except in resize mode)
               pDoc->GetElemList()->CreateTempVars(m_bFirstMove && pDoc->GetEditMode()!=EM_RESIZE);  // mark temp vars only
               
               pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateSelectionHint);
               pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateMarkedElemHint);
               pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateRubberHint);
               pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateFixPointsHint);

               pDoc->GetElemList()->PurgeTempVars();  // create temp vars (if necessary/possible)

               p = CPoint(m_EndPos - m_StartPos);
               bool bDropOutPurge = false;
               if( m_bFirstMove )
               {
                  CWaitCursor wc;
                  m_bFirstMove = false;

                  if( pDoc->GetEditMode() == EM_MOVE )   // update BEFORE deconneting elements!!
                  {
                     pDoc->GetElemList()->UpdateGroupSelect();
                  }
                  pDoc->GetElemList()->SignalChangeInit(true);

                  if (pDoc->GetEditMode() != EM_INSERT)
                  {
                     //---------------- drop out as default (+SHIFT key: reroute)   (see: OnMouseMove(...)!!!)
                     if(/*!*/(GetKeyState(VK_SHIFT) < 0)) // /*no*/ shift key pressed
                     {
                        // insert rubber lines at needed positions OR unroute wires
                        pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateWindowHint);
                        pDoc->GetElemList()->ReRouteDeconnect(pDoc->GetReconMode(),
                           pDoc->GetElemList()->GetIECLanguange(m_CurPos, 1) == LANG_FBD);
                     }else
                     {
                        // no rubber line insertion (instead of: drop out, if possible)
                        if(pDoc->GetElemList()->DropOutSelElems())
                        {
                           bDropOutPurge = true;
                           pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateWindowHint);
                        }
                        pDoc->GetElemList()->SignalChangeInit(true);
                     }
                  }
                  pDoc->GetElemList()->UpdatePinStatus();
               }
               
               // perform operation itself
               if( pDoc->GetEditMode() == EM_RESIZE )
               {
                  bSucc = pDoc->GetElemList()->ResizeSelElems( p, m_eResizeMode);
                  if(!bSucc)
                  {
                     pDoc->GetElemList()->ClearConflictingTempVars();
                     bSucc = pDoc->GetElemList()->ResizeSelElems( p, m_eResizeMode);
                  }
               }else 
               {
                  pDoc->GetElemList()->MoveFixPoints( CSize(p.x,p.y) );
                  bSucc = pDoc->GetElemList()->MoveSelElems( p );
                  if(!bSucc)
                  {
                     pDoc->GetElemList()->MoveFixPoints( CSize(-p.x,-p.y) );
                  }
               }

               if(bDropOutPurge)
               {
                  pDoc->GetElemList()->DropOutPurge();
                  bDropOutPurge = false;
               }

               if( bSucc )
               {
                  m_StartPos = m_EndPos;
               } else {
                  m_EndPos = m_StartPos;
                  MovePos(m_StartPos.x, m_StartPos.y); 
               }
               // temp var handling(
               pDoc->GetElemList()->PurgeTempVars();  // create temp vars (if necessary/possible)
               pDoc->GetElemList()->CreateTempVars(false);
               pDoc->GetElemList()->IsOneOverlapped(false,EF_SELECT);   // must update EF_EMPHASIZE flag
               // temp var handling)

               pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateMarkedElemHint);
               pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateSelectionHint);
            }
            break;

         case EM_SELECT:
            if( Brush.CreateStockObject(NULL_BRUSH) )
            {
               OldRop   = dc.SetROP2(R2_NOT);
               OldBrush = dc.SelectObject(&Brush);
               OldPen   = dc.SelectObject(GDIHelper::GetPen(m_bDrawOnScreen, PE_NORMAL));
               // delete old rect
               dc.Rectangle( m_StartPos.x, m_StartPos.y, m_EndPos.x, m_EndPos.y);
               m_EndPos = m_LogCurPos;
               // draw new rect
               dc.Rectangle( m_StartPos.x, m_StartPos.y, m_EndPos.x, m_EndPos.y);

               dc.SetROP2(OldRop);
               dc.SelectObject(OldBrush);
               dc.SelectObject(OldPen  );
            }
            break;

         case EM_LINE:
            if( pDoc->GetLineMode() == LDM_RUBB )
            {
               p  = m_CurPos;             // new destination
               p2 = m_CurPos - m_EndPos;  // all in one step

               m_EndPos = m_StartPos + p2;
               if(p2.x!=0 || p2.y!=0)
               {
                  EditLineProc();
               }
            } else {
               ASSERT(m_EditLine);
               // change mouse <-> keyboard
               if( (pDoc->GetLineMode() == LDM_ORTHO) &&    // orthogonal mode ? 
                   (m_EditLine != NULL) &&                  // already a line ?
                   (m_EditLine->GetSize() != CSize(0,0)) && // is line orient.
                   (m_CurPos.x != m_StartPos.x) &&          // dx != 0 and 
                   (m_CurPos.y != m_StartPos.y) )           //             dy != 0
               {
                  if( m_EditLine->GetSize().cx == 0 )
                  {
                     m_CurPos.x = m_EditLine->GetOrigin().x;
                  } else {
                     m_CurPos.y = m_EditLine->GetOrigin().y;
                  }
                  MovePos(m_CurPos.x, m_CurPos.y);
   //               UpdateCursor();
                  MoveViewToCursor(TRUE);
               }
               // new destination
               m_EndPos = m_CurPos;  
               // dir change ? -> insert a new line
               if( (pDoc->GetLineMode() == LDM_ORTHO) &&    // orthogonal mode ? 
                   (m_EditLine != NULL) &&                  // already a line ?
                   (m_EditLine->GetSize() != CSize(0,0)) && // is line orient.
                   (m_StartPos != m_EndPos) &&              // moved ?
                   (CGRTool::GetDir(m_EditLine->GetOrigin(), 
                           m_EditLine->GetOrigin() + m_EditLine->GetSize()) & 1) != 
                   (CGRTool::GetDir(m_StartPos, m_EndPos) & 1) )
               {  
                  // end the actual line
                  EditLineStop();
                  // check connect point for break an existing line ..
                  EditLineStart(m_StartPos);
                  m_EndPos = m_CurPos;  
               }

               pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateDrawElemHint, m_EditLine);
               if( (m_StartPos != m_EndPos) && !EditLineProc() )       
               {
                  MovePos(m_StartPos.x, m_StartPos.y);
               }
               pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateLightPinHint, NULL);
               pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateDrawElemHint, m_EditLine);
               pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateMarkedElemHint);
            }
            break;

         case EM_LINE_START:
            pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateLightPinHint, NULL);
            break;

         case EM_INSCOL : 
         case EM_INSCOL1: 
         case EM_DELCOL : 
         case EM_DELCOL1: 
         case EM_INSROW : 
         case EM_INSROW1: 
         case EM_DELROW : 
         case EM_DELROW1: 
            CheckSlots(); 
            break;
      }
      MoveViewToCursor(TRUE);
      UpdateCursor();
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
bool CGrEditorView::CheckSlots()
{
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   bool bOK = true;
   switch ( pDoc->GetEditMode() ) {
      case EM_INSCOL : 
      case EM_INSCOL1: 
      case EM_DELCOL : 
      case EM_DELCOL1: 
         m_bLocalSpacing = !( GetKeyState(VK_CONTROL) < 0); // global operation
         if( GetKeyState(VK_SHIFT) < 0) 
         {
            if( bOK = pDoc->GetElemList()->CheckSpace(CPoint(-2,0), m_CurPos, m_bLocalSpacing) ) 
            { 
               pDoc->SetEditMode( EM_DELCOL );
            } else {                                           
               pDoc->SetEditMode( EM_DELCOL1 );
            }
         } else {
            if( bOK = pDoc->GetElemList()->CheckSpace(CPoint( 2,0), m_CurPos, m_bLocalSpacing) ) 
            {
               pDoc->SetEditMode( EM_INSCOL );
            } else {                                           
               pDoc->SetEditMode( EM_INSCOL1 );
            }
         }
         break; 

      case EM_INSROW : 
      case EM_INSROW1: 
      case EM_DELROW : 
      case EM_DELROW1: 
         m_bLocalSpacing = !( GetKeyState(VK_CONTROL) < 0); // global operation
         if( GetKeyState(VK_SHIFT) < 0) 
         {
            if( bOK = pDoc->GetElemList()->CheckSpace(CPoint(0,-2), m_CurPos, m_bLocalSpacing) ) 
            {
               pDoc->SetEditMode( EM_DELROW );
            } else {
               pDoc->SetEditMode( EM_DELROW1 );
            }
         } else {
            if( bOK = pDoc->GetElemList()->CheckSpace(CPoint(0, 2), m_CurPos, m_bLocalSpacing) ) 
            {
               pDoc->SetEditMode( EM_INSROW );
            } else {
               pDoc->SetEditMode( EM_INSROW1 );
            }
         }
         break;
   }
   return bOK;
}

void CGrEditorView::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   switch ( pDoc->GetEditMode() ) 
   {
      case EM_EDITPROP:
         break;

      default:
         if( GetKeyState(VK_MENU) < 0)
         {
            OnKeyMove(nChar);  
         }
         break;
   }
	
	CDbgEditorView::OnSysKeyDown(nChar, nRepCnt, nFlags);
}

void CGrEditorView::OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDbgEditorView::OnSysKeyUp(nChar, nRepCnt, nFlags);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   switch ( pDoc->GetEditMode() ) 
   {
      case EM_EDITPROP   :
         ASSERT(m_pSEC_Edit || m_pPropEdit);
         if (m_pSEC_Edit)
         {
            CFrameWnd* pFrame = GetParentFrame();
            if (pFrame)
            {
               pFrame->SetActiveView(m_pSEC_Edit);
            }
            m_pSEC_Edit->SetFocus();
            m_pSEC_Edit->PostMessage(WM_KEYDOWN, nChar, (LPARAM)nRepCnt | ((LPARAM)nFlags << 16));
         } else if (m_pPropEdit)
         {
            m_pPropEdit->SetFocus();
            m_pPropEdit->PostMessage(WM_KEYDOWN, nChar, (LPARAM)nRepCnt | ((LPARAM)nFlags << 16));
         }
         break;

      default:
         switch ( nChar ) {
            case VK_ESCAPE: 
               EventCancel();
               break;

            case VK_SPACE : 
               if (OnModifyContent ())
               {
                  EventDown();  
               }
               break;

            case VK_DELETE: 
               // if(pDoc->GetEditMode() != EM_MONITOR)  // während EM_INSERT konnte gelöscht werden!!!
               if(OnModifyContent() && (pDoc->GetEditMode() == EM_NORMAL))
               {
                  DeleteElem(); 
               }
               else
               {
                   ::MessageBeep(-1);
               }
               break;

            case VK_CONTROL : 
            case VK_SHIFT : 
               CheckSlots(); 
               UpdateCursor(false); 
               break;

            default: 
               OnKeyMove(nChar);  
               break;
         }
         CDbgEditorView::OnKeyDown(nChar, nRepCnt, nFlags); 
         break;
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
   switch ( nChar ) {
      case VK_SPACE : EventUp();    break;
      
      case VK_CONTROL:
      case VK_SHIFT : 
         CheckSlots(); 
         UpdateCursor(); 
         break;
   }
	CDbgEditorView::OnKeyUp(nChar, nRepCnt, nFlags);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnSize(UINT nType, int cx, int cy) 
{
   ResyncScrollSizes();        // ensure that scroll info is up-to-date
   CDbgEditorView::OnSize(nType, cx, cy);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
BOOL CGrEditorView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if( nHitTest == HTCLIENT && pWnd == this )
   {
      m_eCursorMode = CT_INIT;  // work around
      UpdateCursor(false);

      return TRUE;    // we will handle it in the mouse move
   }
   return CDbgEditorView::OnSetCursor(pWnd, nHitTest, message);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnSelAll() 
{
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   pDoc->GetElemList()->SetFlags(EF_SELECT);
	pDoc->UpdateAllViews(NULL);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnEditFind() 
{
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   ASSERT_VALID(this);
   switch ( pDoc->GetEditMode() ) 
   {
      case EM_NORMAL:
      case EM_LINE_START:
      case EM_INSCOL:
      case EM_INSCOL1:
      case EM_DELCOL:
      case EM_DELCOL1:
      case EM_INSROW:
      case EM_INSROW1:
      case EM_DELROW:
      case EM_DELROW1:
      case EM_MONITOR:
         OnEditFindReplace(TRUE);
         break;
   }
   ASSERT_VALID(this);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnEditReplace() 
{
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   ASSERT_VALID(this);

   if (!OnModifyContent ())
   {
      ::MessageBeep(-1);
      return;
   }
   switch (pDoc->GetEditMode())
   {
      case EM_NORMAL:
      case EM_LINE_START:
      case EM_INSCOL:
      case EM_INSCOL1:
      case EM_DELCOL:
      case EM_DELCOL1:
      case EM_INSROW:
      case EM_INSROW1:
      case EM_DELROW:
      case EM_DELROW1:
         OnEditFindReplace(FALSE);
         break;
   }
   ASSERT_VALID(this);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnEditRepeat() 
{
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   ASSERT_VALID(this);

   if (!OnModifyContent ())
   {
      ::MessageBeep(-1);
      return;
   }
   switch (pDoc->GetEditMode())
   {
      case EM_EDITPROP: 
         // edit repeat in control .. 
         break;

      default: 
         if( !FindText(true,true,true) )
         {
            OnTextNotFound(m_SearchDat.m_strFind);
         }
         break;
   }
   ASSERT_VALID(this);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnEditFindReplace(BOOL bFindOnly)
{
   if (!OnModifyContent ())
   {
      ::MessageBeep(-1);
      return;
   }
   ASSERT_VALID(this);
   if( m_pFindReplaceDlg != NULL )
   {
      if( m_SearchDat.m_bFindOnly == (bFindOnly == TRUE))
      {
         m_pFindReplaceDlg->SetActiveWindow();
         m_pFindReplaceDlg->ShowWindow(SW_SHOW);
         return;
      }
      ASSERT((m_SearchDat.m_bFindOnly == TRUE) != bFindOnly);
      m_pFindReplaceDlg->SendMessage(WM_CLOSE);
      ASSERT(m_pFindReplaceDlg == NULL);
      ASSERT_VALID(this);
   }

   CString strFind;
   GetSelectedText(strFind);
   if( strFind.IsEmpty() ) 
   {
      strFind = m_SearchDat.m_strFind;
   }
   CString strReplace = m_SearchDat.m_strReplace;
   m_pFindReplaceDlg = new CFindReplaceDialog;
   ASSERT(m_pFindReplaceDlg != NULL);
   DWORD dwFlags = FR_HIDEWHOLEWORD;
   if (m_SearchDat.m_bNext)
   {
      dwFlags |= FR_DOWN;
   }
   if (m_SearchDat.m_bCase)
   {
      dwFlags |= FR_MATCHCASE;
   }
   if (!m_pFindReplaceDlg->Create(bFindOnly, strFind, strReplace, dwFlags, this))
   {
      m_pFindReplaceDlg = NULL;
      ASSERT_VALID(this);
      return;
   }

   m_pFindReplaceDlg->SetActiveWindow();
   m_pFindReplaceDlg->ShowWindow(SW_SHOW);
   ASSERT(m_pFindReplaceDlg != NULL);
   m_SearchDat.m_bFindOnly = (bFindOnly == TRUE);
   ASSERT_VALID(this);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnFindNext(LPCTSTR lpszFind, BOOL bNext, BOOL bCase)
{
   ASSERT_VALID(this);
   m_SearchDat.m_strFind = lpszFind;
   m_SearchDat.m_bCase   = ((bCase == TRUE) ? true : false);
   m_SearchDat.m_bNext   = ((bNext == TRUE) ? true : false);

   if( !FindText(true,true,true)) // m_strFind, m_bNext, m_bCase)) 
   {
      OnTextNotFound(m_SearchDat.m_strFind);
   }
   ASSERT_VALID(this);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnReplaceSel(LPCTSTR lpszFind, BOOL bNext, BOOL bCase,
        LPCTSTR lpszReplace)
{
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   ASSERT_VALID(this);

   if (!OnModifyContent ()) 
   {
      ::MessageBeep(-1);
      return; 
   }
   m_SearchDat.m_strFind    = lpszFind;
   m_SearchDat.m_strReplace = lpszReplace;
   m_SearchDat.m_bCase      = ((bCase == TRUE) ? true : false);
   m_SearchDat.m_bNext      = ((bNext == TRUE) ? true : false);

   if (!InitializeReplace()) 
   {
      return;
   }

   ReplaceSel(m_SearchDat.m_strReplace);
   FindText(true,true,true); // m_strFind, bNext, bCase);

   pDoc->m_Action.StoreState(pDoc->GetElemList());
   pDoc->SetModifiedFlag(pDoc->m_Action.StateChangedQ());
   pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateWindowHint);
   ASSERT_VALID(this);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnReplaceAll(LPCTSTR lpszFind, LPCTSTR lpszReplace, BOOL bCase)
{
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   ASSERT_VALID(this);
   m_SearchDat.m_strFind    = lpszFind;
   m_SearchDat.m_strReplace = lpszReplace;
   m_SearchDat.m_bCase      = ((bCase == TRUE) ? true : false);
   m_SearchDat.m_bNext      = true;

   if (!OnModifyContent ()) 
   {
      ::MessageBeep(-1);
      return; 
   }
   if (!InitializeReplace() && 
       !SameAsSelected(m_SearchDat.m_strFind, m_SearchDat.m_bCase))
   {
      // initial find was not successful
      return;
   }
   // store position for end check
   CPoint SaveFindCur   = m_SearchDat.m_FindCur;
   int    iSaveFindLen  = m_SearchDat.m_iFindLen;
   bool   bSaveNext     = m_SearchDat.m_bNext;
   m_SearchDat.m_FindCur   = CPoint(0,0);
   m_SearchDat.m_iFindElem = INT_MIN;
   m_SearchDat.m_iFindPos  = 0;
   m_SearchDat.m_iFindLen  = 0;
   m_SearchDat.m_bNext = true; // go forward for a sorted list ..

   while( FindText(true, false, true) )
   {
      ReplaceSel(m_SearchDat.m_strReplace);
   };

   m_SearchDat.m_FindCur   = SaveFindCur;
   m_SearchDat.m_iFindElem = INT_MIN;
   m_SearchDat.m_iFindPos  = 0;
   m_SearchDat.m_iFindLen  = iSaveFindLen;
   m_SearchDat.m_bNext = bSaveNext;

   MovePos( m_SearchDat.m_FindCur.x, m_SearchDat.m_FindCur.y); // no cursor move ..
   CenterViewToCursor(m_SearchDat.m_FindCur);      // scroll view to pE ..
   pDoc->m_Action.StoreState(pDoc->GetElemList());
   pDoc->SetModifiedFlag(pDoc->m_Action.StateChangedQ());
   pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateWindowHint);

   ASSERT_VALID(this);
}

/*func*------------------------------------------------------------------------
  description: helper to do find first if no selection
  in :
  out:
-----------------------------------------------------------------------------*/
BOOL CGrEditorView::InitializeReplace()
{
   ASSERT_VALID(this);

   // do find next if no selection
   if (m_SearchDat.m_iFindLen == 0)
   {
      if( !FindText(true,true,true)) // m_strFind, m_bNext, m_bCase) )
      {  
         // text not found
         OnTextNotFound(m_SearchDat.m_strFind);
         return FALSE;
      }
   }

   if( !SameAsSelected(m_SearchDat.m_strFind, m_SearchDat.m_bCase))
   {
      if( !FindText(true,true,true)) //m_strFind, m_bNext, m_bCase))
      {  
         // text not found
         OnTextNotFound(m_SearchDat.m_strFind);
         return FALSE;
      }
   }

   ASSERT_VALID(this);
   return TRUE;
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
LRESULT CGrEditorView::OnFindReplaceCmd(WPARAM, LPARAM lParam)
{
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   ASSERT_VALID(this);

   CFindReplaceDialog* pDialog = CFindReplaceDialog::GetNotifier(lParam);
   ASSERT(pDialog != NULL);
   ASSERT(pDialog == m_pFindReplaceDlg);
   if( pDialog->IsTerminating() )
   {
      m_pFindReplaceDlg = NULL;
      // remove (search) selection on sheet
      m_SearchDat.m_FindCur = m_SearchDat.m_bNext ? CPoint(0,0) : CPoint(MAX_XGRID,MAX_YGRID); 
      m_SearchDat.m_iFindElem = INT_MIN;
      m_SearchDat.m_iFindPos  = 0;
      m_SearchDat.m_iFindLen  = 0;
      pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateSelectionHint);
   }
   else if (pDialog->FindNext())
   {
      OnFindNext(pDialog->GetFindString(), 
                 pDialog->SearchDown(), 
                 pDialog->MatchCase());
   }
   else if (pDialog->ReplaceCurrent())
   {
      ASSERT(!m_SearchDat.m_bFindOnly);
      OnReplaceSel(pDialog->GetFindString(),
                   pDialog->SearchDown(), 
                   pDialog->MatchCase(),
                   pDialog->GetReplaceString());
   }
   else if (pDialog->ReplaceAll())
   {
      ASSERT(!m_SearchDat.m_bFindOnly);
      OnReplaceAll(pDialog->GetFindString(), 
                   pDialog->GetReplaceString(),
                   pDialog->MatchCase());
   }
   ASSERT_VALID(this);
   return 0;
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
BOOL CGrEditorView::SameAsSelected(LPCTSTR lpszCompare, BOOL bCase)
{
   // check length first
   size_t nLen = lstrlen(lpszCompare);
   if( nLen != (size_t)(m_SearchDat.m_iFindLen)) return FALSE;

   // length is the same, check contents
   CString strSelect;
   GetSelectedText(strSelect);
   return ( bCase && lstrcmp (lpszCompare, strSelect) == 0) ||
          (!bCase && lstrcmpi(lpszCompare, strSelect) == 0);
 
   return FALSE;
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::GetSelectedText(CString& strResult)
{
   CGrEditorDoc* pDoc = GetDocument();
   ASSERT_VALID(pDoc);
	ASSERT_VALID(this);
   if (pDoc)
   {
      CElem *pE = pDoc->GetElemList()->GetCurElem(m_SearchDat.m_FindCur);
      if (pE)
      {
         pE->GetText( m_SearchDat.m_iFindElem, 
                      m_SearchDat.m_iFindPos, 
                      m_SearchDat.m_iFindLen, 
                      strResult);
      }
   }
	ASSERT_VALID(this);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::ReplaceSel(LPCTSTR lpszNewText)
{
   CGrEditorDoc* pDoc = GetDocument();
   ASSERT_VALID(pDoc);
	ASSERT_VALID(this);

   CSTEditorView *pView;
   POSITION       pos;
   UINT Key;

   if (pDoc)
   {
      CElem *pE = pDoc->GetElemList()->GetCurElem(m_SearchDat.m_FindCur);
      if (pE)
      {
         pE->ReplaceSelText(  m_SearchDat.m_iFindElem,
                              m_SearchDat.m_iFindPos,
                              m_SearchDat.m_iFindLen, lpszNewText);

         if (pE->ChkFlag(EF_STACTION) && (m_SearchDat.m_iFindElem == -2)) // found in ST text
         {
            pos = m_STViewMap.GetStartPosition();
            while (pos)
            {
               m_STViewMap.GetNextAssoc(pos, Key, pView);
               if (pE->GetOrigin() == pView->GetContext())
               {
                  pView->ReplaceSelection(lpszNewText);
                  break;
               }
            }
         }

         if( m_SearchDat.m_bNext )
         {
            if( m_SearchDat.m_iFindPos < INT_MAX )
            {
               m_SearchDat.m_iFindPos += m_SearchDat.m_strReplace.GetLength()-1;
            }else
            {
               m_SearchDat.m_iFindElem++;
               m_SearchDat.m_iFindPos = 0;
            }
         }else 
         {
            if( m_SearchDat.m_iFindPos>0)  
            {
               m_SearchDat.m_iFindPos -= m_SearchDat.m_iFindLen;
            }else
            {
               m_SearchDat.m_iFindElem--; 
               m_SearchDat.m_iFindPos = INT_MAX; 
            }
         }
      }
   }
	ASSERT_VALID(this);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
bool CGrEditorView::FindText(bool bUpdateScreen, bool bWrap, bool bOnlyBody)
{
   bool    bFirst;
   CElem  *pE;
   CRect   rect;
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   ASSERT_VALID(this);
   ASSERT( (LPCTSTR)m_SearchDat.m_strFind != NULL);
   ASSERT(*m_SearchDat.m_strFind != '\0');
   
   CSTEditorView *pView;
   POSITION       pos;
   UINT Key;

   CWaitCursor wait;

   // clear old selections ..
   UnselectAll();

   bFirst = true;

   // search in the header first
   if(!bOnlyBody && m_SearchDat.m_FindCur == CPoint(0,0) ) // location is in the header
   {
      CSTEditorView *pSTView = (CSTEditorView*) pDoc->GetSTViewRef(CPoint(0,0));
      ASSERT(pSTView);

      if(pSTView->FindText(&m_SearchDat))    
      {
         return true;
      }
      // restart search parameter for body
      m_SearchDat.m_iFindElem = INT_MIN;
      m_SearchDat.m_iFindPos  = 0;
      m_SearchDat.m_iFindLen  = 0;
   }

   // not found or m_SearchDat.m_CurPos is not in the header: continue with the GR-section
   //
   if( (pE = pDoc->GetElemList()->GetCurElem(m_SearchDat.m_FindCur)) == NULL )
   {
      pE = pDoc->GetElemList()->GetNextElem(m_SearchDat.m_FindCur, m_SearchDat.m_bNext);
   } else 
   {
      if( m_SearchDat.m_bNext ) 
      {
         if( m_SearchDat.m_iFindPos < INT_MAX ) 
         { 
            m_SearchDat.m_iFindPos++; 
         }else
         { 
            m_SearchDat.m_iFindElem++; 
            m_SearchDat.m_iFindPos = 0; 
         }
      }else 
      {
         if( m_SearchDat.m_iFindPos > 0 ) 
         { 
            m_SearchDat.m_iFindPos--; 
         }else
         { 
            m_SearchDat.m_iFindElem--; 
            m_SearchDat.m_iFindPos = INT_MAX; 
         }
      }
   }

   do 
   {
      if (pE && pE->FindText( m_SearchDat) )
      {
         if( bUpdateScreen )
         {
            pE->SetFlag(EF_SELECT);
            MovePos( m_SearchDat.m_FindCur.x, m_SearchDat.m_FindCur.y); // no cursor move ..
            CenterViewToCursor(m_SearchDat.m_FindCur);      // scroll view to pE ..
            pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateSelectionHint);

            if (pE->ChkFlag(EF_STACTION) &&
                (m_SearchDat.m_iFindElem == -2)) // found in ST text
            {
               pos = m_STViewMap.GetStartPosition();
               while (pos)
               {
                  m_STViewMap.GetNextAssoc(pos, Key, pView);
                  if (pE->GetOrigin() == pView->GetContext())
                  {
                     CWnd* pWnd = GetFocus();
                     CString str;

                     pE->GetText(-2, 0, 32000, str);
                     if (m_SearchDat.m_iFindPos < str.GetLength())
                     {
                        int iCol=0, iRow=1, i;

                        for (i=0; i<m_SearchDat.m_iFindPos; i++)
                        {
                           if (str[i] == '\n') 
                           { 
                              iRow++; iCol = 0; 
                           } else {
                              iCol++;
                           }
                        }

                        pView->SetFocus();
                        pView->SelectSearchText(iRow, iCol, m_SearchDat.m_iFindLen);

                        pWnd->SetFocus();
                        pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateSelectionHint);
                     }
                     break;
                  }
               }
            }
         }
         return (true);
      }

      do 
      {
         if( !pE ) 
         { 
            bFirst = false;
   
            if (bWrap)
            {
               m_SearchDat.m_FindCur = m_SearchDat.m_bNext ? CPoint(0,0) : CPoint(MAX_XGRID,MAX_YGRID); 
            }
         }
         pE = pDoc->GetElemList()->GetNextElem(m_SearchDat.m_FindCur, m_SearchDat.m_bNext);
      } while ( !pE && bFirst );
      
      m_SearchDat.m_iFindElem = m_SearchDat.m_bNext ? INT_MIN : INT_MAX;
      m_SearchDat.m_iFindPos  = m_SearchDat.m_bNext ?       0 : INT_MAX;
   } while ( pE );

   ASSERT_VALID(this);
   return (false);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnTextNotFound(LPCTSTR psz)
{
   CString str;

   str.Format(IDS_GR_EDIT_NOTFOUND, psz);
   
   AfxMessageBox((LPCTSTR)str);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnEditResize() 
{
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   if (!OnModifyContent ())
   {
      ::MessageBeep(-1);
      return;
   }
   if(pDoc->GetElemList()->SignalChangeCount()>1)
   {
      pDoc->GetElemList()->SignalChangeInit(true);
   }
   pDoc->SetEditMode( EM_RESIZE_START );
}

void CGrEditorView::OnShortcut(UINT nChar)
{
   bool bSFC = GetDocument()->GetSfcMode();
   EDIT_LANG Lang =GetDocument()->GetCurrLang();
   bool bLD  = bSFC || (Lang == EL_LD);
   bool bVIO = (GetDocument()->GetCurrLang() == EL_FBD) || 
               (GetDocument()->GetCurrLang() == EL_ALL);
   bool bUpper = (isupper(nChar) != 0);

   switch(tolower(nChar))
   {
      case 'a': if(bSFC) OnInsAction();         break;
      case 'b': if(bSFC) OnInsActionBlock();    break;
      case 'c': if(bSFC) OnInsSDivCon();        break;
      case 'd': if(bSFC) OnInsDivCon();         break;
      case 'f': if(bVIO) OnInsVarinout();       break;
      case 'i': if(bLD ) OnInsCtNormal();       break;   // normal contact
      case 'l':          OnEditLine();          return;  // (!!!) stay in line draw mode
      case 'p': if(bLD ) OnInsPower();          break;
      case 'q': if(bLD ) OnInsClNormal();       break;   // normal coil
      case 'r':          OnInsVarIn();          break;
      case 's': if(bSFC) OnInsStep();           break;
      case 't': if(bSFC) OnInsTransition();     break;
      case 'w':          OnInsVarOut();         break;
      case 'x':          OnSpacing(bUpper?EM_DELCOL:EM_INSCOL);  return;
      case 'y':          OnSpacing(bUpper?EM_DELROW:EM_INSROW);  return;
      case '.':          OnToggleFixPoint();    return;
#ifdef _DEBUG
      case 'u': GetDocument()->GetElemList()->UnrouteFlaggedLines(false, EF_SELECT);
                GetDocument()->GetElemList()->UnrouteFlaggedLines(true,  EF_SELECT);
                GetDocument()->UpdateAllViews(NULL, CGrEditorDoc::UpdateWindowHint);
#endif
      default: return;
   }

   if(GetDocument()->GetElemList()->IsOneOverlapped(true, EF_SELECT))
   {
      GetDocument()->GetElemList()->ClearConflictingTempVars();
   }
   EventDown();   // put the elem into sheet
   EventUp();     // by simulating user input (LButton)
}


/*func*------------------------------------------------------------------------
  description: toggle bookmark at current cursor position
  in : implicit - uses current grid-cursor position
  out: -
-----------------------------------------------------------------------------*/
void CGrEditorView::OnToggleBookmark()
{
   CStdEditorPosition Pos;

   bool bFound = false;

   // ----------- avoid accumulation of rounding errors ---------
   CPoint PCursor = m_LogCurPos;
   PCursor.x =(PCursor.x+1)>>1;
   PCursor.y =(PCursor.y+1)>>1;
   PCursor  = DocToGrid(PCursor);
   PCursor += PCursor;
   // -----------------------------------------------------------

   // search for a bookmark representing the current scroll position
   for(int i=0; !bFound && i<m_BookMarkTab.GetSize(); i++)
   {
      if(m_BookMarkTab[i].m_Pos == PCursor)
      {
         bFound = true;
         break;   // don't increment i
      }
   }

   CDC *pDC;
   pDC = CDC::FromHandle(::GetDC(NULL));

   if(bFound)
   {
      m_BookMarkTab.RemoveAt(i);
      for(int j=0; j<m_BookMarkTab.GetSize(); j++) // rename bookmarks
      {
         CString Name;
//no bookmark name
//         Name.Format("%i",j);
         m_BookMarkTab[j].m_Name = Name;
      }
   }else
   {
      CString Name;

      // keep bookmark order
      int iNewPos = PointToInt(PCursor);
      bool bReady = false;
      for(int i=0; i<m_BookMarkTab.GetSize(); i++)
      {
         CPoint PScr = m_BookMarkTab[i].m_Pos;
         int    iPos = PointToInt(PScr);           // calc linear position

         if(!bReady && iPos>iNewPos)
         {
            bReady = true;
            m_BookMarkTab.InsertAt(i, BookMark(PCursor, Name));
            m_BookMarkPos = i;
         }
         if(bReady)  // rename the moved bookmarks
         {
//no bookmark name
//            Name.Format("%i",i);
            m_BookMarkTab[i].m_Name = Name;
         }
      }
      if(!bReady) // end is reached without any change
      {
//no bookmark name
//       Name.Format("%i",m_BookMarkTab.GetSize());
         m_BookMarkPos = m_BookMarkTab.Add( BookMark(PCursor, Name));
      }
   }
   GetDocument()->UpdateAllViews(NULL, CGrEditorDoc::UpdateWindowHint);
}


/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnBookmarkNext()
{
   OnGotoBookmark(true);
}


/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnBookmarkPrev()
{
   OnGotoBookmark(false);
}


/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnGotoBookmark(bool bNext)
{
   int N = m_BookMarkTab.GetSize();
   if(N>0)
   {
      CPoint PCursor = m_CurPos;   // get current cursor position in grid-coords

      // current bookmark removed?: find the one thext to cursor, set as current
      if(m_BookMarkPos <0 || m_BookMarkPos > m_BookMarkTab.GetSize())
      {
         for(int i=0; i<m_BookMarkTab.GetSize(); i++)
         {
            if(m_BookMarkTab[i].m_Pos == PCursor)
            {
               m_BookMarkPos = i;   // if found: set as current bookmark index
               break;
            }
         }
      }
      m_BookMarkPos += bNext? 1:-1;

      if(m_BookMarkPos >= N )
         m_BookMarkPos = 0;

      if(m_BookMarkPos < 0)
         m_BookMarkPos = N-1;

      BookMark *pB = &m_BookMarkTab[m_BookMarkPos];
      if(pB)
      {
         // start position keeping the cursor at current screen position (if possible)
         CPoint lastScrollPos = GetScrollPosition ();

         MovePos(pB->m_Pos.x, pB->m_Pos.y);           // set cursor at bookmark (!)
         CenterViewToCursor(pB->m_Pos);

         // get scroll delta
         CPoint newScrollDiff = GetScrollPosition() - lastScrollPos;
         PCursor += DocToGrid(newScrollDiff);
         MovePos(PCursor.x, PCursor.y);               // drag the cursor by scoll delta
         
         UpdateCursor();
      }
   }
}


/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnClearBookmarks()
{
   CDC *pDC;
   pDC = CDC::FromHandle(::GetDC(NULL));

   m_BookMarkTab.RemoveAll();
   GetDocument()->UpdateAllViews(NULL, CGrEditorDoc::UpdateWindowHint);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   switch ( pDoc->GetEditMode() ) {
   case EM_MONITOR:
         if (OnToggleLadder())
         {
            break;
         }
         if (!OnModifyContent ())
         {
            goto m_default;
         }
         // fall through ..

      case EM_NORMAL: 
         if(CheckReadonly())
         {
             ::MessageBeep(-1);
             break;
         }
         switch ( nChar ) {
            case VK_RETURN: EventEnter(); break;
            case '/': InvertPin();        break;
            default:
               OnShortcut(nChar);
         }
         break;

      case EM_EDITPROP: 
         {
            switch ( nChar ) 
            {
            case VK_ESCAPE: 
               EditNameStop(true);
               goto m_end_edit;
               // fall through ..
            case VK_RETURN:
            case 0x0A:    // Ctrl-RETURN 
               EditNameStop(false);
            m_end_edit:
               pDoc->SetEditMode( EM_NORMAL );
               GetParentFrame()->SetActiveView(this);
               SetFocus();
               pDoc->UpdateAllViews(NULL);
               UpdateCursor();

               if( FindNextTempVar())
               {
                  EventEnter();
               }
            }
            break;
         }            

      default:
m_default:
         switch ( nChar ) {
            case VK_RETURN: EventEnter(); break;
         }
         break;
   }

   CDbgEditorView::OnChar(nChar, nRepCnt, nFlags);
}



/*func*------------------------------------------------------------------------
  description: tries to locate the next temp-var for renaming
  in : expects m_pAutoElem is set correctly (to the previous edited temp-var)
       if m_pAutoElem==NULL, the 1st temp-var in the list is searched
  out: -
  ret: true: editable temp-var found
-----------------------------------------------------------------------------*/
bool CGrEditorView::FindNextTempVar()
{
   if (!m_bEditNext) return false;

   CGrEditorDoc *pDoc = GetDocument();
   bool bFound = false;
   CElem   *pE;

   ASSERT_VALID(pDoc);
   ASSERT(m_pAutoElem);
   ASSERT(m_pAutoElem->GetType()==ET_VARIN || m_pAutoElem->GetType()==ET_VAROUT);
   
   if (!bFound && m_pAutoElem)
   {    
      pE = pDoc->GetElemList()->FindNextTempVar(m_pAutoElem);
            
      if(pE)   // next AutoElem found?
      {
         CRect R;
         pE->GetRect(R,1);
         MovePos(R.CenterPoint().x, R.CenterPoint().y);
         bFound = true;
      }
   }
   if (bFound)
   {
      ASSERT(pE); // next temp-var to edit
      ASSERT(pE->GetType()== ET_VARIN || pE->GetType()== ET_VAROUT);
      ASSERT(pE->ChkFlag(EF_AUTOELEM));

      CRect R;
      pE->GetBodyRect(R,1);   // grid units
      CPoint P = R.CenterPoint();
      m_LogCurPos = P;
      MovePos(P.x,P.y);
   }
   return bFound;
}


/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::DeleteElem()
{
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   if (pDoc->GetElemList()->IsOneSelected())
   {
      pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateSelectionHint);
      pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateFixPointsHint);

      OnDeleteSTActions(true);  // delete all selected STActionViews

      if(pDoc->GetReconMode())
      {
         pDoc->GetElemList()->DropOutSelElems();
         pDoc->GetElemList()->DeleteFlaggedElems(EF_SELECT);
         pDoc->GetElemList()->DropOutPurge();
      }else
      {
         pDoc->GetElemList()->DeleteFlaggedElems(EF_SELECT);
      }
      pDoc->GetElemList()->InitMoveFixPoints();
      pDoc->GetElemList()->DoneMoveFixPoints();
      pDoc->GetElemList()->PurgeTempVars();

      if( pDoc->GetElemList()->IsOneOverlapped() )
      {
         pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateMarkedElemHint);

         AfxMessageBox( IDS_GR_ERR_OVERLAP, MB_ICONSTOP | MB_OK);
         OnDeleteSTActions(false);
         pDoc->m_Action.ReloadLastState(pDoc->GetElemList());
         pDoc->UpdateAllViews(NULL);
      }else
      {
         pDoc->GetElemList()->ConnectAllElems();
         pDoc->m_Action.StoreState(pDoc->GetElemList());
      }

      pDoc->SetModifiedFlag(pDoc->m_Action.StateChangedQ());
      pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateWindowHint);
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
   CPalette     *pOldPal;
   CGrEditorDoc *pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   CDbgEditorView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	if( (m_bActive != bActivate) &&
       ((bActivate && (pActivateView == this)) || (!bActivate && (pDeactiveView == this))) )
   {
      m_bActive = bActivate;
      if( m_bActive )
      {
         CClientDC dc(this);
         OnPrepareDC(&dc, NULL);

         pOldPal = GDIHelper::RealizePalette(&dc, true);
         GDIHelper::UnrealizePalette(&dc, pOldPal);
      }
      if (pDoc->GetElemList()->IsOneSelected())
      {
         OnUpdate(NULL, CGrEditorDoc::UpdateSelectionHint, NULL);
      }
   }	
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnInitialUpdate() 
{
   ZoomFont();
   ResyncScrollSizes();

   CDbgEditorView::OnInitialUpdate();

   // register drop target
   m_dropTarget.Register( this );

   if( m_uTimerID == 0 )
   {
      m_uTimerID = SetTimer(1, 100, NULL);
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::SetPageSize()
{
   ResyncScrollSizes();
	OnUpdate(NULL, CGrEditorDoc::UpdateWindowHint, NULL);
   MoveViewToCursor();
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::ResyncScrollSizes()
{
   CGrEditorDoc *pDoc = GetDocument();
	CSize         docSize, pageSize, lineSize;
   CRect         r;

   ASSERT(pDoc);
	if (pDoc == NULL)
	{
      return;
   }

   SetZoomedGrid(pDoc->GetGridSize()); // update zoom

   lineSize.cx  = m_iDZGrid;
   lineSize.cy  = m_iDZGrid;
   docSize  = pDoc->GetDocSize();
   GetClientRect(&r);
   pageSize = CSize(r.Width(), r.Height());

   SetScrollSizes(MM_TEXT, docSize, pageSize, lineSize);

   CPoint p = m_LastScrollPos; // GetScrollPosition (); 
   if ((p.x != 0) && (docSize.cx < pageSize.cx))
   {
      p.x -= pageSize.cx - (docSize.cx - p.x);
      p.x  = max(0, p.x);
   }
   if ((p.y != 0) && (docSize.cy < pageSize.cy))
   {
      p.y -= pageSize.cy - (docSize.cy - p.y);
      p.y  = max(0, p.y);
   }

   m_LogCurPos = GridToDoc(m_CurPos);

   ScrollToPosition (p); 
   if (pDoc->IsStandAloneDoc() || pDoc->IsInPlaceActive())
   {
      m_LastScrollPos = GetScrollPosition ();
   }

   if( m_bFontCreated )
   {
      ZoomFont();
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnCaptureChanged(CWnd *pWnd) 
{
   if( pWnd != this )
   {
      m_bIsCaptured = FALSE; 
   }
	
	CDbgEditorView::OnCaptureChanged(pWnd);
}


/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnDebug() 
{
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   m_bDebug = !m_bDebug;
   pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateWindowHint);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnUpdateDebug1(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_bDebug );
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnDebug2() 
{
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   m_bDebug2 = !m_bDebug2;
   pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateWindowHint);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnUpdateDebug2(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck( m_bDebug2 );
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnTimer(UINT nIDEvent) 
{
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   CClientDC dc(this);
   OnPrepareDC(&dc, NULL);

   // automatic scroll view near window edges
	switch ( pDoc->GetEditMode() ) {
      case EM_NORMAL:
      case EM_LINE_START:
      case EM_DRAG_MOVE:
      case EM_RESIZE_START:
      case EM_RESIZE_DIR:
      case EM_EDITPROP:
      case EM_INSCOL:
      case EM_INSCOL1:
      case EM_DELCOL:
      case EM_DELCOL1:
      case EM_INSROW:
      case EM_INSROW1:
      case EM_DELROW:
      case EM_DELROW1:
      case EM_MONITOR:
         break;

      case EM_RESIZE:
      case EM_LINE:
      case EM_SELECT:
      case EM_INSERT:
      case EM_MOVE:
         if (!m_NoAutoScroll)
         {
            CRect  r;
            CPoint p;

            m_bTick = true;
            m_iTickDelta = m_iZGrid;
            if      (m_bTickCount > 64)
            {
               m_iTickDelta = 32 * m_iZGrid;
            } 
            else if (m_bTickCount > 16)
            {
               m_iTickDelta = 16 * m_iZGrid;
            } 
            else if (m_bTickCount > 8)
            {
               m_iTickDelta = 8 * m_iZGrid;
            }
            else if (m_bTickCount > 4)
            {
               m_iTickDelta = 4 * m_iZGrid;
            }

            p = CPoint(0,0);
            GetClientRect(&r);
            r.OffsetRect( m_LastScrollPos );

            if (m_LogCurPos.x < r.left   + m_iDZGrid ) p.x = -m_iTickDelta;
            if (m_LogCurPos.x > r.right  - m_iDZGrid ) p.x =  m_iTickDelta;
            if (m_LogCurPos.y < r.top    + m_iDZGrid ) p.y = -m_iTickDelta;
            if (m_LogCurPos.y > r.bottom - m_iDZGrid ) p.y =  m_iTickDelta;

            if (p != CPoint(0,0)) 
            {
               // move origin of view
               ScrollToPosition( m_LastScrollPos + p); 
               if (pDoc->IsStandAloneDoc() || pDoc->IsInPlaceActive())
               {
                  m_LastScrollPos = GetScrollPosition ();
               }

               GetCursorPos(&p);
               SetCursorPos(p.x, p.y);

               m_dragFirst = true;
               Invalidate();

               UpdateWindow();

               m_bTickCount++;
            } else {
               m_bTickCount = 0;
            }
         }
         else
         {
            CRect  r;

            GetClientRect (&r);
            r.OffsetRect ( m_LastScrollPos );
            r.DeflateRect (4*m_iZGrid, 4*m_iZGrid);

            if (r.PtInRect (m_LogCurPos))
            {
               m_NoAutoScroll = false;
            }
         }
         break;
   }

// ---- test ----------------------------------------------------------------   
   if( m_bDebug )
   {  
      CPoint p, po;
	   CClientDC dc(this);
	   OnPrepareDC(&dc, NULL);

      po = m_LastScrollPos;
      
      char s[120];
      sprintf(s, "cur   :(%d,%d)  %c  CM:%d  ", m_CurPos.x, m_CurPos.y, m_bIsCaptured?'C':' ', m_eCursorMode); dc.TextOut(po.x+10,po.y+10,s,strlen(s));
      sprintf(s, "logcur:(%d,%d)   ", m_LogCurPos.x, m_LogCurPos.y); dc.TextOut(po.x+10,po.y+25,s,strlen(s));
      sprintf(s, "before:(%d,%d)   ", m_BeforeOpPos.x, m_BeforeOpPos.y); dc.TextOut(po.x+10,po.y+40,s,strlen(s));
      sprintf(s, "start :(%d,%d)   ", m_StartPos.x, m_StartPos.y); dc.TextOut(po.x+10,po.y+55,s,strlen(s));
      sprintf(s, "end   :(%d,%d)   ", m_EndPos.x, m_EndPos.y); dc.TextOut(po.x+10,po.y+70,s,strlen(s));

      CElem *pE = pDoc->GetElemList()->SearchElem(m_LogCurPos, m_iZGrid);
      if( (pE != NULL) && (pE->GetType() != ET_LINE))
      {
         p = pE->GetOrigin();
         sprintf(s, "elem! at(%d,%d)", p.x, p.y); 
         dc.TextOut(po.x+10,po.y+85,s,strlen(s));
      } else {
         sprintf(s, " - - - -                       "); 
         dc.TextOut(po.x+10,po.y+85,s,strlen(s));
      }

      POSITION pos;
      int      i;
      char    *pTyp;

      for( i=0,pos=pDoc->GetElemList()->GetHeadPosition(); pos != NULL; i++)
      {
         pE = (CElem*) pDoc->GetElemList()->GetNext( pos );

         if( pE )
         {
            switch (pE->GetType())
            {
               case ET_CONIN      :
               case ET_CONOUT     : pTyp = "CON";  break;

               case ET_LABEL      :
               case ET_JUMP       : pTyp = "LAB";  break;

               case ET_VARIN      :
               case ET_VAROUT     :
               case ET_VARINOUT   : pTyp = "VAR";  break;

               case ET_COMMENT    : pTyp = "CMT";  break;

               case ET_RETURN     : pTyp = "RET";  break;

               case ET_FB         :
               case ET_FUNCTION   :
               case ET_PROCEDURE  : pTyp = "FB";   break;

               case ET_LINE       : pTyp = "LINE"; break;

               case ET_STEP       : pTyp = "STEP"; break;
            
               case ET_TRANSITION : pTyp = "TRAN"; break;
            
               case ET_ACTION     :
               case ET_ACTIONBLOCK: pTyp = "ACT";  break;

               case ET_ALTDIV     :
               case ET_SIMDIV     : pTyp = "DIV";  break;

               default            : pTyp = "???";  break;
            }
            p = pE->GetOrigin();
            sprintf(s, "%3s O:(%d,%d)  S:(%d,%d) %c%c%c %08x         ", 
               pTyp, p.x, p.y, pE->GetSize().cx, pE->GetSize().cy,
               ( pE->ChkFlag(EF_COMMENT) ? 'C' : '_'), 
               ( pE->ChkFlag(EF_RUBBLINE) ? 'R' : '_'), 
               ( pE->ChkFlag(EF_SELECT) ? 'S' : '_'),
                 pE->GetFlag()
               );
            dc.TextOut(po.x+10,po.y+110+i*16,s,strlen(s));
         }
      }
      sprintf(s, "--- end ---" );
      dc.TextOut(po.x+10,po.y+110+i*15,s,strlen(s));
   }
// --------------------------------------------------------------------------   
	
	CDbgEditorView::OnTimer(nIDEvent);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
BOOL CGrEditorView::DestroyWindow() 
{
   if( m_uTimerID != 0 ) 
   {
      KillTimer(m_uTimerID);
      m_uTimerID = 0;
   }
	
	return CDbgEditorView::DestroyWindow();
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnEditComment() 
{
   CElem*       pTE;
   POSITION     pos;
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   if (!OnModifyContent ())
   {
      ::MessageBeep(-1);
      return;
   }
   for( pos = pDoc->GetElemList()->GetHeadPosition(); pos != NULL; )
   {
      pTE = pDoc->GetElemList()->GetNext(pos);
      ASSERT_VALID(pTE);
      if( pTE && pTE->ChkFlag(EF_SELECT) )
      {
         pTE->SetFlag(EF_COMMENT);
         pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateDrawElemHint, pTE);
      }
   }
   pDoc->m_Action.StoreState(pDoc->GetElemList());  // op. ok -> store
   pDoc->SetModifiedFlag(pDoc->m_Action.StateChangedQ());
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnEditUncomment() 
{
   CElem*       pTE;
   POSITION     pos;
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   if (!OnModifyContent ())
   {
      ::MessageBeep(-1);
      return;
   }
   for( pos = pDoc->GetElemList()->GetHeadPosition(); pos != NULL; )
   {
      pTE = pDoc->GetElemList()->GetNext(pos);
      ASSERT_VALID(pTE);
      if( pTE && pTE->ChkFlag(EF_SELECT) )
      {
         pTE->ClrFlag(EF_COMMENT);
         pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateDrawElemHint, pTE);
      }
   }
   pDoc->m_Action.StoreState(pDoc->GetElemList());  // op. ok -> store
   pDoc->SetModifiedFlag(pDoc->m_Action.StateChangedQ());
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnUpdateEditcol(CCmdUI* pCmdUI) 
{
   if(CheckReadonly())
   {
       pCmdUI->Enable(FALSE);
       return;
   }

   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   BOOL  b;

   b = (pDoc->GetEditMode() >= EM_INSCOL) && (pDoc->GetEditMode() <= EM_DELCOL1);
	pCmdUI->SetCheck( b );
   pCmdUI->Enable  ( (pDoc->GetEditMode() == EM_NORMAL) || b );
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnUpdateEditrow(CCmdUI* pCmdUI) 
{
   if(CheckReadonly())
   {
       pCmdUI->Enable(FALSE);
       return;
   }

   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   BOOL  b;

   b = (pDoc->GetEditMode() >= EM_INSROW) && (pDoc->GetEditMode() <= EM_DELROW1);
	pCmdUI->SetCheck( b );
   pCmdUI->Enable  ( (pDoc->GetEditMode() == EM_NORMAL) || b );
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::NextSearchPos()
{
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnEditCopy() 
{
   CRect   r;
   COleDataSource* pDataSource = NULL;
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   ASSERT_VALID(this);

   switch ( pDoc->GetEditMode() ) {
      case EM_EDITPROP   :
         if (m_pPropEdit) m_pPropEdit->Copy();
         if (m_pSEC_Edit) m_pSEC_Edit->OnEditCopy();
         break;

      case EM_NORMAL:
         {
            try
            {
               CXMLDragItem DragItem;

               DragItem.m_lObjClass = -1;          // object class id -> see cedb.h
               pDoc->GetElemList()->GenXMLData( DragItem.m_strSource, pDoc->GetPathName() ); // source stream

               CSharedFile GlobFile;
               CArchive  ar(&GlobFile, CArchive::store);
               DragItem.Serialize(ar);
               ar.Close();

               pDataSource = new COleDataSource;
               pDataSource->CacheGlobalData(m_uiCBFormatXMLId, GlobFile.Detach());
               pDataSource->SetClipboard();
            }
            catch(CException *e) 
            {
               delete pDataSource;
               e->Delete();
            }
         } 
         break;
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnEditCut() 
{
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   if (!OnModifyContent ())
   {
      ::MessageBeep(-1);
      return;
   }
   switch ( pDoc->GetEditMode() ) {
      case EM_EDITPROP   :
         if (m_pPropEdit) m_pPropEdit->Cut();
         if (m_pSEC_Edit) m_pSEC_Edit->OnEditCut();
         break;

      case EM_NORMAL:
         OnEditCopy();
         OnEditClear();
         break;
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnEditPaste() 
{
   COleDataObject clipboardData;
   CGrEditorDoc  *pDoc = GetDocument();

   if (!OnModifyContent ())
   {
      ::MessageBeep(-1);
      return;
   }
   if (pDoc)
   {
      switch ( pDoc->GetEditMode() ) {
         case EM_EDITPROP   :
            if (m_pPropEdit) m_pPropEdit->Paste();
            if (m_pSEC_Edit) m_pSEC_Edit->OnEditPaste();
            break;

         case EM_NORMAL:
            pDoc->GetElemList()->SignalChangeInit(true);
            clipboardData.AttachClipboard ();
            DoPasteItem (&clipboardData);
            pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateSelectionHint);
            break;
      }
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   ASSERT_VALID(this);

   if(CheckReadonly())
   {
       pCmdUI->Enable(FALSE);
       return;
   }

   BOOL bEnable;
   COleDataObject dataObject;

   switch ( pDoc->GetEditMode() ) {
      case EM_EDITPROP   :
         bEnable = dataObject.AttachClipboard() &&
                   dataObject.IsDataAvailable(CF_TEXT);
         // enable command based on availability
         pCmdUI->Enable( bEnable && (pDoc->GetEditMode() >= EM_NORMAL) && (pDoc->GetEditMode() < EM_MONITOR) );
         break;

      case EM_NORMAL:
         // determine if private or standard OLE formats are on the clipboard
         bEnable = dataObject.AttachClipboard() &&
                  (dataObject.IsDataAvailable(m_uiCBFormatXMLId));
         // enable command based on availability
         pCmdUI->Enable( bEnable && (pDoc->GetEditMode() >= EM_NORMAL) && (pDoc->GetEditMode() < EM_MONITOR) );
         break;

      default:
         pCmdUI->Enable( FALSE );
         break;
   }
   ASSERT_VALID(this);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnEditClear() 
{
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   if (!OnModifyContent ())
   {
      ::MessageBeep(-1);
      return;
   }
   switch ( pDoc->GetEditMode() ) {
      case EM_EDITPROP   :
         if (m_pPropEdit) m_pPropEdit->Clear();
         if (m_pSEC_Edit) m_pSEC_Edit->OnEditClear();
         break;

      case EM_NORMAL:
      case EM_DRAG_MOVE:
         DeleteElem();
         break;
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   ASSERT_VALID(this);
   
   BOOL  bEnable=FALSE;
   DWORD dw;

   switch ( pDoc->GetEditMode() ) {
      case EM_EDITPROP   :
         if( m_pPropEdit ) 
         {
            dw = m_pPropEdit->GetSel();
            if( ((dw >> 16) &0xFFFF) != (dw & 0xFFFF) ) 
            {
               bEnable = TRUE;
            }
         }
         if (m_pSEC_Edit)
         {
            m_pSEC_Edit->OnUpdateEditCopy(pCmdUI);
         } else {
            pCmdUI->Enable( bEnable );
         }
         break;

      case EM_NORMAL:
         // something marked ?, clipboard available ?
         pCmdUI->Enable( pDoc->GetElemList()->IsOneSelected() &&
                        (pDoc->GetEditMode() >= EM_NORMAL) && (pDoc->GetEditMode() < EM_MONITOR) );
         break;

      default:
         pCmdUI->Enable( FALSE );
         break;
   }
   ASSERT_VALID(this);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
   CRect        r;
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   ASSERT_VALID(this);
   
   BOOL  bEnable=FALSE;
   DWORD dw;

   if(CheckReadonly())
   {
       pCmdUI->Enable(FALSE);
       return;
   }

   switch ( pDoc->GetEditMode() ) {
      case EM_EDITPROP   :
         if( m_pPropEdit ) 
         {
            dw = m_pPropEdit->GetSel();
            if( ((dw >> 16) &0xFFFF) != (dw & 0xFFFF) ) 
            {
               bEnable = TRUE;
            }
         }
         if (m_pSEC_Edit)
         {
            m_pSEC_Edit->OnUpdateEditCut(pCmdUI);
         } else {
            pCmdUI->Enable( bEnable );
         }
         break;

      case EM_NORMAL:
         // something marked ?, clipboard available ?
         pCmdUI->Enable( pDoc->GetElemList()->IsOneSelected() &&
                        (pDoc->GetEditMode() >= EM_NORMAL) && (pDoc->GetEditMode() < EM_MONITOR) );
         break;

      default:
         pCmdUI->Enable( FALSE );
         break;
   }
   ASSERT_VALID(this);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnUpdateEditClear(CCmdUI* pCmdUI) 
{
   CRect        r;
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   ASSERT_VALID(this);
   
   BOOL  bEnable=FALSE;
   DWORD dw;

   if(CheckReadonly())
   {
       pCmdUI->Enable(FALSE);
       return;
   }

   switch ( pDoc->GetEditMode() ) {
      case EM_EDITPROP   :
         if( m_pPropEdit ) 
         {
            dw = m_pPropEdit->GetSel();
            if( ((dw >> 16) &0xFFFF) != (dw & 0xFFFF) ) 
            {
               bEnable = TRUE;
            }
         }
         if (m_pSEC_Edit)
         {
            m_pSEC_Edit->OnUpdateEditCut(pCmdUI);  // no clear found ?!
         } else {
            pCmdUI->Enable( bEnable );
         }
         break;

      case EM_NORMAL:
         // something marked ?, clipboard available ?
         pCmdUI->Enable( pDoc->GetElemList()->IsOneSelected() &&
                        (pDoc->GetEditMode() >= EM_NORMAL) && (pDoc->GetEditMode() < EM_MONITOR) );
         break;

      default:
         pCmdUI->Enable( FALSE );
         break;
   }
   ASSERT_VALID(this);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
BOOL CGrEditorView::DoPasteItem(COleDataObject* pDataObject, bool bWithMoveMode)
{
   CGrEditorDoc *pDoc = GetDocument();
   HGLOBAL  hGlobal=NULL;
   BOOL     bResult=FALSE;

   pDoc->GetElemList()->SignalChangeInit(true);

   ASSERT(pDataObject != NULL);
   ASSERT_VALID(pDoc);
   ASSERT_VALID(this);

   if (!OnModifyContent ())
   {
      ::MessageBeep(-1);
      return (FALSE);
   }
   BeginWaitCursor();

   if ( ( pDataObject->IsDataAvailable(m_uiCBFormatXMLId) && 
         ((hGlobal = pDataObject->GetGlobalData(m_uiCBFormatXMLId)) != NULL))/* ||
        ( pDataObject->IsDataAvailable(m_uiClipFormatId) &&
         ((hGlobal = pDataObject->GetGlobalData(m_uiClipFormatId)) != NULL))*/ )
   {
      CSharedFile GlobFile;
      GlobFile.SetHandle(hGlobal, FALSE);

      try {
         // invalidate current selection since it will be deselected
//       pDoc->m_Action.StoreState(pDoc->GetElemList());
//       pDoc->SetModifiedFlag(pDoc->m_Action.StateChangedQ());
         UnselectAll();

         CArchive  ar(&GlobFile, CArchive::load);
         CXMLDragItem DragItem;
         DragItem.Serialize(ar);

         // DragItem.m_lObjClass;        // object class id -> see cedb.h
         // DragItem.m_strSourceFile;    // source file
         LPCTSTR  pszXML = DragItem.m_strSource.GetBuffer (0);
         CMemFile file;
         file.Attach ((BYTE *)pszXML, DragItem.m_strSource.GetLength () * sizeof (TCHAR));
         CXMLArchive arx (&file, CArchive::load);

         arx.Load();
         CString   str;
         pDoc->GetElemList()->SerializeXMLSelected(arx, str); 
         pDoc->GetElemList()->CreateTempVars(false);
         DragItem.m_strSource.ReleaseBuffer ();

         CRect  r;
         pDoc->GetElemList()->GetSelectRect(r);
         pDoc->GetElemList()->UpdatePinStatus(true);

         // enough space at sheet for block ?
         if( (r.Width() > MAX_XGRID) || (r.Height() >= MAX_YGRID) )
         {
            AfxMessageBox( IDS_GR_CLIPTOOBIG, MB_ICONSTOP | MB_OK);
            OnDeleteSTActions(false);
            pDoc->m_Action.ReloadLastState(pDoc->GetElemList());
            pDoc->SetModifiedFlag(pDoc->m_Action.StateChangedQ());
            pDoc->GetElemList()->AddAllElemsToPinTable();
            pDoc->GetElemList()->bValidPos = !pDoc->GetElemList()->IsOneOverlapped();
            pDoc->UpdateAllViews(NULL);
         }
         else 
         {
            if (!bWithMoveMode)
            {
               CRect  r;

               pDoc->GetElemList()->GetSelectRect(r);
               GridToDoc(r);
               CPoint p = DocToGrid( m_LastScrollPos + m_dragPoint - r.TopLeft());
               p.x &= 0xfffffffe;
               p.y &= 0xfffffffe;
               pDoc->GetElemList()->MoveSelElems( p );
               if( pDoc->GetElemList()->IsOneOverlapped( false, EF_SELECT) ) 
               {
                  bWithMoveMode = true;
               }
            }
            if (bWithMoveMode)
            {  // goto to move/insert mode
               m_bFirstMove = false;
               pDoc->SetEditMode( EM_INSERT );
               pDoc->GetElemList()->MoveFlaggedToTail(EF_SELECT);
               pDoc->GetElemList()->MoveFlaggedToTail(EF_RUBBLINE);
               GrSetCapture();
               m_BeforeOpPos = m_StartPos = r.TopLeft();
            }
            // invalidate new pasted stuff
            pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateSelectionHint);
            pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateMarkedElemHint);
            // move cursor first in view - then we want to scroll ..
            m_NoAutoScroll = true;
         }
         // successfully retrieved all info's
         bResult = TRUE;
      }
      catch(CException *e) 
      {
         e->Delete();
      }
   }

   EndWaitCursor();

   return bResult;
}

/*func*------------------------------------------------------------------------
  insert clipboard data in data pool - uses m_ClipElem
  in :
  out:
-----------------------------------------------------------------------------*/
bool CGrEditorView::DoPasteItem(void)
{
   CGrEditorDoc *pDoc = GetDocument();
   bool   bResult=false;
   CRect  r;

   ASSERT_VALID(pDoc);
   ASSERT_VALID(this);

   if (!OnModifyContent ())
   {
      ::MessageBeep(-1);
      return (FALSE);
   }
   BeginWaitCursor();

   if (m_ClipElem.GetCount() > 0)
   {
      try {
         m_ClipElem.CreateTempVars(false);
         m_ClipElem.RecoverTempVars();
         // invalidate current selection since it will be deselected
         UnselectAll();

         pDoc->GetElemList()->SignalChangeInit(true);

         while (m_ClipElem.GetCount() > 0)
         {
            CElem *pE = (CElem*) m_ClipElem.RemoveHead();

            if (pE)
            {
               if (pE->ChkFlag(EF_NEW_LAYOUT))
               {
                  CSize stdSize;
                  CSize elemSize = pE->GetSize();
                  UINT  key;

                  switch(pE->GetType())
                  {
                  case ET_PROCEDURE: key = IDS_GR_REG_PRC_SIZE; break;
                  case ET_FB       : key = IDS_GR_REG_FBLKSIZE; break;
                  case ET_FUNCTION : key = IDS_GR_REG_FUN_SIZE; break;
                  default:
                     continue;
                  }

                  if(!m_bUseAutoSize)
                  {
                     stdSize = GetInsertSize(key);
                  } else {
                     ((CElemFblk*)pE)->OnAutoENO();
                     stdSize = CSize(((CElemFblk*)pE)->CalcOptWidth(), 0);
                  }
                  stdSize.cx -= elemSize.cx;
                  pE->Resize( 0,0, stdSize.cx, 0);
                  pE->ClrFlag(EF_NEW_LAYOUT);
               }
               pDoc->GetElemList()->InsertElem(pE);
            }
         }

         pDoc->GetElemList()->IsOneOverlapped();
         pDoc->GetElemList()->PurgeTempVars();

         pDoc->GetElemList()->GetSelectRect(r);
         pDoc->GetElemList()->UpdatePinStatus(true);

         if( pDoc->GetElemList()->IsOneOverlapped( true, EF_SELECT) ) 
         {
            // goto to move/insert mode
            m_bFirstMove = false;
            pDoc->SetEditMode( EM_INSERT );
            pDoc->GetElemList()->MoveFlaggedToTail(EF_SELECT);
            pDoc->GetElemList()->MoveFlaggedToTail(EF_RUBBLINE);
            GrSetCapture();
            m_BeforeOpPos = m_StartPos = r.TopLeft();
         }else
         {
            pDoc->GetElemList()->ConnectAllElems();
            pDoc->GetElemList()->DropInSelectedElems();  // adjust possible DropIn - Overlappings
         }

         // invalidate new pasted stuff
         pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateSelectionHint);
         pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateMarkedElemHint);
         // move cursor first in view - then we want to scroll ..
         m_NoAutoScroll = true;
         // successfully retrieved all info's
         bResult = true;
      }
      catch(CException *e) 
      {
         e->Delete();
      }
   }

   EndWaitCursor();

   return bResult;
}

/////////////////////////////////////////////////////////////////////////////
// support for drag/drop

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
BOOL CGrEditorView::GetObjectInfo(COleDataObject* pDataObject)
{
   CGrEditorDoc*pDoc = GetDocument();

   HGLOBAL  hGlobal=NULL;
   BOOL     bResult=FALSE;

   m_dragSize       = CSize(0,0);
   m_dragOffset     = CSize(0,0);

   if ( ( pDataObject->IsDataAvailable(m_uiCBFormatXMLId) && 
         ((hGlobal = pDataObject->GetGlobalData(m_uiCBFormatXMLId)) != NULL))/* ||
        ( pDataObject->IsDataAvailable(m_uiClipFormatId) &&
         ((hGlobal = pDataObject->GetGlobalData(m_uiClipFormatId)) != NULL))*/ )
   {
      CSharedFile GlobFile;
      GlobFile.SetHandle(hGlobal, FALSE);

      try {
         CArchive  ar(&GlobFile, CArchive::load);
         CXMLDragItem DragItem;
         DragItem.Serialize(ar);

         // DragItem.m_lObjClass;        // object class id -> see cedb.h
         // DragItem.m_strSourceFile;    // source file
         LPCTSTR  pszXML = DragItem.m_strSource.GetBuffer (0);
         CMemFile file;
         file.Attach ((BYTE *)pszXML, DragItem.m_strSource.GetLength () * sizeof (TCHAR));
         CXMLArchive arx (&file, CArchive::load);

         arx.Load();
         CString   str;
         m_ClipElem.DeleteAllElems();    // clear buffer

         ASSERT_VALID(pDoc);
         ASSERT(pDoc->GetElemList());
         m_ClipElem.SetIECLanguage(pDoc->GetElemList()->GetIECLanguange());

         m_ClipElem.SerializeXMLSelected(arx, str); 
         DragItem.m_strSource.ReleaseBuffer ();

         POSITION pos = m_ClipElem.GetHeadPosition();
         while (pos)
         {
            CElem* pE = (CElem*)m_ClipElem.GetNext(pos);

            if (pE && pE->ChkFlag(EF_NEW_LAYOUT))
            {
               CSize stdSize;
               CSize elemSize = pE->GetSize();
               UINT  key;

               switch(pE->GetType())
               {
               case ET_PROCEDURE: key = IDS_GR_REG_PRC_SIZE; break;
               case ET_FB       : key = IDS_GR_REG_FBLKSIZE; break;
               case ET_FUNCTION : key = IDS_GR_REG_FUN_SIZE; break;
               default:
                  continue;
               }

               if(!m_bUseAutoSize)
               {
                  stdSize = GetInsertSize(key);
               } else {
                  ((CElemFblk*)pE)->OnAutoENO();
                  stdSize = CSize(((CElemFblk*)pE)->CalcOptWidth(), 0);
               }
               stdSize.cx -= elemSize.cx;
               pE->Resize( 0,0, stdSize.cx, 0);
               pE->ClrFlag(EF_NEW_LAYOUT);
            }
         }

         CRect  r;
         m_ClipElem.GetSelectRect(r);
         GridToDoc(r);
         m_ClipElem.UpdatePinStatus(true);   // set rubberline flags !
         m_ClipElem.IsOneOverlapped();
         m_ClipElem.PurgeTempVars();

         m_dragSize.cx = r.Width();
         m_dragSize.cy = r.Height();
         m_dragOffset.cx = r.Width() / 2;
         m_dragOffset.cy = r.Height() / 2;

         m_prevDragPoint = r.TopLeft() +  m_dragOffset;

         // successfully retrieved all info's
         bResult = TRUE;
      }
      catch(CException *e) 
      {
         e->Delete();
      }
   }

   return bResult;
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
DROPEFFECT CGrEditorView::OnDragEnter(COleDataObject* pDataObject,
    DWORD grfKeyState, CPoint point)
{
   m_bDragDataAcceptable = GetObjectInfo(pDataObject);
   m_dragFirst  = true;
   m_dragActive = true;

   return OnDragOver(pDataObject, grfKeyState, point);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
DROPEFFECT CGrEditorView::OnDragOver(COleDataObject*, DWORD grfKeyState, 
   CPoint point)
{
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   POSITION pos;
   CElem   *pE;
   CRect    r;
   bool     bOverlapped=false;
   int      i;

   pDoc = GetDocument ();
   if (pDoc == NULL)
   {
		return DROPEFFECT_NONE;
   }

/*	if(!OnModifyContent()) 
   {
		return DROPEFFECT_NONE;
	}
*/
   if( (pDoc == NULL) || 
       (pDoc->GetEditMode() == EM_MONITOR) ||
       (m_bDragDataAcceptable == FALSE) )
   {
      return DROPEFFECT_NONE;
   }

   GetDocument()->ActivateInPlace();

   // check for point outside logical area -- i.e. in hatched region
   // GetTotalSize() returns the size passed to SetScrollSizes
   CRect rectScroll(CPoint(0, 0), GetTotalSize());

   CRect rectItem(point,m_dragSize);
   rectItem.OffsetRect(GetDeviceScrollPosition());

   DROPEFFECT de = DROPEFFECT_NONE;
   CRect      rectTemp;
   CString    str;
   CPoint     p = DocToGrid((GetDeviceScrollPosition() + point) - m_prevDragPoint);
   p.x = (p.x + 1) & 0xFFFFFFFE;
   p.y = (p.y + 1) & 0xFFFFFFFE;

   pos = m_ClipElem.GetHeadPosition();

   while (pos)
   {
      pE = (CElem*) m_ClipElem.GetNext(pos);
      if (pE)
      {
         pE->Resize(p.x,p.y,0,0);
         if (pDoc->GetElemList()->IsElemOverlapped(pE,false)) // skip container-update
         {
            bOverlapped = true;
         }
      } else {
         break;
      }
   }
   m_prevDragPoint += GridToDoc(p);
 
   if( // rectTemp.IntersectRect(rectScroll, rectItem) &&
       (rectScroll.left   <= rectItem.right ) &&
       (rectScroll.right  >= rectItem.left  ) &&
       (rectScroll.top    <= rectItem.bottom) &&
       (rectScroll.bottom >= rectItem.top   ) && 
      !bOverlapped)
   {
	   // check for force link
      if ((grfKeyState & (MK_CONTROL|MK_SHIFT)) == (MK_CONTROL|MK_SHIFT))
      {
         de = DROPEFFECT_NONE; // GrEditor isn't a linking container
      // check for force copy
      } else if ((grfKeyState & MK_CONTROL) == MK_CONTROL)
      {
         de = DROPEFFECT_COPY;
      // check for force move
      } else if ((grfKeyState & MK_ALT) == MK_ALT)
      {
         de = DROPEFFECT_MOVE;
      // default -- recommended action is move
      } else {
         de = DROPEFFECT_MOVE;
      }
   }
   // remove emphasizes
   pDoc->GetElemList()->ClearFlags(EF_EMPHASIZE);

   if (p == CPoint(0,0)) //point == m_dragPoint )
   {
      return de;
   }

   p = GridToDoc(p);
   // otherwise, cursor has moved -- need to update the drag feedback
   CClientDC dc(this);
   // 1. erase previous focus rect's
   // 2. draw next focus rect's
   if (m_dragFirst)
   {
      i = 1;
   } else { 
      i = 0;
   }
   for (; i<2; i++)
   {
      pos = m_ClipElem.GetHeadPosition();
      while (pos)
      {
         pE = (CElem*) m_ClipElem.GetNext(pos);
         if (pE)
         {
            pE->GetRect(r, m_iZGrid);
            if (i==0) 
            {
               r.OffsetRect(-p);
            }
            r.OffsetRect (-GetDeviceScrollPosition());
            dc.DrawFocusRect(r);
         }
      }
   }
   m_dragPoint = point;
   m_dragFirst = false;

   return de;
}                                                                      

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
BOOL CGrEditorView::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, 
   CPoint /*point*/)
{
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);
	ASSERT_VALID(this);

   if (!OnModifyContent ())
   {
      ::MessageBeep(-1);
      return (FALSE);
   }
   // clean up focus rect
	OnDragLeave();
   if (m_bDropSource && dropEffect == DROPEFFECT_MOVE)
   {
      m_bDropSource = false;
      OnEditClear();
   }
   InitSignalChangeDetect();
	if (DoPasteItem())
	{

      DetectSignalChange();

      pDoc->m_Action.StoreState(pDoc->GetElemList());  // op. ok -> store
      pDoc->SetModifiedFlag(pDoc->m_Action.StateChangedQ());

      return TRUE;
	}
	else
		return FALSE;
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnDragLeave()
{
   POSITION pos;
   CElem   *pE;
   CRect    r;

   CClientDC dc(this);
   // erase previous focus rect's
   pos = m_ClipElem.GetHeadPosition();
   while (pos)
   {
      pE = (CElem*) m_ClipElem.GetNext(pos);
      if (pE)
      {
         pE->GetRect(r, m_iZGrid);
         r.OffsetRect (-GetDeviceScrollPosition());
         dc.DrawFocusRect(r);
      }
   }
   m_dragActive = false;
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnUpdateEditor(CCmdUI* pCmdUI) 
{
	CGrEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
   ASSERT_VALID(this);
   pCmdUI->Enable( (pDoc->GetEditMode() >= EM_NORMAL) && (pDoc->GetEditMode() < EM_MONITOR) );
   ASSERT_VALID(this);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnUpdateEMNormal(CCmdUI* pCmdUI) 
{
	CGrEditorDoc* pDoc = GetDocument();
	ASSERT(pDoc);
   if (pDoc)
   {
      pCmdUI->Enable (pDoc->GetEditMode() == EM_NORMAL);
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnUpdateEdit(CCmdUI* pCmdUI) 
{
	CGrEditorDoc* pDoc = GetDocument();
	ASSERT(pDoc);
   if (pDoc)
   {
      pCmdUI->Enable (pDoc->GetEditMode() == EM_NORMAL && !CheckReadonly());
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnUpdateEditorLD(CCmdUI* pCmdUI) 
{
   if(CheckReadonly())
   {
       pCmdUI->Enable(FALSE);
       return;
   }
	CGrEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
   ASSERT_VALID(this);
   pCmdUI->Enable(((pDoc->GetCurrLang() == EL_LD) || 
                   (pDoc->GetCurrLang() == EL_ALL)) && 
                  (pDoc->GetEditMode() == EM_NORMAL));
   ASSERT_VALID(this);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnUpdateEditorSFC(CCmdUI* pCmdUI) 
{
   if(CheckReadonly())
   {
       pCmdUI->Enable(FALSE);
       return;
   }
	CGrEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
   ASSERT_VALID(this);
   pCmdUI->Enable( pDoc->GetSfcMode() && (pDoc->GetEditMode() == EM_NORMAL));
   ASSERT_VALID(this);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnUpdateEditorVarInOut(CCmdUI* pCmdUI) 
{
   if(CheckReadonly())
   {
       pCmdUI->Enable(FALSE);
       return;
   }
   CGrEditorDoc* pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   ASSERT_VALID(this);
   pCmdUI->Enable(((pDoc->GetCurrLang() == EL_FBD) || 
                   (pDoc->GetCurrLang() == EL_ALL)) && 
                  (pDoc->GetEditMode() == EM_NORMAL));
   ASSERT_VALID(this);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnZoom() 
{
   CZoomDlg       dlg;
   float          f;
	CGrEditorDoc  *pDoc = GetDocument();
	ASSERT_VALID(pDoc);

   // Initialize dialog data
   f = pDoc->m_fZoom;
   if     ( f == 4.0 ) dlg.m_iZoom = 0;
   else if( f == 2.0 ) dlg.m_iZoom = 1;
   else if( f == 1.0 ) dlg.m_iZoom = 2;
   else if( f == 0.5 ) dlg.m_iZoom = 3;
   else                dlg.m_iZoom = 4;
   dlg.m_fValue = f;

   // invoke the dialog box
   if (dlg.DoModal() == IDOK)
   {
      // retrieve the dialog data
      if     ( dlg.m_iZoom == 0 ) f = 4.0;
      else if( dlg.m_iZoom == 1 ) f = 2.0;
      else if( dlg.m_iZoom == 2 ) f = 1.0;
      else if( dlg.m_iZoom == 3 ) f = 0.5;
      else if( dlg.m_iZoom == 4 ) f = dlg.m_fValue;
      else                        f = 1.0;
      pDoc->m_fZoom = f;
      pDoc->ComputePageSize();         // includes view update
      pDoc->UpdateAllViews(NULL, CGrEditorDoc::SetPageSizeHint);

      CString strKey, strSec;
      strSec.LoadString(IDS_GR_REG_SECTION);
      strKey.LoadString(IDS_GR_REG_ZOOMVALUE);
      AfxGetApp()->WriteProfileInt ((LPCTSTR)strSec, (LPCTSTR)strKey, (int)(pDoc->m_fZoom * 100.0f));
   }

   // work around ..
   UpdateWatchpoints ();
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnChangeZoom(ZoomOp op)
{
   const float SmallZoom = 1.0f/6;

   CGrEditorDoc *pDoc = GetDocument();
   ASSERT(pDoc);
   float old = pDoc->m_fZoom;

   switch(op)
   {
   case zoom_in:
      pDoc->m_fZoom *= 2.0;
      break;
   
   case zoom_out:
      pDoc->m_fZoom /= 2.0;
      if( pDoc->GetGridSize() == 0 )
      {
         pDoc->m_fZoom = old;
      }
      break;

   case zoom_in_small:
      if(pDoc->m_fZoom < 5.0f - SmallZoom)
      {
         pDoc->m_fZoom += SmallZoom;
         pDoc->m_fZoom = (int)(pDoc->m_fZoom*100+0.5f)/100.0f;
      }
      break;
   
   case zoom_out_small:
      pDoc->m_fZoom -= SmallZoom;
      pDoc->m_fZoom = (int)(pDoc->m_fZoom*100+0.5)/100.0f;
      
      if( pDoc->GetGridSize() == 0 )
      {
         pDoc->m_fZoom = old;
      }
      break;
   
   case zoom_to_default:
      {
         CString strSec;
         CString strKey;

         strSec.LoadString(IDS_GR_REG_SECTION);
         strKey.LoadString(IDS_GR_REG_ZOOMVALUE);
         int i = AfxGetApp()->GetProfileInt ((LPCTSTR)strSec, (LPCTSTR)strKey, 100);
         pDoc->m_fZoom = (float)i / 100.0f;
      }
      break;
   
   default:
      return;
   }

   pDoc->ComputePageSize();
   pDoc->UpdateAllViews(NULL, CGrEditorDoc::SetPageSizeHint);
   UpdateWatchpoints ();   // workaround
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnZoomin() 
{
   OnChangeZoom(zoom_in);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnZoomout() 
{
   OnChangeZoom(zoom_out);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnUpdateZoom(CCmdUI* pCmdUI) 
{
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   pCmdUI->Enable(pDoc->GetEditMode() != EM_EDITPROP);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnPositionChanged(WPARAM wParam, LPARAM lParam)
{
   if(GetDocument()!=NULL)
	   GetDocument()->PositionChanged (CStdEditorPosition (EDP_STLINE, wParam, lParam));
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnOptSfc() 
{
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   if(!OnModifyContent())
   {
      ::MessageBeep(-1);
      return;
   }

   pDoc->m_Action.StoreState(pDoc->GetElemList());  // op. ok -> store
   pDoc->SetModifiedFlag(pDoc->m_Action.StateChangedQ());

   pDoc->SetSfcMode( !pDoc->GetSfcMode() );
   pDoc->SetModifiedFlag();

//   pDoc->GetElemList()->MarkAutoElems();
   InvalidateMarkedElem();

   if (pDoc->GetSfcMode())
   {
      pDoc->SetCurrLang(EL_ALL);
   } 
   else 
   {
      CStatisticData data;
      pDoc->GetElemList ()->GetStatisticData(data);

      if (data.m_iSUM_SFC > 0) 
      {
         pDoc->SetCurrLang(EL_ALL);
      }
      else 
      {
         if     ((data.m_iSUM_FBD > 0) && 
                 (data.m_iSUM_LD == 0))
         {
            pDoc->SetCurrLang(EL_FBD);
         } 
         else if((data.m_iSUM_FBD == 0) && 
                 (data.m_iSUM_LD  >  0))
         {
            pDoc->SetCurrLang(EL_LD);
         } 
         else 
         {
            pDoc->SetCurrLang(EL_ALL);
         }
      }
   }

   if( pDoc->GetElemList()->IsOneOverlapped() ) 
   {
      pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateMarkedElemHint);
      
      CPoint p;      
      GetCursorPos(&p);  // save actual cursor position ..

      AfxMessageBox( IDS_GR_ERR_INVALID_POS, MB_ICONSTOP | MB_OK);
      OnDeleteSTActions(false);
      pDoc->m_Action.ReloadLastState(pDoc->GetElemList());
      pDoc->SetModifiedFlag(pDoc->m_Action.StateChangedQ());
      pDoc->GetElemList()->AddAllElemsToPinTable();
      pDoc->GetElemList()->bValidPos = !pDoc->GetElemList()->IsOneOverlapped();
      pDoc->UpdateAllViews(NULL);

      SetCursorPos(p.x,p.y);  // restore cursor position
      GrReleaseCapture();
   }

   pDoc->GetElemList()->CreateTempVars(true);
   pDoc->GetElemList()->PurgeTempVars();
   InvalidateMarkedElem();

}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnOptStdSize()
{
   m_bUseAutoSize = !m_bUseAutoSize;

   CString strKey, strSec;
   strSec.LoadString(IDS_GR_REG_SECTION);
   strKey.LoadString(IDS_GR_REG_AUTOSIZE);
   AfxGetApp()->WriteProfileInt (strSec, strKey, m_bUseAutoSize? 1 : 0);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnUpdateOptStdSize(CCmdUI* pCmdUI) 
{
   if(CheckReadonly())
   {
       pCmdUI->Enable(FALSE);
       return;
   }

   CGrEditorDoc* pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   ASSERT_VALID(this);
   pCmdUI->Enable (pDoc->GetEditMode() == EM_NORMAL);
   
   CString strSec; strSec.LoadString(IDS_GR_REG_SECTION);
   CString strKey; strKey.LoadString(IDS_GR_REG_AUTOSIZE);
   int i = AfxGetApp()->GetProfileInt (strSec, strKey, 0);
   m_bUseAutoSize = (i==1);
	pCmdUI->SetCheck( m_bUseAutoSize  );

}
/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnOptChangeWarn()
{
   m_bWarnOnSignalChange = !m_bWarnOnSignalChange;

   CString strKey, strSec;
   strSec.LoadString(IDS_GR_REG_SECTION);
   strKey.LoadString(IDS_GR_REG_CHANGEWARN);
   AfxGetApp()->WriteProfileInt (strSec, strKey, m_bWarnOnSignalChange? 1 : 0);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnUpdateOptChangeWarn(CCmdUI* pCmdUI) 
{

   CGrEditorDoc* pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   ASSERT_VALID(this);
   pCmdUI->Enable (pDoc->GetEditMode() == EM_NORMAL);
   
   CString strSec; strSec.LoadString(IDS_GR_REG_SECTION);
   CString strKey; strKey.LoadString(IDS_GR_REG_CHANGEWARN);
   int i = AfxGetApp()->GetProfileInt (strSec, strKey, 0);
   m_bWarnOnSignalChange = (i==1);
	pCmdUI->SetCheck( m_bWarnOnSignalChange );

}
/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnUpdateOptSfc(CCmdUI* pCmdUI) 
{
   if(CheckReadonly())
   {
       pCmdUI->Enable(FALSE);
       return;
   }
	CGrEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
   ASSERT_VALID(this);
   pCmdUI->Enable (pDoc->GetEditMode() == EM_NORMAL);
	pCmdUI->SetCheck( pDoc->GetSfcMode() );
   ASSERT_VALID(this);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnEditUndo() 
{
   CWaitCursor wc;

   CGrEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

   if (!OnModifyContent ())
   {
      ::MessageBeep(-1);
      return;
   }
   switch ( pDoc->GetEditMode() ) {
      case EM_EDITPROP   :
         if (m_pPropEdit) m_pPropEdit->Undo();
         if (m_pSEC_Edit) m_pSEC_Edit->OnEditUndo();
         break;

      case EM_NORMAL    :
      case EM_LINE_START:
      case EM_INSCOL    :
      case EM_INSCOL1   :
      case EM_DELCOL    :
      case EM_DELCOL1   :
      case EM_INSROW    :
      case EM_INSROW1   :
      case EM_DELROW    :
      case EM_DELROW1   :
         DeleteSTViews();
         pDoc->m_Action.LoadState(pDoc->GetElemList());
         pDoc->SetModifiedFlag(pDoc->m_Action.StateChangedQ());
         pDoc->GetElemList()->AddAllElemsToPinTable();
         pDoc->GetElemList()->bValidPos = !pDoc->GetElemList()->IsOneOverlapped();
         pDoc->GetElemList()->ClearFlags(EF_LIGHTPIN);
         CreateSTViews();
         GetParentFrame()->SetActiveView(this);
         SetFocus();
         pDoc->UpdateAllViews(NULL);
         break;

      default:
         break;
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
   if(CheckReadonly())
   {
       pCmdUI->Enable(FALSE);
       return;
   }
    
   CGrEditorDoc* pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   ASSERT_VALID(this);


   switch ( pDoc->GetEditMode() ) {
      case EM_EDITPROP   :
         if (m_pPropEdit) 
         {
            pCmdUI->Enable (m_pPropEdit->CanUndo());
         }
         if (m_pSEC_Edit) 
         {
	         SECEdit* pEdit = m_pSEC_Edit->GetEdit();
	         ASSERT(pEdit != NULL);

            pCmdUI->Enable (pEdit->m_iUndoPos);
         }
         break;

      default:
         pCmdUI->Enable( pDoc->m_Action.IsLoadAvail() &&
                         ((pDoc->GetEditMode() == EM_NORMAL)     ||
                          (pDoc->GetEditMode() == EM_LINE_START) ||
                          (pDoc->GetEditMode() >= EM_INSCOL) && (pDoc->GetEditMode() <= EM_DELROW1)) );
         break;
   }
   ASSERT_VALID(this);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnEditRedo() 
{
   CWaitCursor wc;
	CGrEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

   if (!OnModifyContent ())
   {
      ::MessageBeep(-1);
      return;
   }
   switch ( pDoc->GetEditMode() ) {
      case EM_NORMAL    :
      case EM_LINE_START:
      case EM_INSCOL    :
      case EM_INSCOL1   :
      case EM_DELCOL    :
      case EM_DELCOL1   :
      case EM_INSROW    :
      case EM_INSROW1   :
      case EM_DELROW    :
      case EM_DELROW1   :
         DeleteSTViews();         
         pDoc->m_Action.ReloadState(pDoc->GetElemList());
         pDoc->SetModifiedFlag(pDoc->m_Action.StateChangedQ());
         pDoc->GetElemList()->AddAllElemsToPinTable();
         pDoc->GetElemList()->bValidPos = !pDoc->GetElemList()->IsOneOverlapped();
         pDoc->GetElemList()->ClearFlags(EF_LIGHTPIN);
         CreateSTViews();
         GetParentFrame()->SetActiveView(this);
         SetFocus();
         pDoc->UpdateAllViews(NULL);
         break;

      default:
         break;
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
   if(CheckReadonly())
   {
       pCmdUI->Enable(FALSE);
       return;
   }

   CGrEditorDoc* pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   ASSERT_VALID(this);
   pCmdUI->Enable( pDoc->m_Action.IsReloadAvail() &&
                   ((pDoc->GetEditMode() == EM_NORMAL)     ||
                    (pDoc->GetEditMode() == EM_LINE_START) ||
                    (pDoc->GetEditMode() >= EM_INSCOL) && (pDoc->GetEditMode() <= EM_DELROW1)) );
   ASSERT_VALID(this);
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnStepToggleinit() 
{
   CElem       *pE;
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   if (!OnModifyContent ()) 
   {
      ::MessageBeep(-1);
      return; 
   }
   if( ((pE = pDoc->GetElemList()->SearchElem(m_LogCurPos, m_iZGrid)) != NULL) && 
        (pE->GetType() == ET_STEP) ) 
   {
      ((CElemStep*)pE)->SetInitType( ! ((CElemStep*)pE)->GetInitType() );
      pDoc->m_Action.StoreState(pDoc->GetElemList());  // op. ok -> store
      pDoc->SetModifiedFlag(pDoc->m_Action.StateChangedQ());
      pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateDrawElemHint, pE);
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnUpdateEditInstamce(CCmdUI* pCmdUI) 
{
   if(CheckReadonly())
   {
       pCmdUI->Enable(FALSE);
       return;
   }

   CElem       *pE;
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   pCmdUI->Enable( 
      ((pE = pDoc->GetElemList()->SearchElem(m_LogCurPos, m_iZGrid)) != NULL) && 
       (pE->GetType() == ET_FB) );
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnEditActQual(UINT nID) 
{
   CElem       *pE;
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   QUAL_TYPE   eQualifier;

   if (!OnModifyContent ())
   {
      ::MessageBeep(-1);
      return;
   }
   if( ((pE = pDoc->GetElemList()->SearchElem(m_LogCurPos, m_iZGrid)) != NULL) && 
        ((pE->GetType() == ET_ACTION) || 
         (pE->GetType() == ET_ACTIONBLOCK)) ) 
   {
      ASSERT((nID >= ID_GR_EDIT_ACT_NONE) && (nID <= ID_GR_EDIT_ACT_P1));
      eQualifier = (QUAL_TYPE)(nID - ID_GR_EDIT_ACT_NONE + QT_none);
      ((CElemAct*)pE)->SetQualifier(eQualifier);
      pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateDrawElemHint, pE);
      pDoc->m_Action.StoreState(pDoc->GetElemList());  // op. ok -> store
      pDoc->SetModifiedFlag(pDoc->m_Action.StateChangedQ());
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::TrackWP (CPoint point)
{
   CPrgWPClientData *pClientData;
	CRect rect;
	CRect wpArea;
	int xDelta;
	int yDelta;

	GetClientRect (&rect);

	if (point.x < rect.left || point.x >= rect.right ||
	    point.y < rect.top  || point.y >= rect.bottom)
	{
		return;
	}

   pClientData = (CPrgWPClientData *)m_pTrackedWP->GetClientData ();
	ASSERT (pClientData != NULL);
	if (pClientData == NULL)
		return;

	wpArea = pClientData->GetArea ();
   wpArea.OffsetRect( -m_LastScrollPos );
   InvalidateRect (&wpArea);

   wpArea = pClientData->GetArea ();

   point += m_LastScrollPos;
	xDelta = point.x - (wpArea.left + m_WPGrabPosition.x);
	yDelta = point.y - (wpArea.top + m_WPGrabPosition.y);

	wpArea.left   += xDelta;
	wpArea.right  += xDelta;
	wpArea.top    += yDelta;
	wpArea.bottom += yDelta;

   pClientData->SetArea (wpArea);
   wpArea.OffsetRect( -m_LastScrollPos );
   InvalidateRect (&wpArea);

	UpdateWindow ();
}


/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnInsertFBCall() 
{
	CGrEditorDoc *pDoc;
	HRESULT hr;

   if (!OnModifyContent ()) 
   {
      ::MessageBeep(-1);
      return; 
   }
	pDoc = GetDocument ();
	if (pDoc == NULL)
		return;

	hr = pDoc->SelectFBForCall ();
}


/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnInsertFBCallHint (const CString &strXML)
{
	CString  strTemp=strXML;
   CString  str;
   CMemFile file;
	LPCTSTR  pszXML;
   HRESULT  hr=S_OK;
   POSITION pos2;
   CRect    r;
	CGrEditorDoc *pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   if (!OnModifyContent () || pDoc->GetLastActiveView()!=this ) 
   {
      ::MessageBeep(-1);
      return; 
   }else
   {
      if(m_pSEC_Edit)
      {
         m_pSEC_Edit->InsertFBCall(strXML);
         return;
      }
   }

   if( pDoc->GetEditMode() == EM_MONITOR ) 
   {
      return;   // FBCall-dialog can be open - but not for me!
   }
   
   // invalidate current selection since it will be deselected
   pDoc->m_Action.StoreState(pDoc->GetElemList());
   pDoc->SetModifiedFlag(pDoc->m_Action.StateChangedQ());
   pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateSelectionHint);

	try
	{
      pszXML = strTemp.GetBuffer (0);
	   file.Attach ((BYTE *)pszXML, strTemp.GetLength () * sizeof (TCHAR));

		CXMLArchive ar (&file, CArchive::load);

      ar.Load();

      m_ClipElem.DeleteAllElems();    // clear buffer
      m_ClipElem.SetIECLanguage(pDoc->GetElemList()->GetIECLanguange());
      m_ClipElem.SerializeXMLSelected(ar, str); 
      POSITION pos = m_ClipElem.GetHeadPosition();
      bool bFirst=true;
      while (pos)
      {
         CElem*    pE = (CElem*)m_ClipElem.GetNext(pos);
         CElemVar *pEV;
         CSize stdSize;
         CSize elemSize;
         UINT  key;

         if (pE && pE->ChkFlag(EF_SELECT))
         {
            switch(pE->GetType())
            {
               case ET_PROCEDURE: 
               case ET_FB       : 
               case ET_FUNCTION : 
                  if (bFirst)
                  {
                     UnselectAll();
                     bFirst = false;
                  }
                            m_ClipElem.RemoveElem(pE);
                  pDoc->GetElemList()->InsertElem(pE);

                  elemSize = pE->GetSize();
                  switch(pE->GetType())
                  {
                     case ET_PROCEDURE: key = IDS_GR_REG_PRC_SIZE; break;
                     case ET_FB       : key = IDS_GR_REG_FBLKSIZE; break;
                     case ET_FUNCTION : key = IDS_GR_REG_FUN_SIZE; break;
                     default:
                        ASSERT(0); break;
                  }

                  if(!m_bUseAutoSize)
                  {
                     stdSize = GetInsertSize(key);
                  } else {
                     ((CElemFblk*)pE)->OnAutoENO();
                     stdSize = CSize(((CElemFblk*)pE)->CalcOptWidth(), 0);
                  }
                  stdSize.cx -= elemSize.cx;
                  pE->Resize( 0,0, stdSize.cx, 0);
                  pE->ClrFlag(EF_NEW_LAYOUT);
                  break;

               case ET_VARIN    : 
                  pEV = (CElemVar*)pE;
                  hr = -1;
                  // check for edited elements ..
                  if (m_pPropElem)
                  {
                     switch (m_pPropElem->GetType())
                     {
                     case ET_VARIN     :
                     case ET_VAROUT    :
                     case ET_VARINOUT  : 
                     case ET_CONTACT   :
                     case ET_COIL      :
                     case ET_TRANSITION:  // possible destination ..
                        if (m_pPropEdit)
                        {
                           m_pPropEdit->SetSel(0,-1);
                           m_pPropEdit->ReplaceSel(pEV->GetName(),TRUE); 
                        }
                        break;
                     };
                  }
                  // check for selected elements ..
                  pos2 = pDoc->GetElemList()->GetHeadPosition();
                  while (pos2)
                  {
                     CElem* pE = (CElem*)pDoc->GetElemList()->GetNext(pos2);

                     if (pE && pE->ChkFlag(EF_SELECT))
                     {
                        switch (pE->GetType())
                        {
                        case ET_VARIN     :
                        case ET_VAROUT    :
                        case ET_VARINOUT  : 
                           ((CElemVar*)pE)->SetName(pEV->GetName()); 
                           break;

                        case ET_CONTACT   :
                        case ET_COIL      :
                           ((CElemLdBase*)pE)->SetName(pEV->GetName()); 
                           break;

                        case ET_TRANSITION:  
                           ((CElemTran*)pE)->SetName(pEV->GetName()); 
                           break;
                        }
                     }
                  }
                  break;
            }
         }
      }
      m_ClipElem.DeleteAllElems();    // clear buffer
	}

	catch (HRESULT hrx)
	{
		FC_DebugPrint ("Failed to insert XML (hr=%08lx)", hr);

      OnDeleteSTActions(false);
      pDoc->m_Action.ReloadLastState(pDoc->GetElemList());
      pDoc->SetModifiedFlag(pDoc->m_Action.StateChangedQ());
      pDoc->GetElemList()->AddAllElemsToPinTable();
      pDoc->GetElemList()->bValidPos = !pDoc->GetElemList()->IsOneOverlapped();
      pDoc->UpdateAllViews(NULL);
      hr = hrx;
	}

	if (SUCCEEDED (hr))
   {
      // goto to move mode
      m_bFirstMove = false;
      m_bIgnoreForFBCall = true;
      pDoc->SetEditMode( EM_INSERT );
      GrSetCapture();
      pDoc->GetElemList()->MoveFlaggedToTail(EF_SELECT);
      pDoc->GetElemList()->MoveFlaggedToTail(EF_RUBBLINE);
      pDoc->GetElemList()->GetSelectRect(r);
      m_BeforeOpPos = m_StartPos = CPoint(r.left, r.top);
      // invalidate new pasted stuff
      pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateSelectionHint);
      // move cursor first in view - then we want to scroll ..
      m_NoAutoScroll = true;
   }

	strTemp.ReleaseBuffer ();
}


/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
   CGrEditorDoc *pDoc = GetDocument();
   CRect rect;
   
   CDbgEditorView::OnHScroll( nSBCode, nPos, pScrollBar);
   if( pDoc )
   {
      switch ( pDoc->GetEditMode() )
      {
         case EM_MONITOR:
            GetClientRect( &rect );
            rect.bottom = rect.top + m_iDZGrid + 1;
            InvalidateRect(&rect);

            GetClientRect( &rect );
            rect.top = max( 0, (rect.bottom - m_iDZGrid - 1));
            InvalidateRect(&rect);
            break;
      }
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
   CGrEditorDoc *pDoc = GetDocument();
   CRect rect;

   CDbgEditorView::OnVScroll( nSBCode, nPos, pScrollBar);
   if( pDoc )
   {
      switch ( pDoc->GetEditMode() )
      {
         case EM_MONITOR:
            GetClientRect( &rect );
            rect.right = rect.left + m_iDZGrid + 1;
            InvalidateRect(&rect);

            GetClientRect( &rect );
            rect.left = max( 0 , (rect.right - m_iDZGrid - 1));
            InvalidateRect(&rect);
            break;
      }
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
BOOL CGrEditorView::OnScrollBy(CSize sizeScroll, BOOL bDoScroll)
{
   CGrEditorDoc *pDoc = GetDocument();
   BOOL  bResult;

   bResult = CDbgEditorView::OnScrollBy( sizeScroll, bDoScroll);
   if( bResult )
   {  
      if (pDoc)
      {
         if (pDoc->IsStandAloneDoc() || pDoc->IsInPlaceActive())
         {
            m_LastScrollPos = GetScrollPosition ();
         }
      }
   }
   m_dragFirst = true;
   Invalidate();

   return bResult;
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnIdle ()
{
   // work around !!!
   // SECedit lose the caret at mouse klicks in STEditor (STH) and 
   // will never can hold the caret in further time.
   // So we reconstruct the caret ...

   CGrEditorDoc *pDoc = GetDocument();
   
   if( (pDoc != NULL) && 
       (pDoc->GetEditMode() == EM_EDITPROP) &&
       (m_pSEC_Edit != NULL)) 
   {
      // For strange reason the caret gets lost under some circumstances
      // (e.g. inserting text as a reaction to context menu selection).
      // This method is used to destroy and then again create the caret
      // which normally helps showing the caret where the caret should
      // already be visible.

      if (!m_pSEC_Edit->m_bHaveCaret)
      {
         ::CreateCaret (m_pSEC_Edit->m_hWnd, NULL, 2, m_pSEC_Edit->m_iLineHeight);
         m_pSEC_Edit->m_bHaveCaret = TRUE;

         m_pSEC_Edit->ShowCaret ();
         m_pSEC_Edit->MySetCaretPos();
      }
   }

   
   if (m_LastPos == m_CurPos)
	{
		return;
	};

	m_LastPos = m_CurPos;

   PostMessage(IDS_GR_ON_UPDATE_POSITION, m_LastPos.x, m_LastPos.y);
   /*
   ASSERT(pDoc);
   if(pDoc != NULL)
	   pDoc->PositionChanged (CStdEditorPosition (m_LastPos.x, m_LastPos.y));
   */
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
   if( pDC->IsPrinting() ) 
   {
      if( pInfo != NULL )
      {
         CGrEditorDoc *pDoc = GetDocument();
         ASSERT_VALID(pDoc);

         pDoc->PreparePrintDC( pDC, pInfo->m_nCurPage - 1);
      }
   } 
   else if (m_DrawTarget == ActiveView)
   {
      CDbgEditorView::OnPrepareDC(pDC, pInfo);
   }
   else 
   { 
      pDC->SetMapMode(MM_TEXT);
      pDC->SetWindowOrg(m_LastScrollPos);
      pDC->SetViewportOrg(CPoint(0,0));
   }
}

/////////////////////////////////////////////////////////////////////////////
// CGrEditorView printing

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
BOOL CGrEditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
   BOOL bResult;
   
   // Calculate max page
   CGrEditorDoc *pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   
   CRect r;
   CPoint p = pDoc->GetPageGrid();
   pDoc->GetElemList()->GetAllElemRect(r);              // in grid
   int i = ((r.right + p.x - 1) / p.x) * ((r.bottom + p.y - 1) / p.y);
   pInfo->SetMaxPage((i > 1) ? i : 1);

	// default preparation
   bResult = DoPreparePrinting(pInfo);

   if( i <= 1 )
      pInfo->m_nNumPreviewPages = 1;

	return bResult;
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnBeginPrinting(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing

   POSITION pos = NULL;
	if (AfxGetApp()->m_pDocManager != NULL)
   {
		pos = AfxGetApp()->m_pDocManager->GetFirstDocTemplatePosition();
   }

	while (pos != NULL)
	{
		CDocTemplate* pTemplate = AfxGetApp()->m_pDocManager->GetNextDocTemplate(pos);
		ASSERT_KINDOF(CDocTemplate, pTemplate);

		POSITION pos2 = pTemplate->GetFirstDocPosition();
		while (pos2 != NULL)
		{
			CDocument* pDocument = pTemplate->GetNextDoc(pos2);
         CGrEditorDoc *pDoc = dynamic_cast<CGrEditorDoc *>(pDocument);
         if( pDoc != NULL )
         {
             m_fOldZoom = pDoc->SetZoom(1.0);
             pDoc->ComputePageSize();
             pDoc->InitPageSize(pDC);
             pDoc->SetMarginRect(TRUE);
             pDoc->ComputePageSize();
             pDoc->UpdateAllViews(NULL, CGrEditorDoc::SetPageSizeHint);
         }
      }
	}
}


/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnEndPrinting(CDC* pDC, CPrintInfo* /*pInfo*/)
{
   POSITION pos = NULL;
   if (AfxGetApp()->m_pDocManager != NULL)
   {
      pos = AfxGetApp()->m_pDocManager->GetFirstDocTemplatePosition();
   }

   while (pos != NULL)
   {
      CDocTemplate* pTemplate = AfxGetApp()->m_pDocManager->GetNextDocTemplate(pos);
      ASSERT_KINDOF(CDocTemplate, pTemplate);

      POSITION pos2 = pTemplate->GetFirstDocPosition();
      while (pos2 != NULL)
      {
         CDocument* pDocument = pTemplate->GetNextDoc(pos2);
         CGrEditorDoc *pDoc = dynamic_cast<CGrEditorDoc *>(pDocument);
         if( pDoc != NULL )
         {
             pDoc->SetZoom(m_fOldZoom);
             pDoc->ComputePageSize();
             pDoc->InitPageSize(pDC);
             pDoc->SetMarginRect(TRUE);
             pDoc->UpdateAllViews(NULL, CGrEditorDoc::SetPageSizeHint);
         }
      }
   }
}


/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
   CFont Font, *pOldFont;
   CPen  Pen,  *pOldPen;
   CGrEditorDoc *pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   // Clear zoom factor
   float fOldZoom = pDoc->SetZoom(1.0);
   pDoc->ComputePageSize();

   // Set font and pen
   Font.CreateFont(
      -((m_iDZGrid*FontToGrid)/100),0,0,0,0,0,0,0,0,0,0,0,0,pDoc->GetFontName());
   pOldFont = pDC->SelectObject(&Font);
   Pen.CreatePen(PS_SOLID, 0, RGB(0,0,0)); 
   pOldPen = pDC->SelectObject(&Pen);

   CRect rectMarg = pDoc->GetMargin() + pDC->GetWindowOrg();
	CRect rectPage = rectMarg;

   pDC->SelectClipRgn(NULL);   

   CString strHelp;
   
   // draw rectangle
	pDC->MoveTo(rectMarg.left,  rectMarg.top);
	pDC->LineTo(rectMarg.right, rectMarg.top);
	pDC->LineTo(rectMarg.right, rectMarg.bottom);
	pDC->LineTo(rectMarg.left,  rectMarg.bottom);
	pDC->LineTo(rectMarg.left,  rectMarg.top);

   // print header right aligned
   strHelp.Format(IDS_GR_PRINTHEADER);
   int iHeight = pDC->GetTextExtent(strHelp).cy;
   rectPage.top -= 2 * iHeight;
   pDC->SetTextAlign(TA_RIGHT | TA_BOTTOM);
   pDC->TextOut(rectMarg.right, rectPage.top, strHelp);
	rectPage.top += iHeight / 2;
	pDC->MoveTo(rectMarg.left, rectPage.top);
	pDC->LineTo(rectMarg.right, rectPage.top);

   // print footer
   SYSTEMTIME SysTime;
   FILETIME FileTime;
   GetSystemTime(&SysTime);
   SystemTimeToFileTime(&SysTime, &FileTime);
   COleDateTime OleTime(FileTime);
   CSize Size;

	rectPage.bottom += 2 * iHeight;
	pDC->MoveTo(rectMarg.left, rectPage.bottom);
	pDC->LineTo(rectMarg.right, rectPage.bottom);

   // print date and time left aligned
	rectPage.bottom += iHeight / 2;
   strHelp = OleTime.Format();
   pDC->SetTextAlign(TA_LEFT | TA_TOP);
	pDC->TextOut(rectMarg.left, rectPage.bottom, strHelp);
   // print filename center aligned
   LPTSTR psz = strHelp.GetBuffer(_MAX_FNAME + _MAX_EXT);
   CString str = pDoc->GetPathName();
   if( strlen(str) == 0 )
      str = pDoc->GetTitle();
	AfxGetFileName((LPCTSTR)str, psz, _MAX_FNAME + _MAX_EXT);
   strHelp.ReleaseBuffer();
   pDC->SetTextAlign(TA_CENTER | TA_TOP);
	pDC->TextOut((rectMarg.right + rectMarg.left) / 2, rectPage.bottom, strHelp);
   // print page number right aligned
   strHelp.Format(IDS_GR_PRINTFOOTER, pInfo->m_nCurPage, pInfo->GetMaxPage( ));
   pDC->SetTextAlign(TA_RIGHT | TA_TOP);
	pDC->TextOut(rectMarg.right, rectPage.bottom, strHelp);
   
	pInfo->m_rectDraw = rectMarg;
   
   // Offset viewport Origin by top and left margins
   pDC->LPtoDP(&rectMarg);
   pDC->SetViewportOrg(rectMarg.TopLeft());

   // Cut the margins by using a clipping region.
   // If print preview mode, convert device units back to logical units,
   // then convert back to device units using the actual screen dc
   // this is necessary because clipping regions are specified using 
   // device units, and aren't scaled by print preview                  
   if (pInfo->m_bPreview)
   {
      pDC->DPtoLP(&rectMarg);
      CDC::FromHandle(pDC->m_hDC)->LPtoDP(&rectMarg);
   }
   
   // Create and select the clipping region.
   CRgn rgn;
   rgn.CreateRectRgnIndirect(&rectMarg);   
   pDC->SelectClipRgn(&rgn);   

   // Restore font and pen
   pDC->SelectObject(pOldFont);
   pDC->SelectObject(pOldPen );

	// draw body text
	CDbgEditorView::OnPrint(pDC, pInfo);
   
   // Restore zoom factore
   pDoc->SetZoom(fOldZoom);
   pDoc->ComputePageSize();
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnContextMenuKey ()
{
	CRect rect;
	CPoint point;

	GetClientRect (rect);
	ClientToScreen (&rect);
	point.x = rect.left + 50;
	point.y = rect.top + 30;
	SendMessage (WM_CONTEXTMENU, (WPARAM)m_hWnd, MAKELPARAM (point.x, point.y));
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnGrRoute() 
{
   bool bUpdate = false;
   CGrEditorDoc *pDoc = GetDocument();

   if (!OnModifyContent ())
   {
      ::MessageBeep(-1);
      return;
   }

   CWaitCursor wait;

   /* unroute first */
   bUpdate |= pDoc->GetElemList()->UnrouteFlaggedLines(false, EF_SELECT);
   bUpdate |= pDoc->GetElemList()->UnrouteFlaggedLines(true,  EF_SELECT);

   // route NON-COMMENT rubber lines
   if(pDoc->GetElemList()->RouteToSignalInit(EF_SELECT,false))
   {
      int i = 0;
      while(pDoc->GetElemList()->RouteFlaggedLines(EF_SELECT) && !(GetKeyState(VK_ESCAPE)<0))
      {
         if(++i == 100)
         {
            pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateWindowHint);
            Sleep(10);
            i=0;
         }
      }
      pDoc->GetElemList()->RouteToSignalDone(EF_SELECT);	
      pDoc->GetElemList()->InitMoveFixPoints();
      pDoc->GetElemList()->DoneMoveFixPoints();
      bUpdate = true;
   }

   // route COMMENT rubber lines
   if(pDoc->GetElemList()->RouteToSignalInit(EF_SELECT,true))
   {
      int i = 0;
      while(pDoc->GetElemList()->RouteFlaggedLines(EF_SELECT))
      {
         if(++i == 100)
         {
            pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateWindowHint);
            Sleep(10);
            i=0;
         }
      }
      pDoc->GetElemList()->RouteToSignalDone(EF_SELECT);	
      pDoc->GetElemList()->InitMoveFixPoints();
      pDoc->GetElemList()->DoneMoveFixPoints();
      bUpdate = true;
      pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateWindowHint);
   }

   if(bUpdate) // anything changed?
   {
      pDoc->GetElemList()->RouteToSignalDone(EF_SELECT);
      pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateWindowHint);

      pDoc->m_Action.StoreState(pDoc->GetElemList());
      pDoc->SetModifiedFlag(pDoc->m_Action.StateChangedQ());
   }
}


/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnUpdateGrRoute(CCmdUI* pCmdUI) 
{
	CGrEditorDoc* pDoc = GetDocument();
	ASSERT(pDoc);
   if (pDoc)
   {
      pCmdUI->Enable (pDoc->GetEditMode() == EM_NORMAL && !CheckReadonly());
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnGrUnroute() 
{
   ASSERT("sollte nichtmehr aufgerufen werden");
   CGrEditorDoc *pDoc = GetDocument();

   if (!OnModifyContent ())
   {
      ::MessageBeep(-1);
      return;
   }

   CWaitCursor wait;

   pDoc->GetElemList()->UnrouteFlaggedLines(false, EF_SELECT);
   pDoc->GetElemList()->UnrouteFlaggedLines(true,  EF_SELECT);

   pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateWindowHint);
   pDoc->m_Action.StoreState(pDoc->GetElemList());
   pDoc->SetModifiedFlag(pDoc->m_Action.StateChangedQ());
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnUpdateGrUnroute(CCmdUI* pCmdUI) 
{
	CGrEditorDoc* pDoc = GetDocument();
	ASSERT(pDoc);
   if (pDoc)
   {
      pCmdUI->Enable (pDoc->GetEditMode() == EM_NORMAL && !CheckReadonly());
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnShowVarSelection()
{
   HRESULT hr;
   CString strPouName;
   CGrEditorDoc* pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   
   if (!pDoc)
   {
      return;
   }
   if (!OnModifyContent()) 
   {
      ::MessageBeep(-1);
      return; 
   }
   if( pDoc->GetEditMode() == EM_MONITOR ) 
   {
      return;
   }
   if( pDoc->GetEditMode() == EM_EDITPROP ) 
   {
      EditNameStop(false);
      pDoc->SetEditMode(EM_NORMAL);
   }   
   if (pDoc->GetEditMode() != EM_NORMAL)
   {
      EventCancel();
   }
   
   CElem  *pE = NULL;
   CRect   r;
   CPoint  p;
   
   if (pDoc->GetElemList()->IsOneSelected())
   {
      pE = pDoc->GetElemList()->GetSelectElem();
   }
   else
   {
      pE = pDoc->GetElemList()->SearchElem(m_LogCurPos, m_iZGrid);
   }
   
   // pou name
   strPouName = pDoc->GetDocumentFile();
   CFile file;
   file.SetFilePath(strPouName);
   strPouName = file.GetFileTitle();
   
   if(pE==NULL)
   {
       hr = pDoc->ShowVarSelection(strPouName);
   }
   else
   {
      CString strVarText;
      LONG lXPos;
      LONG lYPos;
      
      // fill information that is the same for all types of elements
      // get position of
      CPoint pointOrig =  pE->GetOrigin();
      CSize  sizeBox = pE->GetSize();
      pointOrig.y += sizeBox.cy;
      pointOrig = GridToClient(pointOrig);
      ClientToScreen(&pointOrig);
      lXPos = pointOrig.x;
      lYPos = pointOrig.y;
      
      // element type specific information
      switch (pE->GetType())
      {
         // var box
      case ET_VARIN     :
      case ET_VAROUT    :
      case ET_VARINOUT  : 
         {
            strVarText = ((CElemVar*)pE)->GetName();
            if (!strVarText.IsEmpty())
            {
                strPouName += _T('|');
                strPouName += strVarText;
            }      
            hr = pDoc->ShowVarSelection(strPouName);
           
            GetParentFrame()->SetActiveView(this);
            SetFocus();
         }
         break;
      case ET_FB         :
         {
            strVarText = ((CElemFblk*)pE)->GetInst();
            if (!strVarText.IsEmpty())
            {
                strPouName += _T('|');
                strPouName += strVarText;
            }      
            hr = pDoc->ShowVarSelection(strPouName);
            
            GetParentFrame()->SetActiveView(this);
            SetFocus();
         }
         break;
      case ET_TRANSITION:
         {
            strVarText = ((CElemTran*)pE)->GetName();
            if (!strVarText.IsEmpty())
            {
                strPouName += _T('|');
                strPouName += strVarText;
            }      
            hr = pDoc->ShowVarSelection(strPouName);

            GetParentFrame()->SetActiveView(this);
            SetFocus();
         }
         break;
      case ET_CONTACT   :
      case ET_COIL      :
         {
            strVarText = ((CElemLdBase*)pE)->GetName();
            
            if (!strVarText.IsEmpty())
            {
                strPouName += _T('|');
                strPouName += strVarText;
            }      
            hr = pDoc->ShowVarSelection(strPouName);

            GetParentFrame()->SetActiveView(this);
            SetFocus();
         }
         break;
      default:
         {    
            hr = pDoc->ShowVarSelection(strPouName);
         }
         break;
      }
      
      // select box if not already selected
      UnselectAll();
      pE->SetFlag(EF_SELECT);
      pDoc->GetElemList()->MoveFlaggedToTail(EF_SELECT);
      pDoc->GetElemList()->MoveFlaggedToTail(EF_RUBBLINE);
      pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateSelectionHint);
    }
}


/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnViewObjHelp()
{
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   CElem  *pE;
   CRect   r;
   CPoint  p;
   
   if( ((pE = pDoc->GetElemList()->SearchElem(m_LogCurPos, m_iZGrid)) != NULL) && 
      ((pE->GetType() == ET_FB) || (pE->GetType() == ET_FUNCTION)) ) 
   {
      CString typeName;
      typeName = ((CElemFblk*)pE)->GetName();
      pDoc->ShowUserHelp(typeName);
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::InitSignalChangeDetect()
{
   CGrEditorDoc *pDoc = GetDocument();
   if( pDoc && m_bWarnOnSignalChange )
   {
      pDoc->GetElemList()->SignalChangeInit(true);
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::DetectSignalChange()
{
   CGrEditorDoc *pDoc = GetDocument();
   if( pDoc && m_bWarnOnSignalChange )
   {
      if( pDoc->GetElemList()->SignalChangeCount() > 1 )
      {
         AfxMessageBox(IDS_GR_CHANGE_WARNING);
      }
      pDoc->GetElemList()->SignalChangeInit(false);
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnInsertLruPou(UINT nID)
{
   CGrEditorDoc *pDoc;
   HRESULT hr;
   
   UINT nIDZero = nID - ID_GR_INSERT_LRU_FIRST;
   UINT count = 0;
   CString strPouName;
   CString strPouDef;
   
   POSITION pos = m_listLruPous.GetHeadPosition();
   while(pos)
   {
      CString str = m_listLruPous.GetNext(pos);
      if (count == nIDZero)
      {
         strPouName = str;
         break;
      }
      count++;
   }
   
   if (strPouName.IsEmpty())
   {
      return;
   }
   
   pDoc = GetDocument ();
   if (pDoc != NULL)  
   {   
      hr = pDoc->GetPouXML(strPouName, strPouDef);
      
      if (hr==S_OK)
      {
         OnInsertFBCallHint(strPouDef);
      }
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnXRefChangeRefPositionLast()
{
   CGrEditorDoc *pDoc;
   HRESULT hr;
   
   pDoc = GetDocument ();
   if (pDoc != NULL)  
   {   
      hr = pDoc->XRefChangeRefPosition(FALSE);
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnXRefChangeRefPositionNext()
{
   CGrEditorDoc *pDoc;
   HRESULT hr;
   
   pDoc = GetDocument ();
   if (pDoc != NULL)  
   {   
      hr = pDoc->XRefChangeRefPosition(TRUE);
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnXRefSetActiveQuery()
{
   CGrEditorDoc *pDoc;
   CPoint point;
   CString strText;
   
   pDoc = GetDocument ();
   if (pDoc != NULL)
   {
      CString parentName = pDoc->GetDocumentFile();
      int     lbs;
      lbs = parentName.ReverseFind(_T('\\'))+1;
      parentName = parentName.Mid(lbs, parentName.GetLength() - lbs);
      
      parentName =  _T("|") + parentName;

      CStdEditorPosition startPos;
      CStdEditorPosition endPos;
      CStdEditorPosition   posDoc;
      posDoc  = CStdEditorPosition(EDP_GRBOX, m_LogCurPos.x, m_LogCurPos.y, -1, -1); 
      strText = pDoc->GetSmartTextGr(posDoc, startPos, endPos, false, m_iZGrid);;

      if (!strText.IsEmpty())
      {
         strText += parentName;
      }      
      (void)pDoc->XRefSetActiveQuery(strText);
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CGrEditorView::OnShowVarSelPopup()
{
   CGrEditorDoc* pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   
   if (!pDoc)
   {
      return;
   }
   if (!OnModifyContent()) 
   {
      ::MessageBeep(-1);
      return; 
   }
   if( pDoc->GetEditMode() == EM_MONITOR ) 
   {
      return;
   }
   if( pDoc->GetEditMode() == EM_EDITPROP ) 
   {
      EditNameStop(false);
      pDoc->SetEditMode(EM_NORMAL);
   }   
   if (pDoc->GetEditMode() != EM_NORMAL)
   {
      EventCancel();
   }
   
   CElem  *pE = NULL;
   CRect   r;
   CPoint  p;
   
   if (pDoc->GetElemList()->IsOneSelected())
   {
      pE = pDoc->GetElemList()->GetSelectElem();
   }
   else
   {
      pE = pDoc->GetElemList()->SearchElem(m_LogCurPos, m_iZGrid);
   }
   
   if( pE != NULL ) 
   {
      HRESULT hr;
      CString strVarText;
      LONG lXPos;
      LONG lYPos;
      LONG lCol;
      CString strDataType;
      CString strPouName;
      CString strSelection;
      LONG lColStart;
      LONG lColEnd;
      
      // fill information that is the same for all types of elements
      
      // pou name
      strPouName = pDoc->GetDocumentFile();
      CFile file;
      file.SetFilePath(strPouName);
      strPouName = file.GetFileTitle();
      
      // get position of
      CPoint pointOrig =  pE->GetOrigin();
      CSize  sizeBox = pE->GetSize();
      pointOrig.y += sizeBox.cy;
      pointOrig = GridToClient(pointOrig);
      ClientToScreen(&pointOrig);
      lXPos = pointOrig.x;
      lYPos = pointOrig.y;
      
      // element type specific information
      switch (pE->GetType())
      {
         // var box
      case ET_VARIN     :
      case ET_VAROUT    :
      case ET_VARINOUT  : 
         {
            lCol = 0;
            strVarText = ((CElemVar*)pE)->GetName();
            
            hr = pDoc->ShowVarSelectionBox(lXPos, lYPos, 
               strVarText, lCol, 
               _T(""),
               strPouName,
               strSelection, lColStart, lColEnd);

            if (hr == S_OK)
            {
               ((CElemVar*)pE)->SetName(strSelection);
               pDoc->UpdateAllViews(NULL);
               pDoc->SetModifiedFlag(true);
            }
            GetParentFrame()->SetActiveView(this);
            SetFocus();
         }
         break;
      case ET_FB         :
         {
            lCol = 0;
            strVarText = ((CElemFblk*)pE)->GetInst();
            strDataType = ((CElemFblk*)pE)->GetName();
            
            hr = pDoc->ShowVarSelectionBox(lXPos, lYPos, 
               strVarText, lCol, 
               strDataType,
               strPouName,
               strSelection, lColStart, lColEnd);
            
            if (hr == S_OK)
            {
               ((CElemFblk*)pE)->SetInst(strSelection);
               pDoc->UpdateAllViews(NULL);
               pDoc->SetModifiedFlag(true);
            }
            GetParentFrame()->SetActiveView(this);
            SetFocus();
         }
         break;
      case ET_TRANSITION:
         {
            lCol = 0;
            strVarText = ((CElemTran*)pE)->GetName();
            strDataType = _T("BOOL");
            
            hr = pDoc->ShowVarSelectionBox(lXPos, lYPos, 
               strVarText, lCol, 
               strDataType,
               strPouName,
               strSelection, lColStart, lColEnd);
            
            if (hr == S_OK)
            {
               ((CElemTran*)pE)->SetName(strSelection);
               pDoc->UpdateAllViews(NULL);
               pDoc->SetModifiedFlag(true);
            }
            GetParentFrame()->SetActiveView(this);
            SetFocus();
         }
         break;
      case ET_CONTACT   :
      case ET_COIL      :
         {
            lCol = 0;
            strVarText = ((CElemLdBase*)pE)->GetName();
            strDataType = _T("BOOL");
            
            hr = pDoc->ShowVarSelectionBox(lXPos, lYPos, 
               strVarText, lCol, 
               strDataType,
               strPouName,
               strSelection, lColStart, lColEnd);
            
            if (hr == S_OK)
            {
               ((CElemLdBase*)pE)->SetName(strSelection);
               pDoc->UpdateAllViews(NULL);
               pDoc->SetModifiedFlag(true);
            }
            GetParentFrame()->SetActiveView(this);
            SetFocus();
         }
         break;
         
      }
      
      // select box if not already selected
      UnselectAll();
      pE->SetFlag(EF_SELECT);
      pDoc->GetElemList()->MoveFlaggedToTail(EF_SELECT);
      pDoc->GetElemList()->MoveFlaggedToTail(EF_RUBBLINE);
      pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateSelectionHint);
    }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
CSize BookMark::GetSize(CDC *pDC, CGrEditorView * pView, CSize *pSize)
{
   CSize tsz;  // text size
   tsz = pDC->GetTextExtent( m_Name.IsEmpty() ?"0" :m_Name );
   
   CSize bsz;  // box size
   bsz.cx = max( pView->m_iZGrid<<1, tsz.cx + tsz.cy/4);
   bsz.cy = max( pView->m_iZGrid<<1, tsz.cy);

   if(pSize)
      *pSize = tsz;

   return bsz;
}


/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void BookMark::Draw(CDC * pDC, CGrEditorView * pView)
{   
/*
 *  draw bookmarks
 *  !keep pos/size consistent with that in the routine:
 *     CGrEditorView::DrawBookMarks(...)
 */
   if (pView)
   {
      CRect r;
      CPoint tpos;
      CPoint bpos = pView->GridToDoc(m_Pos);

      CSize tsz;                              // text size
      CSize bsz = GetSize(pDC, pView, &tsz);  // box size

      r.SetRect(bpos,bpos+bsz);
      r -= r.CenterPoint()-r.TopLeft();
//      r.DeflateRect(1,1,1,1);

      tpos = r.CenterPoint();
      tpos.x -= tsz.cx/2;
      tpos.y -= tsz.cy/2;

      pDC->RoundRect(&r, CPoint(bsz.cy>>1, bsz.cy>>1) );
      pDC->TextOut(tpos.x, tpos.y, m_Name);   // draw names inside the bookmarks
   }
}
