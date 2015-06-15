#ifndef __ROUTER_H_INCLUDED__
#define __ROUTER_H_INCLUDED__

#include "Set.h"  // CSet

//=============================================== CMatrix<T>:: declaration
// - supports both associative, AND random access
//
template <class T, class PointType> class CMatrix
{
   public:
      CMatrix();
      CMatrix(const CMatrix&);
      virtual ~CMatrix();

      T&       operator()(POINTS pos);
      T&       operator()(int  x, int  y);
      CMatrix& operator=(const CMatrix& M);

      bool Resize  (int xmin, int ymin, int xmax, int ymax, bool bCopy = false);
      void Erase   ()         {ZeroMemory(p_matrix, getBytes());              }
      long getBytes()         {return getElems() * sizeof(T);                 }
      long getElems()         {return m_xcount * m_ycount;                    }
      int  getXSize()         {return m_xcount;                               }
      int  getYSize()         {return m_ycount;                               }
      void getSize(int &xmin, int &ymin, int &xmax, int &ymax);
      bool IsValidPoint(POINTS P);
      bool IsValidPoint(int x, int y);

   protected:
      T* GetData()            { return p_matrix;                              }
      int Offset(int x,int y) { return (x-m_xmin) + m_xcount*(y-m_ymin);      }
      int Offset(POINTS p)    { return (p.x-m_xmin) + m_xcount*(p.y-m_ymin);  }

      T*  p_matrix;
      int m_xcount;
      int m_ycount;
      int m_xmin; // min valid index   (m_xmin <= x < m_xmax)
      int m_xmax; // 1st invalid index
      int m_ymin; 
      int m_ymax;
};//CMatrix<T> ----------------------------------------------------------------
template <class T,class PointType> inline
void CMatrix<T,PointType>::getSize(int &xmin,int &ymin,int &xmax,int &ymax) 
{
   xmax = m_xmax-1; ymax = m_ymax-1;
   xmin = m_xmin; ymin = m_ymin;
}
template <class T,class PointType> inline 
bool CMatrix<T,PointType>::IsValidPoint(POINTS P)
{
   return IsValidPoint(P.x,P.y);
}

template <class T,class PointType> inline 
bool CMatrix<T,PointType>::IsValidPoint(int x, int y)
{
   return (x >= m_xmin) && (x < m_xmax) && 
      (y >= m_ymin) && (y < m_ymax);
}
template <class T,class PointType> inline 
CMatrix<T,PointType>::CMatrix()
{
   m_xmin = 0;
   m_ymin = 0;
   m_xmax = 0;
   m_ymax = 0;
   p_matrix = 0;
}
template <class T,class PointType> inline 
CMatrix<T,PointType>::~CMatrix()
{ 
   if(p_matrix)
   {
      delete p_matrix;
      p_matrix = NULL;
   }
}
template <class T,class PointType> inline 
T& CMatrix<T,PointType>::operator()(POINTS pos)
{
   ASSERT( IsValidPoint(pos.x,pos.y) );
   return p_matrix[Offset(pos)];
}
template <class T,class PointType> inline 
T& CMatrix<T,PointType>::operator()(int x, int y)
{
   ASSERT( IsValidPoint(x,y) );
   return p_matrix[Offset(x,y)];
}
template <class T,class PointType> inline 
bool CMatrix<T,PointType>::Resize(int xmin, int ymin, int xmax, int ymax, bool bCopy)
{
   T* p = NULL; 
   int xcount = xmax-xmin+1;              // [min:max]
   int ycount = ymax-ymin+1;
   int N = xcount * ycount;

   try {
      if(!bCopy && p_matrix)              // save memory if possible
      {
         delete p_matrix;
         p_matrix = 0;
      }
      p = new T[N];
      FillMemory(p,N*sizeof(T),0);
   }
   catch(...)
   {
      return false;
   }

   if(p_matrix && p)
   {
      if(bCopy)
      {
         int xEnd = min(xmax, m_xmax);    // common area calculation
         int yEnd = min(ymax, m_ymax);

         int xStart = max(xmin, m_xmin);
         int yStart = max(ymin, m_ymin);

         for(int idy = yStart; idy <= yEnd; idy++ )
            for(int idx = xStart; idx <= xEnd; idx++ )
               p[idx + xcount*idy] = p_matrix[Offset(idx,idy)];
      }
      delete p_matrix;
   }

   m_xcount = xcount;
   m_ycount = ycount;
   m_xmax   = xmax;
   m_ymax   = ymax;
   m_xmin   = xmin;
   m_ymin   = ymin;
   p_matrix = p;
   return (p_matrix != NULL);
}
template <class T,class PointType> inline 
CMatrix<T,PointType>::CMatrix(const CMatrix<T,PointType>& M)
{
   p_matrix = 0;
   Resize(M.m_xmin, M.m_ymin, M.m_xmax, M.m_ymax, false);
   CopyMemory(p_matrix, M.p_matrix, getBytes());
}
template <class T,class PointType> inline 
CMatrix<T,PointType>& CMatrix<T,PointType>::operator=(const CMatrix<T,PointType>& M)
{
   if(&M!=this)
   {
      Resize(M.m_x0, M.m_y0, M.m_dx, M.m_dy, false);
      CopyMemory(p_matrix, M.p_matrix, getBytes());
   }
   return *this;
}


