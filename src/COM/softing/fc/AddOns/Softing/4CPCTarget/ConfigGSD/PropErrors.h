#if !defined(AFX_PROPERRORS_H__509120D3_47B0_11D2_822B_00104B43B0E0__INCLUDED_)
#define AFX_PROPERRORS_H__509120D3_47B0_11D2_822B_00104B43B0E0__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PropErrors.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// PropErrors dialog

class PropErrors : public CPropertyPage
{
	DECLARE_DYNCREATE(PropErrors)

// Construction
public:
	PropErrors();
	~PropErrors();
	void AddErrorString(CString szError);
	inline void SetErrorCount(DWORD dwCount);

// Dialog Data
	//{{AFX_DATA(PropErrors)
	enum { IDD = IDD_PROP_ERRORS };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(PropErrors)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(PropErrors)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	DWORD		 m_dwErrorCount;
	CStringArray m_StringArray;
};

//------------------------ INLINE FUNCTION -----------------------------------*
inline void PropErrors::SetErrorCount(DWORD dwCount)
{
	m_dwErrorCount = dwCount;
}

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERRORS_H__509120D3_47B0_11D2_822B_00104B43B0E0__INCLUDED_)
