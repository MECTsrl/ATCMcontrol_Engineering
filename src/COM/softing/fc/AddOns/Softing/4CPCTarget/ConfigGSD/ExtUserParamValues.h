#if !defined(AFX_EXTUSERPARAMVALUES_H__43D26881_43CB_11D2_822A_00104B43B0E0__INCLUDED_)
#define AFX_EXTUSERPARAMVALUES_H__43D26881_43CB_11D2_822A_00104B43B0E0__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ExtUserParamValues.h : header file
//

#include "resource.h"
#include "ExtParam.h"

/////////////////////////////////////////////////////////////////////////////
// ExtUserParamValues dialog

class ExtUserParamValues : public CDialog
{
// Construction
public:
	ExtUserParamValues(ExtParam* pParam, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(ExtUserParamValues)
	enum { IDD = IDD_EXT_USER_PRM_VALUES };
	CComboBox	m_cboExtPrmValues;
	DWORD	m_dwParamMax;
	DWORD	m_dwParamMin;
	DWORD	m_dwOffset;
	//}}AFX_DATA
	
	ExtParam* m_pParam;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ExtUserParamValues)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ExtUserParamValues)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeExtPrmValues();
	virtual void OnOK();
	afx_msg void OnEditchangeExtPrmValues();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL IsValidParamValue(BOOL bFinalCheck = TRUE);
		// test if given entry is valid and in range

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXTUSERPARAMVALUES_H__43D26881_43CB_11D2_822A_00104B43B0E0__INCLUDED_)
