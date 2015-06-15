#ifndef __FILETREECTRL_H_
#define __FILETREECTRL_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "NavDef.h"
#include "ctrlext.h"
#include "regex.h"

#include "SBLLexTree.h"
//#include "XMLHelper/XMLNode.h"
//#include "XMLHelper/XMLNodeList.h"
//#include "XMLHelper/XMLDocument.h"
#include "NavTreeControl.h"
#include "KADHelper/KADFileInfo.h"

#include <comdef.h>
#include <atlbase.h>
#include <afxmt.h>



//----  Aggregate Includes:   --------------------------------------*

//----  Forward Class Definitions:   -------------------------------*
class CFileTreeCtrl;

//----  Defines:    ------------------------------------------------*

// enum for node types
typedef enum EFVNodeType
{
    E_NAV_FV_UNKNOWN = -1,      //-1: unknown
    E_NAV_FV_FILE,              // 0: file
    E_NAV_FV_FOLDER,            // 1: folder
    E_NAV_FV_SOURCE_GROUP,      // 2: source group folder
    E_NAV_FV_HELP_GROUP,        // 3: help group folder
    E_NAV_FV_ADDITIONAL_GROUP,  // 4: additional group folder
    E_NAV_FV_ROOT,              // 5: root node (project)
};

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

class CFileThreadInfo
{
public:
    CString         m_strFolder;        // folder to be checked
    HANDLE          m_hEvent;           // event handle to stop thread
    CFileTreeCtrl*  m_pFileTreeCtrl;    // pointer to file view
};

// file tree control class

//------------------------------------------------------------------*
/**
 *  class CFileTreeCtrl
 *
 *    - derived from CNavTreeCtrl (CTreeCtrl)
 *    - visualization of files in project directory
 *    - checks project directory for new or removed files (Check())
 *    - uses XML-document for persistent storage
 *
 *  @see    CNavTreeCtrl
 */
class CFileTreeCtrl : public CNavTreeCtrl
{
	//{{AFX_MSG(CFileTreeCtrl)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
    afx_msg void OnFileChanged(LPARAM lParam, WPARAM wParam);
    afx_msg void OnDecrementCheckDisableCount(LPARAM lParam, WPARAM wParam);
	DECLARE_MESSAGE_MAP()

public:

    // construction / destruction
    CFileTreeCtrl();
    virtual ~CFileTreeCtrl();

    // return view index (0 for file view)
    int GetViewIndex();

    // load files for project
    BOOL Load();
    BOOL LoadFromXML(CXMLDocument* pXMLDoc);
    BOOL LoadFromProject(CXMLDocument* pXMLDoc);
    // write XML-document to disk (fileview.xml)
    BOOL SaveXML();

    // build complete tree
    virtual BOOL BuildTree();

    // check if files appeared or disappeared in project folder
    void Check(BOOL bSilent = FALSE);

    // check if file cat has changed
    BOOL CheckCategories(CTreeCursor& rtTreeCursor);

    // add context menu
    BOOL AddContextMenu(CMenu* pMenu);

    // context menu commands
    void OnCommand(UINT nID);
    
    // start editing label
    void OnBeginlabeledit(TV_DISPINFO* pDispInfo, LRESULT* pResult);

    // end editing label
    void OnEndlabeledit(TV_DISPINFO* pDispInfo, LRESULT* pResult);

    // update node
    BOOL UpdateNode(const CString& rcs_id_path);

    // cut / copy / paste
    void OnCut();
    void OnCopy();
    void OnPaste();

    // unload project data
    virtual void Unload();

    void EnableFileCheck(BOOL bFileCheck = TRUE);

    BOOL CopyExternalFileToProject(const CString& crstrFilePathName, E_FV_FileCategory eFileCat = FV_FILE_UNKNOWN);

    void OnFileAdded(const CString& crstrFilePath, enum E_FV_FileCategory eFileCat);

    void OnFileRemoved(const CString& crstrFilePath, enum E_FV_FileCategory eFileCat);

    BOOL IsCheckAllowed();

    int AddSourceFiles(CStringArray& rastrFiles, const CString& crstrForeignProjPath);

protected:
    // create DOM-nodes for root, source, help and additional files folder
    BOOL InitializeDOM(CXMLDocument* pXMLDoc = NULL);

    // get all file names from disk and add to tLexTree
    void GetAllFilesFromDisk(CSBLLexTree<bool>& tLexTree, BOOL bUseFilter = FALSE);

