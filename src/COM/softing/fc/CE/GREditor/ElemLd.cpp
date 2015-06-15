
/*doc*=======================================================================*/
/*inc*=======================================================================*/

#include "stdafx.h"

#include "GrEditor.h"
#include "GrEditorBase.h"
#include "GrEditorDoc.h"
#include "GrEditorView.h"
#include "GrEditorUtil.h"
#include "ElemLd.h"
#include "PosArray.h"

/*const*=====================================================================*/
/*macro*=====================================================================*/
/*glovar*====================================================================*/
/*type*======================================================================*/
/*locvar*====================================================================*/

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/*proto*=====================================================================*/
/*func*======================================================================*/

/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
CElemPower::CElemPower()
{ 
   CSize minSize(0,4);
   CSize maxSize(0,MAX_YGRID);

   SetMinSize (minSize);
   SetMaxSize (maxSize);

   SetType(ET_POWERRAIL);
}

/*func*------------------------------------------------------------------------
  create a power rail
  in :Origin  - insertion point 
      Size    - startup size of element 
  out:pointer to new element
-----------------------------------------------------------------------------*/
CElemPower::CElemPower(CPoint Origin, CSize Size)
{
   CPin  *pPin;
   CSize  minSize(0,4);
   CSize  maxSize(0,MAX_YGRID);

   SetType(ET_POWERRAIL);

   SetMinSize (minSize);
   SetMaxSize (maxSize);

   SetOrigin(Origin);
   SetSize  (Size);

   for( int i=0; i<=Size.cy; i+=2)
   {
      pPin = new CPin(_T(""), _T(""), _T("ANY"), 
         Origin + CPoint(0,i), CSize(0,0), PF_INOUT);
      if( pPin ) m_Con.Add(pPin);
   }
}

/*func*------------------------------------------------------------------------
  destructor
-----------------------------------------------------------------------------*/
CElemPower::~CElemPower()
{
}

/*func*------------------------------------------------------------------------
  draw the power rail
  in :pDC   - pointer to device context
      pView - pointer to document (data)
      iGrid - actual display resolution
  out:-
-----------------------------------------------------------------------------*/
void CElemPower::Draw(CDC * pDC, CGrEditorView * pView, int iGrid)
{
   CRect    r;
   CSize    size;
   CPin    *pPin;
   CString  str;
   int      i, iPinNr;
   PEN_TYPE PenType;
   int      x, y1, y2;
   bool     bScreen = !pDC->IsPrinting();

   if (!pView)
   {
      return;
   }
   ASSERT(GetType()  == ET_POWERRAIL);
   // prepare drawing
   if     ( ChkFlag(EF_COMMENT) ) PenType = PE_COMMENT;
   else   /* normal */            PenType = PE_NORMAL;

   pDC->SelectObject(GDIHelper::GetPen  (bScreen, PenType ));
   if (pView->IsManualMode())
   {
      pDC->SelectObject(GDIHelper::GetBrush(bScreen, BT_BACKGD_SFC));
   } else {
      pDC->SelectObject(GDIHelper::GetBrush(bScreen, BT_BACKGD));
   }

   // draw the connect point
   DrawPins(pDC, iGrid);

   // draw the power rail body
   if(m_Con.GetSize() > 0)
   {
      iPinNr = 0;
      if( (pPin = m_Con.GetAt(iPinNr)) != NULL )
      {
         x  = (m_Origin.x            )*iGrid;
         y1 = (m_Origin.y            )*iGrid;
         y2 = (m_Origin.y + m_Size.cy)*iGrid;
         for( i=0; i<2; i++) 
         { 
            pDC->MoveTo(x+i,y1);
            pDC->LineTo(x+i,y2);
         }
         // draw element icon 
//       DrawIcon( pDC, pView, m_Origin, IDB_ICONPOWER, iGrid);
      }
   }

   if( ChkFlag(EF_EMPHASIZE) ) 
   {
      GetRect(r, iGrid);
      pDC->SelectObject(GDIHelper::GetPen(bScreen, PE_ERROR));
      pDC->MoveTo(r.left,  r.top);
      pDC->LineTo(r.right, r.top);
      pDC->LineTo(r.right, r.bottom);
      pDC->LineTo(r.left,  r.bottom);
      pDC->LineTo(r.left,  r.top);
   }
}

/*func*------------------------------------------------------------------------
  draw the resize frame for the divergence
  in :pDC   - pointer to device context
      pView - pointer to document
      iGrid - actual display resolution
  out:-
-----------------------------------------------------------------------------*/
void CElemPower::DrawSelectFrame(CDC * pDC, CGrEditorView * pView, int iGrid, 
                                 bool bWithResizePoints)
{
   int      iX1, iX2, iY1, iY2;
   PEN_TYPE PenType;
   CRect    r;
   bool     bScreen = !pDC->IsPrinting();

   if (!pView)
   {
      return;
   }
   if( ChkFlag(EF_SELECT) )
   {
      // draw frame 
      pDC->SetBkMode(TRANSPARENT);
      pDC->SelectObject(GDIHelper::GetPen(bScreen, PE_SELECT));

      GetRect(r, iGrid); 
      iX1 = r.left;  
      iY1 = r.top;
      iX2 = r.right; 
      iY2 = r.bottom;

      pDC->MoveTo( iX1, iY1);
      pDC->LineTo( iX2, iY2);

      if (bWithResizePoints)
      {
         // draw resize points
         if     ( ChkFlag(EF_COMMENT  ) ) PenType = PE_COMMENT;
         else if( ChkFlag(EF_EMPHASIZE) ) PenType = PE_ERROR;
         else   /* normal */                 PenType = PE_NORMAL;

         pDC->SelectObject(GDIHelper::GetPen  (bScreen, PenType  ));
         pDC->SelectObject(GDIHelper::GetBrush(bScreen, BT_SELECT)); 

         pDC->Rectangle( GetResizeRect(RS_N , r, iGrid) );
         pDC->Rectangle( GetResizeRect(RS_S , r, iGrid) );
      }
   } 
}

