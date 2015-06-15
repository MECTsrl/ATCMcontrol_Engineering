#if !defined(AFX_ELEMPTRLIST_H__13D0E5C1_FB83_11D1_8E7B_444553540000__INCLUDED_)
#define AFX_ELEMPTRLIST_H__13D0E5C1_FB83_11D1_8E7B_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/*inc*=======================================================================*/

#include <afxcoll.h>
#include "GrEditorBase.h"
#include "Elem.h"
#include "Set.h"

/*const*=====================================================================*/
/*macro*=====================================================================*/
/*type*======================================================================*/

struct TPinEntry
{
   CPoint m_Pos;
   CPoint m_NewPos;

   int    m_iSelected;
   int    m_iDeSelected;
   int    m_iRubbLines;

   int    m_iConnects;   // sum of sel,desel,rubb
   int    m_iTemp;       // sum of sel,desel,rubb

   CArray<CElem*, CElem*> m_Elems;

   TPinEntry();
};

class CPinTable : public CMap<CPoint, CPoint, TPinEntry*, TPinEntry*>
{
public:
   CPinTable();
   ~CPinTable();

   TPinEntry *InsertPin(CElem *pE, CPoint NewPos);
   void       MovePin(CElem *pE, TPinEntry *&pPE, CPoint NewPos);
   void       DeletePin(CElem *pE, TPinEntry *&pPE, CPoint NewPos);

   void       InsertPins(CElem *pE);
   void       DeletePins(CElem *pE);
   void       DeleteAllPins();
   void       SelectPins(CElem *pE, bool bSelect);
   void       SaveConnectsToTemp(); // for each: m_iTemp = m_iConnects
};


class CSignal :public CArray<CElem*,CElem*>
{
public:
   CSignal()
   { 
      m_csum = 0xFFFFFFFF;
   }

   CSignal& operator=(const CSignal& S)
   {
      if(&S!=this)
      {
         RemoveAll();
         for(int i=0;i<S.GetSize(); i++)
         {
            Add(S[i]);
         }
         m_csum = S.m_csum;
      }
      return *this;
   }

   int GetCSum()
   {
      if(m_csum == 0xFFFFFFFF)
      {
         for(int i=0;i<GetSize(); i++)
         {
            m_csum ^= (int)GetAt(i);
         }
      }
      return m_csum;
   }

protected:
   int m_csum;
};

class CStatisticData
{
public:
	int		m_iFBD_Cmt;
	int		m_iFBD_Conn;
	int		m_iFBD_Cont;
	int		m_iFBD_FB;
	int		m_iFBD_Fct;
	int		m_iFBD_Jump;
	int		m_iFBD_Label;
	int		m_iFBD_Pro;
	int		m_iFBD_Ret;
	int		m_iFBD_VarFb;
	int		m_iFBD_VarRd;
	int		m_iFBD_VarWr;

   int		m_iSFC_AltDiv;
	int		m_iSFC_SimDiv;
	int		m_iSFC_Step;
	int		m_iSFC_Tra;
	int		m_iSFC_ActBlk;
	int		m_iSFC_Action;
	
	int		m_iLD_Power;
	int		m_iLD_Contact;
	int		m_iLD_Coil;
	
   int		m_iLINE_Fix;
	int		m_iLINE_Rubb;
	
   int		m_iSUM_FBD;
	int		m_iSUM_SFC;
	int		m_iSUM_LD;
	int		m_iSUM_LINE;
	int		m_iSUM_Total;

   CStatisticData() { Clear(); }

   void Clear() 
   {
	   m_iFBD_Cmt    = 0;   m_iSFC_AltDiv = 0;
	   m_iFBD_Conn   = 0;	m_iSFC_SimDiv = 0;
	   m_iFBD_Cont   = 0;	m_iSFC_Step   = 0;
	   m_iFBD_FB     = 0;	m_iSFC_Tra    = 0;
	   m_iFBD_Fct    = 0;	m_iSFC_ActBlk = 0;
	   m_iFBD_Jump   = 0;	m_iSFC_Action = 0;
	   m_iFBD_Label  = 0;   m_iLD_Power   = 0;
	   m_iFBD_Pro    = 0;	m_iLD_Contact = 0;
	   m_iFBD_Ret    = 0;   m_iLD_Coil    = 0;
	   m_iFBD_VarFb  = 0;	
	   m_iFBD_VarRd  = 0; 	m_iLINE_Rubb  = 0;
	   m_iFBD_VarWr  = 0;	m_iLINE_Fix   = 0;
     	
      m_iSUM_FBD    = 0;   m_iSUM_LINE   = 0;
      m_iSUM_SFC    = 0;   m_iSUM_Total  = 0;
      m_iSUM_LD     = 0;
   };
};

