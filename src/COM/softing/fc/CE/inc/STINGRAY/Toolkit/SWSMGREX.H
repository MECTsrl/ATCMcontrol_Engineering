/////////////////////////////////////////////////////////////////////////////
// swsmgrex.h : Declaration of SECWorkspaceManagerEx
//
// Stingray Software Extension Classes
// Copyright (C) 1998 Stingray Software Inc,
// All rights reserved.
//
// This source code is only intended as a supplement to
// the Objective Toolkit Class Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding
// the Objective Toolkit product.
//
// Author:       Mark Isham
// Description:  Declaration of SECWorkspaceManagerEx
//

#ifndef __SECWSMGREX_H__
#define __SECWSMGREX_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#ifdef WIN32
									  
// Header Includes
#ifndef __PLUGIN_H__
#include "Foundation\patterns\MFC\plugin.h"
#endif

#ifndef __SECPERSTREE_H__
#include "toolkit\perstree.h"
#endif

#ifndef __WKSEXDLG_H__
#include "toolkit\WkSExDlg.h"
#endif


//
// SEC Extension DLL
// Initialize declaration context
//
#ifdef _SECDLL
	#undef AFXAPP_DATA
	#define AFXAPP_DATA AFXAPI_DATA
	#undef AFX_DATA
	#define AFX_DATA    SEC_DATAEXT
#endif //_SECDLL

// Constants

// Macros

// Use these macros in the frame window classes to insure default message
// handlers are properly provided by the base.
#define DECLARE_WSMGREX_DEFAULT_HANDLERS()					\
	afx_msg BOOL OnWorkspace(UINT nID);						\
	afx_msg void OnUpdateWorkspaceMenu(CCmdUI* pCmdUI);		\
	afx_msg void OnWorkspaceNew();							\
	afx_msg void OnUpdateWorkspaceNew(CCmdUI* pCmdUI);		\
	afx_msg void OnWorkspaceOpen();							\
	afx_msg void OnUpdateWorkspaceOpen(CCmdUI* pCmdUI);		\
	afx_msg void OnWorkspaceSaveAs();						\
	afx_msg void OnUpdateWorkspaceSaveAs(CCmdUI* pCmdUI);	\
	afx_msg void OnWorkspaceDelete();						\
	afx_msg void OnUpdateWorkspaceDelete(CCmdUI* pCmdUI);	\
	afx_msg void OnWorkspaceClose();						\
	afx_msg void OnUpdateWorkspaceClose(CCmdUI* pCmdUI);	\
	afx_msg void OnWorkspaceSave();							\
	afx_msg void OnUpdateWorkspaceSave(CCmdUI* pCmdUI);

#define IMPLEMENT_WSMGREX_DEFAULT_HANDLERS(base)						\
	BOOL base::OnWorkspace(UINT nID) { nID; return TRUE; }				\
	void base::OnUpdateWorkspaceMenu(CCmdUI* pCmdUI) { pCmdUI; }		\
	void base::OnWorkspaceNew() {}										\
	void base::OnUpdateWorkspaceNew(CCmdUI* pCmdUI) { pCmdUI; }			\
	void base::OnWorkspaceOpen() {}										\
	void base::OnUpdateWorkspaceOpen(CCmdUI* pCmdUI) { pCmdUI; }		\
	void base::OnWorkspaceSaveAs() {}									\
	void base::OnUpdateWorkspaceSaveAs(CCmdUI* pCmdUI) { pCmdUI; }		\
	void base::OnWorkspaceDelete() {}									\
	void base::OnUpdateWorkspaceDelete(CCmdUI* pCmdUI) { pCmdUI; }		\
	void base::OnWorkspaceClose() {}									\
	void base::OnUpdateWorkspaceClose(CCmdUI* pCmdUI) { pCmdUI; }		\
	void base::OnWorkspaceSave() {}										\
	void base::OnUpdateWorkspaceSave(CCmdUI* pCmdUI) { pCmdUI; }
																		
