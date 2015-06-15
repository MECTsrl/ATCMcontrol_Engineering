

#include "stdafx.h"
#include "GrEditor.h"
#include "GrEditorDoc.h"
#include "GrEditorView.h"
#include "GrEditorUtil.h"
#include "ElemFB.h"
#include "PosArray.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define MIN_FBLK_WIDTH    (2+4)
#define MIN_FB_WIDTH      (2+6)

#define LAST_FIND_ELEM   -2

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CElemFblk::CElemFblk()
{
   m_OriginOffs = CPoint(0,1);
   SetName(_T(""));
   SetInst(_T(""));
   m_bShowENO = false;
   m_bENOdefault = false;
   m_LastFindElem  = LAST_FIND_ELEM;
}

CElemFblk::~CElemFblk()
{
}

/*func*------------------------------------------------------------------------
  calculate min. height of FB/FCT/PRO
  (pinlist must reading before!!! - use the same algorithmen like SetPinPos !!!)
  in :-
  out:minimal height for SetMinSize 
-----------------------------------------------------------------------------*/
int CElemFblk::CalcMinHeight(void)
{
   CPin *pPin;
   int   iHeight, i;
   int   iIn=0, iOut=0, iInOut=0, iExt=0;

   iHeight = (GetType() == ET_FB) ? 4 : 2;  // head offset
   for( i=0; i<m_Con.GetSize(); i++ )
   {
      pPin = m_Con.GetAt(i);
      if( pPin )
      {
         if( pPin->m_Flag.Chk(PF_OUT) && 
             pPin->m_Name.IsEmpty()      ) 
         {
            iOut++;
         }
      }
   }

   for( i=0; i<m_Con.GetSize(); i++ )
   {
      pPin = m_Con.GetAt(i);
      if( pPin )
      {
         if( pPin->m_Flag.Chk(PF_IN      )) 
            iIn++;

         if( pPin->m_Flag.Chk(PF_OUT ) &&
             !pPin->m_Name.IsEmpty()      ) 
         {
           iOut++;
         }
         
         if( pPin->m_Flag.Chk(PF_INOUT   )) 
         {
            iHeight += (max (iIn, iOut) * 2);
            iIn  = 0; 
            iOut = 0; 
            iInOut++;
         }
      }
   }
   
   /*for( i=0; i<m_Con.GetSize(); i++ )
   {
      pPin = m_Con.GetAt(i);
      if( pPin )
      {
         //if( pPin->m_Flag.Chk(PF_EXT_INP )) iExt += 2; // ++**++ JD extInParam
         if( pPin->m_Flag.Chk(PF_EXT_OUTP)) iExt += 2; // ++**++ JD extInParam
      }
   }*/
   iExt = 2 * m_arrInExtNameStubs.GetSize();
   iHeight += (max (iIn, iOut) + (iInOut + 1)/2) * 2 + iExt;

   return iHeight;
}

/*func*------------------------------------------------------------------------
  calculate max. height of FB/FCT/PRO
  (pinlist must reading before!!! - use the same algorithmen like SetPinPos !!!)
  in :-
  out:minimal height for SetMaxSize 
-----------------------------------------------------------------------------*/
int CElemFblk::CalcMaxHeight(void)
{
   CPin *pPin;
   int   iHdOffs, iHeight, i;
   int   iIn=0, iOut=0, iInOut=0, iExt=0;

   iHdOffs = (GetType() == ET_FB) ? 4 : 2;
   for( i=0; i<m_Con.GetSize(); i++ )
   {
      pPin = m_Con.GetAt(i);
      if( pPin )
      {
         if( pPin->m_Flag.Chk(PF_IN      )) iIn++;
         if( pPin->m_Flag.Chk(PF_OUT     )) iOut++;
         if( pPin->m_Flag.Chk(PF_INOUT   )) iInOut++;
      }
   }
   for( i=0; i<m_Con.GetSize(); i++ )
   {
      pPin = m_Con.GetAt(i);
      if( pPin )
      {
         if( pPin->m_Flag.Chk(PF_EXT_INP )) iExt = 2;
         if( pPin->m_Flag.Chk(PF_EXT_OUTP)) iExt = 2;
      }
   }
   if (iExt)
   {
      iHeight = iHdOffs + (max (iIn, iOut) + iInOut) * 2 + iExt * (MAX_EXT_INOUT-1);
   } else {
      iHeight = MAX_YGRID - 4;
   }
   return iHeight;
}

/*func*------------------------------------------------------------------------
  check (inout) pins
  in :-
  out:-
-----------------------------------------------------------------------------*/
void CElemFblk::CheckPins()
{
   CPin *pPin, *pPin2;
   int   i, j;
   bool  bFound;

   m_Con.ClearFlags(PF_MODIFIED); //  reset all pin (processed) flags

   // generate in/out pins
   for( i=0; i<m_Con.GetSize(); i++ )
   {
      pPin = m_Con.GetAt(i);
      ASSERT(pPin);
      if( pPin && 
          pPin->m_Flag.Chk(PF_INOUT) && 
         !pPin->m_Flag.Chk(PF_MODIFIED) )
      {  
         // left
         bFound = false;
         for( j=i+1; j<m_Con.GetSize(); j++ )
         {
            pPin2 = m_Con.GetAt(j);
            ASSERT(pPin2);
            if( pPin2 && 
                pPin2->m_Flag.Chk(PF_INOUT) && 
               !pPin2->m_Flag.Chk(PF_MODIFIED) &&
                pPin2->m_Name == pPin->m_Name )
            { // right
               pPin2->m_Flag.Set(PF_MODIFIED);
               bFound = true;
            }
         }
         if( !bFound )
         { // insert second pin
            pPin2 = new CPin(pPin);
            ASSERT(pPin2);
            if( pPin2 )
            {
               pPin2->m_Flag.Set(PF_MODIFIED);
               m_Con.Add(pPin2);
            }
         }
         pPin->m_Flag.Set(PF_MODIFIED);
      }
   }
}

