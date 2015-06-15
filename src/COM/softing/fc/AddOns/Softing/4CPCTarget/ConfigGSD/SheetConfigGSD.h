
#if !defined(AFX_SHEETCONFIGGSD_H__788B7724_04FE_11D2_81DF_00104B43B0E0__INCLUDED_)
#define AFX_SHEETCONFIGGSD_H__788B7724_04FE_11D2_81DF_00104B43B0E0__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//----  Aggregate Includes:   --------------------------------------*
#include "PropGSDModule.h"
#include "PropGSDSlaveInfos.h"
#include "PropGSDSlaveModule.h"
#include "PropCfgUserData.h"
#include "PropErrors.h"
#include "ExtParam.h"
#include "ConfigGSD.h"

//----  Forward Class Definitions:   -------------------------------*

//----  Defines:    ------------------------------------------------*
#define CONFIG_GSD_STATE_VERSION  1
//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

/////////////////////////////////////////////////////////////////////////////
// CSheetConfigGSD

class CSheetConfigGSD : public CPropertySheet
{
	DECLARE_DYNAMIC(CSheetConfigGSD)

// Construction
public:
	CSheetConfigGSD();
	CSheetConfigGSD(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CSheetConfigGSD(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

	virtual ~CSheetConfigGSD();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSheetConfigGSD)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation

	// Generated message map functions
protected:
	//{{AFX_MSG(CSheetConfigGSD)
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Inline functions
public:
	void	SetHelpConnection(IConfigGSDHelp* pHelp);

public:
	BOOL	SetUserChanged(BOOL bNew);
	BOOL	HasExtUserPrm();
	BOOL	StationIsModular();
	BOOL	GetUserPrmData(CByteArray* pUserPrms);
	LONG	GetIdentNumber();
			// returns Indent of Slave
	BOOL	GetCfgByteArray(CByteArray* pCfgBytes);
			// Delivers the CfgBytes
	HRESULT	ParseAndInitPages();
			// read GSD-File and fill the Pages
	void	SetGSDFileName(CString szFileName);
			// Filename with full pathinformation
	HRESULT GetGSDObject(LPUNKNOWN* ppUnk);
			// returns an interface to the Parsers GSDObject
    CString GetState();
    HRESULT SetState(CString &state);

private:
	BOOL	EncodeSupportedBaudRates(VARIANT vtBaudrates);
			// Decode and create Supported Baudrates as WORD
	BOOL	InitPropSlaveInfos();
			// Init the Slaveinfos-Propertypage with its contents
	BOOL	InitPropSlaveModules();
			// Init Module-Proppertypage with its contents
	HRESULT	InitPropGSDModule();
			// Init Dialog with Modulespecific data
	BOOL	GetAndSaveConfigBytes(IModule* pIModule, CDP_Module* pModule);
			// Reads the Moduleconfigbytes and saves them in PropGSDSlaveModule
	void	InitSheet();
			// Init sheet
	BOOL	InitPropCfgUserData();
			// Init Dialog with Config and Userparameter
	void	InitErrorPage();
			// Init Dialog with Errormessages and errorcount
	static HRESULT GetExtUserParams(IExtUserParam* pParam, ExtParam* pExtParam);
			// gets ExtUserPrms from device or module
    static void SetExtParamType(IExtUserParam* pParam, ExtParam* pExtParam);

private:
	BOOL			m_bHasExtUserPrm;
					// True if Extended Userparameter exist
	BOOL			m_bIsModular;
					// True if Station is Modular
	BOOL			m_bSpecificView;
					// TRUE if Specific view is aktive
	IUnknown*		m_pUnknown;
					// Unknowninterface for Parser
	WORD			m_wSuppBaudRates;
					// Supported Baudrates
	CString			m_szFileName;	
					// Filename of GSD
	IParseGsdFile*	m_pParser;		
					// GSDParser - Interface 
	PropGSDSlaveInfos*	m_pSlaveInfos;
					// Page with Slaveinformation
	PropGSDSlaveModule*	m_pSlaveModule;
					// Page with all Modules of Slave
	PropGSDModule*	m_pModuleSpec;
					// Page with Modulespecific data
	PropCfgUserData* m_pCfgUserData;
					// Page with Confing and User data if compact slave
	PropErrors*		m_pErrorPage;
					// Page with Errormessages
	CComPtr<IConfigGSDHelp> m_pHelpConnection;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHEETCONFIGGSD_H__788B7724_04FE_11D2_81DF_00104B43B0E0__INCLUDED_)

