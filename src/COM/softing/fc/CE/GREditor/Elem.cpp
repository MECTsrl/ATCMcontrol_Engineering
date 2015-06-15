

/*doc*=======================================================================*/
/*inc*=======================================================================*/
#include "stdafx.h"

#include "GrEditor.h"
#include "GrEditorDoc.h"
#include "GrEditorView.h"
#include "GrEditorUtil.h"
#include "Elem.h"
#include "ElemPtrList.h"
#include "PosArray.h"

/*const*=====================================================================*/
const YLineGrid = 2;                         // text line height in grid units

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

//////////////////////////////////////////////////////////////////////
// CText Class
//////////////////////////////////////////////////////////////////////

/*func*------------------------------------------------------------------------
  count the lines in comment
  in :-
  out:number of lines (0..n)
-----------------------------------------------------------------------------*/
int CText::CountLines(void)
{
   CString  str, str2;
   int      i, iLines; 

   str    = m_Str;
   iLines = 0;
   while ( str.GetLength() > 0 ) 
   {
      i = str.Find(_T("\r"));
      if( i == -1 ) i = str.GetLength();
      iLines++;
      str = str.Right( str.GetLength() - i - 2);
   };
   return iLines;
}

/*func*------------------------------------------------------------------------
  check string for max. length, if necessary reduce him und signalize this
  with '..'
  in :str     - string for check
      dc      - actual device context for test
      iMaxLen - max. length for string in log. device units (pixel)
  out:true  - string was abbreviated
      false - string was not abbreviated
-----------------------------------------------------------------------------*/
void CText::ProcessIdent(CString& str, CDC& dc, int iMaxLen, int iLineNo)
{
   CSize    size;
   LPCTSTR  pszPntStr = _T("..");
   int      i, iLen, iPntLen;


   str = m_Str;
   if (iLineNo == 0)
   {
      m_Flag -= TF_ABBR;
   }
   // search start position of line 
   while (!str.IsEmpty() && iLineNo > 0) 
   {
      i = str.Find(_T("\r"));
      if( i != -1 ) 
      {
         str.Delete(0,i+2);
      }
      iLineNo--;
   }
   // search end of line
   i = str.Find(_T("\r"));
   if( i != -1 ) 
   {
      str = str.Left(i);
   }
   // check contents of line
   size = dc.GetTextExtent(str);
   if( size.cx > iMaxLen )
   {
      m_Flag += TF_ABBR;
      iLen = str.GetLength();
      size = dc.GetTextExtent(pszPntStr);
      iPntLen = size.cx;
      while ( iLen > 0 ) 
      {
         iLen--;
         str  = str.Left(iLen);
         size = dc.GetTextExtent(str);
         if( size.cx + iPntLen <= iMaxLen )
         {
            break;
         }
      }
      str += pszPntStr;
   }
}

/*func*------------------------------------------------------------------------
  calculate the len for the str
  in :str     - string 
  out:length for string in raster coords
-----------------------------------------------------------------------------*/
int CText::GetIdentLen(CString & str)
{
   int          iDC, i, iStart, iStop, iLen;
   int          grid=8;
   CFont        Font;
   CSize        size;
   CString      str2;
   CClientDC    dc(NULL);  // .. for calculate size of element

   iLen = 0;
   if (!str.IsEmpty())
   {
      iDC = dc.SaveDC();
      Font.CreateFont( 
         -((2*grid*FontToGrid)/100),0,0,0,0,0,0,0,0,0,0,0,0,_T("Arial"));
      dc.SelectObject( &Font );

      iStart = 0;
      do 
      {
         iStop = str.Find(_T("\r"), iStart);
         if (iStop == -1)
         {
            iStop = str.GetLength();
         }
         str2 = str.Mid(iStart, iStop - iStart);
         size = dc.GetTextExtent(str2);
         if( grid )
         {
            i = (((size.cx + grid - 1) / grid) + 1) & 0xFFFFFFFE;
         } else {
            i = 2;   // hoppala
         }
         iStart = iStop + 2;
         if (iLen < i)
         {
            iLen = i;
         }
      } while (iStart < str.GetLength());

      dc.RestoreDC(iDC);
   }
   return iLen;
}


/*func*------------------------------------------------------------------------
 in: int&             iStart  - line start index (init to 0 to get the 1st line)
     const CString&   sLine   - destination string (unchanged if false returned)
ret: bool - false if no more lines or *piLine is invalid
-----------------------------------------------------------------------------*/
CString CText::NextLine(int& iStart)
{
   int iE = m_Str.Find(_T('\r'), iStart);
   if(iE < 0)
   {
      iE = m_Str.GetLength();
   }
   iE -= iStart;
   iStart += iE + sizeof(_T('\r'));
   
   return m_Str.Mid( iStart-iE, iE);
}

#if 0
/*func*------------------------------------------------------------------------
  calculate the xy-size of <str> (handles linefeeds AND automatic wrapping)
  in :str     - string 
      scsize  - available rectangle size on screen (pixels!!)
  out:scsize  - required rectangle size in pixels (with cy unchanged!)
  ret:CSize: required rectangle size in grid units
-----------------------------------------------------------------------------*/
CSize CText::ScreenSize(CSize& scsize)
{
   CClientDC   dc(NULL);   // .. for calculate size of element (dc to entire screen)
   CSize       res(0,0);   // result
   CFont       Font;       // (HACK!!! must be the drawing font)
   int         iDC;        // handle to saved dc
   int         grid=8;

   if (!str.IsEmpty())
   {
      iDC = dc.SaveDC();
      Font.CreateFont( -((2*grid*FontToGrid)/100),0,0,0,0,0,0,0,0,0,0,0,0,_T("Arial"));
      dc.SelectObject( &Font );

      if(m_Flag.Chk(TF_WRAP))
      {

      }else // text is NOT wrapped
      {
         int N = m_Str.GetLength();
         for(int iS=0; iS < N; )
         {
            scsize = dc.GetTextExtent( NextLine(iS,m_Str)).cx;  // changes <iS>
            res.cx   = max(res.cx, scsize.cx);
            res.cy  += YLineGrid;                                 // YLineGrid is defined at the top
         }
         res.cx = (((res.cx + grid -1) / grid)+1) & 0xFFFFFFFE;   // doc to grid conversion
      }
      dc.RestoreDC(iDC);
      Font.DeleteObject();
   }
   return res;
}
#endif

/*func*------------------------------------------------------------------------
  return the number of chars that can be drawn in x-range of <iWidth>
  ret: number of the next <n> characters of <txt> beginning at <iStart>
       <bTrunc> is set to TRUE only on abbreviation otherwise not changed
-----------------------------------------------------------------------------*/
int CText::GetNextLine(CString& txt, BOOL& bTrunc, CDC *pDC, int iWidth, int iStart, bool bLastLine)
{
   //algorithm: modified (uses estimation) binary search for max legth

   int cx    = 0;
   int iSkip = 0;
   int nMax  = m_Str.GetLength()-iStart;     // remaining text length
   int n     = nMax;

   ASSERT( &txt != &m_Str);
   
   if(nMax>0)                                // anything left to read
   {
      txt = m_Str.Mid(iStart);               // get the remaining string
      n = txt.FindOneOf(_T("\r\n"));         // search for explicit line ends
      if(n>=0)
      {
         iSkip++;                            // count characters to be skipped
         if(n<nMax-1 && txt[n]==_T('\r') && txt[n+1]==_T('\n'))
         {
            iSkip++;
         }
         txt.Delete(n,nMax-n);               // remove the rest
         nMax = n;                           // include the '\r'
      }else
      {
         n = nMax;
      }
      
      // first estimation of character count
      int iStep;
      int iLoops = 0;
      
      cx = pDC->GetTextExtent(txt,txt.GetLength()).cx;

      if(cx > iWidth)                       // line exceeds iWidth?
      {
         n = nMax * iWidth / cx;             // 1st estimation for <n>
         iStep = 3;                          // optimize for faster iteration
      
         // search max. <n> with (cx <= iWidth)
         while(iLoops++ < nMax)
         {
            cx = pDC->GetTextExtent(txt,n).cx;
            if(iStep==0 || cx==iWidth || n==nMax && cx<iWidth) // ready ?
            {
               break;
            }
         
            if( (cx<iWidth)==(iStep<0) )     // proper search direction?
            {                             
               iStep = -iStep;               // no: reverse
               iStep = iStep/2;              // set smaller steps
            }
            n += iStep;
         }
         // ensure <cx> is always less or equal <iWidth>
         if(pDC->GetTextExtent(txt,n).cx > iWidth)
         {
            n--;
         }
         cx = pDC->GetTextExtent(txt,n).cx;
      }

      bool bDoTruncate = false;
      if(n < nMax)
      {
         if(m_Flag.Chk(TF_WRAP) && !bLastLine)
         {
               txt = txt.Left(n);
               iSkip = 0;
         }else
         {
            bDoTruncate = true;
         }
      }else if(bLastLine && nMax < m_Str.GetLength()-iStart)
      {
         bDoTruncate = true;
      }

      if(bDoTruncate)
      {
         txt = txt.Left(n);                                 // abbreviate if requested
         LPCTSTR  pszPntStr = _T("..");
         int iStrWidth = pDC->GetTextExtent(pszPntStr).cx;
         if( iStrWidth + cx > iWidth)
         {
            int N = txt.GetLength();
            for(int i=1; i<N; i++)
            {
               if(pDC->GetTextExtent(txt.Right(i)).cx >= iStrWidth)
                  break;
            }
            txt.Delete(N-i,i);
         }
         txt += pszPntStr;
         
         n = nMax;        
         bTrunc = true;                                     // report extra characters
      }
   }else
   {
      txt.Empty();
   }

   return n + iSkip;                                     // text length + [\r\n]*
}


/*func*------------------------------------------------------------------------
  get absolute line (must search for!)
  HINT:  don't use this routine inside a loop to iterate on all lines
         - use GetNextLine(..) instead of because this is faster
  in :txt      - string ref to put the result (requested line)
      pDC      - pointer to device context
      iWidth   - width of the drawing area
      iLine    - requested line number [1..N-1]
  out:txt      - destiantion string for the requested line
  ret:int      - string offset of the 1st char (add txt.GetLength() to obtain
                 the next start position i.e. for using as arg to GetNextLine
               - if the result is 0, the line was not found
-----------------------------------------------------------------------------*/
int CText::GetLine(CString& txt, BOOL& bTrunc, CDC *pDC, int iWidth, int iLine)
{
   int nChars;
   int nPos = 0;  // absolute string offset
   int iL   = 0;  // line counter
   ASSERT(iLine < 10000);
   
   if(!m_Str.IsEmpty())
   {
      while(iL++ < iLine)
      {
         nChars = GetNextLine(txt, bTrunc, pDC, iWidth, nPos, true);  // writes to <&txt>
         nPos  += nChars;         // adjust read position
         if(!nChars)             // anything read?
         {
            txt.Empty();
            nPos = 0;
            break;
         }
      }
   }else
   {
      txt.Empty();            // empty the result-string
   }
   return nPos;
}



/*func*------------------------------------------------------------------------
  draw a text of element with search result (NEW FUNCTION)
  draw reference point is left-top
  in :pDC      - pointer to device context
      r        - drawing rectangle in client coords
      iGrid    - curret grid scaling factor
      ColorType- CElem::GetColorType() result
      pSD      - pointer to SearchDat struct if location is inside this text
      iconWidth- icon space at the 1st line (>0:right; <0:left; ==0:none)
  out:-
  ret: GetTextExtent of the text that was drawn
-----------------------------------------------------------------------------*/
CSize CText::Draw(CDC *pDC, CRect& r, int iGrid, COLOR_TYPE ColorType, SearchDat* pSD, int iconWidth)
{
   COLORREF OldTextCol;
   CSize    size(0,0);
   int      iLineStart  = 0;     // char position for the next line start
   int      iRectLines  = r.Height() / (2*iGrid);
   int      yOfs        = iGrid;
   bool     bScreen     = !pDC->IsPrinting();

   ASSERT(pDC);
   OldTextCol = pDC->SetTextColor(GDIHelper::GetColor(bScreen, ColorType));
   pDC->SetBkMode( TRANSPARENT );
   pDC->SetTextAlign( TA_LEFT | TA_TOP );

   int iExtra = m_Flag.Chk(TF_WITH_COLON) ?pDC->GetTextExtent(_T(":")).cx :0;

   BOOL bTrunc = FALSE;    // result (truncation done at any line)
   for(int iLine = 1; (iLineStart < m_Str.GetLength()) && (iLine <= iRectLines); iLine++)
   {
      CString sLine;
      int iStart  = iLineStart;  // save start offset of sLine
      int iWidth  = r.Width()-iExtra;
      if(iLine == 1 && iconWidth != 0)
      {
         iWidth -= iconWidth>0 ?iconWidth :-iconWidth;  // -= abs(iconWidth)
      }

      iLineStart += GetNextLine(sLine, bTrunc, pDC, iWidth, iLineStart, iLine == iRectLines); // ->sLine

      if(m_Flag.Chk(TF_WITH_COLON))
      {
         ASSERT(iLine==1);                         // expect one line names
         ASSERT(!m_Flag.Chk(TF_MULTILINE));
         ASSERT(!m_Flag.Chk(TF_WRAP));
         sLine += _T(':');
      }

      if(!sLine.IsEmpty())
      {
         size = pDC->GetTextExtent(sLine);         // calculate output position
         int x = r.left;                           // assume TF_LEFT
         int y = r.top + yOfs - size.cy/2;

         if (m_Flag.Chk(TF_RIGHT))
         {
            x = r.right - size.cx;
            if(iLine==1 && iconWidth>0)
            {
               x -= iconWidth;
            }
         }else if (!m_Flag.Chk(TF_LEFT))
         {
            x += r.Width()/2 - size.cx/2;
            if(iLine==1 && iconWidth<0)
            {
               x -= iconWidth;
            }
         }

         if(pSD)
         {
            CString s;

            int N = sLine.GetLength();
            
            int P = pSD->m_iFindPos - iStart;
            int L = pSD->m_iFindLen;

            // selection start index (1st selected character)
            int i1 = max(0, min( N, P ));
            
            // selection end   index (1st unselected character)
            int i2 = max(0, min( N, P + L)); 
            
            if( i1 > 0 || i2 == 0 ) // section 1: (in front of selection)
            {
               s = sLine.Left(i1);
               pDC->TextOut( x, y, s);
               x += pDC->GetTextExtent(s).cx;
            }

            if( i1 < N && i2 > 0 )  // section 2   (draw selection)
            {
               // invert text colors
               COLORREF cTxt = pDC->SetTextColor(GDIHelper::GetColor(bScreen, CO_BACKGD));
               COLORREF cBkg = pDC->SetBkColor  (GDIHelper::GetColor(bScreen, CO_NORMAL));
               pDC->SetBkMode( OPAQUE );

               // draw text
               s = sLine.Mid(i1, i2-i1);
               pDC->TextOut( x, y, s);
               x += pDC->GetTextExtent(s).cx;

               // restore text colors
               pDC->SetBkMode( TRANSPARENT );
               pDC->SetTextColor( cTxt );
               pDC->SetBkColor  ( cBkg );
            }
            
            if( i2>=0 && i2 < N )   // section 3   (behind selection)
            {
               s = sLine.Right(N-i2);
               pDC->TextOut( x, y, s);
               x += pDC->GetTextExtent(s).cx;
            }
         }else
         {
            pDC->TextOut(x,y,sLine);
         }
      }
      yOfs += 2 * iGrid;
   }//for( iLine...

   if(bTrunc)
   {
      m_Flag.Set(TF_ABBR);
   }else
   {
      m_Flag.Clr(TF_ABBR);
   }

   pDC->SetTextColor(OldTextCol);
   return size;
}

BOOL CText::Find(SearchDat& SD)
{
   LPCTSTR psz;
   int iLen = SD.m_strFind.GetLength();
   if( SD.m_bNext )
   {
      if (SD.m_bFindOnly || m_Flag.Chk(TF_EDITABLE))
      {
         psz = m_Str.GetBuffer(0);
         SD.m_iFindLen = (int)_tcslen(psz);
         if( SD.m_iFindPos < 0 ) 
         {
            SD.m_iFindPos = 0;
         }
         if( SD.m_iFindLen >= iLen )
         {
            for( ; SD.m_iFindPos <= SD.m_iFindLen-iLen; SD.m_iFindPos++ )
            {
               if( SD.m_bCase ?
                  ( _tcsncmp (psz + SD.m_iFindPos, SD.m_strFind, iLen) == 0 ) :
                  ( _tcsnicmp(psz + SD.m_iFindPos, SD.m_strFind, iLen) == 0 ) )
               {
                  m_Str.ReleaseBuffer();
                  SD.m_iFindLen = (int)_tcslen(SD.m_strFind);
                  return TRUE;
               }
            }
         }
         m_Str.ReleaseBuffer();
         SD.m_iFindPos = 0;
      }
   } else 
   {
      if (SD.m_bFindOnly || m_Flag.Chk(TF_EDITABLE))
      {
         psz = m_Str.GetBuffer(0);
         SD.m_iFindLen = (int)_tcslen(psz);
         
         if( SD.m_iFindPos > SD.m_iFindLen ) 
         {
            SD.m_iFindPos = SD.m_iFindLen-iLen;
         }

         for( ; SD.m_iFindPos >= 0; SD.m_iFindPos-- )
         {
            if( SD.m_bCase ?
               ( _tcsncmp (psz + SD.m_iFindPos, SD.m_strFind, iLen) == 0 ) :
               ( _tcsnicmp(psz + SD.m_iFindPos, SD.m_strFind, iLen) == 0 ) )
            {
               m_Str.ReleaseBuffer();
               SD.m_iFindLen = (int)_tcslen(SD.m_strFind);
               return TRUE;
            }
         }
         SD.m_iFindPos = INT_MAX;   // a big integer ..
         m_Str.ReleaseBuffer();
      }
   }

   SD.m_iFindLen = 0;
   return FALSE;
}

void CText::Replace(SearchDat& SD)
{
   if(m_Flag.Chk(TF_EDITABLE))
   {
      if(SD.m_iFindPos>=0 && SD.m_iFindPos + SD.m_iFindLen < m_Str.GetLength())
      {
         m_Str.Delete(SD.m_iFindPos,SD.m_iFindLen);
         m_Str.Insert(SD.m_iFindPos,SD.m_strReplace);
      }
   }
}
//////////////////////////////////////////////////////////////////////
// CPin Class
//////////////////////////////////////////////////////////////////////

/*func*------------------------------------------------------------------------
  Construction/Destruction
  in :
  out:
-----------------------------------------------------------------------------*/
CPin::CPin()
{
	m_Pos        = CPoint(0,0);
	m_Size       = CSize(0,0);
   m_strIVal    = _T("");
   m_strType    = _T("");
   m_Flag.Set(PF_HIDDEN);  // size is 0,0 !
   m_Entry      = NULL;
}

CPin::CPin(CPin * pPin)
{
	m_Pos        = pPin->m_Pos;    
	m_Size       = pPin->m_Size;
   m_strIVal    = pPin->m_strIVal;
   m_Name       = pPin->m_Name;
   m_strType    = pPin->m_strType;
	m_Flag       = pPin->m_Flag;
   m_Entry      = NULL;
}

CPin::CPin(LPCTSTR pszName, LPCTSTR pszIVal, LPCTSTR pszType,
   CPoint pos, CSize size, CPinFlag flags )
{
   m_Name       = pszName;
   m_strIVal    = pszIVal;
   m_strType    = pszType;
	m_Pos        = pos;
	m_Size       = size;
   m_Flag       = flags; 
   if( m_Size == CSize(0,0) ) 
   {
      m_Flag.Set(PF_HIDDEN);
   }
   m_Entry      = NULL;
}

CPin::~CPin()
{
}

void CPin::SetName(LPCTSTR pszName)
{
   m_Name = pszName;
}

void CPin::SetIVal(LPCTSTR pszIVal)
{
   m_strIVal = pszIVal;
}

void CPin::SetType(LPCTSTR pszType)
{
   m_strType = pszType;
}

