

/*doc*=======================================================================*/
/*inc*=======================================================================*/

#include "stdafx.h"
#include "utilif.h"

#include "MainFrm.h"
#include "ElemPtrList.h"
#include "GrEditor.h"
#include "GrEditorDoc.h"
#include "GrEditorUtil.h"
#include "ElemEditorView.h"

#include "InsFbDlg.h"
#include "InsConDlg.h"
#include "InsLabDlg.h"
#include "InsVarDlg.h"
#include "InsCmtDlg.h"
#include "InsStepDlg.h"
#include "InsTranDlg.h"
#include "InsActDlg.h"
#include "InsDivDlg.h"
#include "InsCtDlg.h"
#include "InsClDlg.h"
#include "PropLineDlg.h"
#include "PropPRail.h"

#include "CEGRError.h"

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

/////////////////////////////////////////////////////////////////////////////
// CElemEditorView

IMPLEMENT_DYNCREATE(CElemEditorView, CScrollView)

CElemEditorView::CElemEditorView()
{
   SetZoomedGrid(8);                   // default init.
   m_LastScrollPos = CPoint(0,0);	


   m_pPropEdit     = NULL;
   m_pSEC_Edit     = NULL;
//   m_pPropFont     = NULL;
//   m_pFixedFont    = NULL;
   m_pPropText     = NULL;
   m_pPropElem     = NULL;
   m_pAutoElem     = NULL;

   m_EditLine      = NULL;

   m_BeforeOpPos   = CPoint(0,0);
   m_EndPos        = CPoint(0,0);
   m_StartPos      = CPoint(0,0);
   m_CurPos        = CPoint(0,0);
   m_LogCurPos     = CPoint(0,0);
   m_OldCurPos     = CPoint(0,0);
   m_bFirstMove    = true;
   m_bEditNext     = false;
   m_eCursorMode   = CT_INIT;
   m_bUpdateMousePos = FALSE;
   m_eResizeMode   = RS_LAST;
   m_bIsCaptured   = FALSE;
   m_NextActionType= EF_LAST;  // invalid flag: FBD-Action
   m_bFontCreated  = FALSE;

   CString strSec; 
   CString strKey; 
   strSec.LoadString(IDS_GR_REG_SECTION);
   
   strKey.LoadString(IDS_GR_REG_AUTOSIZE);
   int i = AfxGetApp()->GetProfileInt (strSec, strKey, 0);
   m_bUseAutoSize = (i==1);

   strKey.LoadString(IDS_GR_REG_CHANGEWARN);
   i = AfxGetApp()->GetProfileInt (strSec, strKey, 0);
   m_bWarnOnSignalChange = (i==1);

   m_bIgnoreForFBCall = false;
   m_NoAutoScroll  = false;
}

CElemEditorView::~CElemEditorView()
{
}


BEGIN_MESSAGE_MAP(CElemEditorView, CScrollView)
	//{{AFX_MSG_MAP(CElemEditorView)
   ON_WM_MOUSEACTIVATE()
   ON_MESSAGE( WM_USER, OnSECEditCtrlChar )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CElemEditorView drawing

void CElemEditorView::OnDraw(CDC* /*pDC*/)
{
}

void CElemEditorView::OnActivateView(BOOL bActivate, CView* pActivateView, 
   CView* pDeactiveView) 
{
   CScrollView::OnActivateView(bActivate, pActivateView, pDeactiveView);

   if (bActivate)
   {
      if (pActivateView != this)
         return;

      m_eCursorMode = CT_INIT;
      UpdateCursor(false); 
   }
}

BOOL CElemEditorView::PreTranslateMessage (MSG* pMsg) 
{
	return (CScrollView::PreTranslateMessage(pMsg));
}

/////////////////////////////////////////////////////////////////////////////
// CElemEditorView diagnostics

#ifdef _DEBUG
void CElemEditorView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CElemEditorView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CGrEditorDoc* CElemEditorView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGrEditorDoc)));
	return (CGrEditorDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CElemEditorView message handlers

// convertion functions :

/*func*------------------------------------------------------------------------
  process events mouse:lbuttondown or keyboard:space down
  in :- 
  out:-
-----------------------------------------------------------------------------*/
void CElemEditorView::EventDown()
{
}

/*func*------------------------------------------------------------------------
  process events mouse:lbuttonup or keyboard:space up
  in :- 
  out:-
-----------------------------------------------------------------------------*/
void CElemEditorView::EventUp()
{
}

/*func*------------------------------------------------------------------------
  process events mouse:rbuttondown or keyboard:esc
  abort current action
  in :- 
  out:-
-----------------------------------------------------------------------------*/
void CElemEditorView::EventCancel()
{
   CRect        r, r2;
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   switch ( pDoc->GetEditMode() ) {
      case EM_RESIZE_DIR  :
      case EM_RESIZE_START:
//    case EM_SELECT      :
      case EM_INSCOL     :
      case EM_INSCOL1    :
      case EM_DELCOL     :
      case EM_DELCOL1    :
      case EM_INSROW     :
      case EM_INSROW1    :
      case EM_DELROW     :
      case EM_DELROW1    :
         pDoc->SetEditMode( EM_NORMAL );
         goto m_cancelend;

      case EM_LINE:
         if((pDoc->GetLineMode() == LDM_RUBB) &&
            (m_EditLine != NULL))
         {
            pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateDrawElemHint, m_EditLine);
         }
         EditLineStop();
         pDoc->SetEditMode( EM_LINE_START );
         goto m_cancelend;

      case EM_LINE_START:
         pDoc->SetEditMode (EM_NORMAL);
         pDoc->GetElemList()->ClearFlags(EF_LIGHTPIN);
         goto m_cancelend;
            
      case EM_INSERT:
      case EM_RESIZE:
      case EM_MOVE  :
         OnDeleteSTActions(false);
         pDoc->m_Action.ReloadLastState(pDoc->GetElemList());
         pDoc->SetModifiedFlag(pDoc->m_Action.StateChangedQ());
         pDoc->GetElemList()->AddAllElemsToPinTable();
         pDoc->GetElemList()->bValidPos = !pDoc->GetElemList()->IsOneOverlapped();
         pDoc->SetEditMode( EM_NORMAL );
         m_bFirstMove = true;     
         MoveViewToCursor();
	      m_NoAutoScroll = true;
         goto m_cancelend;

      case EM_EDITPROP:
         EditNameStop(true);
         pDoc->SetEditMode(EM_NORMAL);
         goto m_cancelend;

      m_cancelend:
         pDoc->UpdateAllViews(NULL);
         UpdateCursor();
         break;
   }
   GrReleaseCapture();
}

/*func*------------------------------------------------------------------------
  process events mouse:lbuttondblclick or keyboard:enter
  in :- 
  out:-
-----------------------------------------------------------------------------*/
void CElemEditorView::EventEnter()
{
}

void CElemEditorView::CheckMode()
{
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   switch ( pDoc->GetEditMode() ) {
      case EM_NORMAL  : break;
      case EM_EDITPROP: EventEnter();  break;
      default         : EventCancel(); break;
   }
}

CSize CElemEditorView::GetInsertSize(UINT RegKey)
{
   CSize size(0,0);

   if(!m_bUseAutoSize)
   {
      CString strSec; strSec.LoadString(IDS_GR_REG_SECTION);
      CString strKey; strKey.LoadString(RegKey);
      
      int iSize = AfxGetApp()->GetProfileInt (strSec, strKey, 0);
      size = CSize(abs(iSize >>16), abs(iSize & 0x0000FFFF));
   }
   return size;
}

void CElemEditorView::SetInsertSize(UINT RegKey, CSize Size)
{
   UINT std = (abs(Size.cx)<<16) | (abs(Size.cy) & 0x0000FFFF);
   CString strSec; strSec.LoadString(IDS_GR_REG_SECTION);
   CString strKey; strKey.LoadString(RegKey);
   if(!strKey.IsEmpty())
      AfxGetApp()->WriteProfileInt (strSec, strKey, std);
}

/*func*------------------------------------------------------------------------
  calculate the size for the string(s)
  in :str     - string 
  out:size for string(s) in raster coords
-----------------------------------------------------------------------------*/
CSize CElemEditorView::GetCmtSize(CString str)
{
   int          i, iDC, iCX, iCY, iLines;
   CSize        size;
   CFont        Font;
   CString      str2;
   CClientDC    dc(this);  // .. for calculate size of element
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   iDC = dc.SaveDC();
   Font.CreateFont( 
      -((m_iDZGrid*FontToGrid)/100),0,0,0,0,0,0,0,0,0,0,0,0,pDoc->GetFontName());
   dc.SelectObject( &Font );

   iLines = 0;
   iCX = 0;
   iCY = 0;
   do {
      i = str.Find(_T("\r"));
      if( i == -1 ) i = str.GetLength();
      str2 = str.Left(i);
      size = dc.GetTextExtent(str2);
      iCX = max( iCX, size.cx );
      iCY = max( iCY, size.cy );
      iLines++;
      str = str.Right( str.GetLength() - i - 2);
   } while ( str.GetLength() > 0 );

   if( m_iZGrid )
   {
      size.cx = (((iCX          + m_iZGrid) / m_iZGrid) + 1) & 0xFFFFFFFE;
      size.cy = (((iCY * iLines + m_iZGrid) / m_iZGrid) + 1) & 0xFFFFFFFE;
   } else {
      size = CSize(4,4);
   }

   dc.RestoreDC(iDC);
   return size;
}



void CElemEditorView::DocToClient(CRect &r)
{
   r.OffsetRect( -m_LastScrollPos );
}

CPoint CElemEditorView::DocToClient(CPoint p)
{
   return ( p - m_LastScrollPos );
}

void CElemEditorView::DocToGrid(CRect& r)
{
   r.left   = (r.left   + m_iHZGrid) / m_iZGrid;
   r.right  = (r.right  + m_iHZGrid) / m_iZGrid;
   r.top    = (r.top    + m_iHZGrid) / m_iZGrid;
   r.bottom = (r.bottom + m_iHZGrid) / m_iZGrid;
}

CPoint CElemEditorView::DocToGrid(CPoint p)
{
   p.x = (p.x + m_iHZGrid) / m_iZGrid;
   p.y = (p.y + m_iHZGrid) / m_iZGrid;

   return p;
}


void CElemEditorView::ClientToGrid(CRect &r)
{
   r.OffsetRect( m_LastScrollPos );  // client -> doc
   r.left   = (r.left   + m_iHZGrid) / m_iZGrid;
   r.right  = (r.right  + m_iHZGrid) / m_iZGrid;
   r.top    = (r.top    + m_iHZGrid) / m_iZGrid;
   r.bottom = (r.bottom + m_iHZGrid) / m_iZGrid;
}

CPoint CElemEditorView::ClientToGrid(CPoint p)
{
   p += m_LastScrollPos;  // client -> doc
   p.x = (p.x + m_iHZGrid) / m_iZGrid;
   p.y = (p.y + m_iHZGrid) / m_iZGrid;

   return p;
}

void CElemEditorView::ClientToDoc(CRect &r)
{
   r.OffsetRect( m_LastScrollPos );
}

CPoint CElemEditorView::ClientToDoc(CPoint p)
{
   return ( p + m_LastScrollPos );
}


CPoint CElemEditorView::GridToClient(CPoint p)
{
   p.x *= m_iZGrid;                     // grid -> doc
   p.y *= m_iZGrid;
   return ( p - m_LastScrollPos ); // doc -> client
}

void CElemEditorView::GridToClient(CRect &r)
{
   r.left   *= m_iZGrid;
   r.top    *= m_iZGrid;
   r.right  *= m_iZGrid;
   r.bottom *= m_iZGrid;
   r.OffsetRect( -m_LastScrollPos );
}

void CElemEditorView::GridToDoc(CRect &r)
{
   r.left   *= m_iZGrid;
   r.right  *= m_iZGrid;
   r.top    *= m_iZGrid;
   r.bottom *= m_iZGrid;
}

CPoint CElemEditorView::GridToDoc(CPoint p)
{
   p.x = p.x * m_iZGrid;
   p.y = p.y * m_iZGrid;
   return p;
}

void CElemEditorView::CheckRastRect(CRect &r)
{
   r.NormalizeRect();
   if( r.left   < 0 ) r.left   = 0;
   if( r.top    < 0 ) r.top    = 0;
   if( r.right  >= MAX_XGRID ) r.right  = MAX_XGRID-1;
   if( r.bottom >= MAX_YGRID ) r.bottom = MAX_YGRID-1;
}

void CElemEditorView::CheckRastPoint(CPoint & p)
{
   if( p.x < 0 ) p.x = 0;
   if( p.y < 0 ) p.y = 0;
   if( p.x >= MAX_XGRID ) p.x = MAX_XGRID-1;
   if( p.y >= MAX_YGRID ) p.y = MAX_YGRID-1;
}

/*func*------------------------------------------------------------------------
  end the line drawing and connect the last line
  in :- 
  out:-
-----------------------------------------------------------------------------*/
bool CElemEditorView::OnModifyContent ()
{
   CGrEditorDoc *pDoc;
   
   pDoc = GetDocument ();
   if (pDoc == NULL)
   {
      return (true);
   }

   if (pDoc->GetEditMode() == EM_MONITOR)
   {
      pDoc->SetEditMode( EM_NORMAL );
      // we handle this specially, but we need the events ..
//      return (true);
      pDoc->PrepareEdit(); // change to class tab
   }
  
   
   if (pDoc->CheckReadonly ())
   {
//      ::MessageBeep (-1);
      return (false);
   }

//   if (pDoc->HadReadOnlyWarning())
//   {
//      return (true);
//   }
   
//   if (AfxMessageBox (IDS_GR_READONLY_WARNING, MB_OKCANCEL | MB_DEFBUTTON2) == IDCANCEL)
//   {
//      return (false);
//   }

//   pDoc->HadReadOnlyWarning(true);
   CSECEditor* pEdit = pDoc->GetEditor(0);
   if (pEdit)
   {
      pEdit->SetReadOnly(false);
   } else {
      ASSERT(0);
   }
   return (true);
}


//------------------------------------------------------------------*
/**
 * check read only.
 *
 *  calls CheckReadonly of StdEditorDoc.
 *  if document pointer is NULL, return false, i.e. not read only.
 *
 * @param           -
 * @return          is document file readonly?
 * @exception       -
 * @see             -
*/
bool CElemEditorView::CheckReadonly()
{
   CGrEditorDoc *pDoc;
   
   pDoc = GetDocument ();
   if (pDoc == NULL)
   {
      return false;
   }

   return pDoc->CheckReadonly();
}

