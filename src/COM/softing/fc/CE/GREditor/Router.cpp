

#include "stdafx.h"
#include "Router.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifndef ASSERT
#define ASSERT(f)          ((void)0)
#endif

#ifndef VERIFY
#define VERIFY(f)          ((void)(f))
#endif

//=============================================================================
//{CRouterBase class implementation

CRouterBase::CRouterBase()
{
   ASSERT(RPVar::CHECK_ASSERTIONS());
   ASSERT(RPFix::CHECK_ASSERTIONS());
   m_SID       = 0;
   m_bDelTemp  = false;
}

CRouterBase::~CRouterBase()
{
   POSITION pos = m_Matrix.m_Link.GetStartPosition();
   POINTS Key;
   CPointsSet *pPL;
   
   while(pos)
   {
      m_Matrix.m_Link.GetNextAssoc(pos, Key, pPL);
      if( pPL )
         delete pPL;
   }
}

void CRouterBase::Init(CRect area)
{
   VERIFY(m_Matrix.m_Fix.Resize( area.left, area.top, area.right, area.bottom, false ));
   VERIFY(m_Matrix.m_Var.Resize( area.left, area.top, area.right, area.bottom, false ));
   m_FIFO.Flush();
   m_Matrix.m_Link.RemoveAll();
   m_SID = RPFix::LockID();
}

void CRouterBase::DelTempDat()
{
   m_Matrix.m_Var.Erase();
   m_FIFO.Flush();
}

void CRouterBase::MarkSignal(POINTS P)                // marks connected points
{
   ASSERT( m_Matrix.m_Fix.IsValidPoint(P) );
   MPoint MP(P,m_Matrix);

   if(!MP.ChkMark())
   {
      if( MP.X_ID()==m_SID || MP.Y_ID()==m_SID )
      {
         MP.SetMark();
         
         if(m_bPush && !((P.x|P.y) & 0x01))  // to be pushed && has even x- AND even y-coord
            m_FIFO.Push(P);
         
         
         UsageMask UMask = MP.GetUMask();
         POINTS PT;

         if( UMask & USE_XR)
         {
            PT = P;
            PT.x++;
            MarkSignal(PT);
         }
         
         if( UMask & USE_XL)
         {
            PT = P;
            PT.x--;
            MarkSignal(PT);
         }
         
         if( UMask & USE_YT)
         {
            PT = P;
            PT.y--;
            MarkSignal(PT);
         }
         
         if( UMask & USE_YB)
         {
            PT = P;
            PT.y++;
            MarkSignal(PT);
         }
      }
      
      CPointsSet* pPL = NULL;
      if(m_Matrix.m_Link.Lookup(P,pPL))
      {
         for(int idx=0; idx< pPL->Size(); idx++)
         {
            POINTS P = (*pPL)[idx];
            //               if(MP.ChkMark())
            MarkSignal(P);
         }
      }
   }
}

void CRouterBase::AddPin(CPoint P, UsageMask UM)
{
   MPoint MP(C2P(P),m_Matrix);
   MP.AddPin(m_SID,UM);
}

void CRouterBase::SetPin(CPoint P, UsageMask UM)
{
   MPoint MP(C2P(P),m_Matrix);
   MP.SetPin(m_SID,UM);
}

