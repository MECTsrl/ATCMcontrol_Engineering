/*H>> $Header: /4CReleased/V2.20.00/COM/softing/fc/CE/GREditor/Elem.h 1     28.02.07 18:58 Ln $
 *----------------------------------------------------------------------------*
 *
 * =FILENAME            $Workfile: Elem.h $
 *   $Logfile: /4CReleased/V2.20.00/COM/softing/fc/CE/GREditor/Elem.h $
 *
 * =PROJECT             CAK1020    ATCMControl V2.0
 *
 * =SWKE                CE
 *
 * =COMPONENT           GREDITOR
 *
 * =CURRENT      $Date: 28.02.07 18:58 $
 *               $Revision: 1 $
 *
 * =REFERENCES
 *
 *----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*
 * =DESCRIPTION
 *    
 *==
 *----------------------------------------------------------------------------*
 * =MODIFICATION
 *  20.02.01  HA/RW      File created
 *  see history at end of file !
 *==
 *******************************************************************************
H<<*/

#if !defined(AFX_ELEM_H__E46EFCED_FAF7_11D1_8E7B_444553540000__INCLUDED_)
#define AFX_ELEM_H__E46EFCED_FAF7_11D1_8E7B_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/*inc*=======================================================================*/
#include "GrEditorBase.h"
#include "Flag.h"

/*const*=====================================================================*/
/*macro*=====================================================================*/
/*type*======================================================================*/

typedef enum 
{
   // permanent element flags for archiv
   EF_COMMENT=0,   // 0x000001 xml  element is only comment
   EF_RUBBLINE,    // 0x000002 xml  element is rubberband line 
   EF_STACTION,    // 0x000004 xml  element is a ST action(block)
   // to be set in constructor or serialize(XML)
   EF_EXT_INP,     // 0x000008 set  element has extensible inputs (FB/FCT/PRO)
   EF_EXT_OUTP,    // 0x000010 set  element has extensible outputs (FB/FCT/PRO)
   EF_NO_INNAME,   // 0x000020 set  element has no named inputs
   EF_NO_OUTNAME,  // 0x000040 set  element has no named outputs
   EF_CONTAIN,     // 0x000080 set  element can have a group of kind elements (Act)
   EF_NEW_LAYOUT,  // 0x000100 set  element was new layouted (created) 
   // temporary element flags for processing
   EF_SELECT,      // 0x000200 tmp  element is selected
   EF_EMPHASIZE,   // 0x000400 tmp  element is emphasize
   EF_UPDATE,      // 0x000800 tmp  element must updated on screen
   EF_PROCESSED,   // 0x001000 tmp  element was already processed
   EF_CURRENT,     // 0x002000 tmp  element is current selected to process
   EF_EDITLINE,    // 0x004000 tmp  element is line in edit mode
   EF_SFC_FORCED,  // 0x008000 tmp  element (SFC) is forced
   EF_SFC_BLOCKED, // 0x010000 tmp  element (SFC) is blocked
   EF_LIGHTPIN,    // 0x020000 tmp  element has highlighted pins
   EF_AUTOELEM,    // 0x040000 tmp  automatic generated element 
   EF_TEMPELEM,    // 0x080000 tmp  marks temporary generated elements

   EF_LDACTION,    // 0x100000 set  element a LD-action (otherwise FBD-action)

   EF_LAST
} TElemFlags;

