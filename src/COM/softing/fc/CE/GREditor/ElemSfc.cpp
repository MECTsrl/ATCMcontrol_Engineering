

/*doc*=======================================================================*/
/*inc*=======================================================================*/

#include "stdafx.h"

#include "GrEditor.h"
#include "GrEditorBase.h"
#include "GrEditorDoc.h"
#include "GrEditorView.h"
#include "GrEditorUtil.h"
#include "ElemSfc.h"
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
CElemStep::CElemStep()
{ 
   CSize size(4,4);

   SetType(ET_STEP);
   SetMinSize (size);
   m_bInitial = FALSE;
   SetName(_T(""));
}

/*func*------------------------------------------------------------------------
  create a new step
  created step is selected !!
  in :Origin  - insertion point 
      Size    - startup size of element 
      strName - name-symbol of step
  out:pointer to new step element
-----------------------------------------------------------------------------*/
CElemStep::CElemStep(CPoint Origin, CSize Size, LPCTSTR pszName, 
   BOOL bInitial)
{
   CString  strPN, strPT;
   CPin    *pPin;
   CSize    size(4,4);

   SetType(ET_STEP);
   SetMinSize (size);
   m_bInitial = bInitial;

   SetOrigin(Origin);
   Size.cx &= ~1;
   Size.cy &= ~1;

   SetSize  (Size);
   SetName  (pszName);

   pPin = new CPin(_T(""), _T(""), _T("ANY"), 
      Origin + CPoint( (Size.cx/2+1)&0xFFFFFFFE, 0 ), CSize(0,PIN_LENGTH), CPinFlag(PF_CENTER_X)+CPinFlag(PF_IN));
   if( pPin ) m_Con.Add(pPin);

   // step flag is ***.X or ***.T 
   pPin = new CPin(_T(""), _T(""), _T("ANY"), 
      Origin + CPoint( Size.cx, (Size.cy/2+1)&0xFFFFFFFE ), CSize(0,0), CPinFlag(PF_CENTER_Y)+CPinFlag(PF_OUT));
   if( pPin ) m_Con.Add(pPin);

   pPin = new CPin(_T(""), _T(""), _T("ANY"), 
      Origin + CPoint( (Size.cx/2+1)&0xFFFFFFFE, Size.cy ), CSize(0,-PIN_LENGTH), CPinFlag(PF_CENTER_X)+CPinFlag(PF_OUT));
   if( pPin ) m_Con.Add(pPin);

}

/*func*------------------------------------------------------------------------
  destructor
-----------------------------------------------------------------------------*/
CElemStep::~CElemStep()
{
}