//=============================================================================
// CValQueue<T> template implementation of CQueue
//=============================================================================
template<class T>
class CValQueue
{
   int iR;  // read insed
   int iW;  // write index
   int L;   // max elemCount
   int N;   // elems inside
   int b;   // realloc size
   T* pBuf;

   void Grow();

public:
   CValQueue(int blockSize=512);
   ~CValQueue();
   
   void  Push(T& e);
   bool  Pop(T& e);   
   void  Flush();
   int   Size();
   T& operator[](int idx);
};// CValQueue<T> -------------------------------------------------------------

template <class T>
inline CValQueue<T>::CValQueue(int blockSize)
{
   pBuf = NULL;
   L = 0;
   if(blockSize>0)
   {
      b = blockSize;
      Grow();
   }
   Flush();
}

template <class T>
inline CValQueue<T>::~CValQueue()
{
   delete pBuf;
   pBuf = NULL;
}

template <class T>
inline T& CValQueue<T>::operator[](int idx)
{
   ASSERT( idx >= 0 );
   ASSERT( idx <  N );
   return pBuf[idx];
}

template <class T>
inline void CValQueue<T>::Flush()
{
   iR = 0;
   iW = 0;
   N  = 0;
}

template <class T>
inline int CValQueue<T>::Size()
{
   return N;
}

template <class T>
inline void CValQueue<T>::Grow()
{
   if(!pBuf)
   {
      pBuf = new T[b];
      L = b;
   }else
   {
      T* p = new T[L+b];
      CopyMemory(p,&pBuf[iR],sizeof(T)*(N-iR));
      CopyMemory(&p[N-iR],pBuf,sizeof(T)*iR);
      
      L += b;
      iW = N;
      iR = 0;
      
      delete pBuf;
      pBuf = p;
   }
}

template <class T>
inline void CValQueue<T>::Push(T& e)
{
   if(N>=L)
      Grow();
   
   N++;
   pBuf[iW] = e;
   if(++iW==L)
      iW = 0;
}

template <class T>
inline bool CValQueue<T>::Pop(T& e)
{
   if(N)
   {
      N--;
      e = pBuf[iR];
      if(++iR==L)
         iR=0;
      return true;
   }
   return false;
}


//=============================================================================
// array of points - unique insertion supported
//=============================================================================
typedef int (PointCompare)(const CPoint*, const CPoint*);

typedef struct CPointArray:public CArray<CPoint, CPoint>
{
   public:
      CPointArray(){};
      CPointArray(const CPointArray& V);
      CPointArray& operator=(const CPointArray& V);
      int Find(const CPoint& p);
      int AddUnique(const CPoint& p); // no duplicate entries; returns elem index
      void Sort(PointCompare* CompFN);
      void Remove(const CPoint& p);
}CPointArray;
//===========

