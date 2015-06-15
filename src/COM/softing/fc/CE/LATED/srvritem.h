// srvritem.h : interface of the CLATEDSrvrItem class
//

#if !defined(AFX_SRVRITEM_H__0EAB4DDB_85D0_4EFE_B6C6_AA1357E3DA52__INCLUDED_)
#define AFX_SRVRITEM_H__0EAB4DDB_85D0_4EFE_B6C6_AA1357E3DA52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLATEDSrvrItem : public CDocObjectServerItem
{
	DECLARE_DYNAMIC(CLATEDSrvrItem)

// Constructors
public:
	CLATEDSrvrItem(CLATEDDoc* pContainerDoc);

// Attributes
	CLATEDDoc* GetDocument() const
		{ return (CLATEDDoc*)CDocObjectServerItem::GetDocument(); }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLATEDSrvrItem)
	public:
	virtual BOOL OnDraw(CDC* pDC, CSize& rSize);
	virtual BOOL OnGetExtent(DVASPECT dwDrawAspect, CSize& rSize);
	//}}AFX_VIRTUAL

// Implementation
public:
	~CLATEDSrvrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SRVRITEM_H__0EAB4DDB_85D0_4EFE_B6C6_AA1357E3DA52__INCLUDED_)