#define MAP_WSMGREX_DEFAULT_HANDLERS()									\
	ON_COMMAND_EX_RANGE(ID_WORKSPACE_1, ID_WORKSPACE_SPACES, OnWorkspace) \
	ON_UPDATE_COMMAND_UI(ID_WORKSPACE_1, OnUpdateWorkspaceMenu)			\
	ON_COMMAND(ID_WORKSPACE_NEW, OnWorkspaceNew)						\
	ON_UPDATE_COMMAND_UI(ID_WORKSPACE_NEW, OnUpdateWorkspaceNew)		\
	ON_COMMAND(ID_WORKSPACE_OPEN, OnWorkspaceOpen)						\
	ON_UPDATE_COMMAND_UI(ID_WORKSPACE_OPEN, OnUpdateWorkspaceOpen)		\
	ON_COMMAND(ID_WORKSPACE_SAVEAS, OnWorkspaceSaveAs)					\
	ON_UPDATE_COMMAND_UI(ID_WORKSPACE_SAVEAS, OnUpdateWorkspaceSaveAs)	\
	ON_COMMAND(ID_WORKSPACE_DELETE, OnWorkspaceDelete)					\
	ON_UPDATE_COMMAND_UI(ID_WORKSPACE_DELETE, OnUpdateWorkspaceDelete)	\
	ON_COMMAND(ID_WORKSPACE_CLOSE, OnWorkspaceClose)					\
	ON_UPDATE_COMMAND_UI(ID_WORKSPACE_CLOSE, OnUpdateWorkspaceClose)	\
	ON_COMMAND(ID_WORKSPACE_SAVE, OnWorkspaceSave)						\
	ON_UPDATE_COMMAND_UI(ID_WORKSPACE_SAVE, OnUpdateWorkspaceSave)

// Types

// Forward Declarations

// AutoDuck inline comments

//@doc SECWorkspaceManagerEx
//@mfunc int  | SECWorkspaceManagerEx | GetMaxWorkspaceMRUSize | Gets the size of the Most Recently Used workspace list
//@xref <c SECWorkspaceManagerEx>

//@doc SECWorkspaceManagerEx
//@mfunc void  | SECWorkspaceManagerEx | SetMaxWorkspaceMRUSize | Set the size of the Most Recently Used workspace list
//@parm UINT | nMaxSize | Max number of entries in the MRU list.
//@xref <c SECWorkspaceManagerEx>

//@doc SECWorkspaceManagerEx
//@mfunc void  | SECWorkspaceManagerEx | GetWorkspaceFileExtension | Get the default file extension used for file based workspaces.
//@parm CString& | strExt | Reference to string to receive the file extension
//@xref <c SECWorkspaceManagerEx>

//@doc SECWorkspaceManagerEx
//@mfunc void  | SECWorkspaceManagerEx | SetWorkspaceFileExtension | Set the default file extension used for file based workspaces.
//@parm CString& | strExt | Reference to string with the file extension
//@xref <c SECWorkspaceManagerEx>


/////////////////////////////////////////////////////////////////////////////
// SECWorkspaceManagerEx

//@doc SECWorkspaceManagerEx 
//@class Extended workspace manager class for persisting frame and controlbar 
// window state. NOTE: New development should exploit this class, rather
// than SECWorkspaceManager, as SECWorkspaceManager is considered obsolete
// and available only for backward compatibility.
//@BASE public | SecWndPlugIn
//@xref <c SECWorkspaceManager>
//@xref <c SECWndPlugIn>
class SECWorkspaceManagerEx : public stingray::foundation::SECWndPlugIn
{
	DECLARE_DYNCREATE(SECWorkspaceManagerEx);

	//---------------------------------------------------------------------//
	// 						   Public Interface							   //
	//---------------------------------------------------------------------//
public:

	// Initialization/Destruction

	//@cmember
	/* Construction/Initialization*/
	SECWorkspaceManagerEx();
	virtual ~SECWorkspaceManagerEx();

	// Attributes
	// ...Publicly accessible member variables...

	// Accessors
	// ...Get and Set functions...
	
	//@cmember
	/* Set the workspace manager to registry mode*/
	virtual void SetRegistryMode(const CString& strAppSubKey,BOOL bEnable=TRUE,
								 HKEY hKeyApp=HKEY_CURRENT_USER, BOOL bSectionKey=FALSE);
								 
	//@cmember
	/* Set the workspace manager to registry mode*/
	virtual void SetRegistryMode(BOOL bEnable, BOOL bSectionKey = FALSE); 

	// Operations

	//@access Operations

	//@cmember
	/* Close the currently active workspace.*/
	virtual BOOL CloseWorkspace();

	//@cmember
	/* Save the currently active workspace state.*/
	virtual BOOL SaveWorkspace();

	//@cmember
	/* Save the currently active workspace state under a new name.*/
	virtual BOOL SaveWorkspace(CString strWorkspaceName);

	//@cmember
	/* Load a workspace state.*/
	virtual BOOL OpenWorkspace(BOOL bLastActiveWorkspace=FALSE,BOOL bSuppressErrorDlg=FALSE);

	//@cmember
	/* Load the workspace state of the workspace name passed in.*/
	virtual BOOL OpenWorkspace(CString strWorkspaceName,BOOL bSuppressErrorDlg=FALSE);

	//@cmember
	/* Get the name of the currently active workspace. */
	BOOL GetActiveWorkspace(CString& strWorkspaceName); 
	
	// Workspace MRU List manipulation
	
