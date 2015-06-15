#if !defined(AFX_PROPPAGEFILETRANSFER_H__4407DC10_5168_42A4_85D8_860FE5159F11__INCLUDED_)
#define AFX_PROPPAGEFILETRANSFER_H__4407DC10_5168_42A4_85D8_860FE5159F11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropPageFileTransfer.h : header file
//
#include "PMPropPage.h"
#include "MainPropertySheet.h"

/////////////////////////////////////////////////////////////////////////////
// CPropPageFileTransfer dialog

class CPropPageFileTransfer : public CPMPropPage
{
	DECLARE_DYNCREATE(CPropPageFileTransfer)

// Construction
public:
	CPropPageFileTransfer(CMainPropertySheet* pMainSheet = NULL);
	~CPropPageFileTransfer();

    virtual void InitializeFromRegistry(BOOL bGeneral = FALSE);
    virtual void SaveToRegistry(BOOL bGeneral = FALSE);

    virtual void OnCommSettingsChanged();


// Dialog Data
	//{{AFX_DATA(CPropPageFileTransfer)
	enum { IDD = IDD_FILETRANSFER };
	CListBox	m_tPCFileListBox;
	CListBox	m_tControlFileListBox;
	CString	m_strAddress;
	CString	m_strPCPath;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropPageFileTransfer)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropPageFileTransfer)
	afx_msg void OnChangeAddressEdit();
	afx_msg void OnDownloadBtn();
	afx_msg void OnPcPathBtn();
	afx_msg void OnChangePcPathEdit();
	afx_msg void OnUploadBtn();
	virtual BOOL OnInitDialog();
	afx_msg void OnRefreshPcBtn();
	afx_msg void OnRefreshControlBtn();
	afx_msg void OnPcDeleteBtn();
	afx_msg void OnControlDeleteBtn();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
        
protected:
	void EnablePage(BOOL bEnable);	// SUPFILTRANS 24.05.07 SIS

    void FillPCFileList();
    void FillControlFileList();

    BOOL InitializeCommunication();
    void StopCommunication();
    BOOL CheckData();

    BOOL DownloadFiles();
    BOOL UploadFiles();

    BOOL DeletePCFiles();
    BOOL DeleteControlFiles();

private:
    BOOL DoFillControlFileList();
    
protected:
    HMODULE     m_hModule;          // OnlComm.dll library module
    C4CLayer*   m_pLayer;           // online communication layer
    BOOL        m_bAutoConnect;     // connected last time -> reconnect?
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPPAGEFILETRANSFER_H__4407DC10_5168_42A4_85D8_860FE5159F11__INCLUDED_)