/*func*------------------------------------------------------------------------
  calculate max. height of FB/FCT/PRO
  (pinlist must reading before!!! - use the same algorithmen like SetPinPos !!!)
  in :-
  out:minimal height for SetMaxSize 
-----------------------------------------------------------------------------*/
int CElemFblk::CalcOptWidth()
{
   CPin *pPin;
   int   iLeft=0, iRight=0, i, iLen;
   bool  bInFound=false;
   bool  bOutFound=false;

   for( i=0; i<m_Con.GetSize(); i++ )
   {
      pPin = m_Con.GetAt(i);
      if( pPin )
      {
         iLen = CText::GetIdentLen (pPin->m_Name.StrRef());

         if      (pPin->m_Flag.Chk(PF_IN      )) 
         {
            if (iLen > iLeft ) iLeft = iLen;
            bInFound = true;
         } 
         else if (pPin->m_Flag.Chk(PF_OUT     )) 
         {
            if (iLen > iRight) iRight = iLen;
            bOutFound = true;
         } 
         else if (pPin->m_Flag.Chk(PF_INOUT   )) 
         {
            if (iLen > iLeft ) iLeft = iLen;
            if (iLen > iRight) iRight = iLen;
            bInFound = true;
            bOutFound = true;
         } 
         else if (pPin->m_Flag.Chk(PF_EXT_INP )) 
         {
            if (iLen > iLeft ) iLeft = iLen;
            bInFound = true;
         } 
         else if (pPin->m_Flag.Chk(PF_EXT_OUTP)) 
         {
            if (iLen > iRight) iRight = iLen;
            bOutFound = true;
         }
      }
   }
   if (bInFound && bOutFound)
   {
      if (iLeft > iRight)
      {
         iRight = iLeft;
      } else {
         iLeft = iRight;
      }
   }
   i = PIN_LENGTH + iLeft + 1 + iRight + PIN_LENGTH;

   iLen = 2*PIN_LENGTH + CText::GetIdentLen (m_Name.StrRef()) + 1;
   if (iLen > i) 
   {
      i = iLen;
   }

   iLen = 2*PIN_LENGTH + CText::GetIdentLen (m_Inst.StrRef()) + 1;
   if (iLen > i) 
   {
      i = iLen;
   }

   i = (i+1) & 0xFFFFFFFE;

   return i;
}

/*func*------------------------------------------------------------------------
  generate all pin position of one FB/FNC/PRO
  in :-
  out:-
-----------------------------------------------------------------------------*/
void CElemFblk::SetPinPos()
{
   CPin *pPin, *pPin2;
   int   i, j, w, h, iInOffs, iOutOffs, iHdOffs;
   bool  bFound;

   w = GetSize().cx;
   h = GetSize().cy;
   iHdOffs = (GetType() == ET_FB) ? 4 : 2;  // FB has an instance line ..

   m_Con.ClearFlags(PF_MODIFIED);     //  reset all pin (processed) flags

   iInOffs  = iHdOffs + 1;
   iOutOffs = iHdOffs + 1;

   // generate function, .. output pins right at top
   for( i=0; i<m_Con.GetSize(); i++ )
   {
      pPin = m_Con.GetAt(i);
      if (pPin && !pPin->m_Flag.Chk(PF_MODIFIED)) 
      {   
         if( pPin->m_Flag.Chk(PF_DISABLED) )
         {
            pPin->m_Pos = GetOrigin();
            pPin->m_Flag.Set(PF_MODIFIED);
         } 
         else if (pPin->m_Flag.Chk(PF_OUT) && pPin->m_Name.IsEmpty())
         {
            pPin->m_Pos = GetOrigin() + CPoint(w,iOutOffs);
            iOutOffs += 2;
            pPin->m_Flag.Set(PF_MODIFIED);
         }
      }
   }

   // generate input pins
   // generate output pins
   // generate in/out pins
   for( i=0; i<m_Con.GetSize(); i++ )
   {
      pPin = m_Con.GetAt(i);
      if (pPin && !pPin->m_Flag.Chk(PF_MODIFIED)) 
      {   
         if      (pPin->m_Flag.Chk(PF_IN)) 
         {   
            pPin->m_Pos = GetOrigin() + CPoint(0,iInOffs);
            iInOffs += 2;
         }
         else if (pPin->m_Flag.Chk(PF_OUT) )
         {
            pPin->m_Pos = GetOrigin() + CPoint(w,iOutOffs);
            iOutOffs += 2;
         }
         else if (pPin->m_Flag.Chk(PF_INOUT))
         {  
            iOutOffs = iInOffs = max(iInOffs, iOutOffs);
            // left
            bFound = false;
            pPin->m_Pos = GetOrigin() + CPoint(0,iInOffs);
            for( j=i+1; j<m_Con.GetSize(); j++ )
            {
               pPin2 = m_Con.GetAt(j);
               ASSERT(pPin2);
               if( pPin2 && 
                   pPin2->m_Flag.Chk(PF_INOUT) && 
                  !pPin2->m_Flag.Chk(PF_MODIFIED) &&
                   pPin2->m_Name == pPin->m_Name )
               { // right
                  pPin2->m_Pos = GetOrigin() + CPoint(w,iOutOffs);
                  pPin2->m_Flag.Set(PF_MODIFIED);
                  bFound = true;
               }
            }
            if( !bFound )
            { // second pin should be inserted - call CheckPins()
               ASSERT(FALSE);
            }
            iInOffs  += 2;
            iOutOffs += 2;
         }

         pPin->m_Flag.Set(PF_MODIFIED);
      }
   }

   // extensible inputs
   // extensible outputs
   iOutOffs = iInOffs = max(iInOffs, iOutOffs);

   for( i=0; i<m_Con.GetSize(); i++ )
   {
      pPin = m_Con.GetAt(i);
      ASSERT(pPin);
      if (pPin)
      {
         if (pPin->m_Flag.Chk(PF_DISABLED))
         {
            pPin->m_Pos = GetOrigin();
         } 
         else if (pPin->m_Flag.Chk(PF_EXT_INP))
         {
            pPin->m_Pos = GetOrigin() + CPoint(0,iInOffs);
            iInOffs += 2;
         }
         else if (pPin->m_Flag.Chk(PF_EXT_OUTP))
         {
            pPin->m_Pos = GetOrigin() + CPoint(w,iOutOffs);
            iOutOffs += 2;
         }
         pPin->m_Flag.Set(PF_MODIFIED);
      }
   }
}

/*func*------------------------------------------------------------------------
  reconstruct pin size with HIDDEN attribut
  in :-
  out:-
-----------------------------------------------------------------------------*/
void CElemFblk::SetPinSize()
{
   CPin *pPin;
   // layout for FB/FCT/PRO pins:
   //  left input 2 grid long
   //  right output 2 grid long
   for( int i=0; i<m_Con.GetSize(); i++ )
   {
      pPin = m_Con.GetAt(i);
      ASSERT(pPin);
      if( pPin )
      {
         if( pPin->m_Flag.Chk(PF_DISABLED) ) 
         {
            pPin->m_Size = CSize( 0,0);
         } else {
            if( pPin->m_Pos.x == m_Origin.x ) 
            {
               pPin->m_Size = CSize( PIN_LENGTH,0);
            } else {
               pPin->m_Size = CSize(-PIN_LENGTH,0);
            }
            pPin->m_Flag.Clr(PF_HIDDEN);
         }
      }
   }
}