void CRouterBase::AddLine(CPoint P1,CPoint P2)
{
   POINTS P;
   if(P1.x == P2.x)  // vertical line
   {
      ASSERT(P1.x == P2.x);
      
      P.x = (SHORT)P1.x;
      if(P1.y < P2.y)
      {
         m_Matrix.m_Fix(C2P(P1)).AddPin(m_SID, USE_YB);
         for(P.y = P1.y+1; P.y < P2.y; P.y+=1)
            m_Matrix.m_Fix(P).Add_Y(m_SID, USE_Y);
         m_Matrix.m_Fix(C2P(P2)).AddPin(m_SID, USE_YT);
      }else
      {
         m_Matrix.m_Fix(C2P(P2)).AddPin(m_SID, USE_YB);
         for(P.y = P2.y+1; P.y < P1.y; P.y+=1)
            m_Matrix.m_Fix(P).Add_Y(m_SID, USE_Y);
         m_Matrix.m_Fix(C2P(P1)).AddPin(m_SID, USE_YT);
      }
   }else             // horizontal line
   {
      ASSERT(P1.y == P2.y);
      
      P.y = (USHORT)P1.y;
      if(P1.x < P2.x)
      {
         m_Matrix.m_Fix(C2P(P1)).AddPin(m_SID, USE_XR);
         for(P.x = P1.x+1; P.x < P2.x; P.x+=1)
            m_Matrix.m_Fix(P).Add_X(m_SID, USE_X);
         m_Matrix.m_Fix(C2P(P2)).AddPin(m_SID, USE_XL);
      }else
      {
         m_Matrix.m_Fix(C2P(P2)).AddPin(m_SID, USE_XR);
         for(P.x = P2.x+1; P.x < P1.x; P.x+=1)
            m_Matrix.m_Fix(P).Add_X(m_SID, USE_X);
         m_Matrix.m_Fix(C2P(P1)).AddPin(m_SID, USE_XL);
      }
   }
}

void CRouterBase::AddLink(CPoint P1,CPoint P2)
{
   CPointsSet *pPS = NULL;
   
   POINTS PP1 = C2P(P1);
   POINTS PP2 = C2P(P2);
   
   AddPin(P1); // end points
   AddPin(P2);
   
   if(!m_Matrix.m_Link.Lookup(PP1,pPS) )
   {
      pPS = new CPointsSet;
      m_Matrix.m_Link[PP1] = pPS;
   }
   pPS->Add(PP2);
   
   if(!m_Matrix.m_Link.Lookup(PP2,pPS) )
   {
      pPS = new CPointsSet;
      m_Matrix.m_Link[PP2] = pPS;
   }
   pPS->Add(PP1);
}

void CRouterBase::AddLock(POINTS P1,POINTS P2, bool bFill)
{
   SHORT xmin,xmax,ymin,ymax;
   int x,y;
   int xy;

   RPFix LockMask;
   LockMask.Dat  = 0;
   LockMask.X_ID = RPFix::LockID();
   LockMask.Y_ID = RPFix::LockID();
   LockMask.UMask= USE_XY;
   
   xmin = min( P1.x, P2.x );
   ymin = min( P1.y, P2.y );
   xmax = max( P1.x, P2.x );
   ymax = max( P1.y, P2.y );

   if(bFill)
   {
      for(x = xmin; x<=xmax; x++)                  // top line
      {
         for(y = ymin; y<=ymax; y++)                  // left line
         {
            m_Matrix.m_Fix(x,y).Dat |= LockMask.Dat;
         }
      }
   }else
   {
      
      for(x = xmin; x<=xmax; x++)                  // top line
      {
         m_Matrix.m_Fix(x,ymin).Dat |= LockMask.Dat;
      }
      xy = ymin + (ymin & 1);                      // lock even line number too
      if(ymin!=xy)
      {
         for(x = xmin; x<=xmax; x++) 
            m_Matrix.m_Fix(x,xy).Dat |= LockMask.Dat;
      }
      for(x = xmin; x<=xmax; x++)                  // bottom line
      {
         m_Matrix.m_Fix(x,ymax).Dat |= LockMask.Dat;
      }
      xy = ymax - (ymax & 1);
      if(xy!=ymax)
      {
         for(x = xmin; x<=xmax; x++)
            m_Matrix.m_Fix(x,xy).Dat |= LockMask.Dat;
      }
      for(y = ymin; y<=ymax; y++)                  // left line
      {
         m_Matrix.m_Fix(xmin,y).Dat |= LockMask.Dat;
      }
      xy = xmin + (xmin & 1);
      if(xmin!=xy)
      {
         for(y = ymin; y<=ymax; y++)
            m_Matrix.m_Fix(xy,y).Dat |= LockMask.Dat;
      }
      for(y = ymin; y<=ymax; y++)                  // right line
      {
         m_Matrix.m_Fix(xmax,y).Dat |= LockMask.Dat;
      }
      xy = xmax - (xmax & 1);
      if(xmax!=xy)
      {
         for(y = ymin; y<=ymax; y++)
            m_Matrix.m_Fix(xy,y).Dat |= LockMask.Dat;
      }
   }
}