/*func*------------------------------------------------------------------------
  test if p at resize rect in element
  in :p     - point for test in logical coordinates at sheet
      iGrid - actual zoom
  out:resize type at elem
      RS_LAST - no resize corner 
-----------------------------------------------------------------------------*/
RS_TYPE CElemPower::GetResizeType(CPoint p, int iGrid)
{
   CRect r;

   if( GetResizeRect( RS_N , r, iGrid )->PtInRect( p ) ) return RS_N;
   if( GetResizeRect( RS_S , r, iGrid )->PtInRect( p ) ) return RS_S;

   return RS_LAST;
}

/*func*------------------------------------------------------------------------
  test if p at resize rect in element
  in :p     - point for test in logical coordinates at sheet
      iGrid - actual zoom
  out:TRUE  - yes on resize rect
      FALSE - not in resize rect
-----------------------------------------------------------------------------*/
bool CElemPower::IsResizeRect(CPoint p, int iGrid)
{
   CRect r;

   if( GetResizeRect( RS_N , r, iGrid )->PtInRect( p ) ||
       GetResizeRect( RS_S , r, iGrid )->PtInRect( p ) )
   {
      return true;
   } else {
      return false;
   }
}

/*func*------------------------------------------------------------------------
  resize and/or move element with connected pins
  in :dx - delta x for m_Origin
      dy - delta y for m_Origin
      sx - delta x for m_Size
      sy - delta y for m_Size
  out:-
-----------------------------------------------------------------------------*/
bool CElemPower::Resize(int dx, int dy, int sx, int sy)
{
   CPin  *pPin;
   int    i, iNewSY;
   bool   bRes;

   if( (m_MinSize.cx <= m_Size.cx + sx) &&
       (m_MinSize.cy <= m_Size.cy + sy) &&
       (m_MaxSize.cx >= m_Size.cx + sx) &&
       (m_MaxSize.cy >= m_Size.cy + sy) &&
       ((int)((m_Origin.x + dx) & 0x00000001) == m_OriginOffs.x) &&
       ((int)((m_Origin.y + dy) & 0x00000001) == m_OriginOffs.y) )
   {  
      bRes = true;
   } else {
      bRes = false;
   }

   if( bRes )
   {
      ASSERT(m_pPinTable);
      if (m_pPinTable)
      {
         iNewSY = min( m_MaxSize.cy, max(m_MinSize.cy, m_Size.cy + sy));
         // adjust the connect points
         if( iNewSY/2+1 > m_Con.GetSize() )
         {  // insert new connect points
            for( i=m_Con.GetSize()*2; i<=iNewSY; i+=2)
            {
               pPin = new CPin(_T(""), _T(""), _T("ANY"), 
                  m_Origin + CPoint(0,i), CSize(0,0), PF_INOUT);
               if( pPin ) 
               {
                  m_Con.Add(pPin);
                  pPin->m_Entry = m_pPinTable->InsertPin(this, pPin->m_Pos);
                  ASSERT(pPin->m_Entry);
               }
            }
         } else if( iNewSY/2+1 < m_Con.GetSize() )
         {  // delete existing connect points
            for( i=0; i<m_Con.GetSize(); i++)
            {
               pPin = m_Con.GetAt(i);
               ASSERT_VALID(pPin);
               if( pPin && (pPin->m_Pos.y - m_Origin.y > iNewSY) )
               { 
                  ASSERT(pPin->m_Entry);
                  m_pPinTable->DeletePin(this, pPin->m_Entry, CPoint(pPin->m_Pos.x, pPin->m_Pos.y - 2));
                  m_Con.RemoveAt(i);  
                  delete pPin; 
                  i--;  // next pin is now at the same index !
               }
            }
         }
         // resize/move ..
         bRes = CElem::Resize(dx,dy,sx,sy);
      } 
   }

   return bRes;
}

/*func*------------------------------------------------------------------------
  serialize the element to/from stream
  in :ar - the archive
  out:-
-----------------------------------------------------------------------------*/
void CElemPower::Serialize(CArchive & ar)
{
   CElem::Serialize(ar);
}

/*func*------------------------------------------------------------------------
  serialize the element to/from XML stream
  in :ar - the archive
  out:-
-----------------------------------------------------------------------------*/
void CElemPower::SerializeXML(CXMLArchive &ar)
{
   CPin  *pPin;
   int    i, iNewSY;

   if(ar.IsStoring())
   {
      // all CXMLArchive fire HRESULT exceptions, which must be cought!!!
      try {            // name and create node here !!
         CElem::SerializeXML(ar);  // origin, size, selected, comment
         // we save no pin list !!
         // pins will be reconstructed at loading.

//       STATELIST
//       IMAGELIST
      }        
      catch (HRESULT hr)
      {
         TRACE(_T("Cannot save to XML; hr=0x%08X\n"), hr);
         throw hr;   // fire upstairs...        
      }    
   } else {
      try {   
         CElem::SerializeXML(ar);  // origin, size, selected, comment

         // no we construct the pin list ..
         iNewSY = min( m_MaxSize.cy, max(m_MinSize.cy, m_Size.cy));

         for( i=m_Con.GetSize()*2; i<=iNewSY; i+=2)
         {
            pPin = new CPin(_T(""), _T(""), _T("ANY"), 
               m_Origin + CPoint(0,i), CSize(0,0), PF_INOUT);
            if( pPin ) m_Con.Add(pPin);
         }

//       STATELIST
//       IMAGELIST
      }
      catch (HRESULT hr)
      {
         TRACE(_T("Cannot load from XML; hr=0x%08X\n"), hr);
         throw hr;   // fire upstairs...
      }   
   }
}