/*func*------------------------------------------------------------------------
  change the m_EditLine line from 'BeforOpPos'-'StartPos' to 'BeforOpPos'-'EndPos'.
  
  BeforeOpPos -> StartPos -> EndPos -> CurPos  (in grid coords)
      |--------------|.........|    =?   |
  in :- 
  out:-
-----------------------------------------------------------------------------*/
bool CElemEditorView::EditLineProc()
{
   bool   bRes;
   CSize  oldSize, size;
   CPoint p, ps, pe;
   CPoint oldOrigin;
   int    dx, dy;
   POSITION     pos;
   CElem       *pE;
   CElemLine   *pLE;

   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   if(pDoc->GetLineMode() != LDM_RUBB)
   {
      ASSERT(m_EditLine);
      
      // check for new direction and step width ..
      dx = m_EndPos.x - m_StartPos.x;
      dy = m_EndPos.y - m_StartPos.y;
      
      oldSize   = m_EditLine->GetSize();
   }

   ASSERT( (pDoc->GetLineMode() == LDM_RUBB ? 
               TRUE :
               ((abs(dx)==2 && (dy)==0) || ((dx)==0 && abs(dy)==2))) );

   if(pDoc->GetLineMode() == LDM_RUBB)    
   {
      if(pDoc->GetElemList()->RouteToPoint(m_EndPos))
      {
      }
      pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateWindowHint, NULL);
      m_StartPos = m_EndPos;
     
      bRes = true;
   }else
   {

      if( (m_EditLine->GetSize() == CSize(0,0)) &&  // check at first step ..
         ((pLE = pDoc->GetElemList()->SearchLine (m_StartPos, m_EndPos, false)) != NULL) &&
         (pLE != m_EditLine))
      {  
         // check connect point for break an existing line ..
         if (pLE->SearchConn (m_StartPos) == NULL)
         {
            pDoc->GetElemList()->BreakLines (m_StartPos);
            pDoc->GetElemList()->BreakLines (m_StartPos);   // do it twice (two lines are possible)!
            
            pLE = pDoc->GetElemList()->SearchLine (m_StartPos, m_EndPos, false);
            ASSERT(pLE);
         }
         
         pDoc->GetElemList()->DeleteElem (m_EditLine);  
         m_EditLine = pLE;
         m_EditLine->SetFlag(EF_EDITLINE);
         m_EditLine->ClrFlag(EF_TEMPELEM);
         if (m_StartPos == pLE->GetOrigin())
         {  // reorientation  
            pLE->SwapOrigin();
         }
         ASSERT(m_StartPos == pLE->GetOrigin() + pLE->GetSize() );
         m_BeforeOpPos = pLE->GetOrigin();
      }
      
      for (pos = pDoc->GetElemList()->GetHeadPosition(); pos != NULL; )
      {
         pE = pDoc->GetElemList()->GetNext(pos);
         if(pE->ChkFlag(EF_EMPHASIZE))
         {
            pE->ClrFlag(EF_EMPHASIZE);
            pE->SetFlag(EF_UPDATE);
         }
      }
      
      if (m_BeforeOpPos == m_EditLine->GetOrigin())
      {
         bRes = m_EditLine->Resize(  0,  0,dx,dy);
      } else {
         bRes = m_EditLine->Resize(-dx,-dy,dx,dy);
      }
      if (bRes)
      {
         bRes = !pDoc->GetElemList()->IsElemOverlapped(m_EditLine);
      }
      
      if (bRes)
      {  
         m_StartPos = m_EndPos;
         // try to reconnect lines - if lines length decrease ..
         if( (m_EditLine->GetSize() != CSize(0,0)) &&
            (pDoc->GetLineMode() == LDM_ORTHO) &&
            ((abs(oldSize.cx) > abs(m_EditLine->GetSize().cx)) ||
            (abs(oldSize.cy) > abs(m_EditLine->GetSize().cy))) )
         {
            pDoc->GetElemList()->LinkLines(m_StartPos);
         }
      } else {
         m_EndPos = m_StartPos;   // position back !
         
         if( m_BeforeOpPos == m_EditLine->GetOrigin() )
         {
            m_EditLine->Resize( 0, 0,-dx,-dy);
         } else {
            m_EditLine->Resize(dx,dy,-dx,-dy);
         }
      }
   }
   return bRes;
}

/*func*------------------------------------------------------------------------
  end the line drawing and connect the last line
  in :- 
  out:-
-----------------------------------------------------------------------------*/
void CElemEditorView::EditLineStop()
{
   CWaitCursor wc;
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   bool  bNoLine=false;

   if( pDoc->GetLineMode() == LDM_RUBB )
   {
      pDoc->GetElemList()->RouteToPointDone();
   }else
   {
      if( m_EditLine )
      {
         m_EditLine->ClrFlag(EF_EDITLINE); 
         m_EditLine->SetFlag(EF_UPDATE  );
         pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateDrawElemHint, m_EditLine);
         // optimize here for undo/redo stack
         if (m_EditLine->GetSize() == CSize(0,0))
         {
            bNoLine = m_EditLine->ChkFlag(EF_TEMPELEM);//true;
         }
         m_EditLine->ChkFlag(EF_TEMPELEM);
      }
   }
   m_EditLine = NULL;

   pDoc->GetElemList()->BreakLines(m_EndPos);
   pDoc->GetElemList()->ConnectAllElems();
   if (!bNoLine)
   {
      // implicit hand route rubber lines
      pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateRubberHint);
      pDoc->GetElemList()->PurgeRedundantRubber();

      pDoc->m_Action.StoreState(pDoc->GetElemList());  // op. ok -> store
      pDoc->SetModifiedFlag(pDoc->m_Action.StateChangedQ());
   }
}


/*func*------------------------------------------------------------------------
  start edit line
  in :FromPos - start position in grid coords
  out:-
-----------------------------------------------------------------------------*/
void CElemEditorView::EditLineStart(CPoint FromPos)
{
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   CRect Area(1,1,MAX_XGRID,MAX_YGRID);
   if(!Area.PtInRect(FromPos))
      return;

   m_BeforeOpPos = m_EndPos = m_StartPos = FromPos;
   if(pDoc->GetLineMode() == LDM_RUBB)
   {
      pDoc->GetElemList()->ClearFlags(EF_SELECT);
      pDoc->GetElemList()->RouteToPointInit(m_StartPos);
      m_EditLine = NULL;
   }else
   {
      // begin here with size (0,0)
      m_EditLine = new CElemLine(m_BeforeOpPos, m_EndPos, pDoc->GetLineMode() == LDM_RUBB);
      ASSERT(m_EditLine);
      if (m_EditLine)
      {
         m_EditLine->SetFlag(EF_EDITLINE);
         pDoc->GetElemList()->InsertElem( m_EditLine );
         pDoc->GetElemList()->UpdatePinStatus();
      }
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CElemEditorView::CreateFont(void)
{
   CGrEditorDoc* pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   // build resources
   m_PropFont.CreateFont (-((m_iDZGrid*FontToGrid)/100),0,0,0,0,0,0,0,0,0,0,0,0,
      pDoc->GetFontName());
   
   m_FixedFont.CreateFont(-((m_iDZGrid*FontToGrid)/100),0,0,0,0,0,0,0,0,0,0,0,
      FIXED_PITCH|FF_MODERN,
      _T("Lucida Console"));

   m_bFontCreated = TRUE;
}


/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CElemEditorView::ZoomFont(void)
{
   if(m_bFontCreated)
   {
      DeleteFont();
   }
   CreateFont();
}


/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CElemEditorView::DeleteFont(void)
{
   m_bFontCreated = FALSE;
   // free gdi resources
   m_FixedFont.DeleteObject();
   m_PropFont.DeleteObject();
}


/*func*------------------------------------------------------------------------
  searches for ST-Actions and creates a view for each
  in :-
  out:-
-----------------------------------------------------------------------------*/
void CElemEditorView::CreateSTViews()
{
   CElem* pE;
   CGrEditorDoc* pDoc = GetDocument();
   if(pDoc)
   {
      CElemPtrList* pList = pDoc->GetElemList();
      POSITION pos = pList->GetHeadPosition();
      while(pos)
      {
         pE = (CElem*)pList->GetNext(pos);
         if(pE->ChkFlag(EF_STACTION))
         {
            CElemAct* pAct = dynamic_cast<CElemAct*>(pE);
            ASSERT(pAct);

            if(pAct)
            {
               CSTEditorView *pView;
               if(!m_STViewMap.Lookup(pAct->GetID(),pView)) // not created?
               {
                  VERIFY(CreateSTView(pAct));               // create new
               }
            }
         }
      }
   }
}


/*func*------------------------------------------------------------------------
  deletes all ST-action views
  in :-
  out:-
-----------------------------------------------------------------------------*/
void CElemEditorView::DeleteSTViews()
{
   CGrEditorDoc* pDoc = GetDocument();

   if(pDoc)
   {
      CFrameWnd* pFrame = GetParentFrame();
      if (pFrame)
      {
         pFrame->SetActiveView(this);
      } else {
         ASSERT(0);
      }

      POSITION pos = m_STViewMap.GetStartPosition();
      while (pos)
      {
         CSTEditorView *pView;
         UINT           key;
         m_STViewMap.GetNextAssoc(pos, key, pView);
         delete pView;
      }
      m_STViewMap.RemoveAll();
      pDoc->DeleteAllEditors();
   }
}


/*func*------------------------------------------------------------------------
  create an new STEditorView from the ST-action element
  in :pE - pointer to element
  out:-
-----------------------------------------------------------------------------*/
CSTEditorView* CElemEditorView::CreateSTView(CElemAct* pAct)
{
   CCreateContext contextT;
   CSTEditorView  *pView = NULL;
   CGrEditorDoc   *pDoc  = GetDocument();

   ASSERT( pDoc );
   ASSERT( pAct->ChkFlag(EF_STACTION) );
   ASSERT( !m_STViewMap.Lookup(pAct->GetID(),pView) );   // check existence

   pView = (CSTEditorView*) CSTEditorView::CreateObject();
   UINT ID = pDoc->CreateEditor(_T("ST"));
   pView->SetEditID( ID );     // create/ & assign the editor

   CSECEditor* pED = pDoc->GetEditor(ID);
   if (pED)
   {
      pED->m_crBackground = GDIHelper::GetColor(true, CO_BACKGD);
   }
   pView->SetContext(pAct->GetOrigin());                 // container origin
   pView->m_iDZGrid = m_iDZGrid;

   m_STViewMap[pAct->GetID()] = pView;                   // insert into the map
      
   // set info about last pane
   ASSERT(contextT.m_pCurrentFrame == NULL);
   ASSERT(pView->m_hWnd == NULL);                        // not yet created

   contextT.m_pLastView   = this;
   contextT.m_pCurrentDoc = pDoc;

   CRect r;
   pAct->GetBodyRect(r,m_iZGrid);
   r.top += 2*m_iZGrid + 1;
   r.left++;
   DocToClient(r);

   DWORD dwStyle = AFX_WS_DEFAULT_VIEW & ~WS_BORDER & ~WS_VSCROLL & ~WS_VISIBLE;
   pView->Create( NULL, NULL, dwStyle, r, this, IDC_SECEDITCTRL, &contextT);
   
   pView->SetDelayedText( pAct->GetSTtext().StrRef() );  // assign initial body text

   // set the CSTEditor features ALLOWED
   pView->m_Features.Set(CSTEditorView::STF_DECL_INS);   // insert declaration
   pView->m_Features.Set(CSTEditorView::STF_STMT_INS);   // insert statement

   pView->m_Features.Set(CSTEditorView::STF_DEBUGGING);  // debugging
   pView->m_Features.Set(CSTEditorView::STF_MONITORING); // monitoring
   pView->m_Features.Set(CSTEditorView::STF_POU_INS);    // insert POU-call
   pView->m_Features.Set(CSTEditorView::STF_USERHELP);   // help

   // set the CSTEditor features DENIED
   pView->m_Features.Clr(CSTEditorView::STF_SCROLLBAR);  // scrollbars
   
   pView->SendMessage(WM_INITIALUPDATE);                 // implicit add the view to the document
   pView->GetEdit()->SetModified(FALSE);

   return pView;
}


/*func*------------------------------------------------------------------------
  used to edit the name an existing element
  in :pE - pointer to element
  out:-
-----------------------------------------------------------------------------*/
void CElemEditorView::AttachSTView(CSTEditorView *&pView, CElemAct *pAct)
{
   CGrEditorDoc *pDoc = GetDocument();

   if( !m_STViewMap.Lookup(pAct->GetID(),pView) )
   {
      pView = CreateSTView(pAct);
   }
}


/*func*------------------------------------------------------------------------
  used to edit the name an existing element
  in :pE - pointer to element
  out:-
-----------------------------------------------------------------------------*/
void CElemEditorView::DetachSTView(CElemAct *pAct)
{
   CGrEditorDoc *pDoc = GetDocument();
   CSTEditorView *pView;

   if( m_STViewMap.Lookup(pAct->GetID(), pView) )
   {
      m_STViewMap.RemoveKey(pAct->GetID());
      if (pView)
      {
         CFrameWnd* pFrame = GetParentFrame();
         if (pFrame)
         {
            if (pFrame->GetActiveView() == pView)
            {
               pFrame->SetActiveView(this);
            }
         }

         pDoc->DeleteEditor(pView->GetEditID());
         delete pView;
      }
   }
}

/*func*------------------------------------------------------------------------
  edit the name an existing element
  in :pE - pointer to element
  out:-
-----------------------------------------------------------------------------*/
void CElemEditorView::EditNameStart(CText& text, CRect& r, CElem *pE, bool /*bEditName*/)
{
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   DWORD     dwStyle;

   GridToClient( r );

   dwStyle = ES_AUTOHSCROLL | ES_LEFT;
   if (text.m_Flag.Chk(TF_MULTILINE))
   {
      dwStyle |= ES_MULTILINE | ES_AUTOVSCROLL;
   } 

   m_pPropText = &text;
   m_pPropElem = pE;
   m_pAutoElem = pE->ChkFlag(EF_AUTOELEM) ? pE : NULL;
   m_bEditNext = m_pAutoElem !=NULL;

   if(text.m_Flag.Chk(TF_EDIT_OE))    // ST-Action
   {
      CElemAct *pAct= dynamic_cast<CElemAct*>(pE);
      ASSERT(pAct);

      AttachSTView( m_pSEC_Edit, pAct); // creates if necessary
      
      ASSERT(m_pSEC_Edit);
      if (m_pSEC_Edit)
      {
         if (pDoc->CheckInPlaceActive())
         {
            m_pSEC_Edit->m_Features.Set(CSTEditorView::STF_POU_INS);
         }
         m_pSEC_Edit->SetFont( &m_FixedFont );
         m_pSEC_Edit->SetWindowText( m_pPropText->Str() );
         m_pSEC_Edit->ClearSelection (m_pSEC_Edit->m_bUpdate);
         m_pSEC_Edit->EnableWindow(TRUE);
         m_pSEC_Edit->ShowWindow(SW_SHOW);
         CFrameWnd* pFrame = GetParentFrame();
         if (pFrame)
         {
            pFrame->SetActiveView(m_pSEC_Edit);
         }
         m_pSEC_Edit->SetFocus();
      }
   } else // simple element
   {
      r.DeflateRect(1,1);
      
      m_pPropEdit = new CPropEdit();
      ASSERT(m_pPropEdit);
      
      if (m_pPropEdit)
      {
         m_pPropEdit->Create( dwStyle, r, this, 1000 );
         m_pPropEdit->SetWindowText( m_pPropText->Str() );
         m_pPropEdit->SetFont( &m_PropFont );
         m_pPropEdit->SetSel(0,-1);
         m_pPropEdit->ShowWindow(SW_SHOW);
         m_pPropEdit->SetFocus();
      }
   }
   // force modified during edit - info for engineering
   pDoc->SetModifiedFlag(true);
}


/*func*------------------------------------------------------------------------
  ends editing the name an existing element
  in :pE - pointer to element
  out:true  - is stopped
      false - not stopped (error in identifier)
-----------------------------------------------------------------------------*/
void CElemEditorView::EditNameStop(bool bAbort)
{
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   CString str;
   bool    bMod=true;

   if (m_pSEC_Edit)
   {
      if (!bAbort && m_pPropText)
      {
         m_pSEC_Edit->GetWindowText(str);
         if (str != m_pPropText->StrRef())
         {
            bMod = true;
            m_pPropText->StrRef().Empty(); // the next "GetWindowText" does not overwrite but just appends to the string
            m_pPropText->StrRef() = str;
            m_pSEC_Edit->SetWindowText ( m_pPropText->StrRef());
         } else {
            bMod = false;
      		m_pSEC_Edit->GetEdit()->SetModified(FALSE);
         }
      }

      m_pSEC_Edit->GetEdit()->m_crBackground = GDIHelper::GetColor(true, CO_BACKGD);

      if (m_pPropElem)
      {
         pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateDrawElemHint, m_pPropElem);
      } else {
         pDoc->UpdateAllViews(NULL);
      }
      if (!bAbort && bMod)
      {
         pDoc->m_Action.StoreState(pDoc->GetElemList());  // op. ok -> store
      }
      m_pSEC_Edit->GetEdit()->InitUndo();    // kill undo buffer
      m_pSEC_Edit->EnableWindow(FALSE);
      m_pSEC_Edit->ShowWindow(SW_HIDE);
      m_pSEC_Edit = NULL;  // detach ST-View

      ReSyncCursorPos();
   }

   if( m_pPropEdit )
   {
      if (!bAbort && m_pPropText)
      {
         m_pPropEdit->GetWindowText(str);
         if (str != m_pPropText->StrRef())
         {
            bMod = true;
            m_pPropText->StrRef().Empty(); // the next "GetWindowText" does not overwrite but just appends to the string
            m_pPropText->StrRef() = str;
         } else {
            bMod = false;
         }
         if(m_pPropElem)
         {
            ASSERT_VALID(m_pPropElem);
            if(m_pPropElem->GetType() == ET_VARIN  ||
               m_pPropElem->GetType() == ET_VAROUT    )
            {
               if(m_pPropText->Str() == VAR_AUTONAME)
               {
                  m_pPropElem->SetFlag(EF_AUTOELEM);
               }else
               {
                  m_pPropElem->ClrFlag(EF_AUTOELEM);
               }
            }
         }
      }
      if (!bAbort && bMod)
      {
         pDoc->m_Action.StoreState(pDoc->GetElemList());  // op. ok -> store
      }
      // free resources ..
      m_pPropEdit->ShowWindow(SW_HIDE);
      m_pPropEdit->SetFont( GetFont() );
      m_pPropEdit->DestroyWindow();
      delete m_pPropEdit;
      m_pPropEdit  = NULL;
   }
   pDoc->SetModifiedFlag(pDoc->m_Action.StateChangedQ());

   m_pPropText = NULL;
   m_bEditNext &= !bAbort;
   m_pPropElem = NULL;
}


/*func*------------------------------------------------------------------------
  description: re-synchronize the cursor with the current mouse pos.
               (the mouse pointer is not moved)
  in :-
  out:-
-----------------------------------------------------------------------------*/
void CElemEditorView::ReSyncCursorPos()
{
   CPoint pm;
   CPoint pd;
   CRect  r;

   GetCursorPos(&pm);

   if( pm.x > 0x8000) pm.x = (LONG)((SHORT)&pm.x);
   if( pm.y > 0x8000) pm.y = (LONG)((SHORT)&pm.y);
   // watch current mouse (cursor) position for keyboard handling

   ScreenToClient(&pm);
   GetClientRect(&r);
   pd = ClientToDoc (pm);

   if(pm.x > 0 && pm.x < r.right ) 
   {
      m_LogCurPos.x  = pd.x;
   }

   if( pm.y > 0 && pm.y < r.bottom) 
   {
      m_LogCurPos.y  = pd.y;
   }

   CPoint GridPos = DocToGrid(m_LogCurPos);
   SetCurPos(GridPos);
}


/*func*------------------------------------------------------------------------
  description: STEditorView - handling on move-/resize-operations
               -> call BEFORE the element is moved itself
  in :
  out:
-----------------------------------------------------------------------------*/
void CElemEditorView::UpdateViewSizePosFont(CElemAct* pAct, CSTEditorView* pView)
{
   ASSERT(pAct);
   ASSERT(pAct->ChkFlag(EF_STACTION));
   ASSERT(pView);

   if (pAct)
   {
      CRect rE, r;

      pAct->GetBodyRect(rE,m_iZGrid);
      rE.top += 2*m_iZGrid + 1;
      rE.left++;
      DocToClient(rE);

      pView->GetWindowRect(&r);
      ScreenToClient(&r);

      if (r != rE)
      {
         pView->MoveWindow(&rE);
         pView->SetContext(pAct->GetOrigin());       // container origin
      }
      pView->SetFont(&m_FixedFont,false);
   }
}


/*func*------------------------------------------------------------------------
  write back the view contents to the st-action element
  in :
  out:
-----------------------------------------------------------------------------*/
void CElemEditorView::SyncSTActionText(CElemPtrList* pList)
{
   if(pList)
   {
      POSITION pos = pList->GetHeadPosition();
      while(pos)
      {
         CElem *pE = (CElem*)pList->GetNext(pos);
         if(pE->ChkFlag(EF_STACTION))
         {
            CSTEditorView *pView;

            CElemAct* pA = (CElemAct*)pE;
            if(m_STViewMap.Lookup(pA->GetID(),pView))
            {
               CString s;
               pView->GetWindowText( s );
               pA->SetSTtext(s);
            }
         }
      }
   }
}


/*func*------------------------------------------------------------------------
  description: zoom an size update of all ST-action views
  in :
  out:
-----------------------------------------------------------------------------*/
void CElemEditorView::OnDeleteSTActions(bool bOnlySelected)
{
   CGrEditorDoc *pDoc = GetDocument();

   if (pDoc)
   {
      CElemPtrList *pList = pDoc->GetElemList();
      POSITION pos = pList->GetHeadPosition();

      while (pos)
      {
         CElem *pE = (CElem*)pList->GetNext(pos);
         if (pE->ChkFlag(EF_STACTION) && 
             (!bOnlySelected || pE->ChkFlag(EF_SELECT)))
         {
            DetachSTView((CElemAct*)pE);
         }
      }
   }
}

/*func*------------------------------------------------------------------------
  description: zoom an size update of all ST-action views
  in :
  out:
-----------------------------------------------------------------------------*/
void CElemEditorView::UpdateSTActionView(CElemAct* pAct)
{
   CGrEditorDoc *pDoc = GetDocument();
   if (pDoc)
   {
      if (!pAct)
      {
         CElemPtrList * pList = pDoc->GetElemList();
         POSITION pos = pList->GetHeadPosition();

         while (pos)                                   // for each ST-action
         {
            CElem* pE = (CElem*)pList->GetNext(pos);
            ASSERT(pE);

            if (pE->ChkFlag(EF_STACTION))
            {
               CSTEditorView* pView;
               if (m_STViewMap.Lookup( ((CElemAct*)pE)->GetID(), pView ))
               {
                  ASSERT(pView);
                  UpdateViewSizePosFont((CElemAct*)pE, pView);
               } else
               {
                  CreateSTView((CElemAct*)pE);
               }
            }
         }
      } else
      {
         ASSERT(pAct->ChkFlag(EF_STACTION));
         CSTEditorView* pView;

         if (m_STViewMap.Lookup( pAct->GetID(), pView ))
         {
            ASSERT(pView);
            UpdateViewSizePosFont( pAct, pView);
         } else
         {
            CreateSTView(pAct);
         }
      }
   }
}


/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CElemEditorView::UnselectAll()
{
   CGrEditorDoc* pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   ASSERT_VALID(this);

   CSTEditorView *pView;
   POSITION       pos;
   UINT           Key;

   pos = m_STViewMap.GetStartPosition();
   while (pos)
   {
      m_STViewMap.GetNextAssoc(pos, Key, pView);
      if (pView)
      {
         pView->ClearSelection(TRUE);
      }
   }
   pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateSelectionHint);

   if (pDoc && pDoc->GetElemList())
   {
      pDoc->GetElemList()->UnselectAll();
   }
}

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
void CElemEditorView::OnSECEditCtrlChar(WPARAM wParam, LPARAM lParam)
{
   if ((wParam == (UINT)VK_ESCAPE)   &&
       (lParam == (LONG)m_pSEC_Edit) &&
       (m_pSEC_Edit != NULL))
   {
      EditNameStop(false);
   }
}


