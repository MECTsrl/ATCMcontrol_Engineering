#if !defined(AFX_ELEMLD_H__INCLUDED_)
#define AFX_ELEMLD_H__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Elem.h"

class CElemPower : public CElem  
{
   DECLARE_SERIAL(CElemPower)
      
public:
   CElemPower();
   CElemPower(CPoint Origin, CSize Size);
   virtual ~CElemPower();
   virtual void     Draw(CDC *pDC, CGrEditorView* pView, int iGrid);
	virtual void     DrawSelectFrame(CDC *pDC, CGrEditorView *pView, int iGrid, bool bWithResizePoints=true);
	virtual RS_TYPE  GetResizeType(CPoint p, int iGrid);
	virtual bool     IsResizeRect(CPoint p, int iGrid);
	virtual bool     Resize(int dx, int dy, int sx=0, int sy=0);
   virtual void     Serialize(CArchive & ar);
   virtual void     SerializeXML(CXMLArchive &ar);
   virtual bool     IsSpaceable(CPoint At, CPoint Delta, bool bLabelClip);
   virtual bool     Spacing( CPoint At, CPoint Delta, CPosArray* pPin, bool bLabelClip );
   virtual bool     CheckDropIn( CElem* pE);
};

class CElemLdBase : public CElem  
{
   DECLARE_SERIAL(CElemLdBase)

protected:      
   CText        m_Name;
   LADDER_TYPE  m_Type;

public:
   CElemLdBase();
   CElemLdBase(CPoint Origin, CSize Size, LPCTSTR pszName);
   virtual ~CElemLdBase();
   virtual void     Draw(CDC *pDC, CGrEditorView* pView, int iGrid);
   virtual BOOL     FindText(SearchDat& SD);
	virtual CText&   GetTextAt(CPoint p, CPoint &start,int iGrid=1);
	        void     SetName(LPCTSTR pszName);
	        LPCTSTR  GetName();
           void     SetLDType(LADDER_TYPE t){m_Type = t;}
           LADDER_TYPE      GetLDType(){ return m_Type; }
   virtual void     GetBodyRect(CRect& r, int iGrid);
   virtual bool     SetAutoSize(CPoint& dorigin, CSize& dsize);
   virtual bool     GetText(int iElemNr, int iPos, int iLen, CString& str);
   virtual void     ReplaceSelText(int iElemNr, int iPos, int iLen, LPCTSTR pstr);
   virtual void     GetTextRect(int iTxtIdx, CRect &rect, int iGrid);
   virtual void     Serialize(CArchive & ar);
   virtual void     SerializeXML(CXMLArchive &ar);
// virtual bool     JoinTest(TDirection Dir, CElem* pE);
   virtual bool     CheckDropIn( CElem* pE);
   virtual bool     CheckDropOut();
           void     Invert();
   void CreatePins(void);
};

class CElemContact : public CElemLdBase
{
   DECLARE_SERIAL(CElemContact)
      
public:
   CElemContact() { SetType(ET_CONTACT); CreatePins(); }
   CElemContact(CPoint Origin, CSize Size, LPCTSTR pszName, LADDER_TYPE  eType) :
      CElemLdBase(Origin, Size, pszName ) { SetType(ET_CONTACT); m_Type = eType; CreatePins(); }
};

class CElemCoil : public CElemLdBase
{
   DECLARE_SERIAL(CElemCoil)
      
public:
   CElemCoil() { SetType(ET_COIL); CreatePins(); }
   CElemCoil(CPoint Origin, CSize Size, LPCTSTR pszName, LADDER_TYPE  eType) :
      CElemLdBase(Origin, Size, pszName ) { SetType(ET_COIL); m_Type = eType; CreatePins(); }
};

#endif // !defined(AFX_ELEMLD_H__INCLUDED_)
