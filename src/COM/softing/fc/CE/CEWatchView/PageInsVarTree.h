
#if !defined(AFX_PAGEINSVARTREE_H__A09816F4_E5E7_11D2_A00A_00A024363DFC__INCLUDED_)
#define AFX_PAGEINSVARTREE_H__A09816F4_E5E7_11D2_A00A_00A024363DFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageInsVarTree.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageInsVarTree dialog
class CCeWatchList;
#define CEWATCH_TYPE_PC         0
#define CEWATCH_TYPE_RESOURCE   1
#define CEWATCH_TYPE_TASK       2
#define CEWATCH_TYPE_PROGRAM    3
#define CEWATCH_TYPE_FB         4
#define CEWATCH_TYPE_VAR        5
#define CEWATCH_TYPE_VARI       6

class CCeSymbol;
class CCeWatchSheetInsVar;

class CPageInsVarTree : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageInsVarTree)

// Construction
public:
	CPageInsVarTree(CCeWatchList* pWatchList=NULL, CCeWatchSheetInsVar* pParent=NULL, CCeSymbol* pSymbolTable=NULL);
	~CPageInsVarTree();

// Dialog Data
	//{{AFX_DATA(CPageInsVarTree)
	enum { IDD = IDD_INSVARTREE };
	CTreeCtrl	m_VarTreeCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageInsVarTree)
	public:
	virtual BOOL OnApply();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageInsVarTree)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemExpanding(NMHDR* pNMHDR, LRESULT* pResult);	// #2149 27.10.04 SIS >>
	afx_msg void OnKeyDown(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CCeWatchSheetInsVar*   m_pParent;
	CCeWatchList*     m_pWatchList;
	CCeSymbol*        m_pSymbolTable;
	CImageList        m_ImageList;
	bool              m_bActive;

private :
	bool InsertItems(HTREEITEM hParentItem, CCeSymbol* pVar);
	// #2149 27.10.04 SIS >>
	bool InsertTreeItem(HTREEITEM hParentItem, CCeSymbol* pVar);
	bool ExpandTree(HTREEITEM hParentItem, BOOL bOnlyFirstChild = FALSE);   // 23.08.06 SIS
	// #2149 27.10.04 SIS <<
	HTREEITEM GetItem(HTREEITEM hParentItem, const CString& name);
	unsigned CalcIconIndex(CCeSymbol* pVar);
	void Add(); 
	bool CanAdd();
	// 14.12.05 SIS >>
	// changed return type to LONG
	LONG ExpandItem(HTREEITEM hItem);
	// 14.12.05 SIS <<
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEINSVARTREE_H__A09816F4_E5E7_11D2_A00A_00A024363DFC__INCLUDED_)