/*func*------------------------------------------------------------------------
  insert a new action or actionblock
  in :bBlock - TRUE  action
               FALSE actionblock
  out:-
-----------------------------------------------------------------------------*/
void CElemEditorView::InsertAction(BOOL bBlock, CString strName /* = _T("") */)
{
   CString      strMask;
   CString      str;
   CPoint       origin;
   CSize        size;
   CElemAct    *pE;
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   CheckMode();
   // invalidate old select rect ..
   UnselectAll();

   // create new action
   // create only new name, if strName parameter is empty, 
   // else take the name given as parameter
   if (strName.IsEmpty())
   {
        strMask.LoadString(IDS_GR_NAMEMASK_ACT);
        str.Format(strMask, CGRTool::GetNextID(CGRTool::ID_ACT));
   }
   else
   {
       str = strName;
   }
   size.cx = 4 + CText::GetIdentLen(str);
   size.cy = 2 + (bBlock ? 0 : 6);

   CSize stdSize = GetInsertSize(IDS_GR_REG_ACTSIZE);
   if(stdSize.cx>0 && stdSize.cx>0)
   {
      size = stdSize;
   }

   // search a good position for element .. 
   origin = m_CurPos;
   origin.y -= 1;
   origin.x = max(0, min( MAX_XGRID - size.cx - 1, origin.x ));
   origin.y = max(0, min( MAX_YGRID - size.cy - 1, origin.y ));
   
   CText text(str);

   if( bBlock ) pE = new CElemActBlk(origin, size, text, QT_N); 
   else         pE = new CElemAction(origin, size, text, QT_N); 

   ASSERT(m_NextActionType == EF_LAST || m_NextActionType == EF_STACTION || m_NextActionType == EF_LDACTION);

   if(m_NextActionType != EF_LAST)
   {
       pE->SetFlag(m_NextActionType);
   }
   if( pE )
   {
      pE->SetFlag(EF_SELECT);
      pDoc->GetElemList()->InsertElem(pE); // add element to list
      m_bFirstMove = true;
      MovePos( pE->GetOrigin().x, pE->GetOrigin().y+1 );
      // goto to move mode
      pDoc->SetEditMode( EM_INSERT );
      pDoc->GetElemList()->MoveFlaggedToTail(EF_SELECT);
      pDoc->GetElemList()->MoveFlaggedToTail(EF_RUBBLINE);
      GrSetCapture();
      SetCurPos (pE->GetOrigin());
      m_BeforeOpPos = m_StartPos = m_CurPos;
      // center view.
      MoveViewToCursor(TRUE);
      UpdateCursor();
      // update the views
      pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateDrawElemHint, pE);
      // move cursor first in view - then we want to scroll ..
      m_NoAutoScroll = true;
   }
}

/*func*------------------------------------------------------------------------
  insert a new connector or continuation
  in :eConType - type of connector
  out:-
-----------------------------------------------------------------------------*/
void CElemEditorView::InsertCon(CON_TYPE eConType)
{ 
   CString      strMask;
   CString      str; 
   CPoint       origin;
   CSize        size;
   CElemCon    *pE;
   CGrEditorDoc* pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   CheckMode();
   // invalidate old select rect ..
   UnselectAll();
   
   // create new connector
   strMask.LoadString(IDS_GR_NAMEMASK_CON);
   str.Format(strMask, CGRTool::GetNextID(CGRTool::ID_CON));
   ;   
   size.cx = CText::GetIdentLen(str) +2 +2; // +pin+icon
   size.cy = 2;
   // search a good position for element .. 
   origin = m_CurPos;
   origin.y -= 1;
   origin.x = max(0, min( MAX_XGRID - size.cx - 1, origin.x ));
   origin.y = max(0, min( MAX_YGRID - size.cy - 1, origin.y ));
   ;
   switch ( eConType ) {
      case COT_IN : pE = new CElemConIn (origin, size, CText(str)); break;
      case COT_OUT: pE = new CElemConOut(origin, size, CText(str)); break;
      default     : pE = NULL; break;  
   }
   if( pE )
   {
      pE->SetFlag(EF_SELECT);
      pDoc->GetElemList()->InsertElem(pE); // add element to list
      m_bFirstMove = true;
      MovePos( pE->GetOrigin().x, pE->GetOrigin().y );
      // goto to move mode
      pDoc->SetEditMode( EM_INSERT );
      pDoc->GetElemList()->MoveFlaggedToTail(EF_SELECT);
      pDoc->GetElemList()->MoveFlaggedToTail(EF_RUBBLINE);
      GrSetCapture();
      SetCurPos (pE->GetOrigin());
      m_BeforeOpPos = m_StartPos = m_CurPos;
      // center view ..
      MoveViewToCursor(TRUE);
      UpdateCursor();
      // update the views
      pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateDrawElemHint, pE);
      // move cursor first in view - then we want to scroll ..
      m_NoAutoScroll = true;
   }
}

/*func*------------------------------------------------------------------------
  insert a new divergence or alternative
  in :bAltDiv - type of div/alt
  out:-
-----------------------------------------------------------------------------*/
void CElemEditorView::InsertDiv(BOOL bAltDiv)
{ 
   CElemDiv    *pE;
   CPoint       origin;
   CSize        size;
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   CheckMode();
   // invalidate old select rect ..
   UnselectAll();

  // create new divergence
   size.cx = 12;
   size.cy = 0;
   // search a good position for element .. 
   origin = m_CurPos;
   origin.x = max(0, min( MAX_XGRID - size.cx - 1, origin.x ));
   origin.y = max(0, min( MAX_YGRID - size.cy - 1, origin.y ));
   ;
   if( bAltDiv ) {
      pE = new CElemAltDiv(origin, size); 
   } else {
      pE = new CElemSimDiv(origin, size);
   }
   if( pE )
   {
      pE->SetFlag(EF_SELECT);
      pDoc->GetElemList()->InsertElem(pE); // add element to list
      m_bFirstMove = true;
      MovePos( pE->GetOrigin().x, pE->GetOrigin().y );
      // goto to move mode
      pDoc->SetEditMode( EM_INSERT );
      pDoc->GetElemList()->MoveFlaggedToTail(EF_SELECT);
      pDoc->GetElemList()->MoveFlaggedToTail(EF_RUBBLINE);
      GrSetCapture();
      SetCurPos (pE->GetOrigin());
      m_BeforeOpPos = m_StartPos = m_CurPos;
      // center view ..
      MoveViewToCursor(TRUE);
      UpdateCursor();
      // update the views
      pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateDrawElemHint, pE);
      // move cursor first in view - then we want to scroll ..
      m_NoAutoScroll = true;
   }
}