/*func*------------------------------------------------------------------------
  check Possibility of insertion/removal of lines/columns
  in :   Spacing point, spacig amount (unused: *pinArray, bLabelClip )
  out:-
-----------------------------------------------------------------------------*/
bool CElemPower::IsSpaceable(CPoint At, CPoint Delta, bool bLabelClip)
{
   int i1, i2;
   bool bOK = true;

   if(Delta.y<0)  // negative y-spacing
   {
      if(GetSize().cy>0)
      {
         i1 = GetOrigin().y;
         i2 = i1 + GetSize().cy;
      }else
      {
         i2 = GetOrigin().y;
         i1 = i2 + GetSize().cy;
      }

      if(At.y>=i1 && At.y < i2)
      {
         CPoint P1( GetOrigin().x, At.y);    // spacing pin position
         CPoint P2( GetOrigin().x, At.y+2);  // next pin right of the spacing pin position
         CPin *pP1 = SearchPin(P1,1);
         if(pP1->GetConnects()>1)
         {
            CPin* pP2 = SearchPin(P2,1);
            bOK = (pP2->GetConnects()==1);
         }
      }
   }else
   {
      if(Delta.x<0)
      {
         bOK = At.x != GetOrigin().x;
      }
   }
   return bOK;
}

/*func*------------------------------------------------------------------------
  
  in :
  out:-
-----------------------------------------------------------------------------*/
bool CElemPower::Spacing( CPoint Point, CPoint Delta, CPosArray* paPin, bool bLabelClip)
{
   int  i1;                // min element extension in raster coords
   int  i2;                // max element extension in raster coords
   bool bDone = false;     // true if any movement done
   bool bRes  = true;
   int  d;

   ASSERT( Delta.x * Delta.y == 0); // either x or y direction (not both)

   if( (d = Delta.x) > 0 )
   {
      i1 = min( m_Origin.x , m_Origin.x + m_Size.cx );
      i2 = max( m_Origin.x , m_Origin.x + m_Size.cx );

      if( Point.x > i1 )                                    // don't move the entire element
      {
         if( Point.x <= i2)                                 // point inside or at right border
         {
            if( m_Size.cx >= 0 )                            // normal  orientation
            {
               bRes &= Resize( 0,0, d,0);
            }else                                           // reverse orientation
            {
               bRes &= Resize(d,0, -d,0);
            }

            bDone = true;
         }
      }else
      {                                                     // element is strictly right
         bRes &= Resize(d,0,0,0);                           //    shift it
         bDone = true;
      }
   }

   if( (d = Delta.y) > 0 )
   {
      i1 = min( m_Origin.y , m_Origin.y + m_Size.cy );
      i2 = max( m_Origin.y , m_Origin.y + m_Size.cy );

      if( Point.y > i1 )                                    // don't move the entire element
      {
         if( Point.y <= i2)                                 // point inside or at right border
         {
            if( m_Size.cy >= 0 )                            // normal  orientation
            {
               bRes &= Resize( 0,0, 0, d);
            }else                                           // reverse orientation
            {
               bRes &= Resize( 0,d, 0,-d);
            }

            bDone = true;
         }
      }else
      {                                                     // element is strictly right
         bRes &= Resize( 0,d, 0,0);                         //    shift it
         bDone = true;
      }
   }

   if( (d = Delta.x) < 0)
   {
      bDone = CElem::Spacing(Point,Delta,paPin,bLabelClip);
   }

   if( (d = Delta.y) < 0 )
   {
      i1 = min(m_Origin.y, m_Origin.y + m_Size.cy);
      i2 = max(m_Origin.y, m_Origin.y + m_Size.cy);

      if( Point.y < i1)                                     // element is strictly right
      {
         bRes &= Resize( 0,d,0,0);                          // move it
         bDone = true;
      }else
      {
         if( (Point.y >= i1) && (Point.y < i2) )            // point is inside
         {                                                  // shrink it
            if( m_Size.cy >= 0 )                            // normal  orientation
            {
               bRes &= Resize( 0, 0, 0, d);
            }else                                           // reverse orientation
            {
               bRes &= Resize( 0, d, 0,-d);
            }
            bDone = true;
         }
      }
   }

//   ASSERT(bRes);   // bRes may be false if minimum size is reached!
   return bDone;
}

bool  CElemPower::CheckDropIn( CElem* pE)        // requires a normalized line to check against
{
   ASSERT(pE != NULL);

   if((pE != NULL)                  &&
      (pE->GetType() == ET_LINE)    &&
      (pE->GetSize().cy == 0)       &&
      (!pE->ChkFlag(EF_RUBBLINE)))
   {
      return true;
   }
   return false;
}

IMPLEMENT_SERIAL(CElemPower, CElem, SERIAL_VERSION)


/*func*------------------------------------------------------------------------
  description:
  in :
  out:
-----------------------------------------------------------------------------*/
CElemLdBase::CElemLdBase()
{ 
   CSize size(6,4);

   m_OriginOffs = CPoint(0,1);
   SetMinSize(size);
   m_MaxSize.cy = 4;
   SetName(_T(""));
}

