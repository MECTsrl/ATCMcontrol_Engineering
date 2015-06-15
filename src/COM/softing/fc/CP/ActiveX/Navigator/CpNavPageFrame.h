// FcNavPageFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCpNavPageFrame window

class CCpNavPageFrame : public CWnd
{
// Construction
public:
	CCpNavPageFrame();

// Attributes
public:

// Operations
public:
	BOOL Create(CRect& rect, CWnd* pParentWnd, UINT nID);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCpNavPageFrame)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCpNavPageFrame();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCpNavPageFrame)
	virtual LRESULT OnNewURL(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