/*func*------------------------------------------------------------------------
  insert a new label or jump
  in :eLabType - type of label/jump
  out:-
-----------------------------------------------------------------------------*/
void CElemEditorView::InsertLabel(LAB_TYPE eLabType) 
{
   CElemLab    *pE;
   CString      strMask;
	CString      str;
   CPoint       origin;
   CSize        size;
   CGrEditorDoc* pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   CheckMode();
   // invalidate old select rect ..
   UnselectAll();
   // create new label
   strMask.LoadString(IDS_GR_NAMEMASK_LAB);
   str.Format(strMask, CGRTool::GetNextID(CGRTool::ID_LAB));
   ;   
   size.cx = CText::GetIdentLen(str) + 2   // +icon
              + ((eLabType==LT_DEST)?0:2); // +pin
   size.cy = 2;
   // search a good position for element .. 
   origin = m_CurPos;
   origin.y -= 1;
   origin.x = max(0, min( MAX_XGRID - size.cx - 1, origin.x ));
   origin.y = max(0, min( MAX_YGRID - size.cy - 1, origin.y ));
   ;
   switch ( eLabType ) {
      case LT_DEST: pE = new CElemLabel(origin, size, CText(str), TRUE); break;
      case LT_JUMP: pE = new CElemJump (origin, size, CText(str), TRUE); break;
      default     : pE = NULL; break;  
   }
   if( pE )
   {
      pE->SetFlag(EF_SELECT);
      pDoc->GetElemList()->InsertElem(pE); // add element to list
      m_bFirstMove = true;
      MovePos( pE->GetOrigin().x, pE->GetOrigin().y );
      // goto to move mode
      pDoc->SetEditMode( EM_INSERT );
      pDoc->GetElemList()->MoveFlaggedToTail(EF_SELECT);
      pDoc->GetElemList()->MoveFlaggedToTail(EF_RUBBLINE);
      GrSetCapture();
      SetCurPos (pE->GetOrigin());
      m_BeforeOpPos = m_StartPos = m_CurPos;
      // center view ..
      MoveViewToCursor(TRUE);
      UpdateCursor();
      // update the views
      pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateDrawElemHint, pE);
      // move cursor first in view - then we want to scroll ..
      m_NoAutoScroll = true;
   }
}

/*func*------------------------------------------------------------------------
  insert a new variable, constant etc
  in :eVarType - type of var
  out:-
-----------------------------------------------------------------------------*/
void CElemEditorView::InsertVar(VAR_TYPE eVarType)
{
   CString      strMask;
   CString      str;
   CPoint       origin;
   CSize        size;
   CElemVar    *pE;
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   CheckMode();
   // invalidate old select rect ..
   UnselectAll();

   // create new variable
   strMask.LoadString(IDS_GR_NAMEMASK_VAR);
   str.Format(strMask, CGRTool::GetNextID(CGRTool::ID_VAR));
   ;
   size.cx = CText::GetIdentLen(str) /*+2*/ +2 +(eVarType==VT_INOUT?2:0) +4; // EF
   size.cy = 2;               // +icon +pin +?pin 
   // search a good position for element .. 
   origin = m_CurPos;
   origin.y -= 1;
   
   if(eVarType == VT_IN)
      origin.x -= size.cx;

   origin.x = max(0, min( MAX_XGRID - size.cx - 1, origin.x ));
   origin.y = max(0, min( MAX_YGRID - size.cy - 1, origin.y ));
   ;
   switch ( eVarType ) {
      case VT_IN   : pE = new CElemVarIn   (origin, size, CText(str)); break;
      case VT_OUT  : pE = new CElemVarOut  (origin, size, CText(str)); break;
      case VT_INOUT: pE = new CElemVarInOut(origin, size, CText(str)); break;
      default      : pE = NULL; break;  
   }
   if( pE )
   {
      pE->SetFlag(EF_SELECT);
      pDoc->GetElemList()->InsertElem(pE); // add element to list
      m_bFirstMove = true;
//      MovePos( pE->GetOrigin().x, pE->GetOrigin().y );
      if(eVarType == VT_INOUT)
         MoveRel( size.cx, 0);
      // goto to move mode
      pDoc->SetEditMode( EM_INSERT );
      pDoc->GetElemList()->MoveFlaggedToTail(EF_SELECT);
      pDoc->GetElemList()->MoveFlaggedToTail(EF_RUBBLINE);
      GrSetCapture();
      SetCurPos (pE->GetOrigin());
      m_BeforeOpPos = m_StartPos = m_CurPos;
      // center view ..
      MoveViewToCursor(TRUE);
      UpdateCursor();
      // update the views
      pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateDrawElemHint, pE);
      // move cursor first in view - then we want to scroll ..
      m_NoAutoScroll = true;
   }
}

/*func*------------------------------------------------------------------------
  insert a new return element
  in :-
  out:-
-----------------------------------------------------------------------------*/
void CElemEditorView::InsertRet(void)
{
   CPoint       origin;
   CString      strName;
   CSize        size;
   CElemRet*    pE;
   CGrEditorDoc* pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   CheckMode();
   // invalidate old select rect ..
   UnselectAll();
   // insert the return element
   strName = CElemRet::GetDefName();//m_strDefName;
   size.cx = CText::GetIdentLen(strName) +2+2; // +pin+icon
   size.cy = 2;
   // search a good position for element .. 
   origin = m_CurPos;
   origin.y -= 1;
   origin.x = max(0, min( MAX_XGRID - size.cx - 1, origin.x ));
   origin.y = max(0, min( MAX_YGRID - size.cy - 1, origin.y ));
   ;
   pE = new CElemRet(origin, size);
   if( pE )
   {
      pE->SetFlag(EF_SELECT);
      pDoc->GetElemList()->InsertElem(pE); // add element to list
      m_bFirstMove = true;
      MovePos( pE->GetOrigin().x, pE->GetOrigin().y );
      // goto to move mode
      pDoc->SetEditMode( EM_INSERT );
      pDoc->GetElemList()->MoveFlaggedToTail(EF_SELECT);
      pDoc->GetElemList()->MoveFlaggedToTail(EF_RUBBLINE);
      GrSetCapture();
      SetCurPos (pE->GetOrigin());
      m_BeforeOpPos = m_StartPos = m_CurPos;
      // center view ..
      MoveViewToCursor(TRUE);
      UpdateCursor();
      // update the views
      pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateDrawElemHint, pE);
      // move cursor first in view - then we want to scroll ..
      m_NoAutoScroll = true;
   }
}

/*func*------------------------------------------------------------------------
  insert a new comment element
  in :-
  out:-
-----------------------------------------------------------------------------*/
void CElemEditorView::InsertCmt(void)
{
   CString      str;
   CPoint       origin;
   CSize        size;
   CElemCmt    *pE;
   CGrEditorDoc* pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   CheckMode();
   // invalidate old select rect ..
   UnselectAll();

   // create new comment
   size = GetCmtSize(str);
   // search a good position for element .. 
   origin = m_CurPos;
   origin.y -= 1;
   origin.x = max(0, min( MAX_XGRID - size.cx - 1, origin.x ));
   origin.y = max(0, min( MAX_YGRID - size.cy - 1, origin.y ));
   ;
   pE = new CElemCmt( origin, size, CText(str));
   if( pE )
   {
      pE->SetFlag(EF_SELECT);
      pDoc->GetElemList()->InsertElem(pE); // add element to list
      m_bFirstMove = true;
      MovePos( pE->GetOrigin().x, pE->GetOrigin().y );
      // goto to move mode
      pDoc->SetEditMode( EM_INSERT );
      pDoc->GetElemList()->MoveFlaggedToTail(EF_SELECT);
      pDoc->GetElemList()->MoveFlaggedToTail(EF_RUBBLINE);
      GrSetCapture();
      SetCurPos (pE->GetOrigin());
      m_BeforeOpPos = m_StartPos = m_CurPos;
      // center view ..
      MoveViewToCursor(TRUE);
      UpdateCursor();
      // update the views
      pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateDrawElemHint, pE);
      // move cursor first in view - then we want to scroll ..
      m_NoAutoScroll = true;
   }
}


/*func*------------------------------------------------------------------------
  insert a new step element
  in :-
  out:-
-----------------------------------------------------------------------------*/
void CElemEditorView::InsertStep()       
{ 
   CString      strMask;
   CString      str;
   CPoint       origin;
   CSize        size;
   CElemStep   *pE;
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   CheckMode();
   // invalidate old select rect ..
   UnselectAll();

   // create new step
   strMask.LoadString(IDS_GR_NAMEMASK_STEP);
   str.Format(strMask, CGRTool::GetNextID(CGRTool::ID_STEP));
   ;
   size.cx = (CText::GetIdentLen(str) + 3) & 0xFFFFFFFC; 
   size.cy = 1+2+1;

   CSize stdSize = GetInsertSize(IDS_GR_REG_STEPSIZE);
   if(stdSize.cx>0 && stdSize.cy>0)
   {
      size.cx = stdSize.cx;
      size.cy = stdSize.cy;
   }

   // search a good position for element .. 
   origin = m_CurPos;
   origin.x -= size.cx/2;
   origin.x = max(0, min( MAX_XGRID - size.cx - 1, origin.x ));
   origin.y = max(0, min( MAX_YGRID - size.cy - 1, origin.y ));
   ;
   pE = new CElemStep(origin, size, CText(str), FALSE);
   if( pE )
   {
      pE->SetFlag(EF_SELECT);
      pDoc->GetElemList()->InsertElem(pE); // add element to list
      m_bFirstMove = true;
//      MovePos( pE->GetOrigin().x, pE->GetOrigin().y );
      MoveRel(0,pE->GetSize().cy);
      // goto to move mode
      pDoc->SetEditMode( EM_INSERT );
      pDoc->GetElemList()->MoveFlaggedToTail(EF_SELECT);
      pDoc->GetElemList()->MoveFlaggedToTail(EF_RUBBLINE);
      GrSetCapture();
      SetCurPos (pE->GetOrigin());
      m_BeforeOpPos = m_StartPos = m_CurPos;
      // center view ..
      MoveViewToCursor(TRUE);
      UpdateCursor();
      // update the views
      pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateDrawElemHint, pE);
      // move cursor first in view - then we want to scroll ..
      m_NoAutoScroll = true;
   }
}

/*func*------------------------------------------------------------------------
  insert a new transition element
  in :-
  out:-
-----------------------------------------------------------------------------*/
void CElemEditorView::InsertTran() 
{ 
   CString      strMask;
   CString      str;
   CPoint       origin;
   CSize        size;
   CElemTran   *pE;
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   CheckMode();
   // invalidate old select rect ..
   UnselectAll();
   // create new transition
   strMask.LoadString(IDS_GR_NAMEMASK_TRAN);
   str.Format(strMask, CGRTool::GetNextID(CGRTool::ID_TRAN));
   ;   
   size.cx = 4 + CText::GetIdentLen(str);
   size.cy = 2+2;
   CSize stdSize = GetInsertSize(IDS_GR_REG_TRANSIZE);
   if(stdSize.cx>0)
   {
      size.cx = stdSize.cx;
   }

   // search a good position for element .. 
   origin = m_CurPos;
//   origin.y -= 1;
   origin.x -= 2;
   origin.x = max(0, min( MAX_XGRID - size.cx - 1, origin.x ));
   origin.y = max(0, min( MAX_YGRID - size.cy - 1, origin.y ));
   ;
   pE = new CElemTran(origin, size, CText(str));
   if( pE )
   {
      pE->SetFlag(EF_SELECT);
      pDoc->GetElemList()->InsertElem(pE); // add element to list
      m_bFirstMove = true;
///      MovePos( pE->GetOrigin().x, pE->GetOrigin().y );
      MoveRel(0,pE->GetSize().cy);
      // goto to move mode
      pDoc->SetEditMode( EM_INSERT );
      pDoc->GetElemList()->MoveFlaggedToTail(EF_SELECT);
      pDoc->GetElemList()->MoveFlaggedToTail(EF_RUBBLINE);
      GrSetCapture();
      SetCurPos (pE->GetOrigin());
      m_BeforeOpPos = m_StartPos = m_CurPos;
      // center view ..
      MoveViewToCursor(TRUE);
      UpdateCursor();
      // update the views
      pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateDrawElemHint, pE);
      // move cursor first in view - then we want to scroll ..
      m_NoAutoScroll = true;
   }
}

/*func*------------------------------------------------------------------------
  insert a new ladder power element
  in :-
  out:-
-----------------------------------------------------------------------------*/
void   CElemEditorView::InsertPower  (bool bLeft)
{
   CPoint       origin;
   CSize        size;
   CElemPower  *pE;
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   CheckMode();
   // invalidate old select rect ..
   UnselectAll();

   // create new power
   size.cx = 0; 
   size.cy = 6;
   // search a good position for element .. 
   CSize stdSize = GetInsertSize(IDS_GR_REG_PRAILSIZE);
   if(stdSize.cy>0)
   {
      size.cy = stdSize.cy;
   }

   origin = m_CurPos;
//   origin.y -= 1;
   origin.x = max(0, min( MAX_XGRID - size.cx - 1, origin.x ));
   origin.y = max(0, min( MAX_YGRID - size.cy - 1, origin.y ));
   ;
   pE = new CElemPower (origin, size); 
   if( pE )
   {
      pE->SetFlag(EF_SELECT);
      pDoc->GetElemList()->InsertElem(pE); // add element to list
      m_bFirstMove = true;
      MovePos( pE->GetOrigin().x, pE->GetOrigin().y );
      // goto to move mode
      pDoc->SetEditMode( EM_INSERT );
      pDoc->GetElemList()->MoveFlaggedToTail(EF_SELECT);
      pDoc->GetElemList()->MoveFlaggedToTail(EF_RUBBLINE);
      GrSetCapture();
      SetCurPos (pE->GetOrigin());
      m_BeforeOpPos = m_StartPos = m_CurPos;
      // center view ..
      MoveViewToCursor(TRUE);
      UpdateCursor();
      // update the views
      pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateDrawElemHint, pE);
      // move cursor first in view - then we want to scroll ..
      m_NoAutoScroll = true;
   }
}

/*func*------------------------------------------------------------------------
  insert a new ladder contact element
  in :-
  out:-
-----------------------------------------------------------------------------*/
void   CElemEditorView::InsertContact(LADDER_TYPE eLdType)
{
   CString      strMask;
   CString      str;
   CPoint       origin;
   CSize        size;
   CElemContact *pE;
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   CheckMode();
   // invalidate old select rect ..
   UnselectAll();

   // create new coil
   strMask.LoadString(IDS_GR_NAMEMASK_CONTACT);
   str.Format(strMask, CGRTool::GetNextID(CGRTool::ID_CONTACT));
   ;
   size.cx = CText::GetIdentLen(str) +2; 
   size.cy = 4;               // +icon +pin +?pin 

   CSize stdSize = GetInsertSize(IDS_GR_REG_CTSIZE);
   if(stdSize.cx > 0)
   {
      size.cx = stdSize.cx;
   }

   // search a good position for element .. 
   origin = m_CurPos;
   origin.y -= 3; // y pin distance from origin

   origin.x = max(0, min( MAX_XGRID - size.cx - 1, origin.x ));
   origin.y = max(0, min( MAX_YGRID - size.cy - 1, origin.y ));

   pE = new CElemContact(origin, size, CText(str), eLdType); 
   if( pE )
   {
      pE->SetFlag(EF_SELECT);
      pDoc->GetElemList()->InsertElem(pE); // add element to list
      m_bFirstMove = true;
//      MovePos( pE->GetOrigin().x, pE->GetOrigin().y );
      MovePos( pE->GetOrigin().x+pE->GetSize().cx, pE->GetOrigin().y+pE->GetSize().cy-1 );
      // goto to move mode
      pDoc->SetEditMode( EM_INSERT );
      pDoc->GetElemList()->MoveFlaggedToTail(EF_SELECT);
      pDoc->GetElemList()->MoveFlaggedToTail(EF_RUBBLINE);
      GrSetCapture();
//      SetCurPos (pE->GetOrigin());
      m_BeforeOpPos = m_StartPos = m_CurPos;
      // center view ..
      MoveViewToCursor(TRUE);
      UpdateCursor();
      // update the views
      pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateDrawElemHint, pE);
      // move cursor first in view - then we want to scroll ..
      m_NoAutoScroll = true;
   }
}

