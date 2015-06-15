#if !defined(AFX_PMPROPPAGE_H__C12E2AE9_5A82_4A63_AF62_81381E80811F__INCLUDED_)
#define AFX_PMPROPPAGE_H__C12E2AE9_5A82_4A63_AF62_81381E80811F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PMPropPage.h : header file
//

#include <afxtempl.h>
#define _ONLCOMM __declspec(dllimport)
#include "4CLayer.h"

class CMainPropertySheet;

#define     STANDARD_TARGET_TYPE        _T("4CWin")
#define     STANDARD_CONNECT_FORMAT     _T("%s,TCP(%s),SIMPLE(1000),VERSION(21000)")

#define     COL_TARGET_NAME     (0)
#define     COL_ADDRESS_OLD     (1)
#define     COL_ADDRESS_NEW     (2)

#define     FILE_MODE           (0)
#define     TARGET_MODE         (1)

typedef struct _TargetInfo
{
    CString strControl;
    CString strTargetName;
    CString strTargetAddress;
    CString strTargetFile;
    CString strTargetOPCServer;
	CString strCustomConnect;		// #245 14.09.05 SIS
	CString strTargetOPCInstName;
    CString strTargetResource;
    CString strTargetType;
    CString strTargetNewAddress;
} TTargetInfo;

// target information list
typedef CList<TTargetInfo*, TTargetInfo*>   TTIList;

class C4CLayer;

/////////////////////////////////////////////////////////////////////////////
// CPMPropPage dialog

class CPMPropPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CPMPropPage)

// Construction
public:
	CPMPropPage(UINT uiId = 0, CMainPropertySheet* pMainSheet = NULL);
	~CPMPropPage();
    
    virtual BOOL TryLoadTargetInfo(const CString& crstrProjectPath);
    virtual BOOL LoadTargetInfoList(const CString& crstrProjectPath, BOOL bForceReload = FALSE);

    virtual void InitializeFromRegistry(BOOL bGeneral = FALSE);
    virtual void SaveToRegistry(BOOL bGeneral = FALSE);
    
    virtual BOOL OnKillActive();

    virtual CString GetProjectPath();
    
    virtual void OnCommSettingsChanged();

    virtual void SetConnectStatus(BOOL bConnected);
    
    CString GetStandardProjectPath();

    BOOL Launch4C();

    int FindProjectFile(const CString& crstrProjectPath, CString& rstrProjFile);

    BOOL DownloadProject(const CString& crstrFilePath, TTargetInfo* pTargetInfo);
    BOOL UploadProject(const CString& crstrFilePath, const CString& crstrTargetControl, const CString& crstrTargetType);

    BOOL DownloadFile(const CString& crstrFilePath, C4CLayer* pLayer);
    BOOL UploadFile(const CString& crstrFilePath, C4CLayer* pLayer);
    BOOL DeleteControlFile(const CString& crstrFilePath, C4CLayer* pLayer);

    BOOL UploadDirectoryContent(CStringArray& rastrFiles, C4CLayer* pLayer);

    BOOL ConnectToTarget(C4CLayer* pLayer);
    void DisconnectFromTarget(C4CLayer* pLayer);

    int CopyOPCServerConfig(TTargetInfo* pTInfo, const CString& crstrProjectPath);
    int PatchOPCServerRegistry(TTargetInfo* pTInfo, const CString& crstrProjectPath);
    
    int ExecuteCommand(const CString& crstrCmdLine, BOOL bVisible = FALSE, BOOL bWait = TRUE, BOOL bSilent = TRUE);

    BOOL GetTargetInstallPath(const CString& crstrTargetType, CString& rstrTargetInstDir);

    CString GetTargetControlString(const CString& crstrControl);


protected:
    void ResetTargetInfoList();

    int LoadProjectTargetInfo(const CString& crstrProjectPath);
    
    int GetProjectTargets(const CString& crstrProjectPath);
    int FillProjectTargetInfo(TTargetInfo* pTInfo, const CString& crstrProjectPath);

    HMODULE LoadOnlCommLibraryForTarget(const CString& crstrTargetType);
    HMODULE LoadLocalOnlCommLibrary();

    CString FindOPCServer(TTargetInfo* pTInfo);
    CString GetDLGuid(TTargetInfo* pTInfo, const CString& crstrProjectPath);
    CString GetCurrentUser();
    CString GetLocalMachineName();
    
    BOOL LoadFile(LPCTSTR szFile, BYTE **ppData, UINT *upLen);
    BOOL StoreFile(LPCTSTR szFile, BYTE *pData, UINT uLen);

    
// Implementation
protected:
    CMainPropertySheet* m_pMainSheet;
    CString             m_strLoadedProject;
    TTIList             m_tTargetInfoList;
    BOOL                m_bSupportProgressCallback;
};


bool ProgressCB(int iCurrent, int iMax);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PMPROPPAGE_H__C12E2AE9_5A82_4A63_AF62_81381E80811F__INCLUDED_)
