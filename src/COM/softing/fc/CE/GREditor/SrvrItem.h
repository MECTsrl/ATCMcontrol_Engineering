#if !defined(AFX_SRVRITEM_H__59119A26_6422_11D2_9A3D_0000C0D26FBC__INCLUDED_)
#define AFX_SRVRITEM_H__59119A26_6422_11D2_9A3D_0000C0D26FBC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGrEditorSrvrItem : public CDocObjectServerItem
{
	DECLARE_DYNAMIC(CGrEditorSrvrItem)

// Constructors
public:
	CGrEditorSrvrItem(CGrEditorDoc* pContainerDoc);

// Attributes
	CGrEditorDoc* GetDocument() const
		{ return (CGrEditorDoc*)CDocObjectServerItem::GetDocument(); }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGrEditorSrvrItem)
	public:
	virtual BOOL OnDraw(CDC* pDC, CSize& rSize);
	virtual BOOL OnGetExtent(DVASPECT dwDrawAspect, CSize& rSize);
protected:
	virtual void OnShow();
	virtual void OnHide();
	//}}AFX_VIRTUAL

// Implementation
public:
	~CGrEditorSrvrItem();
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

#endif // !defined(AFX_SRVRITEM_H__59119A26_6422_11D2_9A3D_0000C0D26FBC__INCLUDED_)