void CRouterBase::SetLockID()
{
   m_SID = RPFix::LockID();
}

void CRouterBase::SetStartID()
{
   ASSERT( m_SID == RPFix::LockID());
   m_SID = RPFix::StartID();
}

void CRouterBase::SetNextID()
{
   ASSERT( m_SID > RPFix::LockID());            // check initialization first
   ASSERT( m_SID < RPFix::MaxID()-1);
   m_SID++;
}

void CRouterBase::InitPointConnect(CPoint from)
{
   DelTempDat();
   m_Origin = C2P(from);
   
   m_bPush = false;
   MPoint MP(m_Origin,m_Matrix);
   
   UsageMask UMask = MP.GetUMask();
   bool bY = (UMask & USE_Y) > 0;
   bool bX = (UMask & USE_X) > 0;
   
   if( bX != bY)
   {
      MP.SetDir( bY ?UP :LEFT);
   }
   
   
   ULONG OldID = m_SID;
   if(max(MP.X_ID(),MP.Y_ID()) > RPFix::LockID())
      m_SID = max(MP.X_ID(),MP.Y_ID());
// MarkSignal (m_Origin);  // prevent from connecting the origin signal
   m_SID = OldID;
   m_FIFO.Push(m_Origin);
}
   
bool CRouterBase::PointConnect (CPoint Destination, CPointArray& PolyLine)
{
   POINTS         Point;                        // current point
   POINTS         PBase;                        // base point
   POINTS         PDest;                        // destination point
   
   POINTS         PNew[4];                      // adjacent todo-points (reordered -> FiFo)
   int            iNewCount;                    // size of PNew[]
   bool           bFound;                       // success indicator
   
   int            Xmin;                         // routing area without border lines 
   int            Ymin; 
   int            Xmax;
   int            Ymax;
   bool           bReady;
   MPoint         MP;
   MPoint         MDest;
   MPoint         MBase;

   m_Matrix.m_Fix.getSize(Xmin, Ymin, Xmax, Ymax);
   Xmin += 1;
   Xmax -= 1;
   Ymin += 1;
   Ymax -= 1;
   m_PolyLine.RemoveAll();                      // clear result
   
   PDest = C2P(Destination);
   
   // inside the routing matrix?
   if( PDest.x >= Xmin && PDest.x <= Xmax && PDest.y >= Ymin && PDest.y <= Ymax)
   {
      MDest.Init(PDest,m_Matrix);            // handle to current point
      
      bFound = (MDest.GetDir() != NONE) && !MDest.ChkMark();
      bReady = bFound;
      
      if(MDest.ChkLock()||MDest.ChkMark())                    // PDest at locked area? (unreachable)
         return false;
      
      while( !bReady && m_FIFO.Pop(PBase) )
      {
         MBase.Init(PBase,m_Matrix);       // handle to base point
         iNewCount = 0;
         
         if(PBase == PDest)
         {
            bFound = !MBase.ChkMark();
            ASSERT(MBase.GetDir() != NONE);
         }
         
         Point = MoveRight(PBase);        // PBase -->
         if( Point.x <= Xmax )
         {
            MP.Init(Point, m_Matrix);
            if(!MP.GetDir() && !(MBase.GetUMask() & USE_XR | MP.GetUMask() & USE_XL) )
            {
               MP.SetDir(LEFT);     // set backward direction AND(!) implicit mark as processed
               
               if(!MP.X_ID())
                  PNew[iNewCount++] = Point;
               
               if(Point == PDest)
               {
                  bReady = true;
                  bFound = !MP.ChkMark();
               }
            }
         }
         
         Point = MoveLeft(PBase);         // <-- PBase
         if( Point.x >= Xmin  )
         {
            MP.Init(Point, m_Matrix);
            if(!MP.GetDir() && !(MBase.GetUMask() & USE_XL | MP.GetUMask() & USE_XR) )  // unprocessed AND no line from base point
            {
               MP.SetDir(RIGHT);
               
               if(!MP.X_ID())
                  PNew[iNewCount++] = Point;
               
               if(Point == PDest)
               {
                  bReady = true;
                  bFound = !MP.ChkMark();
               }
            }
         }      
         
         Point = MoveDown(PBase);         // PBase
         if( Point.y <= Ymax )            //   |
         {                                //   v
            MP.Init(Point, m_Matrix);
            if(!MP.GetDir() && !(MBase.GetUMask() & USE_YB | MP.GetUMask() & USE_YT) )
            {
               MP.SetDir(UP);
               
               if(!MP.Y_ID())
                  PNew[iNewCount++] = Point;
               
               if(Point == PDest)
               {
                  bReady = true;
                  bFound = !MP.ChkMark();
               }
            }
         }
         
         Point = MoveUp(PBase);           //   ^
         if( Point.y >= Ymin )            //   |
         {                                // PBase
            MP.Init(Point, m_Matrix);
            if(!MP.GetDir() && !(MBase.GetUMask() & USE_YT | MP.GetUMask() & USE_YB) )
            {
               MP.SetDir(DOWN);
               
               if(!MP.Y_ID())
                  PNew[iNewCount++] = Point;
               
               if(Point == PDest)
               {
                  bReady = true;
                  bFound = !MP.ChkMark();
               }
            }
         }
         
         if(iNewCount>0)   // try to prefer previous direction
         {
            int iFound = 0;
            int iP;
            OriCode NewDir;    // current  routing direction
            
            m_LastDir = Dir2OriCode(MBase.GetDir());
            
            for(iP = 0; iP < iNewCount; iP++)
            {
               NewDir = GetOriCode(PBase, PNew[iP]);
               if( NewDir == m_LastDir)
               {
                  iFound = iP;
                  break;
               }
            }
            
            m_FIFO.Push(PNew[iFound]);
            m_LastDir = GetOriCode(PBase, PNew[iFound]);
            
            for(iP = 0; iP < iNewCount; iP++)
            {
               if(iP != iFound)
                  m_FIFO.Push(PNew[iP]);
            }
         }
      }    // until empty
   }else // outside the matrix;
   {
      ASSERT(Xmin > 0);   // ==> matrix origin is (0,0)
      ASSERT(Ymin > 0);
      
      bFound = false;
      if(PDest.x < Xmin)
         PDest.x = Xmin & 1 ?Xmin+1 :Xmin;
      else 
         if(PDest.x > Xmax)
            PDest.x = Xmax & 1 ?Xmax-1 :Xmax;
         
         if(PDest.y < Ymin)
            PDest.y = Ymin & 1 ?Ymin+1 :Ymin;
         else 
            if(PDest.y > Ymax)
               PDest.y = Ymax & 1 ?Xmax-1 :Xmax;
   }
   
   if(bFound)
   {
      TraceBack(PDest, PolyLine);
   }else
   {
      ASSERT(PolyLine.GetSize()==0);
      MDest.Init(PDest,m_Matrix);
      if( !MDest.ChkLock() && !MDest.ChkMark() )
      {
         PolyLine.Add(P2C(m_Origin));
         PolyLine.Add(P2C(PDest));
      }
   }

   return bFound;
}