inline CPointArray::CPointArray(const CPointArray& V)
{
   if(&V!=this && &V!=NULL)
   {
      for(int i=0; i<V.GetSize(); i++)
         Add(V.GetAt(i));
   }
}
inline CPointArray& CPointArray::operator=(const CPointArray& V)
{
   if(&V!=this)
   {
      RemoveAll();
      for(int i=0; i<V.GetSize(); i++)
         Add(V.GetAt(i));
   }
   return *this;
}
inline int CPointArray::Find(const CPoint& p)      // trivial linear search
{
   for(int i=0; i<GetSize(); i++)
   {
      if(GetAt(i)==p)
         return i;
   }
   return -1;
}
inline int CPointArray::AddUnique(const CPoint& p) // linear search
{
   int iPos = Find(p);
   if(iPos<0)
      iPos = Add(p);
   return iPos;
}
inline void CPointArray::Sort(PointCompare* CompFN)
{
   typedef int (*compfn)(const void*, const void*);

   CPoint *pData = GetData();
   if(pData)
   {
      qsort(pData, GetSize(), sizeof(CPoint), (compfn)CompFN );
   }
}
inline void CPointArray::Remove(const CPoint& p)
{
   int iPos = Find(p);
   if(iPos>=0)
      RemoveAt(iPos);
}

//=============================================================================
// router data types 
//=============================================================================
typedef CSet<POINTS>                                    CPointsSet;
typedef CMap  <POINTS,POINTS, CPointsSet*,CPointsSet*>  CLinkTab;
typedef CArray<CPointArray, CPointArray>                CPathList;

typedef enum {
   // basic point usage masks
   USE_NONE = 0x0,            // unused

   USE_XL   = 0x1,            // x is used at left
   USE_XR   = 0x2,            // x is used at right
   USE_X    = USE_XL| USE_XR, // x is entirely used

   USE_YT   = 0x4,            // y is used at top
   USE_YB   = 0x8,            // y is used at bottom
   USE_Y    = USE_YT| USE_YB, // y is entirely used

   // base mask combinations
   USE_LY   = USE_XL| USE_Y , // y is used entirely;  x is used at left
   USE_RY   = USE_XR| USE_Y , // y is used entirely;  x is used at right
   USE_XT   = USE_X | USE_YT, // x is used entirely;  y is used at top
   USE_XB   = USE_X | USE_YB, // x is used entirely;  y is used at bottom
   
   USE_XY   = USE_X | USE_Y,  // both directions are entirely used (connected if X_ID==MaxID())

   USE_TL   = USE_XL| USE_YT, // x is used at left;   y is used at top
   USE_TR   = USE_XR| USE_YT, // x is used at right;  y is used at top
   USE_BL   = USE_XL| USE_YB, // x is used at left;   y is used at bottom
   USE_BR   = USE_XR| USE_YB  // x is used at right;  y is used at bottom
}UsageMask;


typedef enum {                // trace-back directions
   NONE, 
   LEFT, 
   RIGHT, 
   UP, 
   DOWN,
} Dir;

typedef enum {                // orientation codes
   OR_NONE  = 0,              // no orientation
   OR_X     = 1,              // horizontal
   OR_Y     = 2,              // vertical
   OR_XY    = 3               // diagonal
}OriCode;


inline OriCode GetOriCode(POINTS PDst, POINTS PSrc)         // 0: dx==0 && dy==0; 
{                                                           // 1: dx!=0 && dy==0;
   return(OriCode)((PDst.x!=PSrc.x)|((PDst.y!=PSrc.y)<<1)); // 2: dx==0 && dy!=0;
}                                                           // 3: dx!=0 && dy!=0;

inline OriCode Dir2OriCode(Dir D)
{
   if(D == LEFT || D == RIGHT)
      return OR_X;
   if(D == UP || D == DOWN)
      return OR_Y;
   return OR_NONE;
}

