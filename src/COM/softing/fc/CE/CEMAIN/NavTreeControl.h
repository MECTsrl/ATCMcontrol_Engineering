#ifndef __NAVTREECONTROL_H_
#define __NAVTREECONTROL_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*

#include "XMLHelper/xmlnode.h"
#include "XMLHelper/xmlnodelist.h"
#include "XMLHelper/xmldocument.h"
#include "KADHelper/KADInfo.h"
#include "KADHelper/KADMenu.h"
#include "NavDef.h"
#include "Ctrlext.h"
//#include "regex.h"
#include "CEProjMan.h"
#include "NavTreeDropTarget.h"
#include "DragItem.h"

//----  Forward Class Definitions:   -------------------------------*
class CTargetOnlBase;
class CNavigator;
class CPrintSettings;

//----  Defines:    ------------------------------------------------*
//typedef CMap<CXMLNode*, CXMLNode*, CTreeCursor*, CTreeCursor*> T_NodeItemMap;
//typedef CList<CXMLNode*, CXMLNode*> T_NodePtrList;

typedef enum EContextAction
{
    eCut,
    eCopy
};


// TODO: defined elsewhere?
typedef enum ERunState
{
    eError = -1,
    eInit,          // 0
    eStopped,       // 1
    eRunning        // 2
};

typedef enum eTreePrintMode
{
    eTPM_full       = 0x01,     // full expanded
    eTPM_current    = 0x02,     // currently expanded
    eTMP_text       = 0x04,     // text output
    eTMP_grafic     = 0x08,     // graphic output
};

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

// navigator tree control base class

//------------------------------------------------------------------*
/**
 *  class CNavTreeCtrl
 *
 *  - base class for navigator tree views: file, class, instance and target view
 *  - covers main functionality of these views:
 *    - building up tree from XML
 *    - update mechanisms triggered by project manager
 *    - KAD context menus and calls to add on handlers
 *    - item states described in KAD
 *    - drag / drop  and cut / paste operations described in KAD
 *    - text and KAD type filters
 *  - each tree item contains pointer to corresponding XML-node (CXMLNode*)
 *
 *  @see    CFileTreeCtrl, CClassTreeCtrl, CInstTreeCtrl, CHWTreeCtrl
 */
class CNavTreeCtrl : public CTreeCtrlEx
{
public:
    // construction / destruction
    CNavTreeCtrl();
    virtual ~CNavTreeCtrl();

    // initialization
    virtual void Init(CKADInfo* pKADInfo);

    // load data: set project name and path
    virtual BOOL Load();

    // set XML pointers
    virtual BOOL InitializeDOM(CXMLDocument* pXMLDoc = NULL);

    // build complete tree
    virtual BOOL BuildTree();

    // add tree context menu
    virtual BOOL AddContextMenu(CMenu* pMenu);

    BOOL AddContextMenuForNode(CMenu* pMenu, CXMLNode* pNode);

    // context menu commands
    virtual void OnCommand(UINT nID);

    void OnCommand(long lItem, CXMLNode& rtNode);

    // update node: attributes and node structure
    virtual BOOL UpdateNode(const CString& crstrIdPath);

    // update attributes of node
    virtual BOOL UpdateAttributes(const CString& crstrIdPath);

    // update delete node
    virtual BOOL UpdateDelete(const CString& crstrIdPath);

    // update insert node
    virtual BOOL UpdateInsert(const CString& crstrIdPath);

    // update source state
//    virtual BOOL UpdateSourceState(const CString& crstrIdPath);

    // after updating all nodes call this function to update tree view
    void InvalidateTree();

    // set filters (i. e. set expressions and apply filter)
    void SetFilters(int iSelCat, const CString& rstrFilter2);

    // item is expanded or collapsed
    BOOL OnItemexpanding(TVITEM& rtItem, BOOL b_expanded);

    // unload project data
    virtual void Unload();

    // start editing label
    virtual void OnBeginlabeledit(TV_DISPINFO* pDispInfo, LRESULT* pResult);

    // end editing label
    virtual void OnEndlabeledit(TV_DISPINFO* pDispInfo, LRESULT* pResult);

    // begin dragging
    virtual void OnBegindrag(NM_TREEVIEW* pNMTreeView, LRESULT* pResult);