/*func*------------------------------------------------------------------------
  draw the pin at element
  in :pDC   - pointer to device context
      iGrid - actual display resolution
  out:-
-----------------------------------------------------------------------------*/
void CPin::Draw(CDC * pDC, int iGrid)
{
   int    x1, x2;
   int    y1, y2;
   int    ta;     // temp pos (x or y)

   if( !m_Flag.Chk(PF_DISABLED) )
   {
      x1 = (m_Pos.x            ) * iGrid;
      x2 = (m_Pos.x + m_Size.cx) * iGrid;
      y1 = (m_Pos.y            ) * iGrid;
      y2 = (m_Pos.y + m_Size.cy) * iGrid;

      if( m_Size.cy < 0 ) y2--;  // workaround ..

      if( m_Size != CSize(0,0) )
      {
         if( m_Flag.Chk(PF_NEGATE) )
         {
            if (m_Size.cx > 0)
            {
               ta = x2 - (iGrid - 1);
               pDC->Ellipse( ta, y1-(iGrid - 1)/2, x2, y1+(iGrid - 1)/2 );
               pDC->MoveTo( x1, y1 );
               pDC->LineTo( ta, y1 );
            } 
            else if (m_Size.cx < 0)
            {
               ta = x1 - (iGrid - 1);
               pDC->Ellipse( ta, y1-(iGrid - 1)/2, x1, y1+(iGrid - 1)/2 );
               pDC->MoveTo( ta, y1 );
               pDC->LineTo( x2, y1 );
            }
         } else {
            pDC->MoveTo( x1, y1 );
            pDC->LineTo( x2, y2 );
         }
      }

      if( m_bDebug2 )
      {
	      CFont  Font, *OldFont;
         char   s[32];
         UINT   nFlags;
         int    x, y;

         if (m_Size.cy == 0)
         {
            if ( m_Size.cx < 0) { x = m_Pos.x*iGrid - 1; y = m_Pos.y*iGrid + 1; nFlags = TA_RIGHT | TA_TOP;    }
            else                { x = m_Pos.x*iGrid + 1; y = m_Pos.y*iGrid - 1; nFlags = TA_LEFT  | TA_BOTTOM; }
         } else {
            if ( m_Size.cy > 0) { x = m_Pos.x*iGrid + 1; y = m_Pos.y*iGrid + 1; nFlags = TA_LEFT  | TA_TOP;    }
            else                { x = m_Pos.x*iGrid - 1; y = m_Pos.y*iGrid - 1; nFlags = TA_RIGHT | TA_BOTTOM; }
         }
         Font.CreateFont ( 8,0,0,0,0,0,0,0,0,0,0,0,0,"MS Sans Serif");
         OldFont = pDC->SelectObject( &Font ); 

         sprintf (s, "%ld", GetConnects());
         pDC->SetTextAlign (nFlags);
         pDC->TextOut (x, y, s, strlen(s));
         pDC->SelectObject (OldFont); 
      }
   }
}

/*func*------------------------------------------------------------------------
  draw hot point of the pin 
  in :pDC   - pointer to device context
      iGrid - actual display resolution
  out:-
-----------------------------------------------------------------------------*/
void CPin::DrawLightPin(CDC * pDC, int iGrid)
{
   CRect  r;
   bool   bScreen = !pDC->IsPrinting();

   if( !m_Flag.Chk(PF_DISABLED) )
   {
      r.SetRect(m_Pos.x * iGrid - 1, m_Pos.y * iGrid - 1, 
                m_Pos.x * iGrid + 2, m_Pos.y * iGrid + 2);

      pDC->FillRect(r, GDIHelper::GetBrush(bScreen, BT_LIGHTPIN));
   }
}

/*func*------------------------------------------------------------------------
  serialize the element to/from stream
  in :ar - the archive
  out:-
-----------------------------------------------------------------------------*/
void CPin::Serialize(CArchive & ar)
{
	if (ar.IsStoring())
	{
      ar << m_Pos;                // connect point position
      ar << m_Size;               // connect point size
      ar << m_Name;               // var name
      ar << m_strIVal;            // init value (input)
      ar << m_strType;            // var type
      ar << m_Flag;
	}
	else
	{
      ar >> m_Pos;
      ar >> m_Size; 
      ar >> m_Name;            
      ar >> m_strIVal;            
      ar >> m_strType;
      ar >> m_Flag;  
      // clear temporäry flags ..
      m_Flag.Clr(PF_MODIFIED );
      m_Flag.Clr(PF_LIGHTPIN );
      m_Flag.Clr(PF_PROCESSED);
      m_Flag.Clr(PF_SELECT   );
	}
}

/*func*------------------------------------------------------------------------
  serialize the element to/from XML stream
  in :ar - the archive
  out:-
-----------------------------------------------------------------------------*/
void CPin::SerializeXML(CXMLArchive &ar)
{
   if(ar.IsStoring())
   {
      // all CXMLArchive fire HRESULT exceptions, which must be cought!!!
      try {            // name and create node here !!
         if     ( m_Flag.Chk(PF_EXT_INP )) ar.SetAttribute( XML_4GR_ATTR_CLASS, XML_4GR_ATTR_VAL_INEXT   );
         else if( m_Flag.Chk(PF_EXT_OUTP)) ar.SetAttribute( XML_4GR_ATTR_CLASS, XML_4GR_ATTR_VAL_OUTEXT  );
         else if( m_Flag.Chk(PF_IN      )) ar.SetAttribute( XML_4GR_ATTR_CLASS, XML_4GR_ATTR_VAL_INPUT   );
         else if( m_Flag.Chk(PF_OUT     )) ar.SetAttribute( XML_4GR_ATTR_CLASS, XML_4GR_ATTR_VAL_OUTPUT  );
         else if( m_Flag.Chk(PF_INOUT   )) ar.SetAttribute( XML_4GR_ATTR_CLASS, XML_4GR_ATTR_VAL_INOUTPUT);
         else ASSERT(FALSE);

         if( !m_Name.IsEmpty()    ) ar.SetStringAttributeUTF8( XML_4GR_ATTR_NAME,    m_Name);
//       if( !m_strType.IsEmpty() ) ar.SetAttribute( XML_4GR_ATTR_TYPE,    m_strType);
//       if( !m_strIVal.IsEmpty() ) ar.SetAttribute( XML_4GR_ATTR_INITVAL, m_strIVal);

         ar.SetAttribute( XML_4GR_ATTR_ORIGIN,  m_Pos);

         if( m_Flag.Chk(PF_NEGATE  )) ar.SetAttribute( XML_4GR_ATTR_NEGATE,   1L);
         if( m_Flag.Chk(PF_HIDDEN  )) ar.SetAttribute( XML_4GR_ATTR_HIDDEN,   1L);
         if( m_Flag.Chk(PF_DISABLED)) ar.SetAttribute( XML_4GR_ATTR_DISABLE,  1L);
         if( m_Flag.Chk(PF_R_EDGE  )) ar.SetAttribute( XML_4GR_ATTR_R_EDGE,   1L);
         if( m_Flag.Chk(PF_F_EDGE  )) ar.SetAttribute( XML_4GR_ATTR_F_EDGE,   1L);
      }        
      catch (HRESULT hr)
      {
         TRACE(_T("Cannot save to XML; hr=0x%08X\n"), hr); throw hr;         
      }    
   } else {
      CString str;
      CPoint  p;
      long    l;

      if( ar.GetAttribute( XML_4GR_ATTR_CLASS, str) )
      {
         if     ( str == XML_4GR_ATTR_VAL_INEXT   ) { m_Flag.Set(PF_EXT_INP);  }
         else if( str == XML_4GR_ATTR_VAL_OUTEXT  ) { m_Flag.Set(PF_EXT_OUTP); }
         else if( str == XML_4GR_ATTR_VAL_INPUT   ) { m_Flag.Set(PF_IN);       }
         else if( str == XML_4GR_ATTR_VAL_OUTPUT  ) { m_Flag.Set(PF_OUT);      }
         else if( str == XML_4GR_ATTR_VAL_INOUTPUT) { m_Flag.Set(PF_INOUT);    }
         else {
            TRACE(_T("CPin: invalid class\n")); throw E_FAIL; 
         }
      } else {
         TRACE(_T("CPin: missing class\n")); throw E_FAIL; 
      }

      ar.GetAttribute( XML_4GR_ATTR_NAME,    (CString&)m_Name);
//    ar.GetAttribute( XML_4GR_ATTR_TYPE,    m_strType);
//    ar.GetAttribute( XML_4GR_ATTR_INITVAL, m_strIVal);
      if( !ar.GetAttribute( XML_4GR_ATTR_ORIGIN,  m_Pos) )
      {
         TRACE(_T("CPin: missing origin\n")); throw E_FAIL; 
      }

      if( ar.GetAttribute( XML_4GR_ATTR_NEGATE,   l) && l==1) m_Flag.Set(PF_NEGATE  ); else m_Flag.Clr(PF_NEGATE  );
      if( ar.GetAttribute( XML_4GR_ATTR_HIDDED,   l) && l==1) 
      {
         m_Flag.Set(PF_HIDDEN  ); 
      }
      else 
      {
         if( ar.GetAttribute( XML_4GR_ATTR_HIDDEN,   l) && l==1) 
         {
            m_Flag.Set(PF_HIDDEN  ); 
         } else {
            m_Flag.Clr(PF_HIDDEN  );
         }
      }
      if( ar.GetAttribute( XML_4GR_ATTR_DISABLE,  l) && l==1) m_Flag.Set(PF_DISABLED); else m_Flag.Clr(PF_DISABLED);
      if( ar.GetAttribute( XML_4GR_ATTR_R_EDGE,   l) && l==1) m_Flag.Set(PF_R_EDGE  ); else m_Flag.Clr(PF_R_EDGE  );
      if( ar.GetAttribute( XML_4GR_ATTR_F_EDGE,   l) && l==1) m_Flag.Set(PF_F_EDGE  ); else m_Flag.Clr(PF_F_EDGE  );
   }
}

IMPLEMENT_SERIAL(CPin, CXMLObject, SERIAL_VERSION)

int   CPin::GetConnects() 
{ 
   ASSERT (m_Entry);
   ASSERT (m_Entry->m_iSelected + m_Entry->m_iDeSelected + 
           m_Entry->m_iRubbLines == m_Entry->m_iConnects);
   if (m_Entry)
   {
      return m_Entry->m_iConnects;
   } else {
      return 1;
   }
}  

int   CPin::GetTemp() 
{ 
   ASSERT(m_Entry);
   if (m_Entry)
   {
      return m_Entry->m_iTemp;
   } else {
      return 0;
   }
}

//////////////////////////////////////////////////////////////////////
// CElem Class
//////////////////////////////////////////////////////////////////////

/*func*------------------------------------------------------------------------
  Construction/Destruction
  in :
  out:
-----------------------------------------------------------------------------*/
CElem::CElem()
{
	m_Origin     = CPoint(0,0);
   m_OriginOffs = CPoint(0,0);
	m_Size       = CSize(0,0);

   m_MinSize    = CSize(0,0);
   m_MaxSize    = CSize(MAX_XGRID,MAX_YGRID);

   SetType(ET_LAST);
   m_pPinTable  = NULL;
   m_pContain   = NULL;
}

CElem::CElem(CElem * pE)
{
   CopyData(pE);
}

CElem::~CElem()
{
   DeleteAllPin();
   if(m_pContain)
      m_pContain->DelGroupElem(this);
}

void CElem::CopyData(CElem * pE, bool bCopyPins)
{
   CPin *pPin;
   int   i;

   ASSERT(pE);
   ASSERT_VALID(pE);
	m_Flag = pE->m_Flag;        
   if( bCopyPins )
   {
      for ( i=0; i < pE->m_Con.GetSize(); i++ )
      {
         if( (pPin = new CPin(pE->m_Con.GetAt(i))) != NULL ) m_Con.Add(pPin);
      }
   }
	m_Origin     = pE->m_Origin;
	m_OriginOffs = pE->m_OriginOffs;
	m_Size       = pE->m_Size;
	m_MaxSize    = pE->m_MaxSize;
	m_MinSize    = pE->m_MinSize;

   SetType(pE->GetType());
   m_pPinTable  = NULL;
   m_pContain   = NULL;
}

/*func*------------------------------------------------------------------------
  delete all pins in m_Con
  in :-
  out:-
-----------------------------------------------------------------------------*/
void CElem::DeleteAllPin()
{
   CPin *pPin;

   while ( m_Con.GetSize() > 0 ) 
   { 
      pPin = m_Con.GetAt(0); 
      delete pPin; 
      pPin = NULL;
      m_Con.RemoveAt(0);  
   }
}

/*func*------------------------------------------------------------------------
  set flags at element 
  in :-
  out:-
-----------------------------------------------------------------------------*/
void CElem::SetFlag(TElemFlags flag)
{
   if (!m_Flag.Chk(flag))
   {
      m_Flag.Set(flag);
      if ((m_pPinTable != NULL) && (flag == EF_SELECT))
      {
         m_pPinTable->SelectPins(this, true);
      }
   }
}

/*func*------------------------------------------------------------------------
  clear flags at element 
  in :-
  out:-
-----------------------------------------------------------------------------*/
void CElem::ClrFlag(TElemFlags flag)
{
   if (m_Flag.Chk(flag))
   {
      m_Flag.Clr(flag);
      if ((m_pPinTable != NULL) && (flag == EF_SELECT))
      {
         m_pPinTable->SelectPins(this, false);
      }
   }
}

/*func*------------------------------------------------------------------------
  toggle flags at element 
  in :-
  out:-
-----------------------------------------------------------------------------*/
void CElem::TglFlag(TElemFlags flag)
{
   m_Flag.Tgl(flag);
   if ((m_pPinTable != NULL) && (flag == EF_SELECT))
   {
      if (m_Flag.Chk(EF_SELECT))
      {
         m_pPinTable->SelectPins(this, true);
      } else {
         m_pPinTable->SelectPins(this, false);
      }
   }
}

/*func*------------------------------------------------------------------------
  clear flags at element 
  in :-
  out:-
-----------------------------------------------------------------------------*/
bool CElem::ChkFlag(TElemFlags flag)
{
   return m_Flag.Chk(flag);
}

/*func*------------------------------------------------------------------------
  set flags at element 
  in :-
  out:-
-----------------------------------------------------------------------------*/
void CElem::SetFlag(CElemFlag flag)
{
   if (!m_Flag.Chk(flag))
   {
      m_Flag.Set(flag);
      if( (m_pPinTable != NULL) && flag.Chk(EF_SELECT) )
      {
         m_pPinTable->SelectPins(this, true);
      }
   }
}

/*func*------------------------------------------------------------------------
  clear flags at element 
  in :-
  out:-
-----------------------------------------------------------------------------*/
void CElem::ClrFlag(CElemFlag flag)
{
   if (m_Flag.Chk(flag))
   {
      m_Flag.Clr(flag);
      if( (m_pPinTable != NULL) && flag.Chk(EF_SELECT) )
      {
         m_pPinTable->SelectPins(this, false);
      }
   }
}


/*func*------------------------------------------------------------------------
  draw the element (placeholder (dummy) function - should never be called)
  in :pDC   - pointer to device context
      pView - pointer to document
  out:-
-----------------------------------------------------------------------------*/
void CElem::Draw(CDC * pDC, CGrEditorView * pView, int iGrid)
{
   CRect    r;
   PEN_TYPE PenType;
   bool     bScreen = !pDC->IsPrinting();

   if (!pView)
   {
      return;
   }
   if     ( ChkFlag(EF_COMMENT)   ) PenType = PE_COMMENT;
   else if( ChkFlag(EF_EMPHASIZE) ) PenType = PE_ERROR;
   else   /* normal */                 PenType = PE_NORMAL;

   pDC->SelectObject(GDIHelper::GetPen  (bScreen, PenType  ));
   if (pView->IsManualMode())
   {
      pDC->SelectObject(GDIHelper::GetBrush(bScreen, BT_ELEMBK_SFC));
   } else {
      pDC->SelectObject(GDIHelper::GetBrush(bScreen, BT_ELEMBK));
   }

   r.SetRect( m_Origin.x * iGrid,
              m_Origin.y * iGrid,
             (m_Origin.x + m_Size.cx) * iGrid + 1,
             (m_Origin.y + m_Size.cy) * iGrid + 1 );
   r.NormalizeRect();
   pDC->Rectangle( &r );
}

/*func*------------------------------------------------------------------------
  draw the (default) resize frame for the element 
  in :pDC   - pointer to device context
      pView - pointer to document
      iGrid - actual display resolution
  out:-
-----------------------------------------------------------------------------*/
void CElem::DrawSelectFrame(CDC * pDC, CGrEditorView * pView, int iGrid, 
                            bool bWithResizePoints)
{
   int    iX1, iX2, iY1, iY2;
   CRect  r;
   bool   bScreen = !pDC->IsPrinting();

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
      pDC->LineTo( iX2, iY1);
      pDC->LineTo( iX2, iY2);
      pDC->LineTo( iX1, iY2);
      pDC->LineTo( iX1, iY1);

      if (bWithResizePoints)
      {
         // draw resize points
         PEN_TYPE PenType;

         if     ( ChkFlag(EF_COMMENT)   ) PenType = PE_COMMENT;
         else if( ChkFlag(EF_EMPHASIZE) ) PenType = PE_ERROR;
         else   /* normal */                 PenType = PE_NORMAL;

         pDC->SelectObject(GDIHelper::GetPen  (bScreen, PenType  ));
         pDC->SelectObject(GDIHelper::GetBrush(bScreen, BT_SELECT)); 

         pDC->Rectangle( GetResizeRect(RS_NW, r, iGrid) );
         pDC->Rectangle( GetResizeRect(RS_N , r, iGrid) );
         pDC->Rectangle( GetResizeRect(RS_NE, r, iGrid) );
         pDC->Rectangle( GetResizeRect(RS_E , r, iGrid) );
         pDC->Rectangle( GetResizeRect(RS_SE, r, iGrid) );
         pDC->Rectangle( GetResizeRect(RS_S , r, iGrid) );
         pDC->Rectangle( GetResizeRect(RS_SW, r, iGrid) );
         pDC->Rectangle( GetResizeRect(RS_W , r, iGrid) );
      }
   } 
}

/*func*------------------------------------------------------------------------
  draw the pin at element
  in :pDC       - pointer to device context
      pos       - position for icon (left-top-corner of icon) in raster coords
      nIDBitmap - ident of icon (bitmap)
      iGrid     - actual display resolution
  ret:cx != 0:  icon was drawn (result == Size)
      cx == 0:  not drawn
-----------------------------------------------------------------------------*/
CSize CElem::DrawIcon(CDC * pDC, CGrEditorView * pView, CPoint pos, 
                     UINT nIDBitmap, int iGrid)
{
   LPBITMAPINFO   lpbi;
   bool           bScreen = !pDC->IsPrinting();
   CSize size(0,0);

   if (!pView)
   {
      return false;
   }
   lpbi = GDIHelper::GetBmInfo(bScreen, nIDBitmap, ChkFlag(EF_COMMENT),
             pView->IsManualMode());

   if( lpbi == NULL ) {
      return size;   // FALSE
   }

   size.cx = 2*iGrid;
   size.cy = size.cx;
   // 16 colors fix!
   StretchDIBits((HDC)*pDC, 
                 pos.x * iGrid, pos.y * iGrid, 2*iGrid, 2*iGrid, 
                 0, 0, lpbi->bmiHeader.biWidth, lpbi->bmiHeader.biHeight,
                 (PBYTE)(&lpbi->bmiColors[16]), lpbi,
                 DIB_RGB_COLORS, SRCCOPY);

   return size;
}

/*func*------------------------------------------------------------------------
  draw all pins of element
  in :pDC   - pointer to device context
      iGrid - actual display resolution
  out:-
-----------------------------------------------------------------------------*/
void CElem::DrawPins(CDC * pDC, int iGrid)
{
   int   i;
   CPin *pPin;

   ASSERT(pDC);
   ASSERT(iGrid > 0);

   if( pDC && (iGrid > 0) )
   {
      for( i=0; i<m_Con.GetSize(); i++ )
      {
         if( (pPin = m_Con.GetAt(i)) != NULL )
         {
            pPin->Draw(pDC, iGrid);
         }
      }
   }
}

/*func*------------------------------------------------------------------------
  draw all pins of element
  in :pDC   - pointer to device context
      iGrid - actual display resolution
  out:-
-----------------------------------------------------------------------------*/
void CElem::DrawLightPins(CDC * pDC, int iGrid)
{
   int   i;
   CPin *pPin;

   ASSERT(pDC);
   ASSERT(iGrid > 0);

   if( ChkFlag(EF_LIGHTPIN) && pDC && (iGrid > 0) )
   {
      for( i=0; i<m_Con.GetSize(); i++ )
      {
         if( (pPin = m_Con.GetAt(i)) != NULL )
         {
            if (pPin->m_Flag.Chk(PF_LIGHTPIN))
            {
               pPin->DrawLightPin(pDC, iGrid);
            }
         }
      }
   }
}

/*func*------------------------------------------------------------------------
  get actual foreground color constant
  in :-
  out:CO_xxx constant
-----------------------------------------------------------------------------*/
COLOR_TYPE CElem::GetColorType(void)
{
   if     (ChkFlag(EF_COMMENT    )) 
   {
      return CO_COMMENT;
   } 
   else if(ChkFlag(EF_SFC_FORCED )) 
   {
      return CO_FORCED_SFC;
   } 
   else if(ChkFlag(EF_SFC_BLOCKED)) 
   {
      return CO_BLOCKD_SFC;
   } 
   else 
   {
      return CO_NORMAL;
   }
}


/*func*------------------------------------------------------------------------
  find text in element - pins, search from given position for/backward with 
  matching lower of upper cases
  in :iElemNr  - neg. numbers are reserved for DERIVED classes
                 pos. numbers are the pins in m_con
      iPos     - position in strname in pin
      lpszFind - search text
      bNext    - search direction
      bCase    - flag for matching cases
  out:TRUE  - string found (iElemNr and iPos will be updated)
      FALSE - nothing found
-----------------------------------------------------------------------------*/
BOOL CElem::FindText(SearchDat& SD)
{
   LPCTSTR  psz;
   CPin    *pPin;

   int iCount = m_Con.GetSize();
   if ( iCount <= 0 )
   {
      return FALSE;
   }
   if ( SD.m_bNext )
   {
      if ( SD.m_iFindElem >= iCount ) 
      {
         return FALSE;
      }
      if ( SD.m_iFindElem < 0) 
      {
         SD.m_iFindElem = 0;
      }
   } 
   else
   {
      if ( SD.m_iFindElem < 0 ) 
      {
         return FALSE;
      }
      if ( SD.m_iFindElem >= iCount )
      {
         SD.m_iFindElem = iCount - 1;
      }
   }

   int iL = SD.m_strFind.GetLength();
   if( SD.m_bNext )
   {
      for( ; SD.m_iFindElem < m_Con.GetSize(); SD.m_iFindElem++ ) 
      {
         pPin = m_Con.GetAt(SD.m_iFindElem);
         ASSERT_VALID(pPin);
         if( pPin && (SD.m_bFindOnly || pPin->m_Name.m_Flag.Chk(TF_EDITABLE)))
         {
            psz  = (LPCTSTR)pPin->m_Name;
            SD.m_iFindLen = pPin->m_Name.GetLength();//(int)_tcslen(psz);
            
            if( SD.m_iFindPos < 0 ) 
               SD.m_iFindPos = 0;

            if( SD.m_iFindLen >= iL )
            {
               for( ; SD.m_iFindPos <= SD.m_iFindLen - iL; SD.m_iFindPos++ )
               {
                  if( SD.m_bCase ?
                        ( _tcsncmp (psz + SD.m_iFindPos, SD.m_strFind, iL) == 0 ) :
                        ( _tcsnicmp(psz + SD.m_iFindPos, SD.m_strFind, iL) == 0 ) )
                  {
                     SD.m_iFindLen = SD.m_strFind.GetLength();//(int)_tcslen(SD.m_strFind);
                     return TRUE;
                  }
               }
            }
            SD.m_iFindPos = 0;
         }
      }
   } else {
      for( ; SD.m_iFindElem >= 0 && SD.m_iFindElem < m_Con.GetSize(); SD.m_iFindElem-- ) 
      {
         pPin = m_Con.GetAt(SD.m_iFindElem);
         ASSERT_VALID(pPin);
         if( pPin && (SD.m_bFindOnly || pPin->m_Name.m_Flag.Chk(TF_EDITABLE)))
         {
            psz  = (LPCTSTR)pPin->m_Name;
            SD.m_iFindLen = (int)_tcslen(psz);
            if(SD.m_iFindPos > SD.m_iFindLen) 
               SD.m_iFindPos = SD.m_iFindLen - iL;

            for( ; SD.m_iFindPos >= 0; SD.m_iFindPos-- )
            {
               if( SD.m_bCase?
                     ( _tcsncmp (psz + SD.m_iFindPos, SD.m_strFind, iL) == 0 ) :
                     ( _tcsnicmp(psz + SD.m_iFindPos, SD.m_strFind, iL) == 0 ) )
               {
                  SD.m_iFindLen = SD.m_strFind.GetLength();//(int)_tcslen(SD.m_strFind);
                  return TRUE;
               }
            }
            SD.m_iFindPos = INT_MAX;   // a big integer ..
         }
      }
   }

   SD.m_iFindLen = 0;
   return FALSE;
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
bool CElem::GetText(int iElemNr, int iPos, int iLen, CString& str)
{
   CPin  *pPin;
   int    i;

   str = _T("");

   if (iElemNr < 0)
   {
      return (false);
   }

   i = m_Con.GetSize();
   if ( (i <= 0) || (i < iElemNr) )
   {
      return (false);
   }

   pPin = m_Con.GetAt(iElemNr);
   ASSERT_VALID(pPin);
   if( pPin )
   {
      str = (pPin->m_Name.Str()).Mid(iPos, iLen);
      return (true);
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
void CElem::ReplaceSelText(int iElemNr, int iPos, int iLen, LPCTSTR pstr)
{
   CPin  *pPin;
   int    i;

   if (iElemNr < 0)
   {
      return;
   }

   i = m_Con.GetSize();
   if ( (i <= 0) || (i < iElemNr) )
   {
      return;
   }

   pPin = m_Con.GetAt(iElemNr);
   ASSERT(pPin);
   if( pPin )
   {
      (pPin->m_Name.StrRef()).Delete(iPos, iLen);
      (pPin->m_Name.StrRef()).Insert(iPos, pstr);
   }
}

/*func*------------------------------------------------------------------------
  get the resize rect for wished position
  in :eRectTyp - wished position of rect
      r        - place holder for rect
      iGrid    - actual zoom
  out:pointer to rect for draw function with logical coordinates
-----------------------------------------------------------------------------*/
CRect *CElem::GetResizeRect(RS_TYPE eRectTyp, CRect& r, int iGrid)
{
   CRect rr;

   GetRect(rr, iGrid);
   switch ( eRectTyp ) {
      case RS_NW:
         r.SetRect( rr.left, 
                    rr.top, 
                    rr.left, 
                    rr.top );
         goto m_setsize;

      case RS_N :
         r.SetRect( rr.left + rr.Width()/2, 
                    rr.top, 
                    rr.left + rr.Width()/2, 
                    rr.top );
         goto m_setsize;

      case RS_NE:
         r.SetRect( rr.right,
                    rr.top, 
                    rr.right,
                    rr.top );
         goto m_setsize;

      case RS_E :
         r.SetRect( rr.right,
                    rr.top  + rr.Height()/2, 
                    rr.right,
                    rr.top  + rr.Height()/2 );
         goto m_setsize;

      case RS_SE:
         r.SetRect( rr.right,
                    rr.bottom,
                    rr.right,
                    rr.bottom );
         goto m_setsize;

      case RS_S :
         r.SetRect( rr.left + rr.Width()/2, 
                    rr.bottom,
                    rr.left + rr.Width()/2, 
                    rr.bottom );
         goto m_setsize;

      case RS_SW:
         r.SetRect( rr.left,
                    rr.bottom, 
                    rr.left,
                    rr.bottom );
         goto m_setsize;

      case RS_W :
         r.SetRect( rr.left, 
                    rr.top  + rr.Height()/2, 
                    rr.left, 
                    rr.top  + rr.Height()/2 );
        m_setsize:
         r.InflateRect( SP_RAD, SP_RAD ); 
         break;

      default:
         r.SetRectEmpty();
         break;
   }
   return &r;
}

/*func*------------------------------------------------------------------------
  get the resize (point) position at sheet for wished resize corner
  in :eMode - wished resize corner
      p     - actual position of cursor in raster coords
  out:-
     (p - with new coords at screen)
-----------------------------------------------------------------------------*/
void CElem::GetResizePos(RS_TYPE eMode, CPoint& p)
{
#define  OX   (m_Origin.x)
#define  OY   (m_Origin.y)
#define  SX   (m_Size.cx)
#define  SY   (m_Size.cy)
#define  HX   (m_Size.cx/2)
#define  HY   (m_Size.cy/2)

   switch ( eMode ) {
      case RS_NW: p = CPoint( OX     , OY      ); break;
      case RS_N : p = CPoint( OX + HX, OY      ); break;
      case RS_NE: p = CPoint( OX + SX, OY      ); break;
      case RS_E : p = CPoint( OX + SX, OY + HY ); break;
      case RS_SE: p = CPoint( OX + SX, OY + SY ); break;
      case RS_S : p = CPoint( OX + HX, OY + SY ); break;
      case RS_SW: p = CPoint( OX     , OY + SY ); break;
      case RS_W : p = CPoint( OX     , OY + HY ); break;
   }

#undef  OX
#undef  OY
#undef  SX
#undef  SY
#undef  HX
#undef  HY
}

/*func*------------------------------------------------------------------------
  get the resize rect for wished position
  in :r        - place holder for rect
      iGrid    - actual zoom
  out:pointer to rect for draw function with logical coordinates
-----------------------------------------------------------------------------*/
void CElem::GetRect(CRect& r, int iGrid)
{
   ASSERT(iGrid > 0);

   if (iGrid > 0)
   {
      r.SetRect( m_Origin.x * iGrid,
                 m_Origin.y * iGrid,
                (m_Origin.x + m_Size.cx) * iGrid,
                (m_Origin.y + m_Size.cy) * iGrid );
      r.NormalizeRect();
   }
}

/*func*------------------------------------------------------------------------
  get the resize rect for wished position wiht select frame
  in :r        - place holder for rect
      iGrid    - actual zoom
  out:-
-----------------------------------------------------------------------------*/
void CElem::GetBoundingRect(CRect& r, int iGrid)
{
   GetRect(r, iGrid);
   r.InflateRect(SP_RAD+1,SP_RAD+1);  // increase for selection
}

/*func*------------------------------------------------------------------------
  get the body rect without pin range for select operations
  in :r        - place holder for rect
      iGrid    - actual zoom
  out:-
-----------------------------------------------------------------------------*/
void CElem::GetBodyRect(CRect& r, int iGrid)
{
   GetRect(r, iGrid);
}

/*func*------------------------------------------------------------------------
  test if p at resize rect in element
  in :p     - point for test in logical coordinates at sheet
      iGrid - actual zoom
  out:TRUE  - yes on resize rect
      FALSE - not in resize rect
-----------------------------------------------------------------------------*/
bool CElem::IsResizeRect(CPoint p, int iGrid)
{
   CRect r;

   if( GetResizeRect( RS_NW, r, iGrid )->PtInRect( p ) ||
       GetResizeRect( RS_N , r, iGrid )->PtInRect( p ) ||
       GetResizeRect( RS_NE, r, iGrid )->PtInRect( p ) ||
       GetResizeRect( RS_E , r, iGrid )->PtInRect( p ) ||
       GetResizeRect( RS_SE, r, iGrid )->PtInRect( p ) ||
       GetResizeRect( RS_S , r, iGrid )->PtInRect( p ) ||
       GetResizeRect( RS_SW, r, iGrid )->PtInRect( p ) ||
       GetResizeRect( RS_W , r, iGrid )->PtInRect( p ) )
   {
      return true;
   } else {
      return false;
   }
}

/*func*------------------------------------------------------------------------
  test if p at resize rect in element
  in :p     - point for test in logical coordinates at sheet
      iGrid - actual zoom
  out:resize type at elem
      RS_LAST - no resize corner 
-----------------------------------------------------------------------------*/
RS_TYPE CElem::GetResizeType(CPoint p, int iGrid)
{
   CRect r;

   if( GetResizeRect( RS_NW, r, iGrid )->PtInRect( p ) ) return RS_NW;
   if( GetResizeRect( RS_N , r, iGrid )->PtInRect( p ) ) return RS_N;
   if( GetResizeRect( RS_NE, r, iGrid )->PtInRect( p ) ) return RS_NE;
   if( GetResizeRect( RS_E , r, iGrid )->PtInRect( p ) ) return RS_E;
   if( GetResizeRect( RS_SE, r, iGrid )->PtInRect( p ) ) return RS_SE;
   if( GetResizeRect( RS_S , r, iGrid )->PtInRect( p ) ) return RS_S;
   if( GetResizeRect( RS_SW, r, iGrid )->PtInRect( p ) ) return RS_SW;
   if( GetResizeRect( RS_W , r, iGrid )->PtInRect( p ) ) return RS_W;

   return RS_LAST;
}

/*func*------------------------------------------------------------------------
  resize and/or move element with connector pins
  in :dx - delta x for m_Origin
      dy - delta y for m_Origin
      sx - delta x for m_Size
      sy - delta y for m_Size
  out:TRUE  - success 
      FALSE - operation not executed or coordinates limited
-----------------------------------------------------------------------------*/
bool CElem::Resize(int dx, int dy, int sx, int sy)
{
   CPin *pPin;
   int   i;
   bool  bRes;

   if( (m_MinSize.cx <= m_Size.cx + sx) &&
       (m_MinSize.cy <= m_Size.cy + sy) &&
       (m_MaxSize.cx >= m_Size.cx + sx) &&
       (m_MaxSize.cy >= m_Size.cy + sy) &&
       ((int)((m_Origin.x + dx) & 0x00000001) == m_OriginOffs.x) &&
       ((int)((m_Origin.y + dy) & 0x00000001) == m_OriginOffs.y) )
   {
      bRes = true;
      // move connect points (pins)
      for( i=0; i<m_Con.GetSize(); i++ )
      {
         pPin = m_Con.GetAt(i);

         if( pPin )
         {
            if( pPin->m_Flag.Chk(PF_CENTER_X) )
            { // center pins at top/bottom edge for sfc ..
               pPin->m_Pos.x  = (m_Origin.x + dx + (m_Size.cx + sx)/2) & 0xFFFFFFFE;
            } else if( (pPin->m_Pos.x != m_Origin.x + m_Size.cx) ||
                       (pPin->m_Pos.y == m_Origin.y) ) {
               pPin->m_Pos.x +=  dx;
            } else {
               pPin->m_Pos.x += (dx + sx);
            }

            if( pPin->m_Flag.Chk(PF_CENTER_Y) )
            { // center pins at left/right edge for sfc ..
               pPin->m_Pos.y  = (m_Origin.y + dy + (m_Size.cy + sy)/2) & 0xFFFFFFFE;
            } else if( (pPin->m_Pos.y != m_Origin.y + m_Size.cy ) ||
                       (pPin->m_Pos.x == m_Origin.x) ) {
               pPin->m_Pos.y +=  dy;
            } else {
               pPin->m_Pos.y += (dy + sy);
            }

            if (m_pPinTable)
            {
               m_pPinTable->MovePin(this, pPin->m_Entry, pPin->m_Pos);
            }
         }
      }
      // resize base element
      m_Origin.x += dx;
      m_Origin.y += dy;  
      m_Size.cx  += sx;
      m_Size.cy  += sy;  
   } else {
      bRes = false;
   }

   return bRes;
}

/*func*------------------------------------------------------------------------
  search for connector at element
  in :p - search point in raster coords
  out:NULL - no connector found at this point
      pointer to connector
-----------------------------------------------------------------------------*/
CPin * CElem::SearchConn(CPoint p)
{
   CPin  *pPin;
   int    i;

   for( i=0; i<m_Con.GetSize();  i++ ) 
   {
      pPin = m_Con.GetAt(i);
      ASSERT(pPin);
      if(pPin->m_Pos == p)
      {
         return pPin; 
      }
   }
   return NULL;
}

/*func*------------------------------------------------------------------------
  check for edge with pin(s) and the posibility of connect or end of lines and
  other pin(s)
  in :p - search point in raster coords
  out:false - connect possible at this point
      true  - connect possible at this point
-----------------------------------------------------------------------------*/
bool CElem::IsConnectable(CPoint p)
{
   CPin  *pPin;
   int    i;

   if (m_Flag.Chk(EF_RUBBLINE))
   {
      if (p == m_Origin || p == m_Origin + m_Size)
      {
         return true;
      }
   } 
   else 
   {
      if (GetType() == ET_LINE)
      {
         return true;   // in all positions true !
      } 
      else 
      {
         for( i=0; i<m_Con.GetSize(); i++)
         {
            pPin = m_Con.GetAt(i);
            ASSERT_VALID(pPin);

            if( pPin )
            {
               if (pPin->m_Pos == p)
               { 
                  return true;
               }
               if(     (pPin->m_Size.cx > 0) && (p.x == m_Origin.x)) 
               { 
                  ASSERT(pPin->m_Pos.x == m_Origin.x);
                  return true;
               } 
               else if((pPin->m_Size.cx < 0) && (p.x == m_Origin.x + m_Size.cx))
               { 
                  ASSERT(pPin->m_Pos.x == m_Origin.x + m_Size.cx);
                  return true;
               } 
               else if((pPin->m_Size.cy > 0) && (p.y == m_Origin.y))
               { 
                  ASSERT(pPin->m_Pos.y == m_Origin.y);
                  return true;
               } 
               else if((pPin->m_Size.cy < 0) && (p.y == m_Origin.y + m_Size.cy))
               { 
                  ASSERT(pPin->m_Pos.y == m_Origin.y + m_Size.cy);
                  return true;
               }
            }
         }
      }
   }

   return false;
}


/*func*------------------------------------------------------------------------
  checks for spacing possibility
  in :Point - spacing origin position
      Delta - requested spacing amount
  out:false - not allowed at this position
      true  - allowed
-----------------------------------------------------------------------------*/
bool CElem::IsSpaceable(CPoint At, CPoint Delta, bool /*bLabelClip*/)
{
   if(Delta.x < 0)
   {
      int i1 = min(m_Origin.x, m_Origin.x  + m_Size.cx);
      int i2 = max(m_Origin.x, m_Origin.x  + m_Size.cx);
      
      if( Delta.y != 0 || At.x >= i1 && At.x <= i2)
         return false;
   }else

   if(Delta.y < 0)
   {
      int i1 = min(m_Origin.y, m_Origin.y  + m_Size.cy);
      int i2 = max(m_Origin.y, m_Origin.y  + m_Size.cy);
      
      if(Delta.x != 0 || At.y >= i1 && At.y <= i2)
         return false;
   }
   return true;//!( Delta.x < 0 || Delta.y < 0 );
}


/*func*------------------------------------------------------------------------
  execute spacing (doesn't perform any possibility checks)
  (at the moment only in one direction (Delta.x^Delta.y) )
  in :Point - spacing origin position
      Delta - spacing amount
  out:*paPin- list of pins needing line insertion
      false - not alloed at this position
      true  - allowed
-----------------------------------------------------------------------------*/
bool CElem::Spacing(CPoint At, CPoint Delta, CPosArray* paPin, bool /*bLabelClip*/)
{
   int  i1;                // min element extension in raster coords
   int  i2;                // max element extension in raster coords
   bool bDone = false;     // true if any movement done
   bool bRes  = true;
   int  d;                 // ammount of movement

   ASSERT( (Delta.x == 0) != (Delta.y == 0) );                    // either x or y direction (not both)

   if( (d = Delta.x) > 0 )
   {
      if(m_Con.GetSize())                                         // the element has pins?
      {
         i1 = m_Con[0]->m_Pos.x;
         i2 = i1;

         for(int iPin=0; iPin<m_Con.GetSize(); iPin++)            // get pin range
         {
            CPin* pPin = m_Con[iPin];

            if(i1>pPin->m_Pos.x)
               i1=pPin->m_Pos.x;
            if(i2<pPin->m_Pos.x)
               i2=pPin->m_Pos.x;
         }
         if(i1==i2 && i1==GetOrigin().x+GetSize().cx)
         {
            i1=GetOrigin().x;
         }
      }else
      {
         i1 = min( m_Origin.x , m_Origin.x + m_Size.cx );
         i2 = max( m_Origin.x , m_Origin.x + m_Size.cx );
      }

      if( At.x > i1 )                                             // not to move the entire element
      {
         if(At.x <= i2)                                           // point inside or at right border
         {
            if(paPin)
            {
               for(int iPin=0; iPin<m_Con.GetSize(); iPin++)
               {
                  CPin* pPin = m_Con[iPin];
                  int iCount = pPin->GetTemp();

                  if ((pPin->m_Pos.x >= At.x) && 
                      (iCount > 1))                               // saved value of m_iConnets
                  {
                     TPinEntry* pPE;
                     if(m_pPinTable->Lookup(pPin->m_Pos, pPE))
                     {
                        if(iCount - pPE->m_iRubbLines > 1)
                        {
                           paPin->AddPoint(pPin->m_Pos);
                        }
                     }
                  }
               }
            }
            bDone = true;
         }
      } else
      {                                                           // element is right
         bRes &= Resize(d,0,0,0);                                 //    shift it
         bDone = true;
      }
   }

   if( (d = Delta.y) > 0 )
   {
      if(m_Con.GetSize())                                         // the element has pins?
      {
         i1 = m_Con[0]->m_Pos.y;                               
         i2 = i1;

         for(int iPin=0; iPin<m_Con.GetSize(); iPin++)            // get pin range
         {
            CPin* pPin = m_Con[iPin];
            if(i1>pPin->m_Pos.y)
               i1=pPin->m_Pos.y;
            if(i2<pPin->m_Pos.y)
               i2=pPin->m_Pos.y;
         }
      }else
      {
         i1 = min(m_Origin.y, m_Origin.y + m_Size.cy);            // no pins: use bounding box
         i2 = max(m_Origin.y, m_Origin.y + m_Size.cy);
      }

      for(int i=0; i < m_Con.GetSize(); i++)
      {
         if(m_Con[i]->GetTemp() > 1)                              // saved value of m_iConnets
         {
            int y = m_Con[i]->m_Pos.y;
            if(i1 > y)
               i1 = y;
            else
               if(i2 < y)
                  i2 = y;
         }
      }

      if( At.y > i1 )                                             // not to move the entire element
      {
         if( At.y <= i2)                                          // point inside or at right border
         {
            if(paPin)
            {
               for(int iPin=0; iPin<m_Con.GetSize(); iPin++)
               {
                  CPin* pPin = m_Con[iPin];
                  if ((pPin->m_Pos.y >= At.y) && 
                      (pPin->GetTemp() > 1))                      // saved value of m_iConnects
                  {
                     paPin->AddPoint(pPin->m_Pos);
                  }
               }
            }
            bDone = true;
         }
      }else
      {                                                           // element is below
         bRes &= Resize( 0,d, 0,0);                               //    shift it
         bDone = true;
      }
   }

   //---------- negative spacing
   if( (d = Delta.x) < 0 )
   {
      i1 = min( m_Origin.x , m_Origin.x + m_Size.cx );
//      i2 = max( m_Origin.x , m_Origin.x + m_Size.cx );
      if(At.x < i1)
         Resize( d, 0, 0, 0);
      bDone = true;
   }

   if( (d = Delta.y) < 0 )
   {
      i1 = min( m_Origin.y , m_Origin.y + m_Size.cy );
      if(At.y < i1)
         Resize( 0, d, 0, 0);
      bDone = true;
   }

   ASSERT(bRes);
   return bDone;
}

/*func*------------------------------------------------------------------------
  search for connector at element
  in :p - search point in raster coords
  out:NULL - no connector found at this point
      pointer to connector
-----------------------------------------------------------------------------*/
CPin * CElem::SearchPin(CPoint p, int iGrid)
{
   CRect  r;
   CPin  *pPin;
   int    i;

   for( i=0; i<m_Con.GetSize();  i++ ) 
   {
      pPin = m_Con.GetAt(i);
      ASSERT_VALID( pPin );
      if( pPin != NULL) 
      {
         r.left   = iGrid * (pPin->m_Pos.x);
         r.right  = iGrid * (pPin->m_Pos.x + pPin->m_Size.cx);
         r.top    = iGrid * (pPin->m_Pos.y);
         r.bottom = iGrid * (pPin->m_Pos.y + pPin->m_Size.cy);
         r.NormalizeRect();
         r.InflateRect(iGrid,iGrid);

         if (r.PtInRect (p))
         {
            return pPin; 
         }
      }
   }
   return NULL;
}

/*func*------------------------------------------------------------------------
  get the data identifier of the element
  in :p     - grip position for test
      start - position for identify (origin)
  out:identifier of the element
-----------------------------------------------------------------------------*/
CText& CElem::GetTextAt(CPoint /*p*/, CPoint &start, int /*iGrid*/)
{
   static CText text(_T(""));

   start = m_Origin;
   return text;
}

/*func*------------------------------------------------------------------------
  get the text space in elem
  in :rect - placeholder for result
  out:rect - filled with text doc coords
-----------------------------------------------------------------------------*/
void CElem::GetTextRect(int /*iTxtIdx*/, CRect &rect, int iGrid)
{
   rect.SetRect((m_Origin.x            )*iGrid, 
                (m_Origin.y            )*iGrid,
                (m_Origin.x + m_Size.cx)*iGrid, 
                (m_Origin.y + m_Size.cy)*iGrid); 
}

/*func*------------------------------------------------------------------------
  search text rect for point p in doc coords
  in :p    - search point in  coords
      rect - placeholder for text rect in coords
  out:true  - rect with doc coords
      false - empty rect
-----------------------------------------------------------------------------*/
bool CElem::SearchTextRect(CPoint p, CRect &rect, int iGrid)
{ 
   for (int i=GetMinTxtIdx(); i<GetMaxTxtIdx(); i++)
   {
      GetTextRect(i, rect, iGrid);

      if (rect.PtInRect(p) == TRUE)
      { 
         return  true;
      }
   }
   return false;
}

/*func*------------------------------------------------------------------------
  check (limit) and set the origin of the element
  in :origin - the new position
  out:updated (limited) origin
-----------------------------------------------------------------------------*/
void CElem::SetOrigin(CPoint & origin)
{
   // limit range on sheet
   if( origin.x <  0         ) origin.x = 0;
   if( origin.x >= MAX_XGRID ) origin.x = MAX_XGRID-1;
   if( origin.y <  0         ) origin.y = 0;
   if( origin.y >= MAX_YGRID ) origin.y = MAX_YGRID-1;
   // check new origin
   origin.x = (origin.x & 0xFFFFFFFE) + m_OriginOffs.x;
   origin.y = (origin.y & 0xFFFFFFFE) + m_OriginOffs.y;
   // set checked value
   m_Origin = origin;  
}

/*func*------------------------------------------------------------------------
  check (limit) and set the size of the element
  in :size - the new position
  out:updated (limited) size
-----------------------------------------------------------------------------*/
void CElem::SetSize(CSize & size)
{
   if( size.cx >= 0)
   {
      if( (m_Origin.x + size.cx) > MAX_XGRID ) size.cx = MAX_XGRID-1 - m_Origin.x;
      if(  size.cx < m_MinSize.cx ) size.cx =  m_MinSize.cx;
      if(  size.cx > m_MaxSize.cx ) size.cx =  m_MaxSize.cx;
   } else {
      if( (m_Origin.x + size.cx) < 0         ) size.cx = m_Origin.x;
      if( -size.cx < m_MinSize.cx ) size.cx = -m_MinSize.cx;
      if( -size.cx > m_MaxSize.cx ) size.cx = -m_MaxSize.cx;
   }
   if( size.cy >= 0)
   {
      if( (m_Origin.y + size.cy) > MAX_YGRID ) size.cy = MAX_YGRID-1 - m_Origin.y;
      if(  size.cy < m_MinSize.cy ) size.cy =  m_MinSize.cy;
      if(  size.cy > m_MaxSize.cy ) size.cy =  m_MaxSize.cy;
   } else {
      if( (m_Origin.y + size.cy) < 0         ) size.cy = m_Origin.y;
      if( -size.cy < m_MinSize.cy ) size.cy = -m_MinSize.cy;
      if( -size.cy > m_MaxSize.cy ) size.cy = -m_MaxSize.cy;
   }
   m_Size = size; 
}

/*func*------------------------------------------------------------------------
  set the minimum and maximum size for the element
  in :Min - min. size
  out:-
-----------------------------------------------------------------------------*/
void CElem::SetMinSize(CSize& Min)
{
   m_MinSize = Min; 
}

/*func*------------------------------------------------------------------------
  set the minimum and maximum size for the element
  in :Max - max. size
  out:-
-----------------------------------------------------------------------------*/
void CElem::SetMaxSize(CSize& Max)
{
   m_MaxSize = Max; 
}

/*func*------------------------------------------------------------------------
  serialize the element to/from stream
  in :ar - the archive
  out:-
-----------------------------------------------------------------------------*/
void CElem::Serialize(CArchive & ar)
{
   CPin *pPin;
   int   i;

   CXMLObject::Serialize(ar);

	if (ar.IsStoring())
	{
      ar << m_MaxSize << m_MinSize << m_Origin << m_OriginOffs << m_Size << m_Flag;

      ar << m_Con.GetSize();
      for( i=0; i<m_Con.GetSize(); i++ ) ar << m_Con.GetAt(i);
	} else {
      ar >> m_MaxSize >> m_MinSize >> m_Origin >> m_OriginOffs >> m_Size >> m_Flag;

      DeleteAllPin();
      ar >> i;
      for( ; i>0; i-- ) { ar >> pPin; m_Con.Add(pPin); }
	}
}

/*func*------------------------------------------------------------------------
  serialize the element to/from XML stream
   serialize here only attributs, which exists at ALL elements!
  in :ar - the archive
  out:-
-----------------------------------------------------------------------------*/
void CElem::SerializeXML(CXMLArchive &ar)
{
   if(ar.IsStoring())
   {
      ar.SetAttribute( XML_4GR_ATTR_ORIGIN, m_Origin);
      ar.SetAttribute( XML_4GR_ATTR_SIZE,   m_Size);

      if( m_Flag.Chk(EF_COMMENT) ) 
      {
         ar.SetAttribute( XML_4GR_ATTR_COMMENT,  1L);
      }
//    OBJCOMMENT
   } else {
      CSize  size;
      CPoint p;     
      long   l;

      if( ar.GetAttribute( XML_4GR_ATTR_ORIGIN, p) )
      {
         SetOrigin(p);
      } else {
         TRACE(_T("CElem: missing origin\n"));
         throw E_FAIL; 
      }

      if( ar.GetAttribute( XML_4GR_ATTR_SIZE, p) )
      {
         size.cx = p.x;
         size.cy = p.y;
         SetSize(size);
      } else {
         TRACE(_T("CElem: missing size\n"));
         throw E_FAIL; 
      }
      if( ar.GetAttribute( XML_4GR_ATTR_COMMENT,  l) && l==1 ) 
      {
         m_Flag.Set(EF_COMMENT);
      }
//    OBJCOMMENT
   }
}

/*func*------------------------------------------------------------------------
  serialize the m_Com to/from XML stream
  in :ar - the archive
  out:-
-----------------------------------------------------------------------------*/
void CElem::SerializeXMLCon(CXMLArchive &ar)
{
   if(ar.IsStoring())
   {
      if( ar.MoveDown( XML_4GR_ELEM_PINLIST ) )
      {
         ar << &m_Con;
         ar.MoveUp();  // reposition to same node before returning !!
      } else {
         TRACE(_T("CElemXXXX: error at streaming pinlist\n"));
         throw E_FAIL; 
      }
   } else {
      DeleteAllPin();
      if( ar.MoveDown( XML_4GR_ELEM_PINLIST ) ) 
      {
         ar >> &m_Con;
         ar.MoveUp();  // reposition to same node before returning !!
      } else {
         TRACE(_T("CElemXXXX: missing pinlist\n"));
         throw E_FAIL; 
      }
   }
}


/*func*------------------------------------------------------------------------
  tests the *pE position (returns true if allowed)
  in : Dir - relative direction of pE
       pE  - pointer to the element to be tested
  out: always true (to be overwritten)
-----------------------------------------------------------------------------*/
bool CElem::JoinTest(TDirection /*Dir*/, CElem* /*pE*/)
{ 
   return true; 
}

bool CElem::CheckDropIn(CElem* /*pE*/)
{ 
   return false; 
}

bool CElem::CheckDropOut()
{ 
   return false; 
}

void CElem::SetContainer(CElemContain *pCont) 
{ 
   m_pContain = pCont; 
}

IMPLEMENT_SERIAL(CElem, CXMLObject, SERIAL_VERSION)

//////////////////////////////////////////////////////////////////////
// CElemContain Class
//////////////////////////////////////////////////////////////////////

/*func*------------------------------------------------------------------------
  Construction/Destruction
  in :
  out:
-----------------------------------------------------------------------------*/
CElemContain::CElemContain()
{
   SetFlag(EF_CONTAIN);
}

CElemContain::~CElemContain()
{
   for(int i = 0; i<m_Group.GetSize(); i++)
   {
      m_Group.GetAt(i)->SetContainer(NULL);
   }
}

/*func*------------------------------------------------------------------------
  serialize the element to/from stream
  in :ar - the archive
  out:-
-----------------------------------------------------------------------------*/
bool CElemContain::IsMyElem(CElem *pE)
{
   int ex1, ex2, ey1, ey2;
   int tx1, tx2, ty1, ty2;

   // container element should be normalized
   ASSERT(m_Size.cx >= 0);
   ASSERT(m_Size.cy >= 0);

   ex1 = pE->GetOrigin().x;
   ey1 = pE->GetOrigin().y;
   ex2 = pE->GetOrigin().x + pE->GetSize().cx;
   ey2 = pE->GetOrigin().y + pE->GetSize().cy;

   tx1 = m_Origin.x;
   tx2 = m_Origin.x + m_Size.cx;
   ty1 = m_Origin.y;
   ty2 = m_Origin.y + m_Size.cy;

   // either one or both points inside?
   if ((tx1<ex1)&&(ex1<tx2) && (ty1<ey1)&&(ey1<ty2) || // t1 <e1< t2 (1st elem point)
       (tx1<ex2)&&(ex2<tx2) && (ty1<ey2)&&(ey2<ty2) )  // t1 <e2< t2 (2nd elem point)
   { 
      return true;
   } else {
      return false;
   }
}

void CElemContain::DelGroupElem(CElem *pE)
{
   for(int i=0; i<m_Group.GetSize(); i++)
   {
      if(m_Group[i]==pE)
      {
         m_Group.RemoveAt(i);
         break;
      }
   }
}

CElem* CElemContain::GetGroupElem(int i)
{
   return m_Group[i];
}

void CElemContain::AddGroupElem(CElem *pE)
{
   for(int i=0; i<m_Group.GetSize(); i++)
   {
      if(m_Group[i]==pE)
         return;
   }
   m_Group.Add(pE);
}

void CElemContain::EmptyGroup()
{
   m_Group.RemoveAll();
}

int CElemContain::GroupSize()
{
   return m_Group.GetSize();
}

void CElemContain::GetClientRect(CRect& R)
{
   CRect RBody;
   GetBodyRect(RBody,1);
   R.SetRect(GetOrigin(),GetOrigin()+GetSize());
   R.top = RBody.bottom;
   R.DeflateRect(1,1,1,1);
}
/*func*------------------------------------------------------------------------
  serialize the element to/from stream
  in :ar - the archive
  out:-
-----------------------------------------------------------------------------*/
void CElemContain::Serialize(CArchive & ar)
{
   CElem::Serialize(ar);

	if (ar.IsStoring())
	{
//      ar << m_MaxSize;
	} else {
//      ar >> m_MaxSize;
	}
}

bool CElemContain::GroupCheckElemLD()
{
   for(int i=0; i<m_Group.GetSize(); i++)
   {
       CElem* pE = m_Group[i];
       switch(pE->GetType())
       {
       case ET_COIL:
       case ET_CONTACT:
       case ET_POWERRAIL:
           return true;
       }
   }
   return false;
}

void CElemContain::SetFlagGroup(TElemFlags flag)
{
   for(int i=0; i<m_Group.GetSize(); i++)
      m_Group[i]->SetFlag(flag);
}

void CElemContain::ClrFlagGroup(TElemFlags flag)
{
   for(int i=0; i<m_Group.GetSize(); i++)
      m_Group[i]->ClrFlag(flag);
}

IMPLEMENT_SERIAL(CElemContain, CElem, SERIAL_VERSION)

//////////////////////////////////////////////////////////////////////
// CElemCon Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CElemCon::CElemCon()
{
   CElemConInit();
}

/*func*------------------------------------------------------------------------
  create a new connector
  created connector is selected !!
  in :Origin  - insertion point 
      Size    - startup size of element 
      strName - name-symbol of connector
      eCT     - connector typ
  out:pointer to new connector element
-----------------------------------------------------------------------------*/
CElemCon::CElemCon(CPoint Origin, CSize Size, LPCTSTR pszName)
{
   CElemConInit();

   SetOrigin(Origin);
   SetSize  (Size);
   SetName  (pszName);
}

CElemCon::~CElemCon()
{
}

void CElemCon::CElemConInit()
{
   CSize size(6,2);

   m_OriginOffs = CPoint(0,1);
   m_MaxSize.cy = 2;
   SetMinSize (size);
   SetName (_T(""));
}

/*func*------------------------------------------------------------------------
  draw the connector
  in :pDC   - pointer to device context
      pView - pointer to document (data)
      iGrid - actual display resolution
  out:-
-----------------------------------------------------------------------------*/
void CElemCon::Draw(CDC * pDC, CGrEditorView * pView, int iGrid)
{
   CRect    r;
   CSize    size;
   CString  str;
   PEN_TYPE PenType;
   bool     bScreen = !pDC->IsPrinting();

   if (!pView)
   {
      return;
   }
   // prepare drawing
   if     ( ChkFlag(EF_COMMENT  ) ) PenType = PE_COMMENT;
   else   /* normal */              PenType = PE_NORMAL;

   pDC->SelectObject(GDIHelper::GetPen  (bScreen, PenType  ));
   if (pView->IsManualMode())
   {
      pDC->SelectObject(GDIHelper::GetBrush(bScreen, BT_BACKGD_SFC));
   } else {
      pDC->SelectObject(GDIHelper::GetBrush(bScreen, BT_BACKGD));
   }

   // 
   ASSERT((GetType() == ET_CONIN) || (GetType() == ET_CONOUT));
   // draw the connect point
   DrawPins(pDC, iGrid);

   // draw element body
   GetBodyRect(r, iGrid);
   if (pView->IsManualMode())
   {
      pDC->FillRect(r, GDIHelper::GetBrush(bScreen, BT_ELEMBK_SFC));
   } else {
      pDC->FillRect(r, GDIHelper::GetBrush(bScreen, BT_ELEMBK));
   }

   // draw element icon 
   if( GetType() == ET_CONOUT )
   {
      DrawIcon( pDC, pView, 
         m_Origin + CPoint(PIN_LENGTH, 0), IDB_GR_ICONCONLP, iGrid);
      DrawIcon( pDC, pView, 
         m_Origin + CPoint(m_Size.cx-2,0), IDB_GR_ICONCONR, iGrid);
   } else {
      DrawIcon( pDC, pView, 
         m_Origin + CPoint(0,0), IDB_GR_ICONCONL, iGrid);
      DrawIcon( pDC, pView, 
         m_Origin + CPoint(m_Size.cx-PIN_LENGTH-2,0), IDB_GR_ICONCONRP, iGrid);
   }

   // draw element name
   GetTextRect(-1, r, iGrid);
   if( (pView->m_SearchDat.m_FindCur == m_Origin) && (pView->m_SearchDat.m_iFindElem == -1) )
   {
      m_Name.Draw( pDC, r, iGrid, GetColorType(), &pView->m_SearchDat,0);
   }else
   {
      m_Name.Draw( pDC, r, iGrid, GetColorType(), NULL ,0);   // no selection
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
BOOL CElemCon::FindText(SearchDat& SD)
{
// Con has 1 string for search (m_strName)
#define  MAX_ELEM   -1

   CString  str;
   LPCTSTR  psz;
   int      i;

   i = (int)_tcslen(SD.m_strFind);
   if( SD.m_bNext)
   {
      if( SD.m_iFindElem < MAX_ELEM ) 
      {
         SD.m_iFindElem = MAX_ELEM;
      }

      for ( ; SD.m_iFindElem < 0; SD.m_iFindElem++ ) 
      {
         if ( SD.m_iFindElem == -1 ) str = m_Name.Str();
         else break;
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
         if ( SD.m_iFindElem == -1 ) str = m_Name.Str();
         else break;
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
CText& CElemCon::GetTextAt(CPoint /*p*/, CPoint &start, int /*iGrid*/)
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
bool CElemCon::GetText(int iElemNr, int iPos, int iLen, CString& str)
{
   str = _T("");

   if (iElemNr < -1)
   {
      return (false);
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
void CElemCon::ReplaceSelText(int iElemNr, int iPos, int iLen, LPCTSTR pstr)
{
   if (iElemNr < -1)
   {
      return;
   }
   else if (iElemNr == -1)
   {
      (m_Name.StrRef()).Delete(iPos, iLen);
      (m_Name.StrRef()).Insert(iPos, pstr);
      return;
   }
   else 
   {
      CElem::ReplaceSelText( iElemNr, iPos, iLen, pstr);
      return;
   }
}

/*func*------------------------------------------------------------------------
  set name of connector
  in :pszName - the new name
  out:-
-----------------------------------------------------------------------------*/
void CElemCon::SetName(LPCTSTR pszName)  
{ 
   m_Name = pszName; 
   m_Name.m_Flag += TF_EDITABLE;
}

/*func*------------------------------------------------------------------------
  get name of connector
  in :-
  out:the name
-----------------------------------------------------------------------------*/
const CString& CElemCon::GetName() 
{ 
   return m_Name.Str(); 
}

/*func*------------------------------------------------------------------------
  serialize the element to/from stream
  in :ar - the archive
  out:-
-----------------------------------------------------------------------------*/
void CElemCon::Serialize(CArchive & ar)
{
   CString str;
   long lID;

   CElem::Serialize(ar);

	if (ar.IsStoring())
	{
      ar << m_Name;
	}
	else
	{
      ar >> m_Name;
      // update max id number
      str.LoadString(IDS_GR_NAMEMASK_CON);
      if( _stscanf( (LPCTSTR)m_Name, (LPCTSTR)str, &lID ) == 1) 
      {
         CGRTool::SetID(CGRTool::ID_CON, lID);
      }
	}
}

/*func*------------------------------------------------------------------------
  serialize the element to/from XML stream
  in :ar - the archive
  out:-
-----------------------------------------------------------------------------*/
void CElemCon::SerializeXML(CXMLArchive &ar)
{
   long    lID;
   CPoint  p;
   CString str;
   CPin   *pPin=NULL;

   ASSERT((GetType() == ET_CONIN) || (GetType()== ET_CONOUT));
   if(ar.IsStoring())
   {
      // all CXMLArchive fire HRESULT exceptions, which must be cought!!!
      try {            // name and create node here !!
         CElem::SerializeXML(ar);  // origin, size, selected, comment

         ar.SetStringAttributeUTF8( XML_4GR_ATTR_NAME, m_Name.Str());

         ASSERT( m_Con.GetSize() == 1 );
         if( m_Con.GetSize() == 1 ) 
         {
            pPin = m_Con.GetAt(0); 
            ASSERT( pPin );
            if( pPin )
            {
               if( GetType() == ET_CONIN ) 
               {
                  ar.SetAttribute( XML_4GR_ATTR_OUTORIGIN, pPin->m_Pos);
               } else {
                  ar.SetAttribute( XML_4GR_ATTR_INORIGIN,  pPin->m_Pos);
               }
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
         CElem::SerializeXML(ar);  // origin, size, selected, comment

         ar.GetAttribute( XML_4GR_ATTR_NAME, m_Name.StrRef());
         str.LoadString(IDS_GR_NAMEMASK_CON);
         if( _stscanf( (LPCTSTR)m_Name, (LPCTSTR)str, &lID ) == 1) 
         {
            CGRTool::SetID(CGRTool::ID_CON, lID);
         }

         if( GetType() == ET_CONIN ) 
         {
            if( ar.GetAttribute( XML_4GR_ATTR_OUTORIGIN, p) )
            {
               ASSERT( m_Con.GetSize() == 1 );
               CPin  *pPin = m_Con.GetAt(0);
               ASSERT( pPin );
               if( pPin ) pPin->m_Pos = p;
            } else {
               TRACE(_T("CElemConIn: missing outorigin\n"));
               throw E_FAIL; 
            }
         } else {
            if( ar.GetAttribute( XML_4GR_ATTR_INORIGIN, p) )
            {
               ASSERT( m_Con.GetSize() == 1 );
               CPin  *pPin = m_Con.GetAt(0);
               ASSERT( pPin );
               if( pPin ) pPin->m_Pos = p;
            } else {
               TRACE(_T("CElemConOut: missing inorigin\n"));
               throw E_FAIL; 
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

IMPLEMENT_SERIAL(CElemCon, CElem, SERIAL_VERSION)

//////////////////////////////////////////////////////////////////////
// CElemConIn Class
//////////////////////////////////////////////////////////////////////

void CElemConIn::CreatePins(void)
{
   CPin     *pPin;

   pPin = new CPin(_T("_ConIn_"), _T(""), _T("ANY"), 
      GetOrigin() + CPoint(GetSize().cx,1), CSize(-PIN_LENGTH,0), PF_OUT);
   if( pPin ) m_Con.Add(pPin);
}

/*func*------------------------------------------------------------------------
  get the text space in elem
  in :rect - placeholder for result
  out:rect - filled with text grid coords
-----------------------------------------------------------------------------*/
void CElemConIn::GetTextRect(int /*iTxtIdx*/, CRect &rect, int iGrid)
{
   rect.SetRect((m_Origin.x             + 1             )*iGrid,
                (m_Origin.y                             )*iGrid,
                (m_Origin.x + m_Size.cx - 1 - PIN_LENGTH)*iGrid, 
                (m_Origin.y + m_Size.cy                 )*iGrid);
}

/*func*------------------------------------------------------------------------
  resize element to optimal width for text length
  in :-
  out:dorigin - delta origin  (necessary for reset size and position if 
      dsize   - delta size     overlap-check fails)
      true  - element is resized
      false - element is unchanged
-----------------------------------------------------------------------------*/
bool CElemConIn::SetAutoSize(CPoint& dorigin, CSize& dsize) 
{ 
   int i = ((PIN_LENGTH + 2 + CText::GetIdentLen(m_Name.StrRef()) + 1) & 0xFFFFFFFE);
   if ( i < m_MinSize.cx ) i = m_MinSize.cx;
   if ( i > m_MaxSize.cx ) i = m_MaxSize.cx;
   dsize.cx  = i - m_Size.cx;
   dsize.cy  = 0;
   dorigin.x = -dsize.cx; 
   dorigin.y = 0;
   if (dsize.cx != 0)
   {
      Resize(dorigin.x, dorigin.y, dsize.cx, dsize.cy);
      return true;
   } else {
      return false;
   }
}

IMPLEMENT_SERIAL(CElemConIn, CElemCon, SERIAL_VERSION)

//////////////////////////////////////////////////////////////////////
// CElemConOut Class
//////////////////////////////////////////////////////////////////////

void CElemConOut::CreatePins(void)
{
   CPin     *pPin;

   pPin = new CPin(_T("_ConOut_"), _T(""), _T("ANY"), 
      GetOrigin() + CPoint(0,1), CSize(PIN_LENGTH,0), PF_IN);
   if( pPin ) m_Con.Add(pPin);
}

/*func*------------------------------------------------------------------------
  get the text space in elem
  in :rect - placeholder for result
  out:rect - filled with text grid coords
-----------------------------------------------------------------------------*/
void CElemConOut::GetTextRect(int /*iTxtIdx*/, CRect &rect, int iGrid)
{ 
   rect.SetRect((m_Origin.x             + 1 + PIN_LENGTH)*iGrid, 
                (m_Origin.y                             )*iGrid,
                (m_Origin.x + m_Size.cx - 1             )*iGrid,
                (m_Origin.y + m_Size.cy                 )*iGrid); 
}

/*func*------------------------------------------------------------------------
  resize element to optimal width for text length
  in :-
  out:dorigin - delta origin  (necessary for reset size and position if 
      dsize   - delta size     overlap-check fails)
      true  - element is resized
      false - element is unchanged
-----------------------------------------------------------------------------*/
bool CElemConOut::SetAutoSize(CPoint& dorigin, CSize& dsize) 
{ 
   int i = ((PIN_LENGTH + 2 + CText::GetIdentLen(m_Name.StrRef()) + 1) & 0xFFFFFFFE);
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

IMPLEMENT_SERIAL(CElemConOut, CElemCon, SERIAL_VERSION)

//////////////////////////////////////////////////////////////////////
// CElemLab Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CElemLab::CElemLab()
{
   CElemLabInit();
}

/*func*------------------------------------------------------------------------
  create a new label
  created label is selected !!
  in :Origin  - insertion point 
      Size    - startup size of element 
      strName - name-symbol of label
      eLT     - label typ
  out:pointer to new label element
-----------------------------------------------------------------------------*/
CElemLab::CElemLab(CPoint Origin, CSize Size, LPCTSTR pszName, BOOL bLine)
{
   CElemLabInit();

   SetOrigin(Origin);
   SetSize  (Size);
   SetLine  (bLine);
   SetName  (pszName);
}

CElemLab::~CElemLab()
{
}

void CElemLab::CElemLabInit(void)
{
   m_OriginOffs = CPoint(0,1);
   m_MinSize    = CSize(6,2);
   m_MaxSize.cy = 2;
   m_bLine      = FALSE;
   SetName (_T(""));
}

/*func*------------------------------------------------------------------------
  draw the label
  in :pDC   - pointer to device context
      pView - pointer to document (data)
      iGrid - actual display resolution
  out:-
-----------------------------------------------------------------------------*/
void CElemLab::Draw(CDC * pDC, CGrEditorView * pView, int iGrid)
{
   CRect    r;
   CSize    size;
   CString  str;
   PEN_TYPE PenType;
   bool     bScreen = !pDC->IsPrinting();

   if (!pView)
   {
      return;
   }
   ASSERT((GetType() == ET_JUMP) || (GetType() == ET_LABEL));

   // prepare drawing
   if     ( ChkFlag(EF_COMMENT) ) PenType = PE_COMMENT;
   else   /* normal */            PenType = PE_NORMAL;

   pDC->SelectObject(GDIHelper::GetPen  (bScreen, PenType  ));
   if (pView->IsManualMode())
   {
      pDC->SelectObject(GDIHelper::GetBrush(bScreen, BT_BACKGD_SFC));
   } else {
      pDC->SelectObject(GDIHelper::GetBrush(bScreen, BT_BACKGD));
   }

   // draw the label connect point
   DrawPins(pDC, iGrid);

   // draw element body
   GetBodyRect(r, iGrid);
   if (pView->IsManualMode())
   {
      pDC->FillRect(r, GDIHelper::GetBrush(bScreen, BT_ELEMBK_SFC));
   } else {
      pDC->FillRect(r, GDIHelper::GetBrush(bScreen, BT_ELEMBK));
   }

   // draw element icon 
   DrawIcon( pDC, pView, 
      ((GetType() == ET_JUMP) ? (m_Origin + CPoint(PIN_LENGTH,0)) : m_Origin), 
      ((GetType() == ET_JUMP) ? IDB_GR_ICONLABR : IDB_GR_ICONLAB), iGrid);
   // draw element name
   r.left += 2*iGrid;

   if((pView->m_SearchDat.m_FindCur == m_Origin) && (pView->m_SearchDat.m_iFindElem == -1))
   {
      m_Name.Draw( pDC, r, iGrid, GetColorType(), &pView->m_SearchDat, 0);
   }else
   {
      m_Name.Draw( pDC, r, iGrid, GetColorType(), NULL, 0);
   }
   
   // draw vertical line at label
   if( m_bLine && (GetType() == ET_LABEL) ) 
   {
      CPen *OldPen;
      int   x1, x2;

      if (m_pContain)
      { 
         ASSERT_VALID(m_pContain);
         x1 = m_pContain->GetOrigin().x;
         x2 = m_pContain->GetOrigin().x + m_pContain->GetSize().cx;
      } else {
         x1 = 0;
         x2 = MAX_XGRID - 1;
      }

      OldPen = pDC->SelectObject(GDIHelper::GetPen(bScreen, PE_LABEL));
      pDC->MoveTo((x1                    )*iGrid, (m_Origin.y+1)*iGrid); 
      pDC->LineTo((m_Origin.x            )*iGrid, (m_Origin.y+1)*iGrid);
      pDC->MoveTo((m_Origin.x + m_Size.cx)*iGrid, (m_Origin.y+1)*iGrid); 
      pDC->LineTo((x2                    )*iGrid, (m_Origin.y+1)*iGrid);
      pDC->SelectObject(OldPen);
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
  get the data identifier of the element
  in :p     - grip position for test
      start - position for identify (origin)
  out:identifier of the element
-----------------------------------------------------------------------------*/
CText& CElemLab::GetTextAt(CPoint /*p*/, CPoint &start, int /*iGrid*/)
{
   start = m_Origin;
   return m_Name;
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
BOOL CElemLab::FindText(SearchDat& SD)
{
// Lab has 1 string for search (m_strName)
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
            str = m_Name.Str();
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
                     SD.m_iFindLen= (int)_tcslen(SD.m_strFind);
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
            str = m_Name.Str();
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
  get the body rect without pin range for select operations
  in :r        - place holder for rect
      iGrid    - actual zoom
  out:-
-----------------------------------------------------------------------------*/
void CElemLab::GetBodyRect(CRect &r, int iGrid)
{
   ASSERT(iGrid > 0);

   if( iGrid > 0 )
   {
      GetRect(r, iGrid);
      if( GetType() == ET_JUMP )
      {
         r.left += iGrid*PIN_LENGTH;
      }
   }
}

/*func*------------------------------------------------------------------------
  get the resize rect for wished position wiht select frame
  in :eRectTyp - wished position of rect
      r        - place holder for rect
      iGrid    - actual zoom
  out:pointer to rect for draw function with logical coordinates
-----------------------------------------------------------------------------*/
void CElemLab::GetBoundingRect(CRect& r, int iGrid)
{
   GetRect(r, iGrid);
   r.InflateRect(SP_RAD+1,SP_RAD+1);  // increase for selection
   if( m_bLine )
   {
      r.left  = 0; 
      r.right = (MAX_XGRID-1) * iGrid;
   }
   r.NormalizeRect();
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
bool CElemLab::GetText(int iElemNr, int iPos, int iLen, CString& str)
{
   str = _T("");

   if (iElemNr < -1)
   {
      return (false);
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
void CElemLab::ReplaceSelText(int iElemNr, int iPos, int iLen, LPCTSTR pstr)
{
   if (iElemNr < -1)
   {
      return;
   }
   else if (iElemNr == -1)
   {
      (m_Name.StrRef()).Delete(iPos, iLen);
      (m_Name.StrRef()).Insert(iPos, pstr);
      return;
   }
   else 
   {
      CElem::ReplaceSelText( iElemNr, iPos, iLen, pstr);
      return;
   }
}

/*func*------------------------------------------------------------------------
  set name of label
  in :pszName - the new name
  out:-
-----------------------------------------------------------------------------*/
void CElemLab::SetName(LPCTSTR pszName)  
{ 
   m_Name = pszName; 
   m_Name.m_Flag += TF_EDITABLE;
}

/*func*------------------------------------------------------------------------
  get name of label
  in :-
  out:the name
-----------------------------------------------------------------------------*/
const CString& CElemLab::GetName() 
{ 
   return m_Name.Str(); 
}

/*func*------------------------------------------------------------------------
  serialize the element to/from stream
  in :ar - the archive
  out:-
-----------------------------------------------------------------------------*/
void CElemLab::Serialize(CArchive & ar)
{
   CString str;
   long    lID;

   CElem::Serialize(ar);

	if (ar.IsStoring())
	{
      ar << m_Name << m_bLine;
	}
	else
	{
      ar >> m_Name >> m_bLine;
      // update max id number
      str.LoadString(IDS_GR_NAMEMASK_LAB);
      if( _stscanf( (LPCTSTR)m_Name, (LPCTSTR)str, &lID ) == 1) 
      {
         CGRTool::SetID(CGRTool::ID_LAB, lID);
      }
	}
}

/*func*------------------------------------------------------------------------
  serialize the element to/from XML stream
  in :ar - the archive
  out:-
-----------------------------------------------------------------------------*/
void CElemLab::SerializeXML(CXMLArchive & ar)
{
   long    lID;
   CPoint  p;
   CString str;
   CPin   *pPin=NULL;
   long    l;

   ASSERT((GetType() == ET_JUMP) || (GetType() == ET_LABEL));
   if(ar.IsStoring())
   {
      // all CXMLArchive fire HRESULT exceptions, which must be cought!!!
      try {            // name and create node here !!
         CElem::SerializeXML(ar);  // origin, size, selected, comment

         ar.SetStringAttributeUTF8( XML_4GR_ATTR_NAME, m_Name.Str());

         if( GetType() == ET_JUMP ) 
         {
            ASSERT( m_Con.GetSize() == 1 );
            if( m_Con.GetSize() == 1 ) 
            {
               pPin = m_Con.GetAt(0); 
               ASSERT( pPin );
               if( pPin ) ar.SetAttribute( XML_4GR_ATTR_INORIGIN, pPin->m_Pos);
            } 
         }
         if( GetType() == ET_LABEL ) 
         {
            if( m_bLine ) ar.SetAttribute( XML_4GR_ATTR_BORDER, 1L);
         }
      }        
      catch (HRESULT hr)
      {
         TRACE(_T("Cannot save to XML; hr=0x%08X\n"), hr);
         throw hr;   // fire upstairs...        
      }    
   } else {
      try {   
         CElem::SerializeXML(ar);  // origin, size, selected, comment

         ar.GetAttribute( XML_4GR_ATTR_NAME, m_Name.StrRef());
         str.LoadString(IDS_GR_NAMEMASK_LAB);
         if( _stscanf( (LPCTSTR)m_Name, (LPCTSTR)str, &lID ) == 1) 
         {
            CGRTool::SetID(CGRTool::ID_LAB, lID);
         }

         if( GetType() == ET_JUMP ) 
         {
            if( ar.GetAttribute( XML_4GR_ATTR_INORIGIN, p) )
            {
               ASSERT( m_Con.GetSize() != 0 );
               CPin  *pPin = m_Con.GetAt(0);
               ASSERT( pPin );
               if( pPin ) pPin->m_Pos = p;
            } else {
               TRACE(_T("CElemLabel: missing inorigin\n"));
               throw E_FAIL; 
            }
         }
         if( GetType() == ET_LABEL ) 
         {
            if( ar.GetAttribute( XML_4GR_ATTR_BORDER, l) && l==1) m_bLine = TRUE;
         }
      }
      catch (HRESULT hr)
      {
         TRACE(_T("Cannot load from XML; hr=0x%08X\n"), hr);
         throw hr;   // fire upstairs...
      }   
   }
}


IMPLEMENT_SERIAL(CElemLab, CElem, SERIAL_VERSION)

//////////////////////////////////////////////////////////////////////
// CElemLabel Class
//////////////////////////////////////////////////////////////////////

/*func*------------------------------------------------------------------------
  resize element to optimal width for text length
  in :-
  out:dorigin - delta origin  (necessary for reset size and position if 
      dsize   - delta size     overlap-check fails)
      true  - element is resized
      false - element is unchanged
-----------------------------------------------------------------------------*/
bool CElemLabel::SetAutoSize(CPoint& dorigin, CSize& dsize) 
{ 
   int i = ((PIN_LENGTH + 2 + CText::GetIdentLen(m_Name.StrRef()) + 1) & 0xFFFFFFFE);
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
  get the text space in elem
  in :rect - placeholder for result
  out:rect - filled with text grid coords
-----------------------------------------------------------------------------*/
void CElemLabel::GetTextRect(int /*iTxtIdx*/, CRect &rect, int iGrid)
{ 
   rect.SetRect((m_Origin.x + 2        )*iGrid, 
                (m_Origin.y            )*iGrid,
                (m_Origin.x + m_Size.cx)*iGrid, 
                (m_Origin.y + m_Size.cy)*iGrid); 
}

/*func*------------------------------------------------------------------------
  test overlap of element pE with this label
  in :pE - test element
  out:true  - ok, no overlap
      false - overlapping!!
-----------------------------------------------------------------------------*/
bool CElemLabel::JoinTest(TDirection, CElem* pE)
{
/*
   CPoint Orig = pE->GetOrigin();
   CPoint Size = pE->GetSize();

   return !( (max(Orig.y, Orig.y + Size.y) > m_Origin.y            ) && 
             (min(Orig.y, Orig.y + Size.y) < m_Origin.y + m_Size.cy) );
*/
   bool bRes;
   int  y1, y2;

   if (pE->GetSize().cy > 0)
   {
      y1 = pE->GetOrigin().y;
      y2 = y1 + pE->GetSize().cy;
   } else {
      y2 = pE->GetOrigin().y;
      y1 = y2 + pE->GetSize().cy;
   }

   bRes = !((y2 > m_Origin.y) && (y1 < m_Origin.y + m_Size.cy));

   // allow power rails to cross the horizontal line of labels
   if(!bRes && pE->GetType() == ET_POWERRAIL)
   {
      bRes |= (pE->GetOrigin().x < GetOrigin().x) || (pE->GetOrigin().x > GetOrigin().x + GetSize().cx );
   }

   if (m_pContain != NULL)  // we are in the box ?
   {
      // area of label is reduced to box width !
      // container is normalized!
      int cx1, cx2, ex1, ex2;

      cx1 = m_pContain->GetOrigin().x;
      cx2 = m_pContain->GetOrigin().x + m_pContain->GetSize().cx;
      if (pE->GetSize().cx > 0)
      {
         ex1 = pE->GetOrigin().x;
         ex2 = ex1 + pE->GetSize().cx;
      } else {
         ex2 = pE->GetOrigin().x;
         ex1 = ex2 + pE->GetSize().cx;
      }

      if (!((cx1 < ex1) && (ex2 < cx2)))
      {
         // element pE is not complete in container ..
         // -> we don't check with this label
         bRes = true;
      }
   }

   return bRes;
}

/*func*------------------------------------------------------------------------
  shifts the element
  in :CPoint At - spacing origin
  in :CPoint Delta - spacing distance
  in :CPosArray* pPin - list of pins needing line insertion (disrupted lines)
  out:spacing result - true: moved | false: not moved
-----------------------------------------------------------------------------*/
bool CElemLabel::Spacing ( CPoint At, CPoint Delta, CPosArray*  pPin, bool bLabelClip)
{
   if(Delta.x)       // don't move in x direction
      return true;
   else
      return CElem::Spacing(At,Delta,pPin, bLabelClip);
}

IMPLEMENT_SERIAL(CElemLabel, CElemLab, SERIAL_VERSION)

//////////////////////////////////////////////////////////////////////
// CElemJump Class
//////////////////////////////////////////////////////////////////////

/*func*------------------------------------------------------------------------
  get the text space in elem
  in :rect - placeholder for result
  out:rect - filled with text grid coords
-----------------------------------------------------------------------------*/
void CElemJump::GetTextRect(int /*iTxtIdx*/, CRect &rect, int iGrid)
{ 
   rect.SetRect((m_Origin.x + PIN_LENGTH + 2)*iGrid,
                (m_Origin.y                 )*iGrid,
                (m_Origin.x + m_Size.cx     )*iGrid,
                (m_Origin.y + m_Size.cy     )*iGrid); 
}

void CElemJump::CreatePins(void)
{
   CPin     *pPin;

   pPin = new CPin(_T("_Jump_"), _T(""), _T("ANY"), 
           GetOrigin() + CPoint(0,1), CSize(PIN_LENGTH,0), PF_IN);
   if( pPin ) m_Con.Add(pPin);
}

/*func*------------------------------------------------------------------------
  resize element to optimal width for text length
  in :-
  out:dorigin - delta origin  (necessary for reset size and position if 
      dsize   - delta size     overlap-check fails)
      true  - element is resized
      false - element is unchanged
-----------------------------------------------------------------------------*/
bool CElemJump::SetAutoSize(CPoint& dorigin, CSize& dsize) 
{ 
   int i = ((PIN_LENGTH + 2 + CText::GetIdentLen(m_Name.StrRef()) + 1) & 0xFFFFFFFE);
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

IMPLEMENT_SERIAL(CElemJump, CElemLab, SERIAL_VERSION)

//////////////////////////////////////////////////////////////////////
// CElemRet Class
//////////////////////////////////////////////////////////////////////

CString CElemRet::m_strDefName = _T("RETURN");

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CElemRet::CElemRet()
{
   CElemRetInit();
}

/*func*------------------------------------------------------------------------
  create a new return and insert them in the element list
  inserted element is selected !!
  in :Origin  - insertion point 
      Size    - startup size of element 
      strName - name-symbol for return 
  out:pointer of new return element
-----------------------------------------------------------------------------*/
CElemRet::CElemRet(CPoint Origin, CSize Size)
{
   CElemRetInit(Origin, Size);
}

CElemRet::~CElemRet()
{
}

void CElemRet::CElemRetInit(CPoint Origin, CSize Size)
{
   CPin *pPin;
   CSize size(6,2);

   SetType(ET_RETURN);
   m_Name     = m_strDefName;

   m_OriginOffs = CPoint(0,1); 
   SetMinSize (size);
   m_MaxSize.cy = 2;

   SetOrigin(Origin);
   SetSize  (Size  );

   pPin = new CPin(m_strDefName, _T(""), _T("ANY"), 
           Origin + CPoint(0,1), CSize(PIN_LENGTH,0), PF_IN);
   if( pPin ) m_Con.Add(pPin);
}

/*func*------------------------------------------------------------------------
  draw the return
  in :pDC   - pointer to device context
      pView - pointer to document (data)
      iGrid - actual display resolution
  out:-
-----------------------------------------------------------------------------*/
void CElemRet::Draw(CDC * pDC, CGrEditorView * pView, int iGrid)
{
   CRect    r;
   CSize    size;
   CString  str;
   PEN_TYPE PenType;
   bool     bScreen = !pDC->IsPrinting();

   if (!pView)
   {
      return;
   }
   // prepare drawing
   if     ( ChkFlag(EF_COMMENT ) ) PenType = PE_COMMENT;
   else   /* normal */                PenType = PE_NORMAL;

   pDC->SelectObject(GDIHelper::GetPen  (bScreen, PenType  ));
   if (pView->IsManualMode())
   {
      pDC->SelectObject(GDIHelper::GetBrush(bScreen, BT_BACKGD_SFC));
   } else {
      pDC->SelectObject(GDIHelper::GetBrush(bScreen, BT_BACKGD));
   }

   // draw the return connect point
   DrawPins(pDC, iGrid);
   // draw the return element
   GetBodyRect(r, iGrid);
   if (pView->IsManualMode())
   {
      pDC->FillRect(r, GDIHelper::GetBrush(bScreen, BT_ELEMBK_SFC));
   } else {
      pDC->FillRect(r, GDIHelper::GetBrush(bScreen, BT_ELEMBK));
   }

   // draw element icon at FIRST underlaying the text
   DrawIcon( pDC, pView, m_Origin + CPoint(PIN_LENGTH,0), 
      IDB_GR_ICONRETL, iGrid);
   DrawIcon( pDC, pView, m_Origin + CPoint(m_Size.cx-2,0), 
      IDB_GR_ICONRETR, iGrid);

   // draw return text
   r.left  += iGrid;
   r.right -= iGrid;
   if((pView->m_SearchDat.m_FindCur == m_Origin) && (pView->m_SearchDat.m_iFindElem == -1))
   {
      m_Name.Draw( pDC, r, iGrid, GetColorType(), &pView->m_SearchDat, 0);
   }else
   {
      m_Name.Draw( pDC, r, iGrid, GetColorType(), NULL, 0);
   }
   r.left  -= iGrid;
   r.right += iGrid;

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
  get the data identifier of the element
  in :p     - grip position for test
      start - position for identify (origin)
  out:identifier of the element
-----------------------------------------------------------------------------*/
CText& CElemRet::GetTextAt(CPoint /*p*/, CPoint &start, int /*iGrid*/)
{
   start = m_Origin;
   return m_Name;
}

/*func*------------------------------------------------------------------------
  get the body rect without pin range for select operations
  in :r        - place holder for rect
      iGrid    - actual zoom
  out:-
-----------------------------------------------------------------------------*/
void CElemRet::GetBodyRect(CRect &r, int iGrid)
{
   ASSERT(iGrid > 0);

   if( iGrid > 0 )
   {
      GetRect(r, iGrid);
      r.left += iGrid*PIN_LENGTH; 
   }
}

/*func*------------------------------------------------------------------------
  get the text space in elem
  in :rect - placeholder for result
  out:rect - filled with text grid coords
-----------------------------------------------------------------------------*/
void CElemRet::GetTextRect(int /*iTxtIdx*/, CRect &rect, int iGrid)
{ 
   rect.SetRect((m_Origin.x + PIN_LENGTH + 1)*iGrid,
                (m_Origin.y                 )*iGrid,
                (m_Origin.x + m_Size.cx  - 1)*iGrid,
                (m_Origin.y + m_Size.cy     )*iGrid);
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
BOOL CElemRet::FindText(SearchDat& SD)
{
// Ret has 1 string for search (m_strName)
#define  MAX_ELEM   -1

   CString  str;
   LPCTSTR  psz;
   int      i;

   i = (int)_tcslen(SD.m_strFind);
   if( SD.m_bNext)
   {
      if( SD.m_iFindElem < MAX_ELEM ) 
      {
         SD.m_iFindElem = MAX_ELEM;
      }
      for ( ; SD.m_iFindElem < 0; SD.m_iFindElem++ ) 
      {
         if ( SD.m_iFindElem == -1 ) 
         {
            str = m_strDefName;
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
            str = m_strDefName;
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
               if( SD.m_bCase?
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
  get text in element 
  in :iElemNr  - neg. numbers are reserved for inherit classes,
                 pos. numbers are the pins in m_con
      iPos     - position in strname in pin
      iLen     - length of wished string
  out:str   - search text
      true  - string found 
      false - nothing found
-----------------------------------------------------------------------------*/
bool CElemRet::GetText(int iElemNr, int iPos, int iLen, CString& str)
{
   str = _T("");

   if (iElemNr < -1)
   {
      return (false);
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
void CElemRet::ReplaceSelText(int iElemNr, int iPos, int iLen, LPCTSTR pstr)
{
   if (iElemNr < -1)
   {
      return;
   }
   else if (iElemNr == -1)
   {
      (m_Name.StrRef()).Delete(iPos, iLen);
      (m_Name.StrRef()).Insert(iPos, pstr);
      return;
   }
   else 
   {
      CElem::ReplaceSelText( iElemNr, iPos, iLen, pstr);
      return;
   }
}

/*func*------------------------------------------------------------------------
  serialize the element to/from stream
  in :ar - the archive
  out:-
-----------------------------------------------------------------------------*/
void CElemRet::Serialize(CArchive & ar)
{
   CElem::Serialize(ar);
}

/*func*------------------------------------------------------------------------
  serialize the element to/from XML stream
  in :ar - the archive
  out:-
-----------------------------------------------------------------------------*/
void CElemRet::SerializeXML(CXMLArchive &ar)
{
   CPoint p;

   if(ar.IsStoring())
   {
      // all CXMLArchive fire HRESULT exceptions, which must be cought!!!
      try {            // name and create node here !!
         CElem::SerializeXML(ar);  // origin, size, selected, comment

         ASSERT( m_Con.GetSize() > 0 );
         if( m_Con.GetSize() > 0 ) 
         {
            CPin *pPin = m_Con.GetAt(0); 
            ASSERT( pPin );
            if( pPin ) ar.SetAttribute( XML_4GR_ATTR_INORIGIN, pPin->m_Pos);
         } 
      }        
      catch (HRESULT hr)
      {
         TRACE(_T("Cannot save to XML; hr=0x%08X\n"), hr);
         throw hr;   // fire upstairs...        
      }    
   } else {
      try {   
         CElem::SerializeXML(ar);  // origin, size, selected, comment

         if( ar.GetAttribute( XML_4GR_ATTR_INORIGIN, p) )
         {
            ASSERT( m_Con.GetSize() > 0 );
            if( m_Con.GetSize() > 0 ) 
            {
               CPin  *pPin = m_Con.GetAt(0);
               ASSERT( pPin );
               if( pPin ) pPin->m_Pos = p;
            }
         } else {
            TRACE(_T("CElemRet: missing inorigin\n"));
            throw E_FAIL; 
         }
      }
      catch (HRESULT hr)
      {
         TRACE(_T("Cannot load from XML; hr=0x%08X\n"), hr);
         throw hr;   // fire upstairs...
      }   
   }
}

IMPLEMENT_SERIAL(CElemRet, CElem, SERIAL_VERSION)



//////////////////////////////////////////////////////////////////////
// CElemVar Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CElemVar::CElemVar()
{
   CSize size(4,2);

   m_OriginOffs = CPoint(0,1);
   SetMinSize(size);
   m_MaxSize.cy = 2;
   SetName(_T(""));
}

/*func*------------------------------------------------------------------------
  create a new variable
  created variable is selected !!
  in :Origin  - insertion point 
      Size    - startup size of element 
      strName - name-symbol of variable
      eVT     - variable typ
  out:pointer to new variable element
-----------------------------------------------------------------------------*/
CElemVar::CElemVar(CPoint Origin, CSize Size, LPCTSTR pszName )
{
   CSize size(4,2);

   m_OriginOffs = CPoint(0,1);
   SetMinSize(size);
   m_MaxSize.cy = 2;

   SetOrigin(Origin);
   SetSize  (Size);
   SetName(pszName);
}

CElemVar::~CElemVar()
{
}

/*func*------------------------------------------------------------------------
  draw the variable
  in :pDC   - pointer to device context
      pView - pointer to document (data)
      iGrid - actual display resolution
  out:-
-----------------------------------------------------------------------------*/
void CElemVar::Draw(CDC * pDC, CGrEditorView * pView, int iGrid)
{
   CRect    r;
   CSize    size;
   CPoint   p;
   CString  str;
   PEN_TYPE PenType;
   bool     bScreen = !pDC->IsPrinting();

   if (!pView)
   {
      return;
   }
   ASSERT((GetType() == ET_VARIN) || 
          (GetType() == ET_VAROUT) || 
          (GetType() == ET_VARINOUT));
   
   // prepare drawing
   if     ( ChkFlag(EF_COMMENT)||ChkFlag(EF_AUTOELEM) ) PenType = PE_COMMENT;
   else   /* normal */            PenType = PE_NORMAL;

   pDC->SelectObject(GDIHelper::GetPen  (bScreen, PenType  ));
   if (pView->IsManualMode())
   {
      pDC->SelectObject(GDIHelper::GetBrush(bScreen, BT_BACKGD_SFC));
   } else {
      pDC->SelectObject(GDIHelper::GetBrush(bScreen, BT_BACKGD));
   }
   // draw the connect point
   DrawPins(pDC, iGrid);

   // draw element body
   GetBodyRect(r, iGrid);
   r.right++;  // one pixel more ..
   if (pView->IsManualMode())
   {
      pDC->FillRect(r, GDIHelper::GetBrush(bScreen, BT_ELEMBK_SFC));
   } else {
      pDC->FillRect(r, GDIHelper::GetBrush(bScreen, BT_ELEMBK));
   }
/*
   // draw element icon 
   if( GetType() == ET_VAROUT ) 
   {
      p = m_Origin + CPoint(m_Size.cx-2,0); // icon pos
      r.right -= 2*iGrid;                   // adjust text field
   } else if( GetType() == ET_VARINOUT ) {
      p = m_Origin + CPoint(m_Size.cx-4,0); // icon pos
      r.right -= 2*iGrid;                   // adjust text field
   } else {
      p = m_Origin;                         // icon pos
      r.left  += 2*iGrid;                   // adjust text field
   }
   if      (GetType() == ET_VARIN)
   {
      DrawIcon( pDC, pView, p, IDB_GR_ICONVARRD, iGrid);
   } 
   else if (GetType() == ET_VAROUT)
   {
      DrawIcon( pDC, pView, p, IDB_GR_ICONVARWR, iGrid);
   }
   else if (GetType() == ET_VARINOUT)
   {
      DrawIcon( pDC, pView, p, IDB_GR_ICONVARFB, iGrid);
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
  get the body rect without pin range for select operations
  in :r        - place holder for rect
      iGrid    - actual zoom
  out:-
-----------------------------------------------------------------------------*/
void CElemVar::GetBodyRect(CRect &r, int iGrid)
{
   ASSERT(iGrid > 0);

   if( iGrid > 0 )
   {
      GetRect(r, iGrid);
      if( (GetType() == ET_VARINOUT) || 
          (GetType() == ET_VAROUT) ) 
      {
         r.left  += iGrid*PIN_LENGTH; 
      }
      if( (GetType() == ET_VARINOUT) || 
          (GetType() == ET_VARIN) ) 
      {
         r.right -= iGrid*PIN_LENGTH;
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
BOOL CElemVar::FindText(SearchDat& SD)
{
// Var has 1 string for search (m_strName)
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
            str = m_Name.Str();
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
            str = m_Name.Str();
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
CText& CElemVar::GetTextAt(CPoint /*p*/, CPoint &start, int /*iGrid*/)
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
bool CElemVar::GetText(int iElemNr, int iPos, int iLen, CString& str)
{
   str = _T("");

   if (iElemNr < -1)
   {
      return (false);
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
void CElemVar::ReplaceSelText(int iElemNr, int iPos, int iLen, LPCTSTR pstr)
{
   if (iElemNr < -1)
   {
      return;
   }
   else if (iElemNr == -1)
   {
      (m_Name.StrRef()).Delete(iPos, iLen);
      (m_Name.StrRef()).Insert(iPos, pstr);
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
void CElemVar::SetName(LPCTSTR pszName)  
{ 
   m_Name = pszName; 
   m_Name.m_Flag += TF_EDITABLE;
}

/*func*------------------------------------------------------------------------
  get name of var
  in :-
  out:the name
-----------------------------------------------------------------------------*/
const CString& CElemVar::GetName() 
{ 
   return m_Name.Str(); 
}

/*func*------------------------------------------------------------------------
  serialize the element to/from stream
  in :ar - the archive
  out:-
-----------------------------------------------------------------------------*/
void CElemVar::Serialize(CArchive & ar)
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
      str.LoadString(IDS_GR_NAMEMASK_VAR);
      if( _stscanf( (LPCTSTR)m_Name, (LPCTSTR)str, &lID ) == 1) 
      {
         CGRTool::SetID(CGRTool::ID_VAR, lID);
      }
	}
}

/*func*------------------------------------------------------------------------
  serialize the element to/from XML stream
  in :ar - the archive
  out:-
-----------------------------------------------------------------------------*/
void CElemVar::SerializeXML(CXMLArchive &ar)
{
   long    lID;
   CString str;
   CPoint  p;
   CPin   *pPin=NULL;

   ASSERT((GetType() == ET_VARIN) || 
          (GetType() == ET_VAROUT) || 
          (GetType() == ET_VARINOUT) ||
          (GetType() == ET_CONST));
   if(ar.IsStoring())
   {
      // all CXMLArchive fire HRESULT exceptions, which must be cought!!!
      try {            // name and create node here !!
         CElem::SerializeXML(ar);  // origin, size, selected, comment

         ar.SetStringAttributeUTF8( XML_4GR_ATTR_NAME, m_Name.Str());

         ASSERT( m_Con.GetSize() > 0 );
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
         CElem::SerializeXML(ar);  // origin, size, selected, comment

         ar.GetAttribute( XML_4GR_ATTR_NAME, m_Name.StrRef());
         str.LoadString(IDS_GR_NAMEMASK_VAR);
         if( _stscanf( (LPCTSTR)m_Name, (LPCTSTR)str, &lID ) == 1) 
         {
            CGRTool::SetID(CGRTool::ID_VAR, lID);
         }

         if ((GetType() == ET_VARIN))
         {
            if( ar.GetAttribute( XML_4GR_ATTR_OUTORIGIN, p) )
            {
               ASSERT( m_Con.GetSize() == 1 );
               CPin  *pPin = m_Con.GetAt(0);
               ASSERT( pPin );
               if( pPin ) pPin->m_Pos = p;
            } else {
               TRACE(_T("CElemVarIn: missing outorigin\n"));
               throw E_FAIL; 
            }
         } else if( GetType() == ET_CONST ) {
            ASSERT( m_Con.GetSize() == 1 );
            CPin  *pPin = m_Con.GetAt(0);
            ASSERT( pPin );
            // work around -> error in DTD !
            if( ar.GetAttribute( XML_4GR_ATTR_OUTORIGIN, p) )
            {
               if( pPin ) pPin->m_Pos = p;
            }
         } else if( GetType() == ET_VAROUT ) {
            if( ar.GetAttribute( XML_4GR_ATTR_INORIGIN, p) )
            {
               ASSERT( m_Con.GetSize() == 1 );
               CPin  *pPin = m_Con.GetAt(0);
               ASSERT( pPin );
               if( pPin ) pPin->m_Pos = p;
            } else {
               TRACE(_T("CElemVarOut: missing inorigin\n"));
               throw E_FAIL; 
            }
         } else if( GetType() == ET_VARINOUT ) {
            ASSERT( m_Con.GetSize() == 2 );
            for( int i=0; i<2; i++ )
            {
               CPin  *pPin = m_Con.GetAt(i);
               ASSERT( pPin );
               if( pPin ) 
               {
                  if( pPin->m_Size.cx < 0 )
                  {
                     if( !ar.GetAttribute( XML_4GR_ATTR_OUTORIGIN, p) )
                     {
                        TRACE(_T("CElemVarInOut: missing outorigin\n")); throw E_FAIL; 
                     }
                  } else {
                     if( !ar.GetAttribute( XML_4GR_ATTR_INORIGIN, p) )
                     {
                        TRACE(_T("CElemVarInOut: missing inorigin\n")); throw E_FAIL; 
                     }
                  }
                  pPin->m_Pos = p;
               }
            }
         } else ASSERT(FALSE);
      }
      catch (HRESULT hr)
      {
         TRACE(_T("Cannot load from XML; hr=0x%08X\n"), hr);
         throw hr;   // fire upstairs...
      }   
   }
}

IMPLEMENT_SERIAL(CElemVar, CElem, SERIAL_VERSION)

//////////////////////////////////////////////////////////////////////
// CElemConst Class
//////////////////////////////////////////////////////////////////////

/*func*------------------------------------------------------------------------
  get the text space in elem
  in :rect - placeholder for result
  out:rect - filled with text grid coords
-----------------------------------------------------------------------------*/
void CElemConst::GetTextRect(int /*iTxtIdx*/, CRect &rect, int iGrid)
{ 
   rect.SetRect((m_Origin.x                         )*iGrid,
                (m_Origin.y                         )*iGrid,
                (m_Origin.x + m_Size.cx - PIN_LENGTH)*iGrid,
                (m_Origin.y + m_Size.cy             )*iGrid);
}

void CElemConst::CreatePins(void)
{
   CPin     *pPin;

   pPin = new CPin(_T("_Const_"), _T(""), _T("ANY"), 
      GetOrigin() + CPoint(m_Size.cx,1), CSize(-PIN_LENGTH,0), PF_OUT);
   if( pPin ) m_Con.Add(pPin);
}

IMPLEMENT_SERIAL(CElemConst, CElemVar, SERIAL_VERSION)

//////////////////////////////////////////////////////////////////////
// CElemVarIn Class
//////////////////////////////////////////////////////////////////////

/*func*------------------------------------------------------------------------
  get the text space in elem
  in :rect - placeholder for result
  out:rect - filled with text grid coords
-----------------------------------------------------------------------------*/
void CElemVarIn::GetTextRect(int /*iTxtIdx*/, CRect &rect, int iGrid)
{ 
   rect.SetRect((m_Origin.x                         )*iGrid,
                (m_Origin.y                         )*iGrid,
                (m_Origin.x + m_Size.cx - PIN_LENGTH)*iGrid,
                (m_Origin.y + m_Size.cy             )*iGrid);
}

void CElemVarIn::CreatePins(void)
{
   CPin     *pPin;

   pPin = new CPin(_T("_VarIn_"), _T(""), _T("ANY"), 
      GetOrigin() + CPoint(m_Size.cx,1), CSize(-PIN_LENGTH,0), PF_OUT);
   if( pPin ) m_Con.Add(pPin);
}

/*func*------------------------------------------------------------------------
  resize element to optimal width for text length
  in :-
  out:dorigin - delta origin  (necessary for reset size and position if 
      dsize   - delta size     overlap-check fails)
      true  - element is resized
      false - element is unchanged
-----------------------------------------------------------------------------*/
bool CElemVarIn::SetAutoSize(CPoint& dorigin, CSize& dsize) 
{ 
   int i = ((PIN_LENGTH + CText::GetIdentLen(m_Name.StrRef()) + 1) & 0xFFFFFFFE);
   if ( i < m_MinSize.cx ) i = m_MinSize.cx;
   if ( i > m_MaxSize.cx ) i = m_MaxSize.cx;
   dsize.cx  = i - m_Size.cx;
   dsize.cy  = 0;
   dorigin.x = -dsize.cx; 
   dorigin.y = 0;
   if (dsize.cx != 0)
   {
      Resize(dorigin.x, dorigin.y, dsize.cx, dsize.cy);
      return true;
   } else {
      return false;
   }
}

IMPLEMENT_SERIAL(CElemVarIn, CElemVar, SERIAL_VERSION)

//////////////////////////////////////////////////////////////////////
// CElemVarOut Class
//////////////////////////////////////////////////////////////////////

/*func*------------------------------------------------------------------------
  get the text space in elem
  in :rect - placeholder for result
  out:rect - filled with text grid coords
-----------------------------------------------------------------------------*/
void CElemVarOut::GetTextRect(int /*iTxtIdx*/, CRect &rect, int iGrid)
{ 
   rect.SetRect((m_Origin.x + PIN_LENGTH)*iGrid,
                (m_Origin.y             )*iGrid,
                (m_Origin.x + m_Size.cx )*iGrid,  
                (m_Origin.y + m_Size.cy )*iGrid); 
}

void CElemVarOut::CreatePins(void)
{
   CPin     *pPin;

   pPin = new CPin(_T("_VarOut_"), _T(""), _T("ANY"), 
      GetOrigin() + CPoint(0,1), CSize(PIN_LENGTH,0), PF_IN);
   if( pPin ) m_Con.Add(pPin);
}

/*func*------------------------------------------------------------------------
  resize element to optimal width for text length
  in :-
  out:dorigin - delta origin  (necessary for reset size and position if 
      dsize   - delta size     overlap-check fails)
      true  - element is resized
      false - element is unchanged
-----------------------------------------------------------------------------*/
bool CElemVarOut::SetAutoSize(CPoint& dorigin, CSize& dsize) 
{ 
   int i = ((PIN_LENGTH + CText::GetIdentLen(m_Name.StrRef()) + 1) & 0xFFFFFFFE);
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

IMPLEMENT_SERIAL(CElemVarOut, CElemVar, SERIAL_VERSION)

//////////////////////////////////////////////////////////////////////
// CElemVarInOut Class
//////////////////////////////////////////////////////////////////////

/*func*------------------------------------------------------------------------
  get the text space in elem
  in :rect - placeholder for result
  out:rect - filled with text grid coords
-----------------------------------------------------------------------------*/
void CElemVarInOut::GetTextRect(int /*iTxtIdx*/, CRect &rect, int iGrid)
{ 
   rect.SetRect((m_Origin.x             + PIN_LENGTH)*iGrid,
                (m_Origin.y                         )*iGrid,
                (m_Origin.x + m_Size.cx - PIN_LENGTH)*iGrid,
                (m_Origin.y + m_Size.cy             )*iGrid); 
}

void CElemVarInOut::CreatePins(void)
{
   CPin     *pPin;

   pPin = new CPin(_T("_VarInOut_"), _T(""), _T("ANY"), 
      GetOrigin() + CPoint(0,1), CSize(PIN_LENGTH,0), PF_OUT);
   if( pPin ) m_Con.Add(pPin);

   pPin = new CPin(_T("_VarInOut_"), _T(""), _T("ANY"), 
      GetOrigin() + CPoint(m_Size.cx,1), CSize(-PIN_LENGTH,0), PF_IN);
   if( pPin ) m_Con.Add(pPin);
}

/*func*------------------------------------------------------------------------
  resize element to optimal width for text length
  in :-
  out:dorigin - delta origin  (necessary for reset size and position if 
      dsize   - delta size     overlap-check fails)
      true  - element is resized
      false - element is unchanged
-----------------------------------------------------------------------------*/
bool CElemVarInOut::SetAutoSize(CPoint& dorigin, CSize& dsize) 
{ 
   TPinEntry *pPE;
   CPoint     p;
   CPin      *pPin;
   bool       bEastResize;
   int        i;

   ASSERT(m_Con.GetSize() == 2);
   ASSERT(m_pPinTable);

   for (i=0; i<2; i++)
   {
      pPin = m_Con.GetAt(i);
      ASSERT(pPin);
      if (pPin && pPin->m_Size.cx < 0)
      {
         break;
      }
   }
   if (m_pPinTable && pPin)
   {
      if (!m_pPinTable->Lookup(pPin->m_Pos, pPE))
      {
         ASSERT(0);
      } else {
         ASSERT(pPE);
         if (pPE)
         {
            if (pPE->m_iSelected + pPE->m_iDeSelected == 1)
            {
               bEastResize = false;
            } else {
               bEastResize = true;
            }
         }
      }
   }

   i = ((2*PIN_LENGTH + CText::GetIdentLen(m_Name.StrRef()) + 1) & 0xFFFFFFFE);
   if ( i < m_MinSize.cx ) i = m_MinSize.cx;
   if ( i > m_MaxSize.cx ) i = m_MaxSize.cx;
   dsize.cx  = i - m_Size.cx;
   dsize.cy  = 0;
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

bool CElemVarInOut::CheckDropIn( CElem* pE)     // requires a normalized line to check against
{
   CPin* pPin;
   bool bOK = false;

   ASSERT(pE != NULL);
   if((pE != NULL)                        &&
      (pE->GetType() == ET_LINE)          &&    // check if <INSIDE a HOIZONTAL LINE>
      (pE->GetSize().cy==0)               &&    // 
      (pE->GetOrigin().x<=m_Origin.x)     &&    // inside (!!!requires a normalized line)
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
   }
   return bOK;
}

bool CElemVarInOut::CheckDropOut()
{
   return true;
}

IMPLEMENT_SERIAL(CElemVarInOut, CElemVar, SERIAL_VERSION)

//////////////////////////////////////////////////////////////////////
// CElemCmt Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CElemCmt::CElemCmt()
{
   CSize size(6,4);

   SetType(ET_COMMENT); 

   SetMinSize(size);
   SetText(_T(""));
}

/*func*------------------------------------------------------------------------
  create a new comment 
  created element is selected !!
  in :Origin  - insertion point 
      Size    - startup size of element 
      strText - text for comment
  out:pointer to new comment element
-----------------------------------------------------------------------------*/
CElemCmt::CElemCmt(CPoint Origin, CSize Size, LPCTSTR pszText)
{
   CSize size(6,4);

   SetType(ET_COMMENT); 

   SetMinSize(size);

   SetOrigin (Origin);
   SetSize   (Size);
   
   SetText(pszText);
}

CElemCmt::~CElemCmt()
{
}

/*func*------------------------------------------------------------------------
  draw the comment
  in :pDC   - pointer to device context
      pView - pointer to document (data)
      iGrid - actual display resolution
  out:-
-----------------------------------------------------------------------------*/
void CElemCmt::Draw(CDC * pDC, CGrEditorView * pView, int iGrid)
{
   CRect    r;
   bool     bScreen = !pDC->IsPrinting();

   if (!pView)
   {
      return;
   }
   // prepare drawing
   pDC->SelectObject(GDIHelper::GetPen  (bScreen, PE_COMMENT));
   if (pView->IsManualMode())
   {
      pDC->SelectObject(GDIHelper::GetBrush(bScreen, BT_ELEMBK_SFC ));
   } else {
      pDC->SelectObject(GDIHelper::GetBrush(bScreen, BT_ELEMBK ));
   }

   // draw the comment rect
   GetRect(r, iGrid);
   pDC->Rectangle( &r );

   // draw element icon 
   int cx = DrawIcon(pDC, 
                     pView, 
                     CPoint(m_Origin.x + m_Size.cx - 2, m_Origin.y), 
                     IDB_GR_ICONCMT, 
                     iGrid).cx;

   // draw the comment text
   SearchDat *pSD = &pView->m_SearchDat;
   
   if( (pSD->m_FindCur   == GetOrigin()) &&
       (pSD->m_iFindElem == -1         ) &&
       (pSD->m_iFindPos  >= 0          ) )
   {
      m_Text.Draw(pDC, r, iGrid, GetColorType(), pSD, cx);  // cx>0: icon at the right of the 1st line
   }else
   {
      m_Text.Draw(pDC, r, iGrid, GetColorType(), NULL, cx);
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
  virtual base function
  get the data identifier of the element for watch (if necessary "elementname.pinname")
  in :-
  out:identifier of the element
-----------------------------------------------------------------------------*/
CText& CElemCmt::GetTextAt(CPoint /*p*/, CPoint &start, int /*iGrid*/)
{
   start = m_Origin;
   return m_Text;
}

/*func*------------------------------------------------------------------------
  get the text space in elem
  in :rect - placeholder for result
  out:rect - filled with text grid coords
-----------------------------------------------------------------------------*/
void CElemCmt::GetTextRect(int /*iTxtIdx*/, CRect &rect, int iGrid)
{ 
   rect.SetRect((m_Origin.x            )*iGrid, 
                (m_Origin.y            )*iGrid,
                (m_Origin.x + m_Size.cx)*iGrid, 
                (m_Origin.y + m_Size.cy)*iGrid); 
}

/*func*------------------------------------------------------------------------
  resize element to optimal width for text length and height
  in :-
  out:dorigin - delta origin  (necessary for reset size and position if 
      dsize   - delta size     overlap-check fails)
      true  - element is resized
      false - element is unchanged
-----------------------------------------------------------------------------*/
bool CElemCmt::SetAutoSize(CPoint& dorigin, CSize& dsize) 
{ 
   int i = (2 + (CText::GetIdentLen(m_Text.StrRef()) + 1) & 0xFFFFFFFE);
   int j =  2 * m_Text.CountLines();
   j = min(m_MaxSize.cy, max(m_MinSize.cy, j));

   if ( i < m_MinSize.cx ) i = m_MinSize.cx;
   if ( i > m_MaxSize.cx ) i = m_MaxSize.cx;
   dsize.cx  = i - m_Size.cx;
   dsize.cy  = j - m_Size.cy;
   dorigin.x = 0;
   dorigin.y = 0;
   if (dsize.cx != 0 || dsize.cy != 0)
   {
      Resize(dorigin.x, dorigin.y, dsize.cx, dsize.cy);
      return true;
   } else {
      return false;
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
BOOL CElemCmt::FindText(SearchDat& SD)
{
   if (SD.m_bFindOnly || m_Text.m_Flag.Chk(TF_EDITABLE))
   {
      if(m_Text.Find(SD))
      {
         ASSERT(SD.m_FindCur == GetOrigin());
         SD.m_iFindElem = -1;
         return TRUE;
      }
   }
   return FALSE;
}


/*func*------------------------------------------------------------------------
  get a pointer to line number iNr in comment
  in :iNr - line number (0 .. m_Text.CountLines()-1)
      str - string buffer for result
  out:TRUE  - ok, line copied
      FALSE - no comment text or invalid line number
-----------------------------------------------------------------------------*/
bool CElemCmt::GetLine(int iNr, CString& str2)
{
   CString  str;
   int  i;

   if(((m_Text.Str()).GetLength() == 0) || (iNr < 0)) 
   {
      return false;
   }
   str = m_Text.Str();
   while ( (str.GetLength() > 0) && (iNr > 0) )
   {
      iNr--;
      i = str.Find(_T("\r"));
      if( i == -1 ) i = str.GetLength()-2;
      str = str.Right( str.GetLength() - (i+2) );
   }
   if( str.GetLength() == 0 )
   {
      str2 = _T("");
      return false;
   } else {
      i = str.Find(_T("\r"));
      if( i == -1 ) i = str.GetLength();
      str2 = str.Left(i);
      return true;
   } 
}

/*func*------------------------------------------------------------------------
  set the text field of the element
  in :pszText - the new text
  out:-
-----------------------------------------------------------------------------*/
void CElemCmt::SetText(LPCTSTR pszText) 
{
   m_Text = pszText;
   m_Text.m_Flag += TF_EDITABLE;
   m_Text.m_Flag += TF_MULTILINE;
   m_Text.m_Flag += TF_LEFT;
//   m_Text.m_Flag += TF_WRAP;
}

/*func*------------------------------------------------------------------------
  get text of comment
  in :-
  out:the name
-----------------------------------------------------------------------------*/
LPCTSTR CElemCmt::GetText() 
{ 
   return (LPCTSTR)m_Text; 
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
bool CElemCmt::GetText(int iElemNr, int iPos, int iLen, CString& str)
{
   int      i, iLines;

   str = _T("");

   if (iElemNr < 0)
   {
      iLines = m_Text.CountLines();
      i = -iElemNr;
      if ( (i <= iLines) &&
           (GetLine( i-1, str)) )
      {
         str = (m_Text.Str()).Mid(iPos, iLen);
         return (true);
      }
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
void CElemCmt::ReplaceSelText(int iElemNr, int iPos, int iLen, LPCTSTR pstr)
{
   CString str;
   int  i, iLines;

   if (iElemNr < 0)
   {
      iLines = m_Text.CountLines();
      i = -iElemNr;
      if ( (i <= iLines) &&
           (GetLine( i-1, str)) )
      {
         (m_Text.StrRef()).Delete(iPos, iLen);
         (m_Text.StrRef()).Insert(iPos, pstr);
         return;
      }
   }
   else 
   {
      CElem::ReplaceSelText( iElemNr, iPos, iLen, pstr);
      return;
   }
}

/*func*------------------------------------------------------------------------
  serialize the element to/from stream
  in :ar - the archive
  out:-
-----------------------------------------------------------------------------*/
void CElemCmt::Serialize(CArchive & ar)
{
   CElem::Serialize(ar);

	if (ar.IsStoring())
	{
      ar << m_Text;
	}
	else
	{
      ar >> m_Text;
	}
}

/*func*------------------------------------------------------------------------
  serialize the element to/from XML stream
  in :ar - the archive
  out:-
-----------------------------------------------------------------------------*/
void CElemCmt::SerializeXML(CXMLArchive &ar)
{
   if(ar.IsStoring())
   {
      // all CXMLArchive fire HRESULT exceptions, which must be cought!!!
      try {            // name and create node here !!
         CElem::SerializeXML(ar);  // origin, size, selected, comment

         ar.MoveDown( XML_4GR_ATTR_OBJCOMMENT );
         ar.SetCData(m_Text.Str());
         ar.MoveUp();
      }        
      catch (HRESULT hr)
      {
         TRACE(_T("Cannot save to XML; hr=0x%08X\n"), hr);
         throw hr;   // fire upstairs...        
      }    
   } else {
      try {   
         CElem::SerializeXML(ar);  // origin, size, selected, comment

         ar.MoveDown( XML_4GR_ATTR_OBJCOMMENT );
         ar.GetCData(m_Text.StrRef());
         ar.MoveUp();
      }
      catch (HRESULT hr)
      {
         TRACE(_T("Cannot load from XML; hr=0x%08X\n"), hr);
         throw hr;   // fire upstairs...
      }   
   }
}

IMPLEMENT_SERIAL(CElemCmt, CElem, SERIAL_VERSION)

//////////////////////////////////////////////////////////////////////
// CElemLin Class
//////////////////////////////////////////////////////////////////////
CSize CElemLine::m_DefaultSize = CSize(6,6);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CElemLine::CElemLine()
{
   SetType(ET_LINE);

   SetFlag(EF_RUBBLINE);

   CreatePins();  // no pins in XML !!! for line, to be create here
}

/*func*------------------------------------------------------------------------
  copy constructor
  in :line - line for copy
-----------------------------------------------------------------------------*/
CElemLine::CElemLine(CElemLine * pLine)
{
   ASSERT(pLine);
   ASSERT_VALID(pLine);

   CopyData(pLine);
}

/*func*------------------------------------------------------------------------
  create a new line and insert them in the element list
  in :ps - start point in grid coords
      pe - end point of line in grid coords
-----------------------------------------------------------------------------*/
CElemLine::CElemLine(CPoint ps, CPoint pe, bool bRubber)
{
   int        i;

   SetType(ET_LINE);

   if( bRubber ) 
   {
      SetFlag(EF_RUBBLINE); 
   } else {
      ClrFlag(EF_RUBBLINE);
   }

   if( !bRubber )
   {  // normalisize of rect
      if( ps.x > pe.x ) { i = ps.x; ps.x = pe.x; pe.x = i; }
      if( ps.y > pe.y ) { i = ps.y; ps.y = pe.y; pe.y = i; }
   }
   m_Origin = ps;
   m_Size   = pe - ps;

   CreatePins();
}

CElemLine::~CElemLine()
{
}

void CElemLine::CreatePins(void)
{
   CPin      *pPin;

   pPin = new CPin(_T("_LINE_"), _T(""), _T("ANY"), 
      m_Origin, CSize(0,0), PF_INOUT);
   if( pPin ) m_Con.Add(pPin);
   
   pPin = new CPin(_T("_LINE_"), _T(""), _T("ANY"), 
      m_Origin + m_Size, CSize(0,0), PF_INOUT);
   if( pPin ) m_Con.Add(pPin);
}

/*func*------------------------------------------------------------------------
  draw the line
  in :pDC   - pointer to device context
      pView - pointer to document (data)
      iGrid - actual display resolution
  out:-
-----------------------------------------------------------------------------*/
void CElemLine::Draw(CDC * pDC, CGrEditorView * pView, int iGrid)
{
   CPoint   p1, p2;
   CSize    size;
   CRect    r;
   int      iGrid2;
   PEN_TYPE PenType;
   bool     bScreen = !pDC->IsPrinting();

   if (!pView)
   {
      return;
   }
   // prepare drawing
   if     ( ChkFlag(EF_COMMENT)   ) PenType = PE_COMMENT;
// emphasize line will be handled separate ..
// else if( m_Flag.Chk(EF_EMPHASIZE) ) PenType = PE_ERROR;
   else if( ChkFlag(EF_RUBBLINE)  ) PenType = PE_RUBBER;
   else if( ChkFlag(EF_EDITLINE)  ) PenType = PE_EDITLINE;
   else   /* normal */              PenType = PE_NORMAL;

   pDC->SelectObject(GDIHelper::GetPen  (bScreen, PenType  ));
   if (pView->IsManualMode())
   {
      pDC->SelectObject(GDIHelper::GetBrush(bScreen, BT_BACKGD_SFC));
   } else {
      pDC->SelectObject(GDIHelper::GetBrush(bScreen, BT_BACKGD));
   }

   // draw empty space around the line
   if( !ChkFlag(EF_RUBBLINE) && ((m_Size.cx != 0) || (m_Size.cy != 0)) )
   {
      if( (m_Size.cx > 0) || (m_Size.cy > 0) ) iGrid2 =  iGrid/2;
      else                                     iGrid2 = -iGrid/2;
      if( m_Size.cx > 0 )
      {        // horizontal 
         r.SetRect((m_Origin.x            ) * iGrid + iGrid2, 
                   (m_Origin.y            ) * iGrid - iGrid/2,
                   (m_Origin.x + m_Size.cx) * iGrid - iGrid2,
                   (m_Origin.y + m_Size.cy) * iGrid + iGrid/2);
      } else { // vertical
         r.SetRect((m_Origin.x            ) * iGrid - iGrid/2, 
                   (m_Origin.y            ) * iGrid + iGrid2,
                   (m_Origin.x + m_Size.cx) * iGrid + iGrid/2,
                   (m_Origin.y + m_Size.cy) * iGrid - iGrid2);
      }
      if (pView->IsManualMode())
      {
         pDC->FillRect(&r, GDIHelper::GetBrush(bScreen, BT_BACKGD_SFC));
      } else {
         pDC->FillRect(&r, GDIHelper::GetBrush(bScreen, BT_BACKGD));
      }
   }

   // draw the line
   p1 = pView->GridToDoc (m_Origin);
   pDC->MoveTo (p1);
   p2 = pView->GridToDoc (m_Origin + m_Size);
   pDC->LineTo (p2);

   if (ChkFlag(EF_RUBBLINE))
   {
      bool   bScreen = !pDC->IsPrinting();

      if (!m_Con[0]->m_Flag.Chk(PF_DISABLED))
      {
         r.SetRect(p1.x - 1, p1.y - 1, p1.x + 2, p1.y + 2);
         pDC->FillRect(r, GDIHelper::GetBrush(bScreen, BT_RUBBER));
      }
      if (!m_Con[1]->m_Flag.Chk(PF_DISABLED))
      {
         r.SetRect(p2.x - 1, p2.y - 1, p2.x + 2, p2.y + 2);
         pDC->FillRect(r, GDIHelper::GetBrush(bScreen, BT_RUBBER));
      }
   }

   // draw SEPARATE emphasize 
   if (ChkFlag(EF_EMPHASIZE)) 
   {
      CPoint points[5];
      CPoint p;

      if ((m_Size.cx == 0) || 
          (m_Size.cx > 0 && m_Size.cy > 0) ||
          (m_Size.cx < 0 && m_Size.cy < 0))
      {
         if (m_Size.cy < 0) 
         {
            p = p1; p1 = p2; p2 = p;
         }
         points[0].x = p1.x - 1;   points[0].y = p1.y - 1;
         points[1].x = p1.x + 1;   points[1].y = p1.y - 1;
         points[2].x = p2.x + 1;   points[2].y = p2.y + 1;
         points[3].x = p2.x - 1;   points[3].y = p2.y + 1;
      } else {
         if (m_Size.cx < 0) 
         {
            p = p1; p1 = p2; p2 = p;
         }
         points[0].x = p1.x - 1;   points[0].y = p1.y - 1;
         points[1].x = p2.x + 1;   points[1].y = p2.y - 1;
         points[2].x = p2.x + 1;   points[2].y = p2.y + 1;
         points[3].x = p1.x - 1;   points[3].y = p1.y + 1;
      }
      points[4] = points[0];  // close parallelogram
      pView->GridToDoc(r);
      pDC->SelectObject(GDIHelper::GetPen(bScreen, PE_ERROR));
	   pDC->Polygon(points, 5);
   }

   if( m_bDebug2 )
   {
      CGRTool::DIR_TYPE dt;
      CPoint po;
	   CFont  Font, *OldFont;
      CPin  *pPin[2];
      char   s[32];
      int    i, idist = 6;

      Font.CreateFont( 8,0,0,0,0,0,0,0,0,0,0,0,0,"MS Sans Serif");
      OldFont = pDC->SelectObject( &Font ); 
      ASSERT(m_Con.GetSize() == 2);
      pPin[0] = m_Con.GetAt(0);
      pPin[1] = m_Con.GetAt(1);
      for( i=0; i<2; i++ )
      {
         ASSERT_VALID(pPin[i]);
         if(i==0) dt = CGRTool::GetDir(pPin[1]->m_Pos, pPin[0]->m_Pos);
         else     dt = CGRTool::GetDir(pPin[0]->m_Pos, pPin[1]->m_Pos);
         switch ( dt ) 
         {
            case CGRTool::DT_NS: pDC->SetTextAlign( TA_RIGHT | TA_BOTTOM ); po = CPoint(-idist,-idist); break;
            case CGRTool::DT_WE: pDC->SetTextAlign( TA_RIGHT | TA_TOP    ); po = CPoint(-idist, idist); break;
            case CGRTool::DT_SN: pDC->SetTextAlign( TA_LEFT  | TA_TOP    ); po = CPoint( idist, idist); break;
            case CGRTool::DT_EW: pDC->SetTextAlign( TA_LEFT  | TA_BOTTOM ); po = CPoint( idist,-idist); break;
            default: po = CPoint(0,0); break;
         }  
         sprintf(s, "%i", pPin[i]->GetConnects());

         pDC->TextOut( pPin[i]->m_Pos.x*iGrid+po.x, 
                       pPin[i]->m_Pos.y*iGrid+po.y, s, strlen(s));
         pDC->SetPixel(pPin[i]->m_Pos.x*iGrid-1, pPin[i]->m_Pos.y*iGrid-1, RGB(255,0,0));
         pDC->SetPixel(pPin[i]->m_Pos.x*iGrid-1, pPin[i]->m_Pos.y*iGrid+1, RGB(255,0,0));
         pDC->SetPixel(pPin[i]->m_Pos.x*iGrid+1, pPin[i]->m_Pos.y*iGrid-1, RGB(255,0,0));
         pDC->SetPixel(pPin[i]->m_Pos.x*iGrid+1, pPin[i]->m_Pos.y*iGrid+1, RGB(255,0,0));
      }
      pDC->SelectObject( OldFont ); 
   }
}

/*func*------------------------------------------------------------------------
  draw the resize frame for the element 
  in :pDC   - pointer to device context
      pView - pointer to document
      iGrid - actual display resolution
  out:-
-----------------------------------------------------------------------------*/
void CElemLine::DrawSelectFrame(CDC * pDC, CGrEditorView * pView, int iGrid, 
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

      if( !ChkFlag(EF_RUBBLINE) ) 
      {
         GetRect(r, iGrid); 

         iX1 = r.left;  iY1 = r.top;
         iX2 = r.right; iY2 = r.bottom;
      } else {
         iX1 =  m_Origin.x * iGrid;
         iY1 =  m_Origin.y * iGrid;
         iX2 = (m_Origin.x + m_Size.cx) * iGrid;
         iY2 = (m_Origin.y + m_Size.cy) * iGrid;
      }

      pDC->MoveTo( iX1, iY1);
      pDC->LineTo( iX2, iY2);

      if (bWithResizePoints && !ChkFlag(EF_RUBBLINE))
      {
         // draw resize points
         if     ( ChkFlag(EF_COMMENT  ) ) PenType = PE_COMMENT;
         else if( ChkFlag(EF_EMPHASIZE) ) PenType = PE_ERROR;
         else   /* normal */                 PenType = PE_NORMAL;

         pDC->SelectObject(GDIHelper::GetPen  (bScreen, PenType  ));
         pDC->SelectObject(GDIHelper::GetBrush(bScreen, BT_SELECT)); 

         r.SetRect(iX1, iY1, iX1, iY1);
         r.InflateRect(SP_RAD,SP_RAD);
         pDC->Rectangle( r );

         r.SetRect(iX2, iY2, iX2, iY2);
         r.InflateRect(SP_RAD,SP_RAD);
         pDC->Rectangle( r );
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
RS_TYPE CElemLine::GetResizeType(CPoint p, int iGrid)
{
   CRect r;

   if (!ChkFlag(EF_RUBBLINE))
   {
      if (m_Size.cx != 0)
      {
         if ( GetResizeRect( RS_E , r, iGrid )->PtInRect( p ) ) return RS_E;
         if ( GetResizeRect( RS_W , r, iGrid )->PtInRect( p ) ) return RS_W;
      } else {
         if ( GetResizeRect( RS_N , r, iGrid )->PtInRect( p ) ) return RS_N;
         if ( GetResizeRect( RS_S , r, iGrid )->PtInRect( p ) ) return RS_S;
      }
   }
   return RS_LAST;
}

/*func*------------------------------------------------------------------------
  test if p at resize rect in element
  in :p     - point for test in logical coordinates at sheet
      iGrid - actual zoom
  out:TRUE  - yes on resize rect
      FALSE - not in resize rect
-----------------------------------------------------------------------------*/
bool CElemLine::IsResizeRect(CPoint p, int iGrid)
{
   CRect r;

   if(!ChkFlag(EF_RUBBLINE))
   {
      switch( ((m_Size.cx!=0)?1 :0) + ((m_Size.cy!=0) ?2 :0) )   // enumerate possible cases
      {
      case 0: return GetResizeRect( RS_E , r, iGrid )->PtInRect( p )!=0;

      case 1: return(GetResizeRect( RS_E , r, iGrid )->PtInRect( p ) ||
                     GetResizeRect( RS_W , r, iGrid )->PtInRect( p ));

      case 2: return(GetResizeRect( RS_S , r, iGrid )->PtInRect( p ) ||
                     GetResizeRect( RS_N , r, iGrid )->PtInRect( p ));

      default:
         ASSERT("cx*cy != 0: not allowed on fixed lines!"&&0);
      }
   }
   return false;
}

/*func*------------------------------------------------------------------------
  resize and/or move element with connected pins
  in :dx - delta x for m_Origin
      dy - delta y for m_Origin
      sx - delta x for m_Size
      sy - delta y for m_Size
  out:-
-----------------------------------------------------------------------------*/
bool CElemLine::Resize(int dx, int dy, int sx, int sy)
{
   CPoint p;
   CPin  *pPin;
   int    i;
   bool   bRes=true;
   bool   bPntLine;

   if (ChkFlag(EF_RUBBLINE) && !ChkFlag(EF_EDITLINE) )
   {
      ASSERT(m_Con.GetSize() == 2);
      ASSERT(m_pPinTable);

      if (m_pPinTable)
      {
         bPntLine = (m_Size.cx == 0) && (m_Size.cy == 0);

         for (i=0; i<m_Con.GetSize(); i++)
         {
            if ((pPin = m_Con.GetAt(i)) != NULL)
            {
               if (pPin->m_Flag.Chk(PF_SELECT))
               {
                  if (pPin->m_Entry)
                  {
                     p = pPin->m_Entry->m_NewPos - pPin->m_Pos;
                  } else {
                     p = CPoint(0,0);
                  }

                  if (pPin->m_Pos == m_Origin)
                  {
                     m_Origin += p;
                     m_Size   -= CSize(p);
                  } else {
                     m_Size   += CSize(p);
                  }

                  ASSERT(pPin->m_Entry);
                  if (pPin->m_Entry)
                  {
                     pPin->m_Pos = pPin->m_Entry->m_NewPos;
                     m_pPinTable->MovePin(this, pPin->m_Entry, pPin->m_Entry->m_NewPos);
                  }
               }
            }
         }
      }
   } 
   else 
   {
      if (m_Size != CSize(0,0) && !ChkFlag(EF_RUBBLINE))
      {
         if ((m_Size.cx == 0) && (sx != 0)) 
         { 
            bRes = false;
         }
         if ((m_Size.cy == 0) && (sy != 0)) 
         { 
            bRes = false;
         }
      }

      if (bRes &&
          ((int)((m_Origin.x + dx) & 0x00000001) == m_OriginOffs.x) &&
          ((int)((m_Origin.y + dy) & 0x00000001) == m_OriginOffs.y))
      {  
         ASSERT(m_Con.GetSize() == 2);

         for (i=0; i<m_Con.GetSize(); i++)
         {
            if( (pPin = m_Con.GetAt(i)) != NULL )
            {
               if( m_Size == CSize(0,0) )
               {
                  pPin->m_Pos.x = m_Origin.x + dx + i*sx;
                  pPin->m_Pos.y = m_Origin.y + dy + i*sy;
               } else {
                  // move connect points (pins)
                  if( pPin->m_Pos == m_Origin )
                  {
                     pPin->m_Pos.x += dx;
                     pPin->m_Pos.y += dy;
                  } else {
                     pPin->m_Pos.x += (dx + sx);
                     pPin->m_Pos.y += (dy + sy);
                  }
               }
               if (m_pPinTable != NULL)
               {
                  ASSERT(pPin->m_Entry);
                  if (pPin->m_Entry)
                  {
                     m_pPinTable->MovePin(this, pPin->m_Entry, pPin->m_Pos);
                  }
               }
            }
         }
         // resize base element
         m_Origin.x += dx;
         m_Origin.y += dy;
         m_Size.cx  += sx;
         m_Size.cy  += sy;
      } else {
         bRes = false;
      }
   }
   return bRes;
}

bool CElemLine::IsSpaceable( CPoint At, CPoint Delta, bool /*bLabelClip*/)
{
   bool ok = true;

   ASSERT( Delta.x==0 || Delta.y==0 );
   if(Delta.x<0)
   {
      ok = !(GetSize().cx==0 && GetSize().cy!=0 && GetOrigin().x==At.x);
   }else
   {
      if(Delta.y<0)
      {
         ok = !(GetSize().cy==0 && GetSize().cx!=0 && GetOrigin().y==At.y);
      }
   }
   return ok;
}

/*func*------------------------------------------------------------------------
  no search in lines !!
  in :iElemNr  - neg. numbers are reserved for inherit classes,
                 pos. numbers are the pins in m_con
      iPos     - position in strname in pin
      lpszFind - search text
      bNext    - search direction
      bCase    - flag for matching cases
  out:TRUE  - string found (iElemNr and iPos will be updated)
      FALSE - nothing found
-----------------------------------------------------------------------------*/

BOOL CElemLine::FindText(SearchDat& /*SD*/)
{
   return FALSE;
}

/*func*------------------------------------------------------------------------
  swap end points of line
  in :- 
  out:-
-----------------------------------------------------------------------------*/
void CElemLine::SwapOrigin()
{
   // reorientation  
   ASSERT((m_Size.cx==0) || (m_Size.cy==0));
   // Attention: no use of SetOrigin or GetOrigin !!!
   m_Origin += m_Size;
   m_Size.cx = -m_Size.cx;
   m_Size.cy = -m_Size.cy;
}

/*func*------------------------------------------------------------------------
  serialize the element to/from stream
  in :ar - the archive
  out:-
-----------------------------------------------------------------------------*/
void CElemLine::Serialize(CArchive & ar)
{
   CElem::Serialize(ar);
}

/*func*------------------------------------------------------------------------
  serialize the element to/from XML stream
  in :ar - the archive
  out:-
-----------------------------------------------------------------------------*/
void CElemLine::SerializeXML(CXMLArchive &ar)
{
   CPin *pPin;
   long  l;

   if(ar.IsStoring())
   {
      // all CXMLArchive fire HRESULT exceptions, which must be cought!!!
      try {            // name and create node here !!
         CElem::SerializeXML(ar);  // origin, size, selected, comment
         if( ChkFlag(EF_RUBBLINE) ) 
         {
            ar.SetAttribute( XML_4GR_ATTR_RUBBERLINE, 1L);
         }
      }        
      catch (HRESULT hr)
      {
         TRACE(_T("Cannot save to XML; hr=0x%08X\n"), hr);
         throw hr;   // fire upstairs...        
      }    
   } else {
      try {   
         CElem::SerializeXML(ar);  // origin, size, selected, comment
         if( ar.GetAttribute( XML_4GR_ATTR_RUBBERLINE, l) && l==1 ) 
         {
            SetFlag(EF_RUBBLINE);
         } else {
            ClrFlag(EF_RUBBLINE);
         }

         ASSERT( m_Con.GetSize() == 2 );
         if( m_Con.GetSize() == 2 ) 
         {
            pPin = m_Con.GetAt(0); ASSERT( pPin ); if( pPin ) pPin->m_Pos = m_Origin;
            pPin = m_Con.GetAt(1); ASSERT( pPin ); if( pPin ) pPin->m_Pos = m_Origin + m_Size;
         }
      }
      catch (HRESULT hr)
      {
         TRACE(_T("Cannot load from XML; hr=0x%08X\n"), hr);
         throw hr;   // fire upstairs...
      }   
   }
}


bool CElemLine::Spacing( CPoint Point, CPoint Delta, CPosArray* /*paPin*/ , bool /*bLabelClip*/)
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

      if( ChkFlag(EF_RUBBLINE) )
      {
         for(int iCon=0; iCon<m_Con.GetSize(); iCon++)      // alle Pins
         {
            if(m_Con[iCon]->m_Pos.x >= Point.x)             // wenn >= y-Position
            {
               m_Con[iCon]->m_Flag.Set(PF_SELECT);
            }else
            {
               m_Con[iCon]->m_Flag.Clr(PF_SELECT);
            }
         }
      }

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

      if( ChkFlag(EF_RUBBLINE) )
      {
         for(int iCon=0; iCon<m_Con.GetSize(); iCon++)   // alle Pins
         {
            if(m_Con[iCon]->m_Pos.y >= Point.y)           // wenn >= y-Position
            {
               m_Con[iCon]->m_Flag.Set(PF_SELECT);
            }else
            {
               m_Con[iCon]->m_Flag.Clr(PF_SELECT);
            }
         }
      }

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

   //---------- negative spacing
   if( (d = Delta.x) < 0 )
   {
      i1 = min(m_Origin.x, m_Origin.x + m_Size.cx);
      i2 = max(m_Origin.x, m_Origin.x + m_Size.cx);

      if( ChkFlag(EF_RUBBLINE) )
      {
         for(int iCon=0; iCon<m_Con.GetSize(); iCon++)      // alle Pins
         {
            if(m_Con[iCon]->m_Pos.x >= Point.x)             // wenn >= y-Position
            {
               m_Con[iCon]->m_Flag.Set(PF_SELECT);
            }else
            {
               m_Con[iCon]->m_Flag.Clr(PF_SELECT);
            }
         }
      }

      if( Point.x < i1)                                    // element is strictly right
      {
         bRes &= Resize( d,0,0,0);
         bDone = true;
      }else
      {
         if( (Point.x >= i1) && (Point.x < i2) )
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

   if( (d = Delta.y) < 0 )
   {
      i1 = min(m_Origin.y, m_Origin.y + m_Size.cy);
      i2 = max(m_Origin.y, m_Origin.y + m_Size.cy);

      if( ChkFlag(EF_RUBBLINE) )
      {
         for(int iCon=0; iCon<m_Con.GetSize(); iCon++)      // alle Pins
         {
            if(m_Con[iCon]->m_Pos.y >= Point.y)             // wenn >= y-Position
            {
               m_Con[iCon]->m_Flag.Set(PF_SELECT);
            }else
            {
               m_Con[iCon]->m_Flag.Clr(PF_SELECT);
            }
         }
      }

      if( Point.y < i1)                                     // element is strictly right
      {
         bRes &= Resize( 0,d,0,0);
         bDone = true;
      }else
      {
         if( (Point.y >= i1) && (Point.y < i2) )
         {
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
   ASSERT(bRes);
   return bDone;
}

void CElemLine::GetBoundingRect(CRect& r, int iGrid)
{
   if( !ChkFlag(EF_RUBBLINE) && ((m_Size.cx != 0) || (m_Size.cy != 0)) )
   {    
      if( m_Size.cx != 0 )
      {  // horizontal 
         ASSERT(m_Size.cy==0);
         r.SetRect((m_Origin.x            ) * iGrid,
                   (m_Origin.y            ) * iGrid - iGrid/2,
                   (m_Origin.x + m_Size.cx) * iGrid,
                   (m_Origin.y            ) * iGrid + iGrid/2);

         r.NormalizeRect();
      }else 
      {  // vertical
         r.SetRect((m_Origin.x            ) * iGrid - iGrid/2, 
                   (m_Origin.y            ) * iGrid,
                   (m_Origin.x            ) * iGrid + iGrid/2,
                   (m_Origin.y + m_Size.cy) * iGrid);

         r.NormalizeRect();
      }
   }else
   {
      GetRect(r, iGrid);
   }
   r.InflateRect(SP_RAD+1,SP_RAD+1);  // increase for selection
}


IMPLEMENT_SERIAL(CElemLine, CElem, SERIAL_VERSION)