	//@cmember
	/* Set the maximum number of workspaces in the MRU list*/
	void SetMaxWorkspaceMRUSize(UINT nMaxSize)	{ m_nMaxWorkspaceMRUSize=nMaxSize; }
	
	//@cmember
	/* Get the maximum number of workspaces in the MRU list*/
	int GetMaxWorkspaceMRUSize() 				{ return m_nMaxWorkspaceMRUSize; }

	//@cmember
	/* Set the workspace default extension*/
	void SetWorkspaceFileExtension(const CString& strExt)	{ m_strWorkspaceFileExt=strExt; }

	//@cmember
	/* Get the workspace default extension*/
	void GetWorkspaceFileExtension(CString& strExt)			{ strExt=m_strWorkspaceFileExt; }
	
	// Overrides
	virtual BOOL PlugInTo( CWnd* pWnd );

	//---------------------------------------------------------------------//
	// 							 Overridables							   //
	//---------------------------------------------------------------------//
protected:
	//@access Overridable

	//@cmember
	/* Get the persistance strategy (file, registry, etc.)*/
	virtual	SECPTNStrategy* GetPersistanceStrategy(const CString& strWorkspace);
	
	//@cmember
	/* Load the controlbar dockstate for current OpenWorkspace operation.*/
	virtual BOOL OpenWorkspaceDockState(SECPersistentTreeNode* pRoot);

	//@cmember
	/* Load the frame window state for current OpenWorkspace operation.*/
	virtual BOOL OpenWorkspaceFrameWnds(SECPersistentTreeNode* pRoot,
										CFrameWnd** ppActiveChildFrame);

	//@cmember
	/* Save the controlbar dockstate for current SaveWorkspace operation.*/
	virtual BOOL SaveWorkspaceDockState(SECPTNFactory& nodeFactory,
										SECPersistentTreeNode* pRoot);
	//@cmember
	/* Save the frame window state for current SaveWorkspace operation.										*/
	virtual BOOL SaveWorkspaceFrameWnds(SECPTNFactory& nodeFactory,
										SECPersistentTreeNode* pRoot);						
										
	//@cmember
	/* Display prompt dialog for workspace Open or SaveAs*/
	virtual BOOL PromptWorkspaceName(CString& strWorkspaceName,BOOL bSaveAs);	

	//@cmember
	/* Overridable to load your own custom workspace data.*/
	virtual void OpenWorkspaceCustomData(SECPersistentTreeNode* pRoot,
										const CString& strWorkspaceName);

	//@cmember
	/* Overridable to load your own custom workspace data.*/
	virtual void SaveWorkspaceCustomData(SECPTNFactory& nodeFactory,
										SECPersistentTreeNode* pRoot,
										const CString& strWorkspaceName);
	
	//@cmember
	/* Close all the child frame windows.*/
	virtual BOOL CloseAllChildFrames();

	//@cmember
	/* Close all the controlbars.*/
	virtual BOOL CloseControlBars(CPtrList* pListBars,CFrameWnd* pFrame);

	//@cmember
	/* Override to load multiple views per single frame window.*/
	virtual BOOL OpenAdditionalViewPerFrame(const CDocument* pDoc,
									const CFrameWnd* pFrame,
									const SECPersistentTreeNode* pAddViewNode);

	//@cmember
	/* Override to save multiple views per single frame window.*/
	virtual SECPersistentTreeNode* SaveAdditionalViewPerFrame(
									CDocument* pDoc,
									CView* pView,
									CFrameWnd* pFrame,
									SECPTNFactory& nodeFactory,
									SECPersistentTreeNode* pParent,
									int nMultViewFrameUniqueID);

	//---------------------------------------------------------------------//
	// 						Protected Implementation					   //
	//---------------------------------------------------------------------//
protected:

	// Implementation Member Variables

	// Can only use SECWorkspaceManagerEx with SECFrameWnd or SECMDIFrameWnd,
	// but CFrameWnd is the only common base. Constructors will enforce
	// proper type
	CFrameWnd* m_pFrameWnd;					// frame window mgr is attached to
	CString m_strCurrentWorkspace;			// name of currently open workspace
	BOOL m_bOpenWorkspace;					// true if there is a currently open workspace
	CString m_strWorkspaceFileExt;			// default workspace file extension
	
	// For registry persistence toggle
	CString m_strRegAppSubKey;				// Registry subkey for this app
	HKEY m_hRegAppKey;						// Registry root key for this app
	BOOL m_bRegEnabled;						// Registry support enabled toggle.
    BOOL m_bSectionKey;                     // CString is a section key rather
                                            // than an HKCU subkey

	UINT m_nShowCmd;						// The show command of child frame

	// Workspace MRU list	
	UINT m_nMaxWorkspaceMRUSize;			// max number of workspaces in the MRU list

