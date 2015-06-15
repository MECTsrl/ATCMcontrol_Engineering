

/*doc*=======================================================================*/
/*inc*=======================================================================*/

#include "stdafx.h"

#include "GrEditor.h"
#include "Elem.h"
#include "ElemFB.h"
#include "ElemSfc.h"
#include "ElemLd.h"
#include "ElemPtrList.h"
#include "GrEditorUtil.h"
#include "PosArray.h"
#include "Router.h"
/*const*=====================================================================*/
/*macro*=====================================================================*/
/*glovar*====================================================================*/
/*type*======================================================================*/

TPinEntry::TPinEntry()
{
   m_Pos = CPoint(0,0);
   m_iSelected   = 0;
   m_iDeSelected = 0;
   m_iRubbLines  = 0;
   m_iConnects   = 0;
}

/*locvar*====================================================================*/

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/*proto*=====================================================================*/
/*func*======================================================================*/

// gives a <unsigned long> with a single bit set (for mask generation)
inline unsigned long LBit(ELEM_TYPE et)
   { 
      return 1L << et; 
   }

// check for X being (not strictly) inside a range
inline bool chkValAtRange(int x, int xmin, int xmax)
{
   return (x>=xmin) && (x<=xmax);
}

// check for a point being (not strictly) inside a rectangle
inline bool chkPointAtRect(CPoint P, CPoint TL, CPoint BR)
{
   return chkValAtRange(P.x, TL.x, BR.x) &&
          chkValAtRange(P.y, TL.y, BR.y);
}

//  check for a point being (not strictly) inside a rectangle
inline bool chkPointAtRect(CPoint P, CRect R)
{
   return chkValAtRange(P.x, R.left, R.right) &&
          chkValAtRange(P.y, R.top,  R.bottom);
}

// check for X being (strictly) inside a range
inline bool chkValInRange(int x, int xmin, int xmax)
{
   return (x>xmin) && (x<xmax);
}

// check for a point being (strictly) inside a rectangle
inline bool chkPointInRect(CPoint P, CPoint TL, CPoint BR)
{
   return chkValInRange(P.x, TL.x, BR.x) &&
          chkValInRange(P.y, TL.y, BR.y);
}

//  check for a point being (strictly) inside a rectangle
inline bool chkPointInRect(CPoint P, CRect R)
{
   return chkValInRange(P.x, R.left, R.right) &&
          chkValInRange(P.y, R.top,  R.bottom);
}

// use Origin,Size for line definition!
inline bool chkPointAtLine(CPoint P, CPoint O, CSize S)
{
   CPoint E = O + S;
   if(!S.cx) // vertical line
   {
      return chkValAtRange(P.y, min(O.y, E.y), max(O.y, E.y));
   }else if(!S.cy) // horizontal line
   {
      return chkValAtRange(P.x, min(O.x, E.x), max(O.x, E.x));
   }else          // any diagonal line
   {
      if(P == O || P == E) // at any end point
      {
         return true;
      }else
      {
         if(chkValAtRange(P.y, min(O.y, E.y), max(O.y, E.y)) && 
            chkValAtRange(P.x, min(O.x, E.x), max(O.x, E.x))  )
         {
            float dm = (float)(O.x - E.x)/(float)(O.y - E.y)- 
                       (float)(P.x - E.x)/(float)(P.y - E.y);
            const float eps = 0.25f; // 0.25: closer than 1/4 grid
            return ( (dm>0.0 ?dm :-dm) < eps); // identical to: fabs(m1-m2) < eps
         }
      }
   }
   return false;
}

inline int PointToInt(const CPoint& P)                // conversion of CPoint to int
{
   ASSERT(MAX_XGRID*MAX_YGRID < INT_MAX);
   return P.y*MAX_XGRID + P.x;
}

inline int PointToIntSwap(const CPoint& P)            // -''- (to change the search order fon x->y to y->x)
{
   ASSERT(MAX_XGRID*MAX_YGRID < INT_MAX);
   return P.x*MAX_XGRID + P.y;
}

inline CPoint IntToPoint(int iPos)                    // conversion from int to CPoint;
{
   ASSERT(MAX_XGRID*MAX_YGRID < INT_MAX);
   ASSERT(iPos > 0);
   return CPoint(iPos%MAX_XGRID, iPos/MAX_XGRID);
}

bool operator<(const CPoint& P1, const CPoint& P2)    // defines an order on xy-coordinates
{
   return PointToInt(P1) < PointToInt(P2) ?true :false;
}

// used for sorting CPoint
int PointIntOrder(const CPoint* p1, const CPoint* p2)
{
   ASSERT(p1);
   ASSERT(p2);
   if(*p1 == *p2)
      return 0;
   if(*p1 < *p2)
      return -1;
   return 1;
}
// true if there is no intersection of intervals [A1,A2] and [B1,B2]
// otherwise false and the arguments are sorted so that A1<=A2 and B1<=B2
inline bool CheckSort(long& A1, long& A2, long& B1, long& B2)
{
   long temp;
   if( A1 < A2)                        // right order of A
   {
      if(B1 < B2)                      // right order of B
      {
         if(A2 < B1 || A1 > B2)        // A strictly (above||below) B
            return true;
      }else                            // B is reversed
      {
         if(A2 < B2 || A1 > B1)        // A strictly (above||below) B
            return true;
         temp = B1;                    // adjust B
         B1 = B2;
         B2 = temp;
      }
   }else                               // A is reversed
   {
      if(B1 < B2)                      // right order of B
      {
         if(A1 < B1 || A2 > B2)        // A strictly (above||below) B
            return true;
      }else                            // B is reversed
      {
         if(A1 < B2 || A2 > B1)        // A strictly (above||below) B
            return true;
         temp = B1;                    // adjust B
         B1 = B2;
         B2 = temp;
      }
      temp = A1;                       // adjust A
      A1 = A2;
      A2 = temp;
   }
   return false;
}

//============================================================================= COvlTest

class COvlTest
{
   public:
      COvlTest();
      bool ValidPositionQ(CElem* pE1, CElem* pE2);

   private:
      ULONG m_Tab[ ET_LAST ][ DIR_LAST ];

      inline void          Init ();
      inline bool          TabQ (CElem* pE1, TDirection Dir, CElem* pE2);
                           // E2 is at <Dir> relative to E1
} OvlTable; // <- declaration AND global instantiation

/*func*------------------------------------------------------------------------
  constructor of COvlTest
-----------------------------------------------------------------------------*/
COvlTest::COvlTest()
{
   ASSERT( ET_LAST < 8 * sizeof(ULONG) );
   ZeroMemory( m_Tab, sizeof(m_Tab) );
   Init();
}

/*func*------------------------------------------------------------------------
  initialization of the COvlTest class
-----------------------------------------------------------------------------*/
void COvlTest::Init()
{
   ULONG _Act     = LBit(ET_ACTION);
   ULONG _Ab      = LBit(ET_ACTIONBLOCK);
   ULONG _Rem     = LBit(ET_COMMENT);
   ULONG _Ci      = LBit(ET_CONIN);
   ULONG _Co      = LBit(ET_CONOUT);
// ULONG _Const   = LBit(ET_CONST);
   ULONG _Ad      = LBit(ET_ALTDIV);
   ULONG _Sd      = LBit(ET_SIMDIV);
   ULONG _Fb      = LBit(ET_FB);
   ULONG _Fn      = LBit(ET_FUNCTION);
   ULONG _Prc     = LBit(ET_PROCEDURE);
   ULONG _Lbl     = LBit(ET_LABEL);
   ULONG _Jp      = LBit(ET_JUMP);
   ULONG _Lin     = LBit(ET_LINE);
   ULONG _Ret     = LBit(ET_RETURN);
   ULONG _St      = LBit(ET_STEP);
   ULONG _Tr      = LBit(ET_TRANSITION);
   ULONG _Vi      = LBit(ET_VARIN);
   ULONG _Vo      = LBit(ET_VAROUT);
   ULONG _Vio     = LBit(ET_VARINOUT);
   ULONG _PwR     = LBit(ET_POWERRAIL);
   ULONG _Ct      = LBit(ET_CONTACT);
   ULONG _Cl      = LBit(ET_COIL);

   ULONG _Test    = LBit(ET_LAST);
// ULONG _All     =              _Act|_Ab|_Rem|_Ci|_Co|_Ad|_Sd|_Fb|_Fn|_Prc|_Lbl|_Jp|_Lin|_Ret|_St|_Tr|_Vi|_Vo|_Vio|_PwR|_Ct|_Cl      ;
   ULONG _None    = 0;        

   // allowed placement conditions: (the placement conditions are OR-ed to allow a single placement)
   // _Test causes the virtual CElem::JoinTest(...) of this CElem-instance to be called (ie. to check for pin positions)

   m_Tab[ET_ACTION     ][DIR_N]=                                            _Lbl                                                      ;
   m_Tab[ET_ACTION     ][DIR_S]=                                            _Lbl                                                      ;
   m_Tab[ET_ACTION     ][DIR_E]=                                                                                                 _None;
   m_Tab[ET_ACTION     ][DIR_W]=                                                                                                 _None;

   m_Tab[ET_ACTIONBLOCK][DIR_N]=      _Ab|                                  _Lbl                                                      ;
   m_Tab[ET_ACTIONBLOCK][DIR_S]=      _Ab|                                  _Lbl                                                      ;
   m_Tab[ET_ACTIONBLOCK][DIR_E]=                                                                                                 _None;
   m_Tab[ET_ACTIONBLOCK][DIR_W]=                                                                                                 _Test;

   m_Tab[ET_COMMENT    ][DIR_N]=          _Rem                                                                                        ;
   m_Tab[ET_COMMENT    ][DIR_S]=          _Rem                                                                                        ;
   m_Tab[ET_COMMENT    ][DIR_E]=          _Rem                                                                                        ;
   m_Tab[ET_COMMENT    ][DIR_W]=          _Rem                                                                                        ;

   m_Tab[ET_CONIN      ][DIR_N]=               _Ci|_Co|                     _Lbl|_Jp|     _Ret|        _Vi|_Vo|_Vio|     _Ct|_Cl      ;
   m_Tab[ET_CONIN      ][DIR_S]=               _Ci|_Co|                     _Lbl|_Jp|     _Ret|        _Vi|_Vo|_Vio|     _Ct|_Cl      ;
   m_Tab[ET_CONIN      ][DIR_E]=                   _Co|        _Fb|_Fn|_Prc|     _Jp|_Lin|_Ret|    _Tr|    _Vo|_Vio|_PwR|_Ct|_Cl      ;
   m_Tab[ET_CONIN      ][DIR_W]=                                                                                                 _None;

   m_Tab[ET_CONOUT     ][DIR_N]=               _Ci|_Co|                     _Lbl|_Jp|     _Ret|        _Vi|_Vo|_Vio|     _Ct|_Cl      ;
   m_Tab[ET_CONOUT     ][DIR_S]=               _Ci|_Co|                     _Lbl|_Jp|     _Ret|        _Vi|_Vo|_Vio|     _Ct|_Cl      ;
   m_Tab[ET_CONOUT     ][DIR_E]=                                                                                                 _None;
   m_Tab[ET_CONOUT     ][DIR_W]=               _Ci|            _Fb|_Fn|_Prc|         _Lin|             _Vi|    _Vio|_PwR|_Ct|_Cl      ;

   m_Tab[ET_ALTDIV     ][DIR_N]=                                                               _St|_Tr                                ;
   m_Tab[ET_ALTDIV     ][DIR_S]=                                                               _St|_Tr                                ;
   m_Tab[ET_ALTDIV     ][DIR_E]=                                                                                                 _None;
   m_Tab[ET_ALTDIV     ][DIR_W]=                                                                                                 _None;

   m_Tab[ET_SIMDIV     ][DIR_N]=                                                               _St|_Tr                                ;
   m_Tab[ET_SIMDIV     ][DIR_S]=                                                               _St|_Tr                                ;
   m_Tab[ET_SIMDIV     ][DIR_E]=                                                                                                 _None;
   m_Tab[ET_SIMDIV     ][DIR_W]=                                                                                                 _None;

   m_Tab[ET_FB         ][DIR_N]=                                            _Lbl|    _Lin                                             ;
   m_Tab[ET_FB         ][DIR_S]=                                            _Lbl|    _Lin                                             ;
   m_Tab[ET_FB         ][DIR_E]=                   _Co|        _Fb|_Fn|_Prc|     _Jp|_Lin|_Ret|    _Tr|    _Vo|_Vio|_PwR|_Ct|_Cl      ;
   m_Tab[ET_FB         ][DIR_W]=               _Ci|            _Fb|_Fn|_Prc|         _Lin|             _Vi|    _Vio|_PwR|_Ct|_Cl      ;

   m_Tab[ET_FUNCTION   ][DIR_N]=                                            _Lbl|    _Lin                                             ;
   m_Tab[ET_FUNCTION   ][DIR_S]=                                            _Lbl|    _Lin                                             ;
   m_Tab[ET_FUNCTION   ][DIR_E]=                   _Co|        _Fb|_Fn|_Prc|     _Jp|_Lin|_Ret|    _Tr|    _Vo|_Vio|_PwR|_Ct|_Cl      ;
   m_Tab[ET_FUNCTION   ][DIR_W]=               _Ci|            _Fb|_Fn|_Prc|         _Lin|             _Vi|    _Vio|_PwR|_Ct|_Cl      ;

   m_Tab[ET_PROCEDURE  ][DIR_N]=                                            _Lbl|    _Lin                                             ;
   m_Tab[ET_PROCEDURE  ][DIR_S]=                                            _Lbl|    _Lin                                             ;
   m_Tab[ET_PROCEDURE  ][DIR_E]=                   _Co|        _Fb|_Fn|_Prc|     _Jp|_Lin|_Ret|    _Tr|    _Vo|_Vio|_PwR|_Ct|_Cl      ;
   m_Tab[ET_PROCEDURE  ][DIR_W]=               _Ci|            _Fb|_Fn|_Prc|         _Lin|             _Vi|    _Vio|_PwR|_Ct|_Cl      ;

   m_Tab[ET_LABEL      ][DIR_N]= _Act|_Ab|     _Ci|_Co|        _Fb|_Fn|_Prc|_Lbl|_Jp|     _Ret|        _Vi|_Vo|_Vio|     _Ct|_Cl      ;
   m_Tab[ET_LABEL      ][DIR_S]= _Act|_Ab|     _Ci|_Co|        _Fb|_Fn|_Prc|_Lbl|_Jp|     _Ret|        _Vi|_Vo|_Vio|     _Ct|_Cl      ;
   m_Tab[ET_LABEL      ][DIR_E]=                                                                                                 _None;
   m_Tab[ET_LABEL      ][DIR_W]=                                                                                                 _None;

   m_Tab[ET_JUMP       ][DIR_N]=               _Ci|_Co|                     _Lbl|_Jp|     _Ret|        _Vi|_Vo|_Vio|     _Ct|_Cl      ;
   m_Tab[ET_JUMP       ][DIR_S]=               _Ci|_Co|                     _Lbl|_Jp|     _Ret|        _Vi|_Vo|_Vio|     _Ct|_Cl      ;
   m_Tab[ET_JUMP       ][DIR_E]=                                                                                                 _None;
   m_Tab[ET_JUMP       ][DIR_W]=               _Ci|            _Fb|_Fn|_Prc|         _Lin|             _Vi|    _Vio|_PwR|_Ct|_Cl      ;

   m_Tab[ET_LINE       ][DIR_N]=                               _Fb|_Fn|_Prc|                   _St|_Tr|                  _Ct|_Cl      ;
   m_Tab[ET_LINE       ][DIR_S]=                               _Fb|_Fn|_Prc|                   _St|_Tr|                  _Ct|_Cl      ;
   m_Tab[ET_LINE       ][DIR_E]=                   _Co|        _Fb|_Fn|_Prc|     _Jp|     _Ret|    _Tr|    _Vo|_Vio|     _Ct|_Cl      ;
   m_Tab[ET_LINE       ][DIR_W]=               _Ci|            _Fb|_Fn|_Prc|                           _Vi|    _Vio|     _Ct|_Cl      ;

   m_Tab[ET_RETURN     ][DIR_N]=               _Ci|_Co|                     _Lbl|_Jp|     _Ret|        _Vi|_Vo|_Vio|     _Ct|_Cl      ;
   m_Tab[ET_RETURN     ][DIR_S]=               _Ci|_Co|                     _Lbl|_Jp|     _Ret|        _Vi|_Vo|_Vio|     _Ct|_Cl      ;
   m_Tab[ET_RETURN     ][DIR_E]=                                                                                                 _None;
   m_Tab[ET_RETURN     ][DIR_W]=               _Ci|            _Fb|_Fn|_Prc|         _Lin|             _Vi|    _Vio|_PwR|_Ct|_Cl      ;

   m_Tab[ET_STEP       ][DIR_N]=                       _Ad|_Sd|                      _Lin|         _Tr                                ;
   m_Tab[ET_STEP       ][DIR_S]=                       _Ad|_Sd|                      _Lin|         _Tr                                ;
   m_Tab[ET_STEP       ][DIR_E]=                                                                                                 _Test;
   m_Tab[ET_STEP       ][DIR_W]=                                                                                                 _Test;

   m_Tab[ET_TRANSITION ][DIR_N]=                       _Ad|_Sd|                      _Lin|     _St                                    ;
   m_Tab[ET_TRANSITION ][DIR_S]=                       _Ad|_Sd|                      _Lin|     _St                                    ;
   m_Tab[ET_TRANSITION ][DIR_E]=                                                                                                 _None;
   m_Tab[ET_TRANSITION ][DIR_W]=               _Ci|            _Fb|_Fn|_Prc|         _Lin|             _Vi|    _Vio|_PwR|_Ct|_Cl      ;

   m_Tab[ET_VARIN      ][DIR_N]=               _Ci|_Co|                     _Lbl|_Jp|     _Ret|        _Vi|_Vo|_Vio|     _Ct|_Cl      ;
   m_Tab[ET_VARIN      ][DIR_S]=               _Ci|_Co|                     _Lbl|_Jp|     _Ret|        _Vi|_Vo|_Vio|     _Ct|_Cl      ;
   m_Tab[ET_VARIN      ][DIR_E]=                   _Co|        _Fb|_Fn|_Prc|     _Jp|_Lin|_Ret|    _Tr|    _Vo|_Vio                   ;
   m_Tab[ET_VARIN      ][DIR_W]=                                                                                                 _None;

   m_Tab[ET_VAROUT     ][DIR_N]=               _Ci|_Co|                     _Lbl|_Jp|     _Ret|        _Vi|_Vo|_Vio|     _Ct|_Cl      ;
   m_Tab[ET_VAROUT     ][DIR_S]=               _Ci|_Co|                     _Lbl|_Jp|     _Ret|        _Vi|_Vo|_Vio|     _Ct|_Cl      ;
   m_Tab[ET_VAROUT     ][DIR_E]=                                                                                                 _None;
   m_Tab[ET_VAROUT     ][DIR_W]=               _Ci|            _Fb|_Fn|_Prc|         _Lin|             _Vi|    _Vio                   ;

   m_Tab[ET_VARINOUT   ][DIR_N]=               _Ci|_Co|                     _Lbl|_Jp|     _Ret|        _Vi|_Vo|_Vio|     _Ct|_Cl      ;
   m_Tab[ET_VARINOUT   ][DIR_S]=               _Ci|_Co|                     _Lbl|_Jp|     _Ret|        _Vi|_Vo|_Vio|     _Ct|_Cl      ;
   m_Tab[ET_VARINOUT   ][DIR_E]=                   _Co|        _Fb|_Fn|_Prc|     _Jp|_Lin|_Ret|    _Tr|    _Vo|_Vio                   ;
   m_Tab[ET_VARINOUT   ][DIR_W]=               _Ci|            _Fb|_Fn|_Prc|         _Lin|             _Vi|    _Vio                   ;

   m_Tab[ET_POWERRAIL  ][DIR_N]=                                                                                    _PwR              ;
   m_Tab[ET_POWERRAIL  ][DIR_S]=                                                                                    _PwR              ;
   m_Tab[ET_POWERRAIL  ][DIR_E]=                   _Co|        _Fb|_Fn|_Prc|     _Jp|     _Ret|    _Tr|                  _Ct|_Cl      ;
   m_Tab[ET_POWERRAIL  ][DIR_W]=               _Ci|            _Fb|_Fn|_Prc|                                             _Ct|_Cl      ;

   m_Tab[ET_CONTACT    ][DIR_N]=               _Ci|_Co|                     _Lbl|_Jp|_Lin|_Ret|        _Vi|_Vo|_Vio|     _Ct|_Cl      ;
   m_Tab[ET_CONTACT    ][DIR_S]=               _Ci|_Co|                     _Lbl|_Jp|_Lin|_Ret|        _Vi|_Vo|_Vio|     _Ct|_Cl      ;
   m_Tab[ET_CONTACT    ][DIR_E]=                   _Co|        _Fb|_Fn|_Prc|     _Jp|_Lin|_Ret|    _Tr|             _PwR|_Ct|_Cl      ;
   m_Tab[ET_CONTACT    ][DIR_W]=               _Ci|            _Fb|_Fn|_Prc|         _Lin|                          _PwR|_Ct|_Cl      ;

   m_Tab[ET_COIL       ][DIR_N]=               _Ci|_Co|                     _Lbl|_Jp|_Lin|_Ret|        _Vi|_Vo|_Vio|     _Ct|_Cl      ;
   m_Tab[ET_COIL       ][DIR_S]=               _Ci|_Co|                     _Lbl|_Jp|_Lin|_Ret|        _Vi|_Vo|_Vio|     _Ct|_Cl      ;
   m_Tab[ET_COIL       ][DIR_E]=                   _Co|        _Fb|_Fn|_Prc|     _Jp|_Lin|_Ret|    _Tr|             _PwR|_Ct|_Cl      ;
   m_Tab[ET_COIL       ][DIR_W]=               _Ci|            _Fb|_Fn|_Prc|         _Lin|                          _PwR|_Ct|_Cl      ;

#ifdef _DEBUG

   for (ELEM_TYPE i=ET_ACTION; i<ET_LAST; i=(ELEM_TYPE)((int)i+1))
   {
      for (ELEM_TYPE j=ET_ACTION; j<ET_LAST; j=(ELEM_TYPE)((int)j+1))
      {
         ASSERT( ((m_Tab[i][DIR_N] & LBit(j)) != 0) == ((m_Tab[j][DIR_S] & LBit(i)) != 0) );
         ASSERT( ((m_Tab[i][DIR_E] & LBit(j)) != 0) == ((m_Tab[j][DIR_W] & LBit(i)) != 0) );
      }
   }
#endif
}

/*func*------------------------------------------------------------------------
   performs the table based border-position test against one element
   in: pE1 pointer to the table index element
       pE2 pointer to the element used for masing
   out: true if position of E2 relative to E1 is allowed
-----------------------------------------------------------------------------*/
inline bool COvlTest::TabQ(CElem* pE1, TDirection Dir, CElem* pE2) 
{
   unsigned long lTab = m_Tab[ pE1->GetType() ][ Dir ];

   if (lTab & LBit(pE2->GetType()))
   {
      return true;
   }
   return (lTab & LBit(ET_LAST) && pE1->JoinTest(Dir, pE2) );  // do CElem::JoinTest(...) on <_Test> specified
}

// gives true if the position of E1 relative to E2 is allowed
// performas both geometric and table based tests
/*func*------------------------------------------------------------------------
   performs the table based and geometric position test of two elements
   in: pE1,pE2 pointers to the elements to be checked against each other
   out: true if position of E2 relative to E1 or reverse is allowed
-----------------------------------------------------------------------------*/
inline bool COvlTest::ValidPositionQ(CElem* pE1, CElem* pE2)
{
   long e1y1 = pE1->GetOrigin().y;
   long e1y2 = e1y1 + pE1->GetSize().cy;
   long e2y1 = pE2->GetOrigin().y;
   long e2y2 = e2y1 + pE2->GetSize().cy;

   long e1x1;
   long e1x2;
   long e2x1;
   long e2x2;

   if (!CheckSort(e1y1,e1y2,e2y1,e2y2))    // may have side effects
   {
      e1x1 = pE1->GetOrigin().x;
      e1x2 = e1x1 + pE1->GetSize().cx;
      e2x1 = pE2->GetOrigin().x;
      e2x2 = e2x1 + pE2->GetSize().cx;

      if (!CheckSort(e1x1,e1x2,e2x1,e2x2)) // may have side effects
      {
         // e1x1...e2y2 are normalized (e??1 <= e??2)
         if (e1y2 == e2y1)   // E1 is in the NORTH at E2
         {
            if (TabQ(pE1, DIR_S, pE2) || TabQ(pE2, DIR_N, pE1))    // look in table
               return true;
         } else 
         if (e1y1 == e2y2)   // E1 is in the SOUTH at E2
         {
            if (TabQ(pE1, DIR_N, pE2) || TabQ(pE2, DIR_S, pE1))
               return true;
         }

         if (e1x1 == e2x2)   // E1 is in the EAST at E2
         {
            if (TabQ(pE1, DIR_W, pE2) || TabQ(pE2, DIR_E, pE1))
               return true;
         } else
         if (e1x2 == e2x1)   // E1 is in the WEST at E2
         {
            if (TabQ(pE1, DIR_E, pE2) || TabQ(pE2, DIR_W, pE1))
               return true;
         }
      } else {
         return true;
      }
   } else {
      return true;
   }

   // intersection of lines is allowed if they don't overlap

   if(pE1->GetType() == ET_LINE && pE2->GetType() == ET_LINE)
   {
      if(pE1->ChkFlag(EF_RUBBLINE) || pE2->ChkFlag(EF_RUBBLINE) )
      {
         return true;
      } else 
      {
         if( (pE1->GetSize().cx==0) == (pE2->GetSize().cx==0))   // identical orientation
         {
            if(pE1->GetSize().cx==0)            // is vertical
            {
               return !((e1y1 > e2y1 && e1y1 < e2y2) || 
                        (e1y2 > e2y1 && e1y2 < e2y2) ||
                        (e2y1 > e1y1 && e2y1 < e1y2) ||
                        (e2y2 > e1y1 && e2y2 < e1y2)) && !(e1y1 == e2y1 && e1y2 == e2y2);
            }else
            {
               return !((e1x1 > e2x1 && e1x1 < e2x2) || 
                        (e1x2 > e2x1 && e1x2 < e2x2) ||
                        (e2x1 > e1x1 && e2x1 < e1x2) ||
                        (e2x2 > e1x1 && e2x2 < e1x2)) && !(e1x1 == e2x1 && e1x2 == e2x2);
            }
         }
         return true;
      }
   }else // handle Drop-In
   {
      if( pE1->GetType() == ET_LINE )
         return pE2->CheckDropIn(pE1);

      if( pE2->GetType() == ET_LINE )
         return pE1->CheckDropIn(pE2);
   }
   return false;
}