typedef enum 
{
   // permanent pin flags for archiv
   PF_NEGATE=0,    // 0x000001 xml  pin is negated
   PF_DISABLED,    // 0x000002 xml  pin is not visible and not accessible (FB,..)
   PF_HIDDEN,      // 0x000004 xml  pin is hidden (size == CSize(0,0))
   PF_R_EDGE,      // 0x000008 xml  pin will be switch at rising edge 
   PF_F_EDGE,      // 0x000010 xml  pin will be switch at falling edge
   // to be set in constructor or serialize(XML)
   PF_CENTER_X,    // 0x000020 set  center the ONE pin at top/bottom (STEP/TRAN/ACT)
   PF_CENTER_Y,    // 0x000040 set  center the ONE pin at left/right (STEP/TRAN/ACT)
   PF_EXT_INP,     // 0x000080 set  pin is extensible inputs (FB/FCT/PRO)
   PF_EXT_OUTP,    // 0x000100 set  pin is extensible outputs (FB/FCT/PRO)
   PF_IN,          // 0x000200 set  pin is an in    (FB/FCT/PRO)
   PF_OUT,         // 0x000400 set  pin is an out   (FB/FCT/PRO)
   PF_INOUT,       // 0x000800 set  pin is an inout (FB/FCT/PRO)
   // temporary pin flags for processing
   PF_MODIFIED,    // 0x001000 tmp  pin was already processed
   PF_LIGHTPIN,    // 0x002000 tmp  pin was high lighted
   PF_PROCESSED,   // 0x004000 tmp  pin was already processed
   PF_SELECT,      // 0x008000 tmp  pin is at selected element
   PF_LADDER,      // 0x010000 set  EN/ENO-pin
   PF_AUTOVAR,     // 0x020000 position of a removed AutoVar element at FB-pin
   PF_LAST         // 0x040000 
} TPinFlags;

typedef enum 
{
   // permanent text flags for archiv
   TF_EDITABLE=0,  // set  text is editable 
   TF_MULTILINE,   // set  text has multiple lines (comment, STaction, ..)
   TF_EDIT_OE,     // set  text is to edit with oe
   TF_LEFT,        // set  text is left adjusted  (display)
   TF_RIGHT,       // set  text is right adjusted (display)
   TF_WRAP,        // set  text is to wrap at a) white space b) hard (display)
   TF_WITH_COLON,  // set  (display)
   // temporary text flags for processing
   TF_ABBR,        // tmp  pin was already processed

   TF_LAST
} TTextFlags;

// ----------------------------------------------------------------------------

typedef CFlag<TElemFlags>   CElemFlag;
typedef CFlag<TPinFlags>    CPinFlag;
typedef CFlag<TTextFlags>   CTextFlag;

/*glovar*====================================================================*/
/*proto*=====================================================================*/
typedef struct 
{
	CString     m_strFind;        // last find string
	CString     m_strReplace;     // last replace string
	bool        m_bCase;          // true == case sensitive, FALSE==not
	bool        m_bNext;          // true == search down, FALSE== search up
	bool        m_bWord;          // true == match whole word, FALSE==not
   bool        m_bFindOnly;      // true == find only, false == replace is active
   CPoint      m_FindCur;        // last cursor position for find/replace
   int         m_iFindElem;      // last element for find/replace
   int         m_iFindPos;       // last position in element(string) for find/replace
   int         m_iFindLen;       // length of element(string) for find/replace
} SearchDat;


class CText
{
private:
   CString   m_Str;

protected:
   CString     NextLine(int& iLine);
public:
   CTextFlag m_Flag;         // permanent and temp. flags

   CText() {}
   CText(const CText  &text) { m_Str = text.m_Str; m_Flag = text.m_Flag; }
   CText(const LPCTSTR  psz) { m_Str = psz; }
   CText(const CString &str) { m_Str = str; }

   bool operator==(const CText text) { return (m_Str == text.m_Str); }

   bool IsEmpty() const { return (m_Str.IsEmpty() == TRUE); }
   void Empty()         { m_Str.Empty(); }
   int  GetLength()     { return m_Str.GetLength(); }
   int  CountLines(void);
   void ProcessIdent(CString& str, CDC& dc, int iMaxLen, int iLineNo=0);
   static int  GetIdentLen (CString& str);
   
   int         GetNextLine(CString& txt, BOOL& bTrunc, CDC *pDC, int iWidth, int iStart, bool bLastLine);
   int         GetLine    (CString& txt, BOOL& bTrunc, CDC *pDC, int iWidth, int iLine);

   CSize       ScreenSize(CSize& scsize); // ret: in grid units;  arg out: in doc units

   const CText& operator =(const CText &text) { m_Str = text.m_Str; m_Flag = text.m_Flag; return *this; }
   const CText& operator =(const LPCTSTR psz) { m_Str = psz; return *this; }
   const CText& operator =(const CString &str) { m_Str = str; return *this; }

