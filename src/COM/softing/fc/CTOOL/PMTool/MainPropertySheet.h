#if !defined(AFX_MAINPROPERTYSHEET_H__368A73EE_E6E3_4C36_91A2_BE522BF10423__INCLUDED_)
#define AFX_MAINPROPERTYSHEET_H__368A73EE_E6E3_4C36_91A2_BE522BF10423__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainPropertySheet.h : header file
//

#include "FirmwareData.h"


typedef CArray<CFirmwareData, CFirmwareData&> TFirmwareArr;

/////////////////////////////////////////////////////////////////////////////
// CMainPropertySheet

class CMainPropertySheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CMainPropertySheet)

// Construction
public:
	CMainPropertySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CMainPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:
    void SetStatusText(const CString& crstrText);
    
    void InitProgress(int iLower = 0, int iUpper = 25, BOOL bSub = FALSE);
    void SetProgress(int iPosition, int iUpper = -1, BOOL bSub = FALSE);
    void StopProgress(BOOL bSub = FALSE);

    BOOL IsStartup();

    BOOL LoadKADFile(const CString crstrTargetType, TFirmwareArr& rtFirmwareArr);

    CString GetTargetType();
    CString GetControlString( const CString& crstrAddress); 

    CString GetConnectInfo();

	CString GetCommChannel();

	BOOL GetSupportFileTransfer();		// SUPFILTRANS 24.05.07 SIS
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainPropertySheet)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainPropertySheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMainPropertySheet)
    afx_msg void OnLaunch4C();
    afx_msg void OnCommSettings();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
    void InitializeFromRegistry();
    void SaveToRegistry();

    BOOL GetConnectParameters(TFirmwareArr& rtFWArr);

    BOOL GetInstalledTargetTypes();

    BOOL Launch4C();

    BOOL LoadTargetInfo();
    
    BOOL ReadProperties(CXMLNode& node);
    BOOL ReadFirmware(CXMLNode& node, CFirmwareData& tFirmware);
    
    CString GetCommChannelDescription(const CString& crstrCommChannel);
    CString GetCommChannelFromDescription(const CString& crstrCommChannelDesc);
    CString GetStandardConnectParameter(const CString& crstrCommChannel);
    CString GetStandardProtocol(const CString& crstrCommChannel);
    CString GetStandardProtocolParameter(const CString& crstrCommChannel);
    CString BuildOPCInstName(const CString& crstrCommChannel, const CString& crstrConnectPar);

    BOOL GetStandardTargetCommInfo(CString& rstrTargetType, CString& rstrCommChannel, CString& rstrFirmwareVersion);
    
    void InformConnectInfoChanged();


protected:
    HICON           m_hIcon;                // main icon
//    HEVENT  m_hExitEvent;                 // exit event
    CButton         m_tLaunch4C;            // start 4C button
    CButton         m_tCommSettings;        // start communication settings dialog
    CEdit           m_tStatusEdit;          // status edit control
    CProgressCtrl   m_tProgressCtrlMain;    // main progress control
    BOOL            m_bProgressMain;        // is main progress control visible?
    CProgressCtrl   m_tProgressCtrlSub;     // sub progress control
    BOOL            m_bProgressSub;         // is sub progress control visible?
    CString         m_strStatusText;        // status text

    int     m_iActivePage;      // active page (only used for start)
    BOOL    m_bStartup;         // startup? needed for correct synchronizing in setactive/killactive
	
	int		m_iUpperMain;					// upper limit for main progress control
	int		m_iUpperSub;					// upper limit for sub progress control
	int		m_iLowerMain;					// lower limit for main progress control
	int		m_iLowerSub;					// lower limit for sub progress control

    // connection settings
    BOOL            m_bInit;                // firmware initialized
    CStringArray    m_astrTargetTypes;      // target types (file extensions)
    CString         m_strTargetType;        // current target type
    CString         m_strDescription;       // current target description
    CString         m_strFirmwareVersion;   // firmware version
    CString         m_strCommChannel;       // communication channel
    CString         m_strProtocol;          // current protocol
    CString         m_strProtocolPar;       // current protocol parameter
    CString         m_strConnectGuid;       // connect guid

	BOOL			m_bSupportFileTransfer;	// supports file transfer ? // SUPFILTRANS 24.05.07 SIS
};


BOOL EvaluateBracketedTerm(const CString& crstrInput, CString& rstrPrefix, CString& rstrContents);

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINPROPERTYSHEET_H__368A73EE_E6E3_4C36_91A2_BE522BF10423__INCLUDED_)