    // store filter data for view
    void SetFilterData(int iSelCat, int iSelText, CStringArray* pastrFilterText = 0);

    // retrieve filter data for view
    void GetFilterData(int& riSelCat, int& riSelText, CStringArray& astrFilterCat, CStringArray& astrFilterText);

    // is filter active?
    BOOL IsFilterActive();

    // get view index (0:= File, 1:= Class, 2:= Hardware, 3:= Instance, -1:= unknown)
    virtual int GetViewIndex();

    // get return/escape key when renaming a tree item
    BOOL PreTranslateMessage(MSG* pMsg);

    // cut / copy / paste
    virtual void OnCut();
    virtual void OnCopy();
    virtual void OnPaste();

    // collapse / expand all
    void OnCollapseAll();
    void OnExpandAll();

    void ClearStates();

    BOOL ShowContextMenu(CWnd* pWnd, CPoint tPoint);
    BOOL AddContextMenuForNode(CMenu* pMenu, const CString& crstrIdPath);

    BOOL IsTreePath(const CString& crstrIdPath);

    void ShowContextMenu(const CString& crstrIdPath, CPoint tPoint);

    BOOL DeleteNodesByIdPath(CStringArray& rastrIdPath);

    BOOL OpenNode(const CString& crstrIdPath);

    BOOL IsKeyMouseActionAllowed(const CString& crstrIdPath, int iKeyMouseEvent);
    void RenameNode(const CString& crstrIdPath, const CString& crstrNewId);

    BOOL ShowNode(const CString& crstrIdPath);

    void OnContextMenuCommand(UINT nID);

    BOOL Print(CPrintSettings* pPrintSettings, CString& rstrFilePathName);

    void CheckSourceState();

protected:
	//{{AFX_MSG(CNavTreeCtrl)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);	// SHIFT_F10 15.12.04 SIS
	//}}AFX_MSG

    DECLARE_MESSAGE_MAP()

    // clear tree control
    void ClearTree(BOOL bUnloadXML = TRUE);

    // build up tree structure from DOM recursively (no filter)
    BOOL FillChildNodes(CTreeCursor& rtTreeCursor);

    // delete tree node data recursively
    BOOL DeleteTreeNodeData(CTreeCursor& rtTreeCursor);

    // expand tree structure
    BOOL ExpandNodeFromDom(CTreeCursor& rtTreeCursor);

    // translate wildcard expression into regular expression
    CString GetRegularExpression(const CString& rstrFilter);

    // is text passing filters?
    virtual BOOL IsPassingFilters(CString strText, CString strType);

    // reset filters
    void ResetFilters();

    // is XML-node expanded?
    BOOL IsXMLNodeExpanded(CXMLNode* pNode);

    // get tree item from node-id-path
    BOOL GetTreeCursorFromNodeIdPath(const CString& rcsIdPath, CTreeCursor& rtTreeCursor);

    // find child with id
    BOOL FindChildFromId(CTreeCursor& rtTreeCursor, CTreeCursor& rtChild, const CString& rcs_id);

    // add menu item
    void AddContextMenuItem(CMenu* pMenu, CKADMenuItem* pItem, CXMLNode* pNode, long lItem, BOOL bSourceReadOnly);

    // invoke context menu item
    void DoContextAction(long lItem);

    // is kernel standard operation possible
    virtual BOOL IsKSOPossible(int iKSO, CXMLNode* pNode);

    // execute kernel standard operation
    virtual BOOL DoKernelStandardOperation(int iKSO, CXMLNode* pNode);

    // do exe operation
    virtual void DoExeOperation(CXMLNode& rtNode, CKADMenuItem* pItem);

    // execute add on action
    virtual void DoAddOnAction(CKADMenuItem* pItem, BOOL bSilent = FALSE);

    // release XML data
    virtual void UnloadXML();

    //expand, collapse or toggle tree item
    void ExpandTreeItem(CTreeCursor& rtTreeCursor, UINT nCode = TVE_EXPAND, BOOL bRecursive = FALSE);

    // delete all children of a tree item
    BOOL DeleteTreeNodeChildren(CTreeCursor& rtTreeCursor);

    virtual void UpdateTreeNode(CTreeCursor& rtTreeCursor, CXMLNode& rtNode);