//============================================================================= CFixPoint
void CFixPoints::Serialize(CArchive& ar)
{
   if(ar.IsStoring())
   {
      CPointSet S;
      S.Add(Std);
      S.Add(Cur);
      int N = S.Size();
      ar << N;
      for(int i=0; i<S.Size(); i++)
      {
         ar << S[i];
      }
   }else
   {
      int N;
      ar >> N;
      Std.RemoveAll();
      Cur.RemoveAll();
      for( ;N>0; N--)
      {
         CPoint P;
         ar >> P;
         Std.Add(P);
      }
   }
}

inline bool CFixPoints::Includes(const CPoint& P) const
{
   return (Std.Find(P) >= 0) || (Cur.Find(P) >= 0);
}

/*func*------------------------------------------------------------------------
bool CFixPoints::Next(int& idx, CPoint& P)
  in :- iteration index (starts with 0)
  out:-next idx, (>=0:Std-index; <=-1:Cur-Index)
  ret: true if found false: not found (P, idx unchanged)
-----------------------------------------------------------------------------*/
bool CFixPoints::Next(int& idx, CPoint& P)
{
   if(idx>=0 && idx<Std.Size())
   {
      P = Std[idx++];
      if(idx==Std.Size())
      {
         idx = -1;
      }
      return true;
   }else 
   {
      if(Std.Size()==0 && idx==0 && Cur.Size()>0)
      {
         idx--;
      }
      if(idx<0 && -(idx+1) < Cur.Size())   // -1:Cur[0], -2:Cur[1], ...
      {
         P = Cur[-(idx+1)];
         idx--;
         return true;
      }
   }
   return false;
}


//============================================================================= CPinTable
CPinTable::CPinTable()
{
   InitHashTable(2041);
}

CPinTable::~CPinTable()
{
   DeleteAllPins();
}

/*func*------------------------------------------------------------------------
  delete all entrys in pin table
  in :- 
  out:-
-----------------------------------------------------------------------------*/
void CPinTable::DeleteAllPins()
{
   TPinEntry *pPE;
   POSITION   pos;
   CPoint     key;
   BOOL       bRemoved;

   pos = GetStartPosition();
   while (pos != NULL)   
   {
      GetNextAssoc (pos, key, pPE);
      if (pPE)
      {
         bRemoved = RemoveKey (key);
         ASSERT(bRemoved);
         delete pPE;
      }
   }
}

/*func*------------------------------------------------------------------------
  add the pins of one element to pin table
  in :pE - pointer to element
  out:-
-----------------------------------------------------------------------------*/
void CPinTable::InsertPins(CElem *pE)
{
   TPinEntry *pPE;
   CPin      *pPin;
   int        i;

   ASSERT(pE);
   for( i=0; i<pE->m_Con.GetSize(); i++ )
   {  // for all pins at this element
      pPin  = pE->m_Con.GetAt(i);
      if (pPin)
      {
         if (!Lookup (pPin->m_Pos, pPE))
         {
            pPE = new TPinEntry;
            if (pPE)
            {
               pPE->m_Pos    = pPin->m_Pos;
               pPE->m_NewPos = pPin->m_Pos;
               SetAt(pPin->m_Pos, pPE);
            }
         }
         ASSERT(pPE);
         if (pPE)
         {
            if (pE->ChkFlag(EF_RUBBLINE))
            {
               pPE->m_iRubbLines++;
            } 
            else if (pE->ChkFlag(EF_SELECT))
            {
               pPE->m_iSelected++;
            } 
            else 
            {
               pPE->m_iDeSelected++;
            }
            pPE->m_iConnects++;
         }
         pPE->m_Elems.Add(pE);
         // if not pPE - store NULL !?
         pPin->m_Entry = pPE;
      }
   }
}

/*func*------------------------------------------------------------------------
  remove the pins of one element from pin table
  in :pE - pointer to element
  out:-
-----------------------------------------------------------------------------*/
void CPinTable::DeletePins(CElem *pE)
{
   TPinEntry *pPE;
   CPin      *pPin;
   BOOL       bRemoved;
   int        i, j;

   ASSERT(pE);
   for( i=0; i<pE->m_Con.GetSize(); i++ )
   {  // for all pins at this element
      pPin  = pE->m_Con.GetAt(i);
      if( pPin && pPin->m_Entry)
      {
         pPE = pPin->m_Entry;

         if (pE->ChkFlag(EF_RUBBLINE))
         {
            pPE->m_iRubbLines--;
            ASSERT(pPE->m_iRubbLines >= 0);
         } 
         else if (pE->ChkFlag(EF_SELECT))
         {
            pPE->m_iSelected--;
            ASSERT(pPE->m_iSelected >= 0);
         } 
         else 
         {
            pPE->m_iDeSelected--;
            ASSERT(pPE->m_iDeSelected >= 0);
         }
         pPE->m_iConnects--;
         ASSERT(pPE->m_iConnects >= 0);

         for (j=0; j < pPE->m_Elems.GetSize(); j++)
         {
            if (pPE->m_Elems[j] == pE)
            {
               pPE->m_Elems.RemoveAt(j);
               break;
            }
         }

         if (pPE->m_iConnects == 0)
         {
            bRemoved = RemoveKey (pPin->m_Pos);
            ASSERT(bRemoved);
            delete pPE;
            pPin->m_Entry = NULL;
         }      
      }
   }
}

/*func*------------------------------------------------------------------------
  update attribute's of element in pin tabel
  in :pE - pointer to element
  out:-
-----------------------------------------------------------------------------*/
void CPinTable::SelectPins(CElem *pE, bool bSelect)
{
   TPinEntry *pPE;
   CPin      *pPin;
   int        i;

   ASSERT(pE);

   if (pE && !pE->ChkFlag(EF_RUBBLINE))
   {
      for( i=0; i<pE->m_Con.GetSize(); i++ )
      {  // for all pins at this element
         pPin  = pE->m_Con.GetAt(i);
         if (pPin)
         {
            pPE = pPin->m_Entry;
            ASSERT(pPE);
            if (pPE)
            {
               if (bSelect)
               {
                  pPE->m_iSelected++;
                  pPE->m_iDeSelected--;
               } 
               else 
               {
                  pPE->m_iSelected--;
                  pPE->m_iDeSelected++;
               }
               ASSERT(pPE->m_iRubbLines  >= 0);
               ASSERT(pPE->m_iSelected   >= 0);
               ASSERT(pPE->m_iDeSelected >= 0);
               ASSERT(pPE->m_iConnects   >= 0);
            }
         }
      }
   }
}

/*func*------------------------------------------------------------------------
  move one pin in pin table
  in :pE     - pointer to element
      pPE    - pointer to pin entry
      NewPos - the new pin position
  out:-
-----------------------------------------------------------------------------*/
void CPinTable::MovePin(CElem *pE, TPinEntry *&pPE, CPoint NewPos)
{
   ASSERT(pPE);

   if (pPE->m_Pos != NewPos)
   {
      DeletePin(pE, pPE, NewPos);
      pPE = InsertPin(pE, NewPos);
   } else {
      pPE->m_NewPos = NewPos;
   }
}

/*func*------------------------------------------------------------------------
  insert one pin in pin table
  in :pE     - pointer to element
      NewPos - new pin position
  out:pPE    - pointer to pin entry
-----------------------------------------------------------------------------*/
TPinEntry *CPinTable::InsertPin(CElem *pE, CPoint NewPos)
{
   TPinEntry *pPE;

   ASSERT(pE);
   if (!Lookup (NewPos, pPE))
   {
      pPE = new TPinEntry;
      if (pPE)
      {
         pPE->m_Pos    = NewPos;
         pPE->m_NewPos = NewPos;
         SetAt(NewPos, pPE);
      }
   }
   ASSERT(pPE);
   ASSERT(pPE->m_Pos == NewPos);
   if (pPE)
   {
      if (pE->ChkFlag(EF_RUBBLINE))
      {
         pPE->m_iRubbLines++;
      } 
      else if (pE->ChkFlag(EF_SELECT))
      {
         pPE->m_iSelected++;
      } 
      else 
      {
         pPE->m_iDeSelected++;
      }
      pPE->m_iConnects++;
      pPE->m_Elems.Add(pE);
   }
   return pPE;
}

/*func*------------------------------------------------------------------------
  delete one pin in pin table
  in :pE     - pointer to element
      pPE    - pointer to pin entry
  out:-
-----------------------------------------------------------------------------*/
void CPinTable::DeletePin(CElem *pE, TPinEntry *&pPE, CPoint NewPos)
{
   BOOL   bRemoved;
   int    j;

   ASSERT(pE);
   if (pE)
   {
      if (pE->ChkFlag(EF_RUBBLINE))
      {
         pPE->m_iRubbLines--;
         ASSERT(pPE->m_iRubbLines >= 0);
      } 
      else if (pE->ChkFlag(EF_SELECT))
      {
         pPE->m_iSelected--;
         ASSERT(pPE->m_iSelected >= 0);
      } 
      else 
      {
         pPE->m_iDeSelected--;
         ASSERT(pPE->m_iDeSelected >= 0);
      }
      pPE->m_iConnects--;
      ASSERT(pPE->m_iConnects >= 0);

      pPE->m_NewPos = NewPos;

      for (j=0; j < pPE->m_Elems.GetSize(); j++)
      {
         if (pPE->m_Elems[j] == pE)
         {
            pPE->m_Elems.RemoveAt(j);
            break;
         }
      }

      if (pPE->m_iConnects == 0)
      {
         bRemoved = RemoveKey (pPE->m_Pos);
         ASSERT(bRemoved);
         delete pPE;
         pPE = NULL;
      }
   }
}

void CPinTable::SaveConnectsToTemp()
{
   TPinEntry* pPE = NULL;
   POSITION pos = GetStartPosition();
   while(pos)
   {
      CPoint Point;
      GetNextAssoc(pos, Point, pPE);
      pPE->m_iTemp = pPE->m_iConnects;
   }
}

//============================================================================= CElemPtrList

CElemPtrList::CElemPtrList(int nBlockSize) :
   CPtrList(nBlockSize)
{
   bValidPos = false;
   m_pPinTable = new CPinTable;
   m_pRouter = NULL;
   m_bReRoute = false;
   m_bCountSignalChanges = false;
   m_IECLang = LANG_FBD;
}


CElemPtrList::~CElemPtrList()
{
   DeleteAllElems();
   if (m_pPinTable)
   {
      delete m_pPinTable;
      m_pPinTable = NULL;
   }
}

/*func*------------------------------------------------------------------------
  search element in list 
  in :p - coordinates of point in logical coords for search element
      iGrid - actual zoom
      bLines - flag accept lines
      bElems - flag accept elems
  out:NULL - no element found at this point
      pointer to element, which was found
-----------------------------------------------------------------------------*/
void CElemPtrList::AddAllElemsToPinTable()
{
   POSITION  pos;
   CElem    *pTE;

   ASSERT(m_pPinTable);
   if (m_pPinTable)
   {
      m_pPinTable->DeleteAllPins();

      for( pos = GetHeadPosition(); pos != NULL; )
      {
         pTE = (CElem*) GetNext( pos );
         ASSERT(pTE);
         if (pTE)
         {
            pTE->SetPinTable(m_pPinTable);
            m_pPinTable->InsertPins(pTE);
         }
      }
   }
}

/*func*------------------------------------------------------------------------
  search element in list 
  in :p    - coordinates of point in logical coords for search element
      pos  - startpos for search
  out:pos  - modified!
      NULL - no element found at this point
      pointer to element, which was found
-----------------------------------------------------------------------------*/
inline CElem* CElemPtrList::SearchPin(CPoint p, POSITION &pos)
{
   CPin  *pPin;
   CElem *pE; 
   int    i, iCount;

   for( ; pos != NULL; )
   {  
      pE = (CElem*) GetNext( pos );

      iCount = pE->m_Con.GetSize();
      for (i=0; i<iCount; i++ ) 
      {
         pPin = pE->m_Con[i];
         ASSERT(pPin);
         if(pPin->m_Pos == p) 
         {
            return pE; 
         }
      }
   }

   return NULL;
}

/*func*------------------------------------------------------------------------
  search element in list 
  in :p - coordinates of point in logical coords for search element
      iGrid - actual zoom
  out:NULL - no element found at this point
      pointer to element, which was found
-----------------------------------------------------------------------------*/
CElem *CElemPtrList::SearchElem(CPoint p, int iGrid)
{
   CElem   *pE;
   CRect    r;
   CSize    size;
   CPoint   po, pe;
   POSITION pos;
   int      i, iGrid2;

   // first round to search for resize fields ..
   if (iGrid > 1)
   {
      pos = GetHeadPosition();
      for( ; pos != NULL; )
      {  
         pE = (CElem*) GetNext( pos ); 
         ASSERT(pE);
         if ( pE && 
              pE->ChkFlag(EF_SELECT))
         {
            if (pE->IsResizeRect(p, iGrid))
            {
               return pE;
            }
         }
      }
   }
   // two search rounds .. 1. for bounds at element bodies 
   //                      2. for extended bound at element body
   for (i=0; i<=1; i++)
   {
      iGrid2 = i * (iGrid/2 + 1);

      for( pos = GetHeadPosition(); pos != NULL; )
      {  // get next ..
         pE = (CElem*) GetNext( pos );    // very time consuming
         // check elements ..
         if (pE)
         {
            pE->GetBodyRect(r, iGrid);
            r.InflateRect( pE->GetSize().cx>0 ?0 :iGrid2,  pE->GetSize().cy>0 ?0 :iGrid2);

            if( (p.x >= r.left  ) && 
                (p.x <= r.right ) &&
                (p.y >= r.top   ) && 
                (p.y <= r.bottom) )
            {
               if (pE->GetType() != ET_LINE)
               {
                  return pE;
               } else {
                  // check lines ..
                  if( (pE->GetSize().cx == 0) || (pE->GetSize().cy == 0) )
                  {
                     return pE;
                  } else {
                     if( pE->GetSize().cx > 0 )
                     {
                        po = pE->GetOrigin(); pe = pE->GetOrigin() + pE->GetSize();
                     } else {
                        po = pE->GetOrigin() + pE->GetSize(); pe = pE->GetOrigin();
                     }
                     float m = ((float)(po.y - pe.y)) / ((float)(po.x - pe.x));
                     int   y = (int)(m * (float)(p.x - r.left)) + po.y * iGrid;

                     if( (p.y > y-iGrid) && (p.y < y+iGrid) ) 
                     {
                        return pE;
                     }
                  }
               }
            }
            if( (pE->GetType() == ET_ACTION) || 
                (pE->GetType() == ET_ACTIONBLOCK) )
            {  // left edge ..
               if( (p.x >= (pE->GetOrigin().x                   ) * iGrid - iGrid2) && 
                   (p.x <= (pE->GetOrigin().x                   ) * iGrid + iGrid2) &&
                   (p.y >= (pE->GetOrigin().y                   ) * iGrid - iGrid2) && 
                   (p.y <= (pE->GetOrigin().y + pE->GetSize().cy) * iGrid + iGrid2) )
               {
                  return pE;
               }
               // right edge ..
               if( (p.x >= (pE->GetOrigin().x + pE->GetSize().cx) * iGrid - iGrid2) && 
                   (p.x <= (pE->GetOrigin().x + pE->GetSize().cx) * iGrid + iGrid2) &&
                   (p.y >= (pE->GetOrigin().y                   ) * iGrid - iGrid2) && 
                   (p.y <= (pE->GetOrigin().y + pE->GetSize().cy) * iGrid + iGrid2) )
               {
                  return pE;
               }
               // bottom edge ..
               if( (p.x >= (pE->GetOrigin().x                   ) * iGrid - iGrid2) && 
                   (p.x <= (pE->GetOrigin().x + pE->GetSize().cx) * iGrid + iGrid2) &&
                   (p.y >= (pE->GetOrigin().y + pE->GetSize().cy) * iGrid - iGrid2) && 
                   (p.y <= (pE->GetOrigin().y + pE->GetSize().cy) * iGrid + iGrid2) )
               {
                  return pE;
               }
            }
         }
      }
   }
   // last round for search in element rects
   for( pos = GetHeadPosition(); pos != NULL; )
   {  
      pE = (CElem*) GetNext( pos );  
      ASSERT(pE);
      if (pE)
      {
         pE->GetRect(r, iGrid);
//         r.InflateRect(1,1);
         r.InflateRect( pE->GetSize().cx>0 ?0 :1,  pE->GetSize().cy>0 ?0 :1);

         if( (p.x >= r.left  ) && 
             (p.x <= r.right ) &&
             (p.y >= r.top   ) && 
             (p.y <= r.bottom) &&
             (pE->GetType() != ET_ACTION) &&
             (pE->GetType() != ET_ACTIONBLOCK) &&
             (!pE->ChkFlag(EF_RUBBLINE)) )
         {
            return pE;
         }
      }
   }
   return NULL;
}

/*func*------------------------------------------------------------------------
  search line in list
  in :p  - coordinates of point 1 in grid coords for search 
      p2 - coordinates of point 2 in grid coords for search 
      bRubber:
         true  - search only rubberbands 
         false - search only fixed lines
  out:NULL - no element found at this point
      pointer to line, which was found
-----------------------------------------------------------------------------*/
CElemLine* CElemPtrList::SearchLine(CPoint p1, CPoint p2, bool bRubber)
{
   POSITION   pos;
   CElemLine *pE;
   CPoint     po, pe;

   for( pos = GetHeadPosition(); pos != NULL; )
   {
      pE = (CElemLine*) GetNext( pos );

      if(  pE && 
          (pE->GetType() == ET_LINE) &&
           pE->ChkFlag(EF_RUBBLINE) == (bRubber==true) )
      {  
         if (bRubber)
         {
            if (p1 == pE->GetOrigin())
            {
               if (p2 == pE->GetOrigin() + pE->GetSize()) return pE;
            } 
            if (p1 == pE->GetOrigin() + pE->GetSize())
            {
               if (p2 == pE->GetOrigin()) return pE;
            } 
         } 
         else 
         {
            if (pE->GetSize().cx >= 0) 
            {
               po.x = pE->GetOrigin().x; pe.x = pE->GetOrigin().x + pE->GetSize().cx;
            } else {
               pe.x = pE->GetOrigin().x; po.x = pE->GetOrigin().x + pE->GetSize().cx;
            }
            if( pE->GetSize().cy >=0 ) 
            {
               po.y = pE->GetOrigin().y; pe.y = pE->GetOrigin().y + pE->GetSize().cy;
            } else {
               pe.y = pE->GetOrigin().y; po.y = pE->GetOrigin().y + pE->GetSize().cy;
            }

            if (pE->GetSize().cy == 0) 
            {
               if (p1.y == pE->GetOrigin().y &&
                   p2.y == pE->GetOrigin().y &&
                   p1.x >= po.x && p1.x <= pe.x &&
                   p2.x >= po.x && p2.x <= pe.x)
               {
                  return pE;
               }
            }
            if (pE->GetSize().cx == 0) 
            {
               if (p1.x == pE->GetOrigin().x &&
                   p2.x == pE->GetOrigin().x &&
                   p1.y >= po.y && p1.y <= pe.y &&
                   p2.y >= po.y && p2.y <= pe.y)
               {
                  return pE;
               }
            }
         }
      }
   }
   return NULL;
}

/*func*------------------------------------------------------------------------
  unselect all elements in sheet
  in :- 
  out:-
-----------------------------------------------------------------------------*/
void CElemPtrList::UnselectAll()
{
   POSITION  pos;
   CElem    *pE;

   for( pos = GetHeadPosition(); pos != NULL; )
   {
      pE = (CElem*) GetNext( pos );
      ASSERT(pE);
      if (pE)
      {
         pE->ClrFlag(EF_SELECT);
      }
   }
}

/*func*------------------------------------------------------------------------
  gets the (first) selected element in raster coords
  in :-
  out:pE   - pointer to element 
      NULL - no selected element found
-----------------------------------------------------------------------------*/
CElem * CElemPtrList::GetSelectElem()
{
   POSITION  pos;
   CElem    *pE;

   for( pos = GetHeadPosition(); pos != NULL; )
   {
      pE = (CElem*) GetNext( pos );
      if( pE && pE->ChkFlag(EF_SELECT) )
      {
         return pE;
      }
   }

   return NULL;
}

/*func*------------------------------------------------------------------------
  gets the union rect of all selected elements in raster coords
  in :r     - rect to check
  out:pointer to rect for use as function
-----------------------------------------------------------------------------*/
void CElemPtrList::GetSelectRect(CRect& r)
{
   POSITION  pos;
   CElem    *pE;
   bool      bFirst=true;
   int       i;

   r.SetRectEmpty();
   for( pos = GetHeadPosition(); pos != NULL; )
   {
      pE = (CElem*) GetNext( pos );    // very time consuming
      ASSERT(pE);
      if (pE && pE->ChkFlag(EF_SELECT))
      {
         if( bFirst )
         {
            bFirst = false;
            r = CRect(pE->GetOrigin(), pE->GetSize());
            r.NormalizeRect();
         } else {

            i = min(pE->GetOrigin().x, pE->GetOrigin().x + pE->GetSize().cx);
            if( r.left > i) 
            {
               r.left = i;
            }

            i = min(pE->GetOrigin().y, pE->GetOrigin().y + pE->GetSize().cy);
            if( r.top > i) 
            {
               r.top = i;
            }

            i = max(pE->GetOrigin().x, pE->GetOrigin().x + pE->GetSize().cx);
            if( r.right < i) 
            {
               r.right = i;
            }

            i = max(pE->GetOrigin().y, pE->GetOrigin().y + pE->GetSize().cy);
            if( r.bottom < i) 
            {
               r.bottom = i;
            }
         }
      }
   }

   r.NormalizeRect();
}

/*func*------------------------------------------------------------------------
  check, if there are selected elements
  in :-
  out:true  - there are selected elements
      false - there are no selected elements
-----------------------------------------------------------------------------*/
bool CElemPtrList::IsOneSelected()
{
   POSITION  pos;
   CElem    *pE;

   for( pos = GetHeadPosition(); pos != NULL; )
   {
      pE = (CElem*) GetNext( pos );    // very time consuming
      ASSERT(pE);
      if (pE && pE->ChkFlag(EF_SELECT))
      {
         return true;
      }
   }

   return false;
}

/*func*------------------------------------------------------------------------
  gets the union rect of all elements in raster coords
  in :r     - rect to check
  out:pointer to rect for use as function
-----------------------------------------------------------------------------*/
void CElemPtrList::GetAllElemRect(CRect& r)
{
   POSITION  pos;
   CElem    *pE;
   BOOL      bFirst=TRUE;

   r.SetRectEmpty();
   for( pos = GetHeadPosition(); pos != NULL; )
   {
      pE = (CElem*) GetNext( pos );
      if( pE )
      {
         if( bFirst )
         {
            bFirst = FALSE;
            r = CRect(pE->GetOrigin(), pE->GetSize());
         } else {
            if( r.left > pE->GetOrigin().x) 
            {
               r.left = pE->GetOrigin().x;
            }
            if( r.top > pE->GetOrigin().y) 
            {
               r.top = pE->GetOrigin().y;
            }
            if( r.right < pE->GetOrigin().x + pE->GetSize().cx ) 
            {
               r.right = pE->GetOrigin().x + pE->GetSize().cx;
            }
            if( r.bottom < pE->GetOrigin().y + pE->GetSize().cy ) 
            {
               r.bottom = pE->GetOrigin().y + pE->GetSize().cy;
            }
         }
      }
   }

   r.NormalizeRect();
}

/*func*------------------------------------------------------------------------
  toggle at all elements in in given rect the flag 
  in :r - rect to select in grid coords
  out:-
-----------------------------------------------------------------------------*/
void CElemPtrList::ToggleFlagsByRect(TElemFlags flag, CRect &r)
{
   POSITION  pos;
   CElem    *pE;
   CPoint    ps, pe;

   r.NormalizeRect();
   r.InflateRect(1,1);   // snap range ..

   for( pos = GetHeadPosition(); pos != NULL; )
   {
      pE = (CElem*) GetNext( pos );
      ASSERT(pE);
      if( pE )
      { 
         if( pE->GetSize().cx < 0 )
         {
            ps.x = pE->GetOrigin().x + pE->GetSize().cx;
            pe.x = pE->GetOrigin().x;
         } else {
            ps.x = pE->GetOrigin().x;
            pe.x = pE->GetOrigin().x + pE->GetSize().cx;
         }
         if( pE->GetSize().cy < 0 )
         {
            ps.y = pE->GetOrigin().y + pE->GetSize().cy;
            pe.y = pE->GetOrigin().y;
         } else {
            ps.y = pE->GetOrigin().y;
            pe.y = pE->GetOrigin().y + pE->GetSize().cy;
         }
         if( (r.left   <  ps.x) &&
             (r.right  >= pe.x) &&
             (r.top    <  ps.y) &&
             (r.bottom >= pe.y) )
         {
            pE->TglFlag(flag);
         }
      }
   }
}

/*func*------------------------------------------------------------------------
  select all element inside the container IFF the container is selected
-----------------------------------------------------------------------------*/
void CElemPtrList::UpdateGroupSelect()
{
   for(POSITION pos = GetHeadPosition(); pos != NULL; )
   {
      CElem* pE = (CElem*)GetNext(pos);
      if(pE->ChkFlag(EF_CONTAIN))
      {
         if(pE->ChkFlag(EF_SELECT))
            ((CElemContain*)pE)->SetFlagGroup(EF_SELECT);
      }
   }
}

