#ifndef __CEMAINVIEW_H_
#define __CEMAINVIEW_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


//----  Aggregate Includes:   --------------------------------------*
#include <afxcview.h>
#include "XMLHelper/XMLDocument.h"
#include "XMLHelper/XMLNode.h"
#include "XMLHelper/XMLNodeList.h"
#include "ctrlext.h"
#include "SBLLexTree.h"
#include "KADHelper/KADInfo.h"
#include "DragItem.h"

extern CComModule _Module;
#include "ProjManListEvents.h"

//----  Forward Class Definitions:   -------------------------------*
class CCEMAINCntrItem;
class CCEMAINDoc;
class CCEFileHandler;

//----  Defines:    ------------------------------------------------*

//----  Globals:    ------------------------------------------------*

//----  Prototypes:    ---------------------------------------------*

/////////////////////////////////////////////////////////////////////////////



//------------------------------------------------------------------*
/**
 *  class CPLVItemData
 *
 *  encapsulates id path and type for a node
 *
 *  @see    ....
*/
class CPLVItemData
{
public:
    CPLVItemData() {};
    CPLVItemData(const CString& crstrIdPath, const CString& crstrType, const CString& crstrSource)
    {
        m_strIdPath = crstrIdPath;
        m_strType = crstrType;
        m_strSource = crstrSource;
    }

    CString             m_strIdPath;
    CString             m_strType;
    CString             m_strSource;
};


typedef CMap<long, long, CPLVItemData, CPLVItemData&> TItemDataMap;


class CCEMAINView : public CListView
{
protected: // create from serialization only
	CCEMAINView();
	DECLARE_DYNCREATE(CCEMAINView)

    // help class for sorting, used like a structure
    class CSortData
    {
    public:
        CSortData(CListCtrlEx* pListCtrl, int iCol, CSBLLexTree<int>* pIdPathTree, TItemDataMap* pIdMap)
        {
            m_pListCtrl = pListCtrl;
            m_iCol = iCol;
            m_pIdPathTree = pIdPathTree;
            m_pIdMap = pIdMap;
        }

        CSBLLexTree<int>*   m_pIdPathTree;
        TItemDataMap*       m_pIdMap;
        CListCtrlEx*        m_pListCtrl;
        int                 m_iCol;
    };

	void AdjustColumnWidths(void);
	void DisplayColumnHeadings(UINT iStringId);
	void EraseList();
    void EraseListData();
	void ShowObjects(long lOrder = 1);
	BOOL ObjClassToString(long lObjClass, CString& strObjClass);
								// converts the ObjClass-Id to the corresponding text
    void EditLabel(int nItem);
                                    // enters the EditLable mode of the list control                
    void ActivateItem (int iItem);

    BOOL BuildList();
    void AddNodeToList(CXMLNode& rtNode, CListCtrlEx& rtListCtrl, BOOL bRecursive = TRUE);
    void AddNode(CXMLNode& rtNode, CListCtrlEx& rtListCtrl);

    void CollectIdPaths(CXMLNode& rtNode, CSBLLexTree<bool>& rtIdPathsNodeTree);
    void DeleteListItem(int iItem, BOOL bMarkOnly = FALSE);
    void UpdateDelete();

    int GetFirstSelectedItem();

    CString GetProjectTitle();
    
    BOOL GetXMLDocuments(CXMLDocument& rtClassDoc, CXMLDocument& rtTargetDoc);
    BOOL GetXMLNodeFromIdPath(const CString& crstrIdPath, CXMLNode& rtRootNodeClass, CXMLNode& rtRootNodeTarget, CXMLNode& rtNode);
    BOOL GetXMLNodeFromIdPath(const CString& crstrIdPath, CXMLNode& rtNode);

    HRESULT AdviseProjEventHandler();
    void UnadviseProjEventHandler();

    void SortColumn(int iCol);
    void UpdateItemNumbers();

    void UpdateSingleNode(CXMLNode& rtNode, const CString& crstrIdPath = "");