   operator LPCTSTR() const  { return (LPCTSTR)m_Str; }
   CString& StrRef()         { return m_Str; }
   const CString& Str()const { return m_Str; }
	friend CArchive& AFXAPI operator<<(CArchive& ar, const CText& text);
	friend CArchive& AFXAPI operator>>(CArchive& ar, CText& text);

   CSize Draw(CDC *pDC, CRect& r, int iGrid, COLOR_TYPE ColorType, SearchDat* pSD, int iconWidth);
   BOOL  Find(SearchDat& SD);
   void  Replace(SearchDat& SD);
/*
   void Draw(CDC *pDC, CRect& r, int iGrid, bool bSelection, COLOR_TYPE ColorType, 
                int iFindPos, int iFindLen);
*/
};

inline CArchive& AFXAPI operator<<(CArchive& ar, const CText& text)
{
	ar << text.m_Str << text.m_Flag;

	return ar;
}

inline CArchive& AFXAPI operator>>(CArchive& ar, CText& text)
{
	ar >> text.m_Str >> text.m_Flag;

	return ar;
}

//-----------------------------------------------------------------------------

struct TPinEntry;

class CPin : public CXMLObject   
{
protected:
   DECLARE_SERIAL(CPin)

//protected:
   TPinEntry *m_Entry;

public:
   CPinFlag  m_Flag;         // permanent and temp. flags
   CPoint    m_Pos;          // connect point position
   CSize     m_Size;         // connect point size
   CText     m_Name;         // pin name
   CString   m_strIVal;      // init value (input)
   CString   m_strType;      // pin type

   CPin();
   CPin(CPin * pPin);
   CPin(LPCTSTR pszName, LPCTSTR pszIVal, LPCTSTR pszType, CPoint pos, 
      CSize size, CPinFlag flags=CPinFlag());
   virtual ~CPin();
           void  SetType     (LPCTSTR pszType);
           void  SetIVal     (LPCTSTR pszIVal);
           void  SetName     (LPCTSTR pszName);
           void  Draw        (CDC * pDC, int iGrid);
           void  DrawLightPin(CDC * pDC, int iGrid);
   virtual void  Serialize   (CArchive& ar);
   virtual void  SerializeXML(CXMLArchive &ar);
           int   GetConnects();
           int   GetTemp();
           const TPinEntry* GetPinEntry() {return m_Entry;}
};

//-----------------------------------------------------------------------------

class CPinArray : public CTypedPtrArray<CObArray,CPin*> 
{
public:
   bool ClearFlags(CPinFlag flag);
};

/*func*------------------------------------------------------------------------
  clear all wished flags
  in :flag - flags to clear
  out:true  - one or more flags cleared
      false - no flag cleared
-----------------------------------------------------------------------------*/
inline bool CPinArray::ClearFlags(CPinFlag flag)
{
   bool  bChanged = false;

   for(int i=0; i<GetSize(); i++ )
   {  
      CPin* pPin = GetAt(i); 
      ASSERT(pPin);

      if (pPin->m_Flag & flag)
      {
         bChanged = true;
         pPin->m_Flag -= flag;
      }
   }
   return bChanged;
}

//-----------------------------------------------------------------------------

typedef enum
{
   DIR_N,
   DIR_S,
   DIR_E,
   DIR_W,
   DIR_LAST
} TDirection;

class CGrEditorView;
class CPosArray;
class CPinTable;
class CElemContain;

class CElem : public CXMLObject
{
   DECLARE_SERIAL(CElem)         
   //ATTENTION: the macro "DECLARE_SERIAL" sets next declarations to "public:"!!!
private:
   CElemFlag     m_Flag;         // permanent and temp. flags
   ELEM_TYPE     m_eElemTyp;     // typ of element

protected:
   friend class COvlTest; 

public:
	CPinArray     m_Con;          // connector list

	CElem();
	CElem(CElem * pE);
	virtual ~CElem();
   ELEM_TYPE        GetType() { return m_eElemTyp;}
           void     SetType(ELEM_TYPE ET){m_eElemTyp = ET;}

           CElemFlag        GetFlag(){return m_Flag;}
           void     SetFlag(TElemFlags flag);
           void     ClrFlag(TElemFlags flag);
           void     TglFlag(TElemFlags flag);
           bool     ChkFlag(TElemFlags flag);