	// Implementation Member Functions

	// Workspace open/save helper functions
	virtual BOOL OpenWorkspaceMainFrame(SECPersistentTreeNode* pParent);	
	virtual BOOL SaveWorkspaceMainFrame(SECPTNFactory& nodeFactory,
										SECPersistentTreeNode* pParent);
	virtual BOOL OpenWorkspaceChildFrames(SECPersistentTreeNode* pParent,
										CFrameWnd** ppActiveChildFrame);
	virtual BOOL SaveWorkspaceChildFrames(SECPTNFactory& nodeFactory,
										SECPersistentTreeNode* pParent,
										CFrameWnd* pActiveChildFrame);
	virtual BOOL OpenSpecificChildFrame(SECPersistentTreeNode* pNode,
										CDocument** ppDoc,
										CFrameWnd** ppFrame,
										CFrameWnd** ppActiveChildFrame);
	virtual SECPersistentTreeNode* SaveSpecificChildFrame(CDocument* pDoc,
										CView* pView,
										CFrameWnd* pFrame,
										CFrameWnd* pActiveFrame,
										SECPTNFactory& nodeFactory,
										SECPersistentTreeNode* pParent,
										int nChildNum);	
	virtual BOOL SaveSpecificDockedView(CFrameWnd* pFrame,
										SECPersistentTreeNode* pFrameNode);
	virtual BOOL SaveChildFrameDockedViewPositionFloater(CWnd* pParentWnd,
										SECPersistentTreeNode* pFrameNode);

	// Frame window helper functions
	virtual BOOL OnSaveDoc(CFrameWnd* pFrameWnd,CDocument* pDoc);
	virtual CFrameWnd* OpenChildFrame(CDocTemplate* pDocTemplate,
										const CString& strDocPath,
										CDocument** ppDoc);
	virtual CDocument* OpenChildFrameDockedView(SECPersistentTreeNode* pFrameNode,
										CDocTemplate* pDocTemplate,
										const CString& strDocPath);
	virtual BOOL OpenChildFrameDockedViewPositionFloater(
											SECPersistentTreeNode* pFrameNode,
											CDocument* pDocument);
	virtual BOOL OpenChildFrameDockedViewPositionFloater(
											CFrameWnd* pFrame,
											SECPersistentTreeNode* pFrameNode,
											CDocument* pDocument);
	virtual CFrameWnd* OpenExistingDocNewView(CDocTemplate* pDocTemplate,
										CDocument* pDoc,
										CFrameWnd* pFrame);
	virtual CFrameWnd* OpenExistingDocNewDockedView(
										SECPersistentTreeNode* pFrameNode,
										CDocTemplate* pDocTemplate,
										CDocument* pDoc,
										CFrameWnd* pOther);
	virtual void SetWorkspaceActiveChildFrame(CFrameWnd* pActiveChildFrame);
	
	// Workspace MRU list
public:
	virtual BOOL GetWorkspaceMRUList(CStringList& listWorkspaces);
	virtual LPCTSTR GetWorkspaceMRURegSuffix();								 
	
protected:
	virtual BOOL SetWorkspaceMRUList(const CStringList& listWorkspaces);
	virtual void SetMRUWorkspace(const CString& strWorkspaceName,
								 CStringList& listWorkspaces);
	virtual BOOL RemoveFromMRUList(const CString& strWorkspaceName);

	// User input dialogs. Override for custom dialogs, or to prevent display.
	virtual BOOL PromptWorkspaceNameReg(CString& strWorkspaceName,
										BOOL bAllowOpen,BOOL bAllowSave);
	virtual void UpdateWorkspaceRegSettings(CStringList& listOrigWorkspaces,
												CStringList& listNewWorkspaces,
												CStringList& listDeletedWorkspaces);										
	virtual BOOL PromptWorkspaceNameFile(CString& strWorkspaceName,BOOL bSaveAs);

	//---------------------------------------------------------------------//
	// 						  Base Class Overrides						   //
	//---------------------------------------------------------------------//

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SECWorkspaceManagerEx)
	//}}AFX_VIRTUAL

	//---------------------------------------------------------------------//
	// 						Message Map Declarations					   //
	//---------------------------------------------------------------------//

	// Generated message map functions
	//{{AFX_MSG(SECWorkspaceManagerEx)
	//}}AFX_MSG
	DECLARE_WSMGREX_DEFAULT_HANDLERS()		// Handlers as defined above
	DECLARE_MESSAGE_MAP()

	//---------------------------------------------------------------------//
	// 						Private Implementation						   //
	//---------------------------------------------------------------------//
private:

};

/////////////////////////////////////////////////////////////////////////////

//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif // WIN32

#endif //__SECWSMGREX_H__