/*func*------------------------------------------------------------------------
  create a ladder contact
  in :Origin  - insertion point 
      Size    - startup size of element 
  out:pointer to new element
-----------------------------------------------------------------------------*/
CElemLdBase::CElemLdBase(CPoint Origin, CSize Size, LPCTSTR pszName)
{
   CSize size(6,4);

   m_OriginOffs = CPoint(0,1);
   SetMinSize(size);
   m_MaxSize.cy = 4;

   SetOrigin(Origin);
   SetSize  (Size);
   SetName  (pszName);
}

void CElemLdBase::CreatePins(void)
{
   CPin     *pPin;

   pPin = new CPin(_T("_LdIn_"), _T(""), _T("ANY"), 
      GetOrigin() + CPoint(0,3), CSize(0,0), PF_IN);
   if( pPin ) m_Con.Add(pPin);

   pPin = new CPin(_T("_LdOut_"), _T(""), _T("ANY"), 
      GetOrigin() + CPoint(m_Size.cx,3), CSize(0,0), PF_OUT);
   if( pPin ) m_Con.Add(pPin);
}

/*func*------------------------------------------------------------------------
  destructor
-----------------------------------------------------------------------------*/
CElemLdBase::~CElemLdBase()
{
}

/*func*------------------------------------------------------------------------
  draw the ladder element
  in :pDC   - pointer to device context
      pView - pointer to document (data)
      iGrid - actual display resolution
  out:-
-----------------------------------------------------------------------------*/
void CElemLdBase::Draw(CDC * pDC, CGrEditorView * pView, int iGrid)
{
   int      x1, x2, y1, y2;
   CRect    r;
   CSize    size;
   CString  str;
   PEN_TYPE PenType;
   bool     bScreen = !pDC->IsPrinting();
   CPoint   ps[10], p;
   COLORREF OldTextCol;
   bool     bActive;

   if (!pView)
   {
      return;
   }
   ASSERT((GetType() == ET_CONTACT) || 
          (GetType() == ET_COIL));
   
   // prepare drawing
   if     ( ChkFlag(EF_COMMENT) ) PenType = PE_COMMENT;
   else   /* normal */            PenType = PE_NORMAL;

   pDC->SelectObject(GDIHelper::GetPen  (bScreen, PenType  ));

   GetTextRect(0, r, 1);
   p = r.CenterPoint();
   bActive = pView->IsLadderActive(p);
 
   if (pView->IsManualMode())
   {
      pDC->SelectObject(GDIHelper::GetBrush(bScreen, BT_BACKGD_SFC));
   } else {
      pDC->SelectObject(GDIHelper::GetBrush(bScreen, BT_BACKGD));
   }
   // draw the connect point
   DrawPins(pDC, iGrid);
   // draw element body
   switch (GetType())
   {
      case ET_CONTACT:
         // draw  -||-
         y1 = (m_Origin.y +               3)*iGrid;
         x1 =  m_Origin.x                   *iGrid;
         x2 = (m_Origin.x + m_Size.cx/2 - 1)*iGrid;
         pDC->MoveTo( x1, y1); pDC->LineTo( x2, y1);
         x1 = (m_Origin.x + m_Size.cx/2 + 1)*iGrid;
         x2 = (m_Origin.x + m_Size.cx      )*iGrid;
         pDC->MoveTo( x1, y1); pDC->LineTo( x2, y1);

         x1 = (m_Origin.x + m_Size.cx/2 - 1)*iGrid;
         y1 = (m_Origin.y +               2)*iGrid;
         y2 = (m_Origin.y +               4)*iGrid;
         pDC->MoveTo( x1, y1); pDC->LineTo( x1, y2);
         x1 = (m_Origin.x + m_Size.cx/2 + 1)*iGrid;
         pDC->MoveTo( x1, y1); pDC->LineTo( x1, y2);
         break;

      case ET_COIL:
         // draw  -()-
         y1 = (m_Origin.y +               3)*iGrid;
         x1 =  m_Origin.x                   *iGrid;
         x2 = (m_Origin.x + m_Size.cx/2 - 1)*iGrid;
         pDC->MoveTo( x1, y1); pDC->LineTo( x2, y1);

         pDC->Ellipse((m_Origin.x + m_Size.cx/2 - 1)*iGrid, (m_Origin.y + 2)*iGrid,
                      (m_Origin.x + m_Size.cx/2 + 1)*iGrid, (m_Origin.y + 4)*iGrid);
         r.SetRect((m_Origin.x + m_Size.cx/2)*iGrid - iGrid/2, (m_Origin.y + 2)*iGrid,
                   (m_Origin.x + m_Size.cx/2)*iGrid + iGrid/2, (m_Origin.y + 4)*iGrid);
         if (pView->IsManualMode())
         {
            pDC->FillRect(r, GDIHelper::GetBrush(bScreen, BT_BACKGD_SFC));
         } else {
            pDC->FillRect(r, GDIHelper::GetBrush(bScreen, BT_BACKGD));
         }

         x1 = (m_Origin.x + m_Size.cx/2 + 1)*iGrid;
         x2 = (m_Origin.x + m_Size.cx      )*iGrid;
         pDC->MoveTo( x1, y1); pDC->LineTo( x2, y1);

         break;

      default: ASSERT(0); break;
   }
   // draw normal, closed, inverted, set, reset, positive, negative
   switch (m_Type)
   {
      case LD_NORMAL  : str = " "; goto m_type;
//    case LD_CLOSED  : 
      case LD_NEGATIVE: str = "/"; goto m_type;
      case LD_POSTRANS: str = "P"; goto m_type;
      case LD_NEGTRANS: str = "N"; goto m_type;
      case LD_SET     : str = "S"; goto m_type;
      case LD_RESET   : str = "R"; goto m_type;

        m_type:
         size = pDC->GetTextExtent(str);
         OldTextCol = pDC->SetTextColor(GDIHelper::GetColor(bScreen, GetColorType()));
         pDC->SetBkMode( TRANSPARENT );
         pDC->SetTextAlign( TA_CENTER | TA_TOP );
         pDC->TextOut( (m_Origin.x + m_Size.cx/2)*iGrid, 
                       (m_Origin.y + 2)*iGrid + (2*iGrid - size.cy)/2, str);
         pDC->SetTextColor( OldTextCol );
         break;
   }
   // draw text
   GetTextRect(-1, r, iGrid);
   r.right++;  // one pixel more ..

   if (bActive)
   {
      pDC->FillRect(r, GDIHelper::GetBrush(bScreen, BT_LADDBK));
   } else {
      if (pView->IsManualMode())
      {
         pDC->FillRect(r, GDIHelper::GetBrush(bScreen, BT_BACKGD_SFC));
      } else {
         pDC->FillRect(r, GDIHelper::GetBrush(bScreen, BT_ELEMBK));
      }
   }
/*
   // draw element icon 
   if( GetType() == ET_CONTACT ) 
   {
      p = m_Origin + CPoint(m_Size.cx-2,0); // icon pos
      r.right -= 2*iGrid;                   // adjust text field
   } else {  // ET_COIL
      p = m_Origin;                         // icon pos
      r.left  += 2*iGrid;                   // adjust text field
   }
   if      (GetType() == ET_CONTACT)
   {
      DrawIcon( pDC, pView, p, IDB_GR_ICONCONTACT, iGrid);
   } 
   else if (GetType() == ET_COIL)
   {
      DrawIcon( pDC, pView, p, IDB_GR_ICONCOIL, iGrid);
   }
*/
   // draw element name
   if((pView->m_SearchDat.m_FindCur == m_Origin) && (pView->m_SearchDat.m_iFindElem == -1))
   {
      m_Name.Draw( pDC, r, iGrid, GetColorType(), &pView->m_SearchDat, 0);
   }else
   {
      m_Name.Draw( pDC, r, iGrid, GetColorType(), NULL, 0);
   }

   if( ChkFlag(EF_EMPHASIZE) ) 
   {
      GetRect(r, iGrid);
      pDC->SelectObject(GDIHelper::GetPen(bScreen, PE_ERROR));
      pDC->MoveTo(r.left,  r.top);
      pDC->LineTo(r.right, r.top);
      pDC->LineTo(r.right, r.bottom);
      pDC->LineTo(r.left,  r.bottom);
      pDC->LineTo(r.left,  r.top);
   }
}