           void     SetFlag(CElemFlag flag); // set   multiple flags
           void     ClrFlag(CElemFlag flag); // clear multiple flage

           CPinTable    *GetPinTable(void)                 { return m_pPinTable;      }
           void          SetPinTable(CPinTable *pPinTable) { m_pPinTable = pPinTable; }

           CElemContain *GetContainer(void)                { return m_pContain;  }
           void          SetContainer(CElemContain *pCont);

   virtual void     SwapOrigin(){};
           void     CopyData(CElem * pE, bool bCopyPins=true);
   virtual void     DeleteAllPin();
	virtual void     Draw(CDC *pDC, CGrEditorView* pView, int iGrid);
	virtual CSize    DrawIcon(CDC *pDC, CGrEditorView * pView, CPoint pos, UINT nIDBitmap, int iGrid);
           void     DrawPins(CDC * pDC, int iGrid);
           void     DrawLightPins(CDC * pDC, int iGrid);
     COLOR_TYPE     GetColorType(void);
   virtual void     DrawSelectFrame(CDC *pDC, CGrEditorView *pView, int iGrid, bool bWithResizePoints=true);
   virtual BOOL     FindText(SearchDat& SD);
	virtual bool     GetText(int iElemNr, int iPos, int iLen, CString& str);
   virtual void     ReplaceSelText(int iElemNr, int iPos, int iLen, LPCTSTR pstr);
	virtual bool     Resize(int dx, int dy, int sx=0, int sy=0);
	virtual CText&   GetTextAt(CPoint p, CPoint &start, int iGrid=1);
	virtual CPin    *SearchConn(CPoint p);
	        bool     IsConnectable(CPoint p);

	virtual CPin    *SearchPin(CPoint p, int iGrid);
           CPoint   GetOrigin()    const        { return m_Origin;   }
	        void     SetOrigin(CPoint& origin);
           CSize    GetSize()      const        { return m_Size;     }
	        void     SetSize(CSize& size);
   virtual bool     SetAutoSize(CPoint& dorigin, CSize& dsize){ dorigin=dsize=CPoint(0,0); return false;}
	virtual void     GetRect(CRect& r, int iGrid);
	virtual void     GetBoundingRect(CRect& r, int iGrid);
	virtual void     GetBodyRect(CRect& r, int iGrid);
   virtual void     GetTextRect(int iTxtIdx, CRect &rect, int iGrid);
   virtual bool     SearchTextRect(CPoint p, CRect &rect, int iGrid);
   virtual int      GetMinTxtIdx() { return -1; }
   virtual int      GetMaxTxtIdx() { return m_Con.GetSize(); }
	        CRect   *GetResizeRect(RS_TYPE eRectTyp, CRect& r, int iGrid);
	virtual RS_TYPE  GetResizeType(CPoint p, int iGrid);
	virtual bool     IsResizeRect(CPoint p, int iGrid);
	virtual void     GetResizePos(RS_TYPE eMode, CPoint& p);
           void     SetMinSize(CSize& Min);
           void     SetMaxSize(CSize& Max);
	virtual void     Serialize(CArchive& ar);
   virtual void     SerializeXML(CXMLArchive &ar);
           void     SerializeXMLCon(CXMLArchive &ar);

   virtual bool     JoinTest   ( TDirection, CElem*);
   virtual bool     CheckDropIn( CElem* pE);          // checks for Drop-In  possibility
   virtual bool     CheckDropOut();                   // checks for Drop-Out possibility

   virtual bool     IsSpaceable( CPoint At, CPoint Delta, bool bLabelClip);
   virtual bool     Spacing    ( CPoint At, CPoint Delta, CPosArray*  pPin, bool bLabelClip);

protected:
	CPoint        m_Origin;      // origin position on area in raster steps
	CPoint        m_OriginOffs;  // offset for origin to grid raster 
	CSize         m_Size;        // size of element on area in raster steps
	CSize         m_MaxSize;
	CSize         m_MinSize;

   CPinTable    *m_pPinTable;
   CElemContain *m_pContain;
};