/*func*------------------------------------------------------------------------
  move all selected elements with relativ offset in p
  no check for conflicts with other elements - move over ..!
  resize rubberbands at moved elements!
  in :p        - relativ offset for move
  out:TRUE  - ok, moved
      FALSE - aborted
-----------------------------------------------------------------------------*/
bool CElemPtrList::MoveSelElems(CPoint& p)
{
   bool      bRes=true;
   POSITION  pos, pos2=NULL;
   CElem    *pE;

   // clear all update flags 
   ClearFlags(EF_UPDATE);

   // now process all/selected elements
   for( pos=GetHeadPosition(); bRes && ((pos2=pos) != NULL); )
   {
      pE = (CElem*) GetNext( pos );
      if(pE)
      {
         if((pE->ChkFlag(EF_SELECT) ||    // only selected elements
            pE->ChkFlag(EF_RUBBLINE)))    // or rubberband lines !
         {  
            bRes = bRes && pE->Resize(p.x, p.y); // move this element 
            pE->SetFlag(EF_UPDATE);
         }
         if(pE->ChkFlag(EF_EMPHASIZE))    // mark emphasize-changes to be redrawn
         {
            pE->ClrFlag(EF_EMPHASIZE);
            pE->SetFlag(EF_UPDATE);
         }
      }
   }
   
   IsOneOverlapped(true, EF_SELECT);

   if (!bRes) 
   {                                // roll back
      pos = pos2;
      if (pos != NULL)
      {
         GetPrev(pos);              //  ... from the previous of the breaked element
      } else {
         pos = GetTailPosition();   //  ... from tail 
      }

      for ( ; pos!=NULL; )
      {
         pE = (CElem*) GetPrev(pos);
         ASSERT(pE);
         if( pE && 
            (pE->ChkFlag(EF_SELECT) ||    // only selected elements
             pE->ChkFlag(EF_RUBBLINE)) )  // move no rubberband lines !
         {  
            bRes = pE->Resize(-p.x, -p.y);   // move this element back
            ASSERT(bRes);
         }
      }
      bRes = false;
   }
   return bRes;
}

/*func*------------------------------------------------------------------------
  check if two elements are layered
  in :bNoActCheck - true: ignore actions & action blocks (just set their flags)
      FMask - if set, only these Elements are checked against the others; if 
              not set, each element is echecked (i.e. check the selected only)
  out:TRUE  - is OVERLAPPED !!! 
      FALSE - ok, nothing overlap
-----------------------------------------------------------------------------*/
bool CElemPtrList::IsOneOverlapped(bool bNoActCheck, CElemFlag FMask)
{
   CElemContain *pCont;
   CElem        *pE, *pRubb=NULL;
   POSITION      pos, pos2, PosRubb = NULL;
   bool          bOcc = false;
   int           i, iMCount = 0;      // numer of masked elements
   CTypedPtrArray<CObArray,CElem*> m_Contains;

   for( pos = GetHeadPosition(); (pos2 = pos) != NULL; )
   {
      pE  = (CElem*) GetNext(pos);

      if (pE == pRubb)
      {
         break;     // now we get only rubberlines ...
      }
      
      if (pE->ChkFlag(EF_EMPHASIZE))
      {
         pE->ClrFlag(EF_EMPHASIZE);
         pE->ClrFlag(EF_UPDATE);
      }

      if (pE->GetFlag() & FMask)
      {
         iMCount++;
      }

      if (pE->ChkFlag(EF_RUBBLINE))
      {  // only move to tail of list - NO change of data, only new position
         RemoveAt(pos2);
         AddTail( pE );

         if (!pRubb)
         {
            pRubb   = pE;      // save first rubberline ..
            PosRubb = pos2;    //  .. and her pos
         }
      }

      if (pE->ChkFlag(EF_CONTAIN))
      {
         m_Contains.Add(pE);
         ((CElemContain*)pE)->EmptyGroup();
      }
   }

   // update container hierarchie for resize, move, insert, delete operations !
   for (pos = GetHeadPosition(); pos != NULL; )
   {
      pE  = (CElem*) GetNext( pos );
      ASSERT(pE);
      if (pE)
      {
         pE->SetContainer(NULL);
         if (!pE->ChkFlag(EF_CONTAIN))  // container in container is not allowed !
         {
            for (i=0; i<m_Contains.GetSize(); i++)
            {
               pCont = (CElemContain*) m_Contains[i];
               ASSERT(pCont);
               if ( pCont &&
                    pCont->IsMyElem(pE))
               {
                  pE->SetContainer(pCont);
                  pCont->AddGroupElem(pE);
                  break;
               }
            }
         }
      }
   }

   if (iMCount) // selective iteration only if elements are flagged
   {
      for( pos = GetHeadPosition(); pos != PosRubb; )
      {
         pE  = (CElem*) GetNext( pos );
         if( (pE->GetFlag() & FMask) && !pE->ChkFlag(EF_EMPHASIZE) )
         {
            if(pE->GetType() ==ET_LABEL)
            {
               bOcc |= IsElemOverlapped(pE, NULL, bNoActCheck, NULL);
            } else {
               bOcc |= IsElemOverlapped(pE, NULL, bNoActCheck, pRubb);
            }
         }

         if (pE->GetType() ==ET_LABEL) // check only labels against rubber lines
         {                                              // with at least one pin selected
            bOcc |= IsElemOverlapped(pE, PosRubb, bNoActCheck, NULL);
         }
      }
   } else
   {
      for( pos = GetHeadPosition(); pos != NULL; )
      {
         pE  = (CElem*) GetNext( pos );
         if(pE==pRubb)
            break;

         if(!pE->ChkFlag(EF_EMPHASIZE))
         {
            bOcc |= IsElemOverlapped(pE, pos, bNoActCheck, pRubb);
         }

         if(pE->GetType() ==ET_LABEL)  // check only labels against rubber lines
         {
            bOcc |= IsElemOverlapped(pE, PosRubb, bNoActCheck, NULL);
         }
      }
   }
   return bOcc;
}

/*func*------------------------------------------------------------------------
  searches for containers of this element
  (needed for "labels inside actions")
------------------------------------------------------------------------------*/
inline void CElemPtrList::UpdateContainers(CElem *pE)
{
   CElemContain *pCont;
   CElem        *pTE;
   POSITION      pos;
   int           i;
   CTypedPtrArray<CObArray,CElem*> m_Contains;

   for (pos = GetHeadPosition(); pos != NULL; )
   {
      pTE  = (CElem*) GetNext(pos);
      ASSERT(pTE);
      if (pTE->ChkFlag(EF_CONTAIN))
      {
         m_Contains.Add(pTE);       
         ((CElemContain*)pTE)->DelGroupElem(pE);
      }
   }
   
   // update container hierarchie for resize, move, insert, delete operations !
   ASSERT(pE);
   if (pE)
   {
      pE->SetContainer(NULL);
      if (!pE->ChkFlag(EF_CONTAIN))  // container in container is not allowed !
      {
         for (i=0; i<m_Contains.GetSize(); i++)
         {
            pCont = (CElemContain*) m_Contains[i];
            ASSERT(pCont);
            if ( pCont && pCont->IsMyElem(pE))
            {
               pE->SetContainer(pCont);
               pCont->AddGroupElem(pE);
               break;
            }
         }
      }
   }
}

/*func*-----------------------------------------------------------------------
  check if one element is overlapped with others
  bUpdateContainers is true by default (introduced on "labels inside actions")
  in   :pE - pointer to element for checking
  out  :false no element is overlapped
        true  at least one element is overlapped
  flags:EF_UPDATE      (set if EF_EMPHASIZE is changed )
        EF_EMPHASIZE   (set on overlapped elements only)
-----------------------------------------------------------------------------*/
bool CElemPtrList::IsElemOverlapped (CElem *pE, bool bUpdateContainers) // check for invalid position
{
   if (bUpdateContainers)
   {
      UpdateContainers(pE);
   }
   return IsElemOverlapped (pE, NULL, false, NULL);
}

/*func*------------------------------------------------------------------------
  check if one element is overlapped with others
  in   :pE1   - pointer to element for checking
        pos2  - starting position for pool iteration (default: 1st element)
        bNoActCheck - do not check against actions or a_blocks (used on resize)
  out  :false no element is overlapped
        true  at least one element is overlapped
  flags:EF_UPDATE      (set if EF_EMPHASIZE is changed )
        EF_EMPHASIZE   (set on overlapped elements only)
-----------------------------------------------------------------------------*/
inline bool CElemPtrList::IsElemOverlapped(CElem * pE1, POSITION posNext, bool bNoActCheck, CElem* pEnd)
{
   CElem* pE2;
   bool   bOK = true;

   if ((pE1->GetOrigin().x <= 0) ||
       (pE1->GetOrigin().y <= 0) ||
       (pE1->GetOrigin().x + pE1->GetSize().cx >= MAX_XGRID) ||
       (pE1->GetOrigin().y + pE1->GetSize().cy >= MAX_YGRID))
   {
      bOK = false;
      pE1->SetFlag(EF_EMPHASIZE);
      pE1->SetFlag(EF_UPDATE);
   }

   if (posNext == NULL)
   {
      posNext = GetHeadPosition();
   }
   for ( ; posNext != NULL; )
   {
      pE2 = (CElem*) GetNext( posNext );

      if(pE2->GetType() == ET_FB         ||
         pE2->GetType() == ET_FUNCTION   ||
         pE2->GetType() == ET_PROCEDURE  )
      {
         ((CElemFblk*)pE2)->OnAutoENO();
      }

      if (pE2 == pEnd)
      {
         break;
      }
      if (pE1 != pE2)
      {
         if (OvlTable.ValidPositionQ(pE1,pE2) || pE1->ChkFlag(EF_RUBBLINE))   // table test
         {                                               // was successful...
            if (pE1->GetType() == ET_LABEL)              // labels are more restricted:
            {
               if (!pE1->JoinTest(DIR_LAST,pE2))          // always use their individual test routine
               {
                  bOK = false;
                  pE1->SetFlag(EF_EMPHASIZE);
                  pE1->SetFlag(EF_UPDATE);
                  pE2->SetFlag(EF_EMPHASIZE);
                  pE2->SetFlag(EF_UPDATE);
               }
            } else 
            {
               if (pE2->GetType() == ET_LABEL)
               {
                  if (!pE2->JoinTest(DIR_LAST,pE1))
                  {
                     bOK = false;
                     pE1->SetFlag(EF_EMPHASIZE);
                     pE1->SetFlag(EF_UPDATE);
                     pE2->SetFlag(EF_EMPHASIZE);
                     pE2->SetFlag(EF_UPDATE);
                  }
               }
            }
         }else                                           // table test has failed
         {                                               // less restricted: actions & action blocks
            if (pE1->GetType() == ET_ACTION || pE1->GetType() == ET_ACTIONBLOCK)
            {
               if (!pE1->JoinTest(DIR_LAST,pE2))
               {
                  if (!bNoActCheck)
                  {
                     bOK = false;
                  }
                  pE1->SetFlag(EF_EMPHASIZE);
                  pE1->SetFlag(EF_UPDATE);
                  pE2->SetFlag(EF_EMPHASIZE);
                  pE2->SetFlag(EF_UPDATE);
               }
            } else
            {
               if(pE2->GetType() == ET_ACTION || pE2->GetType() == ET_ACTIONBLOCK)
               {
                  if(!pE2->JoinTest(DIR_LAST,pE1))
                  {
                     if(!bNoActCheck)
                        bOK = false;
                     pE1->SetFlag(EF_EMPHASIZE);
                     pE1->SetFlag(EF_UPDATE);
                     pE2->SetFlag(EF_EMPHASIZE);
                     pE2->SetFlag(EF_UPDATE);
                  }
               }else
               {
                  bOK = false;
                  pE1->SetFlag(EF_EMPHASIZE);
                  pE1->SetFlag(EF_UPDATE);
                  pE2->SetFlag(EF_EMPHASIZE);
                  pE2->SetFlag(EF_UPDATE);
               }
            }
         }
      }
   }
   return !bOK;
}

/*func*------------------------------------------------------------------------
  resize's all selected elements with delta from given start point to end point
  do nothing, if overlap an other element
  in :p     - relativ offset for resize in grid units
      eTyp  - resize corner
  out:TRUE  resized
      FALSE nothing done
-----------------------------------------------------------------------------*/
bool CElemPtrList::ResizeSelElems(CPoint& p, RS_TYPE eTyp)
{
   bool      bRes=true;
   POSITION  pos, pos2=NULL;
   CElem    *pE;

   // look for resize dir
   if( eTyp != RS_LAST ) 
   {
      // clear all update flags 
      ClearFlags(EF_UPDATE);
      // now process all/selected elements
      for (pos=GetHeadPosition(); bRes && ((pos2=pos) != NULL); )
      {
         pE = (CElem*) GetNext( pos );
         ASSERT(pE);
         if( pE && 
            (pE->ChkFlag(EF_SELECT) ||     // only selected elements
             pE->ChkFlag(EF_RUBBLINE)))    // or rubberband lines !
         {  
            if (!ResizeDo (pE, p.x, p.y, eTyp))
            {
               bRes = false;
            }
            pE->SetFlag(EF_UPDATE);
         }
      }

      // if an error appears during resizing, the error producing element was not resized
      if (!bRes || IsOneOverlapped(true, EF_SELECT)) 
      {                                // roll back
         for ( pos=GetHeadPosition(); (pos != NULL) && (pos != pos2); )
         {
            pE = (CElem*) GetNext(pos);
            ASSERT(pE);
            if( pE && 
               (pE->ChkFlag(EF_SELECT) ||     // only selected elements
                pE->ChkFlag(EF_RUBBLINE)))    // or rubberband lines !
            {  
               bRes = ResizeDo (pE, -p.x, -p.y, eTyp);
               ASSERT(bRes);
            }
         }
         bRes = false;
      }
   }
   return bRes;
}

/*func*------------------------------------------------------------------------
  resize the given element with given delta and resize typ,
  adjust the rubberband lines 
  in :pE    - pointer to the element
      dx    - relativ x-offset for resize in grid units
      dy    - relativ y-offset for resize in grid units
      eTyp  - resize corner
  out:TRUE  resized
      FALSE nothing done
-----------------------------------------------------------------------------*/
bool CElemPtrList::ResizeDo(CElem *pE, int dx, int dy, RS_TYPE eTyp)
{
   bool  bRes;

   switch ( eTyp ) {
      case RS_NW  : bRes = pE->Resize( dx,  dy, -dx, -dy ); break;
      case RS_NE  : bRes = pE->Resize(  0,  dy,  dx, -dy ); break;
      case RS_SE  : bRes = pE->Resize(  0,   0,  dx,  dy ); break;
      case RS_SW  : bRes = pE->Resize( dx,   0, -dx,  dy ); break;
      case RS_N   : bRes = pE->Resize(  0,  dy,   0, -dy ); break;
      case RS_E   : bRes = pE->Resize(  0,   0,  dx,   0 ); break;
      case RS_S   : bRes = pE->Resize(  0,   0,   0,  dy ); break;
      case RS_W   : bRes = pE->Resize( dx,   0, -dx,   0 ); break;
      default     : bRes = false; break;
   }

   return bRes;
}

/*func*------------------------------------------------------------------------
  replace one element to another element
  in :pEold - pointer to old element
      pEnew - pointer to new element
  out:-
-----------------------------------------------------------------------------*/
void CElemPtrList::ReplaceElem(CElem *pEold, CElem *pEnew)
{
   DeleteElem (pEold);
   InsertElem (pEnew);
   UpdateContainers(pEnew);
}

/*func*------------------------------------------------------------------------
  insert a new element
  in :pE - the element
  out:-
-----------------------------------------------------------------------------*/
void CElemPtrList::InsertElem(CElem *pE)
{
   ASSERT(pE);
   ASSERT(m_pPinTable);

   if (pE && m_pPinTable)
   {
      AddTail(pE);
      pE->SetPinTable(m_pPinTable);
      m_pPinTable->InsertPins(pE);      

      if(pE->GetType() == ET_FB        ||    // CElemFblk based?
         pE->GetType() == ET_PROCEDURE ||
         pE->GetType() == ET_FUNCTION    )
      {
         ((CElemFblk*)pE)->SetENODefault((m_IECLang == LANG_LD));
      }
      UpdateContainers(pE);
   }
}

/*func*------------------------------------------------------------------------
  serialize the element to/from stream
  in :ar - the archive
  out:-
-----------------------------------------------------------------------------*/
void CElemPtrList::Serialize(CArchive & ar)
{
   POSITION  pos;
   CElem    *pE;
   int       i;

   m_FixPoints.Serialize(ar);
	if (ar.IsStoring())
	{
      ar << (int)m_IECLang;
      ar << GetCount(); 
      for( pos = GetHeadPosition(); pos != NULL; )
      {
         pE = (CElem*) GetNext( pos );
         ar << pE;
      }
   } else {
      DeleteAllElems();
      ar >> i; m_IECLang = (IEC_GR_Lang)i;
      ar >> i;
      for( ; i>0; i-- )
      {
         ar >> pE;
         InsertElem( pE ); 
      }
   }
}

IMPLEMENT_SERIAL(CElemPtrList, CPtrList, SERIAL_VERSION)

/*func*------------------------------------------------------------------------
  filter clipboard - elements
  in :pE - read element
  out:true  - ok, add element to pool
      false - cancel 
-----------------------------------------------------------------------------*/
bool CElemPtrList::FCCBFilter(CElem **pE)
{
   CString str;

   if( *pE )
   {
      // change Const to VarRead
      if( (*pE)->GetType() == ET_CONST )
      {
         CElem *pE2;

         pE2 = new CElemVarIn( (CElemVar*)*pE );
         if( pE2 )
         {
            *pE = pE2;
            // optimize size ..
            str = ((CElemVar*)(*pE))->GetName();
            (*pE)->Resize(0, 0, 2 + CText::GetIdentLen(str) - (*pE)->GetSize().cx, 0);
         }
          
      }
      // layout FC/FB/PRO
      ;
/*
      // set instance in FB 
      if( (*pE)->GetType() == ET_FB )
      {
         str.LoadString(IDS_GR_NAMEMASK_FB);
         ((CElemFB*)(*pE))->m_strInst.Format(str, GRU_GetNextID(ID_INST));
      }
*/
      // ready
      return true;
   } 
   else 
   {
      return false;
   }
}

/*func*------------------------------------------------------------------------
  register all class in list
  in :-
  out:-
-----------------------------------------------------------------------------*/
void CElemPtrList::RegisterXMLTypes(CXMLArchive &ar)
{
   ar.AddToClassMap(RUNTIME_CLASS(CElemActBlk  ), XML_4GR_ELEM_ACTBLCK      );
   ar.AddToClassMap(RUNTIME_CLASS(CElemAction  ), XML_4GR_ELEM_ACTION       );
   ar.AddToClassMap(RUNTIME_CLASS(CElemAltDiv  ), XML_4GR_ELEM_ALTDIV       );
   ar.AddToClassMap(RUNTIME_CLASS(CElemCmt     ), XML_4GR_ELEM_COMMENT      );
   ar.AddToClassMap(RUNTIME_CLASS(CElemConIn   ), XML_4GR_ELEM_CONTINUATION );
   ar.AddToClassMap(RUNTIME_CLASS(CElemConOut  ), XML_4GR_ELEM_CONNECTOR    );
   ar.AddToClassMap(RUNTIME_CLASS(CElemFB      ), XML_4GR_ELEM_FB           );
   ar.AddToClassMap(RUNTIME_CLASS(CElemFunc    ), XML_4GR_ELEM_FNC          );
   ar.AddToClassMap(RUNTIME_CLASS(CElemProc    ), XML_4GR_ELEM_PRO          );
   ar.AddToClassMap(RUNTIME_CLASS(CElemJump    ), XML_4GR_ELEM_JUMP         );
   ar.AddToClassMap(RUNTIME_CLASS(CElemLabel   ), XML_4GR_ELEM_LABEL        );
   ar.AddToClassMap(RUNTIME_CLASS(CElemRet     ), XML_4GR_ELEM_RETURN       );
   ar.AddToClassMap(RUNTIME_CLASS(CElemSimDiv  ), XML_4GR_ELEM_SIMDIV       );
   ar.AddToClassMap(RUNTIME_CLASS(CElemStep    ), XML_4GR_ELEM_STEP         );
   ar.AddToClassMap(RUNTIME_CLASS(CElemTran    ), XML_4GR_ELEM_TRANS        );
   ar.AddToClassMap(RUNTIME_CLASS(CElemConst   ), XML_FCCB_ELEM_CONST       );
   ar.AddToClassMap(RUNTIME_CLASS(CElemVarIn   ), XML_4GR_ELEM_VARREAD      );
   ar.AddToClassMap(RUNTIME_CLASS(CElemVarOut  ), XML_4GR_ELEM_VARWRITE     );
   ar.AddToClassMap(RUNTIME_CLASS(CElemVarInOut), XML_4GR_ELEM_VARFEEDBACK  );
   ar.AddToClassMap(RUNTIME_CLASS(CElemLine    ), XML_4GR_ELEM_LINE         );
   ar.AddToClassMap(RUNTIME_CLASS(CPin         ), XML_4GR_ELEM_PIN          );
   ar.AddToClassMap(RUNTIME_CLASS(CElemPower   ), XML_4GR_ELEM_POWERRAIL    );
   ar.AddToClassMap(RUNTIME_CLASS(CElemContact ), XML_4GR_ELEM_CONTACT      );
   ar.AddToClassMap(RUNTIME_CLASS(CElemCoil    ), XML_4GR_ELEM_COIL         );
}

/*func*------------------------------------------------------------------------
  serialize the element to/from XML stream
  in :ar - the archive
  out:-
-----------------------------------------------------------------------------*/
void CElemPtrList::SerializeXMLSelected(CXMLArchive &ar, CString source)
{
   CString   str;
   POSITION  pos;

   RegisterXMLTypes(ar);

   if(ar.IsStoring())    
   {        
      try {
         // set doctype once in document!!
         ar.SetDocType( XML_FCCB_DTDTYPE );
         ar.SetDTD( XML_FCCB_DTDFILE );

         ar.MoveDown( XML_FCCB_CBHEADER );
         ar.SetAttribute( XML_FCCB_ATTR_CBSOURCE, source);
         ar.MoveUp();

         ar.MoveDown( XML_4GR_ELEM_BODY );
         // walk thru list of childs and serialize them
         
         for( pos = GetHeadPosition(); pos != NULL; )
         {
            CElem *pObj = (CElem *)GetNext(pos);
            ASSERT(pObj != NULL);
            if( pObj->ChkFlag(EF_CONTAIN) && pObj->ChkFlag(EF_SELECT))
               ((CElemContain*)pObj)->SetFlagGroup(EF_SELECT);
         }

         for( pos = GetHeadPosition(); pos != NULL; )
         {
            CElem *pObj = (CElem *)GetNext(pos);
            ASSERT(pObj != NULL);
            ASSERT_KINDOF(CXMLObject, pObj);

            if( pObj->ChkFlag(EF_SELECT) && !pObj->ChkFlag(EF_AUTOELEM) ) 
            {
               ar << pObj;
            }
         }
         ar.MoveUp();
      }
      catch (HRESULT hr)        
      {
         TRACE(_T("Cannot save to XML; hr=0x%08X\n"), hr);
         throw hr;   // fire upstairs...        
      }    
   } else {
      try {
         ar.MoveDown( XML_FCCB_CBHEADER );     
         ar.GetAttribute( XML_FCCB_ATTR_CBSOURCE, str);
         ar.MoveUp();
         //
         ar.MoveDown( XML_4GR_ELEM_BODY );
         // slurp in all child element nodes and create objects as necessary!!!
         NodeIter  iter(ar);
         for( pos = iter.begin(); pos != NULL; )
         {
            iter.next(pos);
            CXMLObject *pObj = ar.ReadObject();
            ASSERT(pObj != NULL);
            ASSERT((CElem*)pObj->IsKindOf(RUNTIME_CLASS(CElem)));
            ((CElem*)pObj)->SetFlag(EF_SELECT);

            if( FCCBFilter( (CElem**)(&pObj) ) )
            {
               InsertElem((CElem*)pObj);
               ELEM_TYPE T = ((CElem*)pObj)->GetType();

               if(T==ET_FB || T==ET_FUNCTION || T==ET_PROCEDURE)
               {
                  CElemFblk *pFB = (CElemFblk*)pObj;
                  for(int i=0; i<pFB->m_Con.GetSize(); i++)
                  {
                     pFB->m_Con[i]->m_Flag.Set(PF_AUTOVAR);
                  }
               }
            }
         }
         ar.MoveUp();
      }
      catch (HRESULT hr)
      {
         TRACE(_T("Cannot load from XML; hr=0x%08X\n"), hr);
         throw hr;   // fire upstairs...
      }
   }
}

/*func*------------------------------------------------------------------------
  serialize the selected elements to/from stream
  in :ar - the archive
      p  - offset for elements
  out:-
-----------------------------------------------------------------------------*/
void CElemPtrList::SerializeSelected(CArchive & ar)
{
   POSITION  pos;
   CElem    *pE;
   int       i;

	if (ar.IsStoring())
	{
      for( i=0,pos = GetHeadPosition(); pos != NULL; )
      {
         pE = (CElem*) GetNext( pos );
         ASSERT_VALID(pE);
         if( pE && pE->ChkFlag(EF_SELECT) )
         {
            i++;
         }
      }
      ar << i; 
      for( pos = GetHeadPosition(); pos != NULL; )
      {
         pE = (CElem*) GetNext( pos );
         ASSERT_VALID(pE);
         if( pE && pE->ChkFlag(EF_SELECT) )
         {
            ar << pE;
         }
      }
	} else {
      // no delete of existing elements !!!
      ar >> i;
      for( ; i>0; i-- )
      {
         ar >> pE;
         ASSERT_VALID(pE);
         if( pE )
         { 
            if( pE->ChkFlag(EF_SELECT) )
            {
               InsertElem( pE ); 
            } else {
               delete pE;
            }
         }
      }
   }
}

/*func*------------------------------------------------------------------------
  remove all selected elements from list and delete them
  in :-
  out:-
-----------------------------------------------------------------------------*/
void CElemPtrList::DeleteFlaggedElems(CElemFlag flag)
{
   POSITION  pos;
   CElem    *pE;

   for( pos = GetHeadPosition(); pos != NULL; )
   {
      pE = (CElem*) GetNext( pos );
      ASSERT_VALID(pE);
      if( pE && pE->GetFlag() == flag )
      {
         bool bRestart = pE->ChkFlag(EF_CONTAIN) && (pos!=NULL);

         // performance optimization
         if(bRestart)  // pre-check if pos becomes invalid
         {
            CElem* pNxt    = NULL;
            POSITION pos2  = pos;
            pNxt = (CElem*)GetNext(pos2);
            ASSERT_VALID(pNxt);
            if(!pNxt->GetContainer())  // *pos is not contained?
               bRestart = false;       // no restart required
         }
         
         DeleteElem(pE);

         if(bRestart)
            pos = GetHeadPosition();   // (the 2nd element may have been removed!)
      }
   }
}

/*func*------------------------------------------------------------------------
  remove all elements from list and delete the elements
  in :-
  out:-
-----------------------------------------------------------------------------*/
void CElemPtrList::DeleteAllElems()
{
   CElem    *pE;

   while ( !IsEmpty() )
   {
      pE = (CElem*) GetHead();
      ASSERT_VALID(pE);
      if( pE )
      {
         DeleteElem (pE);
      }
   }
}