// overwrites the signal IDs at the routing origins
// (this is necessary since an origin point may belong
// to differnent signals - i.e. two selected rubber lines 
// are linked at a fix point)
void CRouterBase::UpdateSignalID(CPointArray& points)
{
   MPoint MP;
   for(int i=0; i< points.GetSize(); i++)
   {
      MP.Init(C2P(points[i]), m_Matrix);
      ASSERT(MP.X_ID()>1);
      ASSERT(MP.Y_ID()>1);

      MP.SetPin(m_SID,MP.GetUMask());
   }
}
   
   
void CRouterBase::InitSignalConnect(CPointArray& from)
{
   DelTempDat();
   m_Origin = C2P(from[0]);
   m_bPush = true;         // push marked signal points into FIFO

   UpdateSignalID(from);   // became necessary since fix point introduction

   MarkSignal (m_Origin);
   m_bPush = false;         // push marked signal points into FIFO
   
   MPoint MP(m_Origin,m_Matrix);
   
   // try to continue the previous direction
   UsageMask UMask = MP.GetUMask();
   bool bY = (UMask & USE_Y) > 0;
   bool bX = (UMask & USE_X) > 0;
   
   if( bX != bY)
   {
      MP.SetDir( bY ?UP :LEFT);
   }
}

bool CRouterBase::SignalConnect(CPointArray& PolyLine)
{
   POINTS   Point;
   POINTS   PBase;
   POINTS   PNew[4];                            // adjacent todo-points (reordered -> FiFo)
   int      iNewCount;                          // size of PNew[]
   int      Xmin;                               // routing area without border lines 
   int      Ymin;
   int      Xmax;
   int      Ymax;
   MPoint   MP;
   MPoint   MBase;

   m_Matrix.m_Fix.getSize(Xmin, Ymin, Xmax, Ymax);
   Xmin += 1;
   Xmax -= 1;
   Ymin += 1;
   Ymax -= 1;
   
   bool bFound = false;
   
   for(int idx=0; idx<m_FIFO.Size(); idx++)  // try to insert a single pin instead of a line
   {
      Point = m_FIFO[idx];
      MP.Init(Point,m_Matrix);
      ASSERT(MP.ChkMark());

      if( MP.X_ID() == m_SID && MP.Y_ID() == m_SID)
      {
         CPointsSet* pDummy = NULL;
         if(!MP.ChkPin())
         {
            PolyLine.RemoveAll();
            PolyLine.Add(P2C(Point));  // add connection point
            return true;
         }
      }
   }
   
   while(m_FIFO.Pop(PBase))
   {
      MBase.Init(PBase,m_Matrix);
      iNewCount = 0;
      Point     = MoveRight(PBase);        // PBase -->
      if(Point.x <= Xmax)
      {
         MP.Init(Point, m_Matrix);
         if(!MP.GetDir() && !MP.ChkMark() && !(MBase.GetUMask() & USE_XR | MP.GetUMask() & USE_XL) )
         {
            bFound = (MP.X_ID()==m_SID || MP.Y_ID()==m_SID) && !MP.ChkMark();
            if( bFound ) 
            {
               MP.SetDir(LEFT);
               break;
            }
            if( !MP.X_ID() )
            {
               MP.SetDir(LEFT);
               PNew[iNewCount++] = Point;
            }
         }
      }
      Point = MoveLeft(PBase);         // <-- PBase
      if( Point.x >= Xmin  )
      {
         MP.Init(Point, m_Matrix);
         if(!MP.GetDir() && !MP.ChkMark() && !(MBase.GetUMask() & USE_XL | MP.GetUMask() & USE_XR) )  // unprocessed AND no line from base point
         {
            bFound = (MP.X_ID()==m_SID || MP.Y_ID()==m_SID) && !MP.ChkMark();
            if( bFound ) 
            {
               MP.SetDir(RIGHT);
               break;
            }
            if( !MP.X_ID() )
            {
               MP.SetDir(RIGHT);
               PNew[iNewCount++] = Point;
            }
         }
      }      
      
      Point = MoveDown(PBase);         // PBase
      if( Point.y <= Ymax )            //   |
      {                                //   v
         MP.Init(Point, m_Matrix);
         if(!MP.GetDir() && !MP.ChkMark() && !(MBase.GetUMask() & USE_YB | MP.GetUMask() & USE_YT) )
         {
            bFound = (MP.X_ID()==m_SID || MP.Y_ID()==m_SID) && !MP.ChkMark();
            if( bFound ) 
            {
               MP.SetDir(UP);
               break;
            }
            if( !MP.Y_ID() )
            {
               MP.SetDir(UP);
               PNew[iNewCount++] = Point;
            }
         }
      }
      Point = MoveUp(PBase);           //   ^
      if( Point.y >= Ymin )            //   |
      {                                // PBase
         MP.Init(Point, m_Matrix);
         if(!MP.GetDir() && !MP.ChkMark() && !(MBase.GetUMask() & USE_YT | MP.GetUMask() & USE_YB) )
         {
            bFound = (MP.X_ID()==m_SID || MP.Y_ID()==m_SID) && !MP.ChkMark();
            if( bFound ) 
            {
               MP.SetDir(DOWN);
               break;
            }
            if( !MP.Y_ID() )
            {
               MP.SetDir(DOWN);
               PNew[iNewCount++] = Point;
            }
         }
      }
      
      if(iNewCount>0)                        // try to prefer previous direction
      {
         int iFound = 0;
         int iP;
         OriCode  NewDir;                    // current  routing direction
         
         MBase.Init(PBase,m_Matrix);
         m_LastDir = Dir2OriCode(MBase.GetDir());
         
         for(iP = 0; iP < iNewCount; iP++)
         {
            NewDir = GetOriCode(PBase,PNew[iP]);
            if( NewDir == m_LastDir)
            {
               iFound = iP;
               break;
            }
         }
         m_FIFO.Push(PNew[iFound]);
         m_LastDir = GetOriCode(PBase,PNew[iFound]);
         
         for(iP = 0; iP < iNewCount; iP++)
         {
            if(iP != iFound)
               m_FIFO.Push(PNew[iP]);
         }
      }
   }

   if(bFound)
      TraceBack(Point, PolyLine);               // updates m_Path

   return bFound;
}