    void UpdateTreeNodeDeleteRun(CTreeCursor& rtTreeCursor, CXMLNode& rtNode);
    void UpdateTreeNodeInsertRun(CTreeCursor& rtTreeCursor, CXMLNode& rtNode);
    void UpdateItem(CTreeCursor& rtTreeCursor, CXMLNode& rtNode);
    void UpdateItem(CTreeCursor& rtTreeCursor);

    CTreeCursor InsertNewItemAfter(CTreeCursor& rtCursorParent, CTreeCursor& rtCursorChildBefore, CXMLNode& rtNodeToInsert);

    // virtual methods for adding tree items
    virtual CTreeCursor InsertItemAfter(const CString& crstrText, CTreeCursor& rtCursorParent, HTREEITEM hItemAfter, CXMLNode& rtNode, int iImageIdx = -1);
    virtual CTreeCursor AddItemTail(const CString& crstrText, CTreeCursor& rtCursorParent, CXMLNode& rtNode, int iImageIdx = -1);
    virtual CTreeCursor AddItemHead(const CString& crstrText, CTreeCursor& rtCursorParent, CXMLNode& rtNode, int iImageIdx = -1);
    virtual void DeleteTreeItem(CTreeCursor& rtCursor);

    BOOL IsCorresponding(CTreeCursor& rtTreeCursor, CXMLNode& rtNode);
    BOOL IsNodeToBeInserted(CXMLNode& rtNode);

    // filter nodes that are passing filter recursively
    BOOL FilterNodes(CXMLNode* pNode);

    // initialize filters
    virtual void InitFilters();

    // is drag allowed
    virtual BOOL IsDragAllowed(CXMLNode* pNode);
    virtual BOOL IsDropAllowed(CXMLNode* pDragNode, CXMLNode* pDropNode);

    BOOL GetDropHandler(CXMLNode* pDragNode, CXMLNode* pDropNode, GUID& rtGUID);

    BOOL DoDragDropAction(CTreeCursor& rtDragCursor, CTreeCursor& rtDropCursor, DROPEFFECT dropEffect, CXMLNodeDragItem& rtDragItem);
    
    // set drag data 
    virtual BOOL SetClipboardData(CXMLNode* pNode, BOOL bDragDrop = FALSE);
    HRESULT SetInternalClipboardData(CXMLNode* pNode, COleDataSource* pDataSource);
    HRESULT SetEditorClipboardData(CXMLNode* pNode, COleDataSource* pDataSource);

    // virtual methods for drag and drop called by CNavTreeDropTarget
    virtual BOOL OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
    virtual DROPEFFECT OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
    virtual void OnDragLeave(CWnd* pWnd);
    virtual DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
    virtual DROPEFFECT OnDragScroll(CWnd* pWnd, DWORD dwKeyState, CPoint point);
    virtual DROPEFFECT OnDropEx(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, DROPEFFECT dropEffectList, CPoint point);
    virtual DROPEFFECT GetDropEffect(CTreeCursor& rtTreeCursor, DWORD dwKeyState);

    virtual CDocument* OpenSourceFile(const CString& crstrFile, const CString& crstrLocation = _T(""));
    virtual CDocument* OpenSourceFileAsText(const CString& crstrFile);

    void OpenUrl(const CString& strSource);

    CString GetCategoryFilterExpr(int iSelCat);

    virtual BOOL ExpandXMLNode(CXMLNode* pNode, BOOL bExpand = TRUE, BOOL bRecursive = FALSE);

    BOOL GetImageIds(CXMLNode* pNode, int& riImageId, int& riOverlay);

    void UpdateTreeItemState(CTreeCursor& rtTreeCursor, CXMLNode* pNode = NULL, BOOL bRecursive = FALSE);

    void ClearStates(CXMLNode& rtNode);

    void SaveAllFiles(bool bAskBefore = false);

    CTargetOnlBase* GetTargetOnlObject(const CString& crstrTargetName);

    int GetTaskRunInfo(CXMLNode* pNode, int& riNumTasksRunning, int& riNumTasksNotRunning);

    void SelectLastInsertNode();

    // get insert item
    virtual CTreeCursor GetNewInsertItem(CPoint& rtPoint);

    BOOL FindTreeNode(const CString& crstrIdPath, CXMLNode& rtNode);