/*func*------------------------------------------------------------------------
  draw the step (SFC)
  in :pDC   - pointer to device context
      pView - pointer to document (data)
      iGrid - actual display resolution
  out:-
-----------------------------------------------------------------------------*/
void CElemStep::Draw(CDC * pDC, CGrEditorView * pView, int iGrid)
{
   CRect    r;
   CSize    size;
   CString  str;
   int      i;
   PEN_TYPE PenType;
   bool     bScreen = !pDC->IsPrinting();

   if (!pView)
   {
      return;
   }
   // prepare drawing
   PenType = ChkFlag(EF_COMMENT) ? PE_COMMENT : PE_NORMAL;
   pDC->SelectObject(GDIHelper::GetPen  (bScreen, PenType  ));
   if (pView->IsManualMode())
   {
      pDC->SelectObject(GDIHelper::GetBrush(bScreen, BT_BACKGD_SFC));
   } else {
      pDC->SelectObject(GDIHelper::GetBrush(bScreen, BT_BACKGD));
   }

   // draw the connect point
   DrawPins(pDC, iGrid);
   // draw the step name
   GetBodyRect(r, iGrid);
   r.right++;   // one pixel more ..
   if (pView->IsStepActive(m_Origin))
   {
      pDC->SelectObject(GDIHelper::GetBrush(bScreen, BT_ACTIVE_SFC));
   } else {
      if (pView->IsManualMode())
      {
         pDC->SelectObject(GDIHelper::GetBrush(bScreen, BT_ELEMBK_SFC));
      } else {
         pDC->SelectObject(GDIHelper::GetBrush(bScreen, BT_ELEMBK));
      }
   }
   pDC->Rectangle(r);
   if( m_bInitial )
   {
      i = max(2, iGrid / 4);
      r.DeflateRect(i,i);
      pDC->Rectangle(r);
      r.InflateRect(i,i);
   }
   if (pView->IsManualMode())
   {
      pDC->SelectObject(GDIHelper::GetBrush(bScreen, BT_BACKGD_SFC));
   } else {
      pDC->SelectObject(GDIHelper::GetBrush(bScreen, BT_BACKGD));
   }
   //r.bottom--; 
//   r.right--;
   r.left++; 
   //r.top++;
   // draw element icon 
// DrawIcon( pDC, pView, m_Origin, IDB_ICONSTEP, iGrid);

   int nChars = 0;
   int nChPos = 0;
   int yOfs   = r.top;
   SearchDat *pSD = &pView->m_SearchDat;
   
   if( (pSD->m_FindCur   == GetOrigin()) &&
       (pSD->m_iFindPos  >= 0          ) )
   {
      m_Name.Draw(pDC, r, iGrid, GetColorType(), pSD, 0);
   }else
   {
      m_Name.Draw(pDC, r, iGrid, GetColorType(), NULL, 0);
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
void CElemStep::GetBodyRect(CRect &r, int iGrid)
{
   ASSERT(iGrid > 0);

   if( iGrid > 0 )
   {
      GetRect(r, iGrid);
      r.top    += iGrid*PIN_LENGTH; 
      r.bottom -= iGrid*PIN_LENGTH;
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
BOOL CElemStep::FindText(SearchDat& SD)
{
// step has 1 string for search (m_strName)
#define  MAX_ELEM   -1

   CString  str;
   LPCTSTR  psz;
   int      i;

   i = (int)_tcslen(SD.m_strFind);

   if( SD.m_bNext )  // forward search
   {
      if( SD.m_iFindElem < MAX_ELEM )
      {
         SD.m_iFindElem = MAX_ELEM;
      }

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
   } else // backward search
   {
      if( SD.m_iFindElem >= 0 ) 
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
  get the data identifier of the element
  in :p     - grip position for test
      start - position for identify (origin)
  out:identifier of the element
-----------------------------------------------------------------------------*/
CText& CElemStep::GetTextAt(CPoint /*p*/, CPoint &start, int /*iGrid*/)
{
   start = m_Origin;
   return m_Name;
}

/*func*------------------------------------------------------------------------
  get the text space in elem
  in :rect - placeholder for result
  out:rect - filled with text grid coords
-----------------------------------------------------------------------------*/
void CElemStep::GetTextRect(int iTxtIdx, CRect &rect, int iGrid)
{ 
   GetBodyRect(rect,iGrid);
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
bool CElemStep::GetText(int iElemNr, int iPos, int iLen, CString& str)
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
  resize and/or move element with connector pins
  in :dx - delta x for m_Origin
      dy - delta y for m_Origin
      sx - delta x for m_Size
      sy - delta y for m_Size
  out:-
-----------------------------------------------------------------------------*/
bool CElemStep::Resize(int dx, int dy, int sx, int sy)
{
   bool    bRes;

   if ((sx != 0) || (sy != 0))
   {
      if (dx == 0) dx = -sx; 
      sx = 2*sx;
   }
   bRes = CElem::Resize( dx, dy, sx, sy);
   return bRes;
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
void CElemStep::ReplaceSelText(int iElemNr, int iPos, int iLen, LPCTSTR pstr)
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
  set name of step
  in :pszName - the new name
  out:-
-----------------------------------------------------------------------------*/
void CElemStep::SetName(LPCTSTR pszName) 
{ 
   m_Name = pszName; 
   m_Name.m_Flag += TF_EDITABLE;
   m_Name.m_Flag += TF_WRAP;
//   m_Name.m_Flag += TF_MULTILINE;
}

/*func*------------------------------------------------------------------------
  get name of step
  in :-
  out:the name
-----------------------------------------------------------------------------*/
const CString& CElemStep::GetName() 
{ 
   return m_Name.Str(); 
}

/*func*------------------------------------------------------------------------
  resize element to optimal width for text length
  in :-
  out:dorigin - delta origin  (necessary for reset size and position if 
      dsize   - delta size     overlap-check fails)
      true  - element is resized
      false - element is unchanged
-----------------------------------------------------------------------------*/
bool CElemStep::SetAutoSize(CPoint& dorigin, CSize& dsize) 
{ 
   int i = (((m_bInitial ? 4 : 0) +
      CText::GetIdentLen((CString&)m_Name) + 1) & 0xFFFFFFFE);
   if ( i < m_MinSize.cx ) i = m_MinSize.cx;
   if ( i > m_MaxSize.cx ) i = m_MaxSize.cx;
   dsize.cx  = i - m_Size.cx;
   dsize.cy  = 0;
   dorigin.x = -(int)((dsize.cx / 2) & 0xFFFFFFFE);
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
  serialize the element to/from stream
  in :ar - the archive
  out:-
-----------------------------------------------------------------------------*/
void CElemStep::Serialize(CArchive & ar)
{
   CString str;
   long    lID;

   CElem::Serialize(ar);

	if (ar.IsStoring())
	{
      ar << m_Name << m_bInitial;
   }
	else
	{ 
      ar >> m_Name >> m_bInitial;
      // update max id number
      str.LoadString(IDS_GR_NAMEMASK_STEP);
      if( _stscanf( (LPCTSTR)m_Name, (LPCTSTR)str, &lID ) == 1) 
      {
         CGRTool::SetID(CGRTool::ID_STEP, lID);
      }
	}
}

/*func*------------------------------------------------------------------------
  set pin attributes (centre_x/centre_y, ..)
  in :-
  out:-
-----------------------------------------------------------------------------*/
void CElemStep::SetPinAttr(void)
{
   CPin *pPin;
   int   i;

   for( i=0; i<m_Con.GetSize(); i++ )
   {
      if( (pPin = m_Con.GetAt(i)) != NULL ) 
      {
         if( pPin->m_Pos.x == m_Origin.x + m_Size.cx ) 
         {
            pPin->m_Flag.Set(PF_CENTER_Y);
#ifdef GR_AUTO_CORRECTION
            //  we must repair an old error at step flag (in -> out)
            pPin->m_Flag.Clr(PF_IN);
            pPin->m_Flag.Set(PF_OUT);
#endif
         } 
         else if( pPin->m_Pos.y == m_Origin.y ) 
         {
            pPin->m_Flag.Set(PF_CENTER_X);
         }
         else if( pPin->m_Pos.y == m_Origin.y + m_Size.cy ) 
         {
            pPin->m_Flag.Set(PF_CENTER_X);
         }
      }
   }
}

/*func*------------------------------------------------------------------------
  serialize the element to/from XML stream
  in :ar - the archive
  out:-
-----------------------------------------------------------------------------*/
void CElemStep::SerializeXML(CXMLArchive &ar)
{
   CString str;
   long    lID;
   long    l;

   if(ar.IsStoring())
   {
      // all CXMLArchive fire HRESULT exceptions, which must be cought!!!
      try {            // name and create node here !!
         CElem::SerializeXML(ar);  // origin, size, selected, comment
         SerializeXMLCon(ar);

         ar.SetStringAttributeUTF8( XML_4GR_ATTR_NAME, (CString&)m_Name);

         if( m_bInitial ) ar.SetAttribute( XML_4GR_ATTR_INITSTEP, 1L);

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
         SerializeXMLCon(ar);
	      SetPinAttr();

         ar.GetAttribute( XML_4GR_ATTR_NAME, (CString&)m_Name);
         str.LoadString(IDS_GR_NAMEMASK_STEP);
         if( _stscanf( (LPCTSTR)m_Name, (LPCTSTR)str, &lID ) == 1) 
         {
            CGRTool::SetID(CGRTool::ID_STEP, lID);
         }

         if( ar.GetAttribute( XML_4GR_ATTR_INITSTEP, l) && l==1) m_bInitial = TRUE; 
         else                                                    m_bInitial = FALSE; 

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
  serialize the pins from/to XML archiv
  postprocessing of flags for reading pins
  in :ar - the archive
  out:-
-----------------------------------------------------------------------------*/
void CElemStep::SerializeXMLCon(CXMLArchive &ar)
{
   CPin *pPin;

   CElem::SerializeXMLCon(ar);

   if(ar.IsStoring())
   {
      ;
   } else {
      // layout for Step pins:
      //  top input 1 grid long
      //  bottom output 1 grid long
      for( int i=0; i<m_Con.GetSize(); i++ )
      {
         pPin = m_Con.GetAt(i);
         ASSERT(pPin);
         if( pPin )
         {
            if( pPin->m_Pos.y == m_Origin.y ) 
            {
               pPin->m_Size = CSize(0,1);
               pPin->m_Flag.Clr(PF_HIDDEN);
            } else if( pPin->m_Pos.y == m_Origin.y + m_Size.cy) {
               pPin->m_Size = CSize(0,-1);
               pPin->m_Flag.Clr(PF_HIDDEN);
            }
#ifdef GR_AUTO_CORRECTION
            ((CString&)pPin->m_Name).Empty();
#endif
         }
      }
   }
}

bool  CElemStep::JoinTest(TDirection Dir, CElem* pE)
{
   if(Dir==DIR_E)
   {
      if( pE->GetType() == ET_CONOUT  ||
          pE->ChkFlag(EF_RUBBLINE)  ||
          (pE->GetType() == ET_LINE)&&   // line may be normalized or not
          (pE->GetSize().cy   == 0)  &&
          (SearchConn(pE->GetOrigin()) || (SearchConn(pE->GetOrigin()+pE->GetSize())) )
        )
        return true;
   }else
   {
      if(Dir == DIR_W)
      {
         if((pE->GetType() == ET_COIL  || pE->GetType() == ET_CONTACT) &&
            (pE->m_Con[0]->m_Pos.y > GetOrigin().y+GetSize().cy))
         return true;
      }
   }
   return false;
}


bool  CElemStep::CheckDropIn( CElem* pE)        // requires a normalized line to check against
{
   CPin* pPin;
   bool bOK = false;

   ASSERT(pE);

   if ((pE != NULL)                  &&
       (pE->GetType()    == ET_LINE) &&      // check if a line
       (pE->GetSize().cx == 0))              // check if a VERTICAL line
   {
      if ((pE->GetOrigin().y<=m_Origin.y)     &&    // strictly inside (!!!requires a normalized line)
          (pE->GetOrigin().y+pE->GetSize().cy >= m_Origin.y+m_Size.cy) )
      {
         bOK = true;
         for( int i=0; bOK && i<m_Con.GetSize(); i++ )
         {
            pPin = m_Con.GetAt(i);
            ASSERT(pPin);
            if( pPin )
            {
               if (pPin->m_Pos.y == m_Origin.y ||           // pin at top|bottom (not the right pin!)
                   pPin->m_Pos.y == m_Origin.y + m_Size.cy
                  ) 
               {
                  bOK &= (pPin->m_Pos.x == pE->GetOrigin().x);
               }
            }
         }
      } else // not strictly inside: 2nd chance: a single and unconnected line point below the transition
      {
         TPinEntry *pPE = NULL;

         if( pE->GetOrigin().y > m_Origin.y)
            m_pPinTable->Lookup(pE->GetOrigin(),pPE);
         else if(pE->GetOrigin().y+pE->GetSize().cy < m_Origin.y+m_Size.cy)
            m_pPinTable->Lookup( pE->GetOrigin() + pE->GetSize(), pPE);

         if(pPE && pPE->m_iConnects == 1)
         {
            for(int i=0; !bOK && (i< m_Con.GetSize()); i++)
            {
               if(m_Con[i] && m_Con[i]->m_Pos.x == pE->GetOrigin().x)
                  bOK = true;
            }
         }
      }
   }
   return bOK;
}

bool  CElemStep::CheckDropOut()
{
   return true;
}

bool CElemStep::Spacing(CPoint At, CPoint Delta, CPosArray* paPin, bool bLabelClip)
{
   int  i1;                // min element extension in raster coords
   int  i2;                // max element extension in raster coords
   bool bDone = false;     // true if any movement done
   bool bRes  = true;
   int  d;                 // ammount of movement

   ASSERT( Delta.x == 0 || Delta.y == 0);                   // either x or y direction (not both)

   if( (d = Delta.x) > 0 )
   {
      i2 = max( m_Origin.x , m_Origin.x + m_Size.cx );
      i1 = i2;
      for(int idx=0; idx<m_Con.GetSize(); idx++)            // attend the connected pins
      {
         if(m_Con[idx]->GetTemp() > 1 &&                    // saved value of m_iConnets
            m_Con[idx]->m_Pos.x < i1)
         {
            i1 = m_Con[idx]->m_Pos.x;
         }
      }

      if(i1==i2)
        i1 = min( m_Origin.x , m_Origin.x + m_Size.cx );

      if( At.x > i1 )                                       // not to move the entire element
      {  // not moved
         if( At.x <= i2)                                    // point inside or at right border
         {
            for(int iPin=0; iPin<m_Con.GetSize(); iPin++)
            {
               CPin* pPin = m_Con[iPin];
               if ((pPin->m_Pos.x >= At.x) && 
                   (pPin->GetTemp() > 1 ))                  // saved value of m_iConnets
               {
                  paPin->AddPoint(m_Con[iPin]->m_Pos);
               }
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

      if( At.y > i1 )                                       // not to move the entire element
      {
         if( At.y <= i2 )                                   // don't move if on border
         {
            int yMid = i1;

            for (int idx=0; idx<m_Con.GetSize(); idx++)     // attend the connected pins
            {
               if (m_Con[idx]->GetTemp() > 1 &&             // saved value of m_iConnets
                   m_Con[idx]->m_Pos.y < i2)
               {
                  yMid = max(yMid,m_Con[idx]->m_Pos.y);
               }
            }

            if(At.y<=yMid)
            { // move
               for(int iPin=0; iPin<m_Con.GetSize(); iPin++)
               {
                  if ((m_Con[iPin]->m_Pos.y < At.y) && 
                      (m_Con[iPin]->GetTemp() > 1 ))        // saved value of m_iConnets
                  {
                     paPin->AddPoint(m_Con[iPin]->m_Pos);
                  }
               }
               Resize(0,d,0,0);
               bDone = true;
            }else
            { // keep
               for(int iPin=0; iPin<m_Con.GetSize(); iPin++)
               {
                  if ((m_Con[iPin]->m_Pos.y >= At.y) && 
                      (m_Con[iPin]->GetTemp() > 1 ))
                  {
                     paPin->AddPoint(m_Con[iPin]->m_Pos);
                  }
               }
               bDone = true;
            }
         }
      }else
      {                                                     // element is strictly right
         bRes &= Resize(0,d,0,0);                           //    shift it
         bDone = true;
      }
   }

   if( (d = Delta.x) < 0)
   {
      bDone = CElem::Spacing(At,Delta,paPin, bLabelClip);
   }
   
   if( (d = Delta.y) < 0)
   {
      bDone = CElem::Spacing(At,Delta,paPin, bLabelClip);
   }

   ASSERT(bRes);
   return bDone;
}

IMPLEMENT_SERIAL(CElemStep, CElem, SERIAL_VERSION)

//////////////////////////////////////////////////////////////////////
// ElemSfc.cpp: implementation of the CElemTran class.
//////////////////////////////////////////////////////////////////////

CElemTran::CElemTran()
{
   CSize size(6,4);

   SetType(ET_TRANSITION);

   SetMinSize(size);
   m_MaxSize.cy = 4;
   SetName(_T(""));
}

/*func*------------------------------------------------------------------------
  create a new transition
  created transition is selected !!
  in :Origin  - insertion point 
      Size    - startup size of element 
      strName - name-symbol of transition
  out:pointer to new transition element
-----------------------------------------------------------------------------*/
CElemTran::CElemTran(CPoint Origin, CSize Size, LPCTSTR pszName)
{
   CPin  *pPin;
   CSize  size(6,4);

   SetType(ET_TRANSITION);

   SetMinSize(size);
   m_MaxSize.cy = 4;

   SetOrigin(Origin);
   SetSize  (Size);
   SetName  (pszName);

   pPin = new CPin(_T(""), _T(""), _T("ANY"),
      Origin + CPoint(2,0), CSize(0,2), PF_IN);
   if( pPin ) m_Con.Add(pPin);

   pPin = new CPin(_T(""), _T(""), _T("ANY"),
      Origin + CPoint(0,2), CSize(0,0), PF_IN);
   if( pPin ) m_Con.Add(pPin);

   pPin = new CPin(_T(""), _T(""), _T("ANY"),
      Origin + CPoint(2,4), CSize(0,-2), PF_OUT);
   if( pPin ) m_Con.Add(pPin);
}

CElemTran::~CElemTran()
{
}

/*func*------------------------------------------------------------------------
  draw the transition (SFC)
  in :pDC   - pointer to device context
      pView - pointer to document (data)
      iGrid - actual display resolution
  out:-
-----------------------------------------------------------------------------*/
void CElemTran::Draw(CDC * pDC, CGrEditorView * pView, int iGrid)
{
   CRect    r;
   CSize    size;
   CString  str;
   int      i, x1, x2, y;
   PEN_TYPE PenType;
   bool     bScreen = !pDC->IsPrinting();

   if (!pView)
   {
      return;
   }

   if (pView->IsTransitionForced (m_Origin)) SetFlag(EF_SFC_FORCED );
   else                                      ClrFlag(EF_SFC_FORCED );
   if (pView->IsTransitionBlocked(m_Origin)) SetFlag(EF_SFC_BLOCKED);
   else                                      ClrFlag(EF_SFC_BLOCKED);

   // prepare drawing
   if (pView->IsManualMode())
   {
      pDC->SelectObject(GDIHelper::GetBrush(bScreen, BT_BACKGD_SFC));
   } else {
      pDC->SelectObject(GDIHelper::GetBrush(bScreen, BT_BACKGD));
   }

   // draw the connect point
   if     (ChkFlag(EF_COMMENT    )) PenType = PE_COMMENT;
   else                                PenType = PE_NORMAL;
   pDC->SelectObject(GDIHelper::GetPen  (bScreen, PenType  ));

   DrawPins(pDC, iGrid);

   // draw the transition line
   if     (ChkFlag(EF_COMMENT    )) PenType = PE_COMMENT;
   else if(ChkFlag(EF_SFC_FORCED )) PenType = PE_FORCED;
   else if(ChkFlag(EF_SFC_BLOCKED)) PenType = PE_BLOCKED;
   else                                PenType = PE_NORMAL;
   pDC->SelectObject(GDIHelper::GetPen  (bScreen, PenType  ));

   x1 =  m_Origin.x     *iGrid;
   x2 = (m_Origin.x + 4)*iGrid;
   y  = (m_Origin.y + 2)*iGrid;
   for( i=-1; i<2; i++)
   {
      pDC->MoveTo( x1, y+i ); pDC->LineTo( x2, y+i );
   }

   // draw element icon 
// DrawIcon( pDC, pView, m_Origin, IDB_ICONTRAN, iGrid);
   // draw element name
   GetBodyRect(r, iGrid);
   if (pView->IsManualMode())
   {
      pDC->FillRect(r, GDIHelper::GetBrush(bScreen, BT_ELEMBK_SFC));
   } else {
      pDC->FillRect(r, GDIHelper::GetBrush(bScreen, BT_ELEMBK));
   }

   SearchDat *pSD = &pView->m_SearchDat;
   if((pSD->m_FindCur   == GetOrigin()) &&
      (pSD->m_iFindPos  >= 0          ) )
   {
      m_Name.Draw(pDC, r, iGrid, GetColorType(), pSD, 0);
   }else
   {
      m_Name.Draw(pDC, r, iGrid, GetColorType(), NULL, 0);
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
void CElemTran::GetBodyRect(CRect &r, int iGrid)
{
   ASSERT(iGrid > 0);

   if( iGrid > 0 )
   {
      GetRect(r, iGrid);
      r.top    +=   iGrid*PIN_LENGTH;
      r.bottom -=   iGrid*PIN_LENGTH;
      r.left   += 4*iGrid;
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
BOOL CElemTran::FindText(SearchDat& SD)
{
// transition has 1 string for search (m_strName)
#define  MAX_ELEM   -1

   CString  str;
   LPCTSTR  psz;
   int      i;

   i = (int)_tcslen(SD.m_strFind);
   if( SD.m_bNext )
   {
      if( SD.m_iFindElem < MAX_ELEM ) 
      {
         SD.m_iFindElem = MAX_ELEM;
      }
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
   } else {
      if( SD.m_iFindElem >= 0 ) 
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
  get the data identifier of the element
  in :p     - grip position for test
      start - position for identify (origin)
  out:identifier of the element
-----------------------------------------------------------------------------*/
CText& CElemTran::GetTextAt(CPoint /*p*/, CPoint &start, int /*iGrid*/)
{
   start = m_Origin;
   return m_Name;
}

/*func*------------------------------------------------------------------------
  get the text space in elem
  in :rect - placeholder for result
  out:rect - filled with text grid coords
-----------------------------------------------------------------------------*/
void CElemTran::GetTextRect(int iTxtIdx, CRect &rect, int iGrid)
{ 
   rect.SetRect((m_Origin.x         + 4             )*iGrid,
                (m_Origin.y             + PIN_LENGTH)*iGrid,
                (m_Origin.x + m_Size.cx             )*iGrid,
                (m_Origin.y + m_Size.cy - PIN_LENGTH)*iGrid);
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
bool CElemTran::GetText(int iElemNr, int iPos, int iLen, CString& str)
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
void CElemTran::ReplaceSelText(int iElemNr, int iPos, int iLen, LPCTSTR pstr)
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
  set name of transition
  in :pszName - the new name
  out:-
-----------------------------------------------------------------------------*/
void CElemTran::SetName(LPCTSTR pszName) 
{ 
   m_Name = pszName; 
   m_Name.m_Flag += TF_EDITABLE;
}

/*func*------------------------------------------------------------------------
  get name of transition
  in :-
  out:the name
-----------------------------------------------------------------------------*/
const CString& CElemTran::GetName() 
{ 
   return m_Name.Str();
}

/*func*------------------------------------------------------------------------
  resize element to optimal width for text length
  in :-
  out:dorigin - delta origin  (necessary for reset size and position if 
      dsize   - delta size     overlap-check fails)
      true  - element is resized
      false - element is unchanged
-----------------------------------------------------------------------------*/
bool CElemTran::SetAutoSize(CPoint& dorigin, CSize& dsize) 
{ 
   int i = ((4 + CText::GetIdentLen((CString&)m_Name) + 1) & 0xFFFFFFFE);
   if ( i < m_MinSize.cx ) i = m_MinSize.cx;
   if ( i > m_MaxSize.cx ) i = m_MaxSize.cx;
   dsize.cx  = i - m_Size.cx;
   dsize.cy  = 0;
   dorigin.x = 0; 
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
  serialize the element to/from stream
  in :ar - the archive
  out:-
-----------------------------------------------------------------------------*/
void CElemTran::Serialize(CArchive & ar)
{
   CString str;
   long    lID;

   CElem::Serialize(ar);

	if (ar.IsStoring())
	{
      ar << m_Name;
   }
	else
	{ 
      ar >> m_Name;
      // update max id number
      str.LoadString(IDS_GR_NAMEMASK_TRAN);
      if( _stscanf( (LPCTSTR)m_Name, (LPCTSTR)str, &lID ) == 1) 
      {
         CGRTool::SetID(CGRTool::ID_TRAN, lID);
      }
	}
}

/*func*------------------------------------------------------------------------
  serialize the element to/from XML stream
  in :ar - the archive
  out:-
-----------------------------------------------------------------------------*/
void CElemTran::SerializeXML(CXMLArchive &ar)
{
   CString str;
   long    lID;

   if(ar.IsStoring())
   {
      // all CXMLArchive fire HRESULT exceptions, which must be cought!!!
      try {            // name and create node here !!
         CElem::SerializeXML(ar);  // origin, size, selected, comment
         SerializeXMLCon(ar);

         ar.SetStringAttributeUTF8( XML_4GR_ATTR_NAME, (CString&)m_Name);
      }        
      catch (HRESULT hr)
      {
         TRACE(_T("Cannot save to XML; hr=0x%08X\n"), hr);
         throw hr;   // fire upstairs...        
      }    
   } else {
      try {   
         CElem::SerializeXML(ar);  // origin, size, selected, comment
         SerializeXMLCon(ar);

         ar.GetAttribute( XML_4GR_ATTR_NAME, (CString&)m_Name);
         str.LoadString(IDS_GR_NAMEMASK_TRAN);
         if( _stscanf( (LPCTSTR)m_Name, (LPCTSTR)str, &lID ) == 1) 
         {
            CGRTool::SetID(CGRTool::ID_TRAN, lID);
         }
      }
      catch (HRESULT hr)
      {
         TRACE(_T("Cannot load from XML; hr=0x%08X\n"), hr);
         throw hr;   // fire upstairs...
      }   
   }
}

/*func*------------------------------------------------------------------------
  serialize the pins from/to XML archiv
  postprocessing of flags for reading pins
  in :ar - the archive
  out:-
-----------------------------------------------------------------------------*/
void CElemTran::SerializeXMLCon(CXMLArchive &ar)
{
   CPin *pPin;

   CElem::SerializeXMLCon(ar);

   if(ar.IsStoring())
   {
      ;
   } else {
      // layout for Transition pins:
      //  top input 1 grid long
      //  bottom output 1 grid long
      for( int i=0; i<m_Con.GetSize(); i++ )
      {
         pPin = m_Con.GetAt(i);
         ASSERT(pPin);
         if( pPin )
         {
            if( pPin->m_Pos.y == m_Origin.y ) 
            {
               pPin->m_Size = CSize(0,2);
               pPin->m_Flag.Clr(PF_HIDDEN);
            } else if( pPin->m_Pos.y == m_Origin.y + m_Size.cy) {
               pPin->m_Size = CSize(0,-2);
               pPin->m_Flag.Clr(PF_HIDDEN);
            }
#ifdef GR_AUTO_CORRECTION
            ((CString&)pPin->m_Name).Empty();
#endif
         }
      }
   }
}

bool  CElemTran::CheckDropIn( CElem* pE)     // requires a normalized line to check against
{
   CPin* pPin;
   bool bOK = false;

   ASSERT(pE != NULL);

   if((pE != NULL)               &&    
      (pE->GetType() == ET_LINE) && 
      (pE->GetSize().cx==0))           // check if a VERTICAL LINE
   {
      if((pE->GetOrigin().y<=m_Origin.y)     &&    // strictly inside? (!!!requires a normalized line)
         (pE->GetOrigin().y+pE->GetSize().cy >= m_Origin.y+m_Size.cy) )
      {
         bOK = true;
         for( int i=0; bOK && i<m_Con.GetSize(); i++ )
         {
            pPin = m_Con.GetAt(i);
            ASSERT(pPin);
            if( pPin )
            {
               if( pPin->m_Pos.y == m_Origin.y ||           // pin at top|bottom
                  pPin->m_Pos.y == m_Origin.y + m_Size.cy
                  ) 
               {
                  bOK &= (pPin->m_Pos.x == pE->GetOrigin().x);
               }
            }
         }
      }else // not strictly inside: 2nd chance: a single and unconnected line point below the transition
      {
         TPinEntry *pPE = NULL;

         if( pE->GetOrigin().y > m_Origin.y)
            m_pPinTable->Lookup(pE->GetOrigin(),pPE);
         else if(pE->GetOrigin().y+pE->GetSize().cy < m_Origin.y+m_Size.cy)
            m_pPinTable->Lookup( pE->GetOrigin() + pE->GetSize(), pPE);

         if(pPE && pPE->m_iConnects == 1)
         {
            for(int i=0; !bOK && (i< m_Con.GetSize()); i++)
            {
               if(m_Con[i] && m_Con[i]->m_Pos.x == pE->GetOrigin().x)
                  bOK = true;
            }
         }
      }
   }
   return bOK;
}

bool  CElemTran::CheckDropOut()
{
   return true;
}

bool CElemTran::Spacing(CPoint At, CPoint Delta, CPosArray* paPin, bool bLabelClip)
{
   int  i1;                // min element extension in raster coords
   int  i2;                // max element extension in raster coords
   bool bDone = false;     // true if any movement done
   bool bRes  = true;
   int  d;                 // ammount of movement

   ASSERT( Delta.x == 0 || Delta.y == 0);                   // either x or y direction (not both)

   if( (d = Delta.x) > 0 )
   {
      i1 = min( m_Origin.x , m_Origin.x + m_Size.cx );
      i2 = i1; //max( m_Origin.x , m_Origin.x + m_Size.cx );

      for(int idx=0; idx<m_Con.GetSize(); idx++)            // attend the connected pins
      {
         if ((m_Con[idx]->GetTemp() > 1 ) && 
             (m_Con[idx]->m_Pos.x > i2))
         {
            i2 = m_Con[idx]->m_Pos.x;
         }
      }

      if( At.x > i1 )                                       // not to move the entire element
      {
         if( At.x <= i2)                                    // point inside or at right border
         {
            for(int iPin=0; iPin<m_Con.GetSize(); iPin++)
            {
               CPin* pPin = m_Con[iPin];

               if ((pPin->m_Pos.x < At.x) && 
                   (pPin->GetTemp() > 1))
               {
                  paPin->AddPoint(pPin->m_Pos);
               }
            }
            Resize(d,0,0,0);
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


      if( At.y > i1 )                                       // not to move the entire element
      {
         if( At.y <= i2 )                                   // don't move if on border
         {
            int yMid = i1;

            for(int idx=0; idx<m_Con.GetSize(); idx++)            // attend the connected pins
            {
               if ((m_Con[idx]->GetTemp() > 1) && 
                   (m_Con[idx]->m_Pos.y<i2))
               {
                  yMid = max(yMid,m_Con[idx]->m_Pos.y);
               }
            }

            if(At.y<=yMid)
            { // move
               for (int iPin=0; iPin<m_Con.GetSize(); iPin++)
               {
                  if ((m_Con[iPin]->m_Pos.y < At.y) && 
                      (m_Con[iPin]->GetTemp() > 1))
                  {
                     paPin->AddPoint(m_Con[iPin]->m_Pos);
                  }
               }
               Resize(0,d,0,0);
               bDone = true;
            } else
            { // keep
               for (int iPin=0; iPin<m_Con.GetSize(); iPin++)
               {
                  if ((m_Con[iPin]->m_Pos.y >= At.y) && 
                      (m_Con[iPin]->GetTemp() > 1))
                  {
                     paPin->AddPoint(m_Con[iPin]->m_Pos);
                  }
               }
               bDone = true;
            }
         }
      }else
      {                                                     // element is strictly right
         bRes &= Resize(0,d,0,0);                           //    shift it
         bDone = true;
      }
   }

   if( (d = Delta.x) < 0)
   {
      bDone = CElem::Spacing(At,Delta,paPin, bLabelClip);
   }
   
   if( (d = Delta.y) < 0)
   {
      bDone = CElem::Spacing(At,Delta,paPin, bLabelClip);
   }

   ASSERT(bRes);
   return bDone;
}

IMPLEMENT_SERIAL(CElemTran, CElem, SERIAL_VERSION)

//////////////////////////////////////////////////////////////////////
// ElemSfc.cpp: implementation of the CElemAct Class.
//////////////////////////////////////////////////////////////////////

const LPCTSTR CElemAct::m_strQual[QT_LAST] = { 
            _T(""  ),   // QT_none 
            _T("N" ),   // QT_N   
            _T("R" ),   // QT_R   
            _T("S" ),   // QT_S   
            _T("L" ),   // QT_L   
            _T("D" ),   // QT_D   
            _T("P" ),   // QT_P   
            _T("SD"),   // QT_SD  
            _T("DS"),   // QT_DS  
            _T("SL"),   // QT_SL
            _T("P0"),   // QT_P0  
            _T("P1") }; // QT_P1  

UINT CElemAct::m_NextID = 1;

CElemAct::CElemAct()
{
   CElemActInit();
}

/*func*------------------------------------------------------------------------
  create a new transition
  created transition is selected !!
  in :Origin  - insertion point 
      Size    - startup size of element 
      strName - name-symbol of transition
  out:pointer to new transition element
-----------------------------------------------------------------------------*/
CElemAct::CElemAct (CPoint Origin, CSize Size, LPCTSTR pszName, QUAL_TYPE eQualifier)
{
   CElemActInit();

   SetOrigin(Origin);
   SetSize  (Size);

   m_eQualifier = eQualifier;
   SetName (pszName);
}

CElemAct::~CElemAct()
{
}

/*func*------------------------------------------------------------------------
  draw the action (SFC)
  in :pDC   - pointer to device context
      pView - pointer to document (data)
      iGrid - actual display resolution
  out:-
-----------------------------------------------------------------------------*/
void CElemAct::CElemActInit(void)
{
   CSize size(8,2);

   m_OriginOffs = CPoint(0,1);
   SetMinSize (size);
   m_eQualifier = QT_none;
   m_Time       = (LPCTSTR) _T("T#0s");
   m_iTClen     = 0;
   SetName  (_T(""));
   SetSTtext(_T(""));
   m_ID = m_NextID++;
}

/*func*------------------------------------------------------------------------
  draw the action (SFC)
  in :pDC   - pointer to device context
      pView - pointer to document (data)
      iGrid - actual display resolution
  out:-
-----------------------------------------------------------------------------*/
void CElemAct::Draw(CDC * pDC, CGrEditorView * pView, int iGrid)
{
   CRect    r;
   CSize    size;
   CString  str;
   int      i;
//   int      i2;
//   int      iLines;
//   int      iOffs; 
   PEN_TYPE PenType;
   bool     bTimed, bScreen = !pDC->IsPrinting();
   
   if (!pView)
   {
      return;
   }
   ASSERT((GetType() == ET_ACTION) || (GetType() == ET_ACTIONBLOCK));

   if( pView->IsActionForced (m_Origin)) SetFlag(EF_SFC_FORCED);
   else                                  ClrFlag(EF_SFC_FORCED);
   if( pView->IsActionBlocked(m_Origin)) SetFlag(EF_SFC_BLOCKED);
   else                                  ClrFlag(EF_SFC_BLOCKED);

   if     (ChkFlag(EF_COMMENT    )) PenType = PE_COMMENT;
   else if(ChkFlag(EF_SFC_FORCED )) PenType = PE_FORCED;
   else if(ChkFlag(EF_SFC_BLOCKED)) PenType = PE_BLOCKED;
   else                                PenType = PE_NORMAL;

   pDC->SelectObject(GDIHelper::GetPen  (bScreen, PenType  ));
   if (pView->IsManualMode())
   {
      pDC->SelectObject(GDIHelper::GetBrush(bScreen, BT_BACKGD_SFC));
   } else {
      pDC->SelectObject(GDIHelper::GetBrush(bScreen, BT_BACKGD));
   }

   // draw the connect point(s)
   DrawPins(pDC, iGrid);

   // draw element body
   GetRect(r, iGrid);
   // action(block) is expanded !
   SearchDat *pSD = &pView->m_SearchDat;

   if( r.Height() > 2*iGrid )
   {  
      // drawn by its individual STEditorView
      // at ST-action draw the ST ..
      if (/*false && */ChkFlag(EF_STACTION))
      {
         r.top += 2*iGrid;
         r.left+= 2;

         if (pView->IsManualMode())
         {
            pDC->FillRect(r, GDIHelper::GetBrush(bScreen, BT_BACKGD_SFC));
         } else {
            pDC->FillRect(r, GDIHelper::GetBrush(bScreen, BT_BACKGD));
         }
#if 0
         if((pSD->m_FindCur   == GetOrigin()) &&
            (pSD->m_iFindElem == -2)          &&
            (pSD->m_iFindPos  >= 0 )            )
         {
            m_STtext.Draw(pDC, r, iGrid, GetColorType(), pSD, 0);
         }else
         {
            m_STtext.Draw(pDC, r, iGrid, GetColorType(), NULL, 0);
         }
#endif
         r.left-= 2;
         r.top -= 2*iGrid;
      }
      pDC->MoveTo(r.left,  r.top + 2*iGrid);
      pDC->LineTo(r.left,  r.bottom);
      pDC->LineTo(r.right, r.bottom);
      pDC->LineTo(r.right, r.top + 2*iGrid);
      r.bottom = r.top + 2*iGrid;
   }
   r.bottom++; r.right++;
   if (pView->IsActionActive(m_Origin))
   {
      pDC->SelectObject(GDIHelper::GetBrush(bScreen, BT_ACTIVE_SFC));
   } else {
      if (pView->IsManualMode())
      {
         pDC->SelectObject(GDIHelper::GetBrush(bScreen, BT_ELEMBK_SFC));
      } else {
         pDC->SelectObject(GDIHelper::GetBrush(bScreen, BT_ELEMBK));
      }
   }
   pDC->Rectangle(r);

   // draw element icon 
// DrawIcon( pDC, pView, m_Origin, IDB_ICONSTEP, iGrid);

   // draw qualifier and time constant
   if( GetType() == ET_ACTIONBLOCK ) 
   {  // check for time constant ..
      if( (m_eQualifier == QT_L ) || (m_eQualifier == QT_D ) ||
          (m_eQualifier == QT_SD) ||
          (m_eQualifier == QT_DS) || (m_eQualifier == QT_SL) )
      {
         bTimed = true;  
      } else {
         bTimed = false; 
      }
      // draw qualifier
      ASSERT((m_eQualifier >= QT_none) && (m_eQualifier < QT_LAST));
      CText Text(m_strQual[m_eQualifier]);
      GetQualRect(r, iGrid);
      Text.Draw(pDC, r, iGrid, GetColorType(), NULL, 0);

      if( bTimed )
      {  // draw time constant
         size = pDC->GetTextExtent((CString&)m_Time);
         m_iTClen = (size.cx + iGrid-1) / iGrid;
         size = pDC->GetTextExtent((CString&)m_Name);
         i  = (size.cx + iGrid-1) / iGrid;
         if( (i + m_iTClen + Q_WIDTH) > m_Size.cx )
         {  // must reduce tc length
            m_iTClen = ((m_Size.cx - Q_WIDTH) * 3) / 10;
         }
         GetTimeRect(r, iGrid);
         m_Time.Draw(pDC, r, iGrid, GetColorType(), NULL, 0);
      } else 
      {
         m_iTClen = 0; 
      }
      pDC->MoveTo(r.right, r.top   );
      pDC->LineTo(r.right, r.bottom);
   }

   // draw element name
   if( !((CString&)m_Name).IsEmpty() )
   {
      GetNameRect(r, iGrid);

      if((pSD->m_FindCur   == GetOrigin()) &&
         (pSD->m_iFindElem == -1)          &&
         (pSD->m_iFindPos  >= 0          ) )
      {
         m_Name.Draw(pDC, r, iGrid, GetColorType(), pSD, 0);
      }else
      {
         m_Name.Draw(pDC, r, iGrid, GetColorType(), NULL, 0);
      }
   }

   /* draw indicator     TBD !!!
   pDC->SetTextAlign( TA_CENTER | TA_TOP );
   i = r.left;
   r.left = r.right + 2*iGrid;
   size = pDC->GetTextExtent(m_strIndicator);
   TextOut( pDC, r.left + r.Width()/2 - size.cx/2, r.top, m_strIndicator, -2, pView );
   r.left = i;   */

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
  calculate the rectangle for the name in logical units
  in :r     - to calculated rectangle
      iGrid - actual display resolution
  out:-
-----------------------------------------------------------------------------*/
void CElemAct::GetNameRect(CRect &r, int iGrid)
{
   ASSERT(iGrid > 0);

   if( iGrid > 0 )
   {
      GetRect(r, iGrid);
      r.bottom = r.top + 2*iGrid;
      if( GetType() == ET_ACTIONBLOCK )
      {
         r.left += (Q_WIDTH + m_iTClen)*iGrid + 2; // space to frame line
      }
   }
}

/*func*------------------------------------------------------------------------
  calculate the rectangle for the qualifier in logical units
  in :r     - to calculated rectangle
      iGrid - actual display resolution
  out:-
-----------------------------------------------------------------------------*/
void CElemAct::GetQualRect(CRect &r, int iGrid)
{
   ASSERT(iGrid > 0);

   if( iGrid > 0 )
   {
      GetRect(r, iGrid);
      r.bottom = r.top + 2*iGrid;
      r.right = r.left + Q_WIDTH*iGrid;
   }
}

/*func*------------------------------------------------------------------------
  calculate the rectangle for the time in logical units
  in :r     - to calculated rectangle
      iGrid - actual display resolution
  out:-
-----------------------------------------------------------------------------*/
void CElemAct::GetTimeRect(CRect &r, int iGrid)
{
   ASSERT(iGrid > 0);

   if( iGrid > 0 )
   {
      GetRect(r, iGrid);
      r.bottom = r.top + 2*iGrid;
      r.left += Q_WIDTH*iGrid;
      r.right = r.left + m_iTClen*iGrid;
   }
}

/*func*------------------------------------------------------------------------
  get the body rect without pin range for select operations
  in :r        - place holder for rect
      iGrid    - actual zoom
  out:-
-----------------------------------------------------------------------------*/
void CElemAct::GetBodyRect(CRect& r, int iGrid)
{
   ASSERT(iGrid > 0);

   if( iGrid > 0 )
   {
      GetRect(r, iGrid);
      if (!ChkFlag(EF_STACTION))
      {
         r.bottom = r.top + 2*iGrid;
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
BOOL CElemAct::FindText(SearchDat& SD)
{
// action has 2 string for search (m_strName, m_STtext)
//  no search in m_Time, m_strQual in this release
   CString  str;
   LPCTSTR  psz;
   int      i, MAX_ELEM;

   if (ChkFlag(EF_STACTION) && (m_Size.cy > 2))
   {
      MAX_ELEM = -2;
   } else {
      MAX_ELEM = -1;
   }

   i = (int)_tcslen(SD.m_strFind);

   if( SD.m_bNext )  // forward search
   {
      if( SD.m_iFindElem < MAX_ELEM )
      {
         SD.m_iFindElem = MAX_ELEM;
      }

      for ( ; SD.m_iFindElem < 0; SD.m_iFindElem++ ) 
      {
         if        ( SD.m_iFindElem == -2 ) 
         {
            str = (CString&)m_STtext;
         } else if ( SD.m_iFindElem == -1 ) 
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
   } else // backward search
   {
      if( SD.m_iFindElem >= 0 ) 
      {
         SD.m_iFindElem = -1;
      }

      for( ; SD.m_iFindElem >= MAX_ELEM; SD.m_iFindElem-- )
      {
         if        ( SD.m_iFindElem == -2 ) 
         {
            str = (CString&)m_STtext;
         } else if ( SD.m_iFindElem == -1 ) 
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
}

/*func*------------------------------------------------------------------------
  get the data identifier of the element
  in :p     - grip position for test
      start - position for identify (origin)
  out:identifier of the element
-----------------------------------------------------------------------------*/
CText& CElemAct::GetTextAt(CPoint p, CPoint &start, int iGrid)
{
   CText  &text=CElemContain::GetTextAt(p, start, iGrid);
   CRect   r;

   if (GetType() == ET_ACTIONBLOCK )   // name 
   {
      r.SetRect( (m_Origin.x + Q_WIDTH + m_iTClen)*iGrid, (m_Origin.y    )*iGrid, 
                 (m_Origin.x + m_Size.cx         )*iGrid, (m_Origin.y + 2)*iGrid);
   } else {
      r.SetRect( (m_Origin.x            )*iGrid, (m_Origin.y    )*iGrid, 
                 (m_Origin.x + m_Size.cx)*iGrid, (m_Origin.y + 2)*iGrid);
   }
   if (r.PtInRect(p)) 
   {
      start = m_Origin;
      return m_Name;
   }
   // ST action ?
   if (ChkFlag(EF_STACTION))
   {
      r.SetRect( (m_Origin.x            )*iGrid, (m_Origin.y + 2        )*iGrid, 
                 (m_Origin.x + m_Size.cx)*iGrid, (m_Origin.y + m_Size.cy)*iGrid);
      if (r.PtInRect(p)) 
      {
         start = m_Origin + CPoint(0,2);
         return m_STtext;
      }
   }

   return text;
}

/*func*------------------------------------------------------------------------
  get the text space in elem
  in :rect - placeholder for result
  out:rect - filled with text grid coords
-----------------------------------------------------------------------------*/
void CElemAct::GetTextRect(int iTxtIdx, CRect &rect, int iGrid)
{ 
   if (iTxtIdx == -3)   // qualifier
   {
      if (GetType() == ET_ACTIONBLOCK )
      {
         rect.SetRect((m_Origin.x                     )*iGrid,
                      (m_Origin.y                     )*iGrid,
                      (m_Origin.x + Q_WIDTH + m_iTClen)*iGrid,
                      (m_Origin.y + 2                 )*iGrid);
         return;
      }
   } 
   else if (iTxtIdx == -2)
   {
      if (ChkFlag(EF_STACTION))
      {
         rect.SetRect((m_Origin.x            )*iGrid, 
                      (m_Origin.y + 2        )*iGrid,
                      (m_Origin.x + m_Size.cx)*iGrid, 
                      (m_Origin.y + m_Size.cy)*iGrid);
         return;
      }
   } 
   else if (iTxtIdx == -1)
   {
      if (GetType() == ET_ACTIONBLOCK )
      {
         rect.SetRect((m_Origin.x + Q_WIDTH + m_iTClen)*iGrid,
                      (m_Origin.y                     )*iGrid,
                      (m_Origin.x + m_Size.cx         )*iGrid,
                      (m_Origin.y + 2                 )*iGrid);
         return;
      } else {
         rect.SetRect((m_Origin.x            )*iGrid,
                      (m_Origin.y            )*iGrid,
                      (m_Origin.x + m_Size.cx)*iGrid, 
                      (m_Origin.y + 2        )*iGrid);
         return;
      }
   }
   rect.SetRectEmpty();
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
bool CElemAct::GetText(int iElemNr, int iPos, int iLen, CString& str)
{
   str = _T("");

   if      (iElemNr == -2)
   {
      if (ChkFlag(EF_STACTION) && (m_Size.cy > 2))
      {
         str = ((CString&)m_STtext).Mid(iPos, iLen);
         return (true);
      }
   }
   else if (iElemNr == -1)
   {
      str = ((CString&)m_Name).Mid(iPos, iLen);
      return (true);
   }
   else if (iElemNr >= 0)
   {
      return CElemContain::GetText( iElemNr, iPos, iLen, str);
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
void CElemAct::ReplaceSelText(int iElemNr, int iPos, int iLen, LPCTSTR pstr)
{
   if (iElemNr < -2)
   {
      return;
   }
   else if (iElemNr == -2)
   {
      if (ChkFlag(EF_STACTION) && (m_Size.cy > 2))
      {
         ((CString&)m_STtext).Delete(iPos, iLen);
         ((CString&)m_STtext).Insert(iPos, pstr);
      }
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
      CElemContain::ReplaceSelText( iElemNr, iPos, iLen, pstr);
      return;
   }
}

/*func*------------------------------------------------------------------------
  set name of action
  in :pszName - the new name
  out:-
-----------------------------------------------------------------------------*/
void CElemAct::SetName(LPCTSTR pszName) 
{ 
   m_Name = pszName; 
   m_Name.m_Flag += TF_EDITABLE;
}

/*func*------------------------------------------------------------------------
  get name of action
  in :-
  out:the name
-----------------------------------------------------------------------------*/
const CString& CElemAct::GetName() 
{ 
   return m_Name.Str(); 
}

/*func*------------------------------------------------------------------------
  set body text of the ST action
  in :pszName - the new name
  out:-
-----------------------------------------------------------------------------*/
void CElemAct::SetSTtext(LPCTSTR pszName) 
{ 
   m_STtext = pszName; 
   m_STtext.m_Flag += TF_EDITABLE;
   m_STtext.m_Flag += TF_EDIT_OE;
   m_STtext.m_Flag += TF_MULTILINE;
   m_STtext.m_Flag += TF_LEFT;
}

/*func*------------------------------------------------------------------------
  get body text from the ST action
  in :-
  out:the name
-----------------------------------------------------------------------------*/
CText& CElemAct::GetSTtext() 
{ 
   return m_STtext;
}

/*func*------------------------------------------------------------------------
  serialize the element to/from stream
  in :ar - the archive
  out:-
-----------------------------------------------------------------------------*/
void CElemAct::Serialize(CArchive & ar)
{
   CString str;
   long    lID;
   int     i;

   CElemContain::Serialize(ar);

	if (ar.IsStoring())
	{  
      i = (int)m_eQualifier;
      ar << i << m_Name << m_Time << m_STtext;
   }
	else
	{ 
      ar >> i >> m_Name >> m_Time >> m_STtext;
      m_eQualifier = (QUAL_TYPE)i;
      // update max id number
      str.LoadString(IDS_GR_NAMEMASK_ACT);
      if( _stscanf( (LPCTSTR)m_Name, (LPCTSTR)str, &lID ) == 1) 
      {
         CGRTool::SetID(CGRTool::ID_ACT, lID);
      }
	}
}

/*func*------------------------------------------------------------------------
  serialize the element to/from XML stream
  in :ar - the archive
  out:-
-----------------------------------------------------------------------------*/
void CElemAct::SerializeXML(CXMLArchive &ar)
{
   CString str;
   long    lID;

   const CString STR_LDACTION = _T("LDACTION");

   ASSERT((GetType() == ET_ACTION) || (GetType() == ET_ACTIONBLOCK));
   if(ar.IsStoring())
   {
      // all CXMLArchive fire HRESULT exceptions, which must be cought!!!
      try {            // name and create node here !!
         ar.SetStringAttributeUTF8( XML_4GR_ATTR_NAME, (CString&)m_Name );

         CElemContain::SerializeXML(ar);  // origin, size, selected, comment

         if( GetType() == ET_ACTIONBLOCK ) 
         {
            SerializeXMLCon(ar);

            ar.MoveDown( XML_4GR_ELEM_ACTQUAL );
            ar.SetAttribute( XML_4GR_ATTR_QUALTYPE, m_strQual[m_eQualifier]);
            if( (m_eQualifier == QT_L ) || (m_eQualifier == QT_D ) ||
                (m_eQualifier == QT_SD) ||
                (m_eQualifier == QT_DS) || (m_eQualifier == QT_SL) )
            {
               ar.SetAttribute( XML_4GR_ATTR_QUALTIME, (CString&)m_Time);
            }
            ar.MoveUp();        
         }

         if (ChkFlag(EF_STACTION))
         {
            ar.MoveDown( XML_4GR_ELEM_STACTION );
            ar.SetCData( (CString&)m_STtext );
            ar.MoveUp();
         }else if(ChkFlag(EF_LDACTION))
         {
            ar.SetAttribute( STR_LDACTION, "1");
         }
      }        
      catch (HRESULT hr)
      {
         TRACE(_T("Cannot save to XML; hr=0x%08X\n"), hr);
         throw hr;   // fire upstairs...        
      }    
   } else {
      try {   
         CString str;

         CElemContain::SerializeXML(ar);  // origin, size, selected, comment

         if( GetType() == ET_ACTIONBLOCK ) 
         {
            SerializeXMLCon(ar);

            if( ar.MoveDown( XML_4GR_ELEM_ACTQUAL ) )
            {
               ar.GetAttribute( XML_4GR_ATTR_QUALTYPE, str);
               for ( m_eQualifier=QT_none; 
                     m_eQualifier<QT_LAST; 
                     m_eQualifier = (QUAL_TYPE)((int)m_eQualifier + 1) )
               {
                  if( m_strQual[m_eQualifier] == str ) break;
               }
               if( m_eQualifier == QT_LAST ) 
               {
                  TRACE(_T("CElemActBlk: invalid qualtype\n"));
                  throw E_FAIL; 
               }
               if( (m_eQualifier == QT_L ) || (m_eQualifier == QT_D ) ||
                   (m_eQualifier == QT_SD) ||
                   (m_eQualifier == QT_DS) || (m_eQualifier == QT_SL) )
               {
                  ar.GetAttribute( XML_4GR_ATTR_QUALTIME, (CString&)m_Time);
               }
               ar.MoveUp();
            } else {
               TRACE(_T("CElemActBlk: missing actqual\n"));
               throw E_FAIL; 
            }
         }

         ar.GetAttribute( XML_4GR_ATTR_NAME, (CString&)m_Name);
         str.LoadString(IDS_GR_NAMEMASK_ACT);
         if( _stscanf( (LPCTSTR)m_Name, (LPCTSTR)str, &lID ) == 1) 
         {
            CGRTool::SetID(CGRTool::ID_ACT, lID);
         }

         if( ar.MoveDown( XML_4GR_ELEM_STACTION ) )
         {
            SetFlag(EF_STACTION);
            ar.GetCData( (CString&)m_STtext );
            ar.MoveUp();
         }else if(ar.GetAttribute( STR_LDACTION, str))
         {
            SetFlag(EF_LDACTION);
         }
      }
      catch (HRESULT hr)
      {
         TRACE(_T("Cannot load from XML; hr=0x%08X\n"), hr);
         throw hr;   // fire upstairs...
      }   
   }
}

/*func*------------------------------------------------------------------------
  serialize the pins from/to XML archiv
  postprocessing of flags for reading pins
  in :ar - the archive
  out:-
-----------------------------------------------------------------------------*/
void CElemAct::SerializeXMLCon(CXMLArchive &ar)
{
   CPin *pPin;

   CElemContain::SerializeXMLCon(ar);

   if(ar.IsStoring())
   {
      ;
   } else {
      ;
#ifdef GR_AUTO_CORRECTION
      for( int i=0; i<m_Con.GetSize(); i++ )
      {
         pPin = m_Con.GetAt(i);
         ASSERT(pPin);
         if( pPin )
         {
            ((CString&)pPin->m_Name).Empty();
         }
      }
#endif
   }
}

/*func*------------------------------------------------------------------------
  test overlap of element pE with this action/block
  in :pE - test element
  out:true  - ok, no overlap
      false - overlapping!!
-----------------------------------------------------------------------------*/
bool CElemAct::JoinTest(TDirection /*Dir*/, CElem* pE)
{ 
   if (!ChkFlag(EF_STACTION))
   {
      if( (pE->GetType() != ET_ACTION )     &&  // disabled elements (SFC)
          (pE->GetType() != ET_ACTIONBLOCK) &&
          (pE->GetType() != ET_STEP)        &&
          (pE->GetType() != ET_ALTDIV)      &&
          (pE->GetType() != ET_SIMDIV)      &&
          (pE->GetType() != ET_TRANSITION)  &&
           
          (ChkFlag(EF_LDACTION)             ||  // optional enabled elements (LD)
            (pE->GetType() != ET_POWERRAIL) &&
            (pE->GetType() != ET_CONTACT)   &&
            (pE->GetType() != ET_COIL)
          ) &&
          (!ChkFlag(EF_LDACTION)            ||  // optional enabled elements (FBD)
            (pE->GetType() != ET_VARINOUT)
          )
        )
      {
         long x1 = pE->GetOrigin().x;
         long x2 = x1 + pE->GetSize().cx;
         long y1 = pE->GetOrigin().y;
         long y2 = y1 + pE->GetSize().cy;
      
         if((m_Origin.x   < min(x1,x2) && m_Origin.x+m_Size.cx > max(x1,x2) )   &&
            (m_Origin.y+2 < min(y1,y2) && m_Origin.y+m_Size.cy > max(y1,y2) ))
               return true;
      }
   }
   return false;
}

/*func*------------------------------------------------------------------------
  
  in :
  out:-
-----------------------------------------------------------------------------*/
bool CElemAct::Spacing(CPoint Point, CPoint Delta, CPosArray* paPin, bool bLabelClip)
{
   int  i1;                // min element extension in raster coords
   int  i2;                // max element extension in raster coords
   bool bDone = false;     // true if any movement done
   bool bRes  = true;
   int  d;

   ASSERT( Delta.x * Delta.y == 0); // either x or y direction (not both)

   if( (d = Delta.x) != 0 )
   {
      i1 = min( m_Origin.x , m_Origin.x + m_Size.cx );
      i2 = max( m_Origin.x , m_Origin.x + m_Size.cx );

      if( Point.x > i1 )                                    // don't move the entire element
      {
         bool bInside = (Point.y > GetOrigin().y) && (Point.y < GetOrigin().y+GetSize().cy);
         if( Point.x <= i2 && bInside)                      // point inside or at right border
         {
            int N = m_Group.GetSize();                      // contents spacing
            int i;
            int yMin = GetOrigin().y;
            int yMax = yMin + GetSize().cy;

            if(bLabelClip)
            {
               for(i=0; i<N; i++)
               {
                  CElem* pE = m_Group[i];
                  if(pE->GetType()==ET_LABEL)
                  {
                     int yt = pE->GetOrigin().y;
                     int yb = yt + pE->GetSize().cy;
                     
                     if( (Point.y < yt) && (yMax > yt) )
                        yMax = yt;
                     
                     if( (Point.y > yt) && (yMin < yb) )
                        yMin = yb;
                  }
               }
            }

            for(i=0; i<N; i++)
            {
               CPoint P = m_Group[i]->GetOrigin();
               if(P.y > yMin && P.y <= yMax)
               {
                  m_Group[i]->Spacing(Point,Delta,paPin,bLabelClip);
               }
            }
            bDone = true;
         }
      }else
      {                                                     // element is strictly right
         bRes &= Resize(d,0,0,0);                           //    shift it

         int N = m_Group.GetSize();                         // force contents shift
         for(int i=0; i<N; i++)
            m_Group[i]->Resize(d,0,0,0);

         bDone = true;
      }
   }

   if( (d = Delta.y) != 0 )
   {
      i1 = min( m_Origin.y , m_Origin.y + m_Size.cy );      // yMin
      i2 = max( m_Origin.y , m_Origin.y + m_Size.cy );      // yMax
      
      for(int idx=0; idx<m_Con.GetSize(); idx++)            // watch the connectors (if any)
      {
         if (i1 < m_Con[idx]->m_Pos.y)
         {
            i1 = m_Con[idx]->m_Pos.y;
         }
      }

      if( Point.y > i1 )                                    // point inside the element
      {
         bool bInside = (Point.x > GetOrigin().x) && (Point.x < GetOrigin().x+GetSize().cx);
         if( Point.y < i2 && bInside )                      // point inside or at lower border
         {
            int N = m_Group.GetSize();                      // contents spacing
            for(int i=0; i<N; i++)
            {
               if(m_Group[i]->GetOrigin().y                          >= Point.y || 
                  m_Group[i]->GetOrigin().y+m_Group[i]->GetSize().cy >= Point.y)
               {
                  m_Group[i]->Spacing(Point,Delta,paPin,bLabelClip);
               }
            }
            bDone = true;
         }
      }else
      {                                                     // element is strictly below
         bRes &= Resize( 0,d, 0,0);                         //    move the element
         
         int N = m_Group.GetSize();                         // force contents shift
         for(int i=0; i<N; i++)
            m_Group[i]->Resize(0,d,0,0);
         bDone = true;
      }
   }
   
   ASSERT(bRes);
   return bDone;
}

bool CElemAct::IsSpaceable(CPoint At, CPoint Delta, bool bLabelClip)
{
   int i;
   bool bCan=true;
   int N = m_Group.GetSize();                      // contents spacing

   CRect R(GetOrigin(),GetOrigin()+GetSize());

   if(!R.PtInRect(At))
      return CElem::IsSpaceable(At,Delta,bLabelClip);

   if(Delta.x)
   {
      int yMin = GetOrigin().y;
      int yMax = yMin + GetSize().cy;
      
      if(bLabelClip)
      {
         for(i=0; i<N; i++)
         {
            CElem* pE = m_Group[i];
            if(pE->GetType()==ET_LABEL)
            {
               int yt = pE->GetOrigin().y;
               int yb = yt + pE->GetSize().cy;
               
               if( (At.y < yt) && (yMax > yt) )
                  yMax = yt;
               
               if( (At.y > yt) && (yMin < yb) )
                  yMin = yb;
            }
         }
      }

      if(Delta.x>0)
      {
         int max = GetOrigin().x+GetSize().cx;
         for(i=0; i<N; i++)
         {
            CElem *pE = m_Group[i];
            CPoint P = pE->GetOrigin() + pE->GetSize();
            if(P.y >= yMin && P.y < yMax)
            {
               if(P.x+Delta.x>=max || !pE->IsSpaceable(At,Delta,bLabelClip) )
                  return false;
            }
         }
      }else
      {
         int min = GetOrigin().x+1;
         for(i=0; i<N; i++)         // find bounding box of not shifted elements
         {
            CElem *pE = m_Group[i];
            CPoint P = pE->GetOrigin();
            int xEmax = max(P.x, P.x + pE->GetSize().cx);

            switch(pE->GetType())
            {
            case ET_LINE:
               if(P.y >= yMin && P.y < yMax)          // inside shifting area
               {
                  if( xEmax == At.x && min < xEmax)      // elem not moved?
                     min = xEmax;
               }
               break;
            default:
               if(P.y >= yMin && P.y < yMax)          // inside shifting area
               {
                  if( P.x < At.x && min < xEmax)      // elem not moved?
                     min = xEmax;
               }
            }
         }

         for(i=0; i<N; i++)
         {
            CElem *pE = m_Group[i];
            CPoint P = pE->GetOrigin();
            if(P.y >= yMin && P.y < yMax)
            {
               if((P.x>=At.x && P.x+Delta.x < min) || !pE->IsSpaceable(At,Delta,bLabelClip) )
                  return false;
            }
         }
      }
   }

   if(Delta.y)
   {
      if(Delta.y>0)
      {
         int max = GetOrigin().y+GetSize().cy;
         for(i=0; i<N; i++)
         {
            int y = m_Group[i]->GetOrigin().y + m_Group[i]->GetSize().cy;
            CElem* pE = m_Group[i];
            if(y+Delta.y >= max || !pE->IsSpaceable(At,Delta,bLabelClip) )
               return false;
         }
      }else
      {
         int min = GetOrigin().y+3;                   // title height == 2
         for(i=0; i<N; i++)                           // find bounding box of NOT SHIFTED elements
         {
            CElem *pE = m_Group[i];
            int y     = pE->GetOrigin().y;
            int yEmax = max(y, y + pE->GetSize().cy);

            switch(pE->GetType())
            {
            case ET_LINE:
               if( yEmax <= At.y && min < yEmax)   // elem not moved?
               {
                  min = yEmax;
               }
               break;

            default:
               if( y <= At.y && min < yEmax)      // elem not moved?
               {
                  min = yEmax;
               }
            }

         }

         for(i=0; i<N; i++)
         {
            CElem *pE = m_Group[i];
            int y = pE->GetOrigin().y;
            if((y>=At.y && y+Delta.y < min) || !pE->IsSpaceable(At,Delta,bLabelClip) )
               return false;
         }
      }
   }
   return bCan;
}

IMPLEMENT_SERIAL(CElemAct, CElemContain, SERIAL_VERSION)

//////////////////////////////////////////////////////////////////////
// CElemAction Class
//////////////////////////////////////////////////////////////////////

bool CElemAction::JoinTest(TDirection Dir, CElem* pE)
{ 
   if(Dir==DIR_LAST)
      return CElemAct::JoinTest(Dir,pE);
   return false; 
}

IMPLEMENT_SERIAL(CElemAction, CElemAct, SERIAL_VERSION)

//////////////////////////////////////////////////////////////////////
// CElemActBlk Class
//////////////////////////////////////////////////////////////////////

void CElemActBlk::CElemActBlkInit(void)
{
   CPin     *pPin;

   pPin = new CPin(_T(""), _T(""), _T("ANY"), 
      GetOrigin() + CPoint(0,1), CSize(0,0), PF_IN);
   if( pPin ) m_Con.Add(pPin);

   SetType(ET_ACTIONBLOCK);
}

bool CElemActBlk::JoinTest(TDirection Dir, CElem* pE)
{ 
   bool bOK = true;
   ASSERT( (m_Size.cx > 0) && (m_Size.cy > 0) );

   if (Dir==DIR_LAST)
   {
      bOK = CElemAct::JoinTest(Dir,pE);
   } else
   {
      if ((((pE->GetType() == ET_LINE) && (pE->ChkFlag(EF_RUBBLINE)) || (pE->GetSize().cx != 0))) || 
            (pE->GetType() == ET_CONIN) )
      {
         // each pin of (*pE) with the same x position MUST match one pin of the action block
         for (int iConn=0; bOK && (iConn < pE->m_Con.GetSize()); iConn++)
         {
            CPoint ElemPin = pE->m_Con[iConn]->m_Pos;

            if (ElemPin.x == m_Origin.x)  // action block has pins only on the left side
            {
               bOK &= (SearchConn(ElemPin)!=NULL)  || 
                      (ElemPin.y < m_Origin.y)     || 
                      (ElemPin.y > m_Origin.y + m_Size.cy);
            }
         }
      } else
      {
         bOK = false;
      }
   }
   return bOK; 
}

IMPLEMENT_SERIAL(CElemActBlk, CElemAct, SERIAL_VERSION)

//////////////////////////////////////////////////////////////////////
// ElemSfc.cpp: implementation of the CElemDiv Class.
//////////////////////////////////////////////////////////////////////

CElemDiv::CElemDiv()
{
   CSize minSize(4,0);
   CSize maxSize(MAX_XGRID,0);

   SetMinSize (minSize);
   SetMaxSize (maxSize);
}

/*func*------------------------------------------------------------------------
  create a new divergence
  created divergence is selected !!
  in :Origin  - insertion point 
      Size    - startup size of element 
  out:pointer to new divergence element
-----------------------------------------------------------------------------*/
CElemDiv::CElemDiv(CPoint Origin, CSize Size)
{
   CPin  *pPin;
   CSize  minSize(4,0);
   CSize  maxSize(MAX_XGRID,0);

   SetMinSize (minSize);
   SetMaxSize (maxSize);

   SetOrigin(Origin);
   SetSize  (Size);

   for( int i=0; i<=Size.cx; i+=2)
   {
      pPin = new CPin(_T(""), _T(""), _T("ANY"), 
         Origin + CPoint(i,0), CSize(0,0), PF_INOUT);
      if( pPin ) m_Con.Add(pPin);
   }
}

/*func*------------------------------------------------------------------------
  destructor
-----------------------------------------------------------------------------*/
CElemDiv::~CElemDiv()
{
}

/*func*------------------------------------------------------------------------
  draw the divergence (SFC)
  in :pDC   - pointer to device context
      pView - pointer to document (data)
      iGrid - actual display resolution
  out:-
-----------------------------------------------------------------------------*/
void CElemDiv::Draw(CDC * pDC, CGrEditorView * pView, int iGrid)
{
   CRect    r;
   CSize    size;
   CPin    *pPin;
   CString  str;
   int      i, iPinNr;
   PEN_TYPE PenType;
   int      x1, x2, y;
   bool     bScreen = !pDC->IsPrinting();

   if (!pView)
   {
      return;
   }
   ASSERT((GetType() == ET_ALTDIV) || (GetType() == ET_SIMDIV));
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

   // draw the connect point
   DrawPins(pDC, iGrid);

   // draw the divergence body
   if(m_Con.GetSize() > 0)
   {
      iPinNr = 0;
      if( (pPin = m_Con.GetAt(iPinNr)) != NULL )
      {
         x1 = (m_Origin.x            )*iGrid;
         x2 = (m_Origin.x + m_Size.cx)*iGrid;
         y  = (m_Origin.y            )*iGrid;
         if ( GetType() == ET_ALTDIV )
         {
            for( i=0; i<2; i++) { pDC->MoveTo(x1,y+i); pDC->LineTo(x2,y+i); }
         } else if( GetType() == ET_SIMDIV )
         {
            for( i=-2; i<0; i++) { pDC->MoveTo(x1,y+i); pDC->LineTo(x2,y+i); }
            for( i= 1; i<3; i++) { pDC->MoveTo(x1,y+i); pDC->LineTo(x2,y+i); }
         } else {
            ASSERT(FALSE);
         }
         // draw element icon 
//       DrawIcon( pDC, pView, m_Origin, IDB_ICONTRAN, iGrid);
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
void CElemDiv::DrawSelectFrame(CDC * pDC, CGrEditorView * pView, int iGrid, 
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

         pDC->Rectangle( GetResizeRect(RS_E , r, iGrid) );
         pDC->Rectangle( GetResizeRect(RS_W , r, iGrid) );
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
RS_TYPE CElemDiv::GetResizeType(CPoint p, int iGrid)
{
   CRect r;

   if( GetResizeRect( RS_E , r, iGrid )->PtInRect( p ) ) return RS_E;
   if( GetResizeRect( RS_W , r, iGrid )->PtInRect( p ) ) return RS_W;

   return RS_LAST;
}

/*func*------------------------------------------------------------------------
  test if p at resize rect in element
  in :p     - point for test in logical coordinates at sheet
      iGrid - actual zoom
  out:TRUE  - yes on resize rect
      FALSE - not in resize rect
-----------------------------------------------------------------------------*/
bool CElemDiv::IsResizeRect(CPoint p, int iGrid)
{
   CRect r;

   if( GetResizeRect( RS_E , r, iGrid )->PtInRect( p ) ||
       GetResizeRect( RS_W , r, iGrid )->PtInRect( p ) )
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
bool CElemDiv::Resize(int dx, int dy, int sx, int sy)
{
   CPin  *pPin;
   int    i, iNewSX;
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
         iNewSX = min( m_MaxSize.cx, max(m_MinSize.cx, m_Size.cx + sx));
         // adjust the connect points
         if( iNewSX/2+1 > m_Con.GetSize() )
         {  // insert new connect points
            for( i=m_Con.GetSize()*2; i<=iNewSX; i+=2)
            {
               pPin = new CPin(_T(""), _T(""), _T("ANY"), 
                  m_Origin + CPoint(i,0), CSize(0,0), PF_INOUT);
               if( pPin ) 
               {
                  m_Con.Add(pPin);
                  pPin->m_Entry = m_pPinTable->InsertPin(this, pPin->m_Pos);
                  ASSERT(pPin->m_Entry);
               }
            }
         } else if( iNewSX/2+1 < m_Con.GetSize() )
         {  // delete existing connect points
            for( i=0; i<m_Con.GetSize(); i++)
            {
               pPin = m_Con.GetAt(i);
               ASSERT_VALID(pPin);
               if( pPin && (pPin->m_Pos.x - m_Origin.x > iNewSX) )
               { 
                  ASSERT(pPin->m_Entry);
                  m_pPinTable->DeletePin(this, pPin->m_Entry, CPoint(pPin->m_Pos.x - 2, pPin->m_Pos.y));
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
void CElemDiv::Serialize(CArchive & ar)
{
   CElem::Serialize(ar);
}

/*func*------------------------------------------------------------------------
  serialize the element to/from XML stream
  in :ar - the archive
  out:-
-----------------------------------------------------------------------------*/
void CElemDiv::SerializeXML(CXMLArchive &ar)
{
   CPin  *pPin;
   int    i, iNewSX;

   if(ar.IsStoring())
   {
      // all CXMLArchive fire HRESULT exceptions, which must be cought!!!
      try {            // name and create node here !!
         CElem::SerializeXML(ar);  // origin, size, selected, comment
         // we save no pin list !!
         // pins will be reconstructed at loading.
      }        
      catch (HRESULT hr)
      {
         TRACE(_T("Cannot save to XML; hr=0x%08X\n"), hr); throw hr;
      }    
   } else {
      try {   
         CElem::SerializeXML(ar);  // origin, size, selected, comment
         // no we construct the pin list ..
         iNewSX = min( m_MaxSize.cx, max(m_MinSize.cx, m_Size.cx));

         for( i=m_Con.GetSize()*2; i<=iNewSX; i+=2)
         {
            pPin = new CPin(_T(""), _T(""), _T("ANY"), 
               m_Origin + CPoint(i,0), CSize(0,0), PF_INOUT);
            if( pPin ) m_Con.Add(pPin);
         }
      }
      catch (HRESULT hr)
      {
         TRACE(_T("Cannot load from XML; hr=0x%08X\n"), hr); throw hr;
      }   
   }
}

/*func*------------------------------------------------------------------------
  check Possibility of insertion/removal of lines/columns
  in :   Spacing point, spacig amount (unused: *pinArray, bLabelClip )
  out:-
-----------------------------------------------------------------------------*/
bool CElemDiv::IsSpaceable(CPoint At, CPoint Delta, bool bLabelClip)
{
   int i1, i2;
   bool bOK = true;

   if(Delta.x<0)  // negative x-spacing
   {
      if(GetSize().cx>0)
      {
         i1 = GetOrigin().x;
         i2 = i1 + GetSize().cx;
      }else
      {
         i2 = GetOrigin().x;
         i1 = i2 + GetSize().cx;
      }

      if(At.x>=i1 && At.x < i2)
      {
         CPoint P1( At.x,   GetOrigin().y);  // spacing pin position
         CPoint P2( At.x+2, GetOrigin().y);  // next pin right of the spacing pin position
         CPin *pP1 = SearchPin(P1,1);
         if(pP1->GetConnects()>1)
         {
            CPin* pP2 = SearchPin(P2,1);
            bOK = (pP2->GetConnects()==1);
         }
      }
   }else
   {
      if(Delta.y<0)
      {
         bOK = At.y != GetOrigin().y;
      }
   }
   return bOK;
}

/*func*------------------------------------------------------------------------
   perform spacing operations (ins/del:row/col)  
  in :   Spacing point, spacig amount (unused: *pinArray, bLabelClip )
  out:-
-----------------------------------------------------------------------------*/
bool CElemDiv::Spacing( CPoint Point, CPoint Delta, CPosArray* paPin, bool bLabelClip)
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

   //------------------------------------------------------ NEGATVE SPACING
   if( (d = Delta.x) < 0 )
   {
      i1 = min(m_Origin.x, m_Origin.x + m_Size.cx);         // normalize direction
      i2 = max(m_Origin.x, m_Origin.x + m_Size.cx);

      if( Point.x < i1)                                     // element is strictly right
      {
         bRes &= Resize( d,0,0,0);                          // move it
         bDone = true;
      }else
      {
         if( (Point.x >= i1) && (Point.x < i2) )            // point is inside
         {
            if( m_Size.cx >= 0 )                            // normal  orientation
            {
               bRes &= Resize( 0, 0, d, 0);
            }else                                           // reverse orientation
            {
               bRes &= Resize( d, 0,-d, 0);
            }
            bDone = true;
         }
      }
   }

   if( (d = Delta.y) < 0)
   {
      bDone = CElem::Spacing(Point,Delta,paPin,bLabelClip);
   }

//   ASSERT(bRes);   // bRes may be false if minimum size is reached!
   return bDone;
}

bool  CElemDiv::CheckDropIn( CElem* pE)  
{
   ASSERT(pE != NULL);

   if((pE != NULL)                  &&
      (pE->GetType() == ET_LINE)    &&
      (pE->GetSize().cx == 0)       &&
      (!pE->ChkFlag(EF_RUBBLINE)))
   {
      return true;
   }
   return false;
}

IMPLEMENT_SERIAL(CElemDiv, CElem, SERIAL_VERSION)

//////////////////////////////////////////////////////////////////////
// CElemAltDiv Class
//////////////////////////////////////////////////////////////////////

CElemAltDiv::CElemAltDiv(CElemDiv *pE)
{ 
   CopyData(pE);  // with pins
   SetType(ET_ALTDIV); 
} 

IMPLEMENT_SERIAL(CElemAltDiv, CElemDiv, SERIAL_VERSION)

//////////////////////////////////////////////////////////////////////
// CElemAltDiv Class
//////////////////////////////////////////////////////////////////////

CElemSimDiv::CElemSimDiv(CElemDiv *pE)
{ 
   CopyData(pE);  // with pins
   SetType(ET_SIMDIV); 
} 

IMPLEMENT_SERIAL(CElemSimDiv, CElemDiv, SERIAL_VERSION)