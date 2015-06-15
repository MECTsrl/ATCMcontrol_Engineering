

#include "stdafx.h"
#include "PosArray.h"
#include "Elem.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

inline DWORD  CPosArray::ToDWORD(const CPoint P)
{
   return (P.x & 0xFFFF) | (((ULONG)(P.y & 0xFFFF)) << 16);
}

inline CPoint CPosArray::ToPoint(const DWORD dw)
{
   return CPoint((int)((ULONG)( dw        & 0xFFFF)),
                 (int)((ULONG)((dw >> 16) & 0xFFFF)) );
}

CPosArray::CPosArray()
{
}

CPosArray::~CPosArray()
{
}

void CPosArray::AddElem(class CElem *pE)
{
   CPin  *pPin;
   DWORD  dw;
   int    i;
      
   for( i=0; i<pE->m_Con.GetSize(); i++ )
   {
      pPin  = pE->m_Con.GetAt(i);  
      if( pPin != NULL )
      {
         dw = ToDWORD(pPin->m_Pos);
         Add( dw );
      }
   }
}

void CPosArray::AddPoint(class CPoint P)
{
   Add( ToDWORD(P) );
}


CPoint CPosArray::GetAt( int nIndex )
{
   DWORD dw = CDWordArray::GetAt( nIndex );
   return ToPoint(dw);
}

void CPosArray::Join(CPosArray& SrcArray)
{
   ASSERT(&SrcArray);
   int iNSrc = SrcArray.GetSize();

   for(int iSrc=0; iSrc<iNSrc; iSrc++)
   {
      CPoint P = SrcArray.GetAt(iSrc);

      if(Find(P)<0)
         Add(ToDWORD(P));
   }
}

void CPosArray::Drop(CPosArray& SrcArray)
{
   ASSERT(&SrcArray);
   int iNSrc = SrcArray.GetSize();

   for(int iSrc=0; iSrc<iNSrc; iSrc++)
   {
      CPoint P = SrcArray.GetAt(iSrc);

      int pos = Find(P);
      if(pos >= 0)
         RemoveAt(pos);
   }
}

int CPosArray::Find(CPoint P)
{
   int iSize = GetSize();
   for(int iDst=0; iDst<iSize; iDst++)
   {
      if( GetAt(iDst)==P)
         return iDst;
   }
   return -1;
}


bool CPosArray::FindNext(int* pP1, int* pP2, CPosArray& SrcArray)
{
   bool bFound=false;
   if((GetSize()!=0) && (SrcArray.GetSize()!=0))
   {
      double d_min = 1.7E+307;
      int    i_min = -1;
      int    j_min = -1;
      int    i;
      int    j;

      int N = GetSize();
      for(i = 0; i < N; i++ )
      {
         for(j = 0; j < SrcArray.GetSize(); j++ )
         {
            CPoint P1 = GetAt(i);
            CPoint P2 = SrcArray.GetAt(j);
            double d  = abs(P1.x - P2.x) + abs(P1.y - P2.y);

            if( d_min > d)
            {
               d_min = d;
               i_min = i;
               j_min = j;
            }
         }
         bFound = ((i_min >= 0) && (j_min >= 0));
         if( bFound )
         {
            *pP1 = i_min;
            *pP2 = j_min;
         }
      }
   }
   return bFound;
}

#ifdef _DEBUG
bool CPosArray::DebugDump(const CString& txt)      // returns always true
{
   _RPT1(_CRT_WARN,"\n%s\t",txt);
   for(int i=0; i<GetSize(); i++)
   {
      _RPT2(_CRT_WARN,"(%i,%i)\t",GetAt(i).x, GetAt(i).y);
   }
   return true;
}
#endif;