void CRouterBase::TraceBack(POINTS P, CPointArray& PolyLine )// finds the path back to the origin
{
   Dir LastDir = NONE;                          // important!
   PolyLine.RemoveAll();
   MPoint MP(P,m_Matrix);   
   if(!(P == m_Origin))
   {
      do{
         MP.Init(P,m_Matrix);
         
         if(MP.GetDir() != LastDir)            // corner? (not connected: false)
         {
            PolyLine.Add(P2C(P));
            if(Dir2OriCode(LastDir) == Dir2OriCode(MP.GetDir()))  // opposite directions->endless loop
               return;
            LastDir = MP.GetDir();
         }
         switch(MP.GetDir())
         {
         case RIGHT: P.x += 2; break;
         case LEFT : P.x -= 2; break;
         case UP   : P.y -= 2; break;
         case DOWN : P.y += 2; break;
         default: 
            {
               return;              // unconnected: ready
            }
         }
      }while( !(P == m_Origin) );
      
      PolyLine.Add(P2C(P));
      ASSERT(P == m_Origin);
      ASSERT(PolyLine.GetSize()>=2);
   }
   
}

CPoint CRouterBase::SearchNext(CPoint Origin, UINT SID, bool bAnyPoint)
{
   POINTS   P;
   int Xmin,Ymin,Xmax,Ymax;
   
   CPoint   PNext = Origin;
   int      dMin  = INT_MAX;
   POINTS   PO    = C2P(Origin);
   
   ASSERT(SID >= RPFix::StartID()); 
   MarkSignal(PO);
   
   m_Matrix.m_Fix.getSize(Xmin, Ymin, Xmax, Ymax);
   
   if(bAnyPoint)
   {
      for(P.y = Ymin; P.y < Ymax; P.y++)
         for(P.x = Xmin; P.x < Xmax; P.x++)
         {
            MPoint MP(P,m_Matrix);
            if( !MP.ChkMark() && 
               (
               (MP.X_ID() == SID) && (MP.Y_ID() == 0) || 
               (MP.Y_ID() == SID) && (MP.Y_ID() == 0) ||
               (MP.Y_ID() == SID) && (MP.X_ID() == SID)  
               ) && !(P == PO))
            {
               int d = (int)GetDistance(P,PO);
               if(d < dMin && !MP.ChkMark())
               {
                  dMin  = d;
                  PNext = CPoint(P.x,P.y);
               }
            }
         }
   }else
   {
      for(P.y = Ymin; P.y < Ymax; P.y++)
         for(P.x = Xmin; P.x < Xmax; P.x++)
         {
            MPoint MP(P,m_Matrix);
            if( !MP.ChkMark() && (MP.X_ID() == SID) && (MP.Y_ID() == SID) && !(P == PO))
            {
               int d = GetDistance(P,PO);
               if(d < dMin)
               {
                  dMin  = d;
                  PNext = CPoint(P.x,P.y);
               }
            }
         }
   }
   
   return PNext;
}