/*func*------------------------------------------------------------------------
  remove element from list (doesn't free the element)
  in :- *pE  - Element to be removed
  out:-
-----------------------------------------------------------------------------*/
void CElemPtrList::RemoveElem(CElem *pE)
{
   ASSERT(pE);
   ASSERT(m_pPinTable);

   if (pE && m_pPinTable)
   {
      m_pPinTable->DeletePins(pE);      

      POSITION pos = Find (pE);
      ASSERT(pos);
      if (pos)
      {
         RemoveAt (pos);
      }
   }
}

/*func*------------------------------------------------------------------------
  remove element from list (doesn't free the element)
  in :- pos  - elements POSITION for removal
  out:-
-----------------------------------------------------------------------------*/
void CElemPtrList::RemoveElemAt(POSITION pos)
{
   ASSERT(m_pPinTable);
   ASSERT(pos);
   
   if(pos)
   {
      CElem* pE = (CElem*) GetAt(pos);
      if (pE && m_pPinTable)
      {
         m_pPinTable->DeletePins(pE);
         RemoveAt (pos);
      }
   }
}

/*func*------------------------------------------------------------------------
  remove element from list and delete this element
  in :- *pE  - Element to be deleted
        bDel - destruction flag (true: element is destructed == default)
  out:- pE is set to NULL
-----------------------------------------------------------------------------*/
void CElemPtrList::DeleteElem(CElem *pE)
{
   ASSERT(pE);
   ASSERT(m_pPinTable);

   if(pE->ChkFlag(EF_CONTAIN))
   {
      while(((CElemContain*)pE)->GroupSize()>0)
      {
         DeleteElem( ((CElemContain*)pE)->GetGroupElem(0));
      }
      ((CElemContain*)pE)->EmptyGroup();
   }
   RemoveElem(pE);
   delete pE;
}

/*func*------------------------------------------------------------------------
  search next/prev element (up or down) from point p 
  in :p - start point for search in raster coord
      bNext:
         TRUE  - search forward
         FALSE - search backward
  out:pointer to found element
-----------------------------------------------------------------------------*/
CElem * CElemPtrList::GetNextElem(CPoint& p, BOOL bNext)
{
   CElem    *pTE=NULL, *pE;
   POSITION  pos;
   int       i, i2, iCur;

   iCur = p.x + p.y * MAX_XGRID;
   if( bNext )
   {
      i2 = MAX_XGRID * MAX_YGRID;
   } else {
      i2 = 0;
   }
   for( pos = GetHeadPosition(); pos != NULL; )
   {
      pE = (CElem*) GetNext( pos );
      ASSERT_VALID(pE);
      if( pE )
      {
         i = pE->GetOrigin().x + pE->GetOrigin().y * MAX_XGRID;
         if( bNext )
         {                             // forward, down
            if( iCur < i && i <= i2 )
            {
               i2 = i; pTE = pE;
            }
         } else {                      // backward, up
            if( i2 <= i && i < iCur )
            {
               i2 = i; pTE = pE;
            }
         }
      }
   }
   if( pTE )
   {
      p = pTE->GetOrigin();
   }
   return pTE;
}

/*func*------------------------------------------------------------------------
  generate clipboard data in XML format
  in :file - CShared (memory) file as destination for XML
  out:-
-----------------------------------------------------------------------------*/
void CElemPtrList::GenXMLData(CString &strXML, const CString &pathName)
{
   CMemFile file;
   BYTE    *lpBuffer;

   CXMLArchive ar(&file, CArchive::store | CArchive::bNoFlushOnDelete);
   SerializeXMLSelected( ar, pathName); // Serialize selected objects 
   ar.Flush();

   DWORD dwLen = file.GetPosition();   // we must fill the rest with space for UTF-8 !
   file.Write( "\0\0", 2);       // append 0 for IsBadStringPtr !!
   file.Write( "        ", 8);   // CMemFile will round pos to modulo 8 

   lpBuffer =  file.Detach();
   if (lpBuffer)
   {
      strXML = (LPCTSTR) lpBuffer;
      free(lpBuffer);
   }
   strXML.GetBufferSetLength (dwLen);
   strXML.FreeExtra();
}

/*func*------------------------------------------------------------------------
  search current element at point p - ignore lines !!!
  in :p - point for search in raster coord
  out:pointer to found element
      NULL - nothing found
-----------------------------------------------------------------------------*/
CElem * CElemPtrList::GetCurElem(CPoint p)
{
   CElem     *pE;
   POSITION   pos;

   for( pos = GetHeadPosition(); pos != NULL; )
   {
      pE = (CElem*) GetNext( pos );
      ASSERT_VALID(pE);
      if( pE && (pE->GetOrigin() == p) && 
         (pE->GetType() != ET_LINE) &&
         (pE->GetType() != ET_ALTDIV) &&   // like lines : sfc-elements 
         (pE->GetType() != ET_SIMDIV))     //   can have the same pos !!!
      {
         return pE;
      }
   }
   return NULL;
}



/*func*------------------------------------------------------------------------
  search for the elem at the next logical position relative to pStart
  in : pStart - search origin (not returned)
       EF     - single flag for element selection
  ret: CElem* - pointer to found element or NULL in none found
-----------------------------------------------------------------------------*/
CElem* CElemPtrList::FindNextTempVar(CElem* pStart)
{
   CElem *pENext  = NULL;
   CElem *pENxt2  = NULL;
   CElem *pParent = NULL;
   CRect  R;

   ASSERT(pStart);

   if (pStart)
   {
      CPoint Point = pStart->m_Con[0]->m_Pos;
      int    iPos  = PointToIntSwap(Point);

      if( pStart->GetType()==ET_VARIN || pStart->GetType()==ET_VAROUT )
      {
         TPinEntry* pPE = NULL;

         if(m_pPinTable->Lookup(Point,pPE))        // search for the connected FB if any
         {
            for (int i=0; i< pPE->m_Elems.GetSize(); i++)
            {
               if ( (pPE->m_Elems[i] != pStart) &&
                   ((pPE->m_Elems[i]->GetType() == ET_FB       ) ||
                    (pPE->m_Elems[i]->GetType() == ET_FUNCTION ) ||
                    (pPE->m_Elems[i]->GetType() == ET_PROCEDURE)))
               {
                  pParent = pPE->m_Elems[i];
                  break;
               }
            }

            if (pParent != NULL)                   // connected element found?
            {
               int min_idx, max_idx;
               int min_pos = INT_MAX;
               int max_pos = INT_MAX;
                                                   // search for the next temp var at this FB
               for (int idx=0; idx<pParent->m_Con.GetSize(); idx++)
               {
                  int iTmp = PointToIntSwap( pParent->m_Con[idx]->m_Pos);
                  // search the next temp var in order after last temp var
                  if (iTmp > iPos && iTmp < max_pos)
                  {
                     const TPinEntry *pPE1 = pParent->m_Con[idx]->GetPinEntry();
   
                     for (int j=0; j < pPE1->m_Elems.GetSize(); j++)  // search for auto vars 
                     {
                        CElem* pE1 = pPE1->m_Elems[j];

                        if ( pE1->ChkFlag(EF_AUTOELEM) )
                        {
                           max_pos = iTmp;
                           max_idx = idx;
                           pENext = pE1;
                        }
                     }
                  }
                  // search the first temp var at elem
                  if (iTmp < min_pos)
                  {
                     const TPinEntry *pPE1 = pParent->m_Con[idx]->GetPinEntry();
   
                     for (int j=0; j < pPE1->m_Elems.GetSize(); j++)  // search for auto vars 
                     {
                        CElem* pE1 = pPE1->m_Elems[j];

                        if ( pE1->ChkFlag(EF_AUTOELEM) )
                        {
                           min_pos = iTmp;
                           min_idx = idx;
                           pENxt2 = pE1;
                        }
                     }
                  }
               }
               if ((pENext==NULL) && (pENxt2!=NULL))
               {
                  pENext = pENxt2;
               }
            }
         }
      }

      if (pENext==NULL)  // was ready with parent 
      {
         POSITION pos  = GetHeadPosition();
         int      iCur = 0;   // wrap around search
         iPos = INT_MAX;
         
         while (pos)
         {
            CElem* pE = (CElem*) GetNext(pos);
            
            if( pE->ChkFlag(EF_AUTOELEM ) )
            {
               int iElemPos = PointToIntSwap(pE->GetOrigin());  // search order: x first
               iElemPos = min(iElemPos, PointToIntSwap(pE->GetOrigin()+pE->GetSize()));
               
               if(iCur < iElemPos && iElemPos < iPos)
               {
                  pENext = pE;
                  iPos = iElemPos;
               }
            }
         }
      }            
   }
   return pENext;
}



/*func*------------------------------------------------------------------------
  move elements under mask for m_ulAttrFlag to tail of list (to front on view)
  in :
  out:CElem * - ptr to first moved element or NULL
-----------------------------------------------------------------------------*/
CElem *CElemPtrList::MoveFlaggedToTail(CElemFlag flag)
{
   CElem     *pTE=NULL;
   CElem     *pE;
   POSITION   pos, pos2;

   for( pos = GetTailPosition(); (pos2 = pos) != NULL; )
   {
      pE = (CElem*) GetPrev( pos );
      if( pE && (pE->GetFlag() == flag) )
      {
         if (pTE == NULL)
         {
            pTE = pE;
         }
         RemoveAt(pos2);
         AddTail( pE );   // only change position !!
      }
   }
   return pTE;
}



/*func*------------------------------------------------------------------------
  determines the boundings for Insertion, returns Y-min/max of bounding labels
  in : insertion opint, flag wether to watch labels
  out: CRect (working area)
-----------------------------------------------------------------------------*/
CRect CElemPtrList::GetContextRect(CPoint pAT, bool bLabelClip)
{
   CRect Box;
   CRect RElem;

   Box.left    = 0;
   Box.top     = 0;
   Box.bottom  = MAX_YGRID;
   Box.right   = MAX_XGRID;

   CPoint LabelBounds(0,MAX_YGRID);  // x:lower bound (ymin); y:upper bound (ymax)

   POSITION pos;
   CElem *pE;
   
   // search: bounding actions, bounding labels
   for( pos = GetHeadPosition(); pos != NULL; )
   {
      if( (pE = (CElem*) GetNext( pos )) != NULL )
      {
         switch (pE->GetType() )
         {
         case ET_LABEL:                            // adjust y-bounds (label bounds)
            if(bLabelClip)
            {
               CElemContain* pC = pE->GetContainer(); 
               if(pC != NULL)                      // contained label
               {
                  ASSERT(pC->ChkFlag(EF_CONTAIN));

                  pC->GetRect(RElem,1);
                  if( chkPointAtRect(pAT, RElem))
                  {
                     CRect RClient;
                     pC->GetClientRect(RClient);

                     if( Box.bottom > RClient.bottom  ) 
                        Box.bottom = RClient.bottom;
                     
                     if( Box.top < RClient.top ) 
                        Box.top = RClient.top;
                  }else
                  {
                     break;   // skip contained labels if pAT is outside the container
                  }
               }

               int yt = pE->GetOrigin().y;
               int yb = yt + pE->GetSize().cy;
               if(chkPointAtRect(pE->GetOrigin(),Box))
               {
                  if( (pAT.y < yt) && (Box.bottom > yt) )
                     Box.bottom = yt;
               
                  if( (pAT.y > yt) && (Box.top < yb) )
                     Box.top = yb;
               }
            }
            break;

         case ET_ACTION:
         case ET_ACTIONBLOCK:
            {
               ASSERT(pE->ChkFlag(EF_CONTAIN));

               pE->GetRect(RElem,1);
               if( chkPointAtRect(pAT, RElem))
               {
                  CRect RClient;
                  ((CElemContain*)pE)->GetClientRect(RClient);
                  Box.left = RClient.left;
                  Box.right = RClient.right;

                  if( Box.bottom > RClient.bottom  ) 
                     Box.bottom = RClient.bottom;
                  
                  if( Box.top < RClient.top ) 
                     Box.top = RClient.top;
               }
            }
         }
      }
   }
#ifdef _DEBUG
   _RPT4(_CRT_WARN,"\nx(%i..%i) y(%i..%i) ", Box.left, Box.right, Box.top, Box.bottom);
   if(bLabelClip)
      _RPT0(_CRT_WARN," clip ");
#endif
   return Box;
}

/*func*------------------------------------------------------------------------
  check to insert/delete of rows/cols in fbd
  in :p   - point with delta for insert/delete of rows/cols in grid coords 
      pAt - start point for operation
  out:TRUE  - operation possible
      FALSE - operation not possible
-----------------------------------------------------------------------------*/
bool CElemPtrList::CheckSpace(CPoint p, CPoint pAT, bool bLabelClip)
{
   CRect Area;    // the operations working area
   POSITION  pos;
   CElem    *pE;

   CPoint    PMax;
   CPoint    PMin;

   if( (p.x <= -MAX_XGRID) || (p.x >= MAX_XGRID) ) return false;
   if( (p.y <= -MAX_YGRID) || (p.y >= MAX_YGRID) ) return false;

   if( (pAT.x < 0) || (pAT.x >= MAX_XGRID) ) return false;
   if( (pAT.y < 0) || (pAT.y >= MAX_YGRID) ) return false;

   if( p.x && p.y)   // cannot change more than one dimension at a time
      return false;

#ifdef _DEBUG
   if( p.x<0 || p.y<0 )
      p = p;
#endif

   if(p.y!=0)        // get operation area
   {
      bLabelClip = false;
      Area = GetContextRect(pAT, bLabelClip);
      ASSERT(!(pAT.y & 1));    // even!
      Area.top = max(0,pAT.y-1);
   }else
   {
      Area = GetContextRect(pAT, bLabelClip);
      ASSERT(!(pAT.x & 1));    // even!
      Area.left = max(0,pAT.x-1);
   }

   PMin = Area.BottomRight();
   PMax = Area.TopLeft();

   for( pos = GetHeadPosition(); pos != NULL; )
   {
      if( (pE = (CElem*) GetNext( pos )) != NULL )
      {
         if(pE->ChkFlag(EF_CONTAIN))                        // action|action block found
         {
            CRect RElem;
            pE->GetRect(RElem,1);
            
            if( chkPointInRect(pE->GetOrigin(),Area) ||
               chkPointInRect(pE->GetOrigin()+pE->GetSize(),Area) ||
               chkPointAtRect(pAT,RElem)  )
            {
               if(!pE->IsSpaceable(pAT, p, bLabelClip) )    // ask the container
                  return false;
            }
         }else
         {
            if(!pE->GetContainer())    // contained elements are moved by the container itself
            {
               if(p.x && pE->GetType() == ET_LABEL)
                  continue;
               
               if(!chkPointInRect(pE->GetOrigin()+pE->GetSize(),Area) &&
                  !chkPointInRect(pE->GetOrigin()              ,Area))
                  continue;
               
               if(!pE->IsSpaceable(pAT, p, bLabelClip) )     // ask the element
                  return false;
            }
         }
         
         bool bCheckArea = true;
         
         // SIS: bug fix moving elements out of maximum area
         // do not check area if element is line and movement is in negative direction
         // in this case lines will be shortened.
         if((pE->GetType() == ET_LINE     ||
            pE->GetType() == ET_ALTDIV   ||
            pE->GetType() == ET_SIMDIV   ||
            pE->GetType() == ET_POWERRAIL) &&
            (p.x < 0 || p.y < 0))
         {
            bCheckArea = false;
         }
         
         // if area is to be checked and top left or bottom right point lies in area
         // consider this point for maximum and minimum
         if(bCheckArea && (chkPointInRect(pE->GetOrigin(),Area) || chkPointInRect(pE->GetOrigin()+pE->GetSize(),Area)))
         {
            PMin.x = min(PMin.x, min(pE->GetOrigin().x, pE->GetOrigin().x+pE->GetSize().cx));
            PMin.y = min(PMin.y, min(pE->GetOrigin().y, pE->GetOrigin().y+pE->GetSize().cy));
            PMax.x = max(PMax.x, max(pE->GetOrigin().x, pE->GetOrigin().x+pE->GetSize().cx));
            PMax.y = max(PMax.y, max(pE->GetOrigin().y, pE->GetOrigin().y+pE->GetSize().cy));
         }
      }
   }
   bool ok = true;
   
   if(p.x < 0)
      ok &= (PMin.x + p.x >= Area.left);   
   else if(p.x > 0)
      ok &= (PMax.x + p.x < Area.right);
   
   if(p.y < 0)
      ok &= (PMin.y + p.y >= Area.top);
   else if(p.y > 0)
      ok &= (PMax.y + p.y < Area.bottom);
   
#ifdef _DEBUG
   if(!ok)
      ok = ok;
#endif
         return ok;
}//CElemPtrList::CheckSpace


/*func*------------------------------------------------------------------------
 insert/delete p columns/rows at point pAT 
 (CheckSpace should be run at first!)
 * PinStatus must be correctly set
 in :p   - number of spaces in grid coords (one drection at the same time x|y)
 pAT - insert/delete position in grid coords
 out:TRUE  inserted
 FALSE nothing done
 -----------------------------------------------------------------------------*/
bool CElemPtrList::InsertSpace(CPoint p, CPoint pAT, bool bLabelClip)
{
   bool      bUpdCID = false; // flag - signals need for Pin-ID to be updated
   POSITION  pos, pos2;
   CPosArray aPin;
   
   // clear all update flags (for rubberbands)
   ClearFlags(EF_UPDATE );
   ClearFlags(EF_CURRENT);
   
   MoveFlaggedToTail(EF_SELECT);
   MoveFlaggedToTail(EF_RUBBLINE);     // required for moving rubber lines
   
   bLabelClip &= (p.y==0);             // no label clipping on y-operations
   m_pPinTable->SaveConnectsToTemp();  // save old connection counter

   if(p.x<0||p.y<0)  // use the old routine for del-operation
   {
      return InsertSpace_OLD(p,pAT, bLabelClip);
   }else
   {
      CRect Area = GetContextRect(pAT, bLabelClip);

      for( pos=GetHeadPosition(); ((pos2=pos) != NULL); )
      {
         CElem* pE = (CElem*) GetNext(pos);
         if(pE)
         {
            // skip elements outside the bounding labels (optional)
            if(!pE->ChkFlag(EF_CONTAIN))
            {
               if(pE->GetContainer())  // contained elements are moved by the container itself
                  continue;
             
               if(!chkPointAtRect(pE->GetOrigin(),Area) )
                  continue;

               if(p.x && pE->GetType() == ET_LABEL)
                  continue;
            }else
            {
               CRect RClient;
               ((CElemContain*)pE)->GetClientRect(RClient);

               if(!chkPointAtRect(pAT,RClient) && !chkPointInRect(pE->GetOrigin(),Area))
                  continue;
            }

            pE->Spacing(pAT,p,&aPin,bLabelClip);     // move the elements, store deconnected pins to aPin
         }
      }
         
      // re-connect pins
      for(int iPin=0; iPin < aPin.GetSize(); iPin++)
      {
         CPoint P1 = aPin[iPin];
         CElemLine* pLine = new 	CElemLine(P1, P1+p, false);
         ASSERT(P1 != (P1+p));
         InsertElem(pLine);
         bUpdCID |= true;
      }
         
      if(bUpdCID) // anything to be updated
      {
         // adjust endpoints of inserted Lines
         POSITION pos = GetHeadPosition();
         if(pos)
         {
            GetNext(pos);
            do
            {
               CElem* pE = (CElem*)GetNext(pos);
               if(pE->ChkFlag(EF_CURRENT))
               {
                  CPoint Point1 = pE->GetOrigin();
                  CPoint Point2 = pE->GetOrigin() + pE->GetSize();
                  pE->ClrFlag(EF_CURRENT);
                  
                  pos2 = GetHeadPosition();//NULL
                  CElem* pE1 = SearchPin(Point1,pos2);
                  CElem* pE2;
                  
                  if(pE1)
                  {
                     pos2 = GetHeadPosition();//NULL
                     pE2  = SearchPin(Point2,pos2);
                     
                     if( pE1!=pE && pE2!=pE && pE2!=pE1 )
                     {
                        LinkLines(Point1);
                        LinkLines(Point2);
                        pos = GetHeadPosition();
                     }
                     else
                     {
                        DeleteElem (pE);
                        pos = GetHeadPosition();
                     }
                  }
               }
            }while(pos);
         }
      }
   }
   return bUpdCID;
}



bool CElemPtrList::InsertSpace_OLD(CPoint p, CPoint pAT, bool bLabelClip)
{
   bool      bRes=true;
   POSITION  pos, pos2=NULL;
   CRect    Area;

   // clear all update flags (for rubberbands)
   ClearFlags(EF_UPDATE);

   if(p.y!=0)                                               // get operation area
   {
      bLabelClip = false;
      Area = GetContextRect(pAT, bLabelClip);
      Area.top = pAT.y;
   }else
   {
      Area = GetContextRect(pAT, bLabelClip);
   }

   for( pos=GetHeadPosition(); bRes && ((pos2=pos) != NULL); )
   {
      CElem * pE;
      pE = (CElem*)GetNext(pos);
      // skip elements outside the bounding labels (optional)

      if(pE->ChkFlag(EF_CONTAIN))
      {
         CRect RClient;
         ((CElemContain*)pE)->GetClientRect(RClient);
         
         if(!chkPointAtRect(pAT,RClient) && !chkPointInRect(pE->GetOrigin(),Area))
            continue;
      }else
      {
         if(pE->GetContainer())
            continue;

         CPoint P1 = pE->GetOrigin();
         CPoint P2 = P1 + pE->GetSize();

         if( !chkPointInRect(P1,Area) && !chkPointInRect(P2,Area)   )
            continue;

         if(p.x && pE->GetType() == ET_LABEL)
            continue;
      }
      bRes = InsertSpaceDo_OLD(pE, p.x, p.y, pAT, bLabelClip);
   }

   // if an error appears during spacing, the error producing element was not spaced
   if( !bRes ) 
   {                                   // roll back
      pos = pos2;
      GetPrev(pos);                    //  ... from the previous of the breaked element
      for( ; pos!=NULL; )
      {
         bRes = InsertSpaceDo_OLD((CElem*)GetPrev(pos), -p.x, -p.y, pAT, bLabelClip);
         ASSERT(bRes);
      }

      bRes = false;
   }
   return bRes;
}
/*func*------------------------------------------------------------------------
  space the given element p columns/rows at point pAT 
  in :pE    - pointer to the element
      dx    - relativ x-offset for spacing in grid units
      dy    - relativ y-offset for spacing in grid units
      pAT - insert/delete position in grid coords
  out:TRUE  moved
      FALSE nothing done
-----------------------------------------------------------------------------*/
bool CElemPtrList::InsertSpaceDo_OLD(CElem *pE, int dx, int dy, CPoint pAT, bool bLabelClip)
{
   bool      bRes=true;
   int       i1, i2;

   ASSERT(pE);
   if( pE )
   {
      if( dx != 0 )
      {  
         if( pE->GetSize().cx > 0 ) { 
            i1 = pE->GetOrigin().x; 
            i2 = pE->GetOrigin().x + pE->GetSize().cx;
         } else {
            i1 = pE->GetOrigin().x + pE->GetSize().cx;
            i2 = pE->GetOrigin().x; 
         }
         if( (pAT.x >= i1) && (pAT.x < i2) )
         {
            if( pE->GetSize().cx != 0 )
               bRes &= pE->Spacing(pAT,CPoint(dx,dy),NULL,bLabelClip);
         } else if( pAT.x < i1 )
         {
            bRes &= pE->Spacing(pAT,CPoint(dx,dy),NULL,bLabelClip);
         } 
      }
      if( dy != 0 )
      {  
         if( pE->GetSize().cy > 0 ) { 
            i1 = pE->GetOrigin().y; 
            i2 = pE->GetOrigin().y + pE->GetSize().cy;
         } else {
            i1 = pE->GetOrigin().y + pE->GetSize().cy;
            i2 = pE->GetOrigin().y; 
         }
         if( (pAT.y >= i1) && (pAT.y < i2) )
         {
            if(!pE->ChkFlag(EF_CONTAIN))
            {
               if((pE->GetType() != ET_LINE)&&(pE->GetType() != ET_POWERRAIL)) // skip non-lines with pAT inside
                  return true;
            }
            if( pE->GetSize().cy != 0 )
               bRes &= pE->Spacing(pAT,CPoint(dx,dy),NULL,bLabelClip);
         } else if( pAT.y < i1 )
         {
            bRes &= pE->Spacing(pAT,CPoint(dx,dy),NULL, bLabelClip);
         } 
      }
   }

   return bRes;
}//CElemPtrList::InsertSpaceDo_OLD


/*func*------------------------------------------------------------------------
  remove unselected lines connected to <P>
  - terminates at stop points (if any)
  - terminates at points that are connected by more than two lines
  - uses EF as flag for line exclusion (i.e. EF_SELECT: don't remove selected lines at <P>)
  out: returns the end point at P
  ret: number of connects at P (>0: call again with the same origin)
-----------------------------------------------------------------------------*/
int CElemPtrList::WireToRubbLine(CPoint& P, TElemFlags EF, bool bComment)
{
   CPoint PNext;
   TPinEntry *pPE = NULL;
   
   int iCount = 0; 
   CElem* pDel = NULL;
   bool bReady = true;

   ASSERT(EF!=EF_SELECT);
   if( m_pPinTable->Lookup(P,pPE)            &&
      (pPE->m_iConnects > 1)                 && 
      !m_FixPoints.Includes(P)               &&
      (pPE->m_iSelected != pPE->m_iConnects)    )
   {
      for (int i=0; i<pPE->m_Elems.GetSize(); i++)  // for each elem using this pin
      {
         CElem* pE = pPE->m_Elems[i];
         if ( pE->GetType() == ET_LINE &&
             !pE->ChkFlag(EF_SELECT)    &&
             !pE->ChkFlag(EF)           &&
             bComment == pE->ChkFlag(EF_COMMENT)
            )
         {
            if (iCount == 0)
            {
               if (pE->m_Con[0]->m_Pos != P)
               {
                  PNext  = pE->m_Con[0]->m_Pos;
                  bReady = pE->m_Con[0]->GetConnects() != 2;
               } else
               {
                  PNext  = pE->m_Con[1]->m_Pos;
                  bReady = pE->m_Con[1]->GetConnects() != 2;
               }
               pDel = pE;
            }
            iCount++;
         }
      }
      
      if (pDel)
      {
         ASSERT(!pDel->ChkFlag(EF));
         pDel->SetFlag(EF);
         
         if (!bReady)
            WireToRubbLine(PNext,EF,bComment); // ignore further return values
         
         P = PNext;              // set endpoint result
         DeleteElem(pDel);       // call AFTER recursion!!!
         iCount--;
      }
      
   }
   return iCount;
}