//-----------------------------------------------------------------------------

class CElemContain : public CElem  
{
   DECLARE_SERIAL(CElemContain)

protected:
   CTypedPtrArray<CObArray,CElem*> m_Group;

public:
   CElemContain();
   virtual ~CElemContain();

           bool     IsMyElem(CElem *pE);
           void     DelGroupElem(CElem *pE);
           CElem*   GetGroupElem(int i);
           void     AddGroupElem(CElem *pE);
           void     EmptyGroup();
           int      GroupSize();
           bool     GroupCheckElemLD();
   virtual void     Serialize(CArchive & ar);
   virtual void     GetClientRect(CRect& R);
   void SetFlagGroup(TElemFlags flag);
   void ClrFlagGroup(TElemFlags flag);
};

//-----------------------------------------------------------------------------

class CElemCon : public CElem  
{
   DECLARE_SERIAL(CElemCon)

protected:
   CText     m_Name;

public:
   CElemCon();
   CElemCon(CPoint Origin, CSize Size, LPCTSTR pszName);
   virtual ~CElemCon();
   virtual void     Draw(CDC *pDC, CGrEditorView* pView, int iGrid);
   virtual BOOL     FindText(SearchDat& SD);
   virtual CText&   GetTextAt(CPoint p, CPoint &start, int iGrid=1);
   const   CString& GetName(void);
           void     SetName(LPCTSTR pszName);
   virtual bool     GetText(int iElemNr, int iPos, int iLen, CString& str);
   virtual void     ReplaceSelText(int iElemNr, int iPos, int iLen, LPCTSTR pstr);
   virtual void     Serialize(CArchive & ar);
   virtual void     SerializeXML(CXMLArchive &ar);
private:
           void     CElemConInit();
};

class CElemConIn : public CElemCon
{
   DECLARE_SERIAL(CElemConIn)

public:
	CElemConIn() { SetType(ET_CONIN); CreatePins(); }
	CElemConIn(CPoint Origin, CSize Size, LPCTSTR pszName) :
      CElemCon(Origin, Size, pszName ) { SetType(ET_CONIN); CreatePins(); }
	CElemConIn(CElemCon *pE) { CopyData(pE, false); SetType(ET_CONIN); SetName(pE->GetName()); CreatePins(); }

           void     CreatePins(void);
   virtual void     GetTextRect(int iTxtIdx, CRect &rect, int iGrid);
   virtual void     GetBodyRect(CRect& r, int iGrid) { GetRect(r, iGrid); r.right -= PIN_LENGTH*iGrid; }
   virtual bool     SetAutoSize(CPoint& dorigin, CSize& dsize);
};

class CElemConOut : public CElemCon
{
   DECLARE_SERIAL(CElemConOut)

public:
	CElemConOut() { SetType(ET_CONOUT); CreatePins(); }
	CElemConOut(CPoint Origin, CSize Size, LPCTSTR pszName) :
      CElemCon(Origin, Size, pszName ) { SetType(ET_CONOUT); CreatePins(); }
	CElemConOut(CElemCon *pE) { CopyData(pE, false); SetType(ET_CONOUT); SetName(pE->GetName()); CreatePins(); } 

           void     CreatePins(void);
   virtual void     GetTextRect(int iTxtIdx, CRect &rect, int iGrid);
   virtual void     GetBodyRect(CRect& r, int iGrid) { GetRect(r, iGrid); r.left += PIN_LENGTH*iGrid; }
   virtual bool     SetAutoSize(CPoint& dorigin, CSize& dsize);
};

//-----------------------------------------------------------------------------