/*func*------------------------------------------------------------------------
  if pin position and size are correct, we can reconstruct the element helper
  attributes
  in :-
  out:-
-----------------------------------------------------------------------------*/
void CElemFblk::SetAttr()
{
   bool  bInNameFound =false;
   bool  bOutNameFound=false;
   bool  bExtInFound  =false;
   bool  bExtOutFound =false;
   CPin *pPin;

   for( int i=0; i<m_Con.GetSize(); i++ )
   {
      pPin = m_Con.GetAt(i);
      ASSERT(pPin);
      if( pPin )
      {
         if( !(pPin->m_Name.Str()).IsEmpty() )
         {
            if( pPin->m_Pos.x == m_Origin.x             ) bInNameFound  = true;
            if( pPin->m_Pos.x == m_Origin.x + m_Size.cx ) bOutNameFound = true;
         }
         if (pPin->m_Flag.Chk(PF_EXT_INP)) 
         {
            bExtInFound  = true;
         }
         if (pPin->m_Flag.Chk(PF_EXT_OUTP)) 
         {
            bExtOutFound = true;
         }
      }
   }

   if( bInNameFound  ) 
   {
      ClrFlag(EF_NO_INNAME);  
   } else {
      SetFlag(EF_NO_INNAME);  
   }
   if( bOutNameFound ) 
   {
      ClrFlag(EF_NO_OUTNAME); 
   } else {
      SetFlag(EF_NO_OUTNAME); 
   }
   if( bExtInFound   ) 
   {
      SetFlag(EF_EXT_INP);   
   } else {
      ClrFlag(EF_EXT_INP);   
   }
   if( bExtOutFound  ) 
   {
      SetFlag(EF_EXT_OUTP);  
   } else {
      ClrFlag(EF_EXT_OUTP);  
   }
}