/////////////////////////////////////////////////
// static router data - describes a single point
//
typedef struct _RPFix
{
   typedef ULONG T;
   union {
      struct {
         T X_ID  :14;     // X signal identification number
         T Y_ID  :14;     // Y signal identification number
         T UMask :04;     // VarPointMask-value
      };
      T Dat;
   };

   static ULONG LockID()   { return 0x00000001; }
   static ULONG MaxID()    { return 0x00003FFF; }
   static ULONG StartID()  { return 0x00000002; }

   T xID() const    { return (X_ID==MaxID()) ?Y_ID :X_ID;  }  // x signal ID
   T yID() const    { return Y_ID;                         }  // y signal ID
   void  xID(T ID)  { X_ID = ID;                           }  // x signal ID
   void  yID(T ID)  { Y_ID = ID;                           }  // y signal ID
   bool ChkPin()    { return (X_ID == MaxID());            }

   void SetPin(T SID, UsageMask M) // with connection point
   {
      X_ID  = MaxID(); 
      Y_ID  = SID; 
      UMask = M;
   }
   void AddPin(T SID, UsageMask M) // with connection point
   {
      X_ID  = MaxID(); 
      Y_ID  = SID; 
      UMask |= M;
   }
   void Add_X(T SID, UsageMask M)
   {
      ASSERT(!(M & USE_Y));
      X_ID  = SID;
      UMask |= M;
   }
   void Add_Y(T SID, UsageMask M)
   {
      ASSERT(!(M & USE_X));
      Y_ID  = SID;
      UMask |= M;
   }

   static bool CHECK_ASSERTIONS(){ return sizeof(_RPFix)==sizeof(T); }
}RPFix, *PRPFix;


typedef enum
{
   NOmark = 0,
   Xmark  = 1,
   Ymark  = 2,
   XYmark = 3
}Marking;


typedef struct _RPVar   // dynamic router data
{
   typedef UCHAR T;     // 
   union {
      struct {
         T Dir    :03;  // for back tracking 0=none, 1=LEFT, 2=RIGHT, 3=UP, 4=DOWN (5..7=UNDEFINED)
         T Mark   :01;  // X direction is marked
         T _rest_ :04;  // not used
      };
      T Dat;
   };
   static bool CHECK_ASSERTIONS(){ return sizeof(_RPVar)==sizeof(T); }
}RPVar, *PRPVar;

//=============================================================================

typedef  CMatrix  <RPFix,  POINTS> MRPFix;
typedef  CMatrix  <RPVar,  POINTS> MRPVar;
typedef  CValQueue<POINTS> CPointFiFo;


typedef struct                            // routing matrix (container for several data types)
{
   CLinkTab    m_Link;                    // invisible signal connections (used to flag signals)
   MRPFix      m_Fix;                     // elements matrix
   MRPVar      m_Var;                     // tempdat-matrix
}MRouter;


typedef struct _MPoint                    // helper struct for transparent matrix-point access
{
   public:
      void SetDir(Dir dir)       { pV->Dir=dir;          }
      Dir  GetDir()              { return (Dir)pV->Dir;  }
      
      void SetMark()             { pV->Mark = 1;         }
      bool ChkMark()             { return pV->Mark;      }
      
      ULONG X_ID()               { return pF->xID();     }
      ULONG Y_ID()               { return pF->yID();     }

      void AddPin(ULONG id, UsageMask M)
      {
         pF->AddPin(id,M);
      }
      void SetPin(ULONG id, UsageMask M)
      {
         pF->SetPin(id,M);
      }

      bool ChkLock()             { return pF->xID()==RPFix::LockID() || pF->yID()==RPFix::LockID(); }
      bool ChkPin()              { return pF->ChkPin();  }
      
      UsageMask GetUMask()       { return (UsageMask)pF->UMask; }

      void Init(POINTS P, MRouter &M)
      {
         ASSERT(M.m_Fix.IsValidPoint(P));
         pF = & M.m_Fix(P);
         pV = & M.m_Var(P);
      }
      _MPoint(){};
      _MPoint(POINTS P,MRouter &M){ Init(P,M); }

   private:
      PRPFix   pF;
      PRPVar   pV;
}MPoint;

//=============================================================================
// utility functions
//=============================================================================

inline UINT AFXAPI HashKey(POINTS key) {
	return *(UINT*)&key;
}

inline BOOL operator==(const POINTS &P1, const POINTS &P2) {
   return (*(long*)&P1)==(*(long*)&P2);
}

inline POINTS MoveUp   (POINTS P) {return P.y -= 2, P;}
inline POINTS MoveDown (POINTS P) {return P.y += 2, P;}
inline POINTS MoveLeft (POINTS P) {return P.x -= 2, P;}
inline POINTS MoveRight(POINTS P) {return P.x += 2, P;}

inline POINTS C2P(CPoint CP)                 // CPoint to POINTS conversion
{ 
   POINTS PP; 
   PP.x = (SHORT)CP.x;
   PP.y = (SHORT)CP.y; 
   return PP;
}

