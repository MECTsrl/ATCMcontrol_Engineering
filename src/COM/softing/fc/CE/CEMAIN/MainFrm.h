// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////
#ifndef __MAINFRM_H_
#define __MAINFRM_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//----  Aggregate Includes:   --------------------------------------*
#include "navigator.h"
#include "CEStateMan.h"
#include "MsgWnd.h"
#include "WatchWnd.h"
#include "PouVarSelWindow.h"
#include "XRefWindow.h"
#include "BPViewWindow.h"
#include "CoolMenu.h"
#include "cedl.h"
#include "Settings.h"
#include "GenEdCntrItem.h"
#include "FindInFiles.h"
#include "CEFileHandler.h"
#include "KADHelper\KADFileInfo.h"
#include "MainDropTarget.h"
#include "CEDocuManager.h"

//----  Forward Class Definitions:   -------------------------------*
class CKADToolbar;
class CGenEdCont;

//----  Defines:    ------------------------------------------------*
// flags for visibility of docking tools
#define F_DOCKING_NAV       0x1
#define F_DOCKING_MESSAGE   0x2
#define F_DOCKING_POU       0x4
#define F_DOCKING_WATCH     0x8
#define F_DOCKING_BRKPT     0x10
#define F_DOCKING_XREF      0x20

enum ETargetSelectMode
{
    eAll,               // 0
    eConnected,         // 1
    eDisconnected,      // 2
    eConnectedEx,       // 3
    eDisconnectedEx,    // 4
    eDownloadChanges    // 5
};

enum ETargetCommand
{
    eTargetOne,      // 0
    eTargetAll,      // 1    
    eTargetSelect    // 2
};

// loop action enumerator
enum ELoopAction
{
    eLAOk,          // 0 proceed loop
    eLABreak,       // 1 break loop
    eLAContinue     // 2 continue loop
};

typedef CList<ACCEL, ACCEL&>            TAccelList;
typedef CList<CTargetOnl*, CTargetOnl*> TTargOnlList;

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

//------------------------------------------------------------------*


class CCEDockBar : public SECDockBar
{
public:
	DECLARE_DYNCREATE(CCEDockBar)

    virtual void ProcessDelayedInvalidates ();
};


/**
 *  class CMainFrame - represents the main frame window.
 *  Subclass of SECMDIFrameWnd. Holds the main menu and resources.
 *  Will be notified about state changes from the state manager via
 *  the method OnStateChange.
 *
 *  @see    SECMDIFrameWnd
*/
class CMainFrame : public SECMDIFrameWnd
{
//private attributes
    DECLARE_DYNAMIC(CMainFrame)

    BOOL            m_bShowNavigator;
    BOOL            m_bShowMsgView;
    BOOL            m_bShowPouVarSel;
    BOOL            m_bShowWatch;
    BOOL            m_bShowBPView;
    BOOL            m_bShowXRef;
    FC_CALLBACK(CEStateChangeCB, CMainFrame) m_StateChangeCB;
public:
	CMainFrame();

    void ShowPouVarSelection();
    CPouVarSelWindow *GetPouVarSel (bool bMustLive=true);
    void ShowXRef();
    CXRefWindow* GetXRef(bool bMustLive=true);
    CMsgWnd *CMainFrame::GetMsgView (bool bMustLive=true);

    void Build(enum ETargetCommand cmd, const CString& strTarget = _T(""));
    void BuildLibrary();
    void CompileFile(CString strFileName);
    void DownloadTarget(enum ETargetCommand cmd, CEDL_DOWNLD_TYPE dlType, 
                        const CString& strTarget = _T(""), const CString& strConnectInfo = _T(""),
                        CEDL_DOWNLD_OPTIONS eOptions = DOWNLD_NO_OPTIONS);
    void DoSettingsResetDocking();
    void DoSettingsLibrary();
    void DoSettingsEngineering();
    void DoSettingsPassword();
    void DoSettingsPrint();

    void NotifyCompileReady();

    void OpenURL(const CString& strUrl);

