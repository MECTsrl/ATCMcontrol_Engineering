#if !defined(AFX_ELEMSFC_H__36A0A361_4EDA_11D2_810B_444553540000__INCLUDED_)
#define AFX_ELEMSFC_H__36A0A361_4EDA_11D2_810B_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Elem.h"

class CElemStep : public CElem  
{
   DECLARE_SERIAL(CElemStep)

private:
	BOOL    m_bInitial;
   CText   m_Name;

public:
	CElemStep();
	CElemStep(CPoint Origin, CSize Size, LPCTSTR pszName, BOOL bInitial=FALSE);
	virtual ~CElemStep();
	virtual void     Draw(CDC *pDC, CGrEditorView* pView, int iGrid);
   virtual BOOL     FindText(SearchDat& SD);
	virtual CText&   GetTextAt(CPoint p, CPoint &start, int iGrid=1);
	        void     SetName(LPCTSTR pszName);
	const   CString& GetName();
	        BOOL     GetInitType(void) { return m_bInitial; }
	        void     SetInitType(BOOL bInitial) { m_bInitial = bInitial; }
	        void     SetPinAttr(void);
   virtual bool     SetAutoSize(CPoint& dorigin, CSize& dsize);
	virtual void     GetBodyRect(CRect& r, int iGrid);
	virtual bool     GetText(int iElemNr, int iPos, int iLen, CString& str);
	virtual bool     Resize(int dx, int dy, int sx=0, int sy=0);
   virtual void     ReplaceSelText(int iElemNr, int iPos, int iLen, LPCTSTR pstr);
   virtual void     GetTextRect(int iTxtIdx, CRect &rect, int iGrid);
	virtual void     Serialize(CArchive & ar);
   virtual void     SerializeXML(CXMLArchive &ar);
   virtual void     SerializeXMLCon(CXMLArchive &ar);
   virtual bool     JoinTest(TDirection Dir, CElem* pE);
   virtual bool     CheckDropIn( CElem* pE);
   virtual bool     CheckDropOut();
   virtual bool     Spacing    ( CPoint Point, CPoint Delta, CPosArray*  pPin, bool bLabelClip);
};

class CElemTran : public CElem  
{
   DECLARE_SERIAL(CElemTran)

protected:
   CText     m_Name;

public:
	CElemTran();
	CElemTran(CPoint Origin, CSize Size, LPCTSTR pszName);
	virtual ~CElemTran();
	virtual void     Draw(CDC *pDC, CGrEditorView* pView, int iGrid);
   virtual BOOL     FindText(SearchDat& SD);
	virtual CText&   GetTextAt(CPoint p, CPoint &start, int iGrid=1);
	        void     SetName(LPCTSTR pszName);
	const   CString& GetName();
   virtual bool     SetAutoSize(CPoint& dorigin, CSize& dsize);
	virtual void     GetBodyRect(CRect& r, int iGrid);
	virtual bool     GetText(int iElemNr, int iPos, int iLen, CString& str);
   virtual void     ReplaceSelText(int iElemNr, int iPos, int iLen, LPCTSTR pstr);
   virtual void     GetTextRect(int iTxtIdx, CRect &rect, int iGrid);
	virtual void     Serialize(CArchive & ar);
   virtual void     SerializeXML(CXMLArchive &ar);
   virtual void     SerializeXMLCon(CXMLArchive &ar);
   virtual bool     CheckDropIn( CElem* pE);
   virtual bool     CheckDropOut();
   virtual bool     Spacing( CPoint Point, CPoint Delta, CPosArray*  pPin, bool bLabelClip);
};

#define  Q_WIDTH  3

class CElemAct : public CElemContain  
{
   DECLARE_SERIAL(CElemAct)

private:
   static const LPCTSTR m_strQual[QT_LAST];
   static UINT          m_NextID;
	QUAL_TYPE            m_eQualifier;
   CText                m_Name;
	CText                m_Time;
	int                  m_iTClen;
   CText                m_STtext;
   UINT                 m_ID;

public:
	CElemAct();
   CElemAct(CPoint Origin, CSize Size, LPCTSTR pszName, QUAL_TYPE eQualifier);
	virtual ~CElemAct();