/*func*------------------------------------------------------------------------
  completes the point list
  - overwrites <PSet>
  clear EF_CURRENT before each call
  call  EF_UPDATE  before the 1st call
-----------------------------------------------------------------------------*/
void CElemPtrList::GetSignalPoints (CPoint P, CPointSet& PSet, bool bComment)
{
   TPinEntry * pPE;  // dummy
   PSet.RemoveAll();
   if(m_pPinTable->Lookup(P,pPE))   // possible that two non-lines are connected
   {
      PSet.Add(P);
   }

   if(FlagSignal(P, EF_CURRENT, bComment, false))
   {
      CElem* pE = MoveFlaggedToTail(EF_CURRENT);
      POSITION pos = GetTailPosition();
      while(pos)
      {
         CElem* pTE = (CElem*) GetPrev(pos);

         if(pTE->GetType() == ET_LINE)
         {
            PSet.Add(pTE->GetOrigin()                );
            PSet.Add(pTE->GetOrigin()+pTE->GetSize() );
         }
         if(pTE==pE)
            break;
      }
   }
}


/*func*------------------------------------------------------------------------
  copy all EF_AUTOELEMs connected to pE into the pL
-----------------------------------------------------------------------------*/
int CElemPtrList::GetAutoElems(CElem* pE, CElemPtrList* pL)
{
   ASSERT_VALID(pE);
   ASSERT_VALID(pL);
   int n=0;
   const TPinEntry *pPE;
   for (int i=0; i < pE->m_Con.GetSize(); i++)
   {
      pPE = pE->m_Con[i]->GetPinEntry();
      ASSERT(pPE);
      for (int j=0; j < pPE->m_Elems.GetSize(); j++)
      {
         if (pPE->m_Elems[j]->ChkFlag(EF_AUTOELEM))
         {
            n++;
            pL->AddHead(pPE->m_Elems[j]);
         }
      }
   }
   return n;
}


/*func*------------------------------------------------------------------------
  prepares ReRoute operation (used before moving elements)
  - DeConnect operation + Unroute at the border line between selected
    and unselected elements
-----------------------------------------------------------------------------*/
void CElemPtrList::ReRouteDeconnect(bool bInsRubber, bool bReRoute)
{
   TPinEntry *pPE;
   POSITION   pos;
   CPoint     PinPos;
   CElem     *pE;
   int        i;

   ASSERT(m_pPinTable);
   m_bReRoute = false;
   // process deconnection
   if (m_pPinTable)
   {
      {  // delete all auto elements at selected elements ..
         CElemPtrList L;
         for (pos = GetHeadPosition(); pos; )
         {
            CElem* pE = (CElem*)GetNext(pos);
            if(pE->ChkFlag(EF_SELECT) && !pE->ChkFlag(EF_AUTOELEM))
            {
               GetAutoElems(pE, &L);
            }
         }
         
         for (pos = L.GetHeadPosition(); pos; )
         {
            CElem* pE = (CElem*)L.GetNext(pos);
            DeleteElem(pE);
         }
         L.RemoveAll();
      }

      ClearFlags(EF_PROCESSED);                       // used by FlagSignal(...)
      ClearFlags(EF_CURRENT);                         // flag set for UnrouteFlaggedLines(...)
      ClearFlags(EF_TEMPELEM);                        // flag set for results
      ClearFlags(EF_UPDATE);                          // used locally by WireToRubbLine()

      pos = m_pPinTable->GetStartPosition();
      while (pos != NULL)                             // for each pin of the pin table
      {
         m_pPinTable->GetNextAssoc (pos, PinPos, pPE);
         ASSERT(pPE);
         if ( pPE &&                                  // pin with both selected && unselected elements?
              bInsRubber &&
             (pPE->m_iDeSelected > 0) &&
             (pPE->m_iSelected   > 0) )
         {  
            //---- reroute extension {
            int iULines = 0;  // count of unselected lines 
            int iUElems = 0;  // (count [abort counting if >0] of) unselected elements (not lines)

            if (pPE->m_iDeSelected > 0)
            {
               for (int i=0; (iUElems==0) && (i<pPE->m_Elems.GetSize()); i++)  // for each elem@pin
               {
                  CElem* pE = pPE->m_Elems[i];
                  if (!pE->ChkFlag(EF_SELECT))         // elem is NOT selected
                  {
                     if ( pE->GetType() == ET_LINE )
                     {
                        if (!pE->ChkFlag(EF_RUBBLINE)) // fixed line?
                           iULines++;
                     } else
                     {
                        iUElems++;                    // unselected <non-line>
                     }
                  }
               }
            }

            if (iUElems>0) // insert rubber lines, unselected lines are not converted
            {
               bool bComm=true;
               for (i=0; bComm && i<pPE->m_Elems.GetSize(); i++)
               {
                  if (!pPE->m_Elems[i]->ChkFlag(EF_COMMENT))
                     bComm = false; 
               }
               // insert rubberband - reconnect 
               pE = new CElemLine(pPE->m_Pos, pPE->m_Pos, TRUE); // Size(0,0) is required
               ASSERT(pE);
               if (pE) 
               {
                  InsertElem(pE);
                  pE->SetFlag(EF_TEMPELEM);  // mark as to be re-routed
                  if (bComm)
                     pE->SetFlag(EF_COMMENT);
                  m_bReRoute = true;         // ReRoute must be performed - used inside ReRouteDone(...)
                  pE->m_Con.GetAt(1)->m_Flag.Set(PF_SELECT);
               }
            }      // re-route preparation: convert unselected lines into rubber lines
            else 
            {
               CPoint PStart  = pPE->m_Pos;
               CPoint PEnd    = PStart;
               BOOL bRepeat   = true;
               BOOL bRestart  = false;

               if (pPE->m_iConnects > 1)
               {
                  if (bReRoute) // reroute fixed lines ..
                  {
                     for (i=0; bRepeat && (i<10); i++)
                     {
                        // non-comment rubber line
                        bRepeat = WireToRubbLine(PEnd,EF_UPDATE,false) > 0;
                        bool bFixPoint = m_FixPoints.Includes(PStart);
                        if (PEnd != PStart || bFixPoint) // lines removed?
                        {
                           TPinEntry *pPE1=NULL;
                           if(m_pPinTable->Lookup(PEnd,pPE1))
                           {
                              pE = new CElemLine(PStart, PEnd, TRUE);
                              pE->SetFlag(EF_UPDATE);
                              pE->SetFlag(EF_TEMPELEM);
                              InsertElem(pE);
                           }
                           bRestart = !bFixPoint;
                           PEnd = PStart;
                        }
                     }

                     if (bRestart)
                     {
                        pos = m_pPinTable->GetStartPosition();
                        continue;
                     }

                     bRepeat = true;
                     for(i=0; bRepeat && (i<10); i++)
                     {
                        // comment rubber line
                        bRepeat = WireToRubbLine(PEnd,EF_UPDATE,true) > 1;
                        if(PEnd != PStart) // lines removed?
                        {
                           TPinEntry *pPE1=NULL;
                           if(m_pPinTable->Lookup(PEnd,pPE1))
                           {
                              pE = new CElemLine(PStart, PEnd, TRUE);
                              pE->SetFlag(EF_UPDATE);
                              pE->SetFlag(EF_COMMENT);
                              pE->SetFlag(EF_TEMPELEM);
                              InsertElem(pE);
                           }
                           bRestart = true;
                           PEnd = PStart;
                        }
                     }
                  } else {
                     bool bComm=true;

                     for (i=0; bComm && i<pPE->m_Elems.GetSize(); i++)
                     {
                        if (!pPE->m_Elems[i]->ChkFlag(EF_COMMENT))
                           bComm = false; 
                     }

                     pE = new CElemLine(pPE->m_Pos, pPE->m_Pos, TRUE);
                     ASSERT(pE);
                     if (pE) 
                     {
                        InsertElem(pE);
                        pE->SetFlag(EF_TEMPELEM);
                        if (bComm)
                           pE->SetFlag(EF_COMMENT);
                        pE->m_Con.GetAt(1)->m_Flag.Set(PF_SELECT);
                        m_bReRoute = true;         // ReRoute must be performed - used inside ReRouteDone(...)
                     }
                  }
               }

               if(bRestart)
               {
                  pos = m_pPinTable->GetStartPosition();
               }
            }
            //---- reroute extension }
         }
         
         if ( pPE && (pPE->m_iDeSelected == 2) )
         {  
            if(LinkLines(pPE->m_Pos, false))             // link lost line ends
                pos = m_pPinTable->GetStartPosition();
         }
      }
   }
}

/*func*------------------------------------------------------------------------
   re-route the converted rubber lines (called after moving elements)
-----------------------------------------------------------------------------*/
bool CElemPtrList::ReRouteDone()
{
   bool bUpdate = false;
   bool bSignalRouter = true;
   bool bCleanOpenLines = true;

   if(bSignalRouter)
   {
      CPointArray RPoints; // end point set of rubberlines to be routed
      RPoints.RemoveAll();

      if(bCleanOpenLines)
      {
         // collect artefact candidates for later clean up
         POSITION pos = GetHeadPosition();
         while(pos)
         {
            CElem* pE = (CElem*)GetNext(pos);
            if(pE->ChkFlag(EF_RUBBLINE) && pE->ChkFlag(EF_TEMPELEM))
            {
               TPinEntry* pPE;
               CPoint P = pE->GetOrigin();
               // add artefact dandidate points to the PointArray
               if( m_pPinTable->Lookup(P,pPE) && (pPE->m_iConnects ==2) )
               {
                  if((pPE->m_Elems[0]->GetType() == ET_LINE) && pPE->m_Elems[0]!=pE ||
                     (pPE->m_Elems[1]->GetType() == ET_LINE) && pPE->m_Elems[1]!=pE)
                  {
                     RPoints.AddUnique(P);
                  }
               }
               P = P + pE->GetSize();
               if( m_pPinTable->Lookup(P,pPE) && (pPE->m_iConnects ==2) )
               {
                  if((pPE->m_Elems[0]->GetType() == ET_LINE) && pPE->m_Elems[0]!=pE ||
                     (pPE->m_Elems[1]->GetType() == ET_LINE) && pPE->m_Elems[1]!=pE)
                  {
                     RPoints.AddUnique(P);
                  }
               }
            }
         }
      }

      bUpdate = RouteToSignalInit(EF_TEMPELEM,false);	// non-comment
      if(bUpdate)
      {
         int i=0;
         while(RouteFlaggedLines(EF_TEMPELEM))
         {
            if(++i == 10)
            {
               Sleep(1);
               i=0;
            }
         }
         RouteToSignalDone(EF_TEMPELEM);
      }
      
      if(RouteToSignalInit(EF_TEMPELEM,true))			// comment
      {
         int i = 0;
         bUpdate = true;
         while(RouteFlaggedLines(EF_TEMPELEM))
         {
            if(++i == 10)
            {
               Sleep(1);
               i=0;
            }
         }
         RouteToSignalDone(EF_TEMPELEM);
      }
      CreateTempVars(false);
      ClearConflictingTempVars();

      if(bUpdate)
      {
         // clean up artefacts (if any)
         if(bCleanOpenLines)
         for(int idx=0; idx<RPoints.GetSize(); idx++)
         {
            TPinEntry* pPE;
            if( m_pPinTable->Lookup(RPoints[idx],pPE) && (pPE->m_iConnects ==1) )
            {
               if(!pPE->m_Elems[0]->ChkFlag(EF_SELECT))
                  if(pPE->m_Elems[0]->GetType() == ET_LINE)
                     DeleteElem(pPE->m_Elems[0]);
            }
         }

         ClearFlags(EF_TEMPELEM);
      }
   }else
   {
      if(m_bReRoute)
      {
         m_bReRoute = false;
         
         //use point-to-point router
         POSITION pos = GetHeadPosition();
         while(pos)
         {
            CElem* pE = (CElem*)GetNext(pos);
            if(pE->ChkFlag(EF_TEMPELEM) && pE->ChkFlag(EF_RUBBLINE))
            {
               CPoint P1 = pE->GetOrigin();
               CPoint P2 = P1+pE->GetSize();
               bUpdate = true;
               DeleteElem(pE);
               if(RouteToPointInit(P1))
               {
                  RouteToPoint(P2);
                  RouteToPointDone();
               }
            }
         }
      }
   }
   return bUpdate;
}

/*func*------------------------------------------------------------------------
  deconnect the pins of all selected elements from the pins of unselected 
  elements
  in :bInsertRubber: 
         true  - insert rubberband at connection points with size (0,0) to keep
                 the connection
         false - no insertion of lines, connections will be clipped
  out:-
-----------------------------------------------------------------------------*/
void CElemPtrList::DeconnectSelElems(bool bInsertRubber)
{
   TPinEntry *pPE;
   POSITION   pos;
   CPoint     key;
   CElem     *pE;

   ASSERT(m_pPinTable);

   // process deconnection
   if (m_pPinTable)
   {
      pos = m_pPinTable->GetStartPosition();
      while (pos != NULL)   
      {
         m_pPinTable->GetNextAssoc (pos, key, pPE);
         ASSERT(pPE);
         if ( pPE &&
              bInsertRubber &&
             (pPE->m_iDeSelected > 0) &&
             (pPE->m_iSelected   > 0) )
         {  // insert rubberband - reconnect 
            pE = new CElemLine(pPE->m_Pos, pPE->m_Pos, TRUE);
            ASSERT(pE);
            if( pE ) 
            {
               InsertElem(pE);
               pE->m_Con.GetAt(1)->m_Flag.Set(PF_SELECT);
            }
         }
         if ( pPE &&
//             (((pPE->m_iDeSelected == 2) && (pPE->m_iSelected   == 0)) ||
//              ((pPE->m_iDeSelected == 0) && (pPE->m_iSelected   == 2)) ))
               (pPE->m_iDeSelected == 2) )
         {  // link lost line ends
            if(LinkLines(pPE->m_Pos, false))
                pos = m_pPinTable->GetStartPosition();
         }
      }
   }
}

/*func*------------------------------------------------------------------------
 * DropOut - operation (auto-insertion of lines to close lines)
 * - call AFTER the routine "DeconnectSelElems(FALSE)"
 *---------------------------------------------------------------------------*/
bool CElemPtrList::DropOutSelElems()
{
   POSITION   pos;      // iterator
   TPinEntry *pPE;
   CElem     *pTE = NULL;   // changed if DropOut was performed
   CElem     *pE;
   CElemLine *pL;
   CPoint     key;
   bool bChanged = false;

   CArray<CElem*,CElem*> TempElems;
   ClearFlags(EF_TEMPELEM);

   for( pos = GetTailPosition(); pos != NULL; )             // for each element in list
   {
      pE = (CElem*) GetPrev( pos );
      // close drop out ..
      if((pE != NULL) && 
          pE->ChkFlag(EF_SELECT) &&                         // selected...
         !(pE->GetType() == ET_LINE)  )                    // non-lines
      {
         int n = pE->m_Con.GetSize();
         for(int i = 0; i<n; i++)                           // for each pin of elem[pos]
         {
            pL = NULL;
            CPin* pin1 = pE->m_Con[i];
            CPoint P1  = pin1->m_Pos;

            for(int j=i+1; !pL && j<n; j++)                 // against each other pin of elem[pos]
            {
               CPin* pin2 = pE->m_Con[j];
               CPoint P2  = pin2->m_Pos;
               bool   bFblk = pE->GetType()==ET_FB || 
                              pE->GetType()==ET_FUNCTION || 
                              pE->GetType()==ET_PROCEDURE;

               if ((((P1.x == P2.x) && (P1.x != pE->GetOrigin().x) && (P1.x != pE->GetOrigin().x + pE->GetSize().cx)) || 
                    ((P1.y == P2.y) && (P1.y != pE->GetOrigin().y) && (P1.y != pE->GetOrigin().y + pE->GetSize().cy))) &&       // pins are opposite?
                   !pin1->m_Flag.Chk(PF_AUTOVAR) &&
                   !pin2->m_Flag.Chk(PF_AUTOVAR) &&
                   ((bFblk && pin1->m_Flag.Chk(PF_LADDER) && pin2->m_Flag.Chk(PF_LADDER)) || !bFblk)
                  )
               {
                  bool bInsert;
                  m_pPinTable->Lookup(P1,pPE);
                  bInsert  = (pPE && (pPE->m_iConnects>1) && (pPE->m_iConnects != pPE->m_iSelected));

                  m_pPinTable->Lookup(P2,pPE);
                  bInsert &= (pPE && (pPE->m_iConnects>1) && (pPE->m_iConnects != pPE->m_iSelected));

                  if(bInsert && pE->CheckDropOut())
                  {
                     bChanged = true;
                     RemoveElem(pE);                        // drop *pE from list temorary
                     TempElems.Add(pE);
                     if(P1!=P2)
                     {
                        pTE = new CElemLine(P1,P2,false);
                        pTE->SetFlag(EF_UPDATE);
                        pTE->SetFlag(EF_TEMPELEM);

                        InsertElem(pTE);
                        LinkLines(P1,false);
                        LinkLines(P2,false);
                     }else
                     {
                        LinkLines(P1);
                     }

                     pos = GetTailPosition();

                     j = n; i = n;  // leave for-loop's
                  }
               }
            }
         }
      }
   }

   ASSERT(m_pPinTable);
   // process connection
   if (m_pPinTable)
   {
      pos = m_pPinTable->GetStartPosition();
      while (pos != NULL)   
      {
         m_pPinTable->GetNextAssoc (pos, key, pPE);
         ASSERT(pPE);
         if (pPE)
         {  
            // LinkLines can remove elements ! pos can be invalid
            if (LinkLinesInt(pPE, false))
            {
               pos = m_pPinTable->GetStartPosition();
            }
         }
      }
   }

   for(int i=0; i<TempElems.GetSize(); i++)
      InsertElem(TempElems[i]);

   if(pTE)
   {
      MoveFlaggedToTail(EF_SELECT);
      MoveFlaggedToTail(EF_RUBBLINE);
   }
   return bChanged;
}

/*func*------------------------------------------------------------------------
 * resolve overlapping conflicts after DropIn-operations
 * - checks for elements at fixed_line
 * - call AFTER ConnectAllElems (scans for lines that overlaps a selected 
 *   element AND connects two pins of them)
 *---------------------------------------------------------------------------*/
bool CElemPtrList::DropInSelectedElems()
{
   POSITION   pos;
   CElem     *pE, *pTE;
   CPoint     p1, p2;
   bool bChanged=false;
   TPinEntry *pPE;
   int        j;

   for( pos = GetHeadPosition(); pos != NULL; )       // for each element
   {
      pE = (CElem*) GetNext( pos );
      if((pE != NULL) &&
         (pE->GetType() == ET_LINE) &&
         !pE->ChkFlag(EF_RUBBLINE) )
      {
         ASSERT(pE->m_Con.GetSize() == 2);
         p1 = pE->m_Con[0]->m_Pos;
         p2 = pE->m_Con[1]->m_Pos;
         ASSERT((p1.x == p2.x) || (p1.y == p2.y));
         if (!m_pPinTable->Lookup(p1, pPE) || !pPE)
         {
            ASSERT(0);
         } else {
            if(pPE->m_iConnects <2)
            {
               if (!m_pPinTable->Lookup(p2, pPE) || !pPE)
               {
                  ASSERT(0);
               }
            }
            for (j=0; j < pPE->m_Elems.GetSize(); j++)
            {
               pTE = pPE->m_Elems[j];
               if( pTE && (pTE->GetType() != ET_LINE) )
               {
                  CRect R(pTE->GetOrigin(), pTE->GetSize());

                  bool bDel;
                  
                  if(pE->GetSize().cx==0)    
                  {  // vertical   line (deletion candidate)
                     bDel =  chkValInRange(pE->GetOrigin().x, R.left, R.right);
                     bDel &= chkValAtRange(pE->GetOrigin().y, R.top, R.bottom);
                     bDel &= chkValAtRange(pE->GetOrigin().y+pE->GetSize().cy, R.top, R.bottom);
                  }else                      
                  {  // horizontal line (deletion candidate)
                     bDel =  chkValInRange(pE->GetOrigin().y, R.top,  R.bottom);
                     bDel &= chkValAtRange(pE->GetOrigin().x, R.left, R.right);
                     bDel &= chkValAtRange(pE->GetOrigin().x+pE->GetSize().cx, R.left, R.right);
                  }

                  if( bDel )
                  {
                     DeleteElem(pE);
                     pos = GetHeadPosition();   // <<-- check for neceesarity (if pE->is a container)
                     bChanged = true;
                  }
               }
            }
         }
      }
   }
   return bChanged;  // signal modification
}

/*func*------------------------------------------------------------------------
  connect the pins of ALL elements (call after INSERT, MOVE, RESIZE of elements)
  - remove all lines (fixed and rubber) with size 0,0
  - remove rubber lines and other fixed lines at DIV/CONs
  - break and/or link lines at new connect points
  in :-
  out:-
-----------------------------------------------------------------------------*/
void CElemPtrList::ConnectAllElems()
{
   POSITION   pos, pos2, pos_last;
   TPinEntry *pPE;
   CPoint     key;
   CElem     *pE, *pTE, *pRubb;

   pRubb = NULL;
   pos2  = NULL;
   for( pos = GetTailPosition(); (pos_last = pos) != NULL; )
   {
      pE = (CElem*) GetPrev( pos );
      ASSERT(pE);
      if (pE)
      {
         // for mark breaked lines and further remove lines under drop in elems
         pE->ClrFlag(EF_PROCESSED);
         // remove lines with size (0,0)
         if (pE->GetType() == ET_LINE)
         {
            if (((CElemLine*)pE)->GetSize() == CSize(0,0) )
            {
               DeleteElem (pE);
            } 
            else if (pE->ChkFlag(EF_RUBBLINE))
            {  // only change position !!
               RemoveAt (pos_last);
               AddTail (pE);
               if (pRubb == NULL)
               {
                  pRubb = pE;
                  pos2  = GetTailPosition();
               }
            }
            else if(pE->GetSize().cx<0 || pE->GetSize().cy<0)
            {
               ((CElemLine*)pE)->SwapOrigin();
            }
         }
      }
   }

   if (pos2 == NULL)
   {
      pos2 = GetHeadPosition();
   }
   // remove (rubber) lines at
   //   * fixed lines
   //   * div/con elements
   //   * powerrails
   // no check of other flags ..
   for (pos = pos2; pos != NULL; )
   { 
      pos_last = pos;
      pE = (CElem*) GetNext (pos);
      if (pE && (pE->GetType() == ET_LINE))
      {
         if (pE->ChkFlag(EF_RUBBLINE) )
         {
            // check for lines ..
            if ((((pTE = SearchLine(pE->GetOrigin(), pE->GetOrigin()+pE->GetSize(), false))!=NULL) ||
                 ((pTE = SearchLine(pE->GetOrigin(), pE->GetOrigin()+pE->GetSize(), true ))!=NULL)) &&
                (pTE != pE) )
            {
               DeleteElem (pE);
            } else {
               // check for div/con ..
               pos2 = GetHeadPosition();
               while ( (pos2 != NULL) &&
                      ((pTE = SearchPin(pE->GetOrigin(), pos2)) != NULL) )
               {
                  if ( ((pTE->GetType() == ET_ALTDIV) || (pTE->GetType() == ET_SIMDIV)) &&
                       (pE->GetOrigin().y == pTE->GetOrigin().y) &&
                       (pE->GetSize().cy  == 0) &&
                       (pE->GetOrigin().x + pE->GetSize().cx >= pTE->GetOrigin().x                    ) && 
                       (pE->GetOrigin().x + pE->GetSize().cx <= pTE->GetOrigin().x + pTE->GetSize().cx) )
                  {
                     // rubberline is complete over div/con -> remove it and go
                     DeleteElem (pE);
                     break;
                  }
                  if ( (pTE->GetType() == ET_POWERRAIL) &&
                       (pE->GetOrigin().x == pTE->GetOrigin().x) &&
                       (pE->GetSize().cx  == 0) &&
                       (pE->GetOrigin().y + pE->GetSize().cy >= pTE->GetOrigin().y                    ) && 
                       (pE->GetOrigin().y + pE->GetSize().cy <= pTE->GetOrigin().y + pTE->GetSize().cy) )
                  {
                     // rubberline is complete over div/con -> remove it and go
                     DeleteElem (pE);
                     break;
                  }
               }
            }
         }
      }
   }

   ASSERT(m_pPinTable);

   // process each connection
   if (m_pPinTable)
   {
      pos = m_pPinTable->GetStartPosition();
      while (pos != NULL)   
      {
         m_pPinTable->GetNextAssoc (pos, key, pPE);
         ASSERT(pPE);
         if (pPE)
         {  
            BreakLines(pPE->m_Pos);
            // LinkLines can remove elements ! pos can be invalid
            if (LinkLinesInt(pPE))
            {
               pos = m_pPinTable->GetStartPosition();
            }
         }
      }
   }
   PurgeOpenRubberLines(EF_LAST);   // EF_LAST: puge all
}

/*func*------------------------------------------------------------------------
  try to connect fixed lines in one point to reduce line numbers
  (set EF_UPDATE at linked line)
  in :p - point in grid coords at sheet
  out:true  - one line removed
      false - no line removed
-----------------------------------------------------------------------------*/
bool CElemPtrList::LinkLines(CPoint p, bool boAll)
{
   TPinEntry *pPE;
   CPoint     key;

   ASSERT(m_pPinTable);

   if (m_pPinTable && m_pPinTable->Lookup(p, pPE))
   {
      return LinkLinesInt(pPE,boAll);
   } else {
      ASSERT(0&&"invalid argument: p is not a pin position");
      return false;
   }
}