/*func*------------------------------------------------------------------------
  resize element to optimal width for text length
  in :-
  out:dorigin - delta origin  (necessary for reset size and position if 
      dsize   - delta size     overlap-check fails)
      true  - element is resized
      false - element is unchanged
-----------------------------------------------------------------------------*/
bool CElemLdBase::SetAutoSize(CPoint& dorigin, CSize& dsize) 
{ 
   TPinEntry *pPE[2];
   CPoint     p;
   CPin      *pPin;
   bool       bEastResize;
   int        i, j;

   ASSERT(m_Con.GetSize() == 2);
   ASSERT(m_pPinTable);

   if (!m_pPinTable) 
      return false;

   for (i=0; i<2; i++)
   {
      pPin = m_Con.GetAt(i);
      ASSERT(pPin);
      if (pPin)
      {
         if (pPin->m_Pos.x == m_Origin.x) j = 0; else j = 1; // left - right
         if (!m_pPinTable->Lookup(pPin->m_Pos, pPE[j]))
         {
            ASSERT(0);
            return false;
         }
      }
   }
   // first test: right is empty ..
   if (pPE[1]->m_iSelected + pPE[1]->m_iDeSelected == 1)
   {
      bEastResize = false; goto m_resize;
   }
   // second test: left is empty ..
   if (pPE[0]->m_iSelected + pPE[0]->m_iDeSelected == 1)
   {
      bEastResize = true; goto m_resize;
   }
   // third test: ..

   bEastResize = false; 

m_resize:
   i = ((PIN_LENGTH + CText::GetIdentLen((CString&)m_Name) + 1) & 0xFFFFFFFE);
   if ( i < m_MinSize.cx ) i = m_MinSize.cx;
   if ( i > m_MaxSize.cx ) i = m_MaxSize.cx;
   dsize.cx  = i - m_Size.cx;
   dsize.cy  = 0;
   if (((i / 2) & 1) == 0)
      dsize.cx += 2; // adjust ladder element for row alignment !
   dorigin.x = bEastResize ? -dsize.cx : 0; 
   dorigin.y = 0;
   if (dsize.cx != 0)
   {
      Resize(dorigin.x, dorigin.y, dsize.cx, dsize.cy);
      return true;
   } else {
      return false;
   }
}

/*func*------------------------------------------------------------------------
  get the body rect without pin range for select operations
  in :r        - place holder for rect
      iGrid    - actual zoom
  out:-
-----------------------------------------------------------------------------*/
void CElemLdBase::GetBodyRect(CRect &r, int iGrid)
{
   ASSERT(iGrid > 0);

   if( iGrid > 0 )
   {
      GetRect(r, iGrid);
      r.left  += iGrid*PIN_LENGTH; 
      r.right -= iGrid*PIN_LENGTH;
   }

}

