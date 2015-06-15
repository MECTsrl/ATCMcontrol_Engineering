#if !defined(AFX_PROPGSDSLAVEMODULE_H__C8445533_FF6C_11D1_81D9_00104B43B0E0__INCLUDED_)
#define AFX_PROPGSDSLAVEMODULE_H__C8445533_FF6C_11D1_81D9_00104B43B0E0__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PropGSDSlaveModule.h : header file
//

#include "resource.h"
#include "DP_Module.h"

/////////////////////////////////////////////////////////////////////////////
// PropGSDSlaveModule dialog

class PropGSDSlaveModule : public CPropertyPage
{
	DECLARE_DYNCREATE(PropGSDSlaveModule)

// Construction
public:
	void SetModularType(BOOL bModular);
	BOOL CreateCfgByteArray(CByteArray* pCfgBytes);
			// Construct configbytes of all used modules
	BOOL GetUsedModules(CObArray* pModules);
			// get Array of Pointer of used Modules
	PropGSDSlaveModule();
			// Constructor
	~PropGSDSlaveModule();
			// Destructor
	BOOL SetMaxDataLen(LONG lLen);
			// set the DataLen number
	BOOL SetMaxModule(LONG lMaxModule);
			// set the number of max available modules
	BOOL SetMaxOutputLen(LONG lLen);
			// set the number of max outputbytes
	BOOL SetMaxInputLen(LONG lLen);
			// set the number of max inputbytes
	BOOL AddNewModule(CDP_Module* pModule);
			// Adds an Moduledefinition-Ptr to the existing Array
    CString GetState();
    BOOL SetState(LPCTSTR &buffer);

// Dialog Data
	//{{AFX_DATA(PropGSDSlaveModule)
	enum { IDD = IDD_PROP_GSD_MODULES };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(PropGSDSlaveModule)
	public:
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(PropGSDSlaveModule)
	virtual BOOL OnInitDialog();
	afx_msg void OnClkListModules(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListAvailableModules(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonRemoveAll();
	afx_msg void OnButtonRemove();
	afx_msg void OnOneDown();
	afx_msg void OnOneUp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL AddToUsedModules(CDP_Module* pModule, int iItempos = -1);
			// Adds an moduledescription to the usedmodelview
	BOOL BuildHexString(BYTE const* pCfgData, int icntBytes, CString& sCfgTxt);
			// Builds an Hexstring from an bytearray
private:
	void UpdateUI();
	BOOL m_bModular;
	LONG m_lIOLen;
	LONG m_lOutputLen;
	LONG m_lInputLen;
	CObArray m_UsedModules;
	BOOL CreateUsedModuleArray();
	LONG m_lMaxModule;
			// Counter of available modules
	LONG m_lUsedModules;
			// Counter of used modules
	LONG m_lMaxInputLen;
			// Max. number of Inputbytes
	LONG m_lMaxOutputLen;
			// Max. number of Outputbytes
	LONG m_lMaxDataLen;
			// Max. number of Input- and Outputbytes
	CObArray m_Modules;
			// contains all existing Modules
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPGSDSLAVEMODULE_H__C8445533_FF6C_11D1_81D9_00104B43B0E0__INCLUDED_)