    void CreateKADToolbars();
    void AddExploreToolbar();
    void AddHelpToolbar();
  
    void SetEditPositionText(LONG lLine, LONG lColumn, BOOL bShow = TRUE);
	void UpdateStatusBar(LPCTSTR pszText);

    // for exe commands:
    void SubstituteFileMacros(CString& rstrCommand);
    void ReplacePrjMacrosEmpty(CString& rstrCommand);

    int GetCommandStrings(const CString& crstrCommandSeq, CStringArray& rastrCommands);
    CString GetEditorFileNamePath();
    CString GetPathFromNamePath(const CString& crstrFileNamePath);
    CString GetFileNameFromNamePath(const CString& crstrFileNamePath);
    CString GetFileExtension(const CString& crstrFileName);
    CString GetFileBaseName(const CString& crstrFileName);

    void DoKernelStandardOperation(int iKSO);
    void DoExeOperation(CKADMenuItem* pItem, const CString& crstrExeCommand = _T(""));
    void DoAddOnAction(CKADMenuItem* pItem, BOOL bSilent = FALSE);

    void SetVarSelectionHint(const CString& strFileName);

    UINT FindInFilesWorker();
    void TerminateFindInFilesThread();
    BOOL UpdateAllTabFileViews();

    HACCEL GetAccelerators();

    void OnUpdateFrameTitle (BOOL bAddToTitle);

    void PrintProject(const CString& crstrProjectName, const CString& crstrProjectFile);

    BOOL OnIdle(LONG lCount);

    BOOL CreatePanelView(const CString& strName);

    void OpenNewDocument();
    void OpenDocument();

	DROPEFFECT OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);

    BOOL MaximizeMDIWindows(BOOL bMaximize = TRUE);
    BOOL GetMDIMaximizeState();

    CKADFileInfo* GetKADFileInfo();

    BOOL GetPrintInfo(CString &strPrinterName, long* plWidth, long* plHeight);

    BOOL GetTargetCompileDownloadState(const CString& crstrTargetId, BOOL& rbCompiled, BOOL& rbDownloaded);

    BOOL IsProjectReadOnly();

// Attributes
public:

    // guid match codes
    enum GUIDMatchCode
    {
        GUIDMatchGood,
        GUIDMatchUnknown,
        GUIDMatchNotBuilt,
        GUIDMatchNoDownload,
        GUIDMatchNotDownloaded,
        GUIDMatchDifferentDownload
    };


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