/*func*------------------------------------------------------------------------
  get the text space in elem
  in :rect - placeholder for result
  out:rect - filled with text grid coords
-----------------------------------------------------------------------------*/
void CElemLdBase::GetTextRect(int iTxtIdx, CRect &rect, int iGrid)
{ 
   rect.SetRect((m_Origin.x             + 1)*iGrid,
                (m_Origin.y                )*iGrid,
                (m_Origin.x + m_Size.cx - 1)*iGrid,
                (m_Origin.y + 2            )*iGrid);
}

/*func*------------------------------------------------------------------------
  find text in element, search from given position for/backward with matching
  lower of upper cases
  in :iElemNr  - neg. numbers are reserved for inherit classes,
                 pos. numbers are the pins in m_con
      iPos     - position in strname in pin
      lpszFind - search text
      bNext    - search direction
      bCase    - flag for matching cases
  out:TRUE  - string found (iElemNr and iPos will be updated)
      FALSE - nothing found
-----------------------------------------------------------------------------*/
BOOL CElemLdBase::FindText(SearchDat& SD)
{
#define  MAX_ELEM   -1

   CString  str;
   LPCTSTR  psz;
   int      i;

   i = (int)_tcslen(SD.m_strFind);
   if( SD.m_bNext )
   {
      if( SD.m_iFindElem< MAX_ELEM ) SD.m_iFindElem = MAX_ELEM;
      for ( ; SD.m_iFindElem < 0; SD.m_iFindElem++ ) 
      {
         if ( SD.m_iFindElem == -1 )
         {
            str = (CString&)m_Name;
         }else 
         {
            break;
         }
         if (SD.m_bFindOnly || m_Name.m_Flag.Chk(TF_EDITABLE))
         {
            psz = str.GetBuffer(0);
            SD.m_iFindLen = (int)_tcslen(psz);
            if( SD.m_iFindPos < 0 ) SD.m_iFindPos = 0;
            if( SD.m_iFindLen >= i )
            {
               for( ; SD.m_iFindPos <= SD.m_iFindLen-i; SD.m_iFindPos++ )
               {
                  if( SD.m_bCase ?
                         ( _tcsncmp (psz + SD.m_iFindPos, SD.m_strFind, i) == 0 ) :
                         ( _tcsnicmp(psz + SD.m_iFindPos, SD.m_strFind, i) == 0 ) )
                  {
                     str.ReleaseBuffer();
                     SD.m_iFindLen = (int)_tcslen(SD.m_strFind);
                     return TRUE;
                  }
               }
            }
            str.ReleaseBuffer();
            SD.m_iFindPos = 0;
         }
      }
   } else {
      if( SD.m_iFindElem>= 0 ) 
      {
         SD.m_iFindElem = -1;
      }
      for( ; SD.m_iFindElem >= MAX_ELEM; SD.m_iFindElem-- )
      {
         if ( SD.m_iFindElem == -1 ) 
         {
            str = (CString&)m_Name;
         }else 
         {
            break;
         }

         if (SD.m_bFindOnly || m_Name.m_Flag.Chk(TF_EDITABLE))
         {
            psz = str.GetBuffer(0);
            SD.m_iFindLen = (int)_tcslen(psz);
            if( SD.m_iFindPos > SD.m_iFindLen ) 
            {
               SD.m_iFindPos = SD.m_iFindLen-i;
            }
            for( ; SD.m_iFindPos >= 0; SD.m_iFindPos-- )
            {
               if( SD.m_bCase ?
                      ( _tcsncmp (psz + SD.m_iFindPos, SD.m_strFind, i) == 0 ) :
                      ( _tcsnicmp(psz + SD.m_iFindPos, SD.m_strFind, i) == 0 ) )
               {
                  str.ReleaseBuffer();
                  SD.m_iFindLen = (int)_tcslen(SD.m_strFind);
                  return TRUE;
               }
            }
            SD.m_iFindPos = INT_MAX;   // a big integer ..
            str.ReleaseBuffer();
         }
      }
   } 
   SD.m_iFindLen = 0;
   return FALSE;

#undef MAX_ELEM
}

/*func*------------------------------------------------------------------------
  get the data identifier of the element for watch (if necessary "elementname.pinname")
  in :-
  out:identifier of the element
-----------------------------------------------------------------------------*/
CText& CElemLdBase::GetTextAt(CPoint /*p*/, CPoint &start,int /*iGrid*/)
{
   start = m_Origin;
   return m_Name;
}

/*func*------------------------------------------------------------------------
  get text in element 
  in :iElemNr  - neg. numbers are reserved for inherit classes,
                 pos. numbers are the pins in m_con
      iPos     - position in strname in pin
      iLen     - length of wished string
  out:str   - search text
      true  - string found 
      false - nothing found
-----------------------------------------------------------------------------*/
bool CElemLdBase::GetText(int iElemNr, int iPos, int iLen, CString& str)
{
   str = _T("");

   if (iElemNr < -1)
   {
      return (false);
   }
   else if (iElemNr == -1)
   {
      str = ((CString&)m_Name).Mid(iPos, iLen);
      return (true);
   }
   else 
   {
      return CElem::GetText( iElemNr, iPos, iLen, str);
   }

   return (false);
}

