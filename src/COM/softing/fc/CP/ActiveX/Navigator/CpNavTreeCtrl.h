// FcNavTreeCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCpNavTreeCtrl window

#define WM_FCNAV_NEWURL			(WM_USER + 1)
#define PARAM_NEW_WINDOW		0x01

class CCpObjectList;
class CCpNavTreeCtrl : public CTreeCtrl
{
// Construction
public:
	CCpNavTreeCtrl();
	virtual ~CCpNavTreeCtrl();

// Attributes
public:
	inline CCpObjectList* GetObject(HTREEITEM hItem) const {
		ASSERT_VALID(this);
		ASSERT(hItem != NULL);
		return (CCpObjectList*)GetItemData(hItem); }

	unsigned GetItemLevel(HTREEITEM hItem);
// Operations
public:
	BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID);
	HTREEITEM InsertItem(HTREEITEM hParenItem, CCpObjectList* pObject);
	BOOL InsertSubTree(HTREEITEM hParentItem, CCpObjectList* pParent, unsigned InitialDisplayLevel);

	BOOL ShowObjectList(CCpObjectList* pObjectList, unsigned InitialDisplayLevel=99, unsigned ClassFilter=0); 
	void DisplayAsync(CCpObjectList* pObjectList, unsigned InitialDisplayLevel=99, unsigned ClassFilter=0);
	void StopDisplayAsync();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCpNavTreeCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
protected:
	//{{AFX_MSG(CCpNavTreeCtrl)
	afx_msg void OnItemExpanded(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	CImageList		m_ImageList;	// tree icons
	unsigned		m_ClassFilter;	// only objects matching the 
									// class filter mask are display

#define FCNAV_IMAGE_PROJECT				0
#define FCNAV_IMAGE_CONFIGURATION		2
#define FCNAV_IMAGE_WEBRESSOURCE		4
#define FCNAV_IMAGE_CONTROLRESSOURCE	6
#define FCNAV_IMAGE_TASK				8
#define FCNAV_IMAGE_PROGRAM				10
#define FCNAV_IMAGE_FUNCTIONBLOCK		12
#define FCNAV_IMAGE_VARIABLE			14
#define FCNAV_IMAGE_LIST				16
#define FCNAV_IMAGE_INVALIDENTRY		18

// hide public functions, which would destroy the internal structure
private :
	HTREEITEM InsertItem(LPTV_INSERTSTRUCT lpInsertStruct);
	HTREEITEM InsertItem(UINT nMask, LPCTSTR lpszItem, int nImage, int nSelectedImage, UINT nState, UINT nStateMask, LPARAM lParam, HTREEITEM hParent, HTREEITEM hInsertAfter);
	HTREEITEM InsertItem(LPCTSTR lpszItem, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);
	HTREEITEM InsertItem(LPCTSTR lpszItem, int nImage, int nSelectedImage, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);

private :
	BOOL m_bCtrlKey;

// object checking
public:
#ifdef _DEBUG
    virtual void AssertValid() const;    // Override
#endif

};

/////////////////////////////////////////////////////////////////////////////