/*func*------------------------------------------------------------------------
  try to connect fixed lines in one point to reduce line numbers
  (set EF_UPDATE at linked line)
  in :pPE   - TPinEntry* (pin table entry pointer) 
      boAll - true: all lines (default); false: unselected lines only)
  out:true  - one line removed
      false - no line removed
-----------------------------------------------------------------------------*/
bool CElemPtrList::LinkLinesInt(TPinEntry *pPE, bool boAll)
{
   CElem    *pE, *pL[2];
//   CPin     *pPin;
   CPoint    pp[2];
   CElemFlag flag;
   int       i, j;

   pL[0] = NULL;
   pL[1] = NULL;

   if(!m_FixPoints.Includes(pPE->m_Pos))  // not a fix point?
   {
      if ((( boAll && (pPE->m_iConnects   == 2)) ||         // two elements are connected
         (!boAll && (pPE->m_iDeSelected == 2))) &&          // both are selected
         (pPE->m_iRubbLines == 0))                          // non-rubber lines
      {
         if (boAll )                                        // put both elems to pL[1] and pL[0]
         {
            ASSERT(pPE->m_Elems.GetSize() == 2);
            
            pL[0] = pPE->m_Elems[0];
            pL[1] = pPE->m_Elems[1];
         } else {
            ASSERT(pPE->m_Elems.GetSize() >= 2);            // expect exactly two elements
            
            for( i=0,j=0; i<pPE->m_Elems.GetSize(); i++)    // choose the two de-selected elements
            {
               if (!pPE->m_Elems[i]->ChkFlag(EF_SELECT))
               {
                  pL[j++] = pPE->m_Elems[i];
                  if (j > 1) break;                         // skip the loop if both were found
               }
            }
         }
         ASSERT(pL[0]);
         ASSERT(pL[1]);
         
         if( pL[0] && pL[1] && CanJoinElems( pL[0], pL[1], boAll) )
         {
/*
         if ( pL[0] && pL[1] &&
            (boAll || (pL[0]->ChkFlag(EF_SELECT) == pL[1]->ChkFlag(EF_SELECT)) )&&
            (pL[0]->ChkFlag(EF_COMMENT) == pL[1]->ChkFlag(EF_COMMENT)) &&             // don't link comment to others
            (pL[0]->GetType() == ET_LINE) && !pL[0]->ChkFlag(EF_RUBBLINE) &&
            (pL[1]->GetType() == ET_LINE) && !pL[1]->ChkFlag(EF_RUBBLINE) &&
            ((CGRTool::GetDir(pL[0]->GetOrigin(), pL[0]->GetOrigin() + pL[0]->GetSize()) & 1) ==
            (CGRTool::GetDir(pL[1]->GetOrigin(), pL[1]->GetOrigin() + pL[1]->GetSize()) & 1)) )
         {
*/
            // only equal typed elements can be combined
            ASSERT(pL[0]->GetType() == pL[1]->GetType());

            // only the following types are allowed
            ASSERT(pL[0]->GetType() == ET_LINE || pL[0]->GetType() == ET_POWERRAIL);

            // expect horizontal or vertical orientation
            ASSERT( (pL[0]->GetSize().cx==0) || (pL[0]->GetSize().cy==0) );
            ASSERT( (pL[1]->GetSize().cx==0) || (pL[1]->GetSize().cy==0) );

            // find new end points
            pp[0] = pL[0]->GetOrigin()!= pPE->m_Pos ? pL[0]->GetOrigin() :pL[0]->GetOrigin()+pL[0]->GetSize();
            pp[1] = pL[1]->GetOrigin()!= pPE->m_Pos ? pL[1]->GetOrigin() :pL[1]->GetOrigin()+pL[1]->GetSize();

            ASSERT( pp[0] != pp[1]);
            ASSERT( pL[0]->GetOrigin()== pPE->m_Pos || pL[0]->GetOrigin()+pL[0]->GetSize() == pPE->m_Pos);
            ASSERT( pL[1]->GetOrigin()== pPE->m_Pos || pL[1]->GetOrigin()+pL[1]->GetSize() == pPE->m_Pos);
/*
            for( i=0; i<2; i++)
            {
               pPin = pL[i]->SearchConn(pL[i]->GetOrigin());
               ASSERT(pPin);
               if( pPin && (pPin->m_Pos != pPE->m_Pos) ) 
               {
                  pp[i] = pPin->m_Pos;
               }

               pPin = pL[i]->SearchConn(pL[i]->GetOrigin() + pL[i]->GetSize());
               ASSERT(pPin);
               if( pPin && (pPin->m_Pos != pPE->m_Pos) ) 
               {
                  pp[i] = pPin->m_Pos;
               }
            }
 */          
            // save interesting flags (EF_EDITLINE|EF_SELECT)
            flag += pL[0]->GetFlag();
            flag += pL[1]->GetFlag();
            flag = flag & (CElemFlag(EF_SELECT) + CElemFlag(EF_EDITLINE));

            // handle differences in flags
            if(pL[0]->ChkFlag(EF_COMMENT) || pL[1]->ChkFlag(EF_COMMENT))
               flag.Set(EF_COMMENT);

            if(pL[0]->ChkFlag(EF_TEMPELEM) && pL[1]->ChkFlag(EF_TEMPELEM))
               flag.Set(EF_TEMPELEM);

            if(boAll && (pL[0]->ChkFlag(EF_SELECT) != pL[0]->ChkFlag(EF_SELECT)) )
               flag.Clr(EF_SELECT);

            // mark for update screen
            flag.Set(EF_UPDATE);
            
            // save element type
            ELEM_TYPE ET = pL[0]->GetType();

            // remove and delete the elements
            ASSERT(pL[0] != pL[1]);
            DeleteElem (pL[0]); 
            DeleteElem (pL[1]); 

            switch(ET)
            {
            case ET_LINE:        pE = new CElemLine (pp[0], pp[1], false); break;   // origin, end !!
            case ET_POWERRAIL:   pE = new CElemPower(pp[0], pp[1]-pp[0] ); break;   // origin, size!!
            default:
               ASSERT("unexpected element type"&&0);
               pE = NULL;
            }
            ASSERT(pE);

            if (pE)
            {
               pE->SetFlag(flag);
               
               InsertElem(pE);
            }
            return true;
         }
      } 
   }
   return false;
}

/*func*------------------------------------------------------------------------
  decides wether two elements (i.e. lines) can be joined
  in :   2x CElem* (both elements)
  out:-
  ret:   true: yes
-----------------------------------------------------------------------------*/
inline bool CElemPtrList::CanJoinElems( CElem* pE1, CElem* pE2, bool bAll)
{
   bool bCan;
   ELEM_TYPE T1 = pE1->GetType();
   ELEM_TYPE T2 = pE2->GetType();

   // identical selection- and comment-flags
   bCan = (bAll || pE1->ChkFlag(EF_SELECT) == pE2->ChkFlag(EF_SELECT)) &&  // both must have equal flags
          pE1->ChkFlag(EF_COMMENT) == pE2->ChkFlag(EF_COMMENT);            // don't link comment to others

   // identical types
   bCan &= (T1 == T2);

   // check allowed types
   bCan &= (T1 == ET_POWERRAIL) || 
           (T1 == ET_LINE) && !pE1->ChkFlag(EF_RUBBLINE) && !pE2->ChkFlag(EF_RUBBLINE);

   // parallel
   bCan &= ((CGRTool::GetDir(pE1->GetOrigin(), pE1->GetOrigin() + pE1->GetSize()) & 1) ==
            (CGRTool::GetDir(pE2->GetOrigin(), pE2->GetOrigin() + pE2->GetSize()) & 1));

   return bCan;
}


/*func*------------------------------------------------------------------------
  removes unconnected temp-lines, inserted by DropOut-operation
  out:-
-----------------------------------------------------------------------------*/
void CElemPtrList::DropOutPurge()
{
   POSITION pos = GetHeadPosition();
   while(pos)
   {
      CElem* pE = (CElem*)GetNext(pos);
      ASSERT(pE);
      if(pE->ChkFlag(EF_TEMPELEM))
      {
         ASSERT(pE->GetType() == ET_LINE);
         ASSERT(!pE->ChkFlag(EF_RUBBLINE));

         TPinEntry* pPE;
         m_pPinTable->Lookup(pE->GetOrigin(), pPE);
         if(pPE->m_iConnects < 2)
         {
            m_pPinTable->Lookup(pE->GetOrigin()+pE->GetSize(), pPE);
            if(pPE->m_iConnects < 2)
               DeleteElem(pE);      // remove unconnected temp lines
         }
      }
   }
}

/*func*------------------------------------------------------------------------
  try to connect fixed lines and other connect points in one point by breaking
  one fixed line
  (set EF_UPDATE at breaked line)
  in :p - point in grid coords at sheet
  out:-
-----------------------------------------------------------------------------*/
bool CElemPtrList::BreakLines(CPoint p)
{
   POSITION pos;
   CElem   *pE, *pTE;
   CPoint   p2;
   bool     bBroken=false;

   pos = GetHeadPosition();
   if(pos != NULL)
   {
      do {  // search only fixed lines !
         pE = (CElem*) GetNext( pos );
         ASSERT(pE);
         if ( pE &&
            (pE->GetType() == ET_LINE) && 
            !pE->ChkFlag(EF_RUBBLINE) )
         {
            if(pE->GetSize().cx == 0 && p.x == pE->GetOrigin().x)
            {
               if(pE->GetSize().cy >=0 )
               {
                  if( (p.y > pE->GetOrigin().y) && (p.y < pE->GetOrigin().y + pE->GetSize().cy))
                  {
                     bBroken = true;
                  }
               } else {
                  if( (p.y < pE->GetOrigin().y) && (p.y > pE->GetOrigin().y + pE->GetSize().cy))
                  {
                     bBroken = true;
                  }
               }
            }
            
            if(pE->GetSize().cy == 0 && p.y == pE->GetOrigin().y)
            {
               if(pE->GetSize().cx >= 0 )
               {
                  if( (p.x > pE->GetOrigin().x) && (p.x < pE->GetOrigin().x + pE->GetSize().cx))
                  {
                     bBroken = true;
                  }
               } else {
                  if( (p.x < pE->GetOrigin().x) && (p.x > pE->GetOrigin().x + pE->GetSize().cx))
                  {
                     bBroken = true;
                  }
               }
            }
            
            if (bBroken)
            {
               // save original end position
               p2 = pE->GetOrigin() + pE->GetSize();
               if( p != pE->GetOrigin() && p != p2 ) // not an endpoint
               {  // cut original line
                  pE->Resize (0, 0, -(p2.x - p.x), -(p2.y - p.y));
                  // add new line - is always no rubberband!
                  pTE = new CElemLine(p, p2, false);
                  ASSERT(p != p2);
                  ASSERT(pTE);
                  InsertElem(pTE);
                  // set flags ..
                  pE->SetFlag(EF_UPDATE); 
                  pTE->SetFlag(EF_UPDATE);
                  
                  if(pE->ChkFlag(EF_SELECT))    // new inserted line gets the selection attribute
                     pTE->SetFlag(EF_SELECT);
                  
                  if(pE->ChkFlag(EF_COMMENT))   // new inserted line gets the selection attribute
                     pTE->SetFlag(EF_COMMENT);
                  
                  pE->SetFlag(EF_PROCESSED);    // mark breaked lines for drop in remove test
                  pTE->SetFlag(EF_PROCESSED);
                  
                  break;
               }
            }
         }
      } while (!bBroken && (pE != NULL) && (pos != NULL));
   }
   return bBroken;
}


/*func*------------------------------------------------------------------------
  clear flag at all elements in pool
  in :-
  out:-
-----------------------------------------------------------------------------*/
bool CElemPtrList::ClearFlags(TElemFlags flag)
{
   POSITION  pos;
   CElem    *pE;
   bool      bChanged=false;

   for( pos = GetHeadPosition(); pos != NULL; )
   {
      if( (pE = (CElem*) GetNext( pos )) != NULL )
      {
         if (pE->ChkFlag(flag))
         {
            bChanged = true;
         }
         pE->ClrFlag(flag);
      }
   }
   return bChanged;
}


bool CElemPtrList::ClearFlags(CElemFlag EF)
{
   POSITION  pos;
   CElem    *pE;
   bool      bChanged=false;

   for( pos = GetHeadPosition(); pos != NULL; )
   {
      if( (pE = (CElem*) GetNext( pos )) != NULL )
      {
         if (pE->GetFlag() & EF)
         {
            bChanged = true;
         }
         pE->ClrFlag(EF);
      }
   }
   return bChanged;
}

/*func*------------------------------------------------------------------------
  set flag at all elements in pool
  in :-
  out:-
-----------------------------------------------------------------------------*/
bool CElemPtrList::SetFlags(TElemFlags flag)
{
   POSITION  pos;
   CElem    *pE;
   bool      bChanged=false;

   for( pos = GetHeadPosition(); pos != NULL; )
   {
      if( (pE = (CElem*) GetNext( pos )) != NULL )
      {
         if (!pE->ChkFlag(flag))
         {
            bChanged = true;
         }
         pE->SetFlag(flag);
      }
   }
   return bChanged;
}

/**func*------------------------------------------------------------------------
* changes selection flags on a signal
* in :- PinPosition to be started from
*     - MODE: set selection/reset selection
*     - bRem: include lines with comment-flag set (default: false)
* out:- modifies the flags: EF_SELECT, EF_UPDATE
*----------------------------------------------------------------------------*/
bool CElemPtrList::SelectSignal(CPoint p, bool bSet, bool bRem)
{
   CElem     *pE=NULL;
   POSITION  pos;
   POSITION  pos0;
   
   ClearFlags(EF_UPDATE);
//   ClearFlags(EF_SELECT);

   bool bRes = !m_FixPoints.Includes(p) && FlagSignal(p,EF_UPDATE,bRem);

   if(bRes)
   {
      pE = MoveFlaggedToTail(EF_UPDATE);
      ASSERT(pE);
 
      pos = Find(pE);      
      pos0 = pos;

      ASSERT(pos);
 
      if(bSet)
      {
         while(pos)
         {                                    // collect the points of the CURRENT signal
            pE = (CElem*) GetNext(pos);
            pE->SetFlag(EF_SELECT);
         }
      }else
      {
         while(pos)
         {                                    // collect the points of the CURRENT signal
            pE = (CElem*) GetNext(pos);
            pE->ClrFlag(EF_SELECT);
         }
      }
   }
   
   return bRes;
}

/**func*-----------------------------------------------------------------------
  set flags under ulMask at signal (except that <p> is a fix point)
   ATTENTION : you should call ClearFlags(EF_UPDATE) before enter this routine!
  in :p  - search point for signal in grid units
      ulmask - mask for flags to set
  out:true  - signal found and flagged
      false - no signal found, nothing flagged
  side effects: 

-----------------------------------------------------------------------------*/
bool CElemPtrList::FlagSignal(CPoint p, CElemFlag flag, bool bRem, bool bWire)
{
   TPinEntry *pPE;
   CElem     *pE=NULL;
   bool       bChanged=false;
   int        j;

   ASSERT(m_pPinTable);
   ASSERT(flag != CElemFlag(EF_SELECT));
   if (m_pPinTable)
   {
      if (!m_pPinTable->Lookup(p, pPE))
      {
         ASSERT(0&&"the argument <p> is not a pin position");
      } else {
         ASSERT(pPE);
         if (pPE && (!bWire || pPE->m_iConnects == 2) )
         {
            for (j=0; j < pPE->m_Elems.GetSize(); j++)
            {
               pE = pPE->m_Elems[j];
               if ( pE && 
                   (pE->GetType() == ET_LINE)        &&
                   (pE->ChkFlag(EF_COMMENT)==bRem) &&
                   !pE->ChkFlag(EF_UPDATE))
               {
                  bChanged = true;
                  pE->SetFlag(flag + CElemFlag(EF_UPDATE));
                  
                  if(pE->GetType() == ET_LINE) // do recursion at lines only
                  {
                     p = pE->GetOrigin();
                     if(!m_FixPoints.Includes(p))
                     {
                        bChanged |= FlagSignalInt(p, flag, bRem, bWire);
                     }

                     p = pE->GetOrigin() + pE->GetSize();
                     if(!m_FixPoints.Includes(p))
                     {
                        bChanged |= FlagSignalInt(p, flag, bRem, bWire);
                     }
                  }
               }
            }
         }
      }
   }
   return bChanged;
}

/*func*------------------------------------------------------------------------
  internal set flags under ulMask at signal
  in :p  - search point for signal in grid units
      ulmask - mask for flags to set
  out:true  - signal found and flag set
      false - no signal found, nothing changed
-----------------------------------------------------------------------------*/
bool CElemPtrList::FlagSignalInt(CPoint p, CElemFlag flag, bool bRem, bool bWire)
{
   TPinEntry *pPE;
   CElem     *pE;
   bool       bChanged=false;
   int        j;

   ASSERT(m_pPinTable);
   if (m_pPinTable)
   {
      if (!m_pPinTable->Lookup(p, pPE))
      {
         ASSERT(0);
      } else {
         ASSERT(pPE);
         if (pPE && (!bWire|| pPE->m_iConnects==2) )
         {
            for (j=0; j < pPE->m_Elems.GetSize(); j++)
            {
               pE = pPE->m_Elems[j];
               if ( pE && 
                   (pE->GetType() == ET_LINE)        &&
                   (pE->ChkFlag(EF_COMMENT)==bRem) &&
                   !pE->ChkFlag(EF_UPDATE))
               {
                  bChanged = true;
                  pE->SetFlag(flag + CElemFlag(EF_UPDATE));

                  if(pE->GetType() == ET_LINE) // do recursion at lines only
                  {
                     p = pE->GetOrigin();
                     if(!m_FixPoints.Includes(p))
                     {
                        FlagSignalInt(p, flag, bRem, bWire);
                     }
                     p = pE->GetOrigin() + pE->GetSize();
                     if(!m_FixPoints.Includes(p))
                     {
                        FlagSignalInt(p, flag, bRem, bWire);
                     }
                  }
               }
            }
         }
      }
   }
   return bChanged;
}


/*func*------------------------------------------------------------------------
  description: converts selected Lines into rubber bands (multiple iterations 
               are performed, each for a single selected signal)
  in :- set of selected lines (and other elements - ignored)
  out:- success information
-----------------------------------------------------------------------------*/
bool CElemPtrList::UnrouteFlaggedLines(bool bComment, TElemFlags EF)
{
   POSITION    pos = NULL;
   POSITION    pos2;
   CElem*      pE  = NULL;
   CElemFlag   Flags;         // for masking

   int idx;
   CPosArray PExt;            // set of unprocessed points (not inside the <ready set>)
   CPosArray PRdy;            // set of processed   points (<ready set> itself)
   
   ASSERT(EF!=EF_COMMENT);
   ASSERT(EF!=EF_CURRENT);
   ASSERT(EF!=EF_PROCESSED);

   // clear used flags:
   Flags  = EF_PROCESSED;
   Flags += EF_CURRENT;
   ClearFlags( Flags );

   pos = GetHeadPosition();

   while(pos)                 // for all signals
   {
      // find the 1st flagged line that is not marked as PROCESSED
      pE = NULL;
      do{
         CElem* p = (CElem*) GetNext(pos);

         if( (p->GetType() == ET_LINE)   && 
             (p->ChkFlag(EF))          &&
             (p->ChkFlag(EF_COMMENT) == bComment) &&  // keep comment separate from other lines
            !(p->ChkFlag(EF_PROCESSED)))
         {
            pE = p;
         }
      }while(pos && !pE);

      if(pE)                                    // anything found:... else: ready
      {
         // move the lines of the entire signal to the back (unselected too)
         ClearFlags( EF_UPDATE );

         bool bOk;
         if(!m_FixPoints.Includes(pE->m_Con[0]->m_Pos))
         {
            bOk = FlagSignal( pE->m_Con[0]->m_Pos, EF_CURRENT, bComment);
         }else if(!m_FixPoints.Includes(pE->m_Con[1]->m_Pos))
         {
            bOk = FlagSignal( pE->m_Con[1]->m_Pos, EF_CURRENT, bComment);
         }else
         {
            pE->SetFlag(EF_CURRENT);
            bOk = true;
         }

         if(!bOk)
         {
            break;
         }
         pE = MoveFlaggedToTail(EF_CURRENT);    // 1st moved element is returned
         ASSERT(pE);

         POSITION StartPos = Find(pE);
         pos = StartPos;
         ASSERT(pos);

         PExt.RemoveAll();                      // empty the pin pools
         PRdy.RemoveAll();

         do {                                   // collect the points of the CURRENT signal
            pE = (CElem*) GetNext(pos);
            CPosArray Pins;
            Pins.AddElem(pE);
            PExt.Join(Pins);
         } while(pos);

         ASSERT( PExt.GetSize() > 1 );          // at least 2 points should be found
         ASSERT( PExt.DebugDump("PExt:"));      // statement is removed if _DEBUG is undefined
         
         Flags  = EF_CURRENT;                   // delete selected lines of the signal
         Flags += EF;

         if(bComment)
            Flags += EF_COMMENT;
         
         DeleteFlaggedElems(Flags);             

         for(idx = 0; idx<PExt.GetSize();)      // remove isolated pins from signals pin pool
         {
            pos2 = GetHeadPosition();
            CPoint Point = PExt.GetAt(idx);
            if (!SearchPin(Point, pos2) != NULL)
            {
               if(!m_FixPoints.Includes(Point))
               {
                  PExt.RemoveAt(idx,1);
               }
            } else 
            {
               idx++;
            }
         }
         ASSERT( PExt.DebugDump("PExt:"));      // statement is removed if _DEBUG is undefined

         if(PExt.GetSize()>0)                   // if there are remaining pins...
         {                                      // insert nedded lines to complete the signal
            for(idx = 0; idx < PExt.GetSize(); idx++)
            {
               if(!m_FixPoints.Includes(PExt[idx]))   // ignore fix points
               {
                  PRdy.Add(PExt[idx]);          // move the pin to the Ready-Set
                  PExt.RemoveAt(idx);
                  break;
               }
            }
            ASSERT( PExt.DebugDump("PExt:"));      // statement is removed if _DEBUG is undefined
            ASSERT( PRdy.DebugDump("PRdy:"));      // statement is removed if _DEBUG is undefined
            
            pE = NULL;                          // adjust the ready set by connected points
            for(int j = 0; j < PRdy.GetSize(); j++)
            {
               if( (pos = GetHeadPosition()) )
               {
                  do
                  {
                     CPoint Point = PRdy.GetAt(j);
                     pE = SearchPin(Point,pos);
                     if ( pE && (pE->GetType() == ET_LINE) && 
                        (pE->ChkFlag(EF_COMMENT) == bComment))   // line found at this point
                     {
                        CPosArray PosArr;
                        PosArr.AddElem(pE);
                        PRdy.Join(PosArr);
                        PExt.Drop(PosArr);
                     }
                  } while (pos && pE);
               }
            }//for

            ASSERT( PExt.DebugDump("PExt:"));      // statement is removed if _DEBUG is undefined
            ASSERT( PRdy.DebugDump("PRdy:"));      // statement is removed if _DEBUG is undefined

            while(PExt.GetSize()>0)
            {
               int iP1;
               int iP2;

               if( PRdy.FindNext(&iP1, &iP2, PExt) )  // false: use manattan metric
               {
                  CPoint PStart = PRdy[iP1];
                  CPoint PEnd   = PExt[iP2];
                  CElem* pE     = new CElemLine(PStart,PEnd, TRUE);
                  ASSERT_VALID(pE);

                  if( pE ) 
                  {
                     InsertElem(pE);
                     pE->SetFlag(EF_CURRENT);
                     pE->SetFlag(EF);
                     if(bComment)
                        pE->SetFlag(EF_COMMENT);
                  }

                  PRdy.Add(PExt[iP2]);   // move new point to the ready-set
                  PExt.RemoveAt(iP2);

                  // adjust the ready set by connected points
                  pE = NULL;
                  for(int j = 0; PExt.GetSize() > 0 && j < PRdy.GetSize(); j++)
                  {
                     for( pos2 = GetHeadPosition(); pos2; )
                     {
                        CPoint Point = PRdy.GetAt(j);
                        pE = SearchPin( Point, pos2);
                        if(pE)
                        {
                           if( pE && (pE->GetType() == ET_LINE) &&
                              (pE->ChkFlag(EF_COMMENT) == bComment))
                           {
                              CPosArray PosArr;
                              PosArr.AddElem(pE);
                              PRdy.Join(PosArr);
                              PExt.Drop(PosArr);
                           }
                        }else
                        {
                           break;
                        }
                     }//for
                  }//for
                  ASSERT( PExt.DebugDump("PExt:"));      // statement is removed if _DEBUG is undefined
                  ASSERT( PRdy.DebugDump("PRdy:"));      // statement is removed if _DEBUG is undefined
               }
            }
         }
         // adjust the flags for the processed lines
         for(pos2 = GetHeadPosition(); pos2!=NULL; )
         {
            pE = (CElem*) GetNext(pos2);
            if(pE->ChkFlag(EF_CURRENT))
               pE->SetFlag(EF_PROCESSED);
         }
         ClearFlags(EF_CURRENT);
      }else
         break;

      pos = GetHeadPosition();
   }

   ConnectAllElems();
   PurgeOpenRubberLines(EF_SELECT);
   return true;
}

/*func*------------------------------------------------------------------------
removes flagged but open rubber lines
in:   element selection flag (EF_LAST: ignore all flags)
side: invalidates CElem* and POSITION references into the list
-----------------------------------------------------------------------------*/
void CElemPtrList::PurgeOpenRubberLines (TElemFlags EF)
{
   // reverse order to improve performance (assumes rubberlines at the tail)
   POSITION pos = GetTailPosition();
   while(pos)
   {
      CElem* pE = (CElem*)GetPrev(pos);
      ASSERT(pE);
      if( pE->ChkFlag(EF_RUBBLINE) && (pE->ChkFlag(EF) || EF==EF_LAST))
      {
         bool bDel;
         
         // unconnected
         bDel = (pE->m_Con[0]->GetConnects() < 2)  || (pE->m_Con[1]->GetConnects() < 2);
         if( bDel )
         {
            DeleteElem(pE);
            pos = GetTailPosition();
         }
      }
   }
}