/*func*------------------------------------------------------------------------
  draw the line
  in :pDC   - pointer to device context
      pView - pointer to document view
      iGrid - actual display resolution
  out:-
-----------------------------------------------------------------------------*/
void CElemFblk::Draw(CDC * pDC, CGrEditorView * pView, int iGrid)
{
   CRect    r;
   CSize    size;
   CPin    *pPin;
   CPoint   p;
   CString  str;
   int      i; 
   PEN_TYPE PenType;
   bool     bScreen = !pDC->IsPrinting();

   if (!pView)
   {
      return;
   }
   ASSERT((GetType() == ET_FB       ) ||
          (GetType() == ET_FUNCTION ) ||
          (GetType() == ET_PROCEDURE));

   // prepare drawing
   if     ( ChkFlag(EF_COMMENT) ) PenType = PE_COMMENT;
   else   /* normal */               PenType = PE_NORMAL;

    pDC->SelectObject(GDIHelper::GetPen  (bScreen, PenType  ));
   if (pView->IsManualMode())
   {
      pDC->SelectObject(GDIHelper::GetBrush(bScreen, BT_BACKGD_SFC));
   } else {
      pDC->SelectObject(GDIHelper::GetBrush(bScreen, BT_BACKGD));
   }
   // draw the FB/FCT/PRO
   GetBodyRect(r, iGrid);

   if (pView->IsManualMode())
   {
      pDC->FillRect(&r, GDIHelper::GetBrush(bScreen, BT_FBLKBK_SFC));
   } else {
      pDC->FillRect(&r, GDIHelper::GetBrush(bScreen, BT_FBLKBK));
   }

   if( GetType() == ET_FB )
   {  // background ..
      i = r.bottom;
      r.bottom = r.top + 2*iGrid;
      if (pView->IsManualMode())
      {
         pDC->FillRect(&r, GDIHelper::GetBrush(bScreen, BT_ELEMBK_SFC));
      } else {
         pDC->FillRect(&r, GDIHelper::GetBrush(bScreen, BT_ELEMBK));
      }
      r.bottom = i;
   }
// pDC->Rectangle( r.left, r.top, r.right+1, r.bottom );
   if     ( ChkFlag(EF_COMMENT)   ) PenType = PE_COMMENT;
   else if( ChkFlag(EF_EMPHASIZE) ) PenType = PE_ERROR;
   else   /* normal */                 PenType = PE_NORMAL;

   pDC->SelectObject(GDIHelper::GetPen  (bScreen, PenType  ));

   pDC->MoveTo( r.left,  r.top    );
   pDC->LineTo( r.right, r.top    );
   pDC->LineTo( r.right, r.bottom );
   pDC->LineTo( r.left,  r.bottom );
   pDC->LineTo( r.left,  r.top    );

   SearchDat *pSD = &pView->m_SearchDat;

   // draw the 'instanz'-name
   if( GetType() == ET_FB )
   {  // text ..

      if((pSD->m_FindCur   == GetOrigin()) &&
         (pSD->m_iFindElem == -2         ) &&
         (pSD->m_iFindPos  >= 0          ) )
      {
         m_Inst.Draw(pDC, r, iGrid, GetColorType(), pSD, 0);
      }else
      {
         m_Inst.Draw(pDC, r, iGrid, GetColorType(), NULL, 0);
      }

      // draw h-line below instance name
      pDC->MoveTo( r.left,  r.top + 2*iGrid-1 );
      pDC->LineTo( r.right, r.top + 2*iGrid-1 );
   }
   // draw the FB/FCT/PRO name
   //  - text - (url) - (picture) -
   CSize yOfs(0,GetType() == ET_FB ? 2*iGrid : 0);

   if((pSD->m_FindCur   == GetOrigin()) &&
      (pSD->m_iFindElem == -1         ) &&
      (pSD->m_iFindPos  >= 0          ) )
   {
      m_Name.Draw(pDC, r+yOfs, iGrid, GetColorType(), pSD, 0);
   }else
   {
      m_Name.Draw(pDC, r+yOfs, iGrid, GetColorType(), NULL, 0);
   }

   // draw the connect point(s)
   for( i=0; i<m_Con.GetSize(); i++ )
   {
      pPin = m_Con.GetAt(i);
      if( pPin )
      {
         pPin->Draw(pDC, iGrid);
         DrawPinText( pDC, pView, i, pPin, iGrid );
         if (pPin->m_Flag.Chk(PF_R_EDGE))
         {
            p = pPin->m_Pos + pPin->m_Size;
            pDC->MoveTo( p.x*iGrid - iGrid, p.y*iGrid - iGrid/2 );
            pDC->LineTo( p.x*iGrid,         p.y*iGrid );
            pDC->MoveTo( p.x*iGrid - iGrid, p.y*iGrid + iGrid/2 );
            pDC->LineTo( p.x*iGrid,         p.y*iGrid );
         }
         if (pPin->m_Flag.Chk(PF_F_EDGE))
         {
            p = pPin->m_Pos + pPin->m_Size;
            pDC->MoveTo( p.x*iGrid,         p.y*iGrid - iGrid/2 );
            pDC->LineTo( p.x*iGrid - iGrid, p.y*iGrid );
            pDC->MoveTo( p.x*iGrid,         p.y*iGrid + iGrid/2 );
            pDC->LineTo( p.x*iGrid - iGrid, p.y*iGrid );
         }
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
  get the body rect without pin range for select operations
  in :r        - place holder for rect
      iGrid    - actual zoom
  out:-
-----------------------------------------------------------------------------*/
void CElemFblk::GetBodyRect(CRect &r, int iGrid)
{
   ASSERT(iGrid > 0);

   if( iGrid > 0 )
   {
      GetRect(r, iGrid);
      r.left  += PIN_LENGTH*iGrid;
      r.right -= PIN_LENGTH*iGrid;
   }
}

/*func*------------------------------------------------------------------------
  
  in :
  out:-
-----------------------------------------------------------------------------*/
void CElemFblk::DrawPinText(CDC * pDC, CGrEditorView * pView, int iPinNr, 
   CPin *pPin, int iGrid)
{
   CString  str;
   CSize    size;
   CRect    r;
   int      x, y;
   bool     bLeft;

   if (!pView)
   {
      return;
   }
   ASSERT_VALID(pPin);

   y     = (pPin->m_Pos.y) * iGrid;
   bLeft = (pPin->m_Pos.x == m_Origin.x);

   if( bLeft ) 
   {         // left
//      bLeft = true;
      if( ChkFlag(EF_NO_OUTNAME) )
      {
         r.SetRect( (m_Origin.x + PIN_LENGTH                           ) * iGrid, y - iGrid,
                    (m_Origin.x + PIN_LENGTH + m_Size.cx - 2*PIN_LENGTH) * iGrid, y + iGrid);
      } else {
         r.SetRect( (m_Origin.x + PIN_LENGTH                           ) * iGrid, y - iGrid,
                    (m_Origin.x + PIN_LENGTH + m_Size.cx/2-2*PIN_LENGTH) * iGrid, y + iGrid);
      }
   } else {  // right
//      bLeft = false;
      if( ChkFlag(EF_NO_INNAME) )
      {
         r.SetRect( (m_Origin.x + PIN_LENGTH          ) * iGrid, y - iGrid,
                    (m_Origin.x + m_Size.cx-PIN_LENGTH) * iGrid, y + iGrid );
      } else {
         r.SetRect( (m_Origin.x + m_Size.cx/2         ) * iGrid, y - iGrid,
                    (m_Origin.x + m_Size.cx-PIN_LENGTH) * iGrid, y + iGrid );
      }
   }

   if( !pPin->m_Name.IsEmpty() )
   {
      int cx;
      
      if(bLeft)
      {
         pPin->m_Name.m_Flag.Set(TF_LEFT);
      }else
      {
         pPin->m_Name.m_Flag.Set(TF_RIGHT);
      }

      if(bLeft)   // space to line
      {
         r.left += 2;
      }else
      {
         r.right-= 2;
      }

      SearchDat *pSD = &pView->m_SearchDat;
      if((pSD->m_FindCur   == GetOrigin()) &&
         (pSD->m_iFindElem == iPinNr     ) &&
         (pSD->m_iFindPos  >= 0          ) )
      {
         cx = pPin->m_Name.Draw(pDC, r, iGrid, GetColorType(), pSD, 0).cx;
      }else
      {
         cx = pPin->m_Name.Draw(pDC, r, iGrid, GetColorType(), NULL, 0).cx;
      }

      // line for inout pins
      x = r.left + (bLeft ? 2 : (r.Width() - cx - 2));
      if( pPin->m_Flag.Chk(PF_INOUT) )
      {
         pDC->MoveTo((bLeft ? (x + 2 + cx                            ) :
                              ((m_Origin.x + m_Size.cx/2) * iGrid    )), y);
         pDC->LineTo((bLeft ? ((m_Origin.x + m_Size.cx/2) * iGrid + 1) :
                              (x - 2                                 )), y);
      }
   }
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
BOOL CElemFblk::FindText(SearchDat& SD)
{
// FB has 2 string for search (m_Name, m_Inst)
   CString  str;
   LPCTSTR  psz;
   int      i;
   CText   *pText;

   i      = (int)_tcslen(SD.m_strFind);
   if( SD.m_bNext )
   {
      if( SD.m_iFindElem < m_LastFindElem ) 
      {
         SD.m_iFindElem = m_LastFindElem;
      }

      for ( ; SD.m_iFindElem < 0; SD.m_iFindElem++ ) 
      {
         if     ( SD.m_iFindElem == -2 ) pText = &m_Inst;
         else if( SD.m_iFindElem == -1 ) pText = &m_Name;
         else break;

         if (SD.m_bFindOnly || pText->m_Flag.Chk(TF_EDITABLE))
         {
            str = pText->Str();
            psz = str.GetBuffer(0);
            SD.m_iFindLen = (int)_tcslen(psz);
            if( SD.m_iFindPos < 0 ) 
            {
               SD.m_iFindPos = 0;
            }

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
      // search in pins at next ..
      return CElem::FindText(SD);
   } else 
   {
      // search in pins at next ..
      if( CElem::FindText(SD) )
      {
         return TRUE;
      }

      if( SD.m_iFindElem >= 0 )
      {
         SD.m_iFindElem = -1;
      }

      for( ; SD.m_iFindElem >= m_LastFindElem; SD.m_iFindElem-- )
      {
         if     ( SD.m_iFindElem == -2 ) pText = &m_Inst;
         else if( SD.m_iFindElem == -1 ) pText = &m_Name;
         else break;

         if (SD.m_bFindOnly || pText->m_Flag.Chk(TF_EDITABLE))
         {
            str = pText->Str();
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
            SD.m_iFindPos = INT_MAX;    // a big integer ..
            str.ReleaseBuffer();
         }
      }
   } 
   SD.m_iFindLen = 0;
   return FALSE;

}

/*func*------------------------------------------------------------------------
  get the data identifier of the element 
  in :p - search position in grid coords
  out:CText& - text-data
      start  - reference point for this data
-----------------------------------------------------------------------------*/
CText& CElemFblk::GetTextAt(CPoint p, CPoint &start, int iGrid)
{
   CText  &text=CElem::GetTextAt(p, start, iGrid);
   CRect   r;
   CPin   *pPin;
   int     i=0;

   // instance ?
   if (GetType() == ET_FB)
   {
      r.SetRect( (m_Origin.x             + PIN_LENGTH)*iGrid, 
                 (m_Origin.y                         )*iGrid, 
                 (m_Origin.x + m_Size.cx - PIN_LENGTH)*iGrid, 
                 (m_Origin.y + 2                     )*iGrid);
      if (r.PtInRect(p)) 
      {
         start = m_Origin;
         return m_Inst;
      }
      i = 2;
   }
   // name ?
   r.SetRect( (    m_Origin.x             + PIN_LENGTH)*iGrid, 
              (i + m_Origin.y                         )*iGrid, 
              (    m_Origin.x + m_Size.cx - PIN_LENGTH)*iGrid, 
              (i + m_Origin.y + 2                     )*iGrid);
   if (r.PtInRect(p)) 
   {
      start = m_Origin + CPoint(i,i);
      return m_Name;
   }
   // pin ?
   for( i=0; i<m_Con.GetSize(); i++ )
   {
      pPin = m_Con.GetAt(i);
      if( pPin->m_Pos.x == m_Origin.x )
      { // pin is left ..
         r.SetRect( (m_Origin.x             + PIN_LENGTH)*iGrid, 
                    (pPin->m_Pos.y-1                    )*iGrid, 
                    (m_Origin.x + m_Size.cx/2           )*iGrid, 
                    (pPin->m_Pos.y+1                    )*iGrid );
      } else {
         r.SetRect( (m_Origin.x + m_Size.cx/2           )*iGrid, 
                    (pPin->m_Pos.y-1                    )*iGrid, 
                    (m_Origin.x + m_Size.cx - PIN_LENGTH)*iGrid, 
                    (pPin->m_Pos.y+1                    )*iGrid );
      }
      if( pPin && r.PtInRect(p) ) 
      {
         text  = pPin->m_Name;
         start = pPin->m_Pos;
         break;
      }
   }

   return text;
}

/*func*------------------------------------------------------------------------
  get the text space in elem
  in :rect - placeholder for result
  out:rect - filled with text grid coords
-----------------------------------------------------------------------------*/
void CElemFblk::GetTextRect(int iTxtIdx, CRect &rect, int iGrid)
{
   if (iTxtIdx == -2)       // instance
   {
      if (GetType() == ET_FB)
      {
         rect.SetRect( (m_Origin.x             + PIN_LENGTH)*iGrid,
                       (m_Origin.y                         )*iGrid,
                       (m_Origin.x + m_Size.cx - PIN_LENGTH)*iGrid,
                       (m_Origin.y + 2                     )*iGrid);
         return;
      }
   } else if (iTxtIdx == -1) // name
   {
      if (GetType() == ET_FB)
      {
         rect.SetRect( (m_Origin.x             + PIN_LENGTH)*iGrid,
                       (m_Origin.y + 2                     )*iGrid,
                       (m_Origin.x + m_Size.cx - PIN_LENGTH)*iGrid,
                       (m_Origin.y + 4                     )*iGrid);
         return;
      } else {
         rect.SetRect( (m_Origin.x             + PIN_LENGTH)*iGrid,
                       (m_Origin.y                         )*iGrid,
                       (m_Origin.x + m_Size.cx - PIN_LENGTH)*iGrid, 
                       (m_Origin.y + 2                     )*iGrid);
         return;
      }
   } else {                  // pin 
      if (iTxtIdx < m_Con.GetSize())
      {
         CPin *pPin = m_Con.GetAt(iTxtIdx);
         ASSERT(pPin);
         if (pPin)
         {
            if( pPin->m_Pos.x == m_Origin.x )
            { // pin is left ..
               rect.SetRect( (m_Origin.x + PIN_LENGTH )*iGrid,  
                             (pPin->m_Pos.y-1         )*iGrid, 
                             (m_Origin.x + m_Size.cx/2)*iGrid, 
                             (pPin->m_Pos.y+1         )*iGrid);
               return;
            } else {
               rect.SetRect( (m_Origin.x + m_Size.cx/2           )*iGrid,
                             (pPin->m_Pos.y-1                    )*iGrid,
                             (m_Origin.x + m_Size.cx - PIN_LENGTH)*iGrid, 
                             (pPin->m_Pos.y+1                    )*iGrid);
               return;
            }
         }
      }
   }
   rect.SetRectEmpty();
}

/*func*------------------------------------------------------------------------
  resize and/or move element with connector pins
  in :dx - delta x for m_Origin
      dy - delta y for m_Origin
      sx - delta x for m_Size
      sy - delta y for m_Size
  out:-
-----------------------------------------------------------------------------*/
bool CElemFblk::Resize(int dx, int dy, int sx, int sy)
{
   CPin   *pPin, *pPi, *pPo;
   CSize   oldSize;
   int     i, iSY, iN, iIn, iOut;
   CString str, strHd, strHdIn, strHdOut;
   bool    bRes;
   int     iInCount = 0;  // ++**++ JD extInParam
   int     iOutCount = 0; // ++**++ JD extInParam
   int     iInRun = 0;    // ++**++ JD extInParam
   int     iOutRun = 0;   // ++**++ JD extInParam
   int     iInNewNr = 0;  // ++**++ JD extInParam
   int     iOutNewNr = 0; // ++**++ JD extInParam

   ASSERT(m_pPinTable);
   // save old size
   oldSize = m_Size;
   // resize the element body
   bRes = CElem::Resize( dx, dy, sx, sy);
   // are there extensible in/outputs ?
   if ((m_pPinTable != NULL) &&
        bRes &&
       (ChkFlag(EF_EXT_INP) || ChkFlag(EF_EXT_OUTP)) && 
       (oldSize.cy != m_Size.cy) )
   {
      iSY = m_Size.cy - oldSize.cy;
      if( iSY > 0 )
      {  // insert new connect pins
         pPi = pPo = NULL;
         iIn = iOut = 2;
         for( i=0; i<m_Con.GetSize(); i++ )
         {
            pPin = m_Con.GetAt(i);
            ASSERT_VALID( pPin );
            if( pPin )
            {  // search pin counter
               if( pPin->m_Flag.Chk(PF_EXT_INP) || 
                   pPin->m_Flag.Chk(PF_EXT_OUTP) ) 
               {  // extract number at end of string and 
                  // save string head ..
                  str = pPin->m_Name;
                  for( iN = str.GetLength()-1; iN>0; iN-- )
                  {
                     if( !_istdigit(str[iN]) ) break;
                  }
                  strHd = str.Left(iN+1);
                  str = str.Right( str.GetLength() - iN-1);
                  if( _stscanf( (LPCTSTR)str, _T("%d"), &iN) != 1) 
                  {
                     iN = 0;
                  }
                  // save pin data for copy ..
                  if( pPin->m_Flag.Chk(PF_EXT_INP)  )
                  {
                     strHdIn = strHd;
                     if( !pPi      ) pPi = pPin;  // get first pin (data)
                     if( iIn <= iN ) iIn = iN+1;  // save next possible number 
                     iInCount++; // ++**++ JD extInParam
                  }
                  if( pPin->m_Flag.Chk(PF_EXT_OUTP) )
                  {
                     strHdOut = strHd;
                     if( !pPo       ) pPo = pPin; // get first pin (data)
                     if( iOut <= iN ) iOut = iN+1;// save next possible number 
                     iOutCount++; // ++**++ JD extInParam
                  }
               }
            }
         }
         // insert new pins for growing element ..
         // ++**++ JD extInParam -->
         iInRun = iInCount % m_arrInExtNameStubs.GetSize();         
         iInNewNr = iInCount / m_arrInExtNameStubs.GetSize() + 1;   
         if (iInRun > 0)
             iIn--;
         iInNewNr = iInNewNr>=iIn ? iInNewNr : iIn;                 
         // ++**++ JD extInParam <--
         for( i=0; i<iSY; i+=2)
         {
            if( ChkFlag(EF_EXT_INP) )
            {  // do not use copy constructor! (m_lConnectId, m_wFlag, ..)
               //str.Format("%s%d", strHdIn, iIn++);// ++**++ JD extInParam
               str.Format("%s%d",m_arrInExtNameStubs.GetAt(iInRun), iInNewNr);// ++**++ JD extInParam
               pPin = new CPin(str, pPi->m_strIVal, pPi->m_strType, 
                  m_Origin + CPoint(0, oldSize.cy+1 + i), CSize(PIN_LENGTH,0), CPinFlag(PF_EXT_INP)+CPinFlag(PF_IN));
               if( pPin ) 
               {
                  m_Con.Add(pPin);
                  pPin->m_Entry = m_pPinTable->InsertPin(this, pPin->m_Pos);
                  ASSERT(pPin->m_Entry);
               }
               // ++**++ JD extInParam -->
               iInRun++;                                
               iInRun %= m_arrInExtNameStubs.GetSize(); 
               if (iInRun==0)
                   iInNewNr++;  
               // ++**++ JD extInParam <--
            }
            if( ChkFlag(EF_EXT_OUTP) )
            {
               str.Format("%s%d", strHdOut, iOut++);
               pPin = new CPin(str, pPo->m_strIVal, pPo->m_strType, 
                  m_Origin + CPoint(m_Size.cx, oldSize.cy+1 + i), CSize(-PIN_LENGTH,0), CPinFlag(PF_EXT_OUTP)+CPinFlag(PF_OUT));
               if( pPin ) 
               {
                  m_Con.Add(pPin);
                  pPin->m_Entry = m_pPinTable->InsertPin(this, pPin->m_Pos);
                  ASSERT(pPin->m_Entry);
               }
            }
         }
      } else {  
         // delete existing connect pins
         for( i=0; i<m_Con.GetSize(); i++)
         {
            pPin = m_Con.GetAt(i);
            ASSERT_VALID(pPin);
            if( pPin && 
               (pPin->m_Flag.Chk(PF_EXT_INP) || pPin->m_Flag.Chk(PF_EXT_OUTP)) &&
               (pPin->m_Pos.y - m_Origin.y > m_Size.cy) )
            { 
               ASSERT(pPin->m_Entry);
               m_pPinTable->DeletePin(this, pPin->m_Entry, pPin->m_Pos);
               m_Con.RemoveAt(i);  
               delete pPin; 
               i--;  // next pin is now at the same index !
            }
         }
      }
   }
   return bRes;
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
bool CElemFblk::GetText(int iElemNr, int iPos, int iLen, CString& str)
{
   str = _T("");

   if (iElemNr < -2)
   {
      return (false);
   }
   else if (iElemNr == -2)
   {
      str = (m_Inst.Str()).Mid(iPos, iLen);
      return (true);
   }
   else if (iElemNr == -1)
   {
      str = (m_Name.Str()).Mid(iPos, iLen);
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
void CElemFblk::ReplaceSelText(int iElemNr, int iPos, int iLen, LPCTSTR pstr)
{
   if ((iElemNr < -2) || (iElemNr == -1))
   {
      return;
   }
   else if (iElemNr == -2)
   {
      (m_Inst.StrRef()).Delete(iPos, iLen);
      (m_Inst.StrRef()).Insert(iPos, pstr);
      return;
   }
   else 
   {
      CElem::ReplaceSelText( iElemNr, iPos, iLen, pstr);
      return;
   }
}

/*func*------------------------------------------------------------------------
  set name of fb/fct/pro
  in :pszName - the new name
  out:-
-----------------------------------------------------------------------------*/
void CElemFblk::SetName(LPCTSTR pszName)  
{ 
   m_Name = pszName; 
}

/*func*------------------------------------------------------------------------
  get name of fb/fct/pro
  in :-
  out:the name
-----------------------------------------------------------------------------*/
const CString& CElemFblk::GetName() 
{ 
   return m_Name.Str(); 
}

/*func*------------------------------------------------------------------------
  set name of fb/fct/pro
  in :pszName - the new name
  out:-
-----------------------------------------------------------------------------*/
void CElemFblk::SetInst(LPCTSTR pszInst)  
{ 
   m_Inst = pszInst; 
   m_Inst.m_Flag += TF_EDITABLE;
}

/*func*------------------------------------------------------------------------
  get name of fb/fct/pro
  in :-
  out:the name
-----------------------------------------------------------------------------*/
const CString& CElemFblk::GetInst() 
{ 
   return m_Inst.Str(); 
}

/*func*------------------------------------------------------------------------
  serialize the element to/from stream
  in :ar - the archive
  out:-
-----------------------------------------------------------------------------*/
void CElemFblk::Serialize(CArchive & ar)
{
   CElem::Serialize(ar);

	if (ar.IsStoring())
	{
      ar << m_Inst << m_Name;
	}
	else
	{ 
      ar >> m_Inst >> m_Name;
      RecoverENOStatus();
	  GetInExtNameStubs();	// 21.11.07 SIS: crash after undo extend fb
	}
}

/*func*------------------------------------------------------------------------
  serialize the element to/from XML stream
  in :ar - the archive
  out:-
-----------------------------------------------------------------------------*/
void CElemFblk::SerializeXML(CXMLArchive &ar)
{
//   long    lID;
   CPoint  p;
   CSize   minSize, maxSize, size;
   CString str;

   if(ar.IsStoring())
   {
      // all CXMLArchive fire HRESULT exceptions, which must be cought!!!
      try {            // name and create node here !!
         if( GetType() == ET_FB ) 
         {
            ar.SetStringAttributeUTF8( XML_4GR_ATTR_NAME, m_Inst.Str());
         }
         ar.SetStringAttributeUTF8( XML_4GR_ATTR_TYPENAME,  m_Name.Str());
//       ar.SetAttribute( XML_4GR_ATTR_VERSIONID, m_strVersionId);

         CElem::SerializeXML(ar);
         SerializeXMLCon(ar);

//       ar.SetAttribute( XML_4GR_ATTR_HIDEPINNAMES, _T(""));
//       EMMBEDDEDIMAGE
      }        
      catch (HRESULT hr)
      {
         TRACE(_T("Cannot save to XML; hr=0x%08X\n"), hr);
         throw hr;   // fire upstairs...        
      }    
   } else {
      try {   
         if( GetType() == ET_FB ) 
         {
            ar.GetAttribute( XML_4GR_ATTR_NAME, m_Inst.StrRef());
/*
            // update max id number
            str.LoadString(IDS_GR_NAMEMASK_FB);
            if( _stscanf( (LPCTSTR)m_Inst, (LPCTSTR)str, &lID ) == 1) 
            {
               GRU_SetID(ID_INST, lID);
            }
*/
         }
         ar.GetAttribute( XML_4GR_ATTR_TYPENAME,  m_Name.StrRef());
//       ar.GetAttribute( XML_4GR_ATTR_VERSIONID, m_strVersionId);

         CElem::SerializeXML(ar);
         SerializeXMLCon(ar);

         // additional some reconstructions ..
         CheckPins();           // generate pairs of inout pin's !
         GetInExtNameStubs(); // ++**++ JD extInParam 
         if( GetType() == ET_FB ) 
         {
            minSize = CSize(MIN_FB_WIDTH,   CalcMinHeight());  // calc min. size of element
         } else {
            minSize = CSize(MIN_FBLK_WIDTH, CalcMinHeight()); 
         }
         SetMinSize( minSize );                             // set min. size ..
         maxSize = CSize(MAX_YGRID - 4,  CalcMaxHeight());  // calc min. size of element
         SetMaxSize( maxSize );                             // set min. size ..
         ar.GetAttribute( XML_4GR_ATTR_SIZE, p);         // get element size from XML
         if( (p.x < m_MinSize.cx) ||                 
             (p.y < m_MinSize.cy) )                      // check for 'unlayouted' element
         {
            size = CSize(CalcOptWidth(), GetSize().cy);  // display all identifier
            SetFlag(EF_NEW_LAYOUT);
            SetSize (size);                              // set new (min.) size of element
            SetPinPos ();                                // place (layout) the pin's
         }
         SetPinSize();                                   // reconstruct the pin length ..
         SetAttr();                                      // reconstruct some flags ..
         RecoverENOStatus();


//       ar.GetAttribute( XML_4GR_ATTR_HIDEPINNAMES, str);
//       EMMBEDDEDIMAGE
      }
      catch (HRESULT hr)
      {
         TRACE(_T("Cannot load from XML; hr=0x%08X\n"), hr);
         throw hr;   // fire upstairs...
      }   
   }
}

/*func*------------------------------------------------------------------------
  adjust the ENO-status depending on the pin names
  (since the bool value is not stored, it must be reconstructed)
  in :-
  out:-
-----------------------------------------------------------------------------*/
void CElemFblk::RecoverENOStatus()
{
   m_bShowENO = false;
   for(int i=0; i<m_Con.GetSize(); i++)
   {
      if(m_Con[i]->m_Name == CText("ENO"))
      {
         m_Con[i]->m_Flag.Set(PF_LADDER);
         m_bShowENO = true;
      }else if(m_Con[i]->m_Name == CText("EN"))
      {
         m_Con[i]->m_Flag.Set(PF_LADDER);
         m_bShowENO = true;
      }
   }
}

/*func*------------------------------------------------------------------------
  returns an extended rect at the top to ensure proper rect-invalidation
  after size changes (auto-EN/ENO insertion/removal)
  in :-
  out:-
-----------------------------------------------------------------------------*/
void CElemFblk::GetBoundingRect(CRect& r, int iGrid)
{
   CElem::GetBoundingRect(r,iGrid);
   r.top -= 2*iGrid;
}


/*func*------------------------------------------------------------------------
  decides wether spacing can be made (from the view of this element)
  in :-
  out:-
-----------------------------------------------------------------------------*/
bool CElemFblk::IsSpaceable(CPoint At, CPoint Delta, bool bLabelClip)
{
   if(At.y > m_Origin.y && At.y < m_Origin.y+m_Size.cy ) // restrictions only in y-dimension
   {
      if(Delta.x<0 || Delta.y <0)                        // deletion not allowed
      {
         return CElem::IsSpaceable(At,Delta,bLabelClip);
      }

      bool bCanMove = true;
      bool bCanStay = true;
      int NCon = m_Con.GetSize();

      for(int iAct=0; iAct<NCon; iAct++)
      {
         int y  = m_Con[iAct]->m_Pos.y;
         int x  = m_Con[iAct]->m_Pos.x;
         
         if(m_Con[iAct]->GetConnects() > 1 )               // value of m_iConnets
         {
            for(int i=0; i<NCon; i++)
            {
               int yy = m_Con[i]->m_Pos.y;

               if( x == m_Con[i]->m_Pos.x && (i!=iAct))
               {
                  // element move test
                  if( (y<At.y) && (yy<y) && (yy>=y-Delta.y) )
                  {
                     bCanMove = false;
                     if(!bCanStay)
                        return false;
                  }

                  if( (y>=At.y) && (yy>y) && (yy<=y+Delta.y) )
                  {
                     bCanStay = false;
                     if(!bCanMove)
                        return false;
                  }
               }
            }
         }
      }
   }
   return true;
}


/*func*------------------------------------------------------------------------
  performs element spacing
  in :-
  out: paPin is filled with the coordinates of disrupted pins 
-----------------------------------------------------------------------------*/
bool CElemFblk::Spacing( CPoint At, CPoint Delta, CPosArray* paPin, bool bLabelClip)
{
   bool bCanMove = true;
   bool bCanStay = true;
   int NCon = m_Con.GetSize();

   if(Delta.y>0)
   {
      bCanStay &= (At.y > m_Origin.y);
      bCanMove &= (At.y < m_Origin.y + m_Size.cy);

      for(int iAct=0; iAct<NCon; iAct++)
      {
         int y  = m_Con[iAct]->m_Pos.y;
         int x  = m_Con[iAct]->m_Pos.x;
      
         if(m_Con[iAct]->GetTemp() > 1 )
         {
            for(int i=0; i<NCon; i++)
            {
               int yy = m_Con[i]->m_Pos.y;

               if( x == m_Con[i]->m_Pos.x && (i!=iAct))
               {
                  // element Move-Test
                  if( (y < At.y) && (yy<y) && (yy>=y-Delta.y) )
                  {
                     bCanMove = false;
                     if(!bCanStay)
                        return false;
                  }

                  if( (y >= At.y) && (yy>y) && (yy<=y+Delta.y) )
                  {
                     bCanStay = false;
                     if(!bCanMove)
                        return false;
                  }
               }
            }
         }
      }

      if(bCanStay)
      {
         if(paPin)
         {
            for(int iPin=0; iPin<NCon; iPin++)
            {
               if (m_Con[iPin]->m_Pos.y >= At.y && 
                   m_Con[iPin]->GetTemp() > 1 )                            // saved value of m_iConnets
               {
                  paPin->AddPoint(m_Con[iPin]->m_Pos);
               }
            }
         }
         return true;
      }else
         if(bCanMove)
         {
            if(paPin)
            {
               for(int iPin=0; iPin<NCon; iPin++)
               {
                  CPin* p = m_Con[iPin];
                  if ((p->m_Pos.y < At.y) && 
                      (p->GetTemp() > 1 ) )                                // saved value of m_iConnets
                  {
                     paPin->AddPoint(m_Con[iPin]->m_Pos);
                  }
               }
            }
            Resize(0,Delta.y,0,0);
            return true;
         }
   }else
   {
      if(Delta.x>0)
      {
         if(At.x<=m_Origin.x)
         {
            Resize(Delta.x,0,0,0);  // move it
         }else
         {
            if(At.x <= m_Origin.x + m_Size.cx )
            {
               if(paPin)
               {
                  for(int iPin=0; iPin<m_Con.GetSize(); iPin++)
                  {
                     int x = m_Con[iPin]->m_Pos.x;
                     if ((x>=At.x) && 
                        (m_Con[iPin]->GetTemp() > 1 ))                    // saved value of m_iConnets
                     {
                        paPin->AddPoint(m_Con[iPin]->m_Pos);
                     }
                  }
               }
            }
         }
      }else
      {
         ASSERT(Delta.y < 0 || Delta.x < 0);
         return CElem::Spacing(At,Delta,paPin,bLabelClip);
      }
   }
   return false;
}

/*func*------------------------------------------------------------------------
  drop in elem in line - break line
  in :elem to break (line)
  out:true  - 
      false - 
-----------------------------------------------------------------------------*/
bool  CElemFblk::CheckDropIn( CElem* pE)        // requires a normalized line to check against
{
   CPin* pPin;
   bool bOK = false;

   ASSERT(pE);
   if ((pE != NULL)                   && 
       (pE->GetType()    == ET_LINE)  &&    // check if a line
       (pE->GetSize().cy == 0))             // check if <INSIDE a HOIZONTAL LINE>
   {
      if ((pE->GetOrigin().x<=m_Origin.x)     &&      // inside (!!!requires a normalized line)
          (pE->GetOrigin().x+pE->GetSize().cx >= m_Origin.x+m_Size.cx) )
      {
         for ( int i=0; !bOK && i<m_Con.GetSize(); i++ )
         {
            pPin = m_Con.GetAt(i);
            ASSERT(pPin);
            if (pPin)
            {  // there are always two ENO/EN-Pins in opposite !!!
               // both pins must have the same y-value and the ladder-flag !!
               if((pPin->m_Pos.y == pE->GetOrigin().y) &&
                   pPin->m_Flag.Chk(PF_LADDER))
               {
                  bOK = true; 
                  // it is possible to stop after the first pin ..
               }
            }
         }
      } else // not strictly inside: 2nd chance: a single and unconnected line point below the element
      {
         TPinEntry *pPE = NULL;

         if (pE->GetOrigin().x > m_Origin.x)
         {
            m_pPinTable->Lookup(pE->GetOrigin(), pPE);
         } 
         else if(pE->GetOrigin().x+pE->GetSize().cx < m_Origin.x+m_Size.cx)
         {
            m_pPinTable->Lookup( pE->GetOrigin() + pE->GetSize(), pPE);
         }
         if (pPE && pPE->m_iConnects == 1)
         {
            for(int i=0; !bOK && (i< m_Con.GetSize()); i++)
            {
               pPin = m_Con.GetAt(i);
               ASSERT(pPin);
               if (pPin)
               {
                  if((pPin->m_Pos.y == pE->GetOrigin().y) &&
                      pPin->m_Flag.Chk(PF_LADDER))
                  {
                     bOK = true; 
                     // it is possible to stop after the first pin ..
                  }
               }
            }
         }
      }
   }
   return bOK;
}

/*func*------------------------------------------------------------------------
  drop out elem from line - close line
  in :-
  out:true  - can drop out
      false - can not drop out
-----------------------------------------------------------------------------*/
bool  CElemFblk::CheckDropOut()
{
   return true;
}

void CElemFblk::OnAutoENO()
{
   bool bPrev = m_bShowENO;
   if(m_pContain == NULL)
   {
      m_bShowENO = m_bENOdefault;
   }else
   {
      m_bShowENO = m_pContain->ChkFlag(EF_LDACTION);
   }

   CPin* pPin = NULL;
   
   if (bPrev && !m_bShowENO)         // switch off
   {
      m_Origin.y  += 2;
      m_Size.cy   -= 2;
      m_MinSize.cy-= 2;
      
      for (int i=0; i<m_Con.GetSize(); i++)
      {
         pPin = m_Con[i];
         if(pPin->m_Flag.Chk(PF_LADDER))
         {
            ASSERT(pPin->m_Entry);
            m_pPinTable->DeletePin(this, pPin->m_Entry, pPin->m_Pos);
            m_Con.RemoveAt(i);  
            delete pPin; 
            pPin = NULL;
            i--;  // next pin is now at the same index !
         }
      }
      SetAttr();
      SetFlag(EF_UPDATE);
      
   } else if (!bPrev && m_bShowENO)   // switch on
   {
      m_Origin.y  -= 2;    
      m_Size.cy   += 2;
      m_MinSize.cy+= 2;

      CPoint P;
      CPinFlag PF = PF_LADDER;
      if (GetType()==ET_FUNCTION)
      {
         P = m_Origin + CPoint(0,3);
      }
      else
      {
         P = m_Origin + CPoint(0,5);
      }
      pPin = new CPin(_T("EN"), _T(""), _T("ANY"), 
         P, CSize(1,0), PF + CPinFlag(PF_IN));
      m_Con.Add(pPin);
      pPin->m_Entry = m_pPinTable->InsertPin(this, pPin->m_Pos);
      
      
      P.x += m_Size.cx;
      pPin = new CPin(_T("ENO"), _T(""), _T("ANY"), 
         P, CSize(-1,0), PF + CPinFlag(PF_OUT));
      m_Con.Add(pPin);
      pPin->m_Entry = m_pPinTable->InsertPin(this, pPin->m_Pos);
      SetAttr();
      SetFlag(EF_UPDATE);
   }
}

// ++**++ JD extInParam --->
void CElemFblk::GetInExtNameStubs()
{
    CPin *pPin;
    int i;

    m_arrInExtNameStubs.RemoveAll();

    for( i=0; i<m_Con.GetSize(); i++ )
    {
        pPin = m_Con.GetAt(i);
        if (pPin->m_Flag.Chk(PF_EXT_INP))
        {
            CString strName = pPin->m_Name;
            CString strHd;
            for(int iN = strName.GetLength()-1; iN>0; iN-- )
            {
                if( !_istdigit(strName[iN]) ) break;
            }
            strHd = strName.Left(iN+1);
            if (m_arrInExtNameStubs.GetSize()>0 && strHd.Compare(m_arrInExtNameStubs.GetAt(0)) == 0)
            {
                break;
            }
            m_arrInExtNameStubs.Add(strHd);
        }

    }
}
// ++**++ JD extInParam <---



IMPLEMENT_SERIAL(CElemFblk, CElem, SERIAL_VERSION)

//////////////////////////////////////////////////////////////////////
// CElemProc Class
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CElemProc, CElemFblk, SERIAL_VERSION)

//////////////////////////////////////////////////////////////////////
// CElemFunc Class
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CElemFunc, CElemFblk, SERIAL_VERSION)

//////////////////////////////////////////////////////////////////////
// CElemFB Class
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CElemFB, CElemFblk, SERIAL_VERSION)
