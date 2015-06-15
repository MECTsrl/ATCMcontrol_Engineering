#if !defined(AFX_PAGEINSVARLIST_H__A09816F3_E5E7_11D2_A00A_00A024363DFC__INCLUDED_)
#define AFX_PAGEINSVARLIST_H__A09816F3_E5E7_11D2_A00A_00A024363DFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageInsVarList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageInsVarList dialog
class CCeWatchList;
class CCeWatchSheetInsVar;

class CPageInsVarList : public CPropertyPage
{
	DECLARE_DYNCREATE(CPageInsVarList)

// Construction
public:
	CPageInsVarList(CCeWatchList* pWatchList=NULL, CCeWatchSheetInsVar* pParent=NULL, CCeSymbol* pSymbolTable=NULL);
	~CPageInsVarList();

// Dialog Data
	//{{AFX_DATA(CPageInsVarList)
	enum { IDD = IDD_INSVARLIST };
	CListCtrl	m_VarListCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPageInsVarList)
	public:
	virtual BOOL OnApply();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPageInsVarList)
	virtual BOOL OnInitDialog();
	afx_msg void OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CCeWatchSheetInsVar*   m_pParent;
	CCeWatchList*     m_pWatchList;
	CCeSymbol*        m_pSymbolTable;
	bool              m_bActive;
	
	CArray<CCeSymbol*, CCeSymbol*> m_SymArray;

private :
	bool InsertItems(CCeSymbol* pVar);
	void Add();
	bool CanAdd();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGEINSVARLIST_H__A09816F3_E5E7_11D2_A00A_00A024363DFC__INCLUDED_)