inline CPoint P2C(POINTS PP)                 // POINTS to CPoint conversion
{ 
   return CPoint(PP.x,PP.y);
}
//=============================================================================
// base router functionality
//=============================================================================
class CRouterBase
{
protected:
   struct   // routing data set
   {
      MRouter     m_Matrix;                        // routing area (see above)
      CPointFiFo  m_FIFO;                          // routing point queue
      OriCode     m_LastDir;                       // temp dat used for corner point reduction
      ULONG       m_SID;                           // current signal-ID
      bool        m_bDelTemp;                      // incremental routing (leave TempData)
      bool        m_bPush;                         // used by MarkSigXXX  true: push marked points to FiFo
      POINTS      m_Origin;
   };
   CPointArray    m_PolyLine;                      // connected points
   CPointArray    m_Connects;                      // new pin positions (connects crossing lines)

public:
   CRouterBase();
   virtual ~CRouterBase();
   void Init(CRect area);                          // declare routing area size
   void MarkSignal(POINTS P);                      // marks connected points
   void AddPin(CPoint P,UsageMask UM=USE_NONE);
   void SetPin(CPoint P,UsageMask UM=USE_NONE);
   void AddLine(CPoint P1,CPoint P2);
   void AddLink(CPoint P1,CPoint P2);
   void AddLock(POINTS P1,POINTS P2,bool bFill);
   void SetLockID();
   void SetNextID();

   void InitPointConnect(CPoint from);
   bool PointConnect (CPoint Destination, CPointArray& PolyLine);

   void InitSignalConnect(CPointArray &from);
   bool SignalConnect(CPointArray& PolyLine);

protected:
   void UpdateSignalID(CPointArray& from);
   void SetStartID();
   void DelTempDat();
   void TraceBack(POINTS P, CPointArray& PolyLine );
   CPoint SearchNext(CPoint Origin, UINT SID, bool bAnyPoint);
   int  GetDistance(POINTS P1, POINTS P2);
};


//=============================================================================
// AutoRouter
//=============================================================================

class CAutoRouter :protected CRouterBase   // can handle multiple signals
{
   typedef struct _CSignal
   {
      UINT        SID;           // Signal ID
      CPointArray ToDo;          // unprocessed
      CPointArray PDone;         // ready
      CPointArray PSingle;       // destination points for remaining m_ToDo's
      
      _CSignal(UINT SID=0)
      {
         SID = 0;
         ToDo.RemoveAll();
         PDone.RemoveAll();
         PSingle.RemoveAll();
      }
   }CSignal;
   typedef CArray<CSignal,CSignal>  CSignalTab;
   
   CSignalTab  m_SignalTab;   // to be routed
   int         m_iNextSignal; // next Signal to be routed

   typedef CArray<CPointArray, CPointArray> CPLineArray;
   CPLineArray m_Result;      // routed lines
   CPLineArray m_NoResult;    // unroutable lines
   
protected:
   void RouteSignal(CSignal* pSignal);  // route the one point of the given signal
   void UpdateSignalTab();
   
public:
   bool        m_bComment;    // set signal routing mode "including comment"
   
   CAutoRouter(CRect &Area);

   void SetLockID() { CRouterBase::SetLockID(); }
   void SetNextID();          // call after SetLockID 
   void LockRect(CRect R, bool bfill=false);
   void AddToDo(CPoint P);
   void AddPin(CPoint P,UsageMask UM=USE_NONE)  { CRouterBase::AddPin(P,UM);  } 
   void SetPin(CPoint P,UsageMask UM=USE_NONE)  { CRouterBase::SetPin(P,UM);  } 
   void AddLine(CPoint P1,CPoint P2)            { CRouterBase::AddLine(P1,P2);}
   void AddLink(CPoint P1,CPoint P2)            { CRouterBase::AddLink(P1,P2);}
   void StartPointRouting(CPoint POrigin);         // call with the origin point
   void StartSignalRouting();                      // call once before RoutingStep-calls
   bool RouteToPoint(CPoint P);                    // returns success-info
   bool RoutingStep();                             // call until it returns false
   bool GetNextResult(CPointArray& PLine);         // returns a poly line
   bool GetNextRubber(CPointArray& PLine);         // returns a poly line
   int xMin();
   int xMax();
};

#endif