/*func*------------------------------------------------------------------------
removes flagged rubber lines inside of the same signal (no signal function)
in:   element selection flag (EF_LAST: ignore all flags)
side: invalidates CElem* and POSITION references into the list
-----------------------------------------------------------------------------*/
void CElemPtrList::PurgeRedundantRubber()
{
   CTypedPtrList<CObList,CElem*> List; // selected elements

   // reverse order to improve performance (assumes rubberlines at the tail)
   POSITION elpos;
   POSITION pos = GetTailPosition();
   while(pos)
   {
      elpos = pos;
      CElem* pE = (CElem*)GetPrev(pos);
      ASSERT(pE);
      if( pE->ChkFlag(EF_RUBBLINE) )
      {
         bool bDel;
         bDel  =  pE->m_Con[0]->GetPinEntry()->m_iConnects<2;
         bDel |= (pE->m_Con[1]->GetPinEntry()->m_iConnects<2);

         RemoveElemAt(elpos);    // performance: !!!searches again for pE!!!
         List.AddTail(pE);       // deletion candidate
      }
   }

   ClearFlags(EF_UPDATE);
   while(!List.IsEmpty())        // for each element in the purge candidate list
   {
      TPinEntry *pPE = NULL;
      CElem* pE = List.RemoveHead();
      CPoint P1 = pE->GetOrigin();
      CPoint P2 = P1 + pE->GetSize();

      if(!m_pPinTable->Lookup(P1,pPE))
      {
         delete pE;        // redundant: open rubber
         continue;
      }
      if(!m_pPinTable->Lookup(P2,pPE))
      {
         delete pE;        // redundant: open rubber
         continue;
      }

      if(!FlagSignal(P1,EF_UPDATE))
      {
         InsertElem(pE);   // not redundant: re-insert into list
         continue;
      }else
      {
         bool bDel = false;
         for(int i=0; i<pPE->m_Elems.GetSize(); i++)  // pPE still refers to the P2 - pin entry
         {
            if(pPE->m_Elems[i]->ChkFlag(EF_UPDATE))   // flagged
            {
               // expects FlagSignal to flag lines only (not other elements)
               ASSERT(pPE->m_Elems[i]->GetType()==ET_LINE);
               bDel = true;
               break;
            }
         }

         if(bDel)
         {
            delete pE;
         }else
         {
            InsertElem(pE);
         }
      }
   }
}


/*func*------------------------------------------------------------------------
  updates the connect counter of each pin
  clear all precessed flags (for rubberbands)
  in :bInsMode - false: Move/resize
                 true : Insert (no connect rubberlines to unselected elements!)
  out:-
-----------------------------------------------------------------------------*/
void CElemPtrList::UpdatePinStatus(bool bInsMode)
{
   POSITION  pos;
   CElem    *pTE;
   CPin     *pPin;
   int       i;

   ASSERT(m_pPinTable);
   if (m_pPinTable)
   {
      for( pos = GetHeadPosition(); pos != NULL; )
      {
         pTE = (CElem*) GetNext( pos );

         if (pTE)
         {
            for( i=0; i<pTE->m_Con.GetSize(); i++ )
            {  // for all pins at this element
               pPin  = pTE->m_Con.GetAt(i);
               ASSERT(pPin);
               ASSERT(pPin->m_Entry);
               if (pPin && pPin->m_Entry)
               {
                  if (!pTE->ChkFlag(EF_RUBBLINE) || bInsMode)
                  {
                     if (pTE->ChkFlag(EF_SELECT))
                     {
                        pPin->m_Flag.Set(PF_SELECT);
                     } else {
                        pPin->m_Flag.Clr(PF_SELECT);
                     }
                  } else {
                     if (((pTE->GetSize() == CSize(0,0)) && (i==1)) ||
                         (pPin->m_Entry->m_iDeSelected == 0) )
                     {
                        pPin->m_Flag.Set(PF_SELECT);
                     } else {
                        pPin->m_Flag.Clr(PF_SELECT);
                     }
                  }
               } 
            }
         }
      }
   }
}

void CElemPtrList::GetStatisticData(CStatisticData& data)
{
   POSITION  pos;
   CElem    *pE;

   data.Clear();

   for (pos = GetHeadPosition(); pos != NULL; )
   {
      pE = (CElem*) GetNext( pos );
      ASSERT(pE);
      if (pE)
      {
         switch (pE->GetType())
         {
            case ET_ACTION     : data.m_iSFC_Action++; break;
            case ET_ACTIONBLOCK: data.m_iSFC_ActBlk++; break;
            case ET_COMMENT    : data.m_iFBD_Cmt++;    break;
            case ET_CONIN      : data.m_iFBD_Cont++;   break;
            case ET_CONOUT     : data.m_iFBD_Conn++;   break;
            case ET_ALTDIV     : data.m_iSFC_AltDiv++; break;
            case ET_SIMDIV     : data.m_iSFC_SimDiv++; break;
            case ET_FB         : data.m_iFBD_FB++;     break;
            case ET_FUNCTION   : data.m_iFBD_Fct++;    break;
            case ET_PROCEDURE  : data.m_iFBD_Pro++;    break;
            case ET_LABEL      : data.m_iFBD_Label++;  break;
            case ET_JUMP       : data.m_iFBD_Jump++;   break;
            case ET_RETURN     : data.m_iFBD_Ret++;    break;
            case ET_STEP       : data.m_iSFC_Step++;   break;
            case ET_TRANSITION : data.m_iSFC_Tra++;    break;
            case ET_VARIN      : data.m_iFBD_VarRd++;  break;
            case ET_VAROUT     : data.m_iFBD_VarWr++;  break;
            case ET_VARINOUT   : data.m_iFBD_VarFb++;  break;
            case ET_POWERRAIL  : data.m_iLD_Power++;   break;
            case ET_CONTACT    : data.m_iLD_Contact++; break;
            case ET_COIL       : data.m_iLD_Coil++;    break;

            case ET_LINE       : 
               if (pE->ChkFlag(EF_RUBBLINE)) 
               {
                  data.m_iLINE_Rubb++; 
               } else {
                  data.m_iLINE_Fix++; 
               }
               break;

            case ET_CONST      : ASSERT(0);              break;
            default            : ASSERT(0);              break;
         }
      }
   }
   data.m_iSUM_FBD   = data.m_iFBD_Cmt    + data.m_iFBD_Cont   + data.m_iFBD_Conn   +
                       data.m_iFBD_FB     + data.m_iFBD_Fct    + data.m_iFBD_Pro    +
                       data.m_iFBD_Label  + data.m_iFBD_Jump   + data.m_iFBD_Ret    +
                       data.m_iFBD_VarRd  + data.m_iFBD_VarWr  + data.m_iFBD_VarFb;
   data.m_iSUM_SFC   = data.m_iSFC_Action + data.m_iSFC_ActBlk + data.m_iSFC_AltDiv +
                       data.m_iSFC_SimDiv + data.m_iSFC_Step   + data.m_iSFC_Tra;
   data.m_iSUM_LD    = data.m_iLD_Power   + data.m_iLD_Contact + data.m_iLD_Coil;
   data.m_iSUM_LINE  = data.m_iLINE_Rubb  + data.m_iLINE_Fix;
   data.m_iSUM_Total = data.m_iSUM_FBD    + data.m_iSUM_SFC    + data.m_iSUM_LINE;
}


/*func*------------------------------------------------------------------------
  setup routing matrix
  - determines the required routing area
  - instanciates the autoRouter
  - initializes the routing matrix with the elements
  in :- route comment lines too [y/n] (routing step2/step1)
  out:-
-----------------------------------------------------------------------------*/
bool CElemPtrList::RouteToSignalInit(TElemFlags EF,bool bRouteComment)
{
   bool bRoute = false;
   if(RouterCreate(false))
   {
      DeleteFlaggedElems(EF_AUTOELEM);
      RouterTrackRubber(EF); // adjust rubber lines to DivCon, PRail (if necessary/possible)

      ASSERT(m_pRouter);
      m_pRouter->m_bComment = bRouteComment;
      
      CElemFlag Flag(EF);

      if(RouterAddElems(EF))              // selected rubber lines found
      {
         m_pRouter->SetNextID();          // signal 2,ff.: all the lines & pins
         // for integrity test only
         POSITION pos = GetHeadPosition(); 
         while(pos)
         {
            CElem* pE = (CElem*)GetNext(pos);
            switch(pE->GetType())
            {
            case ET_LINE:
               {
                  CPoint P1 = pE->GetOrigin();
                  CPoint P2 = P1 + pE->GetSize();
                  
                  if(pE->ChkFlag(EF_RUBBLINE))
                  {
                     if(!pE->ChkFlag(EF) && pE->ChkFlag(EF_COMMENT)!=m_pRouter->m_bComment )
                        m_pRouter->AddLink(P1,P2);
                  }else
                  {
                     m_pRouter->AddLine(P1,P2);
                  }
               }
               break;            
            default:
               for(int iPin = 0; iPin < pE->m_Con.GetSize(); iPin++)
               {
                  m_pRouter->AddPin(pE->m_Con[iPin]->m_Pos);
               }
            }//switch
         }
         if(RouterAddSelSignals(EF))
         {
            m_pRouter->StartSignalRouting();
            bRoute=true;
         }
      }
   }
   if(!bRoute && m_pRouter)
   {
      delete m_pRouter;
      m_pRouter = NULL;
   }
   return bRoute;
}//CElemPtrList::RouteToSignalInit


/*func*------------------------------------------------------------------------
  crestes the router instance
  - determines the required routing area
  - instanciates the autoRouter
  in :- 
  out:- false: on error
-----------------------------------------------------------------------------*/
bool CElemPtrList::RouterCreate(bool bFull)
{
   CRect    Area;

   CElem*   pE         = NULL;
   POSITION pos        = NULL;

   ASSERT(m_pPinTable);

   if(!bFull)
   {
      Area.SetRectEmpty();
      pos = GetHeadPosition();

      while (pos != NULL)   
      {
         CRect R;
         pE = (CElem*)GetNext(pos); // all distinct points
         pE->GetRect(R,1);
         
         if(Area.left > R.left)
            Area.left = R.left;
         
         if(Area.right < R.right)
            Area.right = R.right;
         
         if(Area.top > R.top)
            Area.left = R.top;

         if(Area.bottom < R.bottom)
            Area.bottom = R.bottom;
      }
      Area.InflateRect(50,50,50,50);
      Area &= CRect(0,0,MAX_XGRID,MAX_YGRID);
   }else
   {
      Area = CRect(0,0,MAX_XGRID,MAX_YGRID);
   }

   if(!m_pRouter)
      m_pRouter = new CAutoRouter( Area );
   ASSERT(m_pRouter);

   return (m_pRouter!=NULL);
}


/*func*------------------------------------------------------------------------
  router: register element positions
  - initializes the routing matrix with the element positions as locked area
  - scans for selected rubber lines (routable ones)
  - call RouterCreate() first
  in :- 
  out:- false: on invalid elem position || no selected rubber line found
-----------------------------------------------------------------------------*/
bool CElemPtrList::RouterAddElems(TElemFlags EF)
{
   POSITION pos;
   CElem* pE;
   bool bSelRubb = false;  // selected rubber lines found

   ASSERT(m_pRouter);
   pos = GetHeadPosition();
   
   if(m_pRouter)
   {
      m_pRouter->SetLockID();
      while(pos) 
      {
         CRect RBody;
         
         pE = (CElem*) GetNext(pos);
         ASSERT(pE);

         // selected rubber lines found?
         bSelRubb |= pE->ChkFlag(EF) && 
             pE->ChkFlag(EF_RUBBLINE) && 
            (pE->ChkFlag(EF_COMMENT) == m_pRouter->m_bComment);
         
         pE->GetBodyRect(RBody,1);
         
         switch(pE->GetType())
         {
         case ET_ACTION:
         case ET_ACTIONBLOCK:
            m_pRouter->LockRect(RBody);            // RBody == header rect (not filled)
            pE->GetRect(RBody,1);
            m_pRouter->LockRect(RBody);            //not filled
            break;
            
         case ET_LABEL:
            {
               CPoint L,R;
               L.y += pE->GetSize().cy/2;
               R.y = L.y;
               
               CElem* pC = pE->GetContainer();
               
               if(pC)
               {
                  L.x = pC->GetOrigin().x;
                  R.x = pC->GetOrigin().x + pC->GetSize().cx;
               } else
               {
                  L.x = m_pRouter->xMin();
                  R.x = m_pRouter->xMax();
               }
               
               m_pRouter->LockRect(RBody);         // not filled
               RBody.top++;
               RBody.bottom--;
               RBody.left  = L.x;
               RBody.right = R.x;
               m_pRouter->LockRect(RBody);         // lock a line
            }
            break;
         
         case ET_TRANSITION:
            RBody.left = pE->GetOrigin().x+1;
            m_pRouter->LockRect(RBody);
            break;

         case ET_POWERRAIL:
         case ET_ALTDIV:
         case ET_SIMDIV:
         case ET_LINE:
            if(!pE->ChkFlag(EF_RUBBLINE))
            {
               m_pRouter->AddLine(pE->GetOrigin(), pE->GetOrigin()+pE->GetSize());
            }
            break;


         default:
            m_pRouter->LockRect(RBody, true);   // filled
         }// switch
         
         for(int iPin = 0; iPin<pE->m_Con.GetSize(); iPin++)
         {
            CPoint point = pE->m_Con[iPin]->m_Pos;
            bool bHidden = pE->m_Con[iPin]->m_Flag.Chk(PF_HIDDEN);
            switch(pE->GetType())
            {
            case ET_LINE:
               break;

            case ET_POWERRAIL:
               m_pRouter->AddPin(point,USE_Y);
               break;
               
            case ET_ALTDIV:
            case ET_SIMDIV:
               m_pRouter->AddPin(point,USE_X);  // no difference between left/right
               break;                           // since the end points cannot conected horizontally

            default:
               ULONG M = USE_NONE;

               if(bHidden) // assume hidden pins at rect to be connectable from a single direction
               {
                  if(chkValInRange(point.x,RBody.left,RBody.right))
                  {
                     if(chkValAtRange(point.y,RBody.top,RBody.bottom))
                     {
                        M = USE_X;
                     }
                  }else 
                  {
                     if(chkValInRange(point.y,RBody.top,RBody.bottom))
                     {
                        if(chkValAtRange(point.x,RBody.left,RBody.right))
                        {
                           M = USE_Y;
                        }
                     }
                  }
               }

               if(point.x>=RBody.right)
                  M |= USE_XL;
               else if(point.x<=RBody.left)
                  M |= USE_XR;
               else if(point.y<=RBody.top)
                  M |= USE_YB;
               else if(point.y>=RBody.bottom)
                  M |= USE_YT;
               else
                  ASSERT(0);

               m_pRouter->SetPin(point,(UsageMask)M); // overwrites UMask (!)
            }
         }
      }// while
   }
   return bSelRubb;
}


/*func*------------------------------------------------------------------------
 registers selected signals
 - enumerates the routwers signalID
 - requires "RouterAddElems" has been called before
 in :- 
 out:- true: selected signals found  false: no selected signals found
-----------------------------------------------------------------------------*/
bool CElemPtrList::RouterAddSelSignals(TElemFlags EF)  // register all the signals to be routed
{
   CElem *pE;
   CElem *pTE;
   CElemFlag Flags;

   bool bRoute    = false;
   bool bSigFound = true;
   POSITION pos   = GetHeadPosition();
      
   ASSERT(m_pRouter);
   if(m_pRouter)
   {
      ClearFlags(EF_PROCESSED);
      while(pos && bSigFound)
      {
         do {
            pE = (CElem*)GetNext(pos);
            bSigFound = pE->ChkFlag(EF) && 
               !pE->ChkFlag(EF_PROCESSED) && 
               (pE->ChkFlag(EF_COMMENT) == m_pRouter->m_bComment) &&
               pE->GetType() == ET_LINE;
         }while( pos && !bSigFound );
         
         if(bSigFound)
         {
            m_pRouter->SetNextID();
            bRoute = true;
            
            Flags = EF_CURRENT;
            Flags += EF_UPDATE;
            ClearFlags(Flags);

            VERIFY( FlagSignal(pE->GetOrigin(),EF_CURRENT, m_pRouter->m_bComment) );
            VERIFY( pTE=MoveFlaggedToTail(EF_CURRENT) );   // returns the 1st flagged
            
            pos = GetTailPosition();
            do {
               pE = (CElem*)GetPrev(pos);
               pE->SetFlag(EF_PROCESSED);
               ASSERT(pE->GetType() == ET_LINE);
               
               if(pE->ChkFlag(EF_RUBBLINE))
               {
                  if( pE->ChkFlag(EF))
                  {
                     TPinEntry* pPE;
                     bool bC;  // comment line(s) present
                     bool bN;  // normal  line(s) present
                     int iE;

                     m_pPinTable->Lookup(pE->GetOrigin(),pPE);
                     ASSERT(pPE!=NULL);

                     if(m_FixPoints.Includes(pPE->m_Pos))
                     {
                        bC = true; // force the point to be routed
                        bN = true;
                     }else
                     {
                        bC = false;  // comment lines
                        bN = false;  // normal lines
                     }
                     for( iE=0; !(bC&&bN) && iE<pPE->m_Elems.GetSize(); iE++)
                     {
                        if(pPE->m_Elems[iE]->ChkFlag(EF_RUBBLINE) &&
                           pPE->m_Elems[iE]->ChkFlag(EF_COMMENT))
                           bC = true;
                        else
                           bN = true;
                     }
                      // not only rubber lines OR both comment AND normal rubber
                     if(pPE->m_iConnects != pPE->m_iRubbLines || bC&&bN )
                     {
                        m_pRouter->AddToDo(pE->GetOrigin());
                     }
                     
                     m_pPinTable->Lookup(pE->GetOrigin()+pE->GetSize(),pPE);
                     ASSERT(pPE!=NULL);
                     
                     if(m_FixPoints.Includes(pPE->m_Pos))
                     {
                        bC = true; // force the point to be routed
                        bN = true;
                     }else
                     {
                        bC = false;  // comment lines
                        bN = false;  // normal lines
                     }
                     for( iE=0; !(bC&&bN) && iE<pPE->m_Elems.GetSize(); iE++)
                     {
                        if(pPE->m_Elems[iE]->ChkFlag(EF_RUBBLINE) &&
                           pPE->m_Elems[iE]->ChkFlag(EF_COMMENT)     )
                           bC = true;
                        else
                           bN = true;
                     }
                      // not only rubber lines OR both comment AND normal rubber
                     if(pPE->m_iConnects != pPE->m_iRubbLines || bC&&bN )
                     {
                        m_pRouter->AddToDo(pE->GetOrigin()+pE->GetSize());
                     }
                     
                     DeleteElem(pE);
                  }else
                  {
                     if(!pE->ChkFlag(EF_COMMENT))
                        m_pRouter->AddLink(pE->GetOrigin(),pE->GetOrigin()+pE->GetSize());
                  }
               }else
               {
                  m_pRouter->AddLine(pE->GetOrigin(),pE->GetOrigin()+pE->GetSize());
               }
               
            }while( (pE != pTE) && pos);
            pos = GetHeadPosition();
         }
      }
   }
   return bRoute;
}

/*func*------------------------------------------------------------------------
 routes the selected rubber lines
 - build up the point list for routing
 - calls the router (if not ready)
 - routes a single line of a signal and returns
 in :-
 out:- true: not ready fasle: nothing to route left
-----------------------------------------------------------------------------*/
bool CElemPtrList::RouteFlaggedLines(TElemFlags Flag)
{
   bool bCallAgain = false;
   ASSERT(m_pRouter);
   
   int n=50;   // view refresh-interval

   if(m_pRouter)
   {
      do{
         bCallAgain = m_pRouter->RoutingStep();
         // insert routed polyline
         CPointArray PLine;
         int iMax = 50000;
         while(m_pRouter->GetNextResult(PLine) && (iMax-->0))
         {
            if(PLine.GetSize()==1)
            {
               BreakLines(PLine[0]);
               BreakLines(PLine[0]);
            }else
            {
               for(int i=0; i<PLine.GetSize()-1; i++)
               {
                  // horizontal or vertical line (not diagonal)
                  ASSERT((PLine[i].x == PLine[i+1].x) || (PLine[i].y == PLine[i+1].y));
               
                  CElem* pE = new CElemLine(PLine[i],PLine[i+1],false);
                  pE->SetFlag(EF_PROCESSED);
                  pE->SetFlag(EF_UPDATE);
                  pE->SetFlag(Flag);
               
                  if(m_pRouter->m_bComment)
                     pE->SetFlag(EF_COMMENT);
               
                  InsertElem(pE);
               }
            }
         }
         ASSERT(iMax>0);   // timeout
         
      }while(--n && bCallAgain);
   }
   return bCallAgain;
}

/*func*------------------------------------------------------------------------
 insert rubber lines for unroutable points;
 kill the router
-----------------------------------------------------------------------------*/
void CElemPtrList::RouteToSignalDone(TElemFlags EF)
{
   if(m_pRouter)
   {
      CPointArray PLine;
      while(m_pRouter->GetNextRubber(PLine))
      {
         ASSERT(PLine.GetSize() == 2);
         
         CElem* pE = new CElemLine(PLine[0],PLine[1],true);
         pE->SetFlag(EF_PROCESSED);
         pE->SetFlag(EF_UPDATE);
         pE->SetFlag(EF);
         
         if(m_pRouter->m_bComment)
            pE->SetFlag(EF_COMMENT);
         
         InsertElem(pE);
      }
      delete m_pRouter;
      m_pRouter = NULL;
   }
   ConnectAllElems();
   CreateTempVars(false);
}

/*func*------------------------------------------------------------------------
 remove all EF_EDITLINE flags; 
 kill the router
-----------------------------------------------------------------------------*/
void CElemPtrList::RouteToPointDone()
{
   if(!m_pRouter)
      return;

   ClearFlags(EF_EDITLINE);
   ConnectAllElems();
   CreateTempVars(false);
   ClearConflictingTempVars();

   if(m_pRouter)
   {
      delete m_pRouter;
      m_pRouter = NULL;
   }
}

/*func*------------------------------------------------------------------------
 initializes the start point for incremental routing
 in :- origin point
 out:- true: valid point position   false: invalid dest point
-----------------------------------------------------------------------------*/
bool CElemPtrList::RouteToPointInit(CPoint P)
{
   CElem* pET;
   CElem* pE;
   if(RouterCreate(true))
   {
      ASSERT(m_pRouter);

      // remove all temp vars
      DeleteFlaggedElems(EF_AUTOELEM);

      m_pRouter->SetLockID();          // locked area
      RouterAddElems(EF_SELECT);       // using SignalID 1

      // init pins
      CElem* pESig = NULL;
      ClearFlags(EF_UPDATE);

      m_pRouter->SetNextID();          // signal 2,ff.: all the lines & pins
                                       // for integrity test only
      POSITION pos = GetHeadPosition(); 
      while(pos)
      {
         pE = (CElem*)GetNext(pos);

         bool bSigElem = (pE->GetType() == ET_LINE);
         bSigElem |= (pE->GetType() == ET_ALTDIV);
         bSigElem |= (pE->GetType() == ET_SIMDIV);
         bSigElem |= (pE->GetType() == ET_POWERRAIL);

         if(bSigElem)
         {
            CPoint P1 = pE->GetOrigin();
            CPoint P2 = P1 + pE->GetSize();

            if(pE->ChkFlag(EF_RUBBLINE))
            {
               m_pRouter->AddLink(P1,P2);
            }else
            {
               m_pRouter->AddLine(P1,P2);
            }
         }

         if(pE->GetType() != ET_LINE)
         {
            for(int iPin = 0; iPin < pE->m_Con.GetSize(); iPin++)
            {
               m_pRouter->AddPin(pE->m_Con[iPin]->m_Pos);
            }
         }
      }
      m_pRouter->SetNextID();
      TPinEntry *pPinEntry;
      if(m_pPinTable && m_pPinTable->Lookup(P,pPinEntry))
      {
         if(FlagSignal(P,EF_UPDATE))
         {
            pET = MoveFlaggedToTail(EF_UPDATE);
            pos = GetTailPosition();
            pE  = NULL;
            while(pos && pE!=pET )
            {
               pE = (CElem*)GetPrev(pos);
//               if(pE == pET)
//                  break;

               ASSERT(pE->GetType() == ET_LINE);
               CPoint P1 = pE->GetOrigin();
               CPoint P2 = P1 + pE->GetSize();

               if(pE->ChkFlag(EF_RUBBLINE))
               {
                  m_pRouter->AddLink(P1,P2);
               }else
               {
                  m_pRouter->AddLine(P1,P2);
               }
            }
         }
      }
      TPinEntry * pPE;
      if(m_pPinTable && m_pPinTable->Lookup(P,pPE))
      {
         ASSERT(pPE);
         bool bFound = false; // scan for non-comment elements
         for(int i=0; !bFound && (i<pPE->m_Elems.GetSize()); i++)
               bFound = !pPE->m_Elems[i]->ChkFlag(EF_COMMENT);

         m_pRouter->m_bComment = !bFound && (pPE->m_Elems.GetSize()>0);
      }else
      {
         if(m_pPinTable)
         {
            CElem *pE = SearchElem(P,1);
            if(pE)
               m_pRouter->m_bComment = pE->ChkFlag(EF_COMMENT);
         }else
            m_pRouter->m_bComment = false;
      }

      m_pRouter->StartPointRouting(P);
      return true;
   }
   return false;
}


/*func*------------------------------------------------------------------------
 routes from StartPoint (must be set before) to the argument point
 - build up the point list for routing
 - 
 in :- destination point
 out:- true: connected - see PolyLine false: invalid dest point
-----------------------------------------------------------------------------*/
bool CElemPtrList::RouteToPoint(CPoint P)
{
   CPointArray PLine;
   DeleteFlaggedElems(EF_EDITLINE);

   if(!m_pRouter)
      return false;

   if(m_pRouter->RouteToPoint(P))  // if routable
   {
      if(m_pRouter->GetNextResult(PLine))
      {
         ASSERT(PLine.GetSize() >= 2);

         for(int idx=0; idx<PLine.GetSize()-1;idx++)  // add fixed poly-line
         {
            ASSERT(PLine[idx].x == PLine[idx+1].x || PLine[idx].y == PLine[idx+1].y);

            CElem* pE = new CElemLine(PLine[idx],PLine[idx+1],false);

            pE->SetFlag(EF_UPDATE);
            pE->SetFlag(EF_EDITLINE);

            if(m_pRouter->m_bComment)
               pE->SetFlag(EF_COMMENT);

            InsertElem(pE);
         }
      }
   }else    // unroutable
   {
      // insert rubber lines instead
      if(m_pRouter->GetNextRubber(PLine))
      {
         CElem* pE = new CElemLine(PLine[0],PLine[1],true); // insert rubber line
         pE->SetFlag(EF_UPDATE);
         pE->SetFlag(EF_EDITLINE);

         if(m_pRouter->m_bComment)
            pE->SetFlag(EF_COMMENT);
         
         InsertElem(pE);
         ASSERT(!m_pRouter->GetNextRubber(PLine));
      }else
      {
      }
   }
   return PLine.GetSize()>0;
}//CElemPtrList::RouteToPoint