/*func*------------------------------------------------------------------------
  replace text in element 
  in :iElemNr  - neg. numbers are reserved for inherit classes,
                 pos. numbers are the pins in m_con
      iPos     - position in strname in pin
      iLen     - length of wished string
      pstr     - new text
  out:-
-----------------------------------------------------------------------------*/
void CElemLdBase::ReplaceSelText(int iElemNr, int iPos, int iLen, LPCTSTR pstr)
{
   if (iElemNr < -1)
   {
      return;
   }
   else if (iElemNr == -1)
   {
      ((CString&)m_Name).Delete(iPos, iLen);
      ((CString&)m_Name).Insert(iPos, pstr);
      return;
   }
   else 
   {
      CElem::ReplaceSelText( iElemNr, iPos, iLen, pstr);
      return;
   }
}

/*func*------------------------------------------------------------------------
  set name of var
  in :pszName - the new name
  out:-
-----------------------------------------------------------------------------*/
void CElemLdBase::SetName(LPCTSTR pszName)  
{ 
   m_Name = pszName; 
   m_Name.m_Flag += TF_EDITABLE;
}

/*func*------------------------------------------------------------------------
  get name of var
  in :-
  out:the name
-----------------------------------------------------------------------------*/
LPCTSTR CElemLdBase::GetName() 
{ 
   return (LPCTSTR)m_Name; 
}

/*func*------------------------------------------------------------------------
  serialize the element to/from stream
  in :ar - the archive
  out:-
-----------------------------------------------------------------------------*/
void CElemLdBase::Serialize(CArchive & ar)
{
   CString str;
   long    lID;
   int     i;

   CElem::Serialize(ar);

	if (ar.IsStoring())
	{
      i = (int) m_Type;
      ar << m_Name << i;
	}
	else
	{
      ar >> m_Name >> i;
      m_Type = (LADDER_TYPE)i;
      // update max id number
      if (GetType() == ET_CONTACT)
      {
         str.LoadString(IDS_GR_NAMEMASK_CONTACT);
      } else {
         str.LoadString(IDS_GR_NAMEMASK_COIL);
      }
      if( _stscanf( (LPCTSTR)m_Name, (LPCTSTR)str, &lID ) == 1) 
      {
         if (GetType() == ET_CONTACT)
         {
            CGRTool::SetID(CGRTool::ID_CONTACT, lID);
         } else {
            CGRTool::SetID(CGRTool::ID_COIL, lID);
         }
      }
	}
}