/*func*------------------------------------------------------------------------
  insert a new ladder coil element
  in :-
  out:-
-----------------------------------------------------------------------------*/
void   CElemEditorView::InsertCoil   (LADDER_TYPE eLdType)
{
   CString      strMask;
   CString      str;
   CPoint       origin;
   CSize        size;
   CElemCoil   *pE;
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   CheckMode();
   // invalidate old select rect ..
   UnselectAll();

   // create new coil
   strMask.LoadString(IDS_GR_NAMEMASK_COIL);
   str.Format(strMask, CGRTool::GetNextID(CGRTool::ID_COIL));
   ;
   size.cx = CText::GetIdentLen(str) +2; 
   size.cy = 4;               // +icon +pin +?pin 

   CSize stdSize = GetInsertSize(IDS_GR_REG_CLSIZE);
   if(stdSize.cx > 0)
   {
      size.cx = stdSize.cx;
   }

   // search a good position for element .. 
   origin = m_CurPos;
   origin.y -= 3; // y pin distance from origin

   origin.x = max(0, min( MAX_XGRID - size.cx - 1, origin.x ));
   origin.y = max(0, min( MAX_YGRID - size.cy - 1, origin.y ));
   ;
   pE = new CElemCoil (origin, size, CText(str), eLdType); 
   if( pE )
   {
      pE->SetFlag(EF_SELECT);
      pDoc->GetElemList()->InsertElem(pE); // add element to list
      m_bFirstMove = true;
//      MovePos( pE->GetOrigin().x, pE->GetOrigin().y );
      MovePos( pE->GetOrigin().x+pE->GetSize().cx, pE->GetOrigin().y+pE->GetSize().cy-1 );
      // goto to move mode
      pDoc->SetEditMode( EM_INSERT );
      pDoc->GetElemList()->MoveFlaggedToTail(EF_SELECT);
      pDoc->GetElemList()->MoveFlaggedToTail(EF_RUBBLINE);
      GrSetCapture();
//      SetCurPos (pE->GetOrigin());
      m_BeforeOpPos = m_StartPos = m_CurPos;
      // center view ..
      MoveViewToCursor(TRUE);
      UpdateCursor();
      // update the views
      pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateDrawElemHint, pE);
      // move cursor first in view - then we want to scroll ..
      m_NoAutoScroll = true;
   }
}

/*func*------------------------------------------------------------------------
  insert a new line element
  in :-
  out:-
-----------------------------------------------------------------------------*/
CElemLine *CElemEditorView::InsertLine(bool bHLine, bool bDirBR)
{
   CString      strMask;
   CString      str;
   CPoint       origin;
   CSize        size;
   CElemLine   *pE;
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   CheckMode();
   // invalidate old select rect ..
   UnselectAll();

   CSize stdSize = GetInsertSize(IDS_GR_REG_LINESIZE);

   if(bHLine)
   {
      size.cy = 0;
      int len = stdSize.cx>0 ?stdSize.cx :CElemLine::GetDefaultSize().cx;
      size.cx = bDirBR ?len :-len;
   }else
   {
      size.cx = 0;
      int len = stdSize.cy>0 ?stdSize.cy :CElemLine::GetDefaultSize().cy;
      size.cy = bDirBR ?len :-len;
   }

   // search a good position for element .. 
   origin = m_CurPos;
   origin.x = max(0, min( MAX_XGRID - size.cx - 1, origin.x ));
   origin.y = max(0, min( MAX_YGRID - size.cy - 1, origin.y ));
   ;
   pE = new CElemLine(origin, origin+size, false); 
   if( pE )
   {
      pE->SetFlag(EF_SELECT);
      pDoc->GetElemList()->InsertElem(pE); // add element to list
      m_bFirstMove = true;

      MovePos( (origin+size).x, (origin+size).y );

//      if(pE->GetSize().cx<0 || pE->GetSize().cy<0 )   // normalize
//         pE->SwapOrigin();

      pDoc->SetEditMode( EM_INSERT );
      pDoc->GetElemList()->MoveFlaggedToTail(EF_SELECT);
      pDoc->GetElemList()->MoveFlaggedToTail(EF_RUBBLINE);
      GrSetCapture();

      m_BeforeOpPos = m_StartPos = m_CurPos;
      // center view ..
      MoveViewToCursor(TRUE);
      UpdateCursor();
      // update the views
      pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateDrawElemHint, pE);
      // move cursor first in view - then we want to scroll ..
      m_NoAutoScroll = true;
   }
   return pE;
}

/*func*------------------------------------------------------------------------
  change all properties of an existing connector
  in :pE - pointer to connector
  out:-
-----------------------------------------------------------------------------*/
void CElemEditorView::PropCon(CElemCon *pE)
{
	CStringList  nameList;
	CStringList  typeList;
   CInsConDlg   dlg;
	LPCTSTR      pstr; 
   CElem       *pTE;
   CElemCon    *pEnew;
   POSITION     pos;
   CPoint       origin;
   CSize        size;
   CGrEditorDoc* pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   ASSERT_VALID(pE);

   // fill list with existing connectors
   for( pos = pDoc->GetElemList()->GetHeadPosition(); pos != NULL; )
   {
      pTE = pDoc->GetElemList()->GetNext(pos);
      if( (pTE->GetType() == ET_CONIN) ||
          (pTE->GetType() == ET_CONOUT)) 
      {
         pstr = (LPCTSTR)((CElemCon*)pTE)->GetName();
         if (pstr && !nameList.Find( pstr )) 
         { 
            nameList.AddTail( pstr );
         }
      }
   }
   // Initialize dialog data
   dlg.m_pNameList = &nameList;
   // set data of connector (change mode)
   // default dlg data for insert mode has been set in constructor of dlg
   dlg.m_strName  = ((CElemCon*)pE)->GetName();
   dlg.m_iConPos  = (pE->GetType() == ET_CONIN ? 0 : 1);
   // Invoke the dialog box
   if (dlg.DoModal() == IDOK)
   {
      // invalidate old select rect ..
      UnselectAll();
      pDoc->m_Action.StoreState(pDoc->GetElemList());
      pDoc->SetModifiedFlag(pDoc->m_Action.StateChangedQ());
      // retrieve the dialog data
      if       ( (CON_TYPE)dlg.m_iConPos == COT_IN ) {
         pEnew = new CElemConIn ( pE ); 
      } else if( (CON_TYPE)dlg.m_iConPos == COT_OUT) {
         pEnew = new CElemConOut( pE );
      } else {
         ASSERT(FALSE); 
         pEnew = NULL; 
      }
      ASSERT( pEnew );
      if( pEnew ) 
      {
         pEnew->SetName(dlg.m_strName);
         // change the element
         pDoc->GetElemList()->ReplaceElem(pE, pEnew);
      } 

      CheckPropEnd(pEnew);
         // center view ..
      MoveViewToCursor();
      UpdateCursor();
   }
}

/*func*------------------------------------------------------------------------
  change all properties of an existing label
  in :pE - pointer to label
  out:-
-----------------------------------------------------------------------------*/
void CElemEditorView::PropLab(CElemLab *pE)
{
	CStringList  nameList;
	CStringList  typeList;
   CInsLabDlg   dlg;
	LPCTSTR      psz;
   CElem       *pTE;
   CElemLab    *pEnew;
   POSITION     pos;
   CPoint       origin;
   CSize        size;
   CGrEditorDoc* pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   ASSERT_VALID(pE);

   // fill list with existing labels
   for( pos = pDoc->GetElemList()->GetHeadPosition(); pos != NULL; )
   {
      pTE = pDoc->GetElemList()->GetNext(pos);
      if( (pTE->GetType() == ET_LABEL) ||
          (pTE->GetType()  == ET_JUMP) )
      {  
         psz = (LPCTSTR)((CElemLab*)pTE)->GetName();
         if (psz && !nameList.Find( psz )) 
         { 
            nameList.AddTail( psz );
         }
      }
   }
   // Initialize dialog data
   dlg.m_pNameList = &nameList;
   // set data of label (change mode)
   // default dlg data for insert mode has been set in constructor of dlg
   dlg.m_strName  = ((CElemLab*)pE)->GetName();
   dlg.m_iLabTyp = (pE->GetType() == ET_LABEL ? LT_DEST : LT_JUMP);
   dlg.m_bLine   = pE->GetLine();
   // Invoke the dialog box
   if (dlg.DoModal() == IDOK)
   {
      // invalidate old select rect ..
      UnselectAll();
      pDoc->m_Action.StoreState(pDoc->GetElemList());
      pDoc->SetModifiedFlag(pDoc->m_Action.StateChangedQ());
      // retrieve the dialog data
      if(      (LAB_TYPE)dlg.m_iLabTyp == LT_DEST) 
      {
         pEnew = new CElemLabel( pE ); 
      } 
      else if ((LAB_TYPE)dlg.m_iLabTyp == LT_JUMP) 
      {
         pEnew = new CElemJump ( pE ); 
      } else {
         ASSERT(FALSE); 
         pEnew = NULL;
      }
      ASSERT( pEnew );
      if( pEnew )
      {
         pEnew->SetName(dlg.m_strName);
         pEnew->SetLine(dlg.m_bLine);
         // change the element
         pDoc->GetElemList()->ReplaceElem(pE, pEnew);
      } 
      CheckPropEnd(pEnew);
      // center view ..
      MoveViewToCursor();
      UpdateCursor();
   }
}

/*func*------------------------------------------------------------------------
  insert a new or change an existing comment
  in :pE - pointer to comment, if NULL - create a new comment
  out:-
-----------------------------------------------------------------------------*/
void CElemEditorView::PropCmt(CElemCmt * pE)
{
   CInsCmtDlg   dlg;
   CPoint       origin;
   CSize        size;
   CGrEditorDoc* pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   ASSERT_VALID(pE);

   // set data of comment (change mode)
   // default dlg data for insert mode has been set in constructor of dlg
   dlg.m_strText  = pE->GetText();
   // Invoke the dialog box
   if (dlg.DoModal() == IDOK)
   {
      // invalidate old select rect ..
      UnselectAll();

      pE->SetText(dlg.m_strText);
      // center view ..
      MoveViewToCursor();
      UpdateCursor();
      // update the views
      pDoc->m_Action.StoreState(pDoc->GetElemList());  // op. ok -> store
      pDoc->SetModifiedFlag(pDoc->m_Action.StateChangedQ());
      pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateDrawElemHint, pE);
   }
}


void CElemEditorView::SetActionType(CElemAct* pE, ActionType type)
{
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   bool bClear = false;

   switch (type)
   {
   case AT_LD:
      if (!(pE->ChkFlag(EF_LDACTION) && !pE->ChkFlag(EF_STACTION)))
      {
         if (pE->ChkFlag(EF_STACTION)) 
            DetachSTView(pE);
         bClear = !pE->ChkFlag(EF_LDACTION) || pE->ChkFlag(EF_STACTION);
         pE->SetFlag(EF_LDACTION);
         pE->ClrFlag(EF_STACTION);
      }
      break;

   case AT_ST:
      if (!(!pE->ChkFlag(EF_LDACTION) && pE->ChkFlag(EF_STACTION)))
      {
         bClear =  pE->ChkFlag(EF_LDACTION) || !pE->ChkFlag(EF_STACTION);
         pE->ClrFlag(EF_LDACTION);
         pE->SetFlag(EF_STACTION);
         CreateSTView(pE);
      }
      break;
   
   case AT_FBD:
      if (!(!pE->ChkFlag(EF_LDACTION) && !pE->ChkFlag(EF_STACTION)))
      {
         if (pE->ChkFlag(EF_STACTION)) 
            DetachSTView(pE);
         bClear =  pE->ChkFlag(EF_LDACTION) || pE->ChkFlag(EF_STACTION);
         pE->ClrFlag(EF_LDACTION);
         pE->ClrFlag(EF_STACTION);
      }
      break;
   
   default:
      ASSERT(0 &&"invalid action type");
   }

   if (bClear)
   {
      while (pE->GroupSize())
      {
         pDoc->GetElemList()->DeleteElem(pE->GetGroupElem(0));
      }
      ASSERT(pE->GroupSize()==0);
   }
}


/*func*------------------------------------------------------------------------
  change all properties of an existing action
  in :pE - pointer to action
  out:-
-----------------------------------------------------------------------------*/
void CElemEditorView::PropAct(CElemAct * pE)
{
	CStringList   nameList;
   CInsActDlg    dlg;
	LPCTSTR       psz;

   CElem        *pTE;
   POSITION      pos;
   CPoint        origin;
   CSize         size;
   CGrEditorDoc* pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   ASSERT_VALID(pE);

   // fill list with existing action(block)s
   for( pos = pDoc->GetElemList()->GetHeadPosition(); pos != NULL; )
   {
      pTE = pDoc->GetElemList()->GetNext(pos);
      if( (pTE->GetType() == ET_ACTIONBLOCK) ||
          (pTE->GetType() == ET_ACTION) ) 
      {
         psz = (LPCTSTR)((CElemAct*)pTE)->GetName();
         if (psz && !nameList.Find( psz )) 
         { 
            nameList.AddTail( psz );
         }
      }
   }
   // Initialize dialog data
   dlg.m_pNameList = &nameList;
   // set data of variable (change mode)
   // default dlg data for insert mode has been set in constructor of dlg
   dlg.m_strName   = ((CElemAct*)pE)->GetName();
   dlg.m_bSTaction = pE->ChkFlag(EF_STACTION) ? TRUE : FALSE;
   dlg.m_bLDaction = pE->ChkFlag(EF_LDACTION) ? TRUE : FALSE;
   // warning for STaction
   if (!pE->ChkFlag(EF_STACTION))
   {
      pE->SetFlag(EF_STACTION);
      if (pDoc->GetElemList()->IsElemOverlapped(pE))
      {
         dlg.m_bSTactWarn = TRUE;
      } else {
         dlg.m_bSTactWarn = FALSE;
      }
      pE->ClrFlag(EF_STACTION);
      pDoc->GetElemList()->IsOneOverlapped();
   }
   // Invoke the dialog box
   if (dlg.DoModal() == IDOK)
   {
      pDoc->m_Action.StoreState(pDoc->GetElemList());
      pDoc->SetModifiedFlag(pDoc->m_Action.StateChangedQ());
      // invalidate old select rect ..
      UnselectAll();

      pE->SetName(dlg.m_strName);
      if (dlg.m_bSTaction == TRUE )          // switch to ST
      {
         SetActionType(pE, AT_ST);
      } else if (dlg.m_bLDaction == TRUE )   // switch to LD
      {
         SetActionType(pE, AT_LD);
      } else
      {
         SetActionType(pE, AT_FBD);
      }

      if(dlg.m_bSetDefaultSize)
      {
         SetInsertSize(IDS_GR_REG_ACTSIZE,pE->GetSize());
      }

      CheckPropEnd(pE);
      // center view ..
      MoveViewToCursor();
      UpdateCursor();
   }
}