    // create new XML node and insert as child
    BOOL InsertNewXMLChildNode(CXMLNode* pParentNode, const CString& rstrName, const CString& rstrType, CXMLNode& rtNodeNew, BOOL bSorted = TRUE);

    // insert XML node as child
    BOOL InsertXMLChildNode(CXMLNode* pNodeParent, CXMLNode* pNodeInsert, BOOL bSorted = TRUE);

    // remove XML node
    BOOL DeleteXMLNode(CXMLNode* pElement);

    // insert XML sibling element before the current element
    CXMLNode* InsertXMLSiblingNodeBefore(CXMLNode* pParentNode, CXMLNode* pBefore, const CString& rstr_name, const CString& rstrType);

    // check files
    BOOL CheckFiles(E_FV_FileCategory eFileCat, CSBLLexTree<bool>& rtFileNameTreeDisk, BOOL bSilent = TRUE);

    // recursive check if all files in a folder are contained in the file name tree
    BOOL CheckFilesExist(   CTreeCursor& rtTreeCursor, 
                            CSBLLexTree<bool>& rtFileNameTreeProject,
                            CSBLLexTree<bool>& rtFileNameTreeDisk,
                            CSBLLexTree<bool>& rtFileNameTreeTree,
                            BOOL bRecursive = TRUE,
                            BOOL bSilent = TRUE);

    // check for new files
    BOOL CheckForNewFiles(  E_FV_FileCategory eFileCat,
                            CSBLLexTree<bool>& rtFileNameTreeProject,
                            CSBLLexTree<bool>& rtFileNameTreeDisk,
                            CSBLLexTree<bool>& rtFileNameTreeTree,
                            BOOL bSilent = TRUE);

    // check for new files
    BOOL CheckRootForNewFiles(CSBLLexTree<bool>& rtFileNameTreeDisk,
                            CSBLLexTree<bool>& rtFileNameTreeTree,
                            BOOL bSilent = TRUE);

    // unload XML
    void UnloadXML();

    // get file category 
    E_FV_FileCategory GetFileCategory(CXMLNode* pNode);

    // get node type tree item (= type of corresponding XML-node)
    CString GetNodeType(CTreeCursor& tTreeCursor);

    // get type id from type name
    EFVNodeType GetTypeFromTypeName(const CString& strType);

    // create DOM-nodes for root, source, help and additional files folder
    BOOL CreateDOMNodes(CXMLDocument* pXMLDoc);
    
    // initialize filters
    void InitFilters();

    // get all files of category from project manager
    BOOL GetAllFilesFromProjMan(enum E_FV_FileCategory eFC, CSBLLexTree<bool>& rtFileNameTree);

    // set tree cursors for source, help and additional
    BOOL SetTreeCursors();

    // rename file
    BOOL RenameFile(CTreeCursor& rtTreeCursor, const CString& crstrNewFileName);

    // rename folder node
    BOOL RenameFolderNode(CTreeCursor& rtTreeCursor, const CString& rstrNewName);

    // create folder node
    void CreateFolderNode();

    // delete context node
    void DeleteContextNode();

    // collect files recursively starting from pNode
    void CollectFiles(CXMLNode* pNode, CSBLLexTree<CXMLNode>& rtLexTree, BOOL bUseFilter = FALSE);

    // insert sorted files as siblings starting from rtTreeCursor
    void InsertSortedSiblings(CTreeCursor& rtTreeCursor, CStringArray& rastrFileNames);

    // insert sorted files as children
    void InsertSortedChildren(CTreeCursor& rtParent, CStringArray& rastrFileNames);

    // is drag allowed
    BOOL IsDragAllowed(CXMLNode* pNode);

    // set clipboard data (cut / copy / drag)
    BOOL SetClipboardData(CXMLNode* pNode, BOOL bDragDrop = FALSE);
    
    // move node
    BOOL MoveNode(CTreeCursor& rtCursorSource, CTreeCursor& rtCursorTarget, BOOL bUpdate = FALSE);
    
    // methods for drag and drop called by CNavTreeDropTarget
    BOOL OnDrop( CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
    DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);

    // get insert item
    virtual CTreeCursor GetNewInsertItem(CPoint& rtPoint);

    // copy file to project
    BOOL CopyFileToProject(const CString& crstrFilePathName, BOOL bSilent = FALSE, BOOL bOverwrite = FALSE);
    