private: //private methods
    void OnStateChange (CEStateNotify stateNotify, unsigned int uiStage);
    void InitNavigator();
    CNavigator *CMainFrame::GetNavigator (bool bMustLive=true);
    void InitMsgView();
    void InitWatch();
    CWatchWindow *GetWatch (bool bMustLive=true);
    void InitPouVarSel();
    void InitXRef();
    void InitBPView();
    void InitFindInFiles();
    CBPViewWindow *GetBPView (bool bMustLive=true);

    BOOL SelectTargets(CStringArray& rastrTargetsSel, ETargetSelectMode eMode, CTargetSet* pTargetSet = NULL);

    BOOL StartResource(CTargetOnlBase* pTarget);
    BOOL StopResource(CTargetOnlBase* pTarget);
    BOOL StartAllTasks(CTargetOnlBase* pTarget);
    BOOL StopAllTasks(CTargetOnlBase* pTarget);
    BOOL ColdStartResource(CTargetOnlBase* pTarget);
    BOOL WarmStartResource(CTargetOnlBase* pTarget);
    BOOL ClearResource(CTargetOnlBase* pTarget);

    BOOL IsKernelStandardOperationPossible(int iKSO);

    void FindInFiles();
    void UpdateFindInFiles(CCmdUI* pCmdUI);
	CCEFileHandler* GetFileHandler();
    BOOL ActivateFileView(const CString & StrName, LPCTSTR pszPosition = NULL);
    CGenEdDoc *ActivateContainerFileView (const CString &strName, LPCTSTR pszPosition=NULL);

    void AppendKADAccelerators(TAccelList& rtAccelList);
    void CreateKADAcceleratorTable(TAccelList& rtAccelList);

    void ToggleViewFullScreen();

    UINT GetDockingToolsVisibilityFlags();

    void ShowDockingTools(UINT uiVisFlags);
	
    void ShowDownloadTab();

    HRESULT EstablishDebugConnection(CTargetOnl* pTarget, CMsgWnd *pMessageWindow);

    BOOL LoadFileInfo();

    ELoopAction CheckDownloadConnection(CTargetOnl* pTarget, CMsgWnd* pMsgWnd, BOOL bLast = TRUE);

    BOOL CheckTaskState(CTargetOnl* pTarget);

    HRESULT DoPreDownloadStep(CTargetOnlBase* pTarget);
    HRESULT DoPostDownloadStep(CTargetOnlBase* pTarget);

    BOOL DisconnectAllTargets();

    void CheckProjectReadOnly();

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnUpdateToggleNavigator(CCmdUI* pCmdUI);
	afx_msg void OnToggleNavigator();
	afx_msg void OnToggleMsgView();
	afx_msg void OnUpdateToggleMsgView(CCmdUI* pCmdUI);
	afx_msg void OnTogglePouSelection();
	afx_msg void OnUpdateTogglePouSelection(CCmdUI* pCmdUI);
    afx_msg void OnToggleXRef();
    afx_msg void OnUpdateToggleXRef(CCmdUI* pCmdUI);
	afx_msg void OnToggleWatchView();
	afx_msg void OnUpdateToggleWatchView(CCmdUI* pCmdUI);
    afx_msg void OnToggleBPView();
	afx_msg void OnUpdateToggleBPView(CCmdUI* pCmdUI);
	afx_msg void OnClose();
    afx_msg void OnUpdateMainMenuCommand(CCmdUI* pCmdUI);
    afx_msg void OnGetDispInfo(UINT id, NMHDR* pTTTStruct, LRESULT* pResult );
    afx_msg void OnUpdateEditPos(CCmdUI* pCmdUI);
    afx_msg void OnPrintProject();
    afx_msg void OnPrintFile();
	afx_msg void OnUpdatePrintFile(CCmdUI* pCmdUI);
    afx_msg void OnPrintXRef();
	afx_msg void OnUpdatePrintXRef(CCmdUI* pCmdUI);
    afx_msg void OnFindInFiles(void);
    afx_msg void OnUpdateFindInFiles(CCmdUI* pCmdUI);
	afx_msg void OnViewFullscreen();
	afx_msg void OnUpdateViewFullscreen(CCmdUI* pCmdUI);
    afx_msg void OnBreak();
    afx_msg void OnCloseAllWindows();
	afx_msg void OnUpdateCloseAllWindows(CCmdUI* pCmdUI);
    afx_msg void OnSoftHomepage();
    afx_msg void OnContinueTask();
	afx_msg void OnNextError();
	afx_msg void OnPreviousError();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
    void OnUpdateFrameMenu(HMENU hMenuAlt);

    void OnUpdateMainMenu(LPARAM lParam, WPARAM wParam);

    // menu helper functions
    int FindSubmenuByName(CMenu* pMainMenu, const CString& crstrName, CMenu*& pSubMenu);
    int FindMenuItemByName(CMenu* pSubMenu, const CString& crstrName);
    int AddMainSubMenu(CMenu* pSubMenu, CKADMenu* pKADSubMenu, CKADMainMenu* pKADMainMenu, TAccelList& rtAccelList, int iStartPos = 0);
    // insert menu item
    BOOL InsertMenuItem(CMenu* pMenu, CKADMenuItem* pItem, CKADMainMenu* pKADMainMenu, TAccelList& rtAccelList, int iInsertPos);

    void OnMainMenuCommand(UINT nID);

    void SaveAllFiles(bool bAskBefore = false);

    void CreateKADToolbar(CKADToolbar* pKADToolbar);

    BOOL LoadKADToolBar(CToolBar& rtToolbar, CToolbarData* pToolbarData);

    HBITMAP BuildToolbarBitmap(CToolbarData* pToolbarData);

    BOOL GetToolTipTextForMainMenuItem(UINT uiID, CString& rstrToolTip) const;
    BOOL GetStatusTextForMainMenuItem(UINT uiID, CString& rstrStatus) const;

	virtual void GetMessageString(UINT nID, CString& rMessage) const;

    HRESULT PrintProjectFile(const CString& crstrFileName, CDC * pdcPrint,
                             CPrintInfo * pPrintInfo, 
                             CDialog *pDlgPrintStatus);
    HRESULT GetPageCount(CGenEdCntrItem * pItem, const CString& crstrFileName, long * plPages);
    HRESULT PrintDocument(CGenEdCntrItem * pItem, const CString& crstrFileName,
                                  CDC * pDCPrint, CPrintInfo * pPrintInfo, 
                                  CDialog * pDlgPrintStatus);

    BOOL GetProjectInfo(ICEProjInfo** ppICEProjInfo);
    BOOL GetFindInFiles(ICEFindInFiles** ppIFindInFiles);

	CGenEdCont* GetContainer();

    void OnTargetLoginNotify ();
    void HandleForcedLogouts ();