/*func*------------------------------------------------------------------------
  change all properties of an existing action
  in :pE - pointer to action
  out:-
-----------------------------------------------------------------------------*/
void CElemEditorView::PropActBlk(CElemActBlk * pE)
{
	CStringList   nameList;
	CStringList   qualList;
   CInsActBlkDlg dlg;
	LPCTSTR       psz;
   CElem        *pTE;
   POSITION      pos;
   CPoint        origin;
   CSize         size;
   CGrEditorDoc* pDoc = GetDocument();

   ASSERT_VALID(pDoc);
   ASSERT_VALID(pE);

   CString      strQual[QT_LAST]; // QT_none .. QT_P1
   for (int i=QT_none; i<QT_LAST; i++)
   {
      strQual[i].LoadString(IDS_GR_DLG_PROPACT_QUAL_NONE+i);
   }

   // fill list with existing action(block)s
   for (pos = pDoc->GetElemList()->GetHeadPosition(); pos != NULL; )
   {
      pTE = pDoc->GetElemList()->GetNext(pos);
      if( (pTE->GetType() == ET_ACTIONBLOCK) ||
          (pTE->GetType() == ET_ACTION) ) 
      {
         psz = (LPCTSTR)((CElemAct*)pTE)->GetName();
         if (psz && !nameList.Find( psz )) 
         { 
            nameList.AddTail( psz );
         }
      }
   }
   // Initialize dialog data
   dlg.m_pNameList = &nameList;

   qualList.AddTail(strQual[QT_none]);
   qualList.AddTail(strQual[QT_N   ]);
   qualList.AddTail(strQual[QT_R   ]);
   qualList.AddTail(strQual[QT_S   ]);
#ifndef V120_SFC_NOTIMEQUALIFIER
   qualList.AddTail(strQual[QT_L   ]);
   qualList.AddTail(strQual[QT_D   ]);
#endif
   qualList.AddTail(strQual[QT_P   ]);
#ifndef V120_SFC_NOTIMEQUALIFIER
   qualList.AddTail(strQual[QT_SD  ]);
   qualList.AddTail(strQual[QT_DS  ]);
   qualList.AddTail(strQual[QT_SL  ]);
#endif
   qualList.AddTail(strQual[QT_P0  ]);
   qualList.AddTail(strQual[QT_P1  ]);
   dlg.m_pQualList = &qualList;
   for (i=QT_none; i<QT_LAST; i++)
   {
      dlg.strQual[i] = strQual[i];
   }
   // set data of variable (change mode)
   // default dlg data for insert mode has been set in constructor of dlg
   dlg.m_strName = pE->GetName ();
#ifdef V120_SFC_NOTIMEQUALIFIER
   QUAL_TYPE eQualifier=pE->GetQualifier();

   dlg.m_strQual  = strQual[eQualifier];
   switch (eQualifier)
   {
      case QT_none: eQualifier = QT_none; break;
      case QT_N   : eQualifier = QT_N;    break;
      case QT_R   : eQualifier = QT_R;    break;
      case QT_S   : eQualifier = QT_S;    break; 
      case QT_L   : eQualifier = QT_none; break;
      case QT_D   : eQualifier = QT_none; break;
      case QT_P   : eQualifier = QT_L;    break;
      case QT_SD  : eQualifier = QT_none; break;
      case QT_DS  : eQualifier = QT_none; break;
      case QT_SL  : eQualifier = QT_none; break;
      case QT_P0  : eQualifier = QT_D;    break;
      case QT_P1  : eQualifier = QT_P;    break;
   }
#else
   dlg.m_strQual  = strQual[pE->GetQualifier()];
#endif
#ifndef V120_SFC_NOTIMEQUALIFIER
   pE->GetTime(dlg.m_strTime);
#endif
   dlg.m_bSTaction = pE->ChkFlag(EF_STACTION) ? TRUE : FALSE;
   dlg.m_bLDaction = pE->ChkFlag(EF_LDACTION) ? TRUE : FALSE;
   // warning for STaction
   if (!pE->ChkFlag(EF_STACTION))
   {
      pE->SetFlag(EF_STACTION);
      if (pDoc->GetElemList()->IsElemOverlapped(pE))
      {
         dlg.m_bSTactWarn = TRUE;
      } else {
         dlg.m_bSTactWarn = FALSE;
      }
      pE->ClrFlag(EF_STACTION);
      pDoc->GetElemList()->IsOneOverlapped();
   }
   // Invoke the dialog box
   if (dlg.DoModal() == IDOK)
   {
      // retrieve the dialog data
      QUAL_TYPE eQualifier=QT_none;

      if     ( dlg.m_strQual == strQual[QT_none] ) eQualifier = QT_none;
      else if( dlg.m_strQual == strQual[QT_N   ] ) eQualifier = QT_N;
      else if( dlg.m_strQual == strQual[QT_R   ] ) eQualifier = QT_R;
      else if( dlg.m_strQual == strQual[QT_S   ] ) eQualifier = QT_S;
      else if( dlg.m_strQual == strQual[QT_L   ] ) eQualifier = QT_L;
      else if( dlg.m_strQual == strQual[QT_D   ] ) eQualifier = QT_D;
      else if( dlg.m_strQual == strQual[QT_P   ] ) eQualifier = QT_P;
      else if( dlg.m_strQual == strQual[QT_SD  ] ) eQualifier = QT_SD;
      else if( dlg.m_strQual == strQual[QT_DS  ] ) eQualifier = QT_DS;
      else if( dlg.m_strQual == strQual[QT_SL  ] ) eQualifier = QT_SL;
      else if( dlg.m_strQual == strQual[QT_P0  ] ) eQualifier = QT_P0;
      else if( dlg.m_strQual == strQual[QT_P1  ] ) eQualifier = QT_P1;
      
      pDoc->m_Action.StoreState(pDoc->GetElemList());
      pDoc->SetModifiedFlag(pDoc->m_Action.StateChangedQ());
      // invalidate old select rect ..
      UnselectAll();

      pE->SetName(dlg.m_strName);
      pE->SetQualifier(eQualifier);
#ifndef V120_SFC_NOTIMEQUALIFIER
      pE->SetTime(dlg.m_strTime);
#endif
      if (dlg.m_bSTaction == TRUE )          // switch to ST
      {
         SetActionType(pE, AT_ST);
      } else if (dlg.m_bLDaction == TRUE )   // switch to LD
      {
         SetActionType(pE, AT_LD);
      } else
      {
         SetActionType(pE, AT_FBD);
      }

      if(dlg.m_bSetDefaultSize)
      {
         SetInsertSize(IDS_GR_REG_ACTSIZE,pE->GetSize());
      }

      // no change of m_bActBlock flag !
      CheckPropEnd(pE);
      // center view ..
      MoveViewToCursor();
      UpdateCursor();
   }
}

/*func*------------------------------------------------------------------------
  change all properties of an existing variable
  in :pE - pointer to variable
  out:-
-----------------------------------------------------------------------------*/
void CElemEditorView::PropVar(CElemVar *pE)
{
	CStringList  nameList;
	CStringList  typeList;
   CInsVarDlg   dlg;
	LPCTSTR      psz;
   CElem       *pTE;
   CElemVar    *pEnew;
   POSITION     pos;
   CPoint       origin;
   CSize        size;
   CGrEditorDoc*pDoc = GetDocument();

   ASSERT_VALID(pDoc);
   ASSERT_VALID(pE);

   CString      strType[VT_LAST];
   for (int i=VT_IN; i<VT_LAST; i++)
   {
      strType[i].LoadString(IDS_GR_DLG_PROPVAR_TYPE_VARREAD+i);
   }
   // fill list with existing variables
   for( pos = pDoc->GetElemList()->GetHeadPosition(); pos != NULL; )
   {
      pTE = pDoc->GetElemList()->GetNext(pos);
      if( (pTE->GetType() == ET_VARIN)  || 
          (pTE->GetType() == ET_VAROUT) || 
          (pTE->GetType() == ET_VARINOUT) ) 
      {
         psz = (LPCTSTR)((CElemVar*)pTE)->GetName();
         if (psz && !nameList.Find( psz )) 
         { 
            nameList.AddTail( psz );
         }
      }
   }
   // Initialize dialog data
   dlg.m_pNameList = &nameList;

   typeList.AddTail(strType[VT_IN ]);
   typeList.AddTail(strType[VT_OUT]);
   typeList.AddTail(strType[VT_INOUT]);
   dlg.m_pTypeList = &typeList;
   // set data of variable (change mode)
   // default dlg data for insert mode has been set in constructor of dlg
   dlg.m_strName  = ((CElemVar*)pE)->GetName();
   if       ( pE->GetType() == ET_VARIN    ) {
      dlg.m_strType = strType[VT_IN];
   } else if( pE->GetType() == ET_VAROUT   ) {
      dlg.m_strType = strType[VT_OUT];
   } else if( pE->GetType() == ET_VARINOUT ) {
      dlg.m_strType = strType[VT_INOUT];
   } else {
      ASSERT(FALSE);
   }
   // Invoke the dialog box
   if (dlg.DoModal() == IDOK)
   {
      // invalidate old select rect ..
      UnselectAll();
      pDoc->m_Action.StoreState(pDoc->GetElemList());  // op. ok -> store
      pDoc->SetModifiedFlag(pDoc->m_Action.StateChangedQ());
      // retrieve the dialog data
      if       ( dlg.m_strType == strType[VT_IN] ) {
         pEnew = new CElemVarIn   ( pE ); 
      } else if( dlg.m_strType == strType[VT_OUT] ) {
         pEnew = new CElemVarOut  ( pE );
      } else if( dlg.m_strType == strType[VT_INOUT] ) {
         pEnew = new CElemVarInOut( pE ); 
      } else {
         ASSERT(FALSE); 
         pEnew = NULL;
      }
      ASSERT( pEnew );
      if( pEnew )
      {
         pEnew->SetName(dlg.m_strName);
         if( pEnew->GetName() == VAR_AUTONAME)
         {
            pE->SetFlag(EF_AUTOELEM);
         }

         // change the element
         pDoc->GetElemList()->ReplaceElem(pE, pEnew);
      } 
      CheckPropEnd(pEnew);
      // center view ..
      MoveViewToCursor();
      UpdateCursor();
   }
}

/*func*------------------------------------------------------------------------
  change all properties of an existing step
  in :pE - pointer to step
  out:-
-----------------------------------------------------------------------------*/
void CElemEditorView::PropStep(CElemStep * pE)
{
	CStringList  nameList;
   CInsStepDlg  dlg;
	LPCTSTR      psz;
   CElem       *pTE;
   POSITION     pos;
   CPoint       origin;
   CSize        size;
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   ASSERT_VALID(pE);

   // fill list with existing steps
   for( pos = pDoc->GetElemList()->GetHeadPosition(); pos != NULL; )
   {
      pTE = pDoc->GetElemList()->GetNext(pos);
      if( pTE->GetType() == ET_STEP ) 
      {
         psz = (LPCTSTR)((CElemStep*)pTE)->GetName();
         if (psz && !nameList.Find( psz )) 
         { 
            nameList.AddTail( psz );
         }
      }
   }
   // Initialize dialog data
   dlg.m_pNameList = &nameList;

   // set data of variable (change mode)
   // default dlg data for insert mode has been set in constructor of dlg
   dlg.m_strName  = ((CElemStep*)pE)->GetName();
   dlg.m_bInitial = pE->GetInitType();
   // Invoke the dialog box
   if (dlg.DoModal() == IDOK)
   {
      // retrieve the dialog data
      if(dlg.m_bCopyDefaultSize)
      {
         SetInsertSize(IDS_GR_REG_STEPSIZE, pE->GetSize());
      }
      // invalidate old select rect ..
      UnselectAll();
      // change to move mode
      pE->SetName(dlg.m_strName);
      pE->SetInitType(dlg.m_bInitial);
      // center view ..
      MoveViewToCursor();
      UpdateCursor();
      // update the views
      pDoc->m_Action.StoreState(pDoc->GetElemList());  // op. ok -> store
      pDoc->SetModifiedFlag(pDoc->m_Action.StateChangedQ());
      pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateDrawElemHint, pE);
   }
}

/*func*------------------------------------------------------------------------
  insert a new or change an existing transition
  in :pE - pointer to step, if NULL - create a new step
  out:-
-----------------------------------------------------------------------------*/
void CElemEditorView::PropTran(CElemTran * pE)
{
	CStringList  nameList;
   CInsTranDlg  dlg;
	LPCTSTR      psz;
   CElem       *pTE;
   POSITION     pos;
   CPoint       origin;
   CSize        size;
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   ASSERT_VALID(pE);

   // fill list with existing transitions
   for( pos = pDoc->GetElemList()->GetHeadPosition(); pos != NULL; )
   {
      pTE = pDoc->GetElemList()->GetNext(pos);
      if( pTE->GetType() == ET_TRANSITION ) 
      {
         psz = (LPCTSTR)((CElemTran*)pTE)->GetName();
         if (psz && !nameList.Find( psz )) 
         { 
            nameList.AddTail( psz );
         }
      }
   }
   // Initialize dialog data
   dlg.m_pNameList = &nameList;
   // set data of variable (change mode)
   // default dlg data for insert mode has been set in constructor of dlg
   dlg.m_strName  = ((CElemTran*)pE)->GetName();
   // Invoke the dialog box
   if (dlg.DoModal() == IDOK)
   {
      // retrieve the dialog data
      if(dlg.m_bSetDefaultSize)
         SetInsertSize(IDS_GR_REG_TRANSIZE, pE->GetSize());
      // invalidate old select rect ..
      UnselectAll();

      pE->SetName(dlg.m_strName);
      // center view ..
      MoveViewToCursor();
      UpdateCursor();
      // update the views
      pDoc->m_Action.StoreState(pDoc->GetElemList());  // op. ok -> store
      pDoc->SetModifiedFlag(pDoc->m_Action.StateChangedQ());
      pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateDrawElemHint, pE);
   }
}

/*func*------------------------------------------------------------------------
  insert a new or change an existing divergence
  in :pE - pointer to divergence, if NULL - create a new divergence
  out:-
-----------------------------------------------------------------------------*/
void CElemEditorView::PropDiv(CElemDiv *pE)
{
   DIV_TYPE     eType;
	CStringList  typeList;
   CInsDivDlg   dlg;
   CElemDiv    *pEnew;
   CPoint       origin;
   CSize        size;
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   ASSERT_VALID(pE);

   // set data of div/con (change mode)
   // default dlg data for insert mode has been set in constructor of dlg
   if (pE->GetType() == ET_ALTDIV) 
   {
      eType = IT_ALTDIV;
   } else {
      eType = IT_SIMDIV;
   }

   if( (eType >= IT_ALTDIV) && (eType < IT_LAST) )
   {
      dlg.m_iSelection = eType;
   }
   // Invoke the dialog box
   if( dlg.DoModal() == IDOK)
   {
      // invalidate old select rect ..
      UnselectAll();
      // retrieve the dialog data
      if       ( dlg.m_iSelection == IT_ALTDIV ) {
         pEnew = new CElemAltDiv( pE ); 
      } else if( dlg.m_iSelection == IT_SIMDIV ) {
         pEnew = new CElemSimDiv( pE ); 
      } else {
         pEnew = NULL;
         ASSERT(FALSE);
      }
      ASSERT( pEnew );
      if( pEnew )
      {
         // change the element
         pDoc->GetElemList()->ReplaceElem(pE, pEnew);
      } 
      // center view ..
      MoveViewToCursor();
      UpdateCursor();
      // update the views
      pDoc->m_Action.StoreState(pDoc->GetElemList());  // op. ok -> store
      pDoc->SetModifiedFlag(pDoc->m_Action.StateChangedQ());
      pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateDrawElemHint, pEnew);
   }
}


