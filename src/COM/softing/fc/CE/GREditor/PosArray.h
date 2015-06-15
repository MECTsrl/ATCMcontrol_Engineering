#if !defined(AFX_POSARRAY_H__643D3A13_248C_11D3_A5DD_00E029398C1C__INCLUDED_)
#define AFX_POSARRAY_H__643D3A13_248C_11D3_A5DD_00E029398C1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPosArray : public CDWordArray  
{
// Construction
public:
	CPosArray();
	virtual ~CPosArray();

// Attributes

// Operations
private:
   DWORD  ToDWORD(const CPoint);
   CPoint ToPoint(const DWORD dw);

// Overrides

// Implementation
public:
   void     AddElem(class CElem *pE);
   void     AddPoint(CPoint P);
   CPoint   GetAt( int nIndex );
   void     Join(CPosArray& pSrcArray);
   void     Drop(CPosArray& pSrcArray);
   int      Find(CPoint P);
   bool     FindNext(int* pP1, int* pP2, CPosArray& pSrcArray);
#ifdef _DEBUG
   bool     DebugDump(const CString& txt=""); // returns always true
#endif
};

#endif // !defined(AFX_POSARRAY_H__643D3A13_248C_11D3_A5DD_00E029398C1C__INCLUDED_)