// square metric (!) for distance calculation
inline int CRouterBase::GetDistance(POINTS P1, POINTS P2)
{
   return abs(P1.x - P2.x) + abs(P1.y - P2.y);
}

//}CRouterBase
//=============================================================================
//{AutoRouter implementation

void CAutoRouter::RouteSignal(CSignal* pSignal)  // route the one point of the given signal
{
   int i;
   ASSERT(pSignal);
   ASSERT(pSignal->ToDo.GetSize()>0);
   
   m_SID = pSignal->SID;
   InitSignalConnect(pSignal->ToDo);
   //      SetSignalID(pSignal->SID);
   
   CPointArray PolyLine;
   if(SignalConnect(PolyLine))
   {
      int N = PolyLine.GetSize();
      
      if( N > 0 )
      {
         if( N == 1 )   // connect 
         {
            AddPin(PolyLine[0]);
         }else
         {
            for(i = 0; i < N-1; i++)               // update the matrix
               AddLine(PolyLine[i],PolyLine[i+1]);
         }  
         m_Result.Add(PolyLine);
         // adjust ready-set
         DelTempDat(); // ignore tempSignalIDs
         m_bPush = false;
         
         if(pSignal->PDone.GetSize()==0)
            MarkSignal(C2P(pSignal->ToDo[0]));
         else
            MarkSignal(C2P(pSignal->PDone[0]));
         
         i = 0;
         while( i < pSignal->ToDo.GetSize())
         {
            MPoint MP(C2P(pSignal->ToDo[i]),m_Matrix);
            if( MP.ChkMark() )
            {
               pSignal->PDone.Add(pSignal->ToDo[i]);
               pSignal->ToDo.RemoveAt(i);
            }else
            {
               i++;
            }
         }
         i = 0;
         while( i < pSignal->PSingle.GetSize())
         {
            MPoint MP(C2P(pSignal->PSingle[i]),m_Matrix);
            if( MP.ChkMark() )
            {
               pSignal->PDone.Add(pSignal->PSingle[i]);
               pSignal->PSingle.RemoveAt(i);
            }else
            {
               i++;
            }
         }
      }
   }else
   {
      pSignal->PSingle.Add(pSignal->ToDo[0]); // unroutable
      pSignal->ToDo.RemoveAt(0);
   }
   
   // generate rubber lines for unroutable points
   if(pSignal->ToDo.GetSize()==0 && pSignal->PSingle.GetSize()>0)
   {
      DelTempDat();     // required by SearchNext(P) to flag signal
      for(int i = 0; i < pSignal->PSingle.GetSize(); i++)
      {
         PolyLine.RemoveAll();
         CPoint P1 = pSignal->PSingle[i];
         
         CPoint P2 = SearchNext(P1,pSignal->SID,false);
         PolyLine.Add(P1);
         PolyLine.Add(P2);

         pSignal->PSingle.Remove(P1);
         pSignal->PSingle.Remove(P2);
         m_NoResult.Add(PolyLine);
         i=0;  // restart
      }
   }
}//CAutoRouter::RouteSignal
   