/*func*------------------------------------------------------------------------
  insert a new or change an existing ladder contact
  in :pE - pointer to contact, if NULL - create a new contact
  out:-
-----------------------------------------------------------------------------*/
void CElemEditorView::PropFblk  (CElemFblk   *pE)
{
   CInsFbDlg     dlg;

   CGrEditorDoc* pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   ASSERT_VALID(pE);

   // Initialize dialog data
   dlg.m_strName = pE->GetInst();
   dlg.EnableInstance( pE->GetType()==ET_FB );

   if(dlg.DoModal() == IDOK)
   {
      // retrieve the dialog data

      if(pE->GetType()==ET_FB)
      {
         pE->SetInst(dlg.m_strName);
      }

      if(dlg.m_bSetDefaultSize)
      {
         switch(pE->GetType())
         {
         case ET_FB:
         case ET_PROCEDURE:
            SetInsertSize(IDS_GR_REG_PRC_SIZE, pE->GetSize());
            SetInsertSize(IDS_GR_REG_FBLKSIZE, pE->GetSize());
            break;

         case ET_FUNCTION:
            SetInsertSize(IDS_GR_REG_FUN_SIZE, pE->GetSize());
            break;
         
         default:
            ASSERT(0&&"unexpected element type");
         }
      }
      // invalidate old select rect ..
      UnselectAll();

      // center view ..
      MoveViewToCursor();
      UpdateCursor();
      // update the views
      pDoc->m_Action.StoreState(pDoc->GetElemList());  // op. ok -> store
      pDoc->SetModifiedFlag(pDoc->m_Action.StateChangedQ());
      pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateDrawElemHint, pE);
   }
}


/*func*------------------------------------------------------------------------
  insert a new or change an existing ladder contact
  in :pE - pointer to contact, if NULL - create a new contact
  out:-
-----------------------------------------------------------------------------*/
void   CElemEditorView::PropContact(CElemContact* pE)
{
	CStringList   nameList;
	CStringList   typeList;
   CInsCtDlg     dlg;
	LPCTSTR       psz;
   CElem        *pTE;
   POSITION      pos;
   CPoint        origin;
   CSize         size;
   CGrEditorDoc* pDoc = GetDocument();

   ASSERT_VALID(pDoc);
   ASSERT_VALID(pE);

   CString       strType[4]; 
   for (int i=0; i<4; i++)
   {
      strType[i].LoadString(IDS_GR_DLG_PROPLD_TYPE_CONTACT+i);
   }
   // fill list with existing contacts
   for( pos = pDoc->GetElemList()->GetHeadPosition(); pos != NULL; )
   {
      pTE = pDoc->GetElemList()->GetNext(pos);
      if( (pTE->GetType() == ET_VARIN)    || 
          (pTE->GetType() == ET_VAROUT)   || 
          (pTE->GetType() == ET_VARINOUT)) 
      {
         psz = ((CElemVar*)pTE)->GetName();
         if (psz && !nameList.Find( psz )) 
         { 
            nameList.AddTail( psz );
         }
      }
      if( (pTE->GetType() == ET_CONTACT)  ||
          (pTE->GetType() == ET_COIL) ) 
      {
         psz = ((CElemLdBase*)pTE)->GetName();
         if (psz && !nameList.Find( psz )) 
         { 
            nameList.AddTail( psz );
         }
      }
   }
   // Initialize dialog data
   dlg.m_pNameList = &nameList;

   typeList.AddTail(strType[LD_NORMAL]);
   typeList.AddTail(strType[LD_CLOSED]);
   typeList.AddTail(strType[LD_POSTRANS]);
   typeList.AddTail(strType[LD_NEGTRANS]);
   dlg.m_pTypeList = &typeList;
   // set data of variable (change mode)
   // default dlg data for insert mode has been set in constructor of dlg
   dlg.m_strName  = pE->GetName();
   if     ( pE->GetLDType() == LD_NORMAL   ) dlg.m_strType = strType[LD_NORMAL];
   else if( pE->GetLDType() == LD_CLOSED   ) dlg.m_strType = strType[LD_CLOSED];
   else if( pE->GetLDType() == LD_POSTRANS ) dlg.m_strType = strType[LD_POSTRANS];
   else if( pE->GetLDType() == LD_NEGTRANS ) dlg.m_strType = strType[LD_NEGTRANS];
   else {
      ASSERT(FALSE);
   }
   // Invoke the dialog box
   if (dlg.DoModal() == IDOK)
   {
      // invalidate old select rect ..
      UnselectAll();
      pDoc->m_Action.StoreState(pDoc->GetElemList());  // op. ok -> store
      pDoc->SetModifiedFlag(pDoc->m_Action.StateChangedQ());
      // retrieve the dialog data
      if     ( dlg.m_strType == strType[LD_NORMAL]   ) pE->SetLDType(LD_NORMAL);
      else if( dlg.m_strType == strType[LD_CLOSED]   ) pE->SetLDType(LD_CLOSED);
      else if( dlg.m_strType == strType[LD_POSTRANS] ) pE->SetLDType(LD_POSTRANS);
      else if( dlg.m_strType == strType[LD_NEGTRANS] ) pE->SetLDType(LD_NEGTRANS);
      
      pE->SetName(dlg.m_strName);
      if(dlg.m_bSetDefaultSize)
      {
         SetInsertSize(IDS_GR_REG_CTSIZE, pE->GetSize());
      }
      CheckPropEnd(pE);
      // center view ..
      MoveViewToCursor();
      UpdateCursor();
   }
}

/*func*------------------------------------------------------------------------
  insert a new or change an existing ladder coil
  in :pE - pointer to coil, if NULL - create a new coil
  out:-
-----------------------------------------------------------------------------*/
void   CElemEditorView::PropCoil   (CElemCoil   *pE)
{
	CStringList   nameList;
	CStringList   typeList;
   CInsClDlg     dlg;
	LPCTSTR       psz;
   CElem        *pTE;
   POSITION      pos;
   CPoint        origin;
   CSize         size;
   CGrEditorDoc* pDoc = GetDocument();

   ASSERT_VALID(pDoc);
   ASSERT_VALID(pE);

   CString      strType[6];
   for (int i=0; i<6; i++)
   {
      strType[i].LoadString(IDS_GR_DLG_PROPCOIL_TYPE_NORMAL+i);
   }
   // fill list with existing variables
   for( pos = pDoc->GetElemList()->GetHeadPosition(); pos != NULL; )
   {
      pTE = pDoc->GetElemList()->GetNext(pos);
      if( (pTE->GetType() == ET_VARIN)    || 
          (pTE->GetType() == ET_VAROUT)   || 
          (pTE->GetType() == ET_VARINOUT)) 
      {
         psz = ((CElemVar*)pTE)->GetName();
         if (psz && !nameList.Find( psz )) 
         { 
            nameList.AddTail( psz );
         }
      }
      if( (pTE->GetType() == ET_CONTACT)  ||
          (pTE->GetType() == ET_COIL) ) 
      {
         psz = ((CElemLdBase*)pTE)->GetName();
         if (psz && !nameList.Find( psz )) 
         { 
            nameList.AddTail( psz );
         }
      }
   }
   // Initialize dialog data
   dlg.m_pNameList = &nameList;

   typeList.AddTail(strType[LD_NORMAL]);
   typeList.AddTail(strType[LD_CLOSED]);
   typeList.AddTail(strType[LD_POSTRANS]);
   typeList.AddTail(strType[LD_NEGTRANS]);
   typeList.AddTail(strType[LD_SET]);
   typeList.AddTail(strType[LD_RESET]);
   dlg.m_pTypeList = &typeList;
   // set data of variable (change mode)
   // default dlg data for insert mode has been set in constructor of dlg
   dlg.m_strName  = pE->GetName();
   if     ( pE->GetLDType() == LD_NORMAL   ) dlg.m_strType = strType[LD_NORMAL  ];
   else if( pE->GetLDType() == LD_CLOSED   ) dlg.m_strType = strType[LD_CLOSED  ];
   else if( pE->GetLDType() == LD_POSTRANS ) dlg.m_strType = strType[LD_POSTRANS];
   else if( pE->GetLDType() == LD_NEGTRANS ) dlg.m_strType = strType[LD_NEGTRANS];
   else if( pE->GetLDType() == LD_SET      ) dlg.m_strType = strType[LD_SET     ];
   else if( pE->GetLDType() == LD_RESET    ) dlg.m_strType = strType[LD_RESET   ];
   else {
      ASSERT(FALSE);
   }
   // Invoke the dialog box
   if (dlg.DoModal() == IDOK)
   {
      // invalidate old select rect ..
      UnselectAll();
      pDoc->m_Action.StoreState(pDoc->GetElemList());  // op. ok -> store
      pDoc->SetModifiedFlag(pDoc->m_Action.StateChangedQ());
      // retrieve the dialog data
      if     ( dlg.m_strType == strType[LD_NORMAL  ] ) pE->SetLDType(LD_NORMAL);
      else if( dlg.m_strType == strType[LD_CLOSED  ] ) pE->SetLDType(LD_CLOSED);
      else if( dlg.m_strType == strType[LD_POSTRANS] ) pE->SetLDType(LD_POSTRANS);
      else if( dlg.m_strType == strType[LD_NEGTRANS] ) pE->SetLDType(LD_NEGTRANS);
      else if( dlg.m_strType == strType[LD_SET     ] ) pE->SetLDType(LD_SET);
      else if( dlg.m_strType == strType[LD_RESET   ] ) pE->SetLDType(LD_RESET);
      
      pE->SetName(dlg.m_strName);
      if(dlg.m_bSetDefaultSize)
      {
         SetInsertSize(IDS_GR_REG_CLSIZE,pE->GetSize());
      }
      CheckPropEnd(pE);
      // center view ..
      MoveViewToCursor();
      UpdateCursor();
   }
}

/*func*------------------------------------------------------------------------
  check the properties (result of the property dialog)
  in :pE - pointer to the element
  out:-
-----------------------------------------------------------------------------*/
void CElemEditorView::CheckPropEnd(CElem *pE)
{
   CPoint        p;
   CGrEditorDoc *pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   if (pDoc->GetElemList()->IsElemOverlapped(pE))
   {
      GetCursorPos(&p);
      if( AfxMessageBox( IDS_GR_ERR_INVALID_POS, MB_ICONSTOP | MB_YESNO ) == IDYES)
      {                                          // op. wrong, aborted
         pDoc->m_Action.LoadState(pDoc->GetElemList());
         pDoc->SetModifiedFlag(pDoc->m_Action.StateChangedQ());
         pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateWindowHint);
      }
      else
      {                                          // op. wrong, continue
         GrSetCapture();
         if( pE ) 
         {
            pE->SetFlag(EF_SELECT);
         }
         pDoc->SetEditMode( EM_MOVE );
         pDoc->GetElemList()->MoveFlaggedToTail(EF_SELECT);
         pDoc->GetElemList()->MoveFlaggedToTail(EF_RUBBLINE);
         pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateSelectionHint);
         m_BeforeOpPos = m_StartPos = m_CurPos;
      }
      SetCursorPos(p.x, p.y);
   }
   else
   {                                             // op. ok
      pDoc->m_Action.RemoveLastState();
      pDoc->m_Action.StoreState(pDoc->GetElemList());  // op. ok -> store
      pDoc->SetModifiedFlag(pDoc->m_Action.StateChangedQ());
      pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateDrawElemHint, pE);
   }
}

/*func*------------------------------------------------------------------------
  move cursor to pos and set cursor shape
  in :x  absolute pos in logical device units (pixel)
      y    - " -
  out:-
-----------------------------------------------------------------------------*/
void CElemEditorView::MovePos(int iX, int iY)
{
   CPoint p;

   // m_CurPos should be inside of valid range 
   p.x = max( 0, min( iX, MAX_XGRID ) );
   p.y = max( 0, min( iY, MAX_YGRID ) );
   SetCurPos (p);
   m_LogCurPos.x = m_CurPos.x * m_iZGrid;
   m_LogCurPos.y = m_CurPos.y * m_iZGrid;
}

/*func*------------------------------------------------------------------------
  move cursor relativ to actual pos and set cursor shape
  in :x  absolute pos in logical device units (pixel)
      y    - " -
  out:-
-----------------------------------------------------------------------------*/
void CElemEditorView::MoveRel(int iDx, int iDy)
{
   MovePos( m_CurPos.x + iDx, m_CurPos.y + iDy);
}

/*func*------------------------------------------------------------------------
  scroll the view relativ to actual pos and set cursor shape
  in :x  relativ offset in grid
      y    - " -
  out:-
-----------------------------------------------------------------------------*/
void CElemEditorView::ScrollRel(int iDx, int iDy, BOOL bEnableMousePos)
{
   CRect rect;
   CGrEditorDoc*pDoc = GetDocument();

   ASSERT(pDoc);

   CPoint p = DocToGrid( m_LastScrollPos );
   p.x += iDx;
   p.y += iDy;
   ScrollToPosition( GridToDoc( p ) ); 

   GetClientRect( &rect );
   DocToGrid(rect);

   if (iDx > 0 && m_CurPos.x < p.x              ) m_CurPos.x = p.x;
   if (iDx < 0 && m_CurPos.x > p.x+rect.Width() ) m_CurPos.x = p.x+rect.Width();
   if (iDy > 0 && m_CurPos.y < p.y              ) m_CurPos.y = p.y;
   if (iDy < 0 && m_CurPos.y > p.y+rect.Height()) m_CurPos.y = p.y+rect.Height();

   m_LogCurPos = GridToDoc(m_CurPos);

   if (pDoc)
	{
      if (pDoc->IsStandAloneDoc() || pDoc->IsInPlaceActive())
      {
         m_LastScrollPos = GetScrollPosition ();
      }

      switch ( pDoc->GetEditMode() )
      {
         case EM_MONITOR:

            if (iDx != 0)
            {
               GetClientRect( &rect );
               rect.bottom = rect.top + m_iDZGrid + 1;
               InvalidateRect(&rect);

               GetClientRect( &rect );
               rect.top = max( 0, (rect.bottom - m_iDZGrid - 1));
               InvalidateRect(&rect);
            }

            if (iDy != 0)
            {
               GetClientRect( &rect );
               rect.right = rect.left + m_iDZGrid + 1;
               InvalidateRect(&rect);

               GetClientRect( &rect );
               rect.left = max( 0 , (rect.right - m_iDZGrid - 1));
               InvalidateRect(&rect);
            }
            break;
      }
   }
}

/*func*------------------------------------------------------------------------
  move the view over cursor
  in :-
  out:-
-----------------------------------------------------------------------------*/
void CElemEditorView::MoveViewToCursor(BOOL bEnableMousePos)
{
   CRect  r;
   CPoint p, pd, pc, po;
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT(pDoc);
   int   hist=20; // pixel on screen

   // get actual view position
   po = m_LastScrollPos;      // origin of view
   p  = m_LogCurPos - po;     // cursor pos in view
   // check view size for scrollframes - border or scrollframes
   GetClientRect(&r);
   // search next valid pos in view 
   if     ( p.x <               hist) p.x =              hist; 
   else if( p.x >= r.Width ()-1-hist) p.x = r.Width ()-1-hist;
   if     ( p.y <               hist) p.y =              hist;
   else if( p.y >= r.Height()-1-hist) p.y = r.Height()-1-hist;
   // calc new view origin
   pd = m_LogCurPos - p;
   pd.x = max( 0, min( MAX_XGRID*m_iZGrid, pd.x ));
   pd.y = max( 0, min( MAX_YGRID*m_iZGrid, pd.y ));
   // is view moved ?
   if( (abs(pd.x-po.x) > hist/2) || (abs(pd.y-po.y) > hist/2) ) 
   {
      // move origin of view
      ScrollToPosition( pd ); 
      if (pDoc->IsStandAloneDoc() || pDoc->IsInPlaceActive())
      {
         m_LastScrollPos = GetScrollPosition ();
      } else {
         m_LastScrollPos = pd;
      }
      pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateWindowHint);
   }
   // move cursor in view (for keyboard handling)
   if( bEnableMousePos )
   {
      ClientToScreen( &p );
      GetCursorPos(&pc);
      if( pc != p )
      {
         SetCursorPos(p.x, p.y);
         // ATTENTION: OnMouseMove followed !!!
      }
   }
}