    // insert node from extern
    BOOL InsertNodeFromExtern(CTreeCursor& rtItemInsert, CXMLNode* pNodeToInsert, const CString& crstrProjPath);

    // change file category
    BOOL ChangeFileCategory(const CStringArray& crastrFiles, E_FV_FileCategory eFromCat, E_FV_FileCategory eToCat, BOOL bTry = FALSE);
    BOOL ChangeFileCategory(const CString& crstrFileName, E_FV_FileCategory eFromCat, E_FV_FileCategory eToCat, BOOL bTry = FALSE);

    // get system cursor for file cat (returns root if eFileCat is left)
    CTreeCursor GetSystemCursor(E_FV_FileCategory eFileCat = FV_FILE_UNKNOWN);

    // get system node
    BOOL GetSystemNode(E_FV_FileCategory eFileCat, CXMLNode& rtNode, CXMLDocument* pXMLDoc = NULL);

    // is clipboard data available?
    BOOL IsClipboardDataAvailable(UINT uiFormat);
    
    virtual BOOL ExpandXMLNode(CXMLNode* pNode, BOOL bExpand = TRUE, BOOL bRecursive = FALSE);

    void OnAddFile();

    int GetFileFilterForCategory(E_FV_FileCategory eCategory, CString& rstrFilter);

    CString GetTypeForSystemNode(E_FV_FileCategory eFileCat);

    void StartFileChangeNotificationThread(const CString& crstrFolder);

    void StopFileChangeNotificationThread();

    static UINT ThreadFunc(LPVOID pParam);

    CString GetFilePath(const CString& crstrFileName);

    BOOL SetContextCursor(CTreeCursor& rtContextCursor);

    BOOL IsFileVisible(const CString& crstrFileName);

    BOOL RemoveFile(const CString& crstrPathName, BOOL bRecycler = TRUE);

    void SetProjectName();

    BOOL FindFileItem(const CString& crstrFileName, CTreeCursor& rtCursorFile);
    BOOL FindFileNode(const CString& crstrFilePath, CXMLNode& rtNode);

    // virtual methods for adding and removing tree items
    virtual CTreeCursor InsertItemAfter(const CString& crstrText, CTreeCursor& rtCursorParent, HTREEITEM hItemAfter, CXMLNode& rtNode, int iImageIdx = -1);
    virtual CTreeCursor AddItemTail(const CString& crstrText, CTreeCursor& rtCursorParent, CXMLNode& rtNode, int iImageIdx = -1);
    virtual CTreeCursor AddItemHead(const CString& crstrText, CTreeCursor& rtCursorParent, CXMLNode& rtNode, int iImageIdx = -1);
    virtual void DeleteTreeItem(CTreeCursor& rtCursor);

    // is file existing on disk?
    BOOL IsFileExisting(const CString& crstrFileString);
    BOOL IsFileReadOnly(const CString& crstrFileString);
    BOOL IsFileReadOnly(CTreeCursor& rtTreeCursor);

    // save project file.
    BOOL SaveProjectFile();

    virtual void CheckNodeSourceState(CTreeCursor& rtTreeCursor, BOOL bRecursive = TRUE);

protected:
    BOOL            m_bLoaded;              // project loaded?
    BOOL            m_bChecking;            // checking?
    int             m_iCheckDisableCount;        // check allowed, if m_iCheckAllow == 0
    BOOL            m_bCatChangeAllowed;    // category changes allowed?

    CKADFileInfo    m_tFileInfo;            // file info from KAD

//    CXMLNode        m_tNodeSource;          // source files system folder node
//    CXMLNode        m_tNodeHelp;            // help files system folder node
//    CXMLNode        m_tNodeAdditional;      // additional files system folder node

    EFVNodeType m_eContextType;             // context type in addition to context type name
    CEvent      m_event;                    // event for file changes
    HANDLE      m_hThread;
    CWinThread* m_pThread;

    CTreeCursor m_tContextCursor;           // cursor to currently selected item

    CMap<CString, LPCTSTR, HTREEITEM, HTREEITEM>  m_tFileAssoc;     // associative memory: file name -> tree item
    CMap<CString, LPCTSTR, CXMLNode*, CXMLNode*>  m_tFileNodeAssoc; // associative memory: file name -> tree item

    BOOL        m_bRejectUpdateMessages;    // reject update messages from project manager?
                                            // needed, if actions are triggered within file
                                            // view
};

#endif // __FILETREECTRL_H_