/*func*------------------------------------------------------------------------
 preprocess rubber lines connected to PowerRail or DivCon
 - moves endpoint that cx||cy == 0 (if possible)
 in :- destination point
 out:- true: connected - see PolyLine false: invalid dest point
-----------------------------------------------------------------------------*/
void CElemPtrList::RouterTrackRubber(TElemFlags EF)
{
   POSITION pos;
   int iPin;

   if(!m_pPinTable)
      return;

   for(pos = GetHeadPosition(); pos; )                                  // for each element
   {
      CElem *pE = (CElem*) GetNext(pos);
      ASSERT_VALID(pE);
      switch(pE->GetType())
      {
      case ET_LINE:  
         if(pE->ChkFlag(EF_RUBBLINE))
         {
            
         }
         break;
      case ET_ALTDIV:
      case ET_SIMDIV:
         for(iPin = 0; iPin < pE->m_Con.GetSize(); iPin++)              // for each pin
         {
            TPinEntry* pPE;
            CPoint P1 = pE->m_Con[iPin]->m_Pos;
            if(m_pPinTable->Lookup(P1,pPE) && (pPE->m_iRubbLines>0))
            {
               for(int i=0; i<pPE->m_Elems.GetSize(); i++)              // for each connected element
               {
                  CElem *pTE=pPE->m_Elems[i];
                  CSize Size = pTE->GetSize();
                  
                  if(pTE->ChkFlag(EF_RUBBLINE) && Size.cx && Size.cy )
                  {
                     CPoint P2 = pTE->GetOrigin()!=P1 ? pTE->GetOrigin() :P1+pTE->GetSize();
                     bool bCan;
                     if(pE->GetSize().cx>0)
                        bCan = chkValAtRange(P2.x, pE->GetOrigin().x, pE->GetOrigin().x+pE->GetSize().cx);
                     else
                        bCan = chkValAtRange(P2.x, pE->GetOrigin().x+pE->GetSize().cx, pE->GetOrigin().x);

                     bCan &= (pE->GetContainer() == pTE->GetContainer());

                     CPoint P3(P2.x,P1.y);
                     CPin *pPin = pE->SearchConn(P3);

                     if(bCan && pPin )
                     {
                        switch(pPin->GetConnects())
                        {
                        case 1: break;
                        case 2:
                           {
                              CElem* pTE = pPin->m_Entry->m_Elems[0];
                              if(pTE==pE)
                                 pTE = pPin->m_Entry->m_Elems[1];
                              int Y = pE->GetOrigin().y;
                              if(P2.y > Y)  // rubber line is below the DivCon
                              {
                                 bCan = (pTE->GetOrigin().y < Y);
                                 bCan |=(pTE->GetOrigin().y+pTE->GetSize().cy < Y);
                              }else
                              {
                                 bCan = (pTE->GetOrigin().y > Y);
                                 bCan |=(pTE->GetOrigin().y+pTE->GetSize().cy > Y);
                              }
                           }
                           break;
                        default:
                           bCan = false;
                        }
                     }else
                     {
                        bCan = false;
                     }

                     if(bCan)
                     {
                        bool bComment = pTE->ChkFlag(EF_COMMENT);
                        DeleteElem(pTE);
                        pTE = new CElemLine(P3,P2,true);
                        pTE->SetFlag(EF);
                        if(bComment)
                           pTE->SetFlag(EF_COMMENT);
                        InsertElem(pTE);
                     }
                  }
               }//for each connected element
            }
         }//for each pin
         break;

      case ET_POWERRAIL:
         for(iPin = 0; iPin < pE->m_Con.GetSize(); iPin++)              // for each pin
         {
            TPinEntry* pPE;
            CPoint P1 = pE->m_Con[iPin]->m_Pos;
            if(m_pPinTable->Lookup(P1,pPE) && (pPE->m_iRubbLines>0))
            {
               for(int i=0; i<pPE->m_Elems.GetSize(); i++)              // for each connected element
               {
                  CElem *pTE=pPE->m_Elems[i];
                  CSize Size = pTE->GetSize();
                  
                  if(pTE->ChkFlag(EF_RUBBLINE) && Size.cx && Size.cy )
                  {
                     CPoint P2 = pTE->GetOrigin()!=P1 ? pTE->GetOrigin() :P1+pTE->GetSize();
                     bool bCan;
                     if(pE->GetSize().cy>0)
                        bCan = chkValAtRange(P2.y, pE->GetOrigin().y, pE->GetOrigin().y+pE->GetSize().cy);
                     else
                        bCan = chkValAtRange(P2.y, pE->GetOrigin().y+pE->GetSize().cy, pE->GetOrigin().y);

                     bCan &= (pE->GetContainer() == pTE->GetContainer());

                     CPoint P3(P1.x,P2.y);
                     CPin *pPin = pE->SearchConn(P3);
                     bCan &= (pPin && pPin->GetConnects()==1);

                     if(bCan)
                     {
                        ASSERT(P3.y == P2.y);
                        DeleteElem(pTE);
                        pTE = new CElemLine(P3,P2,true);
                        pTE->SetFlag(EF);
                        InsertElem(pTE);
                     }
                  }
               }//for each connected element
            }
         }//for each pin
         break;
      }//switch
   }//for each element
}

/*
// rebuild *pSigTab (insert only signals containing at least one line )
// uses EF_UPDATE, EF_PROCESSED
void CElemPtrList::BuildSignalTab(CSignalTab* pSigTab)
{
   CElemFlag Flags;
   POSITION  pos = NULL;

   ASSERT(pSigTab);

   if(m_pPinTable && pSigTab)
   {
      pSigTab->RemoveAll();
//      Flags = EF_PROCESSED;
      Flags = EF_UPDATE;   // used by FlagSignal
      Flags +=EF_CURRENT;
      ClearFlags(Flags);

      CSignal Sig;
      CElem* pE = NULL;
      pos = GetHeadPosition();
      while(pos )
      {
         Sig.RemoveAll();
         pE =(CElem*)GetNext(pos);
         ASSERT(pE);
         bool bFlagIt = !pE->ChkFlag(EF_RUBBLINE)  && 
                        !pE->ChkFlag(EF_CURRENT)   && 
                        !pE->ChkFlag(EF_UPDATE);

         if(pE->GetType() == ET_LINE && bFlagIt )
         {
            if( FlagSignal(pE->GetOrigin(),EF_CURRENT, false) )   // non-comment
            {
               CElem* pEStart;
               pEStart = MoveFlaggedToTail(EF_CURRENT);
               ASSERT(pEStart);
               
               POSITION pos2 = GetTailPosition();
               while(pos)
               {
                  pE = (CElem*)GetPrev(pos);
                  ASSERT(pE->ChkFlag(EF_CURRENT));
                  Sig.Add(pE);
                  if(pE == pEStart)
                     break;
               }
               pSigTab->InsertSignal(Sig);
               ClearFlags(EF_CURRENT);
               pos = GetHeadPosition();
            }
         }
      }
   }
}
*/

void CElemPtrList::SignalChangeInit(bool bActivate)
{
   m_bCountSignalChanges = bActivate;
}


int CElemPtrList::SignalChangeCount()
{
   int iChanges;
   CPointSet Points;
   int iElems = 0;   // counts selected elements

   if(m_bCountSignalChanges)
   {
      ASSERT(m_pPinTable);
      
      TPinEntry *pPE;
      POSITION   pos;
      CPoint     key;
   
      pos = m_pPinTable->GetStartPosition();
      while(pos)
      {
         m_pPinTable->GetNextAssoc(pos, key, pPE);
         ASSERT(pPE);
         if(pPE->m_iDeSelected && pPE->m_iSelected)
         {
            Points.Add(pPE->m_Pos); // always unique
         }
      }
      
      if(Points.Size()>1)
      {
         for(POSITION pos = GetHeadPosition(); pos;)
         {
            CElem* pTE = (CElem*) GetNext(pos);
            if(pTE->ChkFlag(EF_SELECT))
            {
               iElems ++;
               if(iElems >1)
                  break;
            }
         }
      }
   }
   
   iChanges = 0;
   if(iElems>1 && Points.Size()> 0)    // number of involved elements >1 &&
   {                                   // number of affected points > 0
      ClearFlags(EF_UPDATE);
      int idx = 0;
      int N = Points.Size();  // store (size is changing)
      for(int i=0; i<N && Points.GetSize()>0; i++)
      {
         CPointSet PSet;
         ClearFlags(EF_CURRENT);

         GetSignalPoints(Points[0], PSet, false);

#define SIGNAL_CHECK_V2
#ifdef  SIGNAL_CHECK_V2
         int iSel = 0;
         int iUns = 0;

         for(int j=0; j<PSet.Size(); j++)                // for each signal point...
         {
            TPinEntry* pPE;

            m_pPinTable->Lookup(PSet[j],pPE);            
            for(int k=0; k<pPE->m_Elems.GetSize(); k++)  // for each elem at signal point[j]
            {
               CElem* pE = pPE->m_Elems[k];
               if(pE->GetType() != ET_LINE)     // count selected && unselected non-lines
               {
                  if(pE->ChkFlag(EF_SELECT))
                  {
                     iSel++;
                  }else
                  {
                     iUns++;
                  }
               }
            }
            if(iUns>0 && pPE->m_iSelected>1)
            {
               iChanges++;
            }
         }
#endif
         // uncomment the next line if signals containing nothing but lines have to be ignored
         if(iSel>0)   // only if selected elemets are inside the signal too
         iChanges++;

         Points.Subtract(PSet);
//         N = Points.Size();
         idx = 0;//restart
      }
   }
   return iChanges;
}

/*func*------------------------------------------------------------------------
 create/delete temp vars at open FBLK-Pins inside/outside LD-areas
 in: bool bCreate - true: be active (create/remove) false: mark temp vars only
-----------------------------------------------------------------------------*/
void CElemPtrList::CreateTempVars(bool bCopySelection)
{
   POSITION pos = GetHeadPosition();
   while(pos)
   {
      CElem* pE = (CElem*)GetNext(pos);
      if(pE->GetType() == ET_FB        ||
         pE->GetType() == ET_PROCEDURE ||
         pE->GetType() == ET_FUNCTION    )
      {
         if(((CElemFblk*)pE)->WithENOPins()) // LD-area
         {         
            int xc = pE->GetOrigin().x + pE->GetSize().cx/2;
            for(int iPin = 0; iPin < pE->m_Con.GetSize(); iPin++)
            {
               if( pE->m_Con[iPin]->m_Flag.Chk(PF_LADDER) ||   // skip EN/ENO pins
                  !pE->m_Con[iPin]->m_Flag.Chk(PF_AUTOVAR)  )  // skip TempVar protected pins
                  continue;

               CPoint P    = pE->m_Con[iPin]->m_Pos;
               bool bLeft  = (P.x < xc);
               const TPinEntry *pPE = pE->m_Con[iPin]->GetPinEntry();
               
               ASSERT(pPE);

               if(pPE->m_iConnects<2)  // nothing connected: try to create temp var
               {
                  CElem* pTE = NULL;
                  if(bLeft)
                  {
                     pTE = new CElemVarIn(P-CSize(4,1),CSize(4,2), VAR_AUTONAME);
                  }else
                  {
                     pTE = new CElemVarOut(P-CSize(0,1),CSize(4,2), VAR_AUTONAME);
                  }

                  pTE->SetFlag(EF_AUTOELEM);
                  pTE->SetFlag(EF_UPDATE);
                  /*
                  if(pE->ChkFlag(EF_SELECT) && bCopySelection)
                  {
                     pTE->SetFlag(EF_SELECT);
                  }
                  */
                  InsertElem(pTE);
                  if(IsElemOverlapped(pTE))  // check for conflicts
                  {
                     DeleteElem(pTE);
                  }
               }else
               {  // update OVL-Test for connected elements
                  for(int i=0; i<pPE->m_Elems.GetSize(); i++)
                  {
                     if( pPE->m_Elems[i] != pE)
                     {
                        pPE->m_Elems[i]->ChkFlag(EF_EMPHASIZE);
                        IsElemOverlapped(pPE->m_Elems[i]);
                        break;
                     }
                  }
               }

               // copy selection to connected elements (except lines)
               if(bCopySelection && pE->ChkFlag(EF_SELECT))    // FBLK is selected?
               {
                  for(int i=0; i<pPE->m_Elems.GetSize(); i++)  // for each connected elem
                  {
                     if(pPE->m_Elems[i]->GetType()!=ET_LINE && // ignore lines
                       !pPE->m_Elems[i]->ChkFlag(EF_AUTOELEM)) // ignore auto-elements
                     {
                        pPE->m_Elems[i]->SetFlag(EF_SELECT);   // copy selection from FBLK
                     }
                  }
               }
            }
         }
      }else // non-FBLK
      {
         if(pE->ChkFlag(EF_AUTOELEM))  // remove EF_AUTOELEM flag if its name was changed
         {
            pE->SetFlag(EF_UPDATE);
            ASSERT(pE->m_Con.GetSize()>0);
            ASSERT(pE->GetType()==ET_VARIN || pE->GetType()==ET_VAROUT );

            if(((CElemVar*)pE)->GetName() != VAR_AUTONAME)
            {
               pE->ClrFlag(EF_AUTOELEM);
            }
         }
      }
   }//while
}

/*func*------------------------------------------------------------------------
 set EF_UPDATE on each element that may change its layout on language changes
-----------------------------------------------------------------------------*
void CElemPtrList::MarkAutoElems()
{
   for(POSITION pos = GetHeadPosition(); pos; )
   {
      CElem* pE = (CElem*)GetNext(pos);
      if(pE->ChkFlag(EF_AUTOELEM))
      {
         pE->SetFlag(EF_UPDATE);
      }else
      {
         if(pE->GetType() == ET_FB        ||    // CElemFblk based?
            pE->GetType() == ET_PROCEDURE ||
            pE->GetType() == ET_FUNCTION    )
         {
            pE->SetFlag(EF_UPDATE);
         }
      }
   }
}
*/

/*func*------------------------------------------------------------------------
 void CElemPtrList::PurgeTempVars()
 - removes conflicting temp vars at FBLKs without EN/ENO pins
 - removes unconnected temp vars
-----------------------------------------------------------------------------*/
void CElemPtrList::PurgeTempVars()
{
   POSITION pos = GetHeadPosition();
   while(pos)
   {
      CElem* pE = (CElem*)GetNext(pos);
      if(pE->GetType() == ET_FB        ||    // CElemFblk based?
         pE->GetType() == ET_PROCEDURE ||
         pE->GetType() == ET_FUNCTION    )
      {
         if(!((CElemFblk*)pE)->WithENOPins())
         {  // remove temp vars if FBLK is outside LD area
            for(int iPin = 0; iPin < pE->m_Con.GetSize(); iPin++)
            {
               const TPinEntry *pPE = pE->m_Con[iPin]->GetPinEntry();
               CPoint P    = pE->m_Con[iPin]->m_Pos;
               
               if(pPE)
               {  
                  ASSERT(pPE);
                  if(pPE->m_iConnects > 1)  // anything connected
                  {
                     for(int i=0; i<pPE->m_Elems.GetSize(); i++)
                     {
                        CElem* pTE = pPE->m_Elems[i];
                        if(pTE->ChkFlag(EF_AUTOELEM))
                        {
                           DeleteElem(pTE);
                           pos = GetHeadPosition();
                        }
                     }//for
                  }
               }
            }//for
         }
      }else
      {  // remove (unselected && unconnected) temp vars
         if(pE->ChkFlag(EF_AUTOELEM))
         {
            bool bDel = pE->ChkFlag(EF_EMPHASIZE);    // conflicting temp var
            bDel |= (pE->m_Con[0]->GetConnects()==1); // unconnected temp var
            if(pE->m_Con[0]->GetConnects()==2)
            {
               CPin *pPin = NULL;
               const TPinEntry* pPE = pE->m_Con[0]->GetPinEntry();
               if(pPE->m_Elems[0]->GetType() == ET_FB          ||
                  pPE->m_Elems[0]->GetType() == ET_FUNCTION    ||
                  pPE->m_Elems[0]->GetType() == ET_PROCEDURE      )  
               {
                  pPin = pPE->m_Elems[0]->SearchConn(pPE->m_Pos);
               }else if(pPE->m_Elems[1]->GetType() == ET_FB          ||
                        pPE->m_Elems[1]->GetType() == ET_FUNCTION    ||
                        pPE->m_Elems[1]->GetType() == ET_PROCEDURE      ) 
               {
                  pPin = pPE->m_Elems[1]->SearchConn(pPE->m_Pos);
               }

               if(pPin) // kill temp vars at EN/ENO pins (may occur on moving/resizing FBLKs)
               {
                  if(pPin->m_Name.Str() == _T("EN")||pPin->m_Name.Str() == _T("ENO"))
                  {
                     bDel = true;
                     ASSERT(pPin->m_Flag.Chk(PF_LADDER));
                  }
               }
            }else if(pE->m_Con[0]->GetConnects()>2)   // anything elss connected
            {
               bDel = true;
            }

            if(bDel)
            {
               DeleteElem(pE);
            }
         }
      }
   }
}

void CElemPtrList::ClearConflictingTempVars()
{
   CElemFlag EF = EF_AUTOELEM;
   EF += EF_EMPHASIZE;

   DeleteFlaggedElems(EF);
   ClearFlags(EF_EMPHASIZE);
}

/*func*------------------------------------------------------------------------
void CElemPtrList::RecoverTempVars()
 - recovers EF_AUTOELEM flag on temp vars after loding
-----------------------------------------------------------------------------*/
void CElemPtrList::RecoverTempVars()
{
   for(POSITION pos = GetHeadPosition(); pos; )
   {
      CElem*      pE = (CElem*)GetNext(pos);
      ELEM_TYPE Type = pE->GetType();

      if( Type == ET_VARIN || Type == ET_VAROUT)
      {
         if( ((CElemVar*)pE)->GetName() == VAR_AUTONAME)
         {
            pE->SetFlag(EF_AUTOELEM);
         }
      }
   }
}


/*func*------------------------------------------------------------------------
void CElemPtrList::MarkTempVarPins()
 - flags the temp var positions (used to mark possible generation points)
-----------------------------------------------------------------------------*/
void CElemPtrList::FlagTempVarPins()
{
   POSITION pos = GetHeadPosition();
   while(pos)
   {
      CElem* pE = (CElem*) GetNext(pos);
      ELEM_TYPE ET = pE->GetType();

      if(ET == ET_FB || ET == ET_FUNCTION || ET == ET_PROCEDURE)
      {
         if(((CElemFblk*)pE)->WithENOPins())
         {
            CPin* pPin = NULL;
            for(int i=0; i<pE->m_Con.GetSize(); i++)                    // for each pin of the FB
            {
               pPin = pE->m_Con[i];
               const TPinEntry* pPE = pPin->GetPinEntry();
               bool bFound = false;
               
               for(int j=0; !bFound && j<pPE->m_Elems.GetSize(); j++)   // for each pin
               {
                  if(pPE->m_Elems[j]->ChkFlag(EF_AUTOELEM))             // AutoVar connected?
                  {
                     bFound = true;
                     break;
                  }
               }
               
               if(bFound)
               {
                  pPin->m_Flag.Set(PF_AUTOVAR);
               }else
               {
                  pPin->m_Flag.Clr(PF_AUTOVAR);
               }
            }
         }else // element has no ENO
         {
            for(int i=0; i<pE->m_Con.GetSize(); i++)                    // for each pin of the FB
            {
               pE->m_Con[i]->m_Flag.Set(PF_AUTOVAR);
            }
         }
      }
   }
}

/*func*------------------------------------------------------------------------
  GetIECLanguange
    returns the current set IEC-sheet-language (SFC,LD,FBD)
  IEC_GR_Lang 
-----------------------------------------------------------------------------*/
IEC_GR_Lang CElemPtrList::GetIECLanguange()
{
   return m_IECLang;
}

/*func*------------------------------------------------------------------------
  
    returns the current set IEC-sheet-language (SFC,LD,FBD)
  IEC_GR_Lang 
-----------------------------------------------------------------------------*/
/*func*------------------------------------------------------------------------
  GetIECLanguange in this point 
  in :p - coordinates of point in logical coords for search element
      iGrid - actual zoom
  out:IEC_GR_Lang
-----------------------------------------------------------------------------*/
IEC_GR_Lang CElemPtrList::GetIECLanguange(CPoint& p, int iGrid)
{
   CElem *pE;

   switch (m_IECLang)
   {
   case LANG_LD:
   case LANG_FBD:
      return m_IECLang;

   default:
   case LANG_SFC:
      pE = SearchElem(p, iGrid);
      if (pE && pE->GetContainer())
      {
         if (pE->GetContainer()->ChkFlag(EF_LDACTION))
         {
            return LANG_LD;
         } else if (pE->GetContainer()->ChkFlag(EF_STACTION))
         {
            return LANG_ST;  // oops - graphics draws in st =8-o
         } else {
            return LANG_FBD;
         }
         // hierarchical sfc are not allowed
      }
      return m_IECLang;
   }
}

/*func*------------------------------------------------------------------------
  SetIECLanguage
    changed the IEC-language of the sheet (updates default EN/ENOs at FBLKs)
  IEC_GR_Lang   Language
-----------------------------------------------------------------------------*/
void CElemPtrList::SetIECLanguage(IEC_GR_Lang Language)
{
   m_IECLang = Language;
   bool bENO = (m_IECLang == LANG_LD);
   
   POSITION pos = GetHeadPosition();
   while (pos)
   {
      CElem *pE = (CElem*)GetNext(pos);
      if(pE->GetType() == ET_FB        ||    // CElemFblk based?
         pE->GetType() == ET_PROCEDURE ||
         pE->GetType() == ET_FUNCTION    )
      {
         ((CElemFblk*)pE)->SetENODefault(bENO);
         ((CElemFblk*)pE)->OnAutoENO();
      }
   }
}

/*func*------------------------------------------------------------------------
  RecoverIECLang
 - detects the IEC-language by looking for typical elements
-----------------------------------------------------------------------------*/
void CElemPtrList::RecoverIECLang()
{
   IEC_GR_Lang Lang = LANG_FBD;
   POSITION pos = GetHeadPosition();
   bool LD  = false;
   bool SFC = false;

   while(pos)
   {
      CElem *pE = (CElem*)GetNext(pos);
      switch(pE->GetType())
      {
/*
      // types not belonging to LD or SFC
      case ET_VARIN:
      case ET_VAROUT:
      case ET_VARINOUT:
      case ET_COMMENT:
      case ET_CONIN:
      case ET_CONOUT:
      case ET_CONST:
      case ET_LABEL:
      case ET_JUMP:
      case ET_LINE:
      case ET_RETURN:
*/
      case ET_ACTION:
      case ET_ACTIONBLOCK:
      case ET_ALTDIV:
      case ET_SIMDIV:
      case ET_STEP:
      case ET_TRANSITION:
         SFC = true;
         break;

      case ET_POWERRAIL:
      case ET_CONTACT:
      case ET_COIL:
         LD = true;
         break;

      case ET_FB:
      case ET_FUNCTION:
      case ET_PROCEDURE:
         {
            if( ((CElemFblk*)pE)->WithENOPins() )
               LD = true;
         }
      }
   }

   if(SFC)
   {
      SetIECLanguage(LANG_SFC);
   }else if(LD)
   {
      SetIECLanguage(LANG_LD);
   }else
   {
      SetIECLanguage(LANG_FBD);
   }
}



/*func*------------------------------------------------------------------------
bool CElemPtrList::ToggleFixPoint(CPoint At)
 - toggles fix points (only at lines)
-----------------------------------------------------------------------------*/
bool CElemPtrList::ToggleFixPoint(CPoint At)
{
   CElem* pE = SearchElem(At,1);
   if(pE && pE->GetType()==ET_LINE && !pE->ChkFlag(EF_RUBBLINE))
   {
      if(m_FixPoints.Std.Toggle(At))
      {
         BreakLines(At);
      }else
      {
         LinkLines(At);
      }
      return true;
   }
   return false;
}



/*func*------------------------------------------------------------------------
bool CElemPtrList::GetNextFixPoint(int& idx, CPoint& P)
 - gets next stop point position (1st stop point is at position 0)
 - if the returned <idx> is <0, the fix point is in the working set
RET: if <idx> is valid <idx>, <P> are written and true is returned (else:false)
-----------------------------------------------------------------------------*/
bool CElemPtrList::GetNextFixPoint(int& idx, CPoint& P)
{
   return m_FixPoints.Next(idx,P);
}


void CElemPtrList::InitMoveFixPoints()
{
   CPointSet DelSet; // fix points to be removed

   m_FixPoints.Std.Add(m_FixPoints.Cur);
   m_FixPoints.Cur.RemoveAll();

   // remove fix points at invalid positions...
   for(int i=0; i<m_FixPoints.Std.Size(); i++)
   {
      CPoint P  = m_FixPoints.Std[i];
      CElem* pE = SearchElem(P,1);

      if(!pE || 
         pE->GetType()!=ET_LINE ||              // not a line  OR
         pE->ChkFlag(EF_RUBBLINE) &&            // a rubberline && not at any pin
         !( P==pE->GetOrigin() || 
            P==pE->GetOrigin()+pE->GetSize()
         ))
      {
         DelSet.Add(P);
      }else if(pE && pE->GetType()==ET_LINE && !pE->ChkFlag(EF_RUBBLINE))
      {
         if(pE->ChkFlag(EF_SELECT))             // fix point is at selected, fixed line
         {
            m_FixPoints.Cur.Add(P);             // copy to move-set
         }
      }
   }

   m_FixPoints.Std.Subtract(m_FixPoints.Cur);
   m_FixPoints.Std.Subtract(DelSet);
}


/*func*------------------------------------------------------------------------
void CElemPtrList::DoneMoveFixPoints()
 - join fix point sets after move operation
-----------------------------------------------------------------------------*/
void CElemPtrList::DoneMoveFixPoints()
{
   if(m_FixPoints.Cur.Size()>0)
   {
      m_FixPoints.Std.Add(m_FixPoints.Cur);
      m_FixPoints.Cur.RemoveAll();
   }
}


/*func*------------------------------------------------------------------------
bool CElemPtrList::MoveFixPoints(CSize delta)
 - removes fix point that are not located at any fixed line
 - 
RET: true: changes made; false: nothing changed
-----------------------------------------------------------------------------*/
void CElemPtrList::MoveFixPoints(CSize delta)
{
#ifdef _DEBUG
   int N = m_FixPoints.Std.Size() + m_FixPoints.Cur.Size();
#endif
   CPointSet Moved;
   for( int i = 0; i< m_FixPoints.Cur.Size(); i++)
   {
      Moved.Add(m_FixPoints.Cur[i]+delta);
   }
   m_FixPoints.Cur.RemoveAll();
   m_FixPoints.Cur.Add(Moved);

   ASSERT( N == m_FixPoints.Std.Size() + m_FixPoints.Cur.Size() );
}
