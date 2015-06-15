// SrvrItem.h : interface of the CSTEditorSrvrItem class
//

#if !defined(AFX_SRVRITEM_H__9F791448_09AC_11D2_B9A5_006008749B3D__INCLUDED_)
#define AFX_SRVRITEM_H__9F791448_09AC_11D2_B9A5_006008749B3D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CSTEditorSrvrItem : public CDocObjectServerItem
{
	DECLARE_DYNAMIC(CSTEditorSrvrItem)

// Constructors
public:
	CSTEditorSrvrItem(CSTEditorDoc* pContainerDoc);

// Attributes
	CSTEditorDoc* GetDocument() const
		{ return (CSTEditorDoc*)CDocObjectServerItem::GetDocument(); }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSTEditorSrvrItem)
	public:
	virtual BOOL OnDraw(CDC* pDC, CSize& rSize);
	virtual BOOL OnGetExtent(DVASPECT dwDrawAspect, CSize& rSize);
	//}}AFX_VIRTUAL

// Implementation
public:
	~CSTEditorSrvrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SRVRITEM_H__9F791448_09AC_11D2_B9A5_006008749B3D__INCLUDED_)
