#if !defined(AFX_CEWATCHLIST_H__B40EC748_E20D_11D2_A009_00A024363DFC__INCLUDED_)
#define AFX_CEWATCHLIST_H__B40EC748_E20D_11D2_A009_00A024363DFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CeWatchList.h : header file
//
#include "CeWatchElement.h"
#include "CeDisplayElement.h"
#include "XMLHelper/XMLNode.h"
#include "XMLHelper/XMLDocument.h"

/////////////////////////////////////////////////////////////////////////////
// CCeWatchList window
#define WM_CEWATCH_VARLISTCHANGED     (WM_USER + 1)
#define WM_CEWATCH_LOADLIST           (WM_USER + 2)
#define WM_CEWATCH_STORELIST          (WM_USER + 3)
#define WM_CEWATCH_ENDEDIT            (WM_USER + 4)
#define WM_CEWATCH_UPDATECONNECTIONS  (WM_USER + 5)

#define CEWATCH_NCOLUMNS (IDS_HEADER_FORMAT - IDS_HEADER_NAME + 1)
#define CEWATCH_SUBITEM_VALUE (IDS_HEADER_VALUE - IDS_HEADER_NAME)

class CCeWatchBackEnd;
class CCeWatchEditContainer;

#define CEWATCH_FILE_VERSION "1.23"

class CCeWatchDropTarget : public COleDropTarget
{
public:
    virtual DROPEFFECT OnDragEnter (CWnd *pWnd, COleDataObject *pDataObject, DWORD dwKeyState, CPoint point);
    virtual DROPEFFECT OnDragOver (CWnd *pWnd, COleDataObject *pDataObject, DWORD dwKeyState, CPoint point);
    virtual BOOL OnDrop (CWnd *pWnd, COleDataObject *pDataObject, DROPEFFECT dropEffect, CPoint point);
};

class CCeWatchList : public CListCtrl
{
// Construction
public:
	CCeWatchList();

// Attributes
public:
	inline int GetColumnCount() const { return m_nColumns; }
	void  SetBackEnd(CCeWatchBackEnd* pBackEnd);

// Operations
public:
	void Init();
	int InsertExpression(LPCTSTR pstrName, CCeWatchElement *&pInsertedElement, int iItem=-1);
    int InsertExpressionWithStructInfo(LPCTSTR pstrName, CCeWatchElement *&pInsertedElement, int iItem = -1);
	int InsertVariable(CCeSymbol *pSymbol, int iItem=-1);
	int InsertVariable(CCeWatchElement *pVar, int iItem=-1);

//    bool IsVarKindVisible(ECeWatchVarKind varKind);

	CCeDisplayElement* GetSelectedDisplayElement();
    void GetSelectedDisplayElements (CArray<CCeDisplayElement *, CCeDisplayElement *> &displays);
	CCeWatchElement* GetSelectedVar();
	bool DeleteSelectedVar();
	bool DeleteAllVar();
	
    bool Load(LPCTSTR strFileName);
	bool Store(LPCTSTR strFileName);
   
    // SIS: save and load in XML
    bool SaveToNode(CXMLNode& rtNode, CXMLDocument& rtDocument);
    bool LoadFromNode(CXMLNode& rtNode);

    // SIS: disconnect all variables
    void Deactivate();

    void OnForceResubscribe();
	void UpdateConnections();
	void UpdateItem(CCeDisplayElement *pD);
	bool CheckVariableInsert();
	bool CheckVariableDelete();
	bool CheckNameEdit();
	bool CheckValueEdit();
	bool CheckFormatEdit();
	bool CheckIncRange(bool bDec);
	bool MoveVarItem(int SrcItem, int DestItem);
	CImageList* CreateDragImage(int iItem, LPPOINT lpPoint);

	// 14.12.05 SIS >>
	bool IncRange(bool bDec);
	// 14.12.05 SIS <<

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCeWatchList)
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

// Implementation
public:
	virtual ~CCeWatchList();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCeWatchList)
	virtual LRESULT OnEndLabelEdit(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnUpdateConnections(WPARAM wParam, LPARAM lParam);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnRemoveAllVariables();
	afx_msg void OnRemoveSelectedVariables();
	afx_msg void OnAddVariableFromList();
	afx_msg void OnAddVariable();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLoadWatches();
	afx_msg void OnStoreWatches();
	afx_msg void OnDeleteItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBeginLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEditFormat();
	afx_msg void OnEditName();
	afx_msg void OnEditValue();
	afx_msg void OnDecRange();
	afx_msg void OnIncRange();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnDblClick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	BOOL LoadSymbols();		// 10.10.06 SIS

private:
	CString FormatText(CDC* pDC, const CString& Text, int MaxWidth);
	int InsertVarItem(CCeWatchElement* pVar, int index=-1, CArray<CString*, CString*>* pItemData=NULL, bool bEnsureVisible=false); 
    int AddVarItem(CCeWatchElement* pVar);
	void DeleteSubTree(int iItem);
	void InsertSubTree(int iItem);
	void ToggleSubTree(int iItem);
	int GetSelectedItem(); 
	int GetNextInsertPos(int iItemStart);	// 15.12.05 SIS
	void BeginLabelEdit(int iItem, int xPos);
	void EndLabelEdit(WPARAM wParam=IDOK);
	bool EndNameEdit(); 
	bool EndValueEdit(); 
	bool EndFormatEdit(); 
	bool GetEditRect(int iItem, int iEditType, CRect& rect);
	bool ExpandTree(bool bCollapse);

	CCeDisplayElement* GetDisplayElement(int iItem);
	CCeWatchElement* GetVar(int iItem);
	unsigned long GetVertLineMask(int iItem);
	CString GetFormat(int iItem);
	LONG GetExpressionHandle(int iItem);
	bool IsExpanded(int iItem);
	void SetExpanded(int iItem, bool bExpanded);

	// display range of array members
	CCeWatchIndices* GetWatchIndices(int iItem);

	bool DeleteVar(int iItem);
	unsigned long CalcVertLineMask(int iItem) ;
	void UpdateVertLineMasks(int iStartItem);
	bool GetDropTarget(CPoint point, int &iDestItem, int iSrcItem=-1); 

	void ErrorMsg(unsigned IdsMsg); 

	void RefreshWindow();

    BOOL StripId(CString& rstrIdPath, CString& rstrId);
    void SaveExpandInfo();
	
protected:
	CCeSymbol*		m_pRootSymbol;		// 10.10.06 SIS

private :
	CImageList       m_ImageList;
	CCeWatchElement  m_RootVar;
	int              m_nColumns;
	int              m_xIndent;

	CCeWatchEditContainer*  m_pEditContainer;

	CComboBox*       m_pEditValueWnd;

	CImageList*      m_pDragImage;
	int              m_iSrcItem;

	int             m_HeaderWidth[CEWATCH_NCOLUMNS];

// backend for communication
private:
	CCeWatchBackEnd* m_pBackEnd; // pointer to backend wrapper class

// Drag and Drop of expressions from editor
public:
    DROPEFFECT OnDragEnter (COleDataObject *pDataObject, DWORD dwKeyState, CPoint point);
    DROPEFFECT OnDragOver (COleDataObject *pDataObject, DWORD dwKeyState, CPoint point);
    BOOL OnDrop (COleDataObject *pDataObject, DROPEFFECT dropEffect, CPoint point);

private:
    UINT m_uiWatchExpressionCBFormat;
	CCeWatchDropTarget* m_pDropTarget;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CEWATCHLIST_H__B40EC748_E20D_11D2_A009_00A024363DFC__INCLUDED_)