CAutoRouter::CAutoRouter(CRect &Area)
{
   CRouterBase::Init(Area);
   
   m_iNextSignal = 0;
   m_SID = 0;
   m_bComment = false;
}

void CAutoRouter::UpdateSignalTab()
{
   int N = m_SignalTab.GetUpperBound();
   if( N<0 || N < (int)m_SID - (int)RPFix::StartID())
      m_SignalTab.SetAtGrow(m_SID - RPFix::StartID(), CSignal(m_SID));
   m_SignalTab[m_SID - RPFix::StartID()].SID = m_SID;
}

void CAutoRouter::SetNextID()          // call after SetLockID 
{ 
   if(m_SID <= RPFix::LockID())
      CRouterBase::SetStartID();
   else
      CRouterBase::SetNextID();
   UpdateSignalTab();
}

void CAutoRouter::LockRect(CRect R, bool bFill)
{
   AddLock( C2P(R.TopLeft()), C2P(R.BottomRight()), bFill );
}

void CAutoRouter::AddToDo(CPoint P)
{
   ASSERT(m_SignalTab.GetUpperBound() >= (int)m_SID - (int)RPFix::StartID());
   m_SignalTab[m_SID-RPFix::StartID()].ToDo.AddUnique(P);
   AddPin(P);
}

