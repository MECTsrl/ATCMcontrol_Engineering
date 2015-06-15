#if !defined(AFX_PROPCFGUSERDATA_H__DB4A2403_099D_11D2_81E5_00104B43B0E0__INCLUDED_)
#define AFX_PROPCFGUSERDATA_H__DB4A2403_099D_11D2_81E5_00104B43B0E0__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PropCfgUserData.h : header file
//

#include "HexEdit.hpp"	// Added by ClassView
#include "resource.h"
#include "DP_Module.h"

/////////////////////////////////////////////////////////////////////////////
// PropCfgUserData dialog

class PropCfgUserData : public CPropertyPage
{
	DECLARE_DYNCREATE(PropCfgUserData)

// Construction
public:
	BOOL SetUserChanged(BOOL bNew);
	BOOL IsChangedByUser();
	BOOL CreateCfgByteArray(CByteArray* pCfgBytes);
	BOOL GetUserPrmData(CByteArray* pUserPrms);
	BOOL SetUserPrmData(VARIANT& vtUserPrm);
	PropCfgUserData();
	~PropCfgUserData();

// Dialog Data
	//{{AFX_DATA(PropCfgUserData)
	enum { IDD = IDD_PROP_USER_PRMS };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(PropCfgUserData)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(PropCfgUserData)
	virtual BOOL OnInitDialog();
	afx_msg void OnRestoreCfgData();
	afx_msg void OnRestoreUserData();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CByteArray m_DefaultCfgBytes;
	CByteArray m_DefaultUserPrm;
	BOOL m_bUserChanged;
	CHexEdit m_ctrlCfgData;
	CHexEdit m_ctrlUserPrm;
	CByteArray m_UserPrms;
	CByteArray m_CfgBytes;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPCFGUSERDATA_H__DB4A2403_099D_11D2_81E5_00104B43B0E0__INCLUDED_)