class CElemLab : public CElem  
{
   DECLARE_SERIAL(CElemLab)

protected:
	BOOL      m_bLine;
   CText     m_Name;

public:
	CElemLab();
	CElemLab(CPoint Origin, CSize Size, LPCTSTR pszName, BOOL bLine);
	virtual ~CElemLab();
	virtual void     Draw(CDC *pDC, CGrEditorView* pView, int iGrid);
	virtual CText&   GetTextAt(CPoint p, CPoint &start, int iGrid=1);
   virtual BOOL     FindText(SearchDat& SD);
	const   CString& GetName(void);
	        void     SetName(LPCTSTR pszName);
           bool     GetLine() { return ((m_bLine==TRUE) ? true : false); }
	        void     SetLine(BOOL bLine) { m_bLine  = bLine; }
	virtual bool     GetText(int iElemNr, int iPos, int iLen, CString& str);
   virtual void     ReplaceSelText(int iElemNr, int iPos, int iLen, LPCTSTR pstr);
	virtual void     GetBodyRect(CRect& r, int iGrid);
	virtual void     GetBoundingRect(CRect& r, int iGrid);
	virtual void     Serialize(CArchive & ar);
   virtual void     SerializeXML(CXMLArchive &ar);
private:
   void CElemLabInit(void);
};

class CElemLabel : public CElemLab
{
   DECLARE_SERIAL(CElemLabel)

public:
   CElemLabel() { SetType(ET_LABEL); m_Name.m_Flag.Set(TF_WITH_COLON); }
	CElemLabel(CPoint Origin, CSize Size, LPCTSTR pszName, BOOL bLine) : 
      CElemLab(Origin, Size, pszName, bLine ) { SetType(ET_LABEL); m_Name.m_Flag.Set(TF_WITH_COLON); }
	CElemLabel(CElemLab *pE) 
   { CopyData(pE, false); SetType(ET_LABEL); m_Name.m_Flag.Set(TF_WITH_COLON); SetName(pE->GetName()); }

   virtual bool     SetAutoSize(CPoint& dorigin, CSize& dsize);
   virtual void     GetTextRect(int iTxtIdx, CRect &rect, int iGrid);
   virtual bool     JoinTest(TDirection, CElem* pE);
   virtual bool     Spacing ( CPoint At, CPoint Delta, CPosArray*  pPin, bool bLabelClip);
};

class CElemJump : public CElemLab
{
   DECLARE_SERIAL(CElemJump)

public:
	CElemJump() { SetType(ET_JUMP); CreatePins(); }
	CElemJump(CPoint Origin, CSize Size, LPCTSTR pszName, BOOL bLine) : 
      CElemLab(Origin, Size, pszName, bLine ) { SetType(ET_JUMP); CreatePins(); }
	CElemJump(CElemLab *pE) { CopyData(pE, false); SetType(ET_JUMP); SetName(pE->GetName()); CreatePins(); }

   virtual void     GetTextRect(int iTxtIdx, CRect &rect, int iGrid);
   virtual bool     SetAutoSize(CPoint& dorigin, CSize& dsize);
private:
   void CreatePins(void);
};

//-----------------------------------------------------------------------------

class CElemRet : public CElem  
{
   DECLARE_SERIAL(CElemRet)

protected:
   CText     m_Name;
   static CString m_strDefName;

public:
	CElemRet();
	CElemRet(CPoint Origin, CSize Size);
	virtual ~CElemRet();
           void     CElemRetInit(CPoint Origin=CPoint(0,1), CSize Size=CSize(6,2));
	virtual CText&   GetTextAt(CPoint p, CPoint &start, int iGrid=1);
   virtual BOOL     FindText(SearchDat& SD);
	virtual void     Draw(CDC *pDC, CGrEditorView * pView, int iGrid);
	virtual void     GetBodyRect(CRect& r, int iGrid);
	virtual bool     GetText(int iElemNr, int iPos, int iLen, CString& str);
   virtual void     ReplaceSelText(int iElemNr, int iPos, int iLen, LPCTSTR pstr);
   virtual void     GetTextRect(int iTxtIdx, CRect &rect, int iGrid);
	virtual void     Serialize(CArchive & ar);
   virtual void     SerializeXML(CXMLArchive &ar);
   static const CString&   GetDefName(){ return m_strDefName; }
};

//-----------------------------------------------------------------------------

#define VAR_AUTONAME _T("?")