void CAutoRouter::StartPointRouting(CPoint POrigin)
{
   InitPointConnect(POrigin);
}

void CAutoRouter::StartSignalRouting()
{
   m_iNextSignal = 0;
   m_Result.RemoveAll();
   m_NoResult.RemoveAll();
   DelTempDat();
   
   for(int i=0; i<m_SignalTab.GetSize(); i++)   // for each signal
   {
      m_SignalTab[i].PSingle.RemoveAll();
      m_SignalTab[i].PDone.RemoveAll();
      
      for(int j=0; j<m_SignalTab[i].ToDo.GetSize(); j++)    // for each origin-point
      {
         CPoint P1 = m_SignalTab[i].ToDo[j]; 
         m_SID = m_SignalTab[i].SID;
         m_bPush = false;
         MarkSignal(C2P(P1));
         
         for(int k = j+1; k<m_SignalTab[i].ToDo.GetSize();) // for the remaining points
         {
            CPoint P2 = m_SignalTab[i].ToDo[k];
            MPoint MP(C2P(P2),m_Matrix);
            if(MP.ChkMark())                                // already connected?
            {
               m_SignalTab[i].ToDo.RemoveAt(k);             // drop from todo list
            }else
            { 
               k++;
            }
         }
      }
   }
}

bool CAutoRouter::RouteToPoint(CPoint P)
{
   if(m_Result.GetSize()>0)
      m_Result.RemoveAll();
   
   if(m_NoResult.GetSize()>0)
      m_NoResult.RemoveAll();
   
   CPointArray PLine;
   if(PointConnect(P,PLine))        // successful: true (writes result to PLine)
   {
      ASSERT(PLine.GetSize()!=1);   // empty or at least two points
      if(PLine.GetSize()>1)
         m_Result.Add(PLine);       // fixed line (empty if connected already)
      return true;
   }else                            // not routed
   {
      ASSERT(PLine.GetSize()!=1);   // empty or at least two points
      if(PLine.GetSize()>1)
         m_NoResult.Add(PLine);     // add as rubber line
      return false;
   }
}

bool CAutoRouter::RoutingStep()
{
   bool bAgain = false;
   int STSize  = m_SignalTab.GetSize();
   if(m_iNextSignal >= STSize)
      m_iNextSignal = 0;
   
   if(STSize)
   {
      for(int i = 0; i < STSize; i++)
      {
         if(m_SignalTab[m_iNextSignal].ToDo.GetSize()>0)
         {
            RouteSignal(&m_SignalTab[m_iNextSignal]);
            bAgain = true;
         }
         m_iNextSignal = (m_iNextSignal+1) % STSize;
      }
   }
   return bAgain;  // call again
}

bool CAutoRouter::GetNextResult(CPointArray& PLine)   // returns a poly line
{
   int N = m_Result.GetSize()>0;                      // delete in reverse order to
   if(N>0)                                            // prevent from shifting memory
   {
      PLine = m_Result[N-1];
      m_Result.RemoveAt(N-1);
   }
   return N>0;
}

bool CAutoRouter::GetNextRubber(CPointArray& PLine)               // returns a poly line
{
   int N = m_NoResult.GetSize()>0;
   if(N>0)
   {
      PLine = m_NoResult[N-1];
      ASSERT(PLine.GetSize()==2);
      m_NoResult.RemoveAt(N-1);
   }
   return N>0;
}

int CAutoRouter::xMin()
{
   int xmin,i;
   m_Matrix.m_Fix.getSize(xmin, i, i, i);
   return xmin;
}

int CAutoRouter::xMax()
{
   int xmax,i;
   m_Matrix.m_Fix.getSize(i, i, xmax, i);
   return xmax;
}
//}AutoRouter implementation
//=============================================================================