    BOOL DoDeleteNodeAction(CXMLNode& rtNode, BOOL bSilent = FALSE);

    BOOL UnlinkNode(CXMLNode* pNode);

    BOOL RemoveFile(const CString& crstrFilePathName);

    void PrintToFile(CStdioFile& rtFile, CTreeCursor& rtTreeCursor, const CString& crstrIndentAcc);

    BOOL ExpandAttributeMacros(CXMLNode& rtNode, CString& rstrCommand);

    BOOL ExtractNamedMacro(const CString& crstrName, CString& rstrRest, CString& rstrMacroText);

    virtual void CheckNodeSourceState(CTreeCursor& rtTreeCursor, BOOL bRecursive = TRUE);

    BOOL IsKeyMouseActionAllowed(CXMLNode& rtNode, int iKeyMouseEvent);

    BOOL IsProjectReadOnly();

    BOOL EvaluateExpression(const CString& crstrExpression, BOOL bProjectReadOnly, BOOL bProjectOpen, BOOL bSourceReadOnly);

protected:
    BOOL            m_bInitialized;     /**< initialized? */
    CString         m_strProjectName;   /**< name of currently loaded project */
    CString         m_strProjectPath;   /**< path of the current project */

    CXMLDocument*   m_pXMLDoc;          /**< pointer to XML document */
    CTreeCursor     m_tCursorRoot;      /**< cursor on root item */

    CKADInfo*       m_pKADInfo;         /**< pointer on KAD info object residing in navigator */
    CImageList      m_tImageList;       /**< main image list of tree control */
    CImageList      m_tStateImageList;  /**< state image list of tree control */
    CString         m_strRenameText;    /**< item text of the context item (used for rename) */
    CString         m_strRenameId;      /**< item id of context item (used for rename) */
    CString         m_strContextIdPath; /**< id path of context node */

    CStringArray    m_astrFilterText;   /**< array with text filter expressions */
    int             m_iSelCat;          /**< zero based index of currently selected category filter */
    int             m_iSelText;         /**< zero based index of currently selected text filter */

    
    CComQIPtr<ICEProjectManager>    m_pProjMan;     /**< pointer to project manager of navigator */
    CComQIPtr<ICEProjInfo>          m_pProjInfo;    /**< pointer to project information */

	CImageList*     m_pDragImageList;   /**< image list for dragging */
  	BOOL            m_bDragging;        /**< dragging? */
	
	CTreeCursor     m_tDragCursor;      /**< cursor to tree item dragged from */

    CNavTreeDropTarget  m_tDropTarget;  /**< custom drop target object */
    UINT            m_uiCBXMLNodeFormat;    /**< clipboard format for XML nodes */
    UINT            m_uiCBXMLFormat;    /**< clipboard format for editor */
    int             m_iScrollCounter;   /**< counts OnDragScroll() calls */

    COLORREF        m_tBgColFilter;
    EContextAction  m_eContextAction;   /**< cut / copy? */

    CString         m_strInsertIdPath;  /**< id path of last inserted item (for selection) */

    CNavigator*     m_pNavigator;       /**< needed to activate / deactivate file checking */

    // filter data
    BOOL            m_bFilter1;         /**< filter 1 active? */
    CString         m_strFilter1;       /**< filter string 1 */
    BOOL            m_bFilter2;         /**< filter 2 active? */
    CString         m_strFilter2;       /**< filter string 2 */
    BOOL            m_bCatFilter;       /**< category filter active? */
    CKADFilterInfo* m_pCatFilter;       /**< pointer to category filter info object */
    BOOL            m_bUseCase;         /**< distinguish between upper and lower case? */

//    regex_t         m_tRegex1;          /**< filter data for last regular expression 1 */
//    regmatch_t*     m_pRegMatch1;       /**< filter data for last regular expression 1 */
//    int             m_iGroups1;         /**< filter data for last regular expression 1 */
//    regex_t         m_tRegex2;          /**< filter data for last regular expression 2 */
//    regmatch_t*     m_pRegMatch2;       /**< filter data for last regular expression 2 */
//    int             m_iGroups2;         /**< filter data for last regular expression 2 */

    friend class CNavTreeDropTarget;
};

#endif // __NAVTREECONTROL_H_