/*func*------------------------------------------------------------------------
  center the view over cursor 
  no change of cursor !
  in :p - new center point in grid coordinates
  out:-
-----------------------------------------------------------------------------*/
void CElemEditorView::CenterViewToCursor(CPoint p)
{
   CRect   r;
   CPoint  pd, po;
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   // get actual view position
   po = DocToGrid( m_LastScrollPos );  // origin of view
   // check view size for scrollframes - border or scrollframes
   GetClientRect(&r);
   ClientToGrid(r);
   // calc new view origin
   pd.x = p.x - r.Width()/2;
   pd.y = p.y - r.Height()/2;
   pd.x = max( 0, min( MAX_XGRID, pd.x ));
   pd.y = max( 0, min( MAX_YGRID, pd.y ));
   // is view moved ?
   if( pd != po ) 
   {
      // move origin of view
      ScrollToPosition( GridToDoc(pd) );
      m_LastScrollPos = GetScrollPosition ();

      pDoc->UpdateAllViews(NULL, CGrEditorDoc::UpdateWindowHint);
   }
}

/*func*------------------------------------------------------------------------
  set changed cursor
  in :-
  out:-
-----------------------------------------------------------------------------*/
void CElemEditorView::SetCursor(HCURSOR hCursor)
{
   if (hCursor != GetCursor())
   {
      ::SetCursor(hCursor);
   }
}

/*func*------------------------------------------------------------------------
  a) update the cursor shape
  b) at bOnlyShape == FALSE
     set cursor pos and m_LogCurPos in EM_RESIZE_DIR and EM_RESIZE ! for 
     keyboard handling,  ATTENTION: OnMouseMove followed!
  in :bEnableMousePos - set mouse pos or not (produce no circular refereces !!) 
      EMODE
      m_eResizeMode
  out:-
-----------------------------------------------------------------------------*/
void CElemEditorView::UpdateCursor(bool bEnableMousePos)
{
   CRect        r;
   CPoint       p;
   CElem       *pTE;
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);
   int   i;

	switch ( pDoc->GetEditMode() ) {
      case EM_INSCOL :
      case EM_INSCOL1:
      case EM_DELCOL :
      case EM_DELCOL1:
      case EM_INSROW :
      case EM_INSROW1:
      case EM_DELROW :
      case EM_DELROW1:
         i = pDoc->GetEditMode() - EM_INSCOL;
         if( (m_eCursorMode != CT_INSCOL + i) || 
             (GetCursor() != GDIHelper::GetCursor((HCURSOR_TYPE)(HC_INSCOL + i))) )
         {
            m_eCursorMode = (CURSOR_TYPE)(CT_INSCOL + i);
            SetCursor( GDIHelper::GetCursor((HCURSOR_TYPE)(HC_INSCOL + i)) );
         }
         break;

      case EM_LINE:
         if( (m_eCursorMode != CT_LINE) ||
             (GetCursor() != GDIHelper::GetCursor(HC_PENCUR)) )
         {
            m_eCursorMode = CT_LINE;
            SetCursor( GDIHelper::GetCursor(HC_PENCUR) );
         }
         break;

      case EM_LINE_START:
         if( (m_eCursorMode != CT_LINE_START) ||
             (GetCursor() != GDIHelper::GetCursor(HC_PENCUR1)) )
         {
            m_eCursorMode = CT_LINE_START;
            SetCursor( GDIHelper::GetCursor(HC_PENCUR1) );
         }
         break;

      case EM_RESIZE_DIR:
         if( (m_eCursorMode != CT_SIZESEL) ||
             (GetCursor() != GDIHelper::GetCursor(HC_SIZEALL)) )
         {
            m_eCursorMode = CT_SIZESEL;
            SetCursor( GDIHelper::GetCursor(HC_SIZEALL) );
            // move cursor to center of element
            if( bEnableMousePos &&
               ((pTE = pDoc->GetElemList()->SearchElem(m_LogCurPos, m_iZGrid)) != NULL))
            {
               pTE->GetRect(r, m_iZGrid);
               p.x = r.left + r.Width()/2;
               p.y = r.top  + r.Height()/2;
               m_LogCurPos = p;
               p = DocToClient(p);
               ClientToScreen(&p);
               SetCursorPos(p.x,p.y);
            }
         }
         break;

      case EM_MONITOR:
         if ( (m_eCursorMode == CT_INIT) ||
              (GetCursor() != GDIHelper::GetCursor(HC_ARROW)) )
         {
            SetCursor( GDIHelper::GetCursor(HC_ARROW) );
            m_eCursorMode = CT_NORMAL;
         }
         break;

      case EM_RESIZE:
         if( m_eCursorMode != CT_SIZESEL )
         {  // don't change shape in EM_RESIZE !
            break;
         }  // fall through .. !

      default:
          if(CheckReadonly())
          {
            SetCursor( GDIHelper::GetCursor(HC_ARROW) );
            m_eCursorMode = CT_NORMAL;
            break;
          }

         if( ( m_eCursorMode == CT_LINE ) ||
             ((m_eCursorMode >= CT_INSCOL) &&
              (m_eCursorMode <= CT_DELROW1)) ||
             // set default cursor shape at first
             ( m_eCursorMode == CT_INIT) ||
              (GetCursor() != GDIHelper::GetCursor(HC_ARROW)) )
         {
            SetCursor( GDIHelper::GetCursor(HC_ARROW) );
            m_eCursorMode = CT_NORMAL;
         }
         pTE = pDoc->GetElemList()->SearchElem(m_LogCurPos,m_iZGrid);
         if( pTE )
         {  // resize-frame corner(s) is only in selected state visible !!
            if(  pTE->ChkFlag(EF_SELECT) &&
                (pDoc->GetEditMode() != EM_RESIZE_START))
            {
               if       ( (m_eResizeMode == RS_E) || (m_eResizeMode == RS_W) )
               {
                  if( (m_eCursorMode != CT_SIZEWE) || 
                      (GetCursor() != GDIHelper::GetCursor(HC_SIZEWE)) )
                  {
                     SetCursor( GDIHelper::GetCursor(HC_SIZEWE) );
                     m_eCursorMode = CT_SIZEWE;
                  }
               } else if( (m_eResizeMode == RS_N) || (m_eResizeMode == RS_S) ) 
               {
                  if( (m_eCursorMode != CT_SIZENS) || 
                      (GetCursor() != GDIHelper::GetCursor(HC_SIZENS)) )
                  {
                     SetCursor( GDIHelper::GetCursor(HC_SIZENS) );
                     m_eCursorMode = CT_SIZENS;
                  }
               } else if( (m_eResizeMode == RS_NW) || (m_eResizeMode == RS_SE) ) 
               {
                  if( (m_eCursorMode != CT_SIZENWSE) || 
                      (GetCursor() != GDIHelper::GetCursor(HC_SIZENWSE)) )
                  {
                     SetCursor( GDIHelper::GetCursor(HC_SIZENWSE) );
                     m_eCursorMode = CT_SIZENWSE;
                  }
               } else if( (m_eResizeMode == RS_NE) || (m_eResizeMode == RS_SW) ) 
               {
                  if( (m_eCursorMode != CT_SIZENESW) || 
                      (GetCursor() != GDIHelper::GetCursor(HC_SIZENESW)) )
                  {
                     SetCursor( GDIHelper::GetCursor(HC_SIZENESW) );
                     m_eCursorMode = CT_SIZENESW;
                  }
               } else {
                  goto m_rect;
               }
            } 
         } else {
           m_rect:
            if( pTE ) pTE->GetRect(r, m_iZGrid);
            if( pTE && r.PtInRect( m_LogCurPos ) )
            {
               if( (m_eCursorMode != CT_ELEM) || 
                      (GetCursor() != GDIHelper::GetCursor(HC_CROSS)) )
               {
                  SetCursor( GDIHelper::GetCursor(HC_CROSS) );
                  m_eCursorMode = CT_ELEM;
               }
            } else {
               if( (m_eCursorMode != CT_NORMAL) || 
                      (GetCursor() != GDIHelper::GetCursor(HC_ARROW)) )
               {
                  SetCursor( GDIHelper::GetCursor(HC_ARROW) );
                  m_eCursorMode = CT_NORMAL;
               }
            }
         }
         break;
   }

   // for keyboard handling adjust cursor position at resize corner
	switch ( pDoc->GetEditMode() ) 
   {
      case EM_RESIZE:
         // move cursor to selected resize corner of element
         if( bEnableMousePos &&
             (pTE = pDoc->GetElemList()->SearchElem(m_LogCurPos,m_iZGrid)) != NULL )
         {
            pTE->GetResizeRect(m_eResizeMode, r, m_iZGrid);
            p.x = r.left + r.Width()/2;
            p.y = r.top  + r.Height()/2;
            if( p != m_OldCurPos )
            {
               m_OldCurPos = p;
               // update cursor pos ..
               m_LogCurPos = p;
               p = DocToClient(p);
               ClientToScreen( &p );
               SetCursorPos(p.x, p.y);
            }
         }
         break;

      default:
         // reset position ..
         m_OldCurPos = CPoint(0,0);
         break;
   }
}

int CElemEditorView::OnMouseActivate(CWnd *pDesktopWnd, UINT nHitTest, UINT message)
{
   m_bUpdateMousePos = TRUE;
   m_eCursorMode     = CT_INIT;
   UpdateCursor(false); 

   return CScrollView::OnMouseActivate(pDesktopWnd, nHitTest, message );
}

void CElemEditorView::CheckMousePos(CPoint point)
{
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   if( m_bUpdateMousePos )
   {
      m_bUpdateMousePos = FALSE;

      m_LogCurPos = ClientToDoc(point);
      SetCurPos (ClientToGrid(point));
   }
}

/*func*------------------------------------------------------------------------
  set zoomed grid and help variables
  in :iZGrid - set value
  out:-
-----------------------------------------------------------------------------*/
void CElemEditorView::SetZoomedGrid(int iZGrid)
{
   ASSERT(iZGrid > 0);

   if( iZGrid > 0 )
   {
      m_iZGrid  = iZGrid;
      m_iDZGrid = m_iZGrid*2;
      m_iHZGrid = m_iZGrid/2;
   }
}

/*func*------------------------------------------------------------------------
  adjust cursor position
  at insert and resize of SINGLE elements the cursor position is determined
  by element !
  in :newCurPos - requested pos
  out:adjusted position
-----------------------------------------------------------------------------*/
void CElemEditorView::SetCurPos (CPoint newCurPos)
{
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);

   m_CurPos = newCurPos;

   switch ( pDoc->GetEditMode() ) 
   {
      case EM_RESIZE:
      case EM_INSERT:
         m_CurPos.x = (m_CurPos.x & 0xFFFFFFFE) + (m_BeforeOpPos.x & 0x01);
         m_CurPos.y = (m_CurPos.y & 0xFFFFFFFE) + (m_BeforeOpPos.y & 0x01);
         break;

      default:
         m_CurPos.x &= 0xFFFFFFFE;
         m_CurPos.y &= 0xFFFFFFFE;
         break;
   }
}

/*func*------------------------------------------------------------------------
  update statusbar
  in :strMsg - message for display in statusbar (if access enable)
  out:-
-----------------------------------------------------------------------------*/
void CElemEditorView::Message(PSTR strMsg)
{
   CMainFrame* pFrame = (CMainFrame*) AfxGetMainWnd();
   if( pFrame )
   {
      CStatusBar* pStatusBar = (CStatusBar*) pFrame->GetDescendantWindow(AFX_IDW_STATUS_BAR);
      if( pStatusBar )
      {
         pStatusBar->SetPaneText(pStatusBar->CommandToIndex(ID_SEPARATOR), strMsg );
      }
   }
}


/*func*------------------------------------------------------------------------
  Set mouse capture for GR, if it isn't yet done
  in :-
  out:-
-----------------------------------------------------------------------------*/
void CElemEditorView::GrSetCapture()
{
   if( !m_bIsCaptured )
   {
      SetCapture();
      m_bIsCaptured = TRUE;
   }
}

/*func*------------------------------------------------------------------------
  Release mouse capture for GR, if it isn't yet done
  in :-
  out:-
-----------------------------------------------------------------------------*/
void CElemEditorView::GrReleaseCapture()
{
   if( m_bIsCaptured )
   {
      ReleaseCapture();
      m_bIsCaptured = FALSE;
   }
}
/*func*------------------------------------------------------------------------
  update statusbar
  in :- 
  out:-
-----------------------------------------------------------------------------*/
void CElemEditorView::UpdateStatus()
{
   CGrEditorDoc*pDoc = GetDocument();
   ASSERT_VALID(pDoc);

// -------------------- TEST --- TEST --- TEST ------------------
   switch ( pDoc->GetEditMode() ) 
   {
      case EM_NORMAL      : Message(_T("MODE: normal")); break;
      case EM_LINE        : Message(_T("MODE: edit line")); break;
      case EM_LINE_START  : Message(_T("MODE: start edit line")); break;
      case EM_SELECT      : Message(_T("MODE: select")); break;
      case EM_MOVE        : Message(_T("MODE: move")); break;
      case EM_DRAG_MOVE   : Message(_T("MODE: drag or move")); break;
      case EM_INSERT      : Message(_T("MODE: insert")); break;
      case EM_RESIZE      : Message(_T("MODE: resize")); break;
      case EM_RESIZE_START: Message(_T("MODE: resize start (select element ..)")); break;
      case EM_RESIZE_DIR  : Message(_T("MODE: resize dir (select direction ..)")); break;
      case EM_EDITPROP    : Message(_T("MODE: edit property"));            break;
      case EM_INSCOL      : Message(_T("MODE: insert column (enabled)"));  break;
      case EM_INSCOL1     : Message(_T("MODE: insert column (disabled)")); break;
      case EM_DELCOL      : Message(_T("MODE: delete column (enabled)"));  break;
      case EM_DELCOL1     : Message(_T("MODE: delete column (disabled)")); break;
      case EM_INSROW      : Message(_T("MODE: insert row (enabled)"));     break;
      case EM_INSROW1     : Message(_T("MODE: insert row (disabled)"));    break;
      case EM_DELROW      : Message(_T("MODE: delete row (enabled)"));     break;
      case EM_DELROW1     : Message(_T("MODE: delete row (disabled)"));    break;

      case EM_MONITOR     : Message(_T("MODE: monitor")); break;

      default             : Message(_T("MODE: ???")); break;
   }
// -------------------- TEST --- TEST --- TEST ------------------
}

void CElemEditorView::PropLine(CElemLine *pE)
{
   CPropLineDlg Dlg;

   if(pE->ChkFlag(EF_RUBBLINE))
      return;

   if(Dlg.DoModal()==IDOK)
   {
      if(Dlg.m_bSetDefaultSize)
      {
         CSize size = pE->GetSize();
         size.cx = abs(size.cx);
         size.cy = abs(size.cy);

         CSize PrevSize = GetInsertSize(IDS_GR_REG_LINESIZE);
         if(size.cx == 0)
         {
            size.cx = PrevSize.cx;
         }else if(size.cy == 0)
         {
            size.cy = PrevSize.cy;
         }
         SetInsertSize(IDS_GR_REG_LINESIZE, size);
      }
   }
}

void CElemEditorView::PropPower(CElemPower *pE)
{
   CPropPRail Dlg;

   if(pE->ChkFlag(EF_RUBBLINE))
      return;

   if(Dlg.DoModal()==IDOK)
   {
      if(Dlg.m_bSetDefaultSize)
      {
         SetInsertSize(IDS_GR_REG_PRAILSIZE, pE->GetSize());
      }
   }
}