   virtual void      Draw(CDC *pDC, CGrEditorView* pView, int iGrid);
   virtual BOOL      FindText(SearchDat& SD);
	virtual CText&    GetTextAt(CPoint p, CPoint &start, int iGrid=1);
	        void      SetName(LPCTSTR pszName);
	const   CString&  GetName();
	        void      SetSTtext(LPCTSTR pszSTtext);
	        CText&    GetSTtext();
           void      GetNameRect(CRect &r, int iGrid);
           void      GetQualRect(CRect &r, int iGrid);
           void      GetTimeRect(CRect &r, int iGrid);
	virtual void      GetBodyRect(CRect& r, int iGrid);
   virtual int       GetMinTxtIdx() { return -3; }
   virtual void      GetTextRect(int iTxtIdx, CRect &rect, int iGrid);
	virtual bool      GetText(int iElemNr, int iPos, int iLen, CString& str);
   virtual void      ReplaceSelText(int iElemNr, int iPos, int iLen, LPCTSTR pstr);
           QUAL_TYPE GetQualifier(void) { return m_eQualifier; }
	virtual void      SetQualifier(QUAL_TYPE eQualifier) { m_eQualifier = eQualifier; }
           void      SetTime(CString & strTime) { m_Time = strTime; }
           void      GetTime(CString & strTime) { strTime = (CString&)m_Time; }
	virtual void      Serialize(CArchive & ar);
   virtual void      SerializeXML(CXMLArchive &ar);
   virtual void      SerializeXMLCon(CXMLArchive &ar);
   virtual bool      JoinTest(TDirection Dir, CElem* pE);
   virtual bool      IsSpaceable(CPoint At, CPoint Delta, bool bLabelClip);
   virtual bool      Spacing (CPoint Point, CPoint Delta, CPosArray*  pPin, bool bLabelClip);

           UINT      GetID()        { return m_ID; }
private:
	        void      CElemActInit(void);
};

class CElemAction : public CElemAct
{
   DECLARE_SERIAL(CElemAction)

public:
   CElemAction() { SetType(ET_ACTION); }
	CElemAction(CPoint Origin, CSize Size, CText &Name, QUAL_TYPE eQualifier) :
      CElemAct(Origin, Size, Name, eQualifier ) { SetType(ET_ACTION); }

   virtual bool     JoinTest(TDirection Dir, CElem* pE);
};

class CElemActBlk : public CElemAct
{
   DECLARE_SERIAL(CElemActBlk)

public:
	CElemActBlk() { CElemActBlkInit(); }
	CElemActBlk(CPoint Origin, CSize Size, CText &Name, QUAL_TYPE eQualifier) :
      CElemAct(Origin, Size, Name, eQualifier ) { CElemActBlkInit(); }
	void CElemActBlkInit(void);

   virtual bool     JoinTest(TDirection Dir, CElem* pE);
};

class CElemDiv : public CElem  
{
   DECLARE_SERIAL(CElemDiv)

public:
	CElemDiv();
	CElemDiv(CPoint Origin, CSize Size);
	virtual ~CElemDiv();
	virtual void     Draw(CDC *pDC, CGrEditorView* pView, int iGrid);
	virtual void     DrawSelectFrame(CDC *pDC, CGrEditorView *pView, int iGrid, bool bWithResizePoints=true);
	virtual RS_TYPE  GetResizeType(CPoint p, int iGrid);
	virtual bool     IsResizeRect(CPoint p, int iGrid);
	virtual bool     Resize(int dx, int dy, int sx=0, int sy=0);
	virtual void     Serialize(CArchive & ar);
   virtual void     SerializeXML(CXMLArchive &ar);
   virtual bool     IsSpaceable(CPoint At, CPoint Delta, bool bLabelClip);
   virtual bool     Spacing( CPoint At, CPoint Delta, CPosArray* pPin, bool bLabelClip);
   virtual bool     CheckDropIn( CElem* pE);
};

class CElemAltDiv : public CElemDiv
{
   DECLARE_SERIAL(CElemAltDiv)

public:
	CElemAltDiv() { SetType(ET_ALTDIV); }
	CElemAltDiv(CPoint Origin, CSize Size) : 
      CElemDiv(Origin, Size ) { SetType(ET_ALTDIV); }
	CElemAltDiv(CElemDiv *pE);
};

class CElemSimDiv : public CElemDiv
{
   DECLARE_SERIAL(CElemSimDiv)

public:
	CElemSimDiv() { SetType(ET_SIMDIV); }
	CElemSimDiv(CPoint Origin, CSize Size) : 
      CElemDiv(Origin, Size ) { SetType(ET_SIMDIV); }
	CElemSimDiv(CElemDiv *pE);
};


#endif // !defined(AFX_ELEMSFC_H__36A0A361_4EDA_11D2_810B_444553540000__INCLUDED_)
