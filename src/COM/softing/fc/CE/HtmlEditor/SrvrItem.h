#if !defined(AFX_SRVRITEM_H__A04CED87_B51D_11D2_9FD9_00A024363DFC__INCLUDED_)
#define AFX_SRVRITEM_H__A04CED87_B51D_11D2_9FD9_00A024363DFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFcHtmlEdSrvrItem : public CDocObjectServerItem
{
	DECLARE_DYNAMIC(CFcHtmlEdSrvrItem)

// Constructors
public:
	CFcHtmlEdSrvrItem(CFcHtmlEdDoc* pContainerDoc);

// Attributes
	CFcHtmlEdDoc* GetDocument() const
		{ return (CFcHtmlEdDoc*)CDocObjectServerItem::GetDocument(); }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFcHtmlEdSrvrItem)
	public:
	virtual BOOL OnDraw(CDC* pDC, CSize& rSize);
	virtual BOOL OnGetExtent(DVASPECT dwDrawAspect, CSize& rSize);
	//}}AFX_VIRTUAL

// Implementation
public:
	~CFcHtmlEdSrvrItem();
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

#endif // !defined(AFX_SRVRITEM_H__A04CED87_B51D_11D2_9FD9_00A024363DFC__INCLUDED_)