/*func*------------------------------------------------------------------------
  serialize the element to/from XML stream
  in :ar - the archive
  out:-
-----------------------------------------------------------------------------*/
void CElemLdBase::SerializeXML(CXMLArchive &ar)
{
   long    lID;
   CString str;
   CPoint  p;
   CPin   *pPin=NULL;

   ASSERT((GetType() == ET_CONTACT) || 
          (GetType() == ET_COIL));
   if(ar.IsStoring())
   {
      // all CXMLArchive fire HRESULT exceptions, which must be cought!!!
      try {            // name and create node here !!
         CElem::SerializeXML(ar);  // origin, size, comment

         ar.SetStringAttributeUTF8( XML_4GR_ATTR_NAME, (CString&)m_Name);

         if     ( m_Type == LD_NORMAL   ) ar.SetAttribute( XML_4GR_ATTR_FUNC, XML_4GR_ATTR_VAL_NORMAL  );
         else if( m_Type == LD_CLOSED   ) ar.SetAttribute( XML_4GR_ATTR_FUNC, XML_4GR_ATTR_VAL_CLOSED  );
         else if( m_Type == LD_NEGATIVE ) ar.SetAttribute( XML_4GR_ATTR_FUNC, XML_4GR_ATTR_VAL_NEGATIVE);
         else if( m_Type == LD_SET      ) ar.SetAttribute( XML_4GR_ATTR_FUNC, XML_4GR_ATTR_VAL_SET     );
         else if( m_Type == LD_RESET    ) ar.SetAttribute( XML_4GR_ATTR_FUNC, XML_4GR_ATTR_VAL_RESET   );
         else if( m_Type == LD_POSTRANS ) ar.SetAttribute( XML_4GR_ATTR_FUNC, XML_4GR_ATTR_VAL_POSTRANS);
         else if( m_Type == LD_NEGTRANS ) ar.SetAttribute( XML_4GR_ATTR_FUNC, XML_4GR_ATTR_VAL_NEGTRANS);
         else ASSERT(FALSE);

         ASSERT( m_Con.GetSize() == 2 );
         for( int i=0; i<m_Con.GetSize(); i++ ) 
         {
            pPin = m_Con.GetAt(i); 
            ASSERT( pPin );
            if( pPin )
            {
               if( pPin->m_Pos.x == m_Origin.x ) ar.SetAttribute( XML_4GR_ATTR_INORIGIN,  pPin->m_Pos);
               else                              ar.SetAttribute( XML_4GR_ATTR_OUTORIGIN, pPin->m_Pos);
            }
         } 
      }        
      catch (HRESULT hr)
      {
         TRACE(_T("Cannot save to XML; hr=0x%08X\n"), hr);
         throw hr;   // fire upstairs...        
      }    
   } else {
      try {   
         CElem::SerializeXML(ar);  // origin, size, comment

         ar.GetAttribute( XML_4GR_ATTR_NAME, (CString&)m_Name);
         if (GetType() == ET_CONTACT)
         {
            str.LoadString(IDS_GR_NAMEMASK_CONTACT);
         } else {
            str.LoadString(IDS_GR_NAMEMASK_COIL);
         }
         if( _stscanf( (LPCTSTR)m_Name, (LPCTSTR)str, &lID ) == 1) 
         {
            if (GetType() == ET_CONTACT)
            {
               CGRTool::SetID(CGRTool::ID_CONTACT, lID);
            } else {
               CGRTool::SetID(CGRTool::ID_COIL, lID);
            }
         }

         if( ar.GetAttribute( XML_4GR_ATTR_FUNC, str) )
         {
            if     ( str == XML_4GR_ATTR_VAL_NORMAL  ) { m_Type = LD_NORMAL;   }
            else if( str == XML_4GR_ATTR_VAL_CLOSED  ) { m_Type = LD_CLOSED;   }
            else if( str == XML_4GR_ATTR_VAL_NEGATIVE) { m_Type = LD_NEGATIVE; }
            else if( str == XML_4GR_ATTR_VAL_SET     ) { m_Type = LD_SET;      }
            else if( str == XML_4GR_ATTR_VAL_RESET   ) { m_Type = LD_RESET;    }
            else if( str == XML_4GR_ATTR_VAL_POSTRANS) { m_Type = LD_POSTRANS; }
            else if( str == XML_4GR_ATTR_VAL_NEGTRANS) { m_Type = LD_NEGTRANS; }
            else {
               TRACE(_T("CElemLdBase: invalid func\n")); throw E_FAIL; 
            }
         } else {
            TRACE(_T("CElemLdBase: missing func\n")); throw E_FAIL; 
         }

         ASSERT( m_Con.GetSize() == 2 );
         for( int i=0; i<2; i++ )
         {
            CPin  *pPin = m_Con.GetAt(i);
            ASSERT( pPin );
            if( pPin ) 
            {
               if( pPin->m_Flag.Chk(PF_OUT) )
               {
                  if( !ar.GetAttribute( XML_4GR_ATTR_OUTORIGIN, p) )
                  {
                     TRACE(_T("CElemLdBase: missing outorigin\n")); throw E_FAIL; 
                  }
               } else {
                  if( !ar.GetAttribute( XML_4GR_ATTR_INORIGIN, p) )
                  {
                     TRACE(_T("CElemLdBase: missing inorigin\n")); throw E_FAIL; 
                  }
               }
               pPin->m_Pos = p;
            }
         }
      }
      catch (HRESULT hr)
      {
         TRACE(_T("Cannot load from XML; hr=0x%08X\n"), hr);
         throw hr;   // fire upstairs...
      }   
   }
}
/*
bool  CElemLdBase::JoinTest(TDirection Dir, CElem* pE)
{
   if(Dir==DIR_E)
      if( pE->m_eElemTyp == ET_CONOUT  ||
          pE->m_Flag.Chk(EF_RUBBLINE)  ||
          (pE->GetType() == ET_LINE)&&   // line may be normalized or not
          (pE->GetSize().cy   == 0)  &&
          (SearchConn(pE->GetOrigin()) || (SearchConn(pE->GetOrigin()+pE->GetSize())) )
        )
   {
      return true;
   }
   return false;
}
*/
bool  CElemLdBase::CheckDropIn( CElem* pE)        // requires a normalized line to check against
{
   CPin* pPin;
   bool bOK = false;

   ASSERT(pE != NULL);

   if ((pE != NULL)                   && 
       (pE->GetType()    == ET_LINE)  &&    // check if a line
       (pE->GetSize().cy == 0))             // check if <INSIDE a HOIZONTAL LINE>
   {
      if ((pE->GetOrigin().x<=m_Origin.x)     &&      // inside (!!!requires a normalized line)
          (pE->GetOrigin().x+pE->GetSize().cx >= m_Origin.x+m_Size.cx) )
      {
         bOK = true;
         for( int i=0; bOK && i<m_Con.GetSize(); i++ )
         {
            pPin = m_Con.GetAt(i);
            ASSERT(pPin);
            if( pPin )
            {
               bOK &= (pPin->m_Pos.y == pE->GetOrigin().y);
            }
         }
      } else // not strictly inside: 2nd chance: a single and unconnected line point below the element
      {
         TPinEntry *pPE = NULL;

         if (pE->GetOrigin().x > m_Origin.x)
            m_pPinTable->Lookup(pE->GetOrigin(),pPE);
         else if(pE->GetOrigin().x+pE->GetSize().cx < m_Origin.x+m_Size.cx)
            m_pPinTable->Lookup( pE->GetOrigin() + pE->GetSize(), pPE);

         if (pPE && pPE->m_iConnects == 1)
         {
            for(int i=0; !bOK && (i< m_Con.GetSize()); i++)
            {
               if(m_Con[i] && m_Con[i]->m_Pos.y == pE->GetOrigin().y)
                  bOK = true;
            }
         }
      }
   }
   return bOK;
}

bool  CElemLdBase::CheckDropOut()
{
   return true;
}

void  CElemLdBase::Invert()
{
   switch(m_Type)
   {
   case LD_NORMAL:   m_Type = LD_NEGATIVE;   break;
   case LD_NEGATIVE: m_Type = LD_NORMAL;     break;
   case LD_POSTRANS: m_Type = LD_NEGTRANS;   break;
   case LD_NEGTRANS: m_Type = LD_POSTRANS;   break;
   case LD_SET:      m_Type = LD_RESET;      break;
   case LD_RESET:    m_Type = LD_SET;        break;
   }
}

IMPLEMENT_SERIAL(CElemLdBase, CElem, SERIAL_VERSION)

IMPLEMENT_SERIAL(CElemContact, CElemLdBase, SERIAL_VERSION)

IMPLEMENT_SERIAL(CElemCoil, CElemLdBase, SERIAL_VERSION)