/*glovar*====================================================================*/
/*proto*=====================================================================*/

typedef enum
{
   LANG_FBD,
   LANG_LD,
   LANG_SFC,
   LANG_ST,    // should never occur in elements ...
} IEC_GR_Lang;

// hash key definitions for usage with CSet or CMap templates(
template<> 
inline UINT AFXAPI HashKey<CPoint> (CPoint key)
{
	return (key.y << 7) + key.x;
}
// hash key definitions)

typedef CSet<CPoint>    CPointSet;


class CFixPoints
{
public:
   CPointSet Std;   // default set
   CPointSet Cur;   // selected for operation
   void Serialize(CArchive& ar);
   bool Includes(const CPoint &P) const;          //
   bool Next(int& idx, CPoint& P);  // start search with idx==0
};


class CElemPtrList : public CPtrList  
{
   DECLARE_SERIAL(CElemPtrList)

public:
   bool         bValidPos;

   // public functions:
   CElemPtrList(int nBlockSize = 10);
	virtual ~CElemPtrList();
	virtual void Serialize(CArchive & ar);
   virtual void SerializeXMLSelected(CXMLArchive &ar, CString source);

   void         RegisterXMLTypes(CXMLArchive &ar);
	void         SerializeSelected(CArchive & ar);
	void         AddAllElemsToPinTable();

   bool         RouteToSignalInit    (TElemFlags EF, bool bRouteComment); // prepare for routing (setup r-matrix)
   bool         RouteFlaggedLines    (TElemFlags EF); // route partially (false: call again; true: finished)
   bool         RouteToPointInit     (CPoint P);      // origin for incremental routing
   bool         RouteToPoint         (CPoint P);      // do incremental routing
   void         RouteToSignalDone    (TElemFlags EF); // clean up the router (opt: insert rubbLines)
   void         RouteToPointDone     ();              // clean up the router

   void         ReRouteDeconnect     (bool bInsRubber, bool bReRoute=true);
   bool         ReRouteDone          ();              // returns true if elements were modified

   bool         UnrouteFlaggedLines  (bool bComment, TElemFlags EF=EF_SELECT);

	bool         ResizeSelElems       (CPoint& p, RS_TYPE eTyp);
	bool         MoveSelElems         (CPoint& p);

   void         InsertElem           (CElem *pE);
   void         ReplaceElem          (CElem *pEold, CElem *pEnew);
   void         DeleteElem           (CElem *pE);
   void         RemoveElem           (CElem *pE);
   void         RemoveElemAt         (POSITION pos);  // same as RemoveElem (but without searcing)
	void         DeleteAllElems       ();
	void         DeleteFlaggedElems   (CElemFlag flag);

	void         GetAllElemRect       (CRect& r);
   void         GetSelectRect        (CRect& r);
   bool         IsOneSelected        ();
   void         UnselectAll          (void);
	void         ToggleFlagsByRect    (TElemFlags flag, CRect &r);

   CElem       *SearchElem           (CPoint p, int iGrid);

   bool         IsOneOverlapped      (bool bNoActCheck=false, CElemFlag FMask=CElemFlag());
   bool         IsElemOverlapped     (CElem *pE, bool bUpdateContainers = true);

   const CPinTable* GetPinTable      () {return m_pPinTable;}

private:
   void         UpdateContainers     (CElem *pE);

   bool         IsElemOverlapped     (CElem *pE1, POSITION pos2, bool bNoActCheck, CElem* pEnd);
	bool         FlagSignalInt        (CPoint p, CElemFlag flag, bool bRem, bool bWire=false);

   void         RouterTrackRubber    (TElemFlags EF); // prepare rubber lines at special elements (PRail, DivCon)
   bool         RouterAddElems       (TElemFlags EF); // regiser locked areas
   bool         RouterAddSelSignals  (TElemFlags EF); // register signals for routing (toSignal-routing)
   bool         RouterCreate         (bool bFull);    // allocates the router; arg==false: detect used size