public:    
    void SaveSettings();
    void LoadSettings();
    void StoreDockingTools();
    void RestoreDockingTools();
	
	CCEDocuManager*	GetDocuManager();	// XREFPRINT 20.07.05 SIS

protected:
    void StoreDockingTool(SECControlBar* pDockingTool, DockToolSettings* pSettings, BOOL bVisible);
    void RestoreDockingTool(SECControlBar* pDockingTool, DockToolSettings* pSettings, BOOL* pbVisible);
    BOOL m_isRestoringDockingTools;

    void CheckDownloadGUIDs();
    GUIDMatchCode CheckDownloadGUID (CTargetOnl *pTargetOnl);

protected:  // control bar embedded members
	SECStatusBar    m_wndStatusBar;
	CToolBar        m_wndToolBar;           // for stingray use SECToolBar
    CToolBar        m_wndHelpToolBar;
//    SECToolBar    m_wndBuildToolBar;
//    SECToolBar    m_wndEditToolBar;
    CToolBar        m_wndViewToolBar;
    CToolBar        m_wndIEToolBar;
//    SECToolBar    m_wndAddToolBar;
//    SECToolBar    m_wndHelpToolBar;
    CToolBar        m_wndKADToolBar[20];
    CReBar		    m_wndReBar;
    CCoolMenuManager m_MenuMgr;
    int             m_iToolbarCount;        // starts from 0
    BOOL            m_bProjectLoaded;       // for update menu
    HACCEL          m_hKADAccel;            // KAD accelerators
    BOOL            m_bKADAccelLoaded;      // KAD accelarators have been loaded?
    BOOL            m_bViewFullScreen;      // full screen mode?
    UINT            m_uiVisFlags;           // docking tools visibility flags
    BOOL            m_bMaximized;           // maximized in non full screen mode?
    BOOL            m_bProjectReadOnly;     // is project file read only?
    CString         m_strProjFilePathName;  // project file name incl path.
    CKADFileInfo*   m_pFileInfo;            // file extension info from KAD
    CGenEdCont*     m_pContainer;           // pointer to editor container

    // download check
    bool            m_bCheckDownloadGUIDs;  // download guids to be checked?
    TTargOnlList    m_DownloadGUIDCheckPCs; // list for download check
    bool            m_bDoNotAddToCheckList; // do not add to check list?

    CMainDropTarget m_tDropTarget;          // for bringing main window to front

    CCEDocuManager  m_tDocuManager;         // document manager

    struct ForcedLogout
    {
        CString m_strMachine;
        CString m_strPreviousUser;
        CString m_strPreviousHost;
        CString m_strNewUser;
        CString m_strNewHost;
    };

    CArray<ForcedLogout, const ForcedLogout &> m_ForcedLogouts;
    BOOL            m_bHandleForcedLogouts;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __MAINFRM_H_