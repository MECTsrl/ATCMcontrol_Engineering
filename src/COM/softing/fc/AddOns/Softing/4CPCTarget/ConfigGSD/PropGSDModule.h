#if !defined(AFX_PROPGSDMODULE_H__97EA13E3_0420_11D2_81DC_00104B43B0E0__INCLUDED_)
#define AFX_PROPGSDMODULE_H__97EA13E3_0420_11D2_81DC_00104B43B0E0__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PropGSDModule.h : header file
//

#include "resource.h"
#include "PropGSDSlaveModule.h"
#include "HexEdit.hpp"	// Added by ClassView
#include "HexEdit.hpp"
#include "ExtParam.h"

void ModifyParamArray(CByteArray & , ExtParam *);
BYTE MakeBitFieldInByte(DWORD Start, DWORD Length);

/////////////////////////////////////////////////////////////////////////////
// PropGSDModule dialog

class PropGSDModule : public CPropertyPage
{
	DECLARE_DYNCREATE(PropGSDModule)

// Construction
public:
	PropGSDModule(PropGSDSlaveModule* pModule);
	PropGSDModule();
	~PropGSDModule();

	void AddExtUserParam(ExtParam* pElement);
    void AddDefaultUserPrmData(ExtParam* pExtParam);
	BOOL SetUserChanged(BOOL bNew);
	BOOL GetUserPrmData(CByteArray* pUserPrms);
	BOOL SetUserPrmData(VARIANT& rvtBytes);
    CString GetState();
    BOOL SetState(LPCTSTR &buffer);

	inline void SetUserPrmLen(DWORD dwLen);

// Dialog Data
	//{{AFX_DATA(PropGSDModule)
	enum { IDD = IDD_PROP_MODULE_SPECIFIC };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(PropGSDModule)
	protected:
	virtual BOOL OnSetActive();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(PropGSDModule)
	virtual BOOL OnInitDialog();
	afx_msg void OnDevicePrmAdd();
	afx_msg void OnDblclkAvailableExtUserParam(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDevicePrmRemove();
	afx_msg void OnDevicePrmRemoveAll();
	afx_msg void OnDblclkListUsedExtUserParams(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeUsedModules();
	afx_msg void OnDblclkAvailModExtPrm(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnModuleExtprmAdd();
	afx_msg void OnModuleExtprmRemove();
	afx_msg void OnModuleExtprmRemoveAll();
	afx_msg void OnDblclkUsedModExtPrm(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void InitListCrtls();
	void FillDeviceExtUserPrm();
	void CreateExtUserParams();
private:
	void FillModuleExtUserPrm(CDP_Module* pModule);

	BOOL		m_bUserChanged;
	BOOL		m_bExtUserPrms;			// is TRUE if Extended Userprmdata are used
	DWORD		m_dwUserPrmDataLen;		// Number of Userparambytes
	PropGSDSlaveModule* m_pModule;
	CHexEdit	m_ctrlUserPrm;
	CByteArray	m_DefaultUserPrmData;
	CByteArray	m_ExtUserPrmData;
	CObArray	arModules;
	CObList		ExtUserParams;
	CDP_Module* m_pSelModule;
};

inline void PropGSDModule::SetUserPrmLen(DWORD dwLen)	{
	m_dwUserPrmDataLen = dwLen;
}

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPGSDMODULE_H__97EA13E3_0420_11D2_81DC_00104B43B0E0__INCLUDED_)
