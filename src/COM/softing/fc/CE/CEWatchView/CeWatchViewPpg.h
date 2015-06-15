#if !defined(AFX_CEWATCHVIEWPPG_H__FA85C92F_E5B6_11D2_A00A_00A024363DFC__INCLUDED_)
#define AFX_CEWATCHVIEWPPG_H__FA85C92F_E5B6_11D2_A00A_00A024363DFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// CeWatchViewPpg.h : Declaration of the CCeWatchViewPropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CCeWatchViewPropPage : See CeWatchViewPpg.cpp.cpp for implementation.

class CCeWatchViewPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CCeWatchViewPropPage)
	DECLARE_OLECREATE_EX(CCeWatchViewPropPage)

// Constructor
public:
	CCeWatchViewPropPage();

// Dialog Data
	//{{AFX_DATA(CCeWatchViewPropPage)
	enum { IDD = IDD_PROPPAGE_CEWATCHVIEW };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CCeWatchViewPropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CEWATCHVIEWPPG_H__FA85C92F_E5B6_11D2_A00A_00A024363DFC__INCLUDED)
