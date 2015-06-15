
#ifndef __LATEDLIST_H_
#define __LATEDLIST_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*
class CLATEDDoc;
//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*
class CLATEDList : public CListView
{
protected: // create from serialization only
	CLATEDList();
	DECLARE_DYNCREATE(CLATEDList)

public:
	//{{AFX_DATA(CLATEDList)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	CLATEDDoc* GetDocument();
    void DrawInto (CDC *pDC, CSize drawAreaSize);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLATEDList)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
    virtual void OnDraw (CDC *pDC);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLATEDList();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

    // Generated message map functions
protected:
	//{{AFX_MSG(CLATEDList)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
};

#ifndef _DEBUG  // debug version in latedview.cpp
inline CLATEDDoc* CLATEDList::GetDocument()
   { return (CLATEDDoc*)m_pDocument; }
#endif

#endif // __LATEDLIST_H_