    static int CALLBACK CompareItems(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

    void DeleteSelectedItems();
    int  GetSelectedNodeIdPaths(CStringArray& rastrIdPath);
    void DeleteNodes(CStringArray& rastrIdPath);

    DROPEFFECT GetDropEffect(DWORD dwKeyState);

    BOOL GetItemData(int iItem, CPLVItemData& rtItemData);
    BOOL IsDragAllowed(const CString& crstrType);

    BOOL DropInXMLNodeFormat(CXMLNodeDragItem& rtDragItem, DROPEFFECT dropEffect);
    BOOL DropInXMLFormat(CXMLDragItem& rtDragItem, DROPEFFECT dropEffect);

    HRESULT SetEditorClipboardData(CXMLNode& rtNode, COleDataSource* pDataSource);

    int GetSelectedIdPaths(CStringArray& rastrSelIdPaths);

    BOOL CopyNodesToClipboard(CStringArray& rastrSelIdPaths, BOOL bDragDrop = FALSE);

    void CollectFiles(CXMLNode& rtNode, CStringArray& rastrFiles);

    void CheckSourceState(int iItem, const CString& crstrSourceFilePathName, CCEFileHandler* pFileHandler = NULL);

// Attributes
public:
	CCEMAINDoc* GetDocument();

    CString GetProjectPath();

    // update structure and attributes of all nodes below
    void UpdateTreeNode(const CString& crstrIdPath);

    // delete node
    void DeleteTreeNode(const CString& crstrIdPath);
    // update attributes of node
    void UpdateTreeNodeAttributes(const CString& crstrIdPath);
    // insert tree node and all nodes below
    void InsertTreeNode(const CString& crstrIdPath);
    // update source state
//    void UpdateSourceState(const CString& crstrIdPath);

    // on idle
    BOOL OnIdle(LONG lCount);

protected:
    CKADInfo*   m_pKADInfo;         // pointer to KAD-info object
	UINT		m_uiLibCBFormat;    // drag & drop lib format
    UINT        m_uiCBXMLFormat;    // drag & drop xml format
	UINT        m_uiCBXMLNodeFormat;    // drag & drop XML node format
    int			m_iColumns;		    // number of columns in list
    CImageList  m_tImageList;       // image list
    CImageList  m_tStateImageList;  // state image list of list control

    int         m_iItemCount;       // number of items = lines
    long        m_lItemId;          // item id
    int         m_iSortCol;         // column by which is sorted
    BOOL        m_bSortInverse;     // inverse sorting

    CSBLLexTree<int>    m_tIdPathTree;  // lexical tree ID-Path -> item number = line number
    TItemDataMap        m_tIdMap;       // map id number (data element) -> ID-Path
	COleDropTarget      m_tDropTarget;  // for dropping

private:
	CCEMAINDoc*	m_pPrjDoc;
    CComObject<CProjManListEvents>* m_pProjEventHandler;
	DWORD                           m_dwProjEventHandlerCookie;

    CString         m_strContextIdPath; // context id path for ShowNode()

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCEMAINView)
	public:
	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual void OnDragLeave();
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCEMAINView();
//	CImageList m_ClImageList;		// to hold the bitmap for images of the ListView

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    CString GetPOUNameFromLabel(int iItem, int iSubItem);

// Generated message map functions
protected:
	//{{AFX_MSG(CCEMAINView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnReturnPressed(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnDestroy();
    afx_msg void OnDelete();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginrdrag(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnShowNode();
	//}}AFX_MSG
    afx_msg void OnContextMenuCommand(UINT nID);
	DECLARE_MESSAGE_MAP()

// old stuff generated by the wizard:
//public:
	// m_pSelection holds the selection to the current CCEMAINCntrItem.
	// For many applications, such a member variable isn't adequate to
	//  represent a selection, such as a multiple selection or a selection
	//  of objects that are not CCEMAINCntrItem objects.  This selection
	//  mechanism is provided just to help you get started.

	// TODO: replace this selection mechanism with one appropriate to your app.
//	CCEMAINCntrItem* m_pSelection;

};

#ifndef _DEBUG  // debug version in CEMAINView.cpp
inline CCEMAINDoc* CCEMAINView::GetDocument()
   { return (CCEMAINDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __CEMAINVIEW_H_

