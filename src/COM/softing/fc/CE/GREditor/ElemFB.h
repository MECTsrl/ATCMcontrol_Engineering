#if !defined(AFX_ELEMFB_H__6DB13C24_0C04_11D2_8E7C_444553540000__INCLUDED_)
#define AFX_ELEMFB_H__6DB13C24_0C04_11D2_8E7C_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Elem.h"

class CElemFblk : public CElem  
{
   DECLARE_SERIAL(CElemFblk)

protected:
	CText  m_Name;
	CText  m_Inst;
	CText  m_strVersionId;
   bool   m_bShowENO;      // current ENO-status
   bool   m_bENOdefault;   // default ENO-status outside a container
   int    m_LastFindElem;
   CArray<CString,CString> m_arrInExtNameStubs; // ++**++ JD extInParam

public:
	                 CElemFblk();
	virtual         ~CElemFblk();
           int      CalcMinHeight();
           int      CalcMaxHeight();
           int      CalcOptWidth();
           void     CheckPins();
           void     SetPinPos();
           void     SetPinSize();
           void     SetAttr();
	virtual void     Draw(CDC *pDC, CGrEditorView* pView, int iGrid);
           void     DrawPinText(CDC * pDC, CGrEditorView * pView, int iPinNr, 
                                CPin *pPin, int iGrid);
	virtual BOOL     FindText(SearchDat& SD);
	virtual bool     GetText(int iElemNr, int iPos, int iLen, CString& str);
   virtual void     ReplaceSelText(int iElemNr, int iPos, int iLen, LPCTSTR pstr);
	virtual CText&   GetTextAt(CPoint p, CPoint &start, int iGrid=1);
   virtual void     GetTextRect(int iTxtIdx, CRect &rect, int iGrid);
	virtual void     GetBodyRect(CRect& r, int iGrid);
   const   CString& GetName(void);
           void     SetName(LPCTSTR pszName);
   const   CString& GetInst(void);
           void     SetInst(LPCTSTR pszName);
	virtual bool     Resize(int dx, int dy, int sx=0, int sy=0);
	virtual void     Serialize(CArchive & ar);
   virtual void     SerializeXML(CXMLArchive &ar);
	virtual void     GetBoundingRect(CRect& r, int iGrid);

   virtual bool     IsSpaceable( CPoint At, CPoint Delta, bool bLabelClip );
   virtual bool     Spacing    ( CPoint Point, CPoint Delta, CPosArray* paPin, bool bLabelClip);
   virtual bool     CheckDropIn( CElem* pE);
   virtual bool     CheckDropOut();
           void     OnAutoENO(); // decide wether to show EN, ENO
           void     RecoverENOStatus();
           void     SetENODefault(bool bSet){m_bENOdefault = bSet;}
           bool     WithENOPins(){ return m_bShowENO;             }
           void     GetInExtNameStubs(); // ++**++ JD extInParam
};

class CElemFunc : public CElemFblk
{
   DECLARE_SERIAL(CElemFunc)

public:
   CElemFunc() { SetType(ET_FUNCTION); }
};

class CElemProc : public CElemFblk
{
   DECLARE_SERIAL(CElemProc)

public:
   CElemProc() { SetType(ET_PROCEDURE); }
};

class CElemFB : public CElemFblk
{
   DECLARE_SERIAL(CElemFB)

public:
   CElemFB() { SetType(ET_FB); SetInst(_T("")); }
   virtual int      GetMinTxtIdx() { return -2; }
};


#endif // !defined(AFX_ELEMFB_H__6DB13C24_0C04_11D2_8E7C_444553540000__INCLUDED_)