class CElemVar : public CElem  
{
   DECLARE_SERIAL(CElemVar)

protected:
   CText     m_Name;

public:
	CElemVar();
	CElemVar(CPoint Origin, CSize Size, LPCTSTR pszName );
	virtual ~CElemVar();
	virtual void     Draw(CDC * pDC, CGrEditorView * pView, int iGrid);
   virtual BOOL     FindText(SearchDat& SD);
	virtual CText&   GetTextAt(CPoint p, CPoint &start, int iGrid=1);
   const   CString& GetName(void);
           void     SetName(LPCTSTR pszName);
	virtual bool     GetText(int iElemNr, int iPos, int iLen, CString& str);
   virtual void     ReplaceSelText(int iElemNr, int iPos, int iLen, LPCTSTR pstr);
	virtual void     GetBodyRect(CRect& r, int iGrid);
   virtual void     Serialize(CArchive & ar);
   virtual void     SerializeXML(CXMLArchive &ar);
};

class CElemConst : public CElemVar  
{
   DECLARE_SERIAL(CElemConst)

public:
	CElemConst() { SetType(ET_CONST); CreatePins(); }
	CElemConst(CPoint Origin, CSize Size, LPCTSTR pszName ) : 
      CElemVar(Origin, Size, pszName ) { SetType(ET_CONST); CreatePins(); }
	CElemConst(CElemVar *pE) { CopyData(pE, false); SetType(ET_CONST); SetName(pE->GetName()); CreatePins(); }

   virtual void     GetTextRect(int iTxtIdx, CRect &rect, int iGrid);
private:
   void CreatePins(void);
};

class CElemVarIn : public CElemVar  
{
   DECLARE_SERIAL(CElemVarIn)

public:
	CElemVarIn() { SetType(ET_VARIN); CreatePins(); }
	CElemVarIn(CPoint Origin, CSize Size, LPCTSTR pszName ) : 
      CElemVar(Origin, Size, pszName ) { SetType(ET_VARIN); CreatePins(); }
	CElemVarIn(CElemVar *pE) { CopyData(pE, false); SetType(ET_VARIN); SetName(pE->GetName()); CreatePins(); }

   virtual void     GetTextRect(int iTxtIdx, CRect &rect, int iGrid);
   virtual bool     SetAutoSize(CPoint& dorigin, CSize& dsize);
private:
   void CreatePins(void);
};

class CElemVarOut : public CElemVar
{
   DECLARE_SERIAL(CElemVarOut)

public:
	CElemVarOut() { SetType(ET_VAROUT); CreatePins(); }
	CElemVarOut(CPoint Origin, CSize Size, LPCTSTR pszName ) : 
      CElemVar(Origin, Size, pszName ) { SetType(ET_VAROUT); CreatePins(); }
	CElemVarOut(CElemVar *pE) { CopyData(pE, false); SetType(ET_VAROUT); SetName(pE->GetName()); CreatePins(); }

   virtual void     GetTextRect(int iTxtIdx, CRect &rect, int iGrid);
   virtual bool     SetAutoSize(CPoint& dorigin, CSize& dsize);
private:
   void CreatePins(void);
};

class CElemVarInOut : public CElemVar  
{
   DECLARE_SERIAL(CElemVarInOut)

public:
	CElemVarInOut() { SetType(ET_VARINOUT); CreatePins(); }
	CElemVarInOut(CPoint Origin, CSize Size, LPCTSTR pszName ) : 
      CElemVar(Origin, Size, pszName ) { SetType(ET_VARINOUT); CreatePins(); }
	CElemVarInOut(CElemVar *pE) { CopyData(pE, false); SetType(ET_VARINOUT); SetName(pE->GetName()); CreatePins(); }

   virtual bool     CheckDropIn( CElem* pE);
   virtual bool     CheckDropOut();

   virtual void     GetTextRect(int iTxtIdx, CRect &rect, int iGrid);
   virtual bool     SetAutoSize(CPoint& dorigin, CSize& dsize);
private:
   void CreatePins(void);
};

//-----------------------------------------------------------------------------

class CElemCmt : public CElem  
{
   DECLARE_SERIAL(CElemCmt)

private:
   CText     m_Text;

public:

	CElemCmt();
   CElemCmt(CPoint Origin, CSize Size, LPCTSTR pszText);
	virtual ~CElemCmt();
	virtual void     Draw(CDC * pDC, CGrEditorView * pView, int iGrid);
           bool     GetLine(int iNr, CString& str);
	virtual CText&   GetTextAt(CPoint p, CPoint &start, int iGrid=1);
   virtual BOOL     FindText(SearchDat& SD);
           LPCTSTR  GetText();
           void     SetText(LPCTSTR pszText);
	virtual bool     GetText(int iElemNr, int iPos, int iLen, CString& str);
   virtual void     ReplaceSelText(int iElemNr, int iPos, int iLen, LPCTSTR pstr);
   virtual void     GetTextRect(int iTxtIdx, CRect &rect, int iGrid);
   virtual bool     SetAutoSize(CPoint& dorigin, CSize& dsize);
	virtual void     Serialize(CArchive & ar);
   virtual void     SerializeXML(CXMLArchive &ar);
};

//-----------------------------------------------------------------------------

class CElemLine : public CElem  
{
   DECLARE_SERIAL(CElemLine)

public:
	void SwapOrigin();

	CElemLine();
	CElemLine(CElemLine * pLine);
	CElemLine(CPoint ps, CPoint pe, bool bRubber);
	virtual ~CElemLine();
	virtual void Draw(CDC * pDC, CGrEditorView * pView, int iGrid);
	virtual void DrawSelectFrame(CDC *pDC, CGrEditorView *pView, int iGrid, bool bWithResizePoints=true);
	virtual RS_TYPE  GetResizeType(CPoint p, int iGrid);
	virtual bool     IsResizeRect(CPoint p, int iGrid);
	virtual bool Resize(int dx, int dy, int sx=0, int sy=0);
   virtual BOOL     FindText(SearchDat& SD);
   virtual void Serialize(CArchive & ar);
   virtual void SerializeXML(CXMLArchive &ar);
   virtual bool IsSpaceable( CPoint At, CPoint Delta, bool bLabelClip);
   virtual bool Spacing(CPoint Point, CPoint Delta, CPosArray* paPos, bool bLabelClip);

   virtual void     GetBoundingRect(CRect& r, int iGrid);

   static  CSize    GetDefaultSize()            { return m_DefaultSize; }
   static  void     SetDefaultSize(CSize S)     { m_DefaultSize = S;    }

private:
           void CreatePins(void);
   static CSize m_DefaultSize;
};

// --------------------------------------------------------------

/*
 *----------------------------------------------------------------------------*
 *  $History: Elem.h $
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.02.07   Time: 18:58
 * Created in $/4CReleased/V2.20.00/COM/softing/fc/CE/GREditor
 * 
 * *****************  Version 1  *****************
 * User: Ln           Date: 28.08.03   Time: 16:30
 * Created in $/4Control/COM/softing/fc/CE/GREditor
 * 
 * *****************  Version 7  *****************
 * User: Rw           Date: 4.12.01    Time: 17:07
 * Updated in $/4Control/COM/softing/fc/CE/GREditor
 * 
 * *****************  Version 6  *****************
 * User: Rw           Date: 30.10.01   Time: 17:40
 * Updated in $/4Control/COM/softing/fc/CE/GREditor
 * 
 * *****************  Version 5  *****************
 * User: Rw           Date: 30.10.01   Time: 11:23
 * Updated in $/4Control/COM/softing/fc/CE/GREditor
 * 
 * *****************  Version 4  *****************
 * User: Rw           Date: 9.10.01    Time: 16:38
 * Updated in $/4Control/COM/softing/fc/CE/GREditor
 * 
 * *****************  Version 3  *****************
 * User: Rw           Date: 26.09.01   Time: 15:22
 * Updated in $/4Control/COM/softing/fc/CE/GREditor
 * local branch from ha at de closed
 * 
 * *****************  Version 2  *****************
 * User: Rw           Date: 13.06.01   Time: 13:37
 * Updated in $/4Control/COM/softing/fc/CE/GREditor
 * 
 * *****************  Version 1  *****************
 * User: Rw           Date: 23.02.01   Time: 11:08
 * Created in $/4Control/COM/softing/fc/CE/GREditor
 * initial version for V2.0
 * 
 *==
 *----------------------------------------------------------------------------*
*/

#endif // !defined(AFX_ELEM_H__E46EFCED_FAF7_11D1_8E7B_444553540000__INCLUDED_)