   bool         InsertSpace_OLD      (CPoint p, CPoint pAT, bool bLabelClip);
   bool         InsertSpaceDo_OLD    (CElem *pE, int dx, int dy, CPoint pAT, bool bLabelClip);

   int          WireToRubbLine       (CPoint& P, TElemFlags EF, bool bComment); // result == 0: ready
   void         GetSignalPoints      (CPoint P, CPointSet& PSet, bool bComment);
   int          GetAutoElems         (CElem* pE, CElemPtrList* pL);
   bool         CanJoinElems         (CElem* pE1, CElem* pE2, bool bAll);

public:
   void         SignalChangeInit     (bool bActivate);
   int          SignalChangeCount    ();
   void         CreateTempVars       (bool bCopySelection); // creates temp vars at FBs inside LD area, marks the existing ones
   void         PurgeTempVars        (); // removes conflicting temp vars
   void         ClearConflictingTempVars();
   
   void         GetStatisticData     (CStatisticData& data);
   // to decide (public/private):
	void         ConnectAllElems      ();
	void         DeconnectSelElems    (bool bInsertRubber);
   bool         DropInSelectedElems  (); // true if modifications performed
   bool         DropOutSelElems      (); // true if modifications performed
   void         DropOutPurge         ();
   void         PurgeOpenRubberLines (TElemFlags EF=EF_SELECT);   // remove flagged and open rubber lines 
   void         PurgeRedundantRubber ();
   void         UpdateGroupSelect    ();

   CRect        GetContextRect       (CPoint pAT, /*int& lYmin, int& lYmax, */bool bLabelClip);
   bool         InsertSpace          (CPoint p, CPoint pAT, bool bLabelClip);
   bool         CheckSpace           (CPoint p, CPoint pAt, bool bLabelClip);
	bool         SelectSignal         (CPoint p, bool bSet, bool bRem=false);

   bool         FlagSignal           (CPoint p, CElemFlag flag, bool bRem=false, bool bWire=false);
	bool         SetFlags             (TElemFlags flag);
	bool         ClearFlags           (TElemFlags flag);  // single   flag
	bool         ClearFlags           (CElemFlag  flag);  // multiple flags
   void         UpdatePinStatus      (bool bInsMode=false); 
   CElem       *MoveFlaggedToTail    (CElemFlag flag);

	CElem       *GetSelectElem        ();
   CElem       *SearchPin            (CPoint p, POSITION &pos);
   CElemLine   *SearchLine           (CPoint p1, CPoint p2, bool bRubber);
   CElem       *GetCurElem           (CPoint p);

   CElem       *GetNextElem          (CPoint& p, BOOL bNext);

   bool         LinkLines            (CPoint p, bool boAll=true);
   bool         LinkLinesInt         (TPinEntry *pPE, bool boAll=true);
	bool         BreakLines           (CPoint p);

   bool         FCCBFilter           (CElem **pE);
	void         GenXMLData           (CString &strXML, const CString &pathName);

   IEC_GR_Lang  GetIECLanguange      ();
   IEC_GR_Lang  GetIECLanguange      (CPoint& p, int iGrid);
   void         SetIECLanguage       (IEC_GR_Lang Language);
   void         RecoverIECLang       ();
   bool         ToggleFixPoint       (CPoint PAt);
   bool         GetNextFixPoint      (int& idx, CPoint& P);

   void         InitMoveFixPoints    ();
   void         MoveFixPoints        (CSize delta);
   void         DoneMoveFixPoints    ();

   void         RecoverTempVars      ();
   CElem*       FindNextTempVar      (CElem* pStart=NULL);
   void         FlagTempVarPins      ();

private:
   class CAutoRouter* m_pRouter;
   bool         m_bReRoute;
   CPinTable   *m_pPinTable;
   bool         m_bCountSignalChanges;

   // internal functions
   bool         ResizeDo             (CElem *pE, int dx, int dy, RS_TYPE eTyp);

   IEC_GR_Lang  m_IECLang;
   CFixPoints   m_FixPoints;
};


typedef CTypedPtrList<CElemPtrList, CElem*> CElemList; // simplified pointer list

#endif // !defined(AFX_ELEMPTRLIST_H__13D0E5C1_FB83_11D1_8E7B_444553540000__INCLUDED_)